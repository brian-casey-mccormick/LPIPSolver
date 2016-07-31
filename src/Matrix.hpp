#ifndef _Matrix_hpp
#define _Matrix_hpp

#include <stdio.h>
#include "Vector.hpp"

class Matrix 
{
private:
	long numRows;
	long numCols;
	double **val;

public:
	Matrix();
	Matrix(long numRowsIn, long numColsIn);
	Matrix(const Matrix &matrix);
	Matrix& operator = (const Matrix& A);
	~Matrix();

	void ZeroMatrix();
	void IdentityMatrix();
	void ResetMatrixSize(long numRowsIn, long numColsIn);

	Matrix operator + (Matrix B);
	Matrix operator - (Matrix B);

	double &operator () (long row, long col);

	Matrix operator * (Matrix y);
	Vector operator * (Vector y);
	Matrix operator * (double y);
	Matrix operator ~ ();

	long GetNumRows() {return numRows;}
	long GetNumCols() {return numCols;}

	void PrintMatrix();
	void PrintMatrix(FILE *fp);
};

#endif
