#ifndef MATRIX4X4_H	
#define MATRIX4X4_H

class Matrix4x4;

#include "camera.hpp"

class Matrix4x4
{
public:
	float m[4][4] = { 0 };	// array that represents 4x4 matrix

	/* constructors/destructor */
	Matrix4x4();
	Matrix4x4(char rot, float alpha);
	~Matrix4x4();

	/* methods */
	void makeTranslation(Vec3f &trans);		/* use for translate point by 3d vector */
	void makeIdentity();					/* makes identity matrix */
	void makeXrotation(const float alpha);	/* default rotation around X axis */
	void makeYrotation(const float alpha);	/* default rotation around Y axis */
	void makeZrotation(const float alpha);	/* default rotation around Z axis */
};
#endif