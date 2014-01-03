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


#ifndef FINITE_STATE_TRANSITIONS_H
#define FINITE_STATE_TRANSITIONS_H


#include <map>
#include <tuple>

#include "state_transitions.h"

#include "../actions/action.h"
#include "../states/state.h"


/**
 * A class for finite state transitions in an MDP-like object. Informally, there are two basic ways to
 * store finite state transitions: a table lookup mapping state-action-state triples to real values,
 * or a generator function based on one of these triples. In both cases, we require that any class with
 * finite state transitions provide certain get functions so that any generic solver can handle both cases.
 *
 * If you want to create a generator function-based FiniteStateTransitions class, please create a child
 * class which implements the function in the virtual functions described below. You will likely ignore
 * the internal stateTrantions vector variable here.
 */
class FiniteStateTransitions : public StateTransitions {
public:
	/**
	 * The default constructor for the FiniteStateTransitions class.
	 */
	FiniteStateTransitions();

	/**
	 * The default deconstructor for the FiniteStateTransitions class.
	 */
	virtual ~FiniteStateTransitions();

	/**
	 * Set a state transition from a particular state-action-state triple to a probability.
	 * @param state			The current state of the system.
	 * @param action		The action taken at the current state.
	 * @param nextState		The next state with which we assign the probability.
	 * @param probability	The probability of going from the state, taking the action, then
	 * 						moving to the nextState.
	 */
	void set_state_transition(State state, Action action, State nextState, double probability);

	/**
	 * The probability of a transition following the state-action-state triple provided.
	 * @param state		The current state of the system.
	 * @param action	The action taken at the current state.
	 * @param nextState	The next state with which we assign the probability.
	 * @return The probability of going from the state, taking the action, then moving to the nextState.
	 */
	virtual double get_state_transition(State state, Action action, State nextState) const;

private:
	/**
	 * The list of all state-action-state transitions.
	 */
	std::map<std::tuple<State, Action, State>, double> stateTransitions;

};

#endif // FINITE_STATE_TRANSITIONS_H
