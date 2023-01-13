#pragma once
#include <array>
#include "../shader.hpp"
#include "../texture.hpp"
#include "../vertexArray.hpp"
#include "../framebuffer.hpp"




struct GLContext {
	Shader 		 compute;
	Shader 		 program;

	VertexBuffer quadVBO;
	IndexBuffer  quadIBO;
	VertexArray  quad;
	
	Texture2D 	 screenTex;
	Framebuffer  screenFb;
};




namespace Renderer {
	void createGLContext();
	void destroyGLContext();


	void beginBackendFrame();
	void endBackendFrame();

	void bindGLObjects();
	void Render();
	void RenderFBO(std::array<i32, 2> const& workGroups);
	
	void dispatchCompute(std::array<i32, 3> const& workGroups);
	void waitForCompute();


	void 			   setClearColor(std::array<f32, 4> const&);
	std::array<f32, 4> getClearColor();
};
