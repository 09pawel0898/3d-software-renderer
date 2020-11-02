#include "camera.hpp"

Camera::Camera()
{
	position.x = 0;
	position.y = 0;
	position.z = 0;
    x_yaw = 0.0f;
    y_yaw = 0.0f;
    look_at = { 0,0,1 };
}

Camera::~Camera()
{

}

Matrix4x4 Camera::translateByVec2d(sf::Vector2i& center, float delta_time)
{
    sf::Vector2f temp = { (float)(sf::Mouse::getPosition().x - center.x),(float)(sf::Mouse::getPosition().y - center.y) };
    static unsigned iter = 0;
    float modx = 0.01f, mody = 0.01f;
    
    Matrix4x4 matCameraRotY, matCameraRotX, matCameraRotYX;

    if (getVec2Lenght(temp) < 2.0f)
        sf::Mouse::setPosition(center);

    if (sf::Mouse::getPosition() == center)
    {
        matCameraRotY.makeYrotation(y_yaw);
        matCameraRotX.makeXrotation(x_yaw);
        matCameraRotYX = multiplyMatrixByMatrix(matCameraRotX, matCameraRotY);

        return matCameraRotYX;
    }
   
    //float fFovRad = 1.0f / tanf(fFieldOfView * 0.5f / 180.0f * 3.14159f);

    modx = temp.y / 80.0f;
    mody = -temp.x / 80.0f;
    y_yaw -= (mody * delta_time);
    x_yaw -= (modx * delta_time);

    if (x_yaw > 3.6f) x_yaw = 0.0f;
    if (y_yaw > 3.6f) y_yaw = 0.0f;

    matCameraRotY.makeYrotation(y_yaw);
    matCameraRotX.makeXrotation(x_yaw);
    matCameraRotYX = multiplyMatrixByMatrix(matCameraRotX, matCameraRotY);

    if (iter % 8 == 0)
        sf::Mouse::setPosition(sf::Vector2i((int)(center.x + temp.x / 2.2f), (int)(center.y + temp.y / 2.2f)));

    iter++;
    return matCameraRotYX;

}

Matrix4x4 Camera::rotateCamera(sf::Vector2i &window_center, const float dt)
{
    Matrix4x4 matCameraRotYX;
    matCameraRotYX.makeIdentity();
    matCameraRotYX = multiplyMatrixByMatrix(translateByVec2d(window_center, dt), matCameraRotYX);
    return matCameraRotYX;
}

float Camera::getVec2Lenght(sf::Vector2f& vec)
{
    return sqrt(vec.x * vec.x + vec.y * vec.y);
}

float Camera::absf(float number)
{
    if (number >= 0) return number;
    else return -number;
}