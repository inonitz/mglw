#include "context.hpp"
#include "state.hpp"
#include "render.hpp"
#include "glue.hpp"




namespace Context {
	static bool usedGL = false;


	void overrideDefaultCallbacks(
		void* frambuffer_callback, 
		void* keyboard_callback, 
		void* cursor_position_callback
	) {
		__internal::callbackTable() = {
			(framebuffer_callback)frambuffer_callback,
			(key_callback)keyboard_callback,
			(cursor_callback)cursor_position_callback,
		};
		return;
	}


	void init(bool useGLContext, i32 maxWindowWidth, i32 maxWindowHeight) {
		ifcrashdo(maxWindowHeight <= 0 || maxWindowWidth <= 0, {
			printf("Tried to Call Context::init() with invalid window dimensions.\n    arguments that were passed: [width, height] = [%u, %u]\n", 
				maxWindowWidth,
				maxWindowHeight
			);
		});
		ifcrashdo(__internal::underlyingWinHandle() != nullptr, {
			printf("Tried to Call Context::init() more than once. Check your code.\n");
		});
		__internal::global().create(useGLContext, maxWindowWidth, maxWindowHeight);

		
		if((usedGL = useGLContext)) {
			ifcrashdo(__internal::gl().quad.glid() != DEFAULT32, {
				printf("Tried to call Renderer::createGLContext() more than once. Check your code.\n");
			});
			printf("using Default Renderer OpenGL Context.\n");
			Renderer::createGLContext();
		}
		return;
	}
	
	
	void terminate() {
		if(usedGL) {
			ifcrash(__internal::gl().quad.glid() == DEFAULT32);
			Renderer::destroyGLContext();
		}
		

		ifcrash(__internal::underlyingWinHandle() == nullptr);
		__internal::global().destroy();
		return;
	}


	f64 deltaTime() {
		return __internal::global().delta;
	}

	f64  avgDelta() {
		auto& ctx = __internal::global();
		return ctx.delta_sum /(f64)ctx.count;
	}


	bool active() {
		return !__internal::global().closed();
	}
}