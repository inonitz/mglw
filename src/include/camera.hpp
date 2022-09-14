#pragma once
#include "math/vec.hpp"




class Camera 
{
private:
	math::vec3f position;
	math::vec3f direction;
	math::vec3f proj_cfg; /* vertical fov, near clip, far clip */

	math::mat4f projection;
	math::mat4f inv_projection;
	math::mat4f view;
	math::mat4f inv_view;


	void recalcProj();
	void recalcView();

	class Window;
public:
	Camera(float verticalFOV, float nearClip, float farClip) : proj_cfg{verticalFOV, nearClip, farClip} 
	{
		position  = { 0.0f, 0.0f,  3.0f };
		direction = { 0.0f, 0.0f, -1.0f };
	}

	
	void update(Window const& win, float dt);
	void onResize(std::array<i32, 2> const& dims); 
};