#ifndef ENGINE_H
#define ENGINE_H

#include "mesh.hpp"

class Stuff3d;

/* rendering thread */
static void renderingThread(Stuff3d* app);

/* main app class */
class Stuff3d
{
private:

    /* variables */
    sf::RenderWindow* window;            // window
    sf::Event event;                     // sfml event
    sf::Clock clockdt;                   // delta time clock
    sf::Font font;                       // font
    sf::Text info[6];                    // info sentences
    sf::Vector2i mouse_pos;              // mouse position
    sf::Vector2i click_mouse_pos;        // last click mouse position
    sf::Vector2i window_center;          // position of the center of the window
    Camera cam;                          // camera 
    Mesh* objectMesh;                    // mesh loaded from file
    Matrix4x4 projectionMat;             // projection matrix
    Vec3f vUp = { 0,1,0 };               // vUp vector that i should declare somwhere else
    unsigned rotation_type = 0;          // object rotation type
    unsigned frame_nr = 0;               // frame counter
    float delta_time = 0.0f;             // delta time
    bool is_mesh_active = true;          // true - mesh / false - coloured triangles
    bool mouse_button_pressed = false;   // true - mouse pressed / false - mouse released 
    bool clip_hint = false;              // true - colouring clipped triangles / false - leaving them by default

    /* initialization functions */
    void initTexts();                                   /* init info sentences */
    void initProjectionMat();                           /* init projection matrix */

    /* functions */
    void update(void);                                  /* updating frame*/
    void drawMesh(Vec3f &position);                     /* drawing mesh */
    void scaleTriangleIntoScreen(Triangle& triangle);   /* scaling triangle vertexes from (0,1) to (0, windowSize) */
    void clipAgainstNearPlane(std::vector<Triangle>& tris_to_clip, Triangle& triangleProjected, Triangle& triangleViewed, Triangle &triangleTranslated, unsigned &clipped_tris_counter);    /* clipping against near plane */
    void clipAgainstWindowEdges(std::list<Triangle>& tris_to_project, Triangle& current_triangle,  unsigned &clipped_tris_counter);                                      /* clipping against window edges */
    void nextRotationType(void);                        /* switching to the next rotaiton type */   
    void updateEvents(void);                            /* events handling */

    friend static void renderingThread(Stuff3d* app);   /* friendly rendering thread */

public:

    /* constructor / destructor */
    Stuff3d(std::string filename);
    ~Stuff3d();

    /* app starting method */
    void run(void);     
   
};
#endif

