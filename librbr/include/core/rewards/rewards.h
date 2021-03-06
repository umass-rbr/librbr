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


#ifndef REWARDS_H
#define REWARDS_H


/**
 * An abstract class which defines how to interact with a rewards object.
 * This general structure should work for any implementation of rewards,
 * such as a full table lookup, a partial table lookup with 'wildcard' capability,
 * a function, etc. This should be able to map any state, state-action pair, or
 * state-action-state triple to a real value, or a distribution over possible next
 * states for continuous states and/or actions.
 *
 * This high level class allows for a single definition of each MDP-like object, instead
 * of one for every kind of combination of actions, states, etc.
 */
class Rewards {
public:
	/**
	 * The deconstructor for the Rewards class, which ensures that children
	 * classes properly deconstruct themselves.
	 */
	virtual ~Rewards();

};


#endif // REWARDS_H
