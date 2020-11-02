#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "triangle.hpp"

/* calculate normal and returns true if the triangle ought to be visible, false otherwise */
bool calculateNormal(Triangle& triangleTranslated, const Camera& cam);

/* returns dot product of two vectors */
float dotProduct(const Vec3f& vec1, const Vec3f& vec2);

/* returns distance from plane to point in 3d space */
float getDistanceFromPlaneToPoint(const Vec3f& plane_point, Vec3f& plane_normal, const Vec3f& point);

/* returns a cross product of two vectors */
Vec3f crossProduct(const Vec3f& vec1, const Vec3f& vec2);

/* returns a vector multiplied by a matrix */
Vec3f multiplyMatrixByVec(const Vec3f& input, const Matrix4x4& matrix);

/* returns a point of plane and line intersection */
Vec3f vectorIntersectPlane(Vec3f& line_start, Vec3f& line_end, Vec3f& plane_point, Vec3f& plane_normal);

/* returns a matrix produced by matrix by matrix multiplication */
Matrix4x4 multiplyMatrixByMatrix(const Matrix4x4& mat1, const Matrix4x4& mat2);

/* returns a point at matrix depending on camera position */
Matrix4x4 pointAtMatrix(Vec3f& pos, Vec3f& forward, Vec3f& up);

/* returns inversed instance of pointAtMatrix */
Matrix4x4 inversePointAtMatrix(const Matrix4x4& input);

/* returns a triangle translated by a matrix*/
Triangle translateTriangleByMatrix(const Triangle& tri, const Matrix4x4& matrix);

/* clips triangle against given plane point and its normal, returns number od triangles produced from clipping */
int triangleClip(Vec3f plane_point, Vec3f plane_normal, Triangle& input, Triangle& out1, Triangle& out2, unsigned& clipped_tris_counter, bool clip_hint);

#endif