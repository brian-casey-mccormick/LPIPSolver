#ifndef _Vector_hpp
#define _Vector_hpp

#include <stdio.h>
#include <math.h>
#include <assert.h>

class Vector
{
private:
	long size;
	double *value;

public:
	Vector();
	Vector(long sizeIn);
	Vector (const Vector &Vector);
	Vector& operator = (const Vector& A);
	~Vector();
	double &operator () (long element);
	void ZeroVector();
	void ResetVectorSize(long sizeIn);
	Vector operator + (Vector B);
	Vector operator - (Vector B);
	Vector operator * (Vector y);
	Vector operator * (double y);
	long GetSize() {return size;};
	void PrintVector(FILE *fp);
};

#endif
