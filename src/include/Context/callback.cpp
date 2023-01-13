#include "callback.hpp"
#include "state.hpp"
#include "glue.hpp"
#include "GLFW/glfw3.h"
#include "input.hpp"
#include "render.hpp"




namespace Renderer {


	void framebuffer_size_callback(notused GLFWwindow* window, int width, int height)
	{
		width  = (width  % 2) + width;
		height = (height % 2) + height;
		__internal::global().dims = { width, height };
		__internal::gl().screenTex.recreateImage({
			width, height
		});
		__internal::gl().screenFb.attachTexture(
		{
			__internal::gl().screenTex.glid(),
			fboAttach::COLOR0, 
			0
		});
		
		
		// f32 desiredAspect = 2560.0f/1440.0f;
		// f32 nWidth = (f32)width, nHeight = (f32)height;
		
		// f32 cWidth = desiredAspect * nHeight;
		// f32 left   = 0.5 * (nWidth - cWidth);
		// glViewport((i32)left, 0, (i32)cWidth, height);
		glViewport(0, 0, width, height);
		return;
	}

	
}




namespace Context::Input {
	
	
	__unused void glfw_key_callback(
		notused GLFWwindow* window, 
		notused int 		key, 
		notused int 		scancode, 
		notused int 		action, 
		notused int 		mods
	) {
		/* 
			I dunno how to apply this yet in the whole program structure. 
			Will be used in later refactors. 
		*/
		return;
	}


	void glfw_cursor_callback(GLFWwindow *window, double xpos, double ypos)
	{	
		State* winHdl = (State*)glfwGetWindowUserPointer(window);
		winHdl->cursor = { xpos, ypos };
		return;
	}


}