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


#include "../../../include/core/state_transitions/state_transitions_array.h"
#include "../../../include/core/state_transitions/state_transition_exception.h"

#include <algorithm>

StateTransitionsArray::StateTransitionsArray(unsigned int numStates, unsigned int numActions)
{
	states = numStates;
	if (states == 0) {
		states = 1;
	}

	actions = numActions;
	if (actions == 0) {
		actions = 1;
	}

	stateTransitions = new float[states * actions * states];
	successorStates = new std::vector<State *>[states * actions];

	reset();
}

StateTransitionsArray::~StateTransitionsArray()
{
	delete [] stateTransitions;
	delete [] successorStates;
}

void StateTransitionsArray::set(State *state, Action *action, State *nextState, double probability)
{
	IndexedState *s = dynamic_cast<IndexedState *>(state);
	IndexedAction *a = dynamic_cast<IndexedAction *>(action);
	IndexedState *sp = dynamic_cast<IndexedState *>(nextState);

	if (s == nullptr || a == nullptr || sp == nullptr) {
		throw StateTransitionException();
	}

	if (s->get_index() >= states || a->get_index() >= actions || sp->get_index() >= states) {
		throw StateTransitionException();
	}

	stateTransitions[s->get_index() * actions * states +
	                 a->get_index() * states +
	                 sp->get_index()] = (float)std::max(0.0, std::min(1.0, probability));
}

double StateTransitionsArray::get(State *state, Action *action, State *nextState)
{
	IndexedState *s = dynamic_cast<IndexedState *>(state);
	IndexedAction *a = dynamic_cast<IndexedAction *>(action);
	IndexedState *sp = dynamic_cast<IndexedState *>(nextState);

	if (s == nullptr || a == nullptr || sp == nullptr) {
		throw StateTransitionException();
	}

	if (s->get_index() >= states || a->get_index() >= actions || sp->get_index() >= states) {
		throw StateTransitionException();
	}

	return stateTransitions[s->get_index() * actions * states +
	                        a->get_index() * states +
	                        sp->get_index()];
}

void StateTransitionsArray::add_successor(State *state, Action *action, State *successorState)
{
	IndexedState *s = dynamic_cast<IndexedState *>(state);
	IndexedAction *a = dynamic_cast<IndexedAction *>(action);

	if (s == nullptr || a == nullptr) {
		throw StateTransitionException();
	}

	if (s->get_index() >= states || a->get_index() >= actions) {
		throw StateTransitionException();
	}

	successorStates[s->get_index() * actions + a->get_index()].push_back(successorState);
}

const std::vector<State *> &StateTransitionsArray::successors(States *S, State *state, Action *action)
{
	IndexedState *s = dynamic_cast<IndexedState *>(state);
	IndexedAction *a = dynamic_cast<IndexedAction *>(action);

	if (s == nullptr || a == nullptr) {
		throw StateTransitionException();
	}

	if (s->get_index() >= states || a->get_index() >= actions) {
		throw StateTransitionException();
	}

	if (successorStates[s->get_index() * actions + a->get_index()].size() == 0) {
		StatesMap *Smap = dynamic_cast<StatesMap *>(S);
		if (Smap == nullptr) {
			throw StateTransitionException();
		}

		compute_successors(Smap, s, a);
	}

	return successorStates[s->get_index() * actions + a->get_index()];
}

void StateTransitionsArray::set_state_transitions(const float *T)
{
	for (unsigned int s = 0; s < states; s++) {
		for (unsigned int a = 0; a < actions; a++) {
			for (unsigned int sp = 0; sp < states; sp++) {
				stateTransitions[s * actions * states + a * states + sp] =
						T[s * actions * states + a * states + sp];
			}
		}
	}
}

float *StateTransitionsArray::get_state_transitions()
{
	return stateTransitions;
}

unsigned int StateTransitionsArray::get_num_states() const
{
	return states;
}

unsigned int StateTransitionsArray::get_num_actions() const
{
	return actions;
}

void StateTransitionsArray::reset()
{
	for (unsigned int s = 0; s < states; s++) {
		for (unsigned int a = 0; a < actions; a++) {
			for (unsigned int sp = 0; sp < states; sp++) {
				stateTransitions[s * actions * states +
				                 a * states +
				                 sp] = 0.0f;
			}

			successorStates[s * actions + a].clear();
		}
	}
}

void StateTransitionsArray::compute_successors(StatesMap *S, IndexedState *s, IndexedAction *a)
{
	successorStates[s->get_index() * actions + a->get_index()].clear();

	for (unsigned int sp = 0; sp < states; sp++) {
		if (stateTransitions[s->get_index() * actions * states + a->get_index() * states + sp] > 0.0f) {
			successorStates[s->get_index() * actions + a->get_index()].push_back(S->get(sp));
		}
	}
}
