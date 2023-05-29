/*MIT License

Copyright(c) 2020 Thibaut Vidal

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#ifndef POPULATION_H
#define POPULATION_H

#include "Individual.h"
#include "LocalSearch.h"
#include "Split.h"

typedef std::vector <Individual*> SubPopulation ;

class Population
{
   private:

   Params & params ;							// Problem parameters
   Split & split;								// Split algorithm
   LocalSearch & localSearch;					// Local search structure
   SubPopulation feasibleSubpop;			    // Feasible subpopulation, kept ordered by increasing penalized cost
   SubPopulation infeasibleSubpop;		        // Infeasible subpopulation, kept ordered by increasing penalized cost
   std::list <bool> listFeasibilityLoad ;		// Load feasibility of the last 100 individuals generated by LS
   std::list <bool> listFeasibilityDuration ;	// Duration feasibility of the last 100 individuals generated by LS
   std::vector<std::pair<clock_t, double>> searchProgress; // Keeps tracks of the time stamps of successive best solutions
   Individual bestSolutionRestart;              // Best solution found during the current restart of the algorithm
   Individual bestSolutionOverall;              // Best solution found during the complete execution of the algorithm

   // Evaluates the biased fitness of all individuals in the population
   void updateBiasedFitnesses(SubPopulation & pop);

   // Removes the worst individual in terms of biased fitness
   void removeWorstBiasedFitness(SubPopulation & subpop);

   public:

   // Creates an initial population of individuals
   void generatePopulation();

   // Add an individual in the population (survivor selection is automatically triggered whenever the population reaches its maximum size)
   // Returns TRUE if a new best solution of the run has been found
   bool addIndividual (const Individual & indiv, bool updateFeasible);

   // Cleans all solutions and generates a new initial population (only used when running HGS until a time limit, in which case the algorithm restarts until the time limit is reached)
   void restart();

   // Adaptation of the penalty parameters
   void managePenalties();

   // Select an individal by binary tournament in the union of the feasible and infeasible subpopulations.
   const Individual & getBinaryTournament();

   // Accesses the best feasible individual
   const Individual * getBestFeasible();

   // Accesses the best infeasible individual
   const Individual * getBestInfeasible();

   // Accesses the best found solution at all time
   const Individual * getBestFound();

   // Access to the cost history
   std::vector<std::pair<double, double>> getCostHistory();

   // Prints population state
   void printState(int nbIter, int nbIterNoImprovement);

   // Distance measure between two individuals, used for diversity calculations
   double brokenPairsDistance(const Individual & indiv1, const Individual & indiv2);

   // Returns the average broken pairs distance of this individual with the nbClosest individuals in the population
   double averageBrokenPairsDistanceClosest(const Individual & indiv, int nbClosest);

   // Returns the average diversity value among the 50% best individuals in the subpopulation
   double getDiversity(const SubPopulation & pop);

   // Returns the average solution value among the 50% best individuals in the subpopulation
   double getAverageCost(const SubPopulation & pop);

   // Exports in a file the history of solution improvements
   void exportSearchProgress(std::string fileName, std::string instanceName);

   // Exports an Individual in CVRPLib format
   void exportCVRPLibFormat(const Individual & indiv, std::string fileName);

   // Constructor
   Population(Params & params, Split & split, LocalSearch & localSearch);

   // Destructor
   ~Population();
};

#endif
