#include "operations.hpp"

/* calculate normal and returns true if the triangle ought to be visible, false otherwise */
bool calculateNormal(Triangle& triangle, const Camera& cam)
{
    sf::Vector3f sidea, sideb;
    static int c = 0;

    /* vector that represents first side of a triangle */
    sidea.x =  triangle.v[1].x -  triangle.v[0].x;
    sidea.y =  triangle.v[1].y -  triangle.v[0].y;
    sidea.z =  triangle.v[1].z -  triangle.v[0].z;

    /* vector that represents second side of a triangle */
    sideb.x =  triangle.v[2].x -  triangle.v[0].x;
    sideb.y =  triangle.v[2].y -  triangle.v[0].y;
    sideb.z =  triangle.v[2].z -  triangle.v[0].z;


    /* calculating normal vector */
     triangle.normal.x = sidea.y * sideb.z - sidea.z * sideb.y;
     triangle.normal.y = sidea.z * sideb.x - sidea.x * sideb.z;
     triangle.normal.z = sidea.x * sideb.y - sidea.y * sideb.x;


    /* and taking it into a unit vector */
     triangle.normal.normaliseVector();

    /* if Z component of normal vector in compare to the camera position allows triangle to be drawn then i can draw it into the screen */
    if (triangle.normal.x * (triangle.v[0].x - cam.position.x) + triangle.normal.y * (triangle.v[0].y - cam.position.y) + triangle.normal.z * (triangle.v[0].z - cam.position.z) < 0.0f)
        return true;
    else return false;
}

/* returns dot product of two vectors */
float dotProduct(const Vec3f& vec1, const Vec3f& vec2)
{
    return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

/* returns distance from plane to point in 3d space */
float getDistanceFromPlaneToPoint(const Vec3f &plane_point, Vec3f &plane_normal, const Vec3f &point)
{
    plane_normal.normaliseVector();
    return (plane_normal.x * point.x + plane_normal.y * point.y + plane_normal.z * point.z - dotProduct(plane_normal, plane_point));
}

/* returns a cross product of two vectors */
Vec3f crossProduct(const Vec3f& vec1, const Vec3f& vec2)
{
    Vec3f result;
    result.x = vec1.y * vec2.z - vec1.z * vec2.y;
    result.y = vec1.z * vec2.x - vec1.x * vec2.z;
    result.z = vec1.x * vec2.y - vec1.y * vec2.x;
    return result;
}

/* implementation of a multiplication of a matrix by a vector*/
Vec3f multiplyMatrixByVec(const Vec3f& input, const Matrix4x4& matrix)
{
    Vec3f output;
    /* i got 3 coordinate vector and a 4x4 matrix on the input, so what i need to do is to expand input vector by adding fourth element, and i can do it
    by simply adding 1 and beacause of this in the last operations i multiply an element of a matrix by 1 */
    output.x = input.x * matrix.m[0][0] + input.y * matrix.m[1][0] + input.z * matrix.m[2][0] + 1 * matrix.m[3][0];
    output.y = input.x * matrix.m[0][1] + input.y * matrix.m[1][1] + input.z * matrix.m[2][1] + 1 * matrix.m[3][1];
    output.z = input.x * matrix.m[0][2] + input.y * matrix.m[1][2] + input.z * matrix.m[2][2] + 1 * matrix.m[3][2];

    /* and i got one extra coordinate in the output that i will need just for dividing the others by it before projection */
    output.w = input.x * matrix.m[0][3] + input.y * matrix.m[1][3] + input.z * matrix.m[2][3] + matrix.m[3][3];

    return output;
}

/* returns a point of plane and line intersection */
Vec3f vectorIntersectPlane(Vec3f& line_start, Vec3f& line_end, Vec3f& plane_point, Vec3f& plane_normal)
{
    plane_normal.normaliseVector();
    float plane_dP = -dotProduct(plane_normal, plane_point);
    float a_dP = dotProduct(line_start, plane_normal);
    float b_dP = dotProduct(line_end, plane_normal);
    float t = (-plane_dP - a_dP) / (b_dP - a_dP);
    Vec3f line = line_end - line_start;
    Vec3f line_to_intersect = line * t;
    return line_start + line_to_intersect;
}

/* returns a matrix produced by matrix by matrix multiplication */
Matrix4x4 multiplyMatrixByMatrix(const Matrix4x4 &mat1, const Matrix4x4& mat2)
{
    Matrix4x4 result;
    for (unsigned int i = 0; i < 4; i++)
    {
        for (unsigned int j = 0; j < 4; j++)
        {
            result.m[i][j] = 0;
            for (unsigned int k = 0; k < 4; k++)
            {
                result.m[i][j] += mat1.m[i][k] * mat2.m[k][j];
            }
        }
    }
    return result;
}

/* returns a point at matrix depending on camera position */
Matrix4x4 pointAtMatrix(Vec3f& pos, Vec3f& forward, Vec3f& up)
{
    /* calculate new forward direction */
    Vec3f new_forward = forward - pos;
    new_forward.normaliseVector();

    /* calculate new up direction */
    Vec3f temp = new_forward * dotProduct(up, new_forward);
    Vec3f new_up = up - temp;
    new_up.normaliseVector();

    /* calculate new right direction */
    Vec3f new_right = crossProduct(new_up, new_forward);

    /* setting up point at matrix */
    Matrix4x4 mat;
    
    mat.m[0][0] = new_right.x;      mat.m[0][1] = new_right.y;      mat.m[0][2] = new_right.z;      mat.m[0][3] = 0;
    mat.m[1][0] = new_up.x;         mat.m[1][1] = new_up.y;         mat.m[1][2] = new_up.z;         mat.m[1][3] = 0;
    mat.m[2][0] = new_forward.x;    mat.m[2][1] = new_forward.y;    mat.m[2][2] = new_forward.z;    mat.m[2][3] = 0;
    mat.m[3][0] = pos.x;            mat.m[3][1] = pos.y;            mat.m[3][2] = pos.z;            mat.m[3][3] = 1;

    return mat;
}

/* returns inversed instance of pointAtMatrix */
Matrix4x4 inversePointAtMatrix(const Matrix4x4& input)
{
    Matrix4x4 mat;
    /* notice that this code works only for this specific case of matrix(pointAtMatrix) */
    mat.m[0][0] = input.m[0][0];    mat.m[0][1] = input.m[1][0];    mat.m[0][2] = input.m[2][0];    mat.m[0][3] = 0;
    mat.m[1][0] = input.m[0][1];    mat.m[1][1] = input.m[1][1];    mat.m[1][2] = input.m[2][1];    mat.m[1][3] = 0;
    mat.m[2][0] = input.m[0][2];    mat.m[2][1] = input.m[1][2];    mat.m[2][2] = input.m[2][2];    mat.m[2][3] = 0;

    mat.m[3][0] = -(input.m[3][0] * mat.m[0][0] + input.m[3][1] * mat.m[1][0] + input.m[3][2] * mat.m[2][0]);
    mat.m[3][1] = -(input.m[3][0] * mat.m[0][1] + input.m[3][1] * mat.m[1][1] + input.m[3][2] * mat.m[2][1]);
    mat.m[3][2] = -(input.m[3][0] * mat.m[0][2] + input.m[3][1] * mat.m[1][2] + input.m[3][2] * mat.m[2][2]);
    mat.m[3][3] = 1.0f;

    return mat;
}

/* returns a triangle translated by a matrix*/
Triangle translateTriangleByMatrix(const Triangle& tri, const Matrix4x4& matrix)
{
    Triangle result;

    /* multiplying every vertex of the triangle by a matrix */
    result.v[0] = multiplyMatrixByVec(tri.v[0], matrix);
    result.v[0].divideVecByW();
    result.v[1] = multiplyMatrixByVec(tri.v[1], matrix);
    result.v[1].divideVecByW();
    result.v[2] = multiplyMatrixByVec(tri.v[2], matrix);
    result.v[2].divideVecByW();

    return result;
}

/* clips triangle against given plane point and its normal, returns number od triangles produced from clipping */
int triangleClip(Vec3f plane_point, Vec3f plane_normal, Triangle& input, Triangle& out1, Triangle& out2, unsigned& clipped_tris_counter, bool clip_hint)
{
    plane_normal.normaliseVector();

    Vec3f* inside_points[3];
    Vec3f* outside_points[3];
    unsigned inside_points_counter = 0;
    unsigned outside_points_counter = 0;
    float point_distance[3]{ 0 };

    for (unsigned i = 0; i < 3; i++)
        point_distance[i] = getDistanceFromPlaneToPoint(plane_point, plane_normal, input.v[i]);

    for (unsigned i = 0; i < 3; i++)
    {
        if (point_distance[i] >= 0)
            inside_points[inside_points_counter++] = &input.v[i];
        else outside_points[outside_points_counter++] = &input.v[i];
    }

    /* if all points are behind the field of wiev, i reject the whole triangle
    (no valid triangles returned)*/
    if (inside_points_counter == 0) return 0;
    else if (inside_points_counter == 1 && outside_points_counter == 2)
    {
        out1.v[0] = *inside_points[0];

        out1.v[1] = vectorIntersectPlane(*inside_points[0], *outside_points[0], plane_point, plane_normal);
        out1.v[2] = vectorIntersectPlane(*inside_points[0], *outside_points[1], plane_point, plane_normal);
        if (clip_hint) out1.color = sf::Color::Red;
        clipped_tris_counter++;
        return 1;
    }
    else if (inside_points_counter == 2 && outside_points_counter == 1)
    {
        out1.v[0] = *inside_points[0];
        out1.v[1] = *inside_points[1];
        out1.v[2] = vectorIntersectPlane(*inside_points[0], *outside_points[0], plane_point, plane_normal);
        if (clip_hint) out1.color = sf::Color::Yellow;

        out2.v[0] = *inside_points[1];
        out2.v[1] = out1.v[2];
        out2.v[2] = vectorIntersectPlane(*inside_points[1], *outside_points[0], plane_point, plane_normal);
        if (clip_hint) out2.color = sf::Color::Green;
        clipped_tris_counter += 2;
        return 2;
    }
    else if (inside_points_counter == 3)
    {
        out1 = input;
        return 1;
    }

}