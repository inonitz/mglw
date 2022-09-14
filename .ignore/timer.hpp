#pragma once
#include "base.hpp"
#include "x86intrin.h"




namespace Measure 
{


extern void get_timestamp_counter_args(uint32_t* tsc_time_mult, uint32_t* tsc_time_shift);




typedef uint64_t StandardPrecisionTime  ;
typedef float    HighPrecisionTime  	;
typedef double   HighDoublePrecisionTime;


class Timer {
private:
	StandardPrecisionTime tsc;

	static uint32_t tsc_mult ;
	static uint32_t tsc_shift;


	uint64_t nanoseconds(uint64_t tscval) {
		__uint128_t x = tscval;
		x *= tsc_mult;
		x >>= tsc_shift;
		return x;
	}


public:

	Timer() : tsc(0) {
		get_timestamp_counter_args(&tsc_mult, &tsc_shift);
		return;
	}


	__force_inline void     reset() { tsc = __rdtsc(); return; }
	__force_inline uint64_t now()   { return nanoseconds(__rdtsc() - tsc); }


	static uint64_t millisec(uint64_t nanoseconds) { return nanoseconds / 1000000;    }
	static uint64_t microsec(uint64_t nanoseconds) { return nanoseconds / 1000;       }
	static uint64_t sec	    (uint64_t nanoseconds) { return nanoseconds / 1000000000; }


	__force_inline HighDoublePrecisionTime toDoublePrec(StandardPrecisionTime time) { return (double)time; }
	__force_inline HighDoublePrecisionTime toDoublePrec(HighPrecisionTime     time) { return (double)time; }
	__force_inline HighDoublePrecisionTime toHighPrec  (StandardPrecisionTime time) { return (float)time;  }
};




template<typename T, typename U = uint64_t> T duration_cast(U time) 
	requires(
		(
			std::is_same<T, StandardPrecisionTime  >::value ||
			std::is_same<T, HighPrecisionTime      >::value ||
			std::is_same<T, HighDoublePrecisionTime>::value
		) && 
		( 	std::is_same<U, uint64_t	    	   >::value ||
			std::is_same<U, StandardPrecisionTime  >::value ||
			std::is_same<U, HighPrecisionTime      >::value ||
			std::is_same<U, HighDoublePrecisionTime>::value 
		)
	);


template<> StandardPrecisionTime   duration_cast<StandardPrecisionTime  , uint64_t			    >(uint64_t 			    time) { return (StandardPrecisionTime  )time; }
template<> HighPrecisionTime       duration_cast<HighPrecisionTime      , uint64_t			    >(uint64_t 			    time) { return (HighPrecisionTime      )time; }
template<> HighDoublePrecisionTime duration_cast<HighDoublePrecisionTime, uint64_t			    >(uint64_t 			    time) { return (HighDoublePrecisionTime)time; }
template<> HighPrecisionTime       duration_cast<HighPrecisionTime      , StandardPrecisionTime >(StandardPrecisionTime time) { return (HighPrecisionTime      )time; }
template<> HighDoublePrecisionTime duration_cast<HighDoublePrecisionTime, StandardPrecisionTime >(StandardPrecisionTime time) { return (HighDoublePrecisionTime)time; } 
template<> HighDoublePrecisionTime duration_cast<HighDoublePrecisionTime, HighPrecisionTime     >(HighPrecisionTime     time) { return (HighDoublePrecisionTime)time; } 




}