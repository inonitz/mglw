#include "timer.hpp"
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <inttypes.h>
#include <asm/unistd.h>
#include <linux/perf_event.h>




uint32_t Measure::Timer::tsc_mult ;
uint32_t Measure::Timer::tsc_shift;




static long perf_event_open(
	struct perf_event_attr* hw_event, 
	pid_t 				    pid,
    int 					cpu, 
	int 					group_fd, 
	unsigned long 		    flags
) {
    return syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
}



namespace Measure {


void get_timestamp_counter_args(uint32_t* tsc_time_mult, uint32_t* tsc_time_shift)
{
	struct perf_event_attr 		 perf_event;
	struct perf_event_mmap_page* pc;
	void* addr;
	int   fd;
	

	perf_event.type 		  = PERF_TYPE_HARDWARE;
	perf_event.size 	      = sizeof(struct perf_event_attr);
    perf_event.config 		  = PERF_COUNT_HW_INSTRUCTIONS;
    perf_event.disabled 	  = 1;
    perf_event.exclude_kernel = 1;
    perf_event.exclude_hv 	  = 1;
	
	fd = perf_event_open(&perf_event, 0, -1, -1, 0);
	ifcrashdo(fd == -1, { 
		printf("Clock::init::get_timestamp_counter_args::perf_event_open() failed (Error Code %u). Failed to Initialize Clock.\n", errno);
	});


	addr = mmap(NULL, PAGE, PROT_READ, MAP_SHARED, fd, 0);
	ifcrashdo(!addr, {
		printf("Clock::init::get_timestamp_counter_args::mmap() failed (Error Code %u). Failed to Initialize Clock.\n", errno);
	});
	pc = (struct perf_event_mmap_page*)addr;


	ifcrashdo(pc->cap_user_time != 1, {
		printf("Clock::init::get_timestamp_counter_args() - Perf System Doesn't Support User Time.\n");
	});

	
	*tsc_time_mult  = pc->time_mult;
	*tsc_time_shift = pc->time_shift;
	close(fd);
	return;
}


}