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


#ifndef STATE_H
#define STATE_H


#include <string>

/**
 * An abstract state object.
 */
class State {
public:
	/**
	 * The default constructor of the State object.
	 */
	State();

	/**
	 * The copy constructor of the State object.
	 * @param	other	The state to copy.
	 */
	State(const State &other);

	/**
	 * The default deconstructor of the State object.
	 */
	virtual ~State();

	/**
	 * Overload the equals operator to set this state equal to the state provided.
	 * @param	other	The state to copy.
	 * @return	The new version of this state.
	 */
	virtual State &operator=(const State &other);

	/**
	 * Overload the equality comparison operator.
	 * @param	other	The state to compare.
	 * @return	Returns @code{true} if this state is equal to the other; @code{false} otherwise.
	 */
	virtual bool operator==(const State &other) const;

	/**
	 * Overload the less than operator for comparison.
	 * @param	other	The state to compare.
	 * @return	Returns @code{true} if this state is less than the other; @code{false} otherwise.
	 */
	virtual bool operator<(const State &other) const;

	/**
	 * Returns a string representation of this state.
	 * @return	Returns the string representing this state.
	 */
	virtual std::string to_string() const = 0;

	/**
	 * Returns a hash value used to quickly identify this state in a collection of states.
	 * @return	Returns the hash value of this state.
	 */
	virtual unsigned int hash_value() const = 0;

};


#endif // STATE_H
