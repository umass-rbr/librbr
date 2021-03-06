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


#include "../../include/perform_tests.h"

#include <iostream>

#include "../../../librbr/include/core/observation_transitions/observation_transitions_map.h"
#include "../../../librbr/include/core/observation_transitions/observation_transition_exception.h"

#include "../../../librbr/include/core/states/named_state.h"
#include "../../../librbr/include/core/actions/named_action.h"
#include "../../../librbr/include/core/observations/named_observation.h"

#include "../../../librbr/include/core/observations/observations_map.h"

int test_observation_transitions()
{
	int numSuccesses = 0;

	NamedObservation *o1 = new NamedObservation("o1");
	NamedObservation *o2 = new NamedObservation("o2");

	State *s1 = new NamedState("s1");
	State *s2 = new NamedState("s2");

	Action *a1 = new NamedAction("a1");
	Action *a2 = new NamedAction("a2");

	ObservationTransitionsMap *finiteObservationTransitions = new ObservationTransitionsMap();

	std::cout << "ObservationTransitions: Test 'FiniteObservationTransitions::set' and ";
	std::cout << "'FiniteObservationTransitions::get' (All Values Mapped)... ";

	finiteObservationTransitions->set(a1, s1, o1, 0.1);
	finiteObservationTransitions->set(a1, s2, o1, 0.2);
	finiteObservationTransitions->set(a2, s1, o1, 0.3);
	finiteObservationTransitions->set(a2, s2, o1, 0.4);
	finiteObservationTransitions->set(a1, s1, o2, 0.5);
	finiteObservationTransitions->set(a1, s2, o2, 0.6);
	finiteObservationTransitions->set(a2, s1, o2, 0.7);
	finiteObservationTransitions->set(a2, s2, o2, 0.8);

	if (finiteObservationTransitions->get(a1, s1, o1) == 0.1 && finiteObservationTransitions->get(a1, s2, o1) == 0.2 &&
			finiteObservationTransitions->get(a2, s1, o1) == 0.3 && finiteObservationTransitions->get(a2, s2, o1) == 0.4 &&
			finiteObservationTransitions->get(a1, s1, o2) == 0.5 && finiteObservationTransitions->get(a1, s2, o2) == 0.6 &&
			finiteObservationTransitions->get(a2, s1, o2) == 0.7 && finiteObservationTransitions->get(a2, s2, o2) == 0.8) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete finiteObservationTransitions;
	finiteObservationTransitions = new ObservationTransitionsMap();

	std::cout << "ObservationTransitions: Test 'FiniteObservationTransitions::set' and ";
	std::cout << "'FiniteObservationTransitions::get' (Default Zero Values)... ";

//	finiteObservationTransitions->set(a1, s1, o1, 0.1); // Leave undefined to test the default zero value for the next state s1.
	finiteObservationTransitions->set(a1, s2, o1, 0.2);
//	finiteObservationTransitions->set(a2, s1, o1, 0.3);
//	finiteObservationTransitions->set(a2, s2, o1, 0.4); // Leave undefined to test the default zero value for the action a2.
//	finiteObservationTransitions->set(a1, s1, o2, 0.5); // Leave undefined to test the default zero value for the observation o2.
//	finiteObservationTransitions->set(a1, s2, o2, 0.6); // Leave undefined to test the default zero value for the observation o2.
//	finiteObservationTransitions->set(a2, s1, o2, 0.7);	// Leave undefined to test the default zero value for the observation o2.
//	finiteObservationTransitions->set(a2, s2, o2, 0.8);	// Leave undefined to test the default zero value for the observation o2.

	if (finiteObservationTransitions->get(a1, s1, o1) == 0.0 && finiteObservationTransitions->get(a1, s2, o1) == 0.2 &&
			finiteObservationTransitions->get(a1, s1, o2) == 0.0 && finiteObservationTransitions->get(a1, s2, o2) == 0.0 &&
			finiteObservationTransitions->get(a2, s1, o1) == 0.0 && finiteObservationTransitions->get(a2, s2, o1) == 0.0 &&
			finiteObservationTransitions->get(a2, s1, o2) == 0.0 && finiteObservationTransitions->get(a2, s2, o2) == 0.0) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete finiteObservationTransitions;
	finiteObservationTransitions = new ObservationTransitionsMap();

	std::cout << "ObservationTransitions: Test 'FiniteObservationTransitions::set' and ";
	std::cout << "'FiniteObservationTransitions::get' (Wildcard Next State)... ";

	finiteObservationTransitions->set(a1, s1, o1, 0.1);
	finiteObservationTransitions->set(a1, s2, o1, 0.2);
	finiteObservationTransitions->set(a2, s1, o1, 0.3);
	finiteObservationTransitions->set(a2, s2, o1, 0.4);
	finiteObservationTransitions->set(a1, s1, o2, 0.5);
	finiteObservationTransitions->set(a1, s2, o2, 0.6);
//	finiteObservationTransitions->set(a2, s1, o2, 0.7); // Leave undefined to test wildcard next state.
//	finiteObservationTransitions->set(a2, s2, o2, 0.8); // Leave undefined to test wildcard next state.
	finiteObservationTransitions->set(a2, nullptr, o2, 0.9);

	if (finiteObservationTransitions->get(a1, s1, o1) == 0.1 && finiteObservationTransitions->get(a1, s2, o1) == 0.2 &&
			finiteObservationTransitions->get(a2, s1, o1) == 0.3 && finiteObservationTransitions->get(a2, s2, o1) == 0.4 &&
			finiteObservationTransitions->get(a1, s1, o2) == 0.5 && finiteObservationTransitions->get(a1, s2, o2) == 0.6 &&
			finiteObservationTransitions->get(a2, s1, o2) == 0.9 && finiteObservationTransitions->get(a2, s2, o2) == 0.9) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete finiteObservationTransitions;
	finiteObservationTransitions = new ObservationTransitionsMap();

	std::cout << "ObservationTransitions: Test 'FiniteObservationTransitions::set' and ";
	std::cout << "'FiniteObservationTransitions::get' (Wildcard Action)... ";

	finiteObservationTransitions->set(a1, s1, o1, 0.1);
	finiteObservationTransitions->set(a1, s2, o1, 0.2);
	finiteObservationTransitions->set(a2, s1, o1, 0.3);
	finiteObservationTransitions->set(a2, s2, o1, 0.4);
//	finiteObservationTransitions->set(a1, s1, o2, 0.5); // Leave undefined to test wildcard action.
	finiteObservationTransitions->set(a1, s2, o2, 0.6);
//	finiteObservationTransitions->set(a2, s1, o2, 0.7); // Leave undefined to test wildcard action.
	finiteObservationTransitions->set(a2, s2, o2, 0.8);
	finiteObservationTransitions->set(nullptr, s1, o2, 0.9);

	if (finiteObservationTransitions->get(a1, s1, o1) == 0.1 && finiteObservationTransitions->get(a1, s2, o1) == 0.2 &&
			finiteObservationTransitions->get(a2, s1, o1) == 0.3 && finiteObservationTransitions->get(a2, s2, o1) == 0.4 &&
			finiteObservationTransitions->get(a1, s1, o2) == 0.9 && finiteObservationTransitions->get(a1, s2, o2) == 0.6 &&
			finiteObservationTransitions->get(a2, s1, o2) == 0.9 && finiteObservationTransitions->get(a2, s2, o2) == 0.8) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete finiteObservationTransitions;
	finiteObservationTransitions = new ObservationTransitionsMap();

	std::cout << "ObservationTransitions: Test 'FiniteObservationTransitions::set' and ";
	std::cout << "'FiniteObservationTransitions::get' (Wildcard Previous State)... ";

	finiteObservationTransitions->set(a1, s1, o1, 0.1);
	finiteObservationTransitions->set(a1, s2, o1, 0.2);
//	finiteObservationTransitions->set(a2, s1, o1, 0.3); // Leave undefined to test wildcard previous state.
	finiteObservationTransitions->set(a2, s2, o1, 0.4);
	finiteObservationTransitions->set(a1, s1, o2, 0.5);
	finiteObservationTransitions->set(a1, s2, o2, 0.6);
//	finiteObservationTransitions->set(a2, s1, o2, 0.7); // Leave undefined to test wildcard previous state.
	finiteObservationTransitions->set(a2, s2, o2, 0.8);
	finiteObservationTransitions->set(a2, s1, nullptr, 0.9);

	if (finiteObservationTransitions->get(a1, s1, o1) == 0.1 && finiteObservationTransitions->get(a1, s2, o1) == 0.2 &&
			finiteObservationTransitions->get(a2, s1, o1) == 0.9 && finiteObservationTransitions->get(a2, s2, o1) == 0.4 &&
			finiteObservationTransitions->get(a1, s1, o2) == 0.5 && finiteObservationTransitions->get(a1, s2, o2) == 0.6 &&
			finiteObservationTransitions->get(a2, s1, o2) == 0.9 && finiteObservationTransitions->get(a2, s2, o2) == 0.8) {
		std::cout << " Success." << std::endl;
		numSuccesses++;
	} else {
		std::cout << " Failure." << std::endl;
	}

	delete finiteObservationTransitions;
	finiteObservationTransitions = new ObservationTransitionsMap();

	ObservationsMap *Z = new ObservationsMap();
	Z->add(o1);
	Z->add(o2);

	std::cout << "ObservationTransitions: Test 'FiniteObservationTransitions::available'... ";

	finiteObservationTransitions->set(a1, s1, o1, 0.0);
	finiteObservationTransitions->set(a1, s1, o2, 0.0);
	finiteObservationTransitions->set(a1, s2, o1, 1.0);
	finiteObservationTransitions->set(a1, s2, o2, 0.0);
	finiteObservationTransitions->set(a2, s1, o1, 0.0);
	finiteObservationTransitions->set(a2, s1, o2, 1.0);
	finiteObservationTransitions->set(a2, s2, o1, 1.0);
	finiteObservationTransitions->set(a2, s2, o2, 1.0);

	finiteObservationTransitions->add_available(a1, s2, o1);
	finiteObservationTransitions->add_available(a2, s1, o2);
	finiteObservationTransitions->add_available(a2, s2, o1);
	finiteObservationTransitions->add_available(a2, s2, o2);

	try {
		const std::vector<Observation *> &tmp1 = finiteObservationTransitions->available(Z, a1, s1);
		std::cout << " Failure." << std::endl;
	} catch (ObservationTransitionException &err) {
		const std::vector<Observation *> &tmp2 = finiteObservationTransitions->available(Z, a1, s2);
		if (tmp2.size() == 1 && tmp2[0] == o1) {
			const std::vector<Observation *> &tmp3 = finiteObservationTransitions->available(Z, a2, s1);
			if (tmp3.size() == 1 && tmp3[0] == o2) {
				const std::vector<Observation *> &tmp4 = finiteObservationTransitions->available(Z, a2, s2);
				if (tmp4.size() == 2) {
					std::cout << " Success." << std::endl;
					numSuccesses++;
				} else {
					std::cout << " Failure." << std::endl;
				}
			} else {
				std::cout << " Failure." << std::endl;
			}
		} else {
			std::cout << " Failure." << std::endl;
		}
	}

	delete finiteObservationTransitions;
	finiteObservationTransitions = nullptr;

	delete Z;

	delete s1;
	delete s2;
	delete a1;
	delete a2;
//	delete o1; // Taken care of by Z.
//	delete o2; // Taken care of by Z.

	return numSuccesses;
}
