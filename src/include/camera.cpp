#include "camera.hpp"
#include "Context/input.hpp"




void Camera::onResize(std::array<i32, 2> const& dims) 
{
	proj_cfg.x = (float)(dims[0]) / (float)(dims[1]);
	recalcProj();
}


void Camera::onUpdate(f32 dt) 
{
	
}


void Camera::recalcProj()
{
	math::perspective(
		proj_cfg.x, 
		proj_cfg.y, 
		proj_cfg.z, 
		proj_cfg.w, 
		projection
	);
	math::inv_perspective(
		projection,
		inv_projection
	);
}


void Camera::recalcView()
{
	math::vec3f 	  tmp = position + direction;
	const math::vec3f up  = { 0.0f, 1.0f, 0.0f };

	math::lookAt(position, tmp, up, view);
	
}