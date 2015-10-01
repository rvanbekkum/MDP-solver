/*
 * PrismFileWriting.cpp
 *
 *  Created on: Sep 26, 2015
 *      Author: robvanbekkum
 */

#include <fstream>
#include "PrismFileWriting.hpp"
#include "FileUtility.hpp"

#include <sys/types.h>
#include <sys/stat.h>

/**
 * Retrieves a line for a transition from a certain state to all possible successor states
 * and the probability ending up in that state.
 *
 * @param state_no : The identifying number of the source state
 * @param mdp : The MDP defining the total number of states
 * @param policy : The policy defining which action to take from this state
 *
 * @return Line for PRISM mdp input file defining probabilities of getting from a state to each other state with the action define by the passed policy.
 */
static std::string getTransitionLine(int state_no, DecPOMDPDiscreteInterface* mdp, PolicyVector policy) {
	Index maximizing_action_no = policy.get(state_no);
	std::stringstream ss;
	ss << "[] state = " << state_no << " -> ";
	for (int state_suc_no = 0; state_suc_no < mdp->GetNrStates(); state_suc_no++) {
		float prob = mdp->GetTransitionProbability(state_no, maximizing_action_no, state_suc_no);
		ss << prob << ":(state' = " << state_suc_no << ")";
		if (state_suc_no == mdp->GetNrStates() - 1) {
			ss << ";" << std::endl;
		}
		else {
			ss << " + ";
		}
	}
	return ss.str();
}

/**
 * Retrieves a line defining the initial states of the mdp model in a PRISM nm mdp input file.
 *
 * @param mdp : The MDP model defining the initial-state-probabilities
 *
 * @return Line for PRISM mdp input file defining the initial states of the model.
 */
static std::string getInitialStatesLine(DecPOMDPDiscreteInterface* mdp) {
	std::stringstream ss;
	ss << "init ";
	bool isFirst = true;
	for (int state_no = 0; state_no < mdp->GetNrStates(); state_no++) {
		if (mdp->GetInitialStateProbability(state_no) > 0) {
			if (!isFirst) {
				ss << " | ";
			}
			ss << "state=" << state_no;
			isFirst = false;
		}
	}
	ss << " endinit" << std::endl;
	return ss.str();
}

/**
 * Writes to the file specified by filePath contents of a PRISM input file based on the passed
 * MDP definition and the provided policy corresponding to this mdp model.
 *
 * @param filePath : The path of the file to write to
 * @param mdp : The MDP model on which to base the contents of the PRISM file
 * @param policy : The policy that should be used for the mdp
 *
 * Outputs a file with contents of the following form:
 *
 * 	mdp
 *
 *  module FILENAME
 *
 *  state:[0..#states-1];		// states are indexed from 0 to n - 1
 *
 *  [] state = STATE_1 -> PROB_1:(state' = ENDSTATE_1) + ... + PROB_N:(state' = ENDSTATE_N);
 *  [] ....
 *  [] state = STATE_N -> PROB_1:(state' = ENDSTATE_1) + ... + PROB_N:(state' = ENDSTATE_N);
 *
 *  endmodule
 *
 *  init STATE_i=PROB_i | STATE_j=PROB_j | ... endinit
 *
 */
void writePrismFile(std::string filePath, DecPOMDPDiscreteInterface* mdp, PolicyVector policy) {
	std::ofstream prismFile;
	prismFile.open(filePath.c_str());

	// Define that the model is a Markov Decision Process
	prismFile << "mdp" << std::endl << std::endl;
	// Set the module name to the file name
	prismFile << "module " << remove_extension(trimFilePathToName(filePath)) << std::endl << std::endl;
	// Define the (number of) states
	prismFile << "state:[0.." << (mdp->GetNrStates() - 1) << "];" << std::endl << std::endl;
	// Define the transition for each state and probabilities of ending up in other states for that transition
	for (int state_no = 0; state_no < mdp->GetNrStates(); state_no++) {
		prismFile << getTransitionLine(state_no, mdp, policy);
	}
	prismFile << std::endl;
	prismFile << "endmodule" << std::endl << std::endl;

	// Define the initial/starting states of the model
	prismFile << getInitialStatesLine(mdp);

	prismFile.close();
}

/**
 * Creates the results folder in the same directory as the location of the problem definition file.
 *
 * @param problemsFilePath : The full path of the problem definition file
 *
 * @return The results folder path
 */
static std::string createResultsFolder(std::string problemFilePath) {
	std::size_t problemDirPathIndex = problemFilePath.find_last_of("\\/");
	std::string problemDirPath = problemFilePath.substr(0, problemDirPathIndex);
	mkdir((problemDirPath + "/results/").c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	return problemDirPath + "/results/";
}

/**
 * Retrieves the full path of the results file.
 *
 * @param problemsFilePath : The full path of the problem definition file
 *
 * @return The result file path
 */
static std::string getResultsFilePath(std::string problemsFilePath) {
	std::stringstream ss;
	ss << createResultsFolder(problemsFilePath) << remove_extension(trimFilePathToName(problemsFilePath));
	return ss.str();
}

/**
 * Retrieves the full path of where to save the PRISM input file (including parameters in the name).
 *
 * @param problemsFilePath : The full path of the problem definition file
 * @param discount : The discount rate used
 * @param horizon : The horizon used
 *
 * @return full file path of where to store the PRISM input file contents
 */
std::string getPrismFilePath(std::string problemFilePath, double discount, double horizon) {
	std::stringstream ss;
	ss << getResultsFilePath(problemFilePath) << "_d" << fractional_part_as_int(discount, 2) << "_h" << horizon << ".nm";
	return ss.str();
}
