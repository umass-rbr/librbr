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


#include "../../../include/core/rewards/saso_rewards_array.h"
#include "../../../include/core/rewards/reward_exception.h"

#include "../../../include/core/states/indexed_state.h"
#include "../../../include/core/actions/indexed_action.h"
#include "../../../include/core/observations/indexed_observation.h"

#include <limits>

SASORewardsArray::SASORewardsArray(unsigned int numStates, unsigned int numActions, unsigned int numObservations)
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

	rewards = new float[states * actions * states * observations];

	reset();
}

SASORewardsArray::~SASORewardsArray()
{
	delete [] rewards;
}

void SASORewardsArray::set(State *state, Action *action, State *nextState,
		Observation *observation, double reward)
{
	IndexedState *s = dynamic_cast<IndexedState *>(state);
	IndexedAction *a = dynamic_cast<IndexedAction *>(action);
	IndexedState *sp = dynamic_cast<IndexedState *>(nextState);
	IndexedObservation *z = dynamic_cast<IndexedObservation *>(observation);

	if (s == nullptr || a == nullptr || sp == nullptr || z == nullptr) {
		throw RewardException();
	}

	if (s->get_index() >= states || a->get_index() >= actions ||
			sp->get_index() >= states || z->get_index() >= observations) {
		throw RewardException();
	}

	rewards[s->get_index() * actions * states * observations +
	        a->get_index() * states * observations +
	        sp->get_index() * observations +
			z->get_index()] = (float)reward;

	if (Rmin > reward) {
		Rmin = reward;
	}
	if (Rmax < reward) {
		Rmax = reward;
	}
}

double SASORewardsArray::get(State *state, Action *action, State *nextState,
		Observation *observation)
{
	IndexedState *s = dynamic_cast<IndexedState *>(state);
	IndexedAction *a = dynamic_cast<IndexedAction *>(action);
	IndexedState *sp = dynamic_cast<IndexedState *>(nextState);
	IndexedObservation *z = dynamic_cast<IndexedObservation *>(observation);

	if (s == nullptr || a == nullptr || sp == nullptr || z == nullptr) {
		throw RewardException();
	}

	if (s->get_index() >= states || a->get_index() >= actions ||
			sp->get_index() >= states || z->get_index() >= observations) {
		throw RewardException();
	}

	return rewards[s->get_index() * actions * states * observations +
	               a->get_index() * states * observations +
	               sp->get_index() * observations +
	               z->get_index()];
}

void SASORewardsArray::reset()
{
	for (unsigned int s = 0; s < states; s++) {
		for (unsigned int a = 0; a < actions; a++) {
			for (unsigned int sp = 0; sp < states; sp++) {
				for (unsigned int z = 0; z < observations; z++) {
					rewards[s * actions * states * observations +
					        a * states * observations +
					        sp * observations +
					        z] = 0.0f;
				}
			}
		}
	}

	Rmin = std::numeric_limits<double>::lowest() * -1.0;
	Rmax = std::numeric_limits<double>::lowest();
}

void SASORewardsArray::set_rewards(const float *R)
{
	for (unsigned int s = 0; s < states; s++) {
		for (unsigned int a = 0; a < actions; a++) {
			for (unsigned int sp = 0; sp < states; sp++) {
				for (unsigned int z = 0; z < observations; z++) {
					rewards[s * actions * states * observations + a * states * observations + sp * observations + z] =
							R[s * actions * states * observations + a * states * observations + sp * observations + z];

					if (Rmin > R[s * actions * states * observations + a * states * observations + sp * observations + z]) {
						Rmin = R[s * actions * states * observations + a * states * observations + sp * observations + z];
					}
					if (Rmax < R[s * actions * states * observations + a * states * observations + sp * observations + z]) {
						Rmax = R[s * actions * states * observations + a * states * observations + sp * observations + z];
					}
				}
			}
		}
	}
}

float *SASORewardsArray::get_rewards()
{
	return rewards;
}

unsigned int SASORewardsArray::get_num_states() const
{
	return states;
}

unsigned int SASORewardsArray::get_num_actions() const
{
	return actions;
}

unsigned int SASORewardsArray::get_num_observations() const
{
	return observations;
}

double SASORewardsArray::get_min() const
{
	return Rmin;
}

double SASORewardsArray::get_max() const
{
	return Rmax;
}
