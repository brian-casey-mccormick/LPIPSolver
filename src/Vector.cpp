#include <stdio.h>
#include <assert.h>

#include "Vector.hpp"

Vector::Vector()
{
	size = 3;
	value = new double [3];
	assert(value);
}

Vector::Vector(long sizeIn = 3)
{
	size = sizeIn;
	value = new double [size];
	assert(value);
}

Vector::Vector(const Vector &vector) 
{
	long i;
	size = vector.size;

	value = new double [vector.size];
	assert(value);

	for (i=0;i<size;i++) 
	{
		value[i] = vector.value[i];
	}
}

Vector& Vector::operator = (const Vector& A)
{
	if (this != &A ) 
	{	
		if (size != A.size)
		{
			ResetVectorSize(A.size);
		}
		
		for (long i=0;i<size;i++) 
		{	
			value[i] = A.value[i];
		}
	}

	return *this;
}

Vector::~Vector()
{
	if (value) 
	{
		delete [] value;
	}
}

double &Vector::operator () (long element) 
{
	return value[element];
}

void Vector::ZeroVector()
{
	long i;

	for (i=0;i<size;i++) 
	{
		value[i] = 0.0;
	}

	return;
}

void Vector::ResetVectorSize(long sizeIn)
{
	size = sizeIn;

	if (value)
	{
		delete [] value;
	}

	value = new double [size];
	assert(value);

	return;
}

Vector Vector::operator + (Vector B)
{
	long i;

	assert(B.value);
	assert(value);	
	assert(size == B.size);

	Vector C(size);

	for (i=0;i<size;i++) 
	{
		C.value[i] = value[i] + B.value[i];		
	}

	return C;
}

Vector Vector::operator - (Vector B) 
{
	long i;

	assert(B.value);
	assert(value);	
	assert(size == B.size);

	Vector C(size);
	
	for (i=0;i<size;i++) 
	{
		C.value[i] = value[i] - B.value[i];		
	}

	return C;
}

Vector Vector::operator * (Vector y)
{
	assert(size == y.size);

	Vector z(size);

	z.value[0] = value[1] * y.value[2] - value[2] * y.value[1];
	z.value[1] = value[2] * y.value[0] - value[0] * y.value[2];
	z.value[2] = value[0] * y.value[1] - value[1] * y.value[0];
	
	return z;
}

Vector Vector::operator * (double y)
{
	long i;

	Vector z(size);

	for (i=0;i<size;i++) 
	{
		z.value[i] = y * value[i];
	}

	return z;
}
