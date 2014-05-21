/**
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2014 Kyle Wray
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


#ifndef FINITE_OBSERVATION_TRANSITIONS_H
#define FINITE_OBSERVATION_TRANSITIONS_H


#include <map>

#include "observation_transitions.h"

#include "../states/state.h"

#include "../actions/action.h"

#include "../observations/observation.h"
#include "../observations/finite_observations.h"

/**
 * A class for finite observation transitions in an MDP-like object. Informally, there are two basic ways to
 * store finite observation transitions: a table lookup mapping observation-action-state triples to real values,
 * or a generator function based on one of these triples. In both cases, we require that any class with
 * finite observation transitions provide certain get functions so that any generic solver can handle both cases.
 *
 * If you want to create a generator function-based FiniteObservationTransitions class, please create a child
 * class which implements the function in the virtual functions described below. You will likely ignore
 * the internal observationTrantions vector variable here.
 */
class FiniteObservationTransitions : public ObservationTransitions {
public:
	/**
	 * The default constructor for the FiniteObservationTransitions class.
	 */
	FiniteObservationTransitions();

	/**
	 * The default deconstructor for the FiniteObservationTransitions class.
	 */
	virtual ~FiniteObservationTransitions();

	/**
	 * Set a observation transition from a particular observation-action-state triple to a probability.
	 * @param previousAction	The action taken at the previous state which resulted in the current state.
	 * @param state				The current state.
	 * @param observation		The next observation to which we assign a probability.
	 * @param probability		The probability of the observation given we took the action and landed in the state given.
	 */
	void set(const Action *previousAction, const State *state, const Observation *observation, double probability);

	/**
	 * The probability of a transition following the observation-action-state triple provided.
	 * @param previousAction	The action taken at the previous state which resulted in the current state.
	 * @param state				The current state.
	 * @param observation		The next observation to which we assign a probability.
	 * @return The probability of the observation given we took the action and landed in the state given.
	 */
	virtual double get(const Action *previousAction, const State *state, const Observation *observation) const;

	/**
	 * Return a list of the observations available given a previous state and the action taken there.
	 * @param Z			A finite set of observations.
	 * @param action	The action taken at the previous state.
	 * @param state 	The next state after taking the action in the previous state.
	 * @param result	The list to overwrite and set to be the list of successor states.
	 */
	void available(const FiniteObservations *Z, const Action *action, const State *nextState,
			std::vector<const Observation *> &result) const;

	/**
	 * Reset the observation transitions, clearing the internal mapping.
	 */
	virtual void reset();

private:
	/**
	 * The actual get function which returns a value. This will throw an error if the value is undefined.
	 * It is used as a helper function for the public get function.
	 * @param previousAction	The action taken at the previous state which resulted in the current state.
	 * @param state				The current state.
	 * @param observation		The next observation to which we assign a probability.
	 * @return The probability of the observation given we took the action and landed in the state given.
	 * @throws ObservationTransitionException The observation transition was not defined.
	 */
	virtual double get_value(const Action *previousAction, const State *state, const Observation *observation) const;

	/**
	 * The list of all state-action-state transitions.
	 */
	std::map<const Action *, std::map<const State *, std::map<const Observation *, double> > > observationTransitions;

	/**
	 * A special action (implicitly constant) referring to an action wildcard.
	 */
	Action *actionWildcard;

	/**
	 * A special state (implicitly constant) referring to a state wildcard.
	 */
	State *stateWildcard;

	/**
	 * A special observation (implicitly constant) referring to an observation wildcard.
	 */
	Observation *observationWildcard;

};


#endif // FINITE_OBSERVATION_TRANSITIONS_H
