#ifndef _BranchAndBound_hpp
#define _BranchAndBound_hpp

#include "Simplex.hpp"

class BranchAndBound
{
public:

	Simplex t;

	BranchAndBound() {}

	void CurrentSubProblemCanBeFathomed(bool constraintsMet, double soln, double currentBestSoln, bool foundSolution, bool &canFathom);
	bool Branch(Simplex &subproblem1, Simplex &subproblem2);
	bool SolveVariableTypeConstrainedProblemFormulation();
};

#endif
