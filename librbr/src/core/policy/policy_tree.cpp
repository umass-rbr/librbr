/**
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2014 Kyle Hollins Wray, University of Massachusetts
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of
 *  this software and associated documentation files (the "Software"), to deal in
 *  the Software without restriction, including without limitation the rights to
 *  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 *  the Software, and to permit persons to whom the Software is furnished to do so,
 *  subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 *  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 *  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 *  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 *  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#include <iostream>

#include "../../../include/core/policy/policy_tree.h"
#include "../../../include/core/policy/policy_exception.h"

#include "../../../include/core/actions/action_exception.h"
#include "../../../include/core/observations/observation_exception.h"

#include "../../../include/utilities/log.h"
#include "../../../include/utilities/string_manipulation.h"

#include "../../../include/core/actions/action_utilities.h"
#include "../../../include/core/observations/observation_utilities.h"

PolicyTreeNode::PolicyTreeNode()
{
	action = nullptr;
}

PolicyTreeNode::PolicyTreeNode(Action *a)
{
	action = a;
}

PolicyTree::PolicyTree()
{
	nodes.push_back(new PolicyTreeNode());
	root = nullptr;
	current = nullptr;
}

PolicyTree::PolicyTree(ObservationsMap *observations, unsigned int horizon)
{
	root = generate_tree(observations, horizon);
	current = root;
}

PolicyTree::PolicyTree(ObservationsMap *observations, Horizon *horizon)
{
	if (horizon->is_finite()) {
		root = generate_tree(observations, horizon->get_horizon());
	} else {
		root = nullptr;
	}
	current = root;
}

PolicyTree::~PolicyTree()
{
	reset();
}

void PolicyTree::set(const std::vector<Observation *> &history, Action *action)
{
	PolicyTreeNode *node = nullptr;

	try {
		node = traverse(history);
	} catch (const PolicyException &err) {
		throw PolicyException();
	}

	if (node == nullptr) {
		throw PolicyException();
	}

	node->action = action;
}

Action *PolicyTree::get(const std::vector<Observation *> &history)
{
	PolicyTreeNode *node = nullptr;

	try {
		node = traverse(history);
	} catch (const PolicyException &err) {
		throw PolicyException();
	}

	if (node == nullptr) {
		throw PolicyException();
	}

	return node->action;
}

bool PolicyTree::load(std::string filename, ActionsMap *actions, ObservationsMap *observations, Horizon *horizon)
{
	// Reset the current tree, cleaning the memory, and then generate the tree's data.
	reset();

	if (horizon->is_finite()) {
		root = generate_tree(observations, horizon->get_horizon());
	} else {
		root = nullptr;
	}

	char error[1024];

	// Load the file and return if it failed to open.
	std::ifstream file(filename);
	if (!file.is_open()) {
		sprintf(error, "Failed to open file '%s'.", filename.c_str());
		log_message("PolicyTree::load", error);
		return true;
	}

	int rows = 1;
	std::string line;

	// Read each line of the file, setting each node's action according
	// to the history provided in the file.
	while (std::getline(file, line)) {
		// Trim whitespace and skip over blank lines.
		trim_whitespace(line);
		if (line.length() == 0) {
			continue;
		}

		std::vector<std::string> items = split_string_by_colon(line);

		std::vector<Observation *> history;
		Action *action = nullptr;

		unsigned int counter = 1;

		// Create the history of observations and the action to take.
		for (std::string item : items) {
			if (counter == items.size()) {
				// If this is the last observation, then load the action.
				try {
					action = find_action(actions, item);
				} catch (const ActionException &err) {
					sprintf(error, "Action '%s' was undefined on line %i in file '%s'.",
							item.c_str(), rows, filename.c_str());
					log_message("PolicyTree::load", error);
					return true;
				}
			} else {
				// Otherwise, load the observation.
				try {
					history.push_back(find_observation(observations, item));
				} catch (const ObservationException &err) {
					sprintf(error, "Observation '%s' was undefined on line %i in file '%s'.",
							item.c_str(), rows, filename.c_str());
					log_message("PolicyTree::load", error);
					return true;
				}
			}

			counter++;
		}

		// Now that the history of observations and the action are defined, attempt to set it in the tree.
		try {
			set(history, action);
		} catch (const PolicyException &err) {
			sprintf(error, "Failed to set action '%s' given the history on line %i in file '%s'.",
					action->to_string().c_str(), rows, filename.c_str());
			log_message("PolicyTree::load", error);
			return true;
		}

		rows++;
	}

	return false;
}

bool PolicyTree::save(std::string filename)
{
	char error[1024];

	std::ofstream file(filename);
	if (!file.is_open()) {
		sprintf(error, "Failed to open the file '%s' for saving.", filename.c_str());
		log_message("PolicyTree::save", error);
		return true;
	}

	std::vector<Observation *> history;

	save_tree(file, root, history);

	file.close();

	return false;
}

Action *PolicyTree::next(Observation *observation)
{
	Action *action = current->action;
	current = current->next[observation];
	return action;
}

void PolicyTree::reset()
{
	for (PolicyTreeNode *node : nodes) {
		delete node;
	}
	nodes.clear();
	root = nullptr;
	current = nullptr;
}

PolicyTreeNode *PolicyTree::generate_tree(ObservationsMap *observations, unsigned int horizon)
{
	// Create the new node regardless.
	PolicyTreeNode *node = new PolicyTreeNode();
	nodes.push_back(node);

	// Return immediately if this is a leaf node.
	if (horizon == 1) {
		return node;
	}

	// Otherwise, just generate the subtree normally and return.
	for (auto z : *observations) {
		node->next[resolve(z)] = generate_tree(observations, horizon - 1);
	}

	return node;
}

PolicyTreeNode *PolicyTree::traverse(const std::vector<Observation *> &history)
{
	// Traverse the policy tree, following the history path, until the node is reached.
	PolicyTreeNode *node = root;

	for (Observation *o : history) {
		std::map<Observation *, PolicyTreeNode *>::iterator result = node->next.find(o);

		if (result == node->next.end()) {
			throw PolicyException();
		}

		node = result->second;
	}

	return node;
}

void PolicyTree::save_tree(std::ofstream &file, PolicyTreeNode *node, std::vector<Observation *> history)
{
	if (node == nullptr) {
		return;
	}

	// Save the history of observations and the corresponding action.
//	file << history.size() << " : ";
	for (Observation *o : history) {
		file << o->to_string() << " : ";
	}
	if (node->action != nullptr) {
		file << node->action->to_string();
	}
	file << std::endl;

	// Recursively explore the children of this node.
	for (std::map<Observation *, PolicyTreeNode *>::value_type child : root->next) {
		// Create a new history vector with the observation traversed to reach this child included.
		std::vector<Observation *> newHistory = history;
		newHistory.push_back(child.first);

		// Save the child using this new history.
		save_tree(file, child.second, newHistory);
	}
}
