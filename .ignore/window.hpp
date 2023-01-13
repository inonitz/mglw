#pragma once
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#define GLFW_INCLUDE_NONE
#include <array>
#include "timer.hpp"
#include "texture.hpp"




struct GLFWwindow;




namespace __internal {


struct State 
{
private:
	GLFWwindow*        window;

public:
	struct GLContext {
		Shader 		 compute;
		Shader 		 program;

		VertexBuffer quadVBO;
		IndexBuffer  quadIBO;
		VertexArray  quad;
		
		Texture2D 	 screenTex;
	} GL;
	Timer<>            counter;
	f64                delta;                
	std::array<i32, 2> dims;
	std::array<f64, 2> cursor;
	std::array<f32, 4> clear;


public:
	State() : 
		window(nullptr), 
		dims{-1, -1}, 
		cursor{0xDEADBEEF, 0xDEADBEEF},
		clear{1.0f, 0.0f, 1.0f, 1.0f} {} // pruple


	void create(i32 WindowWidth, i32 WindowHeight);
	void destroy();


	bool closed();
	void useScreenTexture();
	__force_inline GLFWwindow* nativeHandle() { return window;         }
	__force_inline void 	   startCounter() { counter.start();       }
	__force_inline void 	   endCounter()   { delta = counter.end(); }
};


}




namespace Context {

void init(i32 maxWindowWidth, i32 maxWindowHeight);
void terminate();
f64  deltaTime();


namespace Input {
	enum class KEY {
		W,
		A,
		S,
		D,
		Q,
		R,
		F,
		G,
		Z,
		X,
		C,
		V,
		NUM0,
		NUM1,
		NUM2,
		NUM3,
		NUM4,
		NUM5,
		NUM6,
		NUM7,
		NUM8,
		NUM9
	};


	std::array<f64, 2> const& cursorPosition();
	bool 					  isKeyPressed (KEY keyEnum);
	bool                      isKeyReleased(KEY keyEnum);
}


namespace Window {
	bool 			   		  closed();
	std::array<i32, 2> const& dimensions();
}




}


namespace Renderer {
	void createGLContext();
	void destroyGLContext();


	void beginBackendFrame();
	void endBackendFrame();
	
	
	void useDefaultTexture();


	void dispatchCompute(std::array<u32, 3> const& workGroups);

	void 			   setClearColor(std::array<f32, 4> const&);
	std::array<f32, 4> getClearColor();
};