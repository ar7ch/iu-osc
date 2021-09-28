/* please see report.pdf */

#include <stdio.h>
#include <stdlib.h>
#include "schedule.h"
#include "iuosc.h"

void scheduler_roundrobin(process * p, int n, int quantum_left) {
	int time_passed = 0;
	for(int done_procs = 0; done_procs < n; ) {
		int had_advance = 0;	
		for(int j = 0; j < n; j++) {
			//printf("time: %d\n", time_passed);
			process * proc = p+j;
			char task_ready = (time_passed >= proc->arrival_time);
			if (proc->done) continue;
			if (!task_ready) continue;
			if (!(proc->exec_start_time)) { proc->exec_start_time = time_passed; }// mark where we started processing 
			if (proc->exec_time_left > quantum_left) { // one quantum is not enough for the process
				proc->exec_time_left -= quantum_left; // this part is done
				time_passed += quantum_left; // whole quantum passed 
				//printf("partially executed AT #%d, now time: %d\n", proc->arrival_time, time_passed);
				had_advance = TRUE;
			}
			else {
				time_passed += proc->exec_time_left;
				//printf("time: %d, finished: AT #%d\n", time_passed, proc->arrival_time);
				proc->exec_time_left = 0;
				// completion time is time from the very beginning to the process termination
				proc->completion_time = time_passed; 
				proc->turnaround_time = proc->completion_time - proc->arrival_time; // time passed from arrival time to completion time
				proc->waiting_time = proc->turnaround_time - proc->burst_time;
				proc->done = TRUE;

				had_advance = TRUE;
				++done_procs;
			}
		}
		// there are no processes to run yet
		if(!had_advance) { 
			//printf("nothing happened on %d\n", time_passed); }
			++time_passed;
		}
	}
}

int main() {
	printf("===================================\n"
	"Round-robin scheduler\n"
	"===================================\n");
	int n, quantum;
	get_int(&quantum, "input quantum length: ");
	get_int(&n, "input number of processes: ");
	process * ptable = get_ptable(n);

	scheduler_roundrobin(ptable, n, quantum);

	proc_print_report(ptable, n);
	remove_ptable(ptable);
	return 0;
}
