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


#ifndef FINITE_ACTIONS_H
#define FINITE_ACTIONS_H


#include <vector>

#include "action.h"
#include "actions.h"


//#include "../states/state.h"
class State;


/**
 * A class for finite sets of actions in an MDP-like object. Informally, there are two basic ways to
 * store finite actions: a vector of actions or a generator function based on a state. In both cases,
 * we require that any class with finite actions provide certain get functions so that any generic
 * solver can handle both cases.
 *
 * If you want to create a generator function-based FiniteActions class, please create a child class
 * which implements the function in the virtual functions described below. You will likely ignore the
 * internal actions vector variable here.
 */
class FiniteActions : public Actions {
public:
	/**
	 * The default constructor for the FiniteActions class.
	 */
	FiniteActions();

	/**
	 * The default deconstructor for the FiniteActions class.
	 */
	virtual ~FiniteActions();

	/**
	 * Add an action to the set of available actions.
	 * @param newAction The new action to include in the set of available actions.
	 */
	void add_action(Action newAction);

	/**
	 * Remove an action to the set of available actions.
	 * @param removeAction The action to remove from the set of available actions.
	 */
	void remove_action(Action removeAction);

	/**
	 * Set the internal actions list given another list, performing a deep copy.
	 * @param newActions The vector of new actions to use.
	 */
	void set_actions(std::vector<Action> newActions);

	/**
	 * Return a list of all the available actions.
	 * @return Return a list of available actions.
	 */
	virtual std::vector<Action> get_actions() const;

	/**
	 * Return a list of the actions available given a state.
	 * @param state The current state.
	 * @return Return a list of available actions.
	 */
	virtual std::vector<Action> get_actions(State state) const;

private:
	/**
	 * The list of all available actions.
	 */
	std::vector<Action> actions;

};

#endif // FINITE_ACTIONS_H
