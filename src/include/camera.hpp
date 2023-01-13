#pragma once
#include "math/vec.hpp"




class Camera 
{
private:
	math::vec3f position;
	math::vec3f direction;
	math::vec4f proj_cfg; /* window aspect ratio (w/h), vertical fov, near clip, far clip */

	math::mat4f projection;
	math::mat4f inv_projection;
	math::mat4f view;
	math::mat4f inv_view;


	void recalcProj();
	void recalcView();


public:
	Camera(
		float aspectRatio, 
		float verticalFOV, 
		float nearClip, 
		float farClip
	) : proj_cfg{aspectRatio, verticalFOV, nearClip, farClip} 
	{
		position  = { 0.0f, 0.0f,  3.0f };
		direction = { 0.0f, 0.0f, -1.0f };
	}

	
	void onUpdate(float dt);
	void onResize(std::array<i32, 2> const& dims); 
};