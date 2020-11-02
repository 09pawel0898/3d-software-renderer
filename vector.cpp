#include "vector.hpp"


Vec3f::~Vec3f()
{
}

void Vec3f::printVecToConsole(void)
{
    std::cout << x << " " << y << " " << z << "\n";
}

void Vec3f::normaliseVector(void)
{
    float lenght = this->getVecLenght();
    x /= lenght;
    y /= lenght;
    z /= lenght;
}

void Vec3f::divideVecByW(void)
{
    /* coming to 2d from 3d coordinates, after this dividing i have appropriate X and Y values to draw points to the 2d screen :)
    (i am not going to use Z component) */
    if (this->w != 0.0f)
    {
        x /= w;
        y /= w;
        z /= w;
    }

}

float Vec3f::getVecLenght(void)
{
    return sqrt(x * x + y * y + z * z);
}

