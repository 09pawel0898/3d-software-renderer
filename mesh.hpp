#ifndef MESH_H	
#define MESH_H

class Mesh;

#include "operations.hpp"


class Mesh
{

public:
    std::vector<Triangle> tris;     // vector for storing polygons 
    Matrix4x4 rotZ, rotX, rotY;     // rotation matrices 
    Vec3f position;                 // object position
    unsigned tris_counter = 0;      // triangles counter

    /* constructors/destructor */
    Mesh();
    Mesh(float x, float y, float z) { position.x = x; position.y = y; position.z = z; }                 /* overloaded constructor with position coords */
    ~Mesh();

    /* methods */
    bool loadObjFromFile(std::string filename);                                                         /* loading object from file*/
    void sortTriangles(std::vector<Triangle>& tris);                                                    /* sorting triangles by averaged Z value*/
    void drawTriangle(Triangle& triangle,sf::RenderWindow* window, bool is_mesh_active, Camera& cam );  /* drawing single triangle */
    void drawBorder(sf::RenderWindow* window, Triangle& triangle);                                      /* drawing a triangle border*/
    void fillTriangle(sf::RenderWindow* window, sf::Color color, Triangle& triangle);                   /* filling triangle with color */
    void setZXRotationMatrix(const float dt);                                                           /* rotating by nice rotation matrix proposed by javidx9 */
    void setXRotationMatrix(const float dt);                                                            /* elementary rotation around X axis */
    void setYRotationMatrix(const float dt);                                                            /* elementary rotation around Y axis */
    void setZRotationMatrix(const float dt);                                                            /* elementary rotation around Z axis */
    void updateRotation(int rotation_type, const float dt);                                             /* update rotation accordingly to the choosen type */
    void rotate(int rotation_type, Matrix4x4& matWorld, Matrix4x4& matTrans);                           /* updates matWorld by multiplying it by rotation and translation matrices */
};
#endif