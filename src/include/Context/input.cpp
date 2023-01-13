#include "input.hpp"
#include "state.hpp"
#include "GLFW/glfw3.h"
#include "glue.hpp"




namespace Context::Input {
	// __unused static void glfw_key_callback(
	// 	notused GLFWwindow* window, 
	// 	notused int 		key, 
	// 	notused int 		scancode, 
	// 	notused int 		action, 
	// 	notused int 		mods
	// ) {
	// 	/* 
	// 		I dunno how to apply this yet in the whole program structure. 
	// 		Will be used in later refactors. 
	// 	*/
	// 	return;
	// }


	// static void glfw_cursor_callback(GLFWwindow *window, double xpos, double ypos)
	// {	
	// 	State* winHdl = (State*)glfwGetWindowUserPointer(window);
	// 	winHdl->cursor = { xpos, ypos };
	// 	return;
	// }


	constexpr u32 glfwEnum(KEY key) 
	{
		constexpr u32 convert[22] = {
			GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_Q, 
			GLFW_KEY_R, GLFW_KEY_F, GLFW_KEY_G, GLFW_KEY_Z, GLFW_KEY_X,
			GLFW_KEY_C, GLFW_KEY_V,
			GLFW_KEY_0, GLFW_KEY_1, GLFW_KEY_2, GLFW_KEY_3, GLFW_KEY_4, 
			GLFW_KEY_5, GLFW_KEY_6, GLFW_KEY_7, GLFW_KEY_8, GLFW_KEY_9,
		};

		return convert[(u32)key];
	}


	bool keyState(KEY keyEnum) {
		return glfwGetKey(__internal::underlyingWinHandle(), glfwEnum(keyEnum));
	}
	



	std::array<f64, 2> const& cursorPosition() { return __internal::global().cursor; }


	bool isKeyPressed (KEY keyEnum) { return GLFW_PRESS   == keyState(keyEnum); }
	bool isKeyReleased(KEY keyEnum) { return GLFW_RELEASE == keyState(keyEnum); }


}