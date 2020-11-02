#include "mesh.hpp"


Mesh::Mesh()
{
    position = { 0.0f,0.0f,16.0f };
}

Mesh::~Mesh()
{

}

/* loading object from file */
bool Mesh::loadObjFromFile(std::string filename)
{
    std::ifstream file("obj/" + filename);
    std::strstream ss;
    std::vector<Vec3f> vectors;
    Vec3f vec;

    Triangle triangle;

    char c, line[200];
    int attribute[3];

    if (!file.good()) return false;

    while (file.getline(line, 200))
    {
        ss << line;
        if (line[0] == 'v')
        {
            ss >> c >> vec.x >> vec.y >> vec.z;
            vectors.push_back(vec);
        }
        else if (line[0] == 'f')
        {
            ss >> c >> attribute[0] >> attribute[1] >> attribute[2];
            triangle.v[0] = vectors[attribute[0] - 1];
            triangle.v[1] = vectors[attribute[1] - 1];
            triangle.v[2] = vectors[attribute[2] - 1];

            tris.push_back(triangle);
            tris_counter++;
        }
        ss = std::strstream();
    }
    return true;

}



void Mesh::sortTriangles(std::vector<Triangle>& tris)
{
    sort(tris.begin(), tris.end(), [](Triangle& t1, Triangle& t2)
    {
        /* for selecting them im averaging its three z components and im using it for comparison */
        float z1 = (t1.v[0].z + t1.v[1].z + t1.v[2].z) / 3.0f;
        float z2 = (t2.v[0].z + t2.v[1].z + t2.v[2].z) / 3.0f;
        return z1 > z2;
    });
}

void Mesh::drawTriangle(Triangle &triangle, sf::RenderWindow* window, bool is_mesh_active, Camera& cam )
{
    /* drawing colored triangles or just a mesh depending on the choosen mode
    thanks to SFML for making it so easy :)
    */
    if (!is_mesh_active)
    {
        /* lighting */
        Vec3f light_dir = Vec3f(-cam.look_at.x,-cam.look_at.y,-cam.look_at.z);
        light_dir.normaliseVector();

        float dot_product = dotProduct(triangle.normal, light_dir);
        /* if dot produt < 0(angle is greater than 90), then i set triangle to be almost black */
        if (dot_product < 0) dot_product = 0.05f;
        else if (dot_product >= 0 && dot_product < 0.1f) dot_product = 0.1f;

        sf::Color color = sf::Color((int)(dot_product * 255), (int)(dot_product * 255), (int)(dot_product * 255));

        if(triangle.color == sf::Color::White)
            fillTriangle(window, color, triangle);
        else
            fillTriangle(window, triangle.color, triangle);
    }
    else
    {
        drawBorder(window, triangle);
    }
}


void Mesh::drawBorder(sf::RenderWindow* window,  Triangle& triangle )
{
    /* drawing triangle border */
    sf::VertexArray lines(sf::LinesStrip, 4);
    lines[0].position = sf::Vector2f(triangle.v[0].x, triangle.v[0].y);
    lines[1].position = sf::Vector2f(triangle.v[1].x, triangle.v[1].y);
    lines[2].position = sf::Vector2f(triangle.v[2].x, triangle.v[2].y);
    lines[3].position = sf::Vector2f(triangle.v[0].x, triangle.v[0].y);

    window->draw(lines);
}

void Mesh::fillTriangle(sf::RenderWindow* window, sf::Color color, Triangle& input_triangle)
{
    /* drawing filled triangles*/
    sf::ConvexShape triangle;
    triangle.setPointCount(3);
    triangle.setPoint(0, sf::Vector2f(input_triangle.v[0].x, input_triangle.v[0].y));
    triangle.setPoint(1, sf::Vector2f(input_triangle.v[1].x, input_triangle.v[1].y));
    triangle.setPoint(2, sf::Vector2f(input_triangle.v[2].x, input_triangle.v[2].y));
    triangle.setFillColor(color);

    window->draw(triangle);
}

/* nice looking rotation matrix proposed by javidx9 */
void Mesh::setZXRotationMatrix(const float dt)
{

    static float alpha;

    alpha += 1.0f * dt;

    //rotation z matrix
    rotZ.m[0][0] = cosf(alpha);
    rotZ.m[0][1] = sinf(alpha);
    rotZ.m[1][0] = -sinf(alpha);
    rotZ.m[1][1] = cosf(alpha);
    rotZ.m[2][2] = 1;
    rotZ.m[3][3] = 1;

    //rotation x matrix
    rotX.m[0][0] = 1;
    rotX.m[1][1] = cosf(alpha * 0.5f);
    rotX.m[1][2] = sinf(alpha * 0.5f);
    rotX.m[2][1] = -sinf(alpha * 0.5f);
    rotX.m[2][2] = cosf(alpha * 0.5f);
    rotX.m[3][3] = 1;
}


/* elementary rotation around X axis */
void Mesh::setXRotationMatrix(const float dt)
{
    static float alpha;
    alpha += 1.0f * dt;

    //rotation X matrix
    rotX.makeXrotation(alpha);
}
/* elementary rotation around Y axis */
void Mesh::setYRotationMatrix(const float dt)
{
    static float alpha;
    alpha += 1.0f * dt;

    //rotation Y matrix
    rotY.makeYrotation(alpha);
}

/* elementary rotation around Z axis */
void Mesh::setZRotationMatrix(const float dt)
{
    static float alpha;
    alpha += 1.0f * dt;

    //rotation Z matrix
    rotZ.makeZrotation(alpha);
}

void Mesh::updateRotation(int rotation_type, const float dt)
{
    if (rotation_type == 1) setZXRotationMatrix(dt);
    else if (rotation_type == 2) setXRotationMatrix(dt);
}

void Mesh::rotate(int rotation_type, Matrix4x4& matWorld, Matrix4x4 &matTrans)
{
    if (rotation_type == 0) /* no rotation, just changing position using camera */
        matWorld = multiplyMatrixByMatrix(matWorld, matTrans);
    else if (rotation_type == 1) // rotation - firstly Z axis, and then X axis
    {
        matWorld = multiplyMatrixByMatrix(rotZ, rotX);
        matWorld = multiplyMatrixByMatrix(matWorld, matTrans);
    }
    else if (rotation_type == 2) // rotation - only X axis
    {
        matWorld = multiplyMatrixByMatrix(matWorld, rotX);
        matWorld = multiplyMatrixByMatrix(matWorld, matTrans);
    }

}