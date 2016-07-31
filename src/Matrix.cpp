#include <stdio.h>
#include <assert.h>
#include "Vector.hpp"
#include "Matrix.hpp"

Matrix::Matrix()
{
	long i;

	numRows = 3;
	numCols = 3;

	val = new double* [numRows];
	assert(val);

	// allocate rows and set pointers to them
	val[0] = new double [numRows * numCols];
	assert(val[0]);

	for (i=1;i<numRows;i++) val[i] = val[i-1] + numCols;
}

Matrix::Matrix(long numRowsIn = 3, long numColsIn = 3)
{
	long i;

	numRows = numRowsIn;
	numCols = numColsIn;

	val = new double* [numRows];
	assert(val);

	// allocate rows and set pointers to them
	val[0] = new double [numRows * numCols];
	assert(val[0]);

	for (i=1;i<numRows;i++) val[i] = val[i-1] + numCols;
}

Matrix::Matrix(const Matrix &matrix) 
{
	long i, j;

	numRows = matrix.numRows;
	numCols = matrix.numCols;

	val = new double* [matrix.numRows];
	assert(val);

	// allocate rows and set pointers to them
	val[0] = new double [matrix.numRows * matrix.numCols];
	assert(val[0]);

	for (i=1;i<matrix.numRows;i++) val[i] = val[i-1] + numCols;

	for (i=0;i<numRows;i++) 
	{
		for (j=0;j<numCols;j++) 
		{
			val[i][j] = matrix.val[i][j];
		}
	}
}

Matrix& Matrix::operator = (const Matrix& A)
{
	if (this != &A) 
	{	
		if (numRows != A.numRows || numCols != A.numCols)
		{
			ResetMatrixSize(A.numRows, A.numCols);
		}
			
		for (long i=0;i<numRows;i++) 
		{	
			for (long j=0;j<numCols;j++) 
			{	
				val[i][j] = A.val[i][j];	
			}
		}
	}

	return *this;
}

Matrix::~Matrix()
{
	if (val)
	{
		delete [] val[0];
		delete [] val;
	}
}

void Matrix::ZeroMatrix()
{
	long i, j;

	for (i=0;i<numRows;i++) 
	{
		for (j=0;j<numCols;j++) 
		{
			val[i][j] = 0.0;
		}
	}

	return;
}

void Matrix::IdentityMatrix()
{
	long i, j;
	assert(numRows == numCols);
	
	for (i=0;i<numRows;i++) 
	{
		for (j=0;j<numRows;j++) 
		{
			if (i==j) val[i][j] = 1.0;
			else val[i][j] = 0.0;
		}
	}

	return;
}

void Matrix::ResetMatrixSize(long numRowsIn, long numColsIn)
{
	long i;

	numRows = numRowsIn;
	numCols = numColsIn;

	if (val)
	{
		delete val[0];
		delete val;
	}

	val = new double* [numRows];
	assert(val);

	// allocate rows and set pointers to them
	val[0] = new double [numRows * numCols];
	assert(val[0]);

	for (i=1;i<numRows;i++) val[i] = val[i-1] + numCols;

	return;
}

Matrix Matrix::operator + (Matrix B) 
{
	long i, j;
	assert(B.val);
	assert(val);	
	assert(numCols == B.numCols && numRows == B.numRows);

	Matrix C(numRows,numCols);

	for (i=0;i<numRows;i++) 
	{
		for (j=0;j<numCols;j++) 
		{
			C.val[i][j] = val[i][j] + B.val[i][j];	
		}
	}

	return C;
}

Matrix Matrix::operator - (Matrix B)
{
	long i, j;
	assert(B.val);
	assert(val);
	assert(numCols == B.numCols && numRows == B.numRows);
	Matrix C(numRows,numCols);

	for (i=0;i<numRows;i++) 
	{
		for (j=0;j<numCols;j++) 
		{
			C.val[i][j] = val[i][j] - B.val[i][j];	
		}
	}

	return C;
}

double &Matrix::operator () (long row, long col) 
{
	return val[row][col];
}

Matrix Matrix::operator * (Matrix y)
{
	long i,j,k;
	assert(numCols == y.numRows);
	Matrix z(numRows, y.numCols);

	for (i=0;i<numRows;i++) 
	{
		for (k=0;k<y.numCols;k++) 
		{
			z.val[i][k]=0;
			for (j=0;j<numCols;j++) 
			{
				if((val[i][j] != 0) && (y.val[j][k] != 0)) 
				{
					z.val[i][k] += val[i][j] * y.val[j][k];
				}
			}
		}
	}

	return z;
}

Vector Matrix::operator * (Vector y)
{
	long i, j;
	assert(numCols == y.GetSize());
	Vector z(numRows);

	for (i=0;i<numRows;i++) 
	{
		z(i) = 0;

		for (j=0;j<numCols;j++) 
		{
			if((val[i][j] != 0) && (y(j) != 0)) 
			{
				z(i) = z(i) + val[i][j] * y(j);
			}
		}
	}

	return z;
}

Matrix Matrix::operator * (double y)
{
	long i, j;
	Matrix z(numRows, numCols);

	for (i=0;i<numRows;i++) 
	{
		for (j=0;j<numCols;j++) 
		{
			z.val[i][j] = y * val[i][j];
		}
	}

	return z;
}

Matrix Matrix::operator ~ ()
{
    long i,j;
    Matrix transpose(numCols, numRows);
    
    for (i=0;i<numCols;i++)
    {
        for (j=0;j<numRows;j++) 
        {
            transpose(i,j) = val[j][i];
        }
    }
    
    return transpose;
}

void Matrix::PrintMatrix()
{
	long i, j;

	for (i=0;i<numRows;i++) 
	{
		for (j=0;j<numCols;j++) 
		{
			printf("%5.1lf ", val[i][j]);
		}

		printf("\n");
	}

	printf("\n");

	return;
}

void Matrix::PrintMatrix(FILE *fp)
{
	long i, j;

	for (i=0;i<numRows;i++) 
	{
		for (j=0;j<numCols;j++) 
		{
			fprintf(fp, "%5.1lf ", val[i][j]);
		}

		fprintf(fp, "\n");
	}

	fprintf(fp, "\n");

	return;
}
