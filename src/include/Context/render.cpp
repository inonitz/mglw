#include "render.hpp"
#include "context.hpp"
#include "state.hpp"
#include "glue.hpp"
#include "GLFW/glfw3.h"
#include "../imgui185/imgui_impl_glfw.h"
#include "../imgui185/imgui_impl_opengl3.h"
#include "window.hpp"
#include "../stb_image.hpp"
#include <cstdlib>




namespace Renderer {

	
	void createGLContext()
	{
		const manvec<float> vertices = 
		{
			// positions               // texture coords
			 1.0f,  1.0f, 0.0f, 1.0f,  1.0f, 1.0f,  // top right
			 1.0f, -1.0f, 0.0f, 1.0f,  1.0f, 0.0f,  // bottom right
			-1.0f, -1.0f, 0.0f, 1.0f,  0.0f, 0.0f,  // bottom left
			-1.0f,  1.0f, 0.0f, 1.0f,  0.0f, 1.0f   // top left 
		};
		const manvec<u32> indices = 
		{
			0, 1, 3, /* first  triangle */
			1, 2, 3  /* second triangle */
		};
		std::string shaderPaths[3] = 
		{
			"C:/Program Files/Programming Utillities/Cygwin/home/themc/mglw/assets/shaders/compute_rtrace/shader.vert",
			"C:/Program Files/Programming Utillities/Cygwin/home/themc/mglw/assets/shaders/compute_rtrace/shader.frag",
			"C:/Program Files/Programming Utillities/Cygwin/home/themc/mglw/assets/shaders/compute_rtrace/shader.comp"	
		};


		// u8*  stbImage;
		// f32* floatImage;
		// i32  imgX, imgY, imgC;
		// stbi_set_flip_vertically_on_load(1);
		// stbImage   = stbi_load("/home/inon/mglw/assets/objs/sample_img.jpg", &imgX, &imgY, &imgC, 4);
		// imgC 	   = 4;
		// floatImage = amalloc_t(float, imgX * imgY * imgC * sizeof(float), sizeof(float) * 8); 
		// TextureConvertRGBAUI32ToRGBA32F(stbImage, floatImage, imgX, imgY);
		// stbi_image_free(stbImage);


		// auto& vbo 	 = __internal::gl().quadVBO;
		// auto& ibo 	 = __internal::gl().quadIBO;
		auto& vao 	 = __internal::gl().quad;
		auto& compute      = __internal::gl().compute;
		// auto& program      = __internal::gl().program;
		auto& screenTex = __internal::gl().screenTex;
		auto& screenFb= __internal::gl().screenFb;


		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		// vbo.create(vertices.begin(), vertices.len(), VertexDataType::f32, GL_DYNAMIC_STORAGE_BIT);
		// ibo.create(indices.begin(),  indices.len(),  VertexDataType::u32, GL_DYNAMIC_STORAGE_BIT);
		// vao.create(vbo, ibo, 
		// {
		// 	{ 4, (u32)VertexDataType::f32, GL_FALSE, 0 },
		// 	{ 2, (u32)VertexDataType::f32, GL_FALSE, 0 }
		// });
		vao.create({}, {}, {});

		// program.create({ 
		// 	loadShader(shaderPaths[0], ShaderType::Vertex  ),
		// 	loadShader(shaderPaths[1], ShaderType::Fragment) 
		// });
		compute.create({ 
			loadShader(shaderPaths[2], ShaderType::Compute ) 
		});

		
		screenTex.create(
			{ GL_RGBA32F, GL_RGBA, GL_FLOAT },
			__internal::global().dims,
			{
				{ GL_TEXTURE_MIN_FILTER, GL_NEAREST         },
		  		{ GL_TEXTURE_MAG_FILTER, GL_NEAREST         },
  				{ GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_BORDER },
				{ GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_BORDER }
			},
			nullptr
		);
		screenFb.create(fboType::READ);
		screenFb.attachTexture({
			screenTex.glid(),  // .textureID  
			fboAttach::COLOR0, // .attachType 
			0				   // .mipMapLevel
		});


		// program.compile();
		compute.compile();


		debug({
			printf("Active Renderer: %s\n", glGetString(GL_RENDERER));
			printf("Current OpenGL Version %s\n", glGetString(GL_VERSION));
			// program.print();
			compute.print();
			vao.print();
			screenTex.print();
		});
		return;
	}


	void destroyGLContext()
	{
		// auto& vbo = __internal::gl().quadVBO;
		// auto& ibo = __internal::gl().quadIBO;
		auto& vao = __internal::gl().quad;
		auto& compute   = __internal::gl().compute;
		// auto& program   = __internal::gl().program;
		auto& screenTex = __internal::gl().screenTex;
		auto& screenFbo = __internal::gl().screenFb;


		/* Unbind Everything */
		screenFbo.unbind();
		screenTex.unbindImageUnit();
		screenTex.unbindTexUnit();
		compute.unbind(); /* unbind all active programs */
		vao.unbind();
		

		/* Cleanup */
		// vbo.destroy();
		// ibo.destroy();
		vao.destroy();
		screenTex.destroy();
		free(screenTex.oncpuData());
		screenFbo.destroy();
		
		
		// program.destroy();
		compute.destroy();
		return;
	}


	void beginBackendFrame()
	{
		__internal::global().startCounter();

		glClearColor(
			__internal::global().clear[0], 
			__internal::global().clear[1], 
			__internal::global().clear[2], 
			__internal::global().clear[3]
		);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();



		return;
	}


	void endBackendFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		glfwPollEvents();
		glfwSwapBuffers(__internal::underlyingWinHandle());


		__internal::global().endCounter();
		return;
	}


	void bindGLObjects()
	{
		auto& gl = __internal::gl();
		gl.screenTex.bindToTexUnit(0);
		gl.screenTex.bindToImageUnit(0);
		gl.quad.bind();
		

		fboStatus status = Framebuffer::framebufferStatus(gl.screenFb);
		ifcrashdo(status != fboStatus::COMPLETE, {
			printf("Framebuffer [ID %u] couldn't be completed. Error flag: %s\n", gl.screenFb.glid(), fboStatusToStr(status));
		});


		gl.screenFb.bind();
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		return;
	}


	void RenderFBO(std::array<i32, 2> const& workGroups)
	{
		auto& glctx = __internal::gl();
		if(  ( !boolean(workGroups[0]) && !boolean(workGroups[1]) ) && glctx.screenTex.dimensions() != workGroups  ) 
		{
			glctx.screenTex.recreateImage(workGroups);
			glctx.screenFb.attachTexture({
				glctx.screenTex.glid(),
				fboAttach::COLOR0, 
				0
			});
		}

		/* 
			We will skip frames until the FBO is complete.
		*/
		if(Framebuffer::framebufferStatus(glctx.screenFb) != fboStatus::COMPLETE) 
			return;
		

		glctx.screenFb.blitFramebuffers(
			{ 
				glctx.screenFb.glid(),
				0,
				0,
				static_cast<u32>(glctx.screenTex.dimensions()[0]),
				static_cast<u32>(glctx.screenTex.dimensions()[1])
			},
			{
				0,
				0,
				0,
				static_cast<u32>(Context::Window::dimensions()[0]),
				static_cast<u32>(Context::Window::dimensions()[1]),
			},
			GL_COLOR_BUFFER_BIT,
			GL_NEAREST
		);
		return;
	}


	void Render()
	{
		auto& glctx = __internal::gl();
		i32 texDataSamplerID = (i32)glctx.screenTex.texUnit();


		glctx.program.setActive();
		glctx.program.setUniform("texData", UniformType::i32p, 1, &texDataSamplerID);
		glctx.screenTex.activeTexture(0);

		glDrawElements(
			GL_TRIANGLES, 
			glctx.quadIBO.count(), 
			glctx.quadIBO.gltype(),
			nullptr
		);
	}


	void dispatchCompute(std::array<i32, 3> const& workGroups)
	{
		auto& glctx = __internal::gl(); 
		auto& comp 	 = glctx.compute;
		f32 dt = __internal::global().delta / __internal::global().max_delta;



		comp.setActive();
		comp.setUniform("time_delta", UniformType::f32p, 1, &dt);
		comp.beginCompute(workGroups);

		return;
	}


	void waitForCompute()     { __internal::gl().compute.barrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT); return; }
	void useDefaultTexture()  { __internal::gl().screenTex.activeTexture(0); 															  return; }
	void 			   setClearColor(std::array<f32, 4> const& vec4) { __internal::global().clear = vec4; }
	std::array<f32, 4> getClearColor() 							     { return __internal::global().clear; }

	
}