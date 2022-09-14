#include "base.hpp"
#include <chrono>




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