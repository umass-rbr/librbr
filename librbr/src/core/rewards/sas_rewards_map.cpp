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


#include "../../../include/core/rewards/sas_rewards_map.h"
#include "../../../include/core/rewards/reward_exception.h"

#include "../../../include/core/states/named_state.h"
#include "../../../include/core/actions/named_action.h"

#include <limits>

SASRewardsMap::SASRewardsMap()
{
	stateWildcard = new NamedState("*");
	actionWildcard = new NamedAction("*");

	Rmin = std::numeric_limits<double>::lowest() * -1.0;
	Rmax = std::numeric_limits<double>::lowest();
}

SASRewardsMap::~SASRewardsMap()
{
	reset();

	delete stateWildcard;
	delete actionWildcard;
}

void SASRewardsMap::set(State *state, Action *action, State *nextState, double reward)
{
	if (state == nullptr) {
		state = stateWildcard;
	}
	if (action == nullptr) {
		action = actionWildcard;
	}
	if (nextState == nullptr) {
		nextState = stateWildcard;
	}

	rewards[state][action][nextState] = reward;

	if (Rmin > reward) {
		Rmin = reward;
	}
	if (Rmax < reward) {
		Rmax = reward;
	}
}

void SASRewardsMap::set(State *state, Action *action, State *nextState,
		Observation *observation, double reward)
{
	set(state, action, nextState, reward);
}

double SASRewardsMap::get(State *state, Action *action, State *nextState)
{
	// Iterate over all possible configurations of wildcards in the get statement.
	// For each, use the get_value() function to check if the value exists. If it
	// does, perhaps using a wildcard, then return that, otherwise continue.
	// Return 0 by default.
	for (int i = 0; i < 8; i++) {
		State *alpha = stateWildcard;
		if (!(bool)(i & (1 << 0))) {
			alpha = state;
		}

		Action *beta = actionWildcard;
		if (!(bool)(i & (1 << 1))) {
			beta = action;
		}

		State *gamma = stateWildcard;
		if (!(bool)(i & (1 << 2))) {
			gamma = nextState;
		}

		try {
			return get_value(alpha, beta, gamma);
		} catch (const RewardException &err) { }
	}

	return 0.0;
}

double SASRewardsMap::get(State *state, Action *action, State *nextState,
		Observation *observation)
{
	return get(state, action, nextState);
}

void SASRewardsMap::reset()
{
	rewards.clear();
	Rmin = std::numeric_limits<double>::lowest() * -1.0;
	Rmax = std::numeric_limits<double>::lowest();
}

double SASRewardsMap::get_value(State *state, Action *action, State *nextState)
{
	std::unordered_map<State *,
		std::unordered_map<Action *,
		std::unordered_map<State *, double> > >::const_iterator alpha = rewards.find(state);
	if (alpha == rewards.end()) {
		throw RewardException();
	}

	std::unordered_map<Action *,
		std::unordered_map<State *, double> >::const_iterator beta = alpha->second.find(action);
	if (beta == alpha->second.end()) {
		throw RewardException();
	}

	std::unordered_map<State *, double>::const_iterator gamma = beta->second.find(nextState);
	if (gamma == beta->second.end()) {
		throw RewardException();
	}

	return gamma->second;
}

double SASRewardsMap::get_min() const
{
	return Rmin;
}

double SASRewardsMap::get_max() const
{
	return Rmax;
}
