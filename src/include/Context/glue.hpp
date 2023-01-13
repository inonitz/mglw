#pragma once




struct State;
struct GLContext;
struct GLFWwindow;


using framebuffer_callback = void (*)(GLFWwindow* window, int    width, int    height);
using key_callback         = void (*)(GLFWwindow* window, int    key,   int    scancode, int action, int mods);
using cursor_callback      = void (*)(GLFWwindow* window, double xpos,  double ypos);

struct callbackFuncTable {
	framebuffer_callback framebuffer_size; 
	key_callback 	     keyboard; 
	cursor_callback      cursor_position;
};




namespace __internal {


State& 	           global();
GLContext&         gl();
GLFWwindow*        underlyingWinHandle();
callbackFuncTable& callbackTable();


}