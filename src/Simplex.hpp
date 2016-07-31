#ifndef _Simplex_hpp
#define _Simplex_hpp

#include <vector>
#include "Matrix.hpp"

using namespace std;

enum VariableTypeConstraint
{
	NONE=0,            //no variable constraints
	PURE_BIP=1,        //pure binary problem
	MIXED_BIP=2,       //mixed binary problem
	PURE_IP=3,         //pure integer problem
	MIXED_IP=4,        //mixed integer problem
	PURE_BIP_IP=5,     //pure binary and integer problem
	MIXED_BIP_IP=6,    //mixed binary and integer problem
};

static char *VariableTypeConstraintToString[7] = 
{
	"NONE",
	"PURE_BIP",
	"MIXED_BIP",
	"PURE_IP",
	"MIXED_IP",
	"PURE_BIP_IP",
	"MIXED_BIP_IP"
};

class Simplex
{
public:

	bool problemIsTwoPhase;
	bool problemHasOnlyPureConstraints;
	VariableTypeConstraint variableTypeConstraint;
	vector<bool> binaryVar;
	vector<bool> integerVar;
	vector<bool> artificialVar;
	vector<bool> slackVar;
	vector<bool> surplusVar;
	vector<bool> forcedBinaryVar;
	vector<double> valueForcedIntoBinaryVar;
	vector<long> rowOfBasicVar;
	vector<double> phase2ObjectiveFunction;
	Matrix tableau;
	Matrix initialTableau;

	Simplex()
	{ 
		problemIsTwoPhase = false;
		problemHasOnlyPureConstraints = false;
		variableTypeConstraint = NONE;
		binaryVar.clear();
		integerVar.clear();
		artificialVar.clear();
		slackVar.clear();
		surplusVar.clear();
		forcedBinaryVar.clear();
		valueForcedIntoBinaryVar.clear();
		rowOfBasicVar.clear();
		phase2ObjectiveFunction.clear();
	}

	Simplex(const Simplex &simplex)
	{
		long i;

		problemIsTwoPhase = simplex.problemIsTwoPhase;
		problemHasOnlyPureConstraints = simplex.problemHasOnlyPureConstraints;
		variableTypeConstraint = simplex.variableTypeConstraint;

		for (i=0; i<simplex.binaryVar.size(); i++)
		{
			binaryVar.push_back(simplex.binaryVar[i]);
		}

		for (i=0; i<simplex.integerVar.size(); i++)
		{
			integerVar.push_back(simplex.integerVar[i]);
		}

		for (i=0; i<simplex.artificialVar.size(); i++)
		{
			artificialVar.push_back(simplex.artificialVar[i]);
		}

		for (i=0; i<simplex.slackVar.size(); i++)
		{
			slackVar.push_back(simplex.slackVar[i]);
		}

		for (i=0; i<simplex.surplusVar.size(); i++)
		{
			surplusVar.push_back(simplex.surplusVar[i]);
		}

		for (i=0; i<simplex.forcedBinaryVar.size(); i++)
		{
			forcedBinaryVar.push_back(simplex.forcedBinaryVar[i]);
		}

		for (i=0; i<simplex.valueForcedIntoBinaryVar.size(); i++)
		{
			valueForcedIntoBinaryVar.push_back(simplex.valueForcedIntoBinaryVar[i]);
		}
		
		for (i=0; i<simplex.rowOfBasicVar.size(); i++)
		{
			rowOfBasicVar.push_back(simplex.rowOfBasicVar[i]);
		}
		
		for (i=0; i<simplex.phase2ObjectiveFunction.size(); i++)
		{
			phase2ObjectiveFunction.push_back(simplex.phase2ObjectiveFunction[i]);
		}

		tableau = simplex.tableau;
		initialTableau = simplex.initialTableau;
	}

	Simplex& operator = (const Simplex &simplexIn)
	{
		if (this != &simplexIn) 
		{
			long i;

			problemIsTwoPhase = simplexIn.problemIsTwoPhase;
			problemHasOnlyPureConstraints = simplexIn.problemHasOnlyPureConstraints;
			variableTypeConstraint = simplexIn.variableTypeConstraint;

			//clear the old data out
			binaryVar.clear();
			integerVar.clear();
			artificialVar.clear();
			slackVar.clear();
			surplusVar.clear();
			forcedBinaryVar.clear();
			valueForcedIntoBinaryVar.clear();
			rowOfBasicVar.clear();
			phase2ObjectiveFunction.clear();

			//store the new data
			for (i=0; i<simplexIn.binaryVar.size(); i++)
			{
				binaryVar.push_back(simplexIn.binaryVar[i]);
			}

			for (i=0; i<simplexIn.integerVar.size(); i++)
			{
				integerVar.push_back(simplexIn.integerVar[i]);
			}

			for (i=0; i<simplexIn.artificialVar.size(); i++)
			{
				artificialVar.push_back(simplexIn.artificialVar[i]);
			}

			for (i=0; i<simplexIn.slackVar.size(); i++)
			{
				slackVar.push_back(simplexIn.slackVar[i]);
			}

			for (i=0; i<simplexIn.surplusVar.size(); i++)
			{
				surplusVar.push_back(simplexIn.surplusVar[i]);
			}

			for (i=0; i<simplexIn.forcedBinaryVar.size(); i++)
			{
				forcedBinaryVar.push_back(simplexIn.forcedBinaryVar[i]);
			}

			for (i=0; i<simplexIn.valueForcedIntoBinaryVar.size(); i++)
			{
				valueForcedIntoBinaryVar.push_back(simplexIn.valueForcedIntoBinaryVar[i]);
			}

			for (i=0; i<simplexIn.rowOfBasicVar.size(); i++)
			{
				rowOfBasicVar.push_back(simplexIn.rowOfBasicVar[i]);
			}

			for (i=0; i<simplexIn.phase2ObjectiveFunction.size(); i++)
			{
				phase2ObjectiveFunction.push_back(simplexIn.phase2ObjectiveFunction[i]);
			}

			tableau = simplexIn.tableau;
			initialTableau = simplexIn.initialTableau;
		}

		return *this;
	}

	~Simplex()
	{
		binaryVar.clear();
		integerVar.clear();
		artificialVar.clear();
		slackVar.clear();
		surplusVar.clear();
		forcedBinaryVar.clear();
		valueForcedIntoBinaryVar.clear();
		rowOfBasicVar.clear();
		phase2ObjectiveFunction.clear();
	}

	long Signum(double value);
	double GetSolutionForBranching();
	double GetSolution() {return tableau(0, 0) * tableau(0, tableau.GetNumCols()-1);}
	
	void PrintTableauData();
	void PrintTableauDataToFile(FILE *fp);
	void PrintTableau();
	void PrintGoodNews();
	void PrintBadNews();

	void AddLessThanConstraint(long col, double value);
	void AddEqualToConstraint(long col, double value);
	
	bool TableauStillValidAfterAddingConstraint();
	bool DetermineIfVariableMeetsVariableTypeConstraint(long row);
	bool DetermineIfSolutionMeetsVariableTypeConstraints();
	bool CurrentTableauIsOptimalForSimplexAlgorithm();
	bool CurrentTableauIsOptimalForDualSimplexAlgorithm();
	bool DeterminePivotRowAndPivotColumnForSimplexAlgorithm(long &pivotRow, long &pivotCol);
	bool DeterminePivotRowAndPivotColumnForDualSimplexAlgorithm(long &pivotRow, long &pivotCol);
	void ExchangePivotRowAndPivotColumn(long pivotRow, long pivotCol);
	void InsertSecondPhaseObjectiveFunctionIntoTableau();
	void ConvertTableauToProperForm();
	bool SolveProblemFormulationUsingDualSimplexAlgorithm();
	bool SolveProblemFormulationUsingSimplexAlgorithm();
	bool SolveRelaxed2PhaseProblemFormulation();
};

#endif
