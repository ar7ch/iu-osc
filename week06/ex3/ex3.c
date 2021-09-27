#include <stdio.h>
#include <stdlib.h>
#include "schedule.h"

const int quantum_left = 2;

void scheduler_roundrobin(process * p, int n) {
	int time_passed = 1;
	for(int done_procs = 0; done_procs < n; ) {
		int had_advance = 0;	
		for(int j = 0; j < n; j++) {
			printf("time: %d\n", time_passed);
			process * proc = p+j;
			char task_ready = (time_passed >= proc->arrival_time);
			if (proc->done) continue;
			if (!task_ready) continue;
			if (!(proc->exec_start_time)) { proc->exec_start_time = time_passed; }// mark where we started processing 
			if (proc->exec_time_left > quantum_left) { // one quantum is not enough for the process
				proc->exec_time_left -= quantum_left; // this part is done
				time_passed += quantum_left; // whole quantum passed 
				printf("partially executed AT #%d, now time: %d\n", proc->arrival_time, time_passed);
			}
			else {
				time_passed += proc->exec_time_left;
				printf("time: %d, finished: AT #%d\n", time_passed, proc->arrival_time);
				//proc_fill_timedata(p+j, &time_passed);
				proc->exec_time_left = 0;
				// completion time is time from the very beginning to the process termination
				proc->completion_time = time_passed - 1; // distance between time_passed and 1
				proc->turnaround_time = proc->completion_time - proc->arrival_time; // time passed from arrival time to completion time
				proc->waiting_time = proc->turnaround_time - proc->burst_time;//((int)(proc->burst_time)/(quantum_left)) - (proc->burst_time)%(quantum_left); // time passed before the beginning of execution
				proc->done = TRUE;

				++done_procs;
			}
		}
		// there are no processes to run yet
		if(!had_advance) { ++time_passed; }
	}
}

int main() {
	int n;
	get_int(&n, "input number of processes: ");
	process * ptable = get_ptable(n);

	scheduler_roundrobin(ptable, n);

	proc_print_report(ptable, n);
	remove_ptable(ptable);
	return 0;
}
