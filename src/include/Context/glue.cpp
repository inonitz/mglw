#include "glue.hpp"
#include "state.hpp"
#include "render.hpp"
#include "callback.hpp"
#include "GLFW/glfw3.h"




namespace __internal {
/* 
	I know the short-comings of such an implementation like this,
	but because I don't need more than 1 window active for this project,
	I'll keep it abstracted like this.
*/
static callbackFuncTable defaultCallbacks = {
	Renderer::framebuffer_size_callback,
	Context::Input::glfw_key_callback,
	Context::Input::glfw_cursor_callback
};
static State 	 gState;
static GLContext GL;




State& 			   global() 			 { return gState; 				 }
GLContext& 		   gl() 	   			 { return GL; 					 }
GLFWwindow* 	   underlyingWinHandle() { return gState.nativeHandle(); }
callbackFuncTable& callbackTable()   	 { return defaultCallbacks; 	 }


}


