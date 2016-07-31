#include "BranchAndBound.hpp"
#include "Simplex.hpp"

void BranchAndBound::CurrentSubProblemCanBeFathomed(bool constraintsMet, double soln, double currentBestSoln, bool foundSolution, bool &canFathom)
{
	if (foundSolution == false || soln <= currentBestSoln || constraintsMet == true)
	{
		canFathom = true;
	}
	else
	{
		canFathom = false;
	}

	return;
}

bool BranchAndBound::Branch(Simplex &subproblem1, Simplex &subproblem2)
{
	long numCols = subproblem1.tableau.GetNumCols();
	
	//branch on first variable that hasn't met its type requirement
	for (long col=1; col<numCols; col++)
	{
		long row = subproblem1.rowOfBasicVar[col];

		if (row > 0)
		{
			if (subproblem1.integerVar[col] == true)
			{
				if (subproblem1.DetermineIfVariableMeetsVariableTypeConstraint(row) == false)
				{
					double value1 = (double)((long)subproblem1.tableau(row, numCols-1));
					subproblem1.AddLessThanConstraint(col, value1);

					double value2 = -value1-1.0;
					subproblem2.AddLessThanConstraint(col, value2);

					return true;
				}
			}
		}

		if (subproblem1.binaryVar[col] == true && subproblem1.forcedBinaryVar[col] == false)
		{
			subproblem1.AddEqualToConstraint(col, 0.0);
			subproblem2.AddEqualToConstraint(col, 1.0);
			
			return true;
		}
	}

	return false;
}

bool BranchAndBound::SolveVariableTypeConstrainedProblemFormulation()
{
	//make sure initial tableau is in the proper form
	t.ConvertTableauToProperForm();

	//solve original problem using LP relaxation
	bool foundOptimalSolution;

	if (t.problemIsTwoPhase)
	{
		foundOptimalSolution = t.SolveRelaxed2PhaseProblemFormulation();
	}
	else
	{
		foundOptimalSolution = t.SolveProblemFormulationUsingSimplexAlgorithm();
	}

	if (foundOptimalSolution)
	{
		//determine if current solution meets binary/integer constraints
		bool constraintsMet = t.DetermineIfSolutionMeetsVariableTypeConstraints();

		//if variables meet type constraints then already found optimal solution
		if (constraintsMet)
		{
			return true;
		}
	}
	else
	{
		//if cannot solve the original problem, no constrained solutions are possible
		return false;
	}

	//push original tableau onto the subproblem list
	Simplex optimalTableau;
	vector<Simplex> subProblem;
	subProblem.push_back(t);

	//create tableaus for branching
	Simplex subproblem1, subproblem2;
	vector<Simplex>::iterator p;

	double currentBestSoln=-1e10;

	//loop until run out of active subproblems
	while (!subProblem.empty())
	{
		//get most recently created subproblem
		bool foundProblemToBranchOn=false;
		long cnt=0, location=-1;
		double soln=-1e10, bestSubproblemSoln=-1e10;
		p = subProblem.begin();

		while (p != subProblem.end())
		{
			//get bound of subproblem
			soln = p->GetSolutionForBranching();
				
			if (soln > bestSubproblemSoln)
			{
				location = cnt;
				bestSubproblemSoln = soln;
				foundProblemToBranchOn = true;
			}

			//now go to next subproblem on the list
			p++;
			cnt++;
		}

		//make sure found a problem to branch on
		if (foundProblemToBranchOn == false)
		{
			printf("Did not find acceptable problem to branch on\n");
			assert(0);
		}
	
		//set subproblems to the one with the greatest bound and
		//delete the problem that we are to branch on
		cnt=0;
		p = subProblem.begin();

		while (p != subProblem.end())
		{
			if (cnt==location)
			{
				subproblem1 = *p;
				subproblem2 = *p;
				subProblem.erase(p);
	
				break;
			}

			p++;
			cnt++;
		}

		//make sure problem to be branched on was found
		if (cnt != location)
		{
			printf("Did not find problem to branch on\n");
			assert(0);
		}

		//branch on the subproblems
		bool branchSuccessful = Branch(subproblem1, subproblem2);

		if (branchSuccessful == false)
		{
			printf("branching not successful, an error must have occured\n");
			assert(0);
		}

		//push subproblems on list, make sure subproblems are valid before pushing them
		long numLoopsReqd=0;
				
		if (subproblem1.TableauStillValidAfterAddingConstraint())
		{
			numLoopsReqd++;
			subProblem.push_back(subproblem1);
		}
		
		if (subproblem2.TableauStillValidAfterAddingConstraint())
		{
			numLoopsReqd++;
			subProblem.push_back(subproblem2);
		}

		bool updatedOptimal=false, foundSolution=false;
		p = subProblem.end();

		for (cnt=0; cnt<numLoopsReqd; cnt++)
		{
			//get subproblem and re-optimize final tableau
			p--;
			foundSolution = p->SolveProblemFormulationUsingDualSimplexAlgorithm();

			//get solution bound for subproblem
			double soln=-1e10;

			if (foundSolution)
			{
				soln = p->GetSolutionForBranching();
			}

			//determine if constraints are met and if subproblem can be fathomed
			bool canFathom, constraintsMet;
			constraintsMet = p->DetermineIfSolutionMeetsVariableTypeConstraints();
			CurrentSubProblemCanBeFathomed(constraintsMet, soln, currentBestSoln, foundSolution, canFathom);
			
			//if can fathom, delete subproblem and set new bounds if applicable
			if (canFathom)
			{
				//if constraints met, reset current bound if current solution is better
				if (constraintsMet == true && soln > currentBestSoln)
				{
					//set current best tableau and solution
					foundOptimalSolution = true;
					optimalTableau = *p;
					currentBestSoln = soln;
					updatedOptimal = true;
				}
				
				//delete current subproblem
				p = subProblem.erase(p);
			}
		}
		
		if (updatedOptimal)
		{
			//go back and fathom other subproblems if possible based on new bound
			p = subProblem.begin();
			
			while (p != subProblem.end())
			{
				//get bound of subproblem
				soln = p->GetSolutionForBranching();
				
				if (soln < currentBestSoln)
				{
					//erase and go to next subproblem on the list
					p = subProblem.erase(p);
				}
				else
				{
					//else just go to next subproblem on the list
					p++;
				}
			}
		}
	}

	//set optimal tableau to global tableau
	t = optimalTableau;

	return foundOptimalSolution;
}
