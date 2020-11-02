#include "matrix4x4.hpp"

Matrix4x4::Matrix4x4()
{

}

Matrix4x4::Matrix4x4(char rot,float alpha)
{ 
	rot = tolower(rot);
	switch (rot)
	{
		case 'x': this->makeXrotation(alpha); break;
		case 'y': this->makeYrotation(alpha); break;
		case 'z': this->makeZrotation(alpha); break;
	}
}

Matrix4x4::~Matrix4x4()
{

}

void Matrix4x4::makeTranslation(Vec3f &trans)
{
	makeIdentity();
	m[3][0] = trans.x;
	m[3][1] = trans.y;
	m[3][2] = trans.z;
}

void Matrix4x4::makeIdentity()
{
	m[0][0] = 1.0f;
	m[1][1] = 1.0f;
	m[2][2] = 1.0f;
	m[3][3] = 1.0f;
}

void Matrix4x4::makeXrotation(const float alpha)
{
	m[0][0] = 1;
	m[1][1] = cosf(alpha);
	m[1][2] = -sinf(alpha);
	m[2][1] = sinf(alpha);
	m[2][2] = cosf(alpha);

}

void Matrix4x4::makeYrotation(const float alpha)
{
	m[0][0] = cosf(alpha);
	m[0][2] = sinf(alpha);
	m[1][1] = 1;
	m[2][0] = -sinf(alpha);
	m[2][2] = cosf(alpha);


}

void Matrix4x4::makeZrotation(const float alpha)
{
	m[0][0] = cosf(alpha);
	m[0][1] = -sinf(alpha);
	m[1][0] = sinf(alpha);
	m[1][1] = cosf(alpha);
	m[2][2] = 1;
}