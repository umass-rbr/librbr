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


#include "../../../include/core/rewards/sa_rewards_map.h"
#include "../../../include/core/rewards/reward_exception.h"

#include "../../../include/core/states/named_state.h"
#include "../../../include/core/actions/named_action.h"

#include <limits>

SARewardsMap::SARewardsMap()
{
	stateWildcard = new NamedState("*");
	actionWildcard = new NamedAction("*");

	Rmin = std::numeric_limits<double>::lowest() * -1.0;
	Rmax = std::numeric_limits<double>::lowest();
}

SARewardsMap::~SARewardsMap()
{
	reset();

	delete stateWildcard;
	delete actionWildcard;
}

void SARewardsMap::set(State *state, Action *action, double reward)
{
	if (state == nullptr) {
		state = stateWildcard;
	}
	if (action == nullptr) {
		action = actionWildcard;
	}

	rewards[state][action] = reward;

	if (Rmin > reward) {
		Rmin = reward;
	}
	if (Rmax < reward) {
		Rmax = reward;
	}
}

void SARewardsMap::set(State *state, Action *action, State *nextState, double reward)
{
	set(state, action, reward);
}

void SARewardsMap::set(State *state, Action *action, State *nextState,
		Observation *observation, double reward)
{
	set(state, action, reward);
}

double SARewardsMap::get(State *state, Action *action)
{
	// Iterate over all possible configurations of wildcards in the get statement.
	// For each, use the get_value() function to check if the value exists. If it
	// does, perhaps using a wildcard, then return that, otherwise continue.
	// Return 0 by default.
	for (int i = 0; i < 4; i++) {
		State *alpha = stateWildcard;
		if (!(bool)(i & (1 << 0))) {
			alpha = state;
		}

		Action *beta = actionWildcard;
		if (!(bool)(i & (1 << 1))) {
			beta = action;
		}

		try {
			return get_value(alpha, beta);
		} catch (const RewardException &err) { }
	}

	return 0.0;
}

double SARewardsMap::get(State *state, Action *action, State *nextState)
{
	return get(state, action);
}

double SARewardsMap::get(State *state, Action *action, State *nextState,
		Observation *observation)
{
	return get(state, action);
}

void SARewardsMap::reset()
{
	rewards.clear();
	Rmin = std::numeric_limits<double>::lowest() * -1.0;
	Rmax = std::numeric_limits<double>::lowest();
}

double SARewardsMap::get_value(State *state, Action *action)
{
	std::unordered_map<State *,
		std::unordered_map<Action *, double> >::const_iterator alpha =
			rewards.find(state);
	if (alpha == rewards.end()) {
		throw RewardException();
	}

	std::unordered_map<Action *, double>::const_iterator beta = alpha->second.find(action);
	if (beta == alpha->second.end()) {
		throw RewardException();
	}

	return beta->second;
}

double SARewardsMap::get_min() const
{
	return Rmin;
}

double SARewardsMap::get_max() const
{
	return Rmax;
}
