/*
 * PrismFileWriting.cpp
 *
 *  Created on: Sep 26, 2015
 *      Author: robvanbekkum
 */

#include <fstream>
#include "PrismFileWriting.hpp"
#include "FileUtility.hpp"

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

void writePrismFile(std::string fileName, DecPOMDPDiscreteInterface* mdp, PolicyVector policy) {
	//mdp
	//
	//module "filename"
	//
	//state:[0..#states-1];
	//
	//[] state = STATE -> PROB1:(state' = ENDSTATE1) + PROB2:(state' = ENDSTATE2);
	//[] ....
	//
	//endmodule
	//
	//init STATE1 | STATE2 | ... endinit

	std::ofstream prismFile;
	prismFile.open(fileName.c_str());
	prismFile << "mdp" << std::endl << std::endl;
	prismFile << "module " << remove_extension(trimFilePathToName(fileName)) << std::endl << std::endl;
	prismFile << "state:[0.." << (mdp->GetNrStates() - 1) << "];" << std::endl << std::endl;
	for (int state_no = 0; state_no < mdp->GetNrStates(); state_no++) {
		prismFile << getTransitionLine(state_no, mdp, policy);
	}
	prismFile << std::endl;
	prismFile << "endmodule" << std::endl << std::endl;
	prismFile << "init ";
	bool isFirst = true;
	for (int state_no = 0; state_no < mdp->GetNrStates(); state_no++) {
		if (mdp->GetInitialStateProbability(state_no) > 0) {
			if (!isFirst) {
				prismFile << " | ";
			}
			prismFile << "state=" << state_no;
			isFirst = false;
		}
	}
	prismFile << " endinit" << std::endl;


	prismFile.close();
}










