/**
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2013 Kyle Wray and Luis Pineda
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


#ifndef POLICY_GRAPH_H
#define POLICY_GRAPH_H


#include <string>
#include <vector>
#include <map>

#include "policy.h"

#include "../observations/observation.h"

/**
 * A small structure for a policy graph's node (Finite State Controller (FSC) policy states).
 * In reality, all that matters for larger problems is having a unique object to represent an
 * abstract policy state. This structure allows us to expand this state in the future, if desired.
 */
struct PolicyGraphNode { };

/**
 * A policy graph (Finite State Controller (FSC)). This FSC captures the policy tree
 * representation, too. Internally, this object is a stochastic FSC, in which
 * every FSC state maps to a distribution over actions, and a subsequent observation
 * maps to a distribution over the next FSC state (not usually world state).
 *
 * This class facilitates the iterative action selection process by first
 * selecting an internal FSC state following the given distribution, and then
 * continually following the stochastic policy mappings via the 'next' function.
 */
class PolicyGraph : public Policy {
public:
	/**
	 * The constructor for a PolicyGraph object which specifies the number of internal policy states.
	 * @param numPolicyStates The number of policy states internal to the FSC policy.
	 */
	PolicyGraph(int numPolicyStates);

	/**
	 * A virtual deconstructor to prevent errors upon the deletion of a child object.
	 */
	virtual ~PolicyGraph();

	/**
	 * A function which must load a policy file.
	 * @param filename The name and path of the file to load.
	 * @return Return @code{true} if an error occurred, @code{false} otherwise.
	 */
	virtual bool load(std::string filename);

	/**
	 * A function which must save a policy file.
	 * @param filename The name and path of the file to save.
	 * @return Return @code{true} if an error occurred, @code{false} otherwise.
	 */
	virtual bool save(std::string filename) const;

	/**
	 * A function which follows the defined policy, having the current state stored internally,
	 * and returns the action to select next.
	 */
	virtual const Action *next();

	/**
	 * Reset the policy to the initial state.
	 */
	virtual void reset();

	/**
	 * Initialize the FSCPolicy object, which resets only the execution variables
	 * such as the random initial selection of the internal FSC state following the
	 * distribution over initial states.
	 */
	void initialize();

private:
	/**
	 * The FSC states (nodes).
	 */
	std::vector<PolicyGraphNode> states;

	/**
	 * The mapping of FSC states to a distribution over actions.
	 */
	std::map<PolicyGraphNode, std::map<Action *, double> > policy;

	/**
	 * The mapping of FSC state-observation pair to a distribution over subsequent FSC states.
	 */
	std::map<PolicyGraphNode, std::map<Observation *, std::map<PolicyGraphNode, double> > > transitions;

	/**
	 * The mapping of an initial distribution over FSC states.
	 */
	std::map<PolicyGraphNode, double> initial;

	/**
	 * Used in execution, this variable denotes the current internal FSC state.
	 */
	PolicyGraphNode current;

};


#endif // POLICY_GRAPH_H