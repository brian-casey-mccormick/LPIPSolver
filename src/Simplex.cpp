#include <stdio.h>
#include "Simplex.hpp"

long Simplex::Signum(double value)
{
	long valueOut;
	
	if (value > 0.0)
	{
		valueOut = 1;
	}
	else if (value < 0.0)
	{
		valueOut = -1;
	}
	else 
	{
		valueOut = 0;
	}

	return valueOut;
}

double Simplex::GetSolutionForBranching()
{
	double soln;

	if (problemHasOnlyPureConstraints == true)
	{
		long tempSoln;

		if (GetSolution() < 0.0)
		{
			tempSoln = (long)(GetSolution()-1.0);
			soln = (double)tempSoln;
		}
		else
		{
			tempSoln = (long)(GetSolution());
			soln = (double)tempSoln;
		}
	}
	else
	{
		soln = GetSolution();
	}

	return soln;
}

void Simplex::PrintTableauData()
{
	long i;
	
	printf("\n");
	printf("problemIsTwoPhase = %ld\n", problemIsTwoPhase);
	printf("problemHasOnlyPureConstraints = %ld\n", problemHasOnlyPureConstraints);
	printf("variableTypeConstraint = %s\n", VariableTypeConstraintToString[variableTypeConstraint]);

	printf("\nbinaryVar\n");

	for (i=0; i<binaryVar.size(); i++)
	{
		printf("%ld ", binaryVar[i]);
	}

	printf("\n");
	printf("\nintegerVar\n");

	for (i=0; i<integerVar.size(); i++)
	{
		printf("%ld ", integerVar[i]);
	}

	printf("\n");
	printf("\nartificialVar\n");

	for (i=0; i<artificialVar.size(); i++)
	{
		printf("%ld ", artificialVar[i]);
	}
	
	printf("\n");
	printf("\nslackVar\n");

	for (i=0; i<slackVar.size(); i++)
	{
		printf("%ld ", slackVar[i]);
	}

	printf("\n");
	printf("\nsurplusVar\n");

	for (i=0; i<surplusVar.size(); i++)
	{
		printf("%ld ", surplusVar[i]);
	}

	if (variableTypeConstraint == PURE_BIP || 
		variableTypeConstraint == MIXED_BIP ||
		variableTypeConstraint == PURE_BIP_IP ||
		variableTypeConstraint == MIXED_BIP_IP)
	{
		printf("\n");
		printf("\nforcedBinaryVar\n");

		for (i=0; i<forcedBinaryVar.size(); i++)
		{
			printf("%ld ", forcedBinaryVar[i]);
		}

		printf("\n");
		printf("\nvalueForcedIntoBinaryVar\n");

		for (i=0; i<valueForcedIntoBinaryVar.size(); i++)
		{
			printf("%4.1lf ", valueForcedIntoBinaryVar[i]);
		}
	}

	printf("\n");
	printf("\nrowOfBasicVar\n");

	for (i=0; i<rowOfBasicVar.size(); i++)
	{
		printf("%ld ", rowOfBasicVar[i]);
	}

	if (phase2ObjectiveFunction.size() > 0)
	{
		printf("\n");
		printf("\nphase 2 objective function\n");

		for (i=0; i<phase2ObjectiveFunction.size(); i++)
		{
			printf("%4.1lf ", phase2ObjectiveFunction[i]);
		}
	}

	printf("\n");
	printf("\nInitialTableau\n");
	initialTableau.PrintMatrix();

	printf("Tableau\n");
	tableau.PrintMatrix();

	printf("\n");

	return;
}

void Simplex::PrintTableauDataToFile(FILE *fp)
{
	long i;
	
	fprintf(fp, "\n");
	fprintf(fp, "problemIsTwoPhase = %ld\n", problemIsTwoPhase);
	fprintf(fp, "problemHasOnlyPureConstraints = %ld\n", problemHasOnlyPureConstraints);
	fprintf(fp, "variableTypeConstraint = %s\n", VariableTypeConstraintToString[variableTypeConstraint]);

	fprintf(fp, "\nbinaryVar\n");

	for (i=0; i<binaryVar.size(); i++)
	{
		fprintf(fp, "%ld ", binaryVar[i]);
	}

	fprintf(fp, "\n");
	fprintf(fp, "\nintegerVar\n");

	for (i=0; i<integerVar.size(); i++)
	{
		fprintf(fp, "%ld ", integerVar[i]);
	}

	fprintf(fp, "\n");
	fprintf(fp, "\nartificialVar\n");

	for (i=0; i<artificialVar.size(); i++)
	{
		fprintf(fp, "%ld ", artificialVar[i]);
	}
	
	fprintf(fp, "\n");
	fprintf(fp, "\nslackVar\n");

	for (i=0; i<slackVar.size(); i++)
	{
		fprintf(fp, "%ld ", slackVar[i]);
	}

	fprintf(fp, "\n");
	fprintf(fp, "\nsurplusVar\n");

	for (i=0; i<surplusVar.size(); i++)
	{
		fprintf(fp, "%ld ", surplusVar[i]);
	}

	if (variableTypeConstraint == PURE_BIP || 
		variableTypeConstraint == MIXED_BIP ||
		variableTypeConstraint == PURE_BIP_IP ||
		variableTypeConstraint == MIXED_BIP_IP)
	{
		fprintf(fp, "\n");
		fprintf(fp, "\nforcedBinaryVar\n");

		for (i=0; i<forcedBinaryVar.size(); i++)
		{
			fprintf(fp, "%ld ", forcedBinaryVar[i]);
		}

		fprintf(fp, "\n");
		fprintf(fp, "\nvalueForcedIntoBinaryVar\n");

		for (i=0; i<valueForcedIntoBinaryVar.size(); i++)
		{
			fprintf(fp, "%4.1lf ", valueForcedIntoBinaryVar[i]);
		}
	}

	fprintf(fp, "\n");
	fprintf(fp, "\nrowOfBasicVar\n");

	for (i=0; i<rowOfBasicVar.size(); i++)
	{
		fprintf(fp, "%ld ", rowOfBasicVar[i]);
	}

	if (phase2ObjectiveFunction.size() > 0)
	{
		fprintf(fp, "\n");
		fprintf(fp, "\nphase 2 objective function\n");

		for (i=0; i<phase2ObjectiveFunction.size(); i++)
		{
			fprintf(fp, "%4.1lf ", phase2ObjectiveFunction[i]);
		}
	}

	fprintf(fp, "\n");
	fprintf(fp, "\nInitialTableau\n");
	initialTableau.PrintMatrix(fp);

	fprintf(fp, "Tableau\n");
	tableau.PrintMatrix(fp);

	fprintf(fp, "\n");

	return;
}

void Simplex::PrintTableau()
{
	long numRows = tableau.GetNumRows();
	long numCols = tableau.GetNumCols();

	for (long row=0; row<numRows; row++)
	{
		for (long col=0; col<numCols; col++)
		{
			printf("%7.1lf", tableau(row, col));
		}

		printf("\n");
	}

	printf("\n");
}

void Simplex::PrintGoodNews()
{
	long numCols = tableau.GetNumCols();

	printf("\n");
	printf("Final Tableau\n");
	PrintTableau();

	printf("Optimal Objective Function Value = %lf\n", GetSolution());

	for (long col=1; col<numCols-1; col++)
	{
		long row = rowOfBasicVar[col];
		
		if (row > 0)
		{
			printf("X%d = %lf\n", col, tableau(row, numCols-1));
		}
		else
		{
			printf("X%d = 0.000000\n", col);
		}
	}

	printf("\n");

	return;
}

void Simplex::PrintBadNews()
{
	printf("solution not found\n");
	printf("\n");
	return;
}

void Simplex::AddLessThanConstraint(long col, double value)
{
	long i, j, cnt;
	long numRows = tableau.GetNumRows();
	long numCols = tableau.GetNumCols();

	//create new slack variable
	cnt=0;
	vector<bool>::iterator p = slackVar.begin();

	while (p != slackVar.end())
	{
		if (cnt==numCols-1)
		{
			slackVar.insert(p, true);
			break;
		}

		p++;
		cnt++;
	}

	//create new basic variable
	cnt=0;
	vector<long>::iterator q = rowOfBasicVar.begin();

	while (q != rowOfBasicVar.end())
	{
		if (cnt==numCols-1)
		{
			rowOfBasicVar.insert(q, numRows);
			break;
		}

		q++;
		cnt++;
	}

	//update rest of model accordingly
	binaryVar.push_back(false);
	integerVar.push_back(false);
	surplusVar.push_back(false);
	forcedBinaryVar.push_back(false);
	valueForcedIntoBinaryVar.push_back(-1.0);
	artificialVar.push_back(false);

	//update tableau
	numRows = tableau.GetNumRows() + 1;
	numCols = tableau.GetNumCols() + 1;
	Matrix tableauTemp(numRows, numCols);

	for (i=0; i<numRows-1; i++)
	{
		for (j=0; j<numCols-2; j++)
		{
			tableauTemp(i, j) = tableau(i, j);
		}
	}

	for (i=0; i<numRows-1; i++)
	{
		tableauTemp(i, numCols-2) = 0.0;
		tableauTemp(i, numCols-1) = tableau(i, numCols-2);
	}

	for (j=0; j<numCols-1; j++)
	{
		if (j==col || j==numCols-2)
		{
			if (value >= 0)
			{
				tableauTemp(numRows-1, j) = 1.0;
			}
			else
			{
				tableauTemp(numRows-1, j) = -1.0;
			}
		}
		else
		{
			tableauTemp(numRows-1, j) = 0.0;
		}
	}

	//set last tableau value and save temp matrix to tableau's matrix
	tableauTemp(numRows-1, numCols-1) = value;
	tableau = tableauTemp;

	return;
}

void Simplex::AddEqualToConstraint(long col, double value)
{
	long i, j, cnt;
	long numRows = tableau.GetNumRows();
	long numCols = tableau.GetNumCols();

	//create new artificial variable
	cnt=0;
	vector<bool>::iterator p = artificialVar.begin();

	while (p != artificialVar.end())
	{
		if (cnt==numCols-1)
		{
			artificialVar.insert(p, true);
			break;
		}

		p++;
		cnt++;
	}

	//update forced binary variable at column and add new one at end of list
	forcedBinaryVar[col] = true;
	forcedBinaryVar.push_back(false);
	
	//update value at forced binary variable location at column and add new one at end of list
	valueForcedIntoBinaryVar[col] = value;
	valueForcedIntoBinaryVar.push_back(-1.0);

	//create new basic variable row
	cnt=0;
	vector<long>::iterator q = rowOfBasicVar.begin();

	while (q != rowOfBasicVar.end())
	{
		if (cnt==numCols-1)
		{
			rowOfBasicVar.insert(q, numRows);
			break;
		}

		q++;
		cnt++;
	}

	//update rest of model accordingly
	surplusVar.push_back(false);
	slackVar.push_back(false);
	binaryVar.push_back(false);
	integerVar.push_back(false);

	//update tableau
	numRows = tableau.GetNumRows() + 1;
	numCols = tableau.GetNumCols() + 1;
	Matrix tableauTemp(numRows, numCols);

	for (i=0; i<numRows-1; i++)
	{
		for (j=0; j<numCols-2; j++)
		{
			tableauTemp(i, j) = tableau(i, j);
		}
	}

	//go through and set zero for the coefficients of the new variable for now
	for (i=0; i<numRows-1; i++)
	{
		tableauTemp(i, numCols-2) = 0.0;
		tableauTemp(i, numCols-1) = tableau(i, numCols-2);
	}

	//go through and set variable coefficients on newly created row
	for (j=0; j<numCols-1; j++)
	{
		if (j==col || j==numCols-2)
		{
			tableauTemp(numRows-1, j) = 1.0;
		}
		else
		{
			tableauTemp(numRows-1, j) = 0.0;
		}
	}

	//set last tableau value and save temp matrix to tableau's matrix
	tableauTemp(numRows-1, numCols-1) = value;
	tableau = tableauTemp;

	return;
}

bool Simplex::TableauStillValidAfterAddingConstraint()
{
	if (variableTypeConstraint == PURE_BIP || 
		variableTypeConstraint == MIXED_BIP ||
		variableTypeConstraint == PURE_BIP_IP ||
		variableTypeConstraint == MIXED_BIP_IP)
	{
		long numRows = initialTableau.GetNumRows();
		long numCols = initialTableau.GetNumCols();

		for (long row=1; row<numRows; row++)
		{
			double temp = initialTableau(row, numCols-1);
		
			//compute 'b' matrix for new tableau
			for (long col=1; col<numCols-1; col++)
			{
				if (forcedBinaryVar[col] == true)
				{
					temp -= valueForcedIntoBinaryVar[col] * initialTableau(row, col);
				}
			}

			if (temp < 0.0)
			{
				return false;
			}
		}
	}

	return true;
}

bool Simplex::DetermineIfVariableMeetsVariableTypeConstraint(long row)
{
	long numCols = tableau.GetNumCols();
	double zeta = fabs((long)(tableau(row, numCols-1)) - tableau(row, numCols-1));

	//adjust zeta if necessary
	if (zeta > 0.5)
	{
		zeta = 1.0 - zeta;
	}

	if (zeta < 1e-10)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Simplex::DetermineIfSolutionMeetsVariableTypeConstraints()
{
	long numCols = tableau.GetNumCols();

	for (long col=1; col<numCols-1; col++)
	{
		long row = rowOfBasicVar[col];

		if (row > 0)
		{
			if (integerVar[col] || binaryVar[col])
			{
				bool constraintsMet = DetermineIfVariableMeetsVariableTypeConstraint(row);
				
				if (constraintsMet == false)
				{
					return false;
				}
			}
		}
	}

	return true;
}

bool Simplex::CurrentTableauIsOptimalForSimplexAlgorithm()
{
	long numCols = tableau.GetNumCols();

	for (long col=1; col<numCols-1; col++)
	{
		if (tableau(0, col) < -1e-10)
		{
			return false;
		}
	}

	return true;
}

bool Simplex::CurrentTableauIsOptimalForDualSimplexAlgorithm()
{
	long numRows = tableau.GetNumRows();
	long numCols = tableau.GetNumCols();

	for (long row=1; row<numRows; row++)
	{
		if (tableau(row, numCols-1) < -1e-10)
		{
			return false;
		}
	}

	return true;
}

bool Simplex::DeterminePivotRowAndPivotColumnForSimplexAlgorithm(long &pivotRow, long &pivotCol)
{
	bool solutionNotBounded=true; 
	double maxNegative=-1e10;
	double minRatio=1e10;
	long numRows = tableau.GetNumRows();
	long numCols = tableau.GetNumCols();

	for (long col=1; col<numCols-1; col++)
	{
		if (tableau(0, col) < -1e-10)
		{
			if (fabs(tableau(0, col)) > maxNegative)
			{
				maxNegative = fabs(tableau(0, col));
				pivotCol = col;
			}
		}
	}

	for (long row=1; row<numRows; row++)
	{
		if (tableau(row, pivotCol) > 1e-10)
		{
			double ratio = tableau(row, numCols-1)/tableau(row, pivotCol);
			
			if (ratio < minRatio)
			{
				minRatio = ratio;
				pivotRow = row;
				solutionNotBounded = false;
			}
		}
	}

	return solutionNotBounded;
}

bool Simplex::DeterminePivotRowAndPivotColumnForDualSimplexAlgorithm(long &pivotRow, long &pivotCol)
{
	bool solutionNotBounded=true; 
	double maxNegative=-1e10;
	double minRatio=1e10;
	long numRows = tableau.GetNumRows();
	long numCols = tableau.GetNumCols();

	for (long row=1; row<numRows; row++)
	{
		if (tableau(row, numCols-1) < 0.0)
		{
			if (fabs(tableau(row, numCols-1)) > maxNegative)
			{
				maxNegative = fabs(tableau(row, numCols-1));
				pivotRow = row;
			}
		}
	}

	for (long col=1; col<numCols-1; col++)
	{
		if (rowOfBasicVar[col] == 0)
		{
			if (tableau(pivotRow, col) < 0.0)
			{
				double ratio = fabs(tableau(0, col)/tableau(pivotRow, col));
				
				if (ratio < minRatio)
				{
					minRatio = ratio;
					pivotCol = col;
					solutionNotBounded = false;
				}
			}
		}
	}

	return solutionNotBounded;
}

void Simplex::ExchangePivotRowAndPivotColumn(long pivotRow, long pivotCol)
{
	double pivotValue = tableau(pivotRow, pivotCol);
	long numCols = tableau.GetNumCols();
	long numRows = tableau.GetNumRows();

	//normalize pivot row
	for (long col=0; col<numCols; col++)
	{
		tableau(pivotRow, col)/=pivotValue;
	}

	//perform elementary row operations
	for (long row=0; row<numRows; row++)
	{
		if (row!=pivotRow)
		{
			double temp = tableau(row, pivotCol);
			double absTemp = fabs(temp);
			long signum = Signum(temp);
			
			if (signum == -1)
			{
				for (long col=1; col<numCols; col++)
				{
					tableau(row, col) += absTemp * tableau(pivotRow, col);
				}
			}
			else if (signum == 1)
			{
				for (long col=1; col<numCols; col++)
				{
					tableau(row, col) -= temp * tableau(pivotRow, col);
				}
			}
		}
	}

	return;
}

void Simplex::InsertSecondPhaseObjectiveFunctionIntoTableau()
{
	long numCols = tableau.GetNumCols();
	long numRows = tableau.GetNumRows();

	//insert the second phase objective function
	for (long col=0; col<numCols; col++)
	{
		tableau(0, col) = phase2ObjectiveFunction[col];

		// zero all elements below first row for the artificial variable columns
		if (artificialVar[col])
		{
			for (long row=1; row<numRows; row++)
			{
				tableau(row, col) = 0.0;
			}
		}
	}

	return;
}

void Simplex::ConvertTableauToProperForm()
{
	long numRows = tableau.GetNumRows();
	long numCols = tableau.GetNumCols();

	//convert columns that are current basic variables
	for (long col=1; col<numCols-1; col++)
	{
		long row = rowOfBasicVar[col];

		if (row>0)
		{
			for (long innerRow=0; innerRow<numRows; innerRow++)
			{
				if (row != innerRow)
				{
					double initialCoefficient = tableau(innerRow, col);
					double absTemp = fabs(initialCoefficient);
					long signum = Signum(initialCoefficient);

					if (signum == -1)
					{
						for (long innerCol=1; innerCol<numCols; innerCol++)
						{
							tableau(innerRow, innerCol) += absTemp * tableau(row, innerCol);
						}
					}
					else if (signum == 1)
					{
						for (long innerCol=1; innerCol<numCols; innerCol++)
						{
							tableau(innerRow, innerCol) -= initialCoefficient * tableau(row, innerCol);
						}
					}
				}
			}			
		}
	}

	return;
}

bool Simplex::SolveProblemFormulationUsingSimplexAlgorithm()
{
	long pivotRow, pivotCol;
	long numCols = tableau.GetNumCols();

	//make sure initial tableau is in the proper form
	ConvertTableauToProperForm();

	while (1)
	{		
		if (CurrentTableauIsOptimalForSimplexAlgorithm())
		{
			return true;
		}
		
		bool solutionNotBounded = DeterminePivotRowAndPivotColumnForSimplexAlgorithm(pivotRow, pivotCol);

		if (solutionNotBounded)
		{
			return false;
		}

		ExchangePivotRowAndPivotColumn(pivotRow, pivotCol);
		
		//update the basic and non-basic variable lists
		long nonBasicColumn=-1;

		for (long col=1; col<numCols; col++)
		{
			if (rowOfBasicVar[col] == pivotRow)
			{
				nonBasicColumn=col;
				break;
			}
		}

		if (nonBasicColumn > 0)
		{
			rowOfBasicVar[nonBasicColumn] = 0;
			rowOfBasicVar[pivotCol] = pivotRow;
		}
		else
		{
			printf("did not find the pivot row of the entering basic variable\n");
			assert(0);
		}
	}
}

bool Simplex::SolveProblemFormulationUsingDualSimplexAlgorithm()
{
	long pivotRow, pivotCol;
	long numRows = tableau.GetNumRows();
	long numCols = tableau.GetNumCols();

	//make sure initial tableau is in the proper form
	ConvertTableauToProperForm();

	//convert last constraint to dual simplex form if needed
	if (tableau(numRows-1, numCols-1) > 0.0)
	{
		for (long col=1; col<numCols; col++)
		{
			tableau(numRows-1, col) *= -1.0;
		}
	}

	while (1)
	{
		if (CurrentTableauIsOptimalForDualSimplexAlgorithm())
		{
			return true;
		}
		
		bool solutionNotBounded = DeterminePivotRowAndPivotColumnForDualSimplexAlgorithm(pivotRow, pivotCol);

		if (solutionNotBounded == true)
		{
			return false;
		}

		ExchangePivotRowAndPivotColumn(pivotRow, pivotCol);
		
		//update the basic and non-basic variable lists
		long nonBasicColumn=-1;
		
		for (long col=1; col<numCols; col++)
		{
			if (rowOfBasicVar[col] == pivotRow)
			{
				nonBasicColumn=col;
				break;
			}
		}

		if (nonBasicColumn > 0)
		{
			rowOfBasicVar[nonBasicColumn] = 0;
			rowOfBasicVar[pivotCol] = pivotRow;
		}
		else
		{
			printf("did not find the pivot row of the entering basic variable\n");
			assert(0);
		}
	}

	//check validity of final artificial variables before proceeding
	for (long col=0; col<numCols; col++)
	{
		if (artificialVar[col])
		{
			if (tableau(0, col)==0.0)
			{
				return false;
			}
		}
	}

	return true;
}

bool Simplex::SolveRelaxed2PhaseProblemFormulation()
{
	//make sure initial tableau is in the proper form
	ConvertTableauToProperForm();

	//find solution for phase 1
	bool foundFirstPhaseSolution = SolveProblemFormulationUsingSimplexAlgorithm();

	if (foundFirstPhaseSolution)
	{
		long numCols = tableau.GetNumCols();

		//check validity of final artificial variables before proceeding
		for (long col=0; col<numCols; col++)
		{
			if (artificialVar[col])
			{
				if (tableau(0, col)==0.0)
				{
					return false;
				}
			}
		}
	}
	else
	{
		return false;
	}

	//find solution for phase 2
	InsertSecondPhaseObjectiveFunctionIntoTableau();
	ConvertTableauToProperForm();
	bool foundSecondPhaseSolution = SolveProblemFormulationUsingSimplexAlgorithm();
	
	return foundSecondPhaseSolution;
}
