#pragma once
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#define GLFW_INCLUDE_NONE
#include <utility>
#include <array>
#include "base.hpp"
#include "GLFW/glfw3.h"




struct Window {
private:
	GLFWwindow*        window;
	std::array<i32, 2> dims;
	std::array<f64, 2> cursor;
	f32                defaultCol[4];


	static void glfw_cursor_callback(GLFWwindow *window, double xpos, double ypos);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void glfw_error_callback(int error, const char* description);

public:
	Window() {}

	void create(i32 WindowWidth, i32 WindowHeight);
	void destroy();
	void beginFrame();
	void endFrame();


	__force_inline std::array<f64, 2> mousePos()     { return cursor; 					  	 }

	__force_inline bool               closed()       { return glfwWindowShouldClose(window); }
	__force_inline GLFWwindow* 		  nativeHandle() { return window; 		  			     }
	__force_inline std::array<i32, 2> dimensions()   { return dims; 			     		 }

};


/* static void framebuffer_size_callback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); } */
