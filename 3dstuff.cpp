#include "3dstuff.hpp"

/* initializer functions */
#pragma region Initialization

void Stuff3d::initTexts()
{
    font.loadFromFile("OpenSans-Light.ttf");
    info[0].setFont(font);
    info[0].setCharacterSize(25);
    info[0].setFillColor(sf::Color::White);
    info[0].setPosition(10, 10);
    info[0].setString(" E - change rotation | Space - color | Q - Clipping visible");
    info[1].setFont(font);
    info[1].setCharacterSize(25);
    info[1].setFillColor(sf::Color::White);
    info[1].setPosition(1000, 40);
    info[3].setFont(font);
    info[3].setCharacterSize(25);
    info[3].setFillColor(sf::Color::White);
    info[3].setPosition(1000, 10);
    info[4].setFont(font);
    info[4].setCharacterSize(25);
    info[4].setFillColor(sf::Color::White);
    info[4].setPosition(1000, 70);
    info[5].setFont(font);
    info[5].setCharacterSize(25);
    info[5].setFillColor(sf::Color::White);
    info[5].setPosition(1000, 100);
    info[5].setString("Mesh tris : " + std::to_string(objectMesh->tris_counter));
}

void Stuff3d::initProjectionMat()
{
    /* declaring essential variables and initializing projection matrix */
    float fNear = 0.1f;
    float fFar = 1000.0f;
    float fFieldOfView = 90.0f;
    float fAspectRatio = (float)HEIGHT / (float)WIDTH;
    float fFovRad = 1.0f / tanf(fFieldOfView * 0.5f / 180.0f * 3.14159f);

    projectionMat.m[0][0] = fAspectRatio * fFovRad;
    projectionMat.m[1][1] = fFovRad;
    projectionMat.m[2][2] = fFar / (fFar - fNear);
    projectionMat.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    projectionMat.m[2][3] = 1.0f;
    projectionMat.m[3][3] = 0.0f;
}
#pragma endregion Initialization

#pragma region Render/Update


/* updating */
void Stuff3d::update(void)
{
    mouse_pos = sf::Mouse::getPosition(*window);
    updateEvents();
}
#pragma endregion Render/Update

#pragma region Drawing/Clipping

/* drawing mesh  */
void Stuff3d::drawMesh(Vec3f &position)
{
    std::vector<Triangle> tris_to_clip;         // vector to store triangles created after first clipping, ready for the second clipping
    Matrix4x4 matWorld, matTrans, matCameraRot; // necessary matrices
    unsigned clipped_tris_counter = 0;          // counter of triangles created by clipping

    matWorld.makeIdentity();
    matTrans.makeTranslation(position);         //translation to position given by an argument
    
    /* camera set up */
    Vec3f v_target = { 0,0,1 };
    matCameraRot = cam.rotateCamera(window_center,delta_time);
    cam.look_at = multiplyMatrixByVec(v_target, matCameraRot);
    v_target = cam.position + cam.look_at;
    Matrix4x4 matCamera = pointAtMatrix(cam.position, v_target, vUp);
    Matrix4x4 matView = inversePointAtMatrix(matCamera);

    /* rotating mesh */
    objectMesh->rotate(rotation_type,matWorld, matTrans);
    
    for (auto i : this->objectMesh->tris)
    {
        
        Triangle triangleProjected, triangleTranslated, triangleViewed; // declaring triangles that represents single state

        /* triangle translated in world space */
        triangleTranslated = translateTriangleByMatrix(i, matWorld);

        /* creating normals and optimizing displaying process */
        if (calculateNormal(triangleTranslated, cam))
        {
            /* conversion from world space to viewed triangle */
            triangleViewed = translateTriangleByMatrix(triangleTranslated, matView);

            /* clipping against the fNear plane, i store newly created polygons in a vector */
            clipAgainstNearPlane(tris_to_clip, triangleProjected, triangleViewed, triangleTranslated, clipped_tris_counter);
            
        }
    }
    
    /* sorting triangles from the farthest to the closest by averaged Z component*/
    objectMesh->sortTriangles(tris_to_clip);

    /* and second clipping - now triangles are in 2d so i just clip against window edges */
    unsigned int drawed_polygons = 0;

    for (auto& i : tris_to_clip)
    {
        std::list<Triangle> tris_to_project;
        
        /* adding initial triangle */
        tris_to_project.push_back(i);

        /* clipping */
        clipAgainstWindowEdges(tris_to_project, i,  clipped_tris_counter);

        /* drawing every triangle created from clipping from single triangle from tris_to_clip */
        for (auto& i : tris_to_project)
        {
            drawed_polygons++;
            objectMesh->drawTriangle(i, window, is_mesh_active, cam);
        }
    }

    /* update info sentences */
    info[4].setString("Clipped tris : " + std::to_string(clipped_tris_counter));
    info[3].setString("Drawed polygons : " + std::to_string(drawed_polygons));
}

void Stuff3d::scaleTriangleIntoScreen(Triangle& triangle)
{
    Vec3f shift_view = { 1,1,0 };

    triangle.v[0].x *= -1.0f;
    triangle.v[1].x *= -1.0f;
    triangle.v[2].x *= -1.0f;
    triangle.v[0].y *= -1.0f;
    triangle.v[1].y *= -1.0f;
    triangle.v[2].y *= -1.0f;
    triangle.v[0] = triangle.v[0] + shift_view;
    triangle.v[1] = triangle.v[1] + shift_view;
    triangle.v[2] = triangle.v[2] + shift_view;
    triangle.v[0].x *= 0.5f * WIDTH;
    triangle.v[0].y *= 0.5f * HEIGHT;
    triangle.v[1].x *= 0.5f * WIDTH;
    triangle.v[1].y *= 0.5f * HEIGHT;
    triangle.v[2].x *= 0.5f * WIDTH;
    triangle.v[2].y *= 0.5f * HEIGHT;


}

void Stuff3d::clipAgainstNearPlane(std::vector<Triangle>& tris_to_clip, Triangle& triangleProjected, Triangle& triangleViewed, Triangle& triangleTranslated, unsigned& clipped_tris_counter)
{
    int clipped_triangles = 0;
    Triangle new_clipped[2];
    clipped_triangles = triangleClip({ 0.0f,0.0f,0.1f }, { 0.0f,0.0f,1.0f }, triangleViewed, new_clipped[0], new_clipped[1], clipped_tris_counter, clip_hint);


    for (int i = 0; i < clipped_triangles; i++)
    {
        triangleProjected = translateTriangleByMatrix(new_clipped[i], projectionMat);
        triangleProjected.normal = triangleTranslated.normal;
        triangleProjected.color = triangleViewed.color;

        /* after doing matrix transformations i got float x,y,z values of vectors in range between 0 and 1, before drawing i have to scale
        it properly to my window */
        scaleTriangleIntoScreen(triangleProjected);

        /* adding selected triangles to the vector */
        tris_to_clip.push_back(triangleProjected);

    }
}
void Stuff3d::clipAgainstWindowEdges(std::list<Triangle>& tris_to_project, Triangle& current_triangle,  unsigned& clipped_tris_counter)
{
    Triangle new_clipped[2];
    int new_triangles = 1;

    for (unsigned side = 0; side < 4; side++)
    {
        int tris_to_add = 0;
        while (new_triangles > 0)
        {
            Triangle temp = tris_to_project.front();
            tris_to_project.pop_front();
            new_triangles--;

            switch (side)
            {
            case 0: tris_to_add = triangleClip({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, temp, new_clipped[0], new_clipped[1], clipped_tris_counter, clip_hint);
                break;
            case 1: tris_to_add = triangleClip({ 0.0f, HEIGHT + 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, temp, new_clipped[0], new_clipped[1], clipped_tris_counter, clip_hint);
                break;
            case 2: tris_to_add = triangleClip({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, temp, new_clipped[0], new_clipped[1], clipped_tris_counter, clip_hint);
                break;
            case 3: tris_to_add = triangleClip({ WIDTH + 1, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, temp, new_clipped[0], new_clipped[1], clipped_tris_counter, clip_hint);
                break;
            }

            for (int i = 0; i < tris_to_add; i++)
            {
                new_clipped[i].normal = current_triangle.normal;
                tris_to_project.push_back(new_clipped[i]);
            }
        }
        new_triangles = tris_to_project.size();
    }
}
#pragma endregion Drawing/Clipping

#pragma region UserEvents

/* switching to the next rotaiton type */
void Stuff3d::nextRotationType(void)
{
    if (rotation_type < 2)
        rotation_type++;
    else rotation_type = 0;
}

/* events handling */
void Stuff3d::updateEvents(void)
{
    Vec3f moveForward = cam.look_at / 2.01f;
    Vec3f moveToSide = crossProduct(cam.look_at, vUp);

    while (window->pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed: window->close(); break;
        case sf::Event::KeyPressed:
            switch (event.key.code)
            {
                case sf::Keyboard::Escape: window->close(); break;
                case sf::Keyboard::Space: is_mesh_active = (is_mesh_active) ? false : true; break; 
                case sf::Keyboard::E: nextRotationType(); break;
                case sf::Keyboard::Q: clip_hint = (clip_hint) ? false : true;  break;
                case sf::Keyboard::W: cam.position += moveForward; break;
                case sf::Keyboard::S: cam.position -= moveForward; break;  
                case sf::Keyboard::D: cam.position += moveToSide; break;  
                case sf::Keyboard::A: cam.position -= moveToSide; break;  
                case sf::Keyboard::F: cam.x_yaw += 0.2f; break;  
                case sf::Keyboard::G: cam.x_yaw -= 0.2f; break;
                case sf::Keyboard::V: cam.y_yaw += 0.2f; break;
                case sf::Keyboard::B: cam.y_yaw -= 0.2f; break;
            }
        break;
        case sf::Event::MouseButtonPressed:
            mouse_button_pressed = true;
            click_mouse_pos = sf::Mouse::getPosition(*window);
        break;
        case sf::Event::MouseButtonReleased:
            mouse_button_pressed = false;
        break;

        }
    }
}
#pragma endregion UserEvents

#pragma region MainLoop/Constructor/Destructor

Stuff3d::Stuff3d(std::string filename)
{
    /* some initialization */
    objectMesh = new Mesh(0.0f, 0.0f, 16.0f);
    window = new sf::RenderWindow(sf::VideoMode(1280, 720, 32), "3dstuff");
    window->setMouseCursorVisible(false);
    window->setMouseCursorGrabbed(true);

    sf::Mouse::setPosition(sf::Vector2i(window->getPosition().x + (int)( WIDTH/2.0f ), window->getPosition().y + (int)(HEIGHT / 2.0f)));
    window_center = sf::Vector2i(window->getPosition().x + (int)(WIDTH / 2.0f), window->getPosition().y + (int)(HEIGHT / 2.0f));


    /* loading object from file */
    if (!objectMesh->loadObjFromFile(filename))
        std::cout << "Failed to load *.obj file";

    initTexts();
    initProjectionMat();

}

Stuff3d::~Stuff3d()
{
    delete window;
    delete objectMesh;
}

void Stuff3d::run(void)
{
    window->setActive(false);

    sf::Thread render(&renderingThread, this);
    render.launch();

    while (window->isOpen())
    {
        update();
    }
}
#pragma endregion MainLoop / Constructor / Destructor

/* rendering */
static void renderingThread(Stuff3d* app)
{
    // activate the window's context in render thread
    app->window->setActive(true);

    while (app->window->isOpen())
    {
        app->window->clear();
        //drawing info strings
        for (int i = 0; i < 6; i++)
            app->window->draw(app->info[i]);

        //debuging camera
        using std::cout;
        //cout << "Look at ->" << app->cam.look_at.x << " " << app->cam.look_at.y << " " << app->cam.look_at.z << std::endl;
        //cout << app->cam.position.x << " " << app->cam.position.y << " " << app->cam.position.z << std::endl;
        //cout << "cam.pos(" << app->cam.position.x << "," << app->cam.position.y << "," << app->cam.position.z << ")  cam.lookat(" << app->cam.look_at.x << "," << app->cam.look_at.y << "," << app->cam.look_at.z;
        //std::cout <<" xyaw, yaw :"<< app->cam.x_yaw << " " << app->cam.y_yaw << std::endl;
        
        /* updating delta time and increasing frame number */
        app->delta_time = app->clockdt.restart().asSeconds();
        app->frame_nr++;

        //std::cout << app->delta_time << std::endl;

        if (app->frame_nr % 30 == 0)
            app->info[1].setString("FPS : " + std::to_string(1.0f / app->delta_time));
        app->window->setTitle(std::to_string(1.0f / app->delta_time));
        
        /* updating rotation and drawing mesh */
        app->objectMesh->updateRotation(app->rotation_type, app->delta_time);
        app->drawMesh(app->objectMesh->position);

        app->window->display();

    }
}