#ifndef CAMERA_H
#define CAMERA_H

class Camera;

#include "vector.hpp"
#include "operations.hpp"

/*
*
*
*		TO DO !!!
*
*
*/

class Camera
{

public: 
	Vec3f position;			// position in world space
	Vec3f look_at;			// camera look at vector
	float y_yaw , x_yaw ;	// x and y deviation

	/* constructor/destructor */
	Camera();
	~Camera();

	/* functions */
	Matrix4x4 translateByVec2d(sf::Vector2i& center, float delta_time);		/* TO FIX */
	Matrix4x4 translateByVec2d2(sf::Vector2i& center, float delta_time);	/* TO FIX */
	Matrix4x4 rotateCamera(sf::Vector2i &window_center, const float dt);	/* returns translation matrix that represents camera rotation */
	float getVec2Lenght(sf::Vector2f& vec);									/* returns 2d vector lenght */
	float absf(float number);												/* returns absolute value */

};
#endif