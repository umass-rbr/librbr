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


#include "../../include/management/raw_file.h"

#include "../../include/utilities/log.h"
#include "../../include/utilities/string_manipulation.h"

#include "../../include/core/core_exception.h"

#include "../../include/core/states/states_map.h"
#include "../../include/core/states/indexed_state.h"

#include "../../include/core/actions/actions_map.h"
#include "../../include/core/actions/indexed_action.h"

#include "../../include/core/state_transitions/state_transitions_array.h"

//#include "../../include/core/observation_transitions/observation_transitions_array.h"

#include "../../include/core/rewards/rewards.h"
#include "../../include/core/rewards/factored_rewards.h"
//#include "../../include/core/rewards/s_rewards.h"
//#include "../../include/core/rewards/s_rewards_array.h"
#include "../../include/core/rewards/sa_rewards.h"
#include "../../include/core/rewards/sa_rewards_array.h"
#include "../../include/core/rewards/sas_rewards.h"
#include "../../include/core/rewards/sas_rewards_array.h"
//#include "../../include/core/rewards/saso_rewards.h"
//#include "../../include/core/rewards/saso_rewards_array.h"
#include "../../include/core/rewards/reward_exception.h"

#include "../../include/core/initial.h"
#include "../../include/core/horizon.h"

RawFile::RawFile()
{ }

RawFile::~RawFile()
{ }

MDP *RawFile::load_raw_mdp(std::string filename)
{
	// Open the file stream and make sure the file is found; throw an exception otherwise.
	std::ifstream file(filename);

	if (!file.is_open()) {
		log_message("RawFile::load_raw_mdp", "Failed to open file '" + filename + "'.");
		throw CoreException();
	}

	// Attempt to read the header information: number of states (n), number of actions (m),
	// number of reward factors (k), rewards type (r), the initial state (s0), and the horizon (h).
	unsigned int n = 0;
	unsigned int m = 0;
	unsigned int k = 0;
	unsigned int r = 0;
	unsigned int s0 = 0;
	unsigned int h = 0; // 0 = infinite
	double g = 1.0;

	try {
		file >> n >> m >> k >> r >> s0 >> h >> g;

		// Read the last blank character at the end of this line. If you do not do this,
		// then the entire load_data sequence will be messed up.
		std::string endOfLine;
		std::getline(file, endOfLine);
	} catch (std::exception &err) {
		log_message("RawFile::load_raw_mdp",
				"Failed to read number of states and actions from file '" + filename + "'.");
		throw CoreException();
	}

	// Ensure that all header variables are valid.
	if (n == 0) {
		log_message("RawFile::load_raw_mdp",
				"Invalid number of states in the header for file '" + filename + "'.");
		throw CoreException();
	}

	if (m == 0) {
		log_message("RawFile::load_raw_mdp",
				"Invalid number of actions in the header for file '" + filename + "'.");
		throw CoreException();
	}

	if (k == 0) {
		log_message("RawFile::load_raw_mdp",
				"Invalid number of reward factors in the header for file '" + filename + "'.");
		throw CoreException();
	}

	if (r == RawFileRewardsType::RawFileSASORewards || r >= RawFileRewardsType::NumRawFileRewardsTypes) {
		log_message("RawFile::load_raw_mdp",
				"Invalid rewards type in the header for file '" + filename + "'.");
		throw CoreException();
	}

	if (s0 >= n) {
		log_message("RawFile::load_raw_mdp",
				"Invalid initial state in the header for file '" + filename + "'.");
		throw CoreException();
	}

	// Note: All values of an unsigned int are valid horizon values.

	if (g < 0.0 || g > 1.0) {
		log_message("RawFile::load_raw_mdp",
				"Invalid discount factor (gamma) in the header for file '" + filename + "'.");
		throw CoreException();
	}

	// Attempt to read in the state transition blocks.
	float *T = new float[n * m * n];
	for (unsigned int s = 0; s < n; s++) {
		try {
			load_data(file, m, n, T, s * m * n);
		} catch (CoreException &err) {
			log_message("RawFile::load_raw_mdp",
					"Failed to load state transition block for state " + std::to_string(s) +
					" for file '" + filename + "'.");
		}
	}

	// Based on the rewards type, load them differently. We have a number of
	// reward factors equal to the factor value loaded.
	float **R = new float*[k];

	for (unsigned int i = 0; i < k; i++) {
		switch (r) {
		case RawFileRewardsType::RawFileSRewards:
			// ToDo: Implement this after creating SRewards.
			throw CoreException();
			break;
		case RawFileRewardsType::RawFileSARewards:
			// Attempt to read in the SA reward blocks.
			R[i] = new float[n * m];

			try {
				load_data(file, n, m, R[i], 0);
			} catch (CoreException &err) {
				log_message("RawFile::load_raw_mdp",
						"Failed to load SA reward data for file '" + filename + "'.");
				throw err;
			}

			break;
		case RawFileRewardsType::RawFileSASRewards:
			// Attempt to read in the SAS reward blocks.
			R[i] = new float[n * m * n];

			try {
				for (unsigned int s = 0; s < n; s++) {
					load_data(file, m, n, R[i], s * m * n);
				}
			} catch (CoreException &err) {
				log_message("RawFile::load_raw_mdp",
						"Failed to load SAS reward data for file '" + filename + "'.");
				throw err;
			}

			break;
		default:
			log_message("RawFile::load_raw_mdp",
					"Failed to load an unknown type of reward data for file '" + filename + "'.");
			throw CoreException();
			break;
		}
	};

	// Create the states, and assign the initial state while iterating.
	IndexedState::reset_indexer();

	StatesMap *states = new StatesMap();

	for (unsigned int i = 0; i < n; i++) {
		IndexedState *s = new IndexedState();
		states->add(s);
	}

	// Create the actions.
	IndexedAction::reset_indexer();

	ActionsMap *actions = new ActionsMap();
	for (unsigned int i = 0; i < m; i++) {
		actions->add(new IndexedAction());
	}

	// Create the state transitions.
	StateTransitionsArray *stateTransitions = new StateTransitionsArray(n, m);
	stateTransitions->set_state_transitions(T);

	delete [] T;

	// Create the rewards, based on the reward type.
	Rewards *rewards = nullptr;

	for (unsigned int i = 0; i < k; i++) {
		Rewards *Ri = nullptr;
		SARewardsArray *RiSA = nullptr;
		SASRewardsArray *RiSAS = nullptr;

		switch (r) {
		case RawFileRewardsType::RawFileSRewards:
			// ToDo: Implement this after creating SRewards.
			throw CoreException();
			break;
		case RawFileRewardsType::RawFileSARewards:
			RiSA = new SARewardsArray(n, m);
			RiSA->set_rewards(R[i]);
			Ri = RiSA;
			break;
		case RawFileRewardsType::RawFileSASRewards:
			RiSAS = new SASRewardsArray(n, m);
			RiSAS->set_rewards(R[i]);
			Ri = RiSAS;
			break;
		};

		// To reuse code, do this trick. If this is a *not* a factored reward, then
		// assign rewards to Ri; however, if it *is* a factored reward, then
		// create the factored reward object (if needed) and assign its i-th factor.
		if (k == 1) {
			rewards = Ri;
		} else {
			if (rewards == nullptr) {
				rewards = new FactoredRewards();
			}

			FactoredRewards *RF = dynamic_cast<FactoredRewards *>(rewards);
			if (RF == nullptr) {
				throw CoreException();
			}

			RF->add_factor(Ri);
		}
	}

	for (unsigned int i = 0; i < k; i++) {
		delete [] R[i];
	}
	delete [] R;

	// Create the horizon.
	Horizon *horizon = new Horizon();
	horizon->set_discount_factor(g);
	horizon->set_horizon(h);

	// Finally, after all that, return the MDP.
	return new MDP(states, actions, stateTransitions, rewards, horizon);
}

void RawFile::save_raw_mdp(MDP *mdp, std::string filename)
{
	// Load the file and throw an error if this cannot be done.
	std::ofstream file(filename);

	if (!file.is_open()) {
		log_message("RawFile::save_raw_mdp", "Failed to create the file '" + filename + "'.");
		throw CoreException();
	}

	// Note: FactoredStates, and even JointActions, will automatically work, because the
	// entire set of all possible state tuples should be generated. State transitions will
	// also already be defined for this. It is different for FactoredRewards, which do *not*
	// have a 'flattened' representation. They must be treated for each possible case.

	// First, obtain all the objects for easy reference. Also, check to make sure this
	// is a valid MDP.
	StatesMap *S = dynamic_cast<StatesMap *>(mdp->get_states());
	ActionsMap *A = dynamic_cast<ActionsMap *>(mdp->get_actions());
	StateTransitions *T = mdp->get_state_transitions();
	Rewards *R = nullptr;
	Horizon *h = mdp->get_horizon();

	unsigned int k = 0;
	unsigned int r = 0;

	R = dynamic_cast<FactoredRewards *>(mdp->get_rewards());
	if (R == nullptr) {
		k = 1;

//		SRewards *Ri = dynamic_cast<SRewards *>(mdp->get_rewards());
//		r = RawFileRewardsType::RawFileSRewards;
//
//		SARewards *Ri = dynamic_cast<SARewards *>(mdp->get_rewards());
//		r = RawFileRewardsType::RawFileSARewards;

		SASRewards *Ri = dynamic_cast<SASRewards *>(mdp->get_rewards());
		r = RawFileRewardsType::RawFileSASRewards;

		// This is not a valid rewards object.
		if (Ri == nullptr) {
			throw CoreException();
		}
	} else {
		FactoredRewards *RF = dynamic_cast<FactoredRewards *>(R);
		if (RF == nullptr) {
			throw CoreException();
		}
		k = RF->get_num_rewards();

		// Note: We attempt to cast the first one, and assume that the rest are of the same class.
		Rewards *R0 = nullptr;
		try {
			R0 = RF->get(0);
		} catch (const RewardException &err) {
			log_message("RawFile::save_raw_mdp",
					"Found null reward, and thus failed to save the file '" + filename + "'.");
			throw CoreException();
		}

		// TODO: Include SRewards.

		// Determine the type of rewards object.

//		SRewards *Ri = dynamic_cast<SRewards *>(R0);
//		r = RawFileRewardsType::RawFileSRewards;
//
//		SARewards *Ri = dynamic_cast<SARewards *>(R0);
//		r = RawFileRewardsType::RawFileSARewards;

		SASRewards *Ri = dynamic_cast<SASRewards *>(R0);
		r = RawFileRewardsType::RawFileSASRewards;

		// This is not a valid rewards object.
		if (Ri == nullptr) {
			throw CoreException();
		}
	}

	if (S == nullptr || S->get_num_states() == 0 || A == nullptr || A->get_num_actions() == 0 ||
			T == nullptr || R == nullptr || h == nullptr) {
		log_message("RawFile::save_raw_mdp",
				"Failed to parse the MDP provided and save the file '" + filename + "'.");
		throw CoreException();
	}

	// Create the header which contains state, action, and horizon information.
	file << S->get_num_states() << " " << A->get_num_actions() << " " << k << " " << r << " ";
	file << h->get_horizon() << " " << h->get_discount_factor() << std::endl;

	// Write all of the state transition information.
	for (auto state : *S) {
		State *s = resolve(state);

		for (auto action : *A) {
			Action *a = resolve(action);
			unsigned int i = 0;

			for (auto nextState : *S) {
				State *sp = resolve(nextState);

				file << T->get(s, a, sp);

				if (i < S->get_num_states() - 1) {
					file << " ";
				}
				i++;
			}
			file << std::endl;
		}
	}

	// Write all of the reward information, based on the reward type.
	for (unsigned int i = 0; i < k; i++) {
		// In order to reuse code, do this small trick. If this is only a single reward, then
		// we will just type cast the normal reward R; however, if this is a factored reward, then
		// get the particular factor we are iterating over.
		Rewards *Ri = nullptr;
		FactoredRewards *RF = nullptr;
		SARewards *RiSA = nullptr;
		SASRewards *RiSAS = nullptr;

		if (k == 1) {
			Ri = R;
		} else {
			RF = dynamic_cast<FactoredRewards *>(R);
			if (RF == nullptr) {
				throw CoreException();
			}
			Ri = RF->get(i);
		}

		switch (r) {
		case RawFileRewardsType::RawFileSRewards:
			// TODO: Implement this after creating SRewards.
			throw CoreException();
			break;
		case RawFileRewardsType::RawFileSARewards:
			for (auto state : *S) {
				State *s = resolve(state);
				unsigned int i = 0;

				for (auto action : *A) {
					Action *a = resolve(action);

					RiSA = dynamic_cast<SARewards *>(Ri);
					if (RiSA == nullptr) {
						throw CoreException();
					}

					file << RiSA->get(s, a);

					if (i < S->get_num_states() - 1) {
						file << " ";
					}
					i++;
				}
				file << std::endl;
			}
			break;
		case RawFileRewardsType::RawFileSASRewards:
			for (auto state : *S) {
				State *s = resolve(state);

				for (auto action : *A) {
					Action *a = resolve(action);
					unsigned int i = 0;

					for (auto nextState : *S) {
						State *sp = resolve(nextState);

						RiSAS = dynamic_cast<SASRewards *>(Ri);
						if (RiSAS == nullptr) {
							throw CoreException();
						}

						file << RiSAS->get(s, a, sp);

						if (i < S->get_num_states() - 1) {
							file << " ";
						}
						i++;

					}
					file << std::endl;
				}
			}
			break;
		}
	}

	file.close();
}

void RawFile::load_data(std::ifstream &file, unsigned int rows, unsigned int cols, float *array, unsigned int offset)
{
	// For each of the rows, attempt to read and parse the line.
	for (unsigned int r = 0; r < rows; r++) {
		if (file.eof()) {
			throw CoreException();
		}

		std::string line;
		std::getline(file, line);

		// Split the line by ' ', attempt to convert each token into a float, and assign the value in
		// the array, if successful.
		std::vector<std::string> items = split_string_by_space(line);
		if (items.size() != cols) {
			throw CoreException();
		}

		for (unsigned int c = 0; c < cols; c++) {
			try {
				array[offset + (r * cols + c)] = std::stof(items[c]);
			} catch (std::exception &err) {
				throw CoreException();
			}
		}
	}
}
