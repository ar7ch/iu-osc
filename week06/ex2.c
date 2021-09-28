#include <stdio.h>
#include <stdlib.h>
#include "schedule.h"

void scheduler_shortest_first(process * p, int n) 
{
	
	qsort(p, n, sizeof(process), &comp_by_arrival_time);
	qsort(p, n, sizeof(process), &comp_by_burst_time);

	int time_passed = 0;
	char had_advance;
	// N schedules
	for(int done_procs = 0; done_procs < n; ) {
		had_advance = FALSE;
		// for every schedule, find the most suitable run candidate
		for(int j = 0; j < n; j++) {
			printf("time: %d\n", time_passed);
			char task_ready = (time_passed >= (p+j)->arrival_time);
			if((p+j)->done) { continue; }
			else if (!task_ready) {	continue; }
			else {
				printf("executing proc (%d, %d)\n",(p+j)->arrival_time, (p+j)->burst_time);
				proc_fill_timedata(p+j, &time_passed);
				done_procs++;	
				had_advance = TRUE;
			}
		}
		if(!had_advance) {
			printf("nothing to execute this time\n");
			++time_passed;
		}
	}
}

int main() {
	printf("===================================\n"
	"Shortest task first scheduler\n"
	"===================================\n");
	int n;
	get_int(&n, "input number of processes: ");
	process * ptable = get_ptable(n);

	scheduler_shortest_first(ptable, n);

	proc_print_report(ptable, n);
	remove_ptable(ptable);
	return 0;
}
