#include "window.hpp"
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "imgui185/imgui_impl_glfw.h"
#include "imgui185/imgui_impl_opengl3.h"




static void gl_debug_message_callback(
	GLenum 		  source, 
	GLenum 		  type, 
	GLuint 		  id, 
	GLenum 		  severity, 
	GLsizei		  length, 
	GLchar const* message, 
	void const*   user_param
);


static void glfw_error_callback(
	int 		error, 
	const char* description
);



namespace Context::Input {
	static void glfw_key_callback(			
		notused GLFWwindow* window, 
		notused int 		key, 
		notused int 		scancode, 
		notused int 		action, 
		notused int 		mods
	);


	static void glfw_cursor_callback(GLFWwindow *window, double xpos, double ypos);


};


namespace Renderer {
	__unused static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
}


namespace Context {


	void init(i32 maxWindowWidth, i32 maxWindowHeight) {
		ifcrashdo(__internal::gState.nativeHandle() != nullptr, {
			printf("Tried to Call Context::init() more than once. Check your code.\n");
		});
		__internal::gState.create(maxWindowWidth, maxWindowHeight);
	}
	
	
	void terminate() {
		ifcrashdbg(__internal::gState.nativeHandle() == nullptr);
		__internal::gState.destroy();
	}


	f64 deltaTime() {
		return __internal::gState.delta;
	}




	namespace Input {
		__unused static void glfw_key_callback(
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


		static void glfw_cursor_callback(GLFWwindow *window, double xpos, double ypos)
		{
			using win_handle = __internal::State*;
			
			win_handle win = (win_handle)glfwGetWindowUserPointer(window);
			win->cursor = { xpos, ypos };
			return;
		}


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
			return glfwGetKey(__internal::underlyingGLFWHandle(), glfwEnum(keyEnum));
		}
		



		std::array<f64, 2> const& cursorPosition() { return __internal::gState.cursor; }


		bool isKeyPressed (KEY keyEnum) { return GLFW_PRESS   == keyState(keyEnum); }
		bool isKeyReleased(KEY keyEnum) { return GLFW_RELEASE == keyState(keyEnum); }


	}




	namespace Window {


		bool closed() 
		{ 
			return __internal::gState.closed(); 
		}


		std::array<i32, 2> const& dimensions() 
		{ 
			return __internal::gState.dims; 
		}

	}
}

