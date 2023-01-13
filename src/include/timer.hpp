#pragma once
#include "base.hpp"
#include "glad/glad.h"
#include <chrono>
#include <cstddef>




template<typename internal_type = double> struct Timer {
private:
#define chrono_nanosec_time_point_now() \
		std::chrono::time_point_cast<std::chrono::nanoseconds>( \
			std::chrono::system_clock::now() \
		) \

	using sys_nanosec = decltype(chrono_nanosec_time_point_now());
	using __nanosec = sys_nanosec;

	__nanosec step;


	__force_inline void reset(__nanosec* dst) {	
		*dst = chrono_nanosec_time_point_now();
		return;
	}
public:

	Timer() {}

	__force_inline void   		 start() { reset(&step); } 
	__force_inline internal_type end() {
		__nanosec step_1;
		reset(&step_1);


		return static_cast<internal_type>(
			std::chrono::duration_cast<std::chrono::nanoseconds>(step_1 - step).count()
		);
	}
};




template<typename internal_type = double, u32 querySwapSize = 2> struct GPUTimer {
	/* Implemented with help from https://www.lighthouse3d.com/tutorials/opengl-timer-query/ */
private:
	/* 2 queries per swap, so that 1 query won't stall the rest of the program. */
	struct {
		union {
			u32 q0, q1;
			u32 q[2];
		};
	} queryIDbuf[querySwapSize];
	u32 swapIdx;

	u64 prevFrame, nextFrame;

public:
	GPUTimer() : prevFrame(0), nextFrame(0), swapIdx(0) {
		glCreateQueries(querySwapSize << 1, &queryIDbuf[0]);
		return;
	}

	~GPUTimer() {
		glDeleteQueries(querySwapSize << 1, &queryIDbuf[0]);
		return;
	}


	u64 previousFrameCount() { return prevFrame; }
	u64 currentFrameCount()  { return nextFrame; }


	__force_inline void startQuery() 
	{
		/* query current elapsed time on gpu into q0 [the 'backbuffer'] */
		glBeginQuery(GL_TIME_ELAPSED, queryIDbuf[swapIdx].q0);
	}


	__force_inline void endQuery()
	{
		glEndQuery(GL_TIME_ELAPSED); /* stop the query */
		prevFrame = nextFrame; 		 /* after frame N, frame N+1 needs to update prevFrame count.     */
		glGetQueryBufferObjectui64v( /* query current elapsed time on gpu into q1 [the 'frontbuffer'] */
			queryIDbuf[swapIdx].q1, 
			GL_QUERY_RESULT, 
			&nextFrame
		);
		/* 
			we're not waiting for the query to be available for performance sake.
			{
				int done = 0;
				glGetQueryObjectiv(query, GL_QUERY_RESULT_AVAILABLE, &done);
			}
			calling GL_QUERY_RESULT_AVAILABLE flushes the pipeline and will
			probably cause massive performance hits. (Waiting for everything to sync...)

			thats why every frame we'll query multiple query objects,
			so that we'll get the result (most of the time.)
		*/
		return;
	}


	void incrementBuffers() {
		++swapIdx;
		swapIdx *= (swapIdx != querySwapSize); /* if not equal, we can keep swapIdx. else, we need to reset to 0. */
		return;
	}
};




template<u32 maxSamples> struct SMATick {
private:
	u64 totalSamples;
	u32 index;
	u64 sum;
	u64 tick[maxSamples];
public:
	SMATick() : totalSamples(0), index(0), sum(0), tick{0} {}


	template<typename T> T calcTick(u64 currFrameTick) 
	{
		sum -= tick[index];
		sum += currFrameTick;
		tick[index] = currFrameTick;
		
		++index;
		++totalSamples;
		index *= (index == maxSamples);
		
		return ((T)sum / (T)maxSamples);
	}
};