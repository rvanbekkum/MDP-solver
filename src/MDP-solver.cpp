/*
 * main.cpp
 *
 *  Created on: Sep 20, 2015
 *      Author: robvanbekkum
 */

#include <iostream>
#include <fstream>

#include "DecPOMDPDiscrete.h"
#include "Timing.h"
#include "SimulationDecPOMDPDiscrete.h"
#include "NullPlanner.h"
#include "directories.h"

#include "MDPValueIteration.h"
#include "QTable.h"
#include "AgentMDP.h"

#include "argumentHandlers.h"
#include "argumentUtils.h"

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

/**
 * Runs the simulation of the MDP.
 */
void runSimulation(const QTable & q, const PlanningUnitDecPOMDPDiscrete *np,
		const SimulationDecPOMDPDiscrete &sim) {
	AgentMDP agent(np, 0, q);
	AgentFullyObservable *newAgent;
	std::vector<AgentFullyObservable*> agents;

	for (Index i = 0; i < np->GetNrAgents(); i++) {
		newAgent = new AgentMDP(agent);
		newAgent->SetIndex(i);
		agents.push_back(newAgent);
	}
	SimulationResult result = sim.RunSimulations(agents);
	result.PrintSummary();

	for (Index i = 0; i < np->GetNrAgents(); i++)
		delete agents[i];
}

/**
 * Checks if the file with the passed name exists.
 *
 * @param fileName : name of the file
 *
 * @return bool indicating whether the file exists
 */
bool file_exists(const std::string& fileName) {
	std::ofstream file(fileName.c_str());
	return file.good();
}

int main(int argc, char **argv) {
	ArgumentHandlers::Arguments args;
	argp_parse(&ArgumentHandlers::theArgpStruc, argc, argv, 0, 0, &args);

	try {
		Timing Time;
		std::cout << "Instantiating the problem..." << std::endl;
		DecPOMDPDiscreteInterface* decpomdp =
				GetDecPOMDPDiscreteInterfaceFromArgs(args);
		std::cout << "...done." << std::endl;

		// Set up output files
		std::string resultsFileName = "/dev/null", timingsFilename = "/dev/null";
		if (!args.dryrun) {
			std::stringstream ss;
			args.dpf;
			ss << args.dpf << "_results" << "_h" << args.horizon;
			resultsFileName = ss.str();
			timingsFilename = resultsFileName + "_Timings";
			if (!file_exists(resultsFileName)) {
				std::cout << "VI: could not open " << resultsFileName << std::endl;
				std::cout << "Results will not be stored to disk." << std::endl;
				args.dryrun = true;
			}
		}

		//start VI
		PlanningUnitDecPOMDPDiscrete *np = new NullPlanner(args.horizon,
				decpomdp);
		MDPValueIteration vi(*np);
		std::cout << "Running value iteration..." << std::endl;
		Time.Start("Plan");
		vi.Plan();
		Time.Stop("Plan");
		std::cout << "...done." << std::endl;
		QTable q = vi.GetQTable(0); //<- infinite horizon, so get 1 value function of stage 0

		int nrRuns = args.nrRuns; //defaults to 1000, see argumentHandlers.h
		int seed = args.randomSeed; //defaults to 42
		std::cout << "Simulating policy with nrRuns: " << nrRuns
				<< " and seed: " << seed << std::endl;
		SimulationDecPOMDPDiscrete sim(*np, nrRuns, seed);

		// Write intermediate simulation results to file
		if (!args.dryrun)
			sim.SaveIntermediateResults(resultsFileName);

		std::vector<double> avgRewards;
		runSimulation(q, np, sim);

		// Write VI timing information to file
		if (!args.dryrun)
			Time.Save(timingsFilename);
	} catch (E& e) {
		e.Print();
	}

	return (0);
}
