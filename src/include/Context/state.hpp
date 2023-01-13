#pragma once
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#define GLFW_INCLUDE_NONE
#include <array>
#include "../timer.hpp"




struct GLFWwindow;


struct State 
{
private:
	GLFWwindow*        window;
	
public:
	Timer<>            counter;
	f64                delta;
	f64                max_delta;
	f64                delta_sum;
	u64                count;

	std::array<i32, 2> dims;
	std::array<f64, 2> cursor;
	std::array<f32, 4> clear;


public:
	State() : 
		window(nullptr), 
		delta{1.0},
		max_delta{1.0},
		delta_sum{0},
		count{0},
		dims{(i32)DEFAULT32, (i32)DEFAULT32}, 
		cursor{(double)DEFAULT64, (double)DEFAULT64},
		clear{1.0f, 0.0f, 1.0f, 1.0f} {} // pruple


	void create(bool useInternalCtxt, i32 WindowWidth, i32 WindowHeight);
	void destroy();


	bool closed();
	void close();
	__force_inline GLFWwindow* nativeHandle() { return window;   }
	__force_inline void 	   startCounter() { counter.start(); }
	__force_inline void 	   endCounter()   { 
		delta      = counter.end();  
		max_delta  = std::max(max_delta, delta);
		
		delta_sum += delta;
		++count;
		return;
	} 
};