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


#ifndef JOINT_ACTIONS_MAP_H
#define JOINT_ACTIONS_MAP_H


#include <vector>

#include "action.h"
#include "actions.h"
#include "actions_map.h"

/**
 * A class for finite sets of joint actions in an MDP-like object. Informally, there are two basic ways to
 * store finite actions: a vector of actions or a generator function based on a state and action. In both
 * cases, we require that any class with finite actions provide certain get functions so that any generic
 * solver can handle both cases.
 *
 * If you want to create a generator function-based FiniteJointActions class, please create a child class which
 * implements the function in the virtual functions described below. You will likely ignore the internal
 * states vector variable here.
 *
 * Note: The protected "states" variable implicitly must only store JointState objects, not State objects.
 */
class JointActionsMap : virtual public ActionsMap {
public:
	/**
	 * The default constructor for the FiniteJointActions class which requires that you
	 * specify the number of factors.
	 * @param	numFactors	The number of action factors, minimum of 1.
	 */
	JointActionsMap(unsigned int numFactors);

	/**
	 * The default deconstructor for the FiniteJointActions class.
	 */
	virtual ~JointActionsMap();

	/**
	 * Add a action to the set of available actions in a factor. This does *not* update the
	 * actions list; please call update() once all factors have been set.
	 * @param	factorIndex 		The index of the factor to add the actions to.
	 * @param	newAction			The new actions to include in the set of available actions.
	 * @throw	ActionException		The index was invalid.
	 */
	void add(unsigned int factorIndex, Action *newAction);

	/**
	 * Remove an action to the set of available actions in a factor. This frees the memory. This does *not*
	 * update the actions list; please call update() once all factors have been set.
	 * @param	factorIndex 		The index of the factor to add the actions to.
	 * @param	removeAction 		The action to remove from the set of available actions.
	 * @throw	ActionException		The index was invalid, or the action was not found in the actions list.
	 */
	void remove(unsigned int factorIndex, Action *removeAction);

	/**
	 * Set the internal actions list for a factor given another list, performing a deep copy. This resets
	 * the current list of actions and frees the memory. This does *not* update the states list; please
	 * call update() once all factors have been set.
	 * @param	factorIndex 		The index of the factor to add the actions to.
	 * @param	newActions 			The vector of new actions to use.
	 * @throw	ActionException		The index was invalid, or newActions was empty.
	 */
	void set(unsigned int factorIndex, const std::vector<Action *> &newActions);

	/**
	 * Get the action at the corresponding index, given the particular factor. The factor index
	 * is defined by the agent, and an action's index is defined by the order in which they are
	 * added and removed.
	 * @param	factorIndex 		The index of the factor.
	 * @param	actionIndex 		The index of the action.
	 * @throw	ActionException		The index was invalid.
	 * @return	The action at the corresponding index.
	 */
	Action *get(unsigned int factorIndex, unsigned int actionIndex);

	/**
	 * Update the internal actions list which holds all permutations of joint actions in an efficient structure.
	 * Note: This *must* be called after sequences of add(), remove(), and/or set() calls.
	 * @throw	ActionException		If a action factor has not been defined.
	 */
	void update();

	/**
	 * Get the number of factored actions.
	 * @return	The number of factored actions.
	 */
	unsigned int get_num_factors();

	/**
	 * Reset the joint actions, clearing the internal list and freeing the memory.
	 */
	virtual void reset();

protected:
	/**
	 * The list of all available actions for each action factor.
	 */
	std::vector<std::vector<Action *> > factoredActions;

private:
	/**
	 * A helper function for updating the internal "actions" variable as part of the update() function.
	 * @param	currentJointAction		The current (incomplete) joint action as a vector of actions.
	 * @param	currentFactorIndex		The current factor index.
	 */
	void update_step(std::vector<Action *> currentJointAction, unsigned int currentFactorIndex);

};


#endif // JOINT_ACTIONS_MAP_H
