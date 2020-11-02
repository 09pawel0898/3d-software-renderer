#ifndef TRIANGLE_H	
#define TRIANGLE_H

struct Triangle;

#include "matrix4x4.hpp"

struct Triangle
{

	Vec3f v[3];								/* 3 triangle points */
	Vec3f normal;							/* triangles normal vector */
	sf::Color color = sf::Color::White;		/* default color */
};
#endif


