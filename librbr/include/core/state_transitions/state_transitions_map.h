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


#ifndef STATE_TRANSITIONS_MAP_H
#define STATE_TRANSITIONS_MAP_H


#include <unordered_map>

#include "state_transitions.h"

#include "../states/states.h"
#include "../states/states_map.h"

#include "../states/state.h"

#include "../actions/action.h"

/**
 * A class for finite state transitions in an MDP-like object. Informally, there are two basic ways to
 * store finite state transitions: a table lookup mapping state-action-state triples to real values,
 * or a generator function based on one of these triples. In both cases, we require that any class with
 * finite state transitions provide certain get functions so that any generic solver can handle both cases.
 *
 * If you want to create a generator function-based StateTransitionsMap class, please create a child
 * class which implements the function in the virtual functions described below. You will likely ignore
 * the internal stateTrantions vector variable here.
 */
class StateTransitionsMap : virtual public StateTransitions {
public:
	/**
	 * The default constructor for the StateTransitionsMap class.
	 */
	StateTransitionsMap();

	/**
	 * The default deconstructor for the StateTransitionsMap class.
	 */
	virtual ~StateTransitionsMap();

	/**
	 * Set a state transition from a particular state-action-state triple to a probability.
	 * @param	state			The current state of the system.
	 * @param	action			The action taken at the current state.
	 * @param	nextState		The next state with which we assign the probability.
	 * @param	probability		The probability of going from the state, taking the action, then
	 * 							moving to the nextState.
	 */
	virtual void set(State *state, Action *action, State *nextState, double probability);

	/**
	 * The probability of a transition following the state-action-state triple provided.
	 * @param	state		The current state of the system.
	 * @param	action		The action taken at the current state.
	 * @param	nextState	The next state with which we assign the probability.
	 * @return	The probability of going from the state, taking the action, then moving to the nextState.
	 */
	virtual double get(State *state, Action *action, State *nextState);

	/**
	 * Add a successor to a particular state.
	 * @param	state						The previous state.
	 * @param	action						The action taken at the previous state.
	 * @param	successorState				The previous state.
	 * @throw	StateTransitionException	The state-action pair could not be found.
	 */
	virtual void add_successor(State *state, Action *action, State *successorState);

	/**
	 * Return a list of the states available given a previous state and the action taken there.
	 * Note: This list may change (continual planning) if nested inside loops incorrectly. To be safe,
	 * just call it once unless you know what you are doing.
	 * @param	S							The set of states.
	 * @param	state						The previous state.
	 * @param	action						The action taken at the previous state.
	 * @throw	StateTransitionException	The state-action pair could not be found.
	 * @return	successors					A reference to the list of successor states.
	 */
	virtual const std::vector<State *> &successors(States *S, State *state, Action *action);

	/**
	 * Reset the state transitions, clearing the internal mapping.
	 */
	virtual void reset();

private:
	/**
	 * The actual get function which returns a value. This will throw an error if the value is undefined.
	 * It is used as a helper function for the public get function.
	 * @param	state						The current state of the system.
	 * @param	action						The action taken at the current state.
	 * @param	nextState					The next state with which we assign the reward.
	 * @throw	StateTransitionException	The state transition was not defined.
	 * @return	The probability of going from the state, taking the action, then moving to the nextState.
	 */
	virtual double get_value(State *state, Action *action, State *nextState);

	/**
	 * Compute the successor states for the state and action pair provided, then store the result
	 * in successorStates via the 'succ' parameter.
	 * @param	S							The finite set of states.
	 * @param	s							The current state.
	 * @param	a							The current action.
	 * @param	succ						The reference to the successors vector.
	 */
	virtual void compute_successors(StatesMap *S, State *s, Action *a, std::vector<State *> &succ);

	/**
	 * The list of all state-action-state transitions.
	 */
	std::unordered_map<State *,
		std::unordered_map<Action *,
		std::unordered_map<State *, double> > > stateTransitions;

	/**
	 * A special state (implicitly constant) referring to a state wildcard.
	 */
	State *stateWildcard;

	/**
	 * A special action (implicitly constant) referring to an action wildcard.
	 */
	Action *actionWildcard;

	/**
	 * A mapping from state-action pairs to a vector of successor states.
	 */
	std::unordered_map<State *, std::unordered_map<Action *, std::vector<State *> > > successorStates;

};


#endif // STATE_TRANSITIONS_MAP_H
