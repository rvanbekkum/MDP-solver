/*
 * main.cpp
 *
 *  Created on: Sep 20, 2015
 *      Author: robvanbekkum
 */

#include <iostream>

#include "DecPOMDPDiscrete.h"
#include "Timing.h"
#include "NullPlanner.h"
#include "directories.h"

#include "MDPValueIteration.h"

#include "argumentHandlers.h"
#include "argumentUtils.h"

#include "PolicyVector.hpp"
#include "FileUtility.hpp"
#include "PrismFileWriting.hpp"

using namespace ArgumentUtils;

// Program documentation
static char doc[] =
		"MDP-solver - loads a single agent MDP problem, runs value iteration, and simulates the resulting joint policy using AgentMDP class. \
\vFor more information please consult the MADP documentation. \
";

//NOTE: make sure that the below value (nrChildParsers) is correct!
const int nrChildParsers = 6;
const struct argp_child childVector[] = { ArgumentHandlers::problemFile_child,
		ArgumentHandlers::globalOptions_child,
		ArgumentHandlers::outputFileOptions_child,
		ArgumentHandlers::modelOptions_child,
		ArgumentHandlers::solutionMethodOptions_child,
		ArgumentHandlers::simulation_child, { 0 } };
#include "argumentHandlersPostChild.h"

static std::string prismFileName;
static std::string timingsFileName;

static DecPOMDPDiscreteInterface* instantiateProblem(ArgumentHandlers::Arguments& args) {
	std::cout << "Instantiating the problem..." << std::endl;
	DecPOMDPDiscreteInterface* mdp = GetDecPOMDPDiscreteInterfaceFromArgs(args);
	std::cout << "...done." << std::endl;
	return mdp;
}

static void setupOutputFiles(ArgumentHandlers::Arguments& args) {
	// Set up output files
	prismFileName = "/dev/null"; timingsFileName = "/dev/null";
	if (!args.dryrun) {
		std::stringstream ss;
		ss << remove_extension(args.dpf) << "_d" << fractional_part_as_int(args.discount, 2) << "_h" << args.horizon << ".nm";
		prismFileName = ss.str();
		timingsFileName = remove_extension(prismFileName) + "_Timings";
		if (!file_exists(prismFileName)) {
			std::cout << "VI: could not open " << prismFileName << std::endl;
			std::cout << "Results will not be stored to disk." << std::endl;
			args.dryrun = true;
		}
	}
}

PolicyVector getOptimalPolicy(DecPOMDPDiscreteInterface* mdp, MDPValueIteration& vi) {
	std::cout << "Optimal policy:" << std::endl;
	std::vector<Index> vector;
	for (int state_no = 0; state_no < mdp->GetNrStates(); state_no++) {
		Index maximizingActionIndex = vi.GetMaximizingAction(0, state_no);
		std::cout << "State no.: " << state_no << " Maximizing action: " << maximizingActionIndex << std::endl;
		vector.push_back(maximizingActionIndex);
	}
	PolicyVector p(vector);
	return p;
}

/**
 * Applies value iteration for the MDP problem.
 */
static PolicyVector applyValueIteration(ArgumentHandlers::Arguments& args, DecPOMDPDiscreteInterface* mdp) {
	// Apply Value Iteration
	PlanningUnitDecPOMDPDiscrete *np = new NullPlanner(args.horizon, mdp);
	MDPValueIteration vi(*np);
	std::cout << "Running value iteration..." << std::endl;
	Timing time;
	time.Start("Plan");
	vi.Plan();
	time.Stop("Plan");
	std::cout << "...done." << std::endl;

	// Write VI timing information to file
	if (!args.dryrun)
		time.Save(timingsFileName);

	PolicyVector policy = getOptimalPolicy(mdp, vi);
	delete np;
	return policy;
}

int main(int argc, char **argv) {
	ArgumentHandlers::Arguments args;
	argp_parse(&ArgumentHandlers::theArgpStruc, argc, argv, 0, 0, &args);

	try {
		// Instantiate the problem, set-up output files and retrieve the optimal policy
		DecPOMDPDiscreteInterface* mdp = instantiateProblem(args);
		setupOutputFiles(args);
		PolicyVector optimalPolicy = applyValueIteration(args, mdp);
		writePrismFile(prismFileName, mdp, optimalPolicy);

	} catch (E& e) {
		e.Print();
	}

	return (0);
}
