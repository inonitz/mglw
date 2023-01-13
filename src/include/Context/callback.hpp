#pragma once




struct GLFWwindow;


namespace Renderer {


	void framebuffer_size_callback(
		GLFWwindow* window, 
		int 		width, 
		int 		height
	);


}


namespace Context::Input {
	
	
	void glfw_key_callback(
		GLFWwindow* window, 
		int 		key, 
		int 		scancode, 
		int 		action, 
		int 		mods
	); 


	void glfw_cursor_callback(
		GLFWwindow* window, 
		double 		xpos, 
		double 		ypos
	);


}