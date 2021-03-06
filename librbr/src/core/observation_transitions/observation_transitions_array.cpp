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


#include "../../../include/core/observation_transitions/observation_transitions_array.h"
#include "../../../include/core/observation_transitions/observation_transition_exception.h"

#include <algorithm>

ObservationTransitionsArray::ObservationTransitionsArray(unsigned int numStates,
		unsigned int numActions, unsigned int numObservations)
{
	states = numStates;
	if (states == 0) {
		states = 1;
	}

	actions = numActions;
	if (actions == 0) {
		actions = 1;
	}

	observations = numObservations;
	if (observations == 0) {
		observations = 1;
	}

	observationTransitions = new float[actions * states * observations];
	availableObservations = new std::vector<Observation *>[actions * states];

	reset();
}

ObservationTransitionsArray::~ObservationTransitionsArray()
{
	delete [] observationTransitions;
	delete [] availableObservations;
}

void ObservationTransitionsArray::set(Action *previousAction, State *state,
		Observation *observation, double probability)
{
	IndexedAction *a = dynamic_cast<IndexedAction *>(previousAction);
	IndexedState *s = dynamic_cast<IndexedState *>(state);
	IndexedObservation *z = dynamic_cast<IndexedObservation *>(observation);

	if (a == nullptr || s == nullptr || z == nullptr) {
		throw ObservationTransitionException();
	}

	if (a->get_index() >= actions || s->get_index() >= states || z->get_index() >= observations) {
		throw ObservationTransitionException();
	}

	observationTransitions[a->get_index() * states * observations +
	                       s->get_index() * observations +
	                       z->get_index()] =
			(float)std::max(0.0, std::min(1.0, probability));
}

double ObservationTransitionsArray::get(Action *previousAction, State *state,
		Observation *observation)
{
	IndexedAction *a = dynamic_cast<IndexedAction *>(previousAction);
	IndexedState *s = dynamic_cast<IndexedState *>(state);
	IndexedObservation *z = dynamic_cast<IndexedObservation *>(observation);

	if (a == nullptr || s == nullptr || z == nullptr) {
		throw ObservationTransitionException();
	}

	if (a->get_index() >= actions || s->get_index() >= states || z->get_index() >= observations) {
		throw ObservationTransitionException();
	}

	return observationTransitions[a->get_index() * states * observations +
	   	                       s->get_index() * observations +
	   	                       z->get_index()];
}

void ObservationTransitionsArray::add_available(Action *previousAction, State *state, Observation *availableObservation)
{
	IndexedAction *a = dynamic_cast<IndexedAction *>(previousAction);
	IndexedState *s = dynamic_cast<IndexedState *>(state);

	if (a == nullptr || s == nullptr) {
		throw ObservationTransitionException();
	}

	if (a->get_index() >= actions || s->get_index() >= states) {
		throw ObservationTransitionException();
	}

	availableObservations[a->get_index() * states + s->get_index()].push_back(availableObservation);
}

const std::vector<Observation *> &ObservationTransitionsArray::available(Observations *Z, Action *previousAction, State *state)
{
	IndexedAction *a = dynamic_cast<IndexedAction *>(previousAction);
	IndexedState *sp = dynamic_cast<IndexedState *>(state);

	if (a == nullptr || sp == nullptr) {
		throw ObservationTransitionException();
	}

	if (a->get_index() >= actions || sp->get_index() >= states) {
		throw ObservationTransitionException();
	}

	if (availableObservations[a->get_index() * states + sp->get_index()].size() == 0) {
		ObservationsMap *Zmap = dynamic_cast<ObservationsMap *>(Z);
		if (Zmap == nullptr) {
			throw ObservationTransitionException();
		}

		compute_available(Zmap, a, sp);
	}

	return availableObservations[a->get_index() * states + sp->get_index()];
}

void ObservationTransitionsArray::set_observation_transitions(const float *O)
{
	for (unsigned int a = 0; a < actions; a++) {
		for (unsigned int sp = 0; sp < states; sp++) {
			for (unsigned int z = 0; z < observations; z++) {
				observationTransitions[a * states * observations + sp * observations + z] =
						O[a * states * observations + sp * observations + z];
			}
		}
	}
}

float *ObservationTransitionsArray::get_observation_transitions()
{
	return observationTransitions;
}

unsigned int ObservationTransitionsArray::get_num_states() const
{
	return states;
}

unsigned int ObservationTransitionsArray::get_num_actions() const
{
	return actions;
}

unsigned int ObservationTransitionsArray::get_num_observations() const
{
	return observations;
}

void ObservationTransitionsArray::reset()
{
	for (unsigned int a = 0; a < actions; a++) {
		for (unsigned int sp = 0; sp < states; sp++) {
			for (unsigned int z = 0; z < observations; z++) {
				observationTransitions[a * states * observations +
				                       sp * observations +
				                       z] = 0.0f;
			}

			availableObservations[a * states + sp].clear();
		}
	}
}

void ObservationTransitionsArray::compute_available(ObservationsMap *Z, IndexedAction *a, IndexedState *sp)
{
	availableObservations[a->get_index() * states + sp->get_index()].clear();

	for (unsigned int z = 0; z < observations; z++) {
		if (observationTransitions[a->get_index() * states * observations + sp->get_index() * observations + z] > 0.0f) {
			availableObservations[a->get_index() * states + sp->get_index()].push_back(Z->get(z));
		}
	}
}
