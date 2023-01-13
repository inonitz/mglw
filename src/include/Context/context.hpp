#pragma once
#include "../base.hpp"




namespace Context {
	void overrideDefaultCallbacks(
		void* frambuffer_callback, 
		void* keyboard_callback, 
		void* cursor_position_callback
	);


	void init(bool useGLContext, i32 maxWindowWidth, i32 maxWindowHeight);
	void terminate();


	f64  deltaTime();
	f64  avgDelta();
	bool active();
}