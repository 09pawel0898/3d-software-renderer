#ifndef VECTOR_H
#define VECTOR_H




class Vec3f;

#include "directives.hpp"

class Vec3f : public sf::Vector3f
{
public:
	float w = 0.0f;	//fourth vector value that is produced from matrix multiplication

	/* constructors/destructor */
	Vec3f() : sf::Vector3f() {};
	Vec3f(float xx, float yy, float zz) : sf::Vector3f(xx, yy, zz) {};
	~Vec3f();

	/* functinons */
	void printVecToConsole(void);		/* shows vector components to the console*/
	void normaliseVector(void);			/* overrides this vector by its normalised version */
	void divideVecByW(void);			/* divides x,y,z components by w value that i receive after matrix multiplication (to get appropriate values to draw on 2d screen) */
	float getVecLenght(void);			/* returns vector lenght */

	/* operators */
	Vec3f operator + (const Vec3f& right){	return Vec3f(this->x + right.x, this->y + right.y, this->z + right.z); }
	Vec3f operator - (const Vec3f& right) { return Vec3f(this->x - right.x, this->y - right.y, this->z - right.z); }
	Vec3f operator * (const float right) { return Vec3f(this->x * right, this->y * right, this->z * right); }
	Vec3f operator / (const float right) { return Vec3f(this->x / right, this->y / right, this->z / right); }


};

#endif