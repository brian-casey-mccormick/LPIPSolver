#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "driver.hpp"
#include "Simplex.hpp"
#include "BranchAndBound.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"
#include "Comment.hpp"

using namespace std;

void InitializeData(Simplex &t)
{
	long numIntegerVariables=0;
	long numBinaryVariables=0;
	long numOriginalVariables=0;

	FILE *fp=NULL;

	if ((fp = fopen("LP.inp", "r")) == NULL)
	{
		printf("Unable to open LP.inp\n");
		printf("\n");
		assert(0);
	}

	//read whether or not problem is two phase problem
	skipcomment(fp);
	fscanf(fp, "problemIsTwoPhase = %ld ", &t.problemIsTwoPhase);

	//read the number of constraint equations
	long numConstraintEquations;
	skipcomment(fp);
	fscanf(fp, "numConstraintEquations = %ld ", &numConstraintEquations);

	//read the number of Xi variables
	long numXiVariables;
	skipcomment(fp);
	fscanf(fp, "numXiVariables = %ld ", &numXiVariables);

	//define the number of rows and the number of columns for the tableau
	long numRows = numConstraintEquations + 1;
	long numCols = numXiVariables + 2;

	//read tableau variable data
	for (long col=0; col<numCols; col++)
	{
		bool tempValue;
		skipcomment(fp);
		fscanf(fp, "%ld ", &tempValue);

		if (tempValue)
		{
			numOriginalVariables++;
		}
	}
	
	for (long col=0; col<numCols; col++)
	{
		bool tempValue;
		skipcomment(fp);
		fscanf(fp, "%ld ", &tempValue);
		t.artificialVar.push_back(tempValue);
	}
	
	for (long col=0; col<numCols; col++)
	{
		bool tempValue;
		skipcomment(fp);
		fscanf(fp, "%ld ", &tempValue);
		t.slackVar.push_back(tempValue);
	}
	
	for (long col=0; col<numCols; col++)
	{
		bool tempValue;
		skipcomment(fp);
		fscanf(fp, "%ld ", &tempValue);
		t.surplusVar.push_back(tempValue);
	}

	for (long col=0; col<numCols; col++)
	{
		bool tempValue;
		skipcomment(fp);
		fscanf(fp, "%ld ", &tempValue);
		t.binaryVar.push_back(tempValue);

		t.forcedBinaryVar.push_back(false);
		t.valueForcedIntoBinaryVar.push_back(-1.0);

		if (tempValue)
		{
			numBinaryVariables++;
		}
	}

	for (long col=0; col<numCols; col++)
	{
		bool tempValue;
		skipcomment(fp);
		fscanf(fp, "%ld ", &tempValue);
		t.integerVar.push_back(tempValue);
		
		if (tempValue)
		{
			numIntegerVariables++;
		}
	}

	//set problem type
	if (numIntegerVariables == 0 && numBinaryVariables == numOriginalVariables)
	{
		t.variableTypeConstraint = PURE_BIP;		
	}
	else if (numIntegerVariables == 0 && numBinaryVariables > 0)
	{
		t.variableTypeConstraint = MIXED_BIP;
	}
	else if (numIntegerVariables == numOriginalVariables && numBinaryVariables == 0)
	{
		t.variableTypeConstraint = PURE_IP;
	}
	else if (numIntegerVariables > 0 && numBinaryVariables == 0)
	{
		t.variableTypeConstraint = MIXED_IP;
	}
	else if (numIntegerVariables + numBinaryVariables == numOriginalVariables)
	{
		t.variableTypeConstraint = PURE_BIP_IP;
	}
	else if (numIntegerVariables > 0 && numBinaryVariables > 0)
	{
		t.variableTypeConstraint = MIXED_BIP_IP;
	}
	else
	{
		t.variableTypeConstraint = NONE;
	}

	if (t.variableTypeConstraint==PURE_BIP || 
		t.variableTypeConstraint==PURE_IP ||
		t.variableTypeConstraint==PURE_BIP_IP)
	{
		t.problemHasOnlyPureConstraints = true;
	}
	else
	{
		t.problemHasOnlyPureConstraints = false;
	}

	t.tableau.ResetMatrixSize(numRows, numCols);

	//read tableau
	for (long row=0; row<numRows; row++)
	{
		for (long col=0; col<numCols; col++)
		{
			double value;
			skipcomment(fp);
			fscanf(fp, "%lf ", &value);
			t.tableau(row, col) = value;
		}
	}

	//set initial tableau
	t.initialTableau = t.tableau;

	//if problem is two phase then read the second phase objective function
	if (t.problemIsTwoPhase)
	{
		for (long col=0; col<numCols; col++)
		{
			double value;
			skipcomment(fp);
			fscanf(fp, "%lf ", &value);
			t.phase2ObjectiveFunction.push_back(value);
		}
	}

	//store basic and non-basic variables
	long cnt=1;

	for (long col=0; col<numCols; col++)
	{
		if (t.slackVar[col] || t.artificialVar[col])
		{
			t.rowOfBasicVar.push_back(cnt);
			cnt++;
		}
		else
		{
			t.rowOfBasicVar.push_back(0);
		}
	}

	return;
}

int main(void)
{
	bool foundSolution=false;
	Simplex t;

	InitializeData(t);

	if (t.variableTypeConstraint == NONE)
	{
		if (t.problemIsTwoPhase)
		{
			foundSolution = t.SolveRelaxed2PhaseProblemFormulation();
		}
		else
		{
			foundSolution = t.SolveProblemFormulationUsingSimplexAlgorithm();
		}

		if (foundSolution)
		{
			t.PrintGoodNews();
		}
		else
		{
			t.PrintBadNews();
		}
	}
	else
	{
		BranchAndBound branchAndBound;
		branchAndBound.t = t;

		//solve constrained variable type problem
		foundSolution = branchAndBound.SolveVariableTypeConstrainedProblemFormulation();

		if (foundSolution)
		{
			branchAndBound.t.PrintGoodNews();
		}
		else
		{
			branchAndBound.t.PrintBadNews();
		}
	}

	return 0;
}
