#include <stdio.h>
#include <stdlib.h>
#include "schedule.h"

void scheduler_fifo(process * ptable, size_t n) 
{
	qsort(ptable, n, sizeof(process), &comp_by_arrival_time);
	int time_passed = 0;
	for(int i = 0; i < n; i++) {
		while(time_passed < (ptable+i)->arrival_time)
			++time_passed;
		proc_fill_timedata(ptable+i, &time_passed);
	}

}

int main() {
	printf("===================================\n"
	"First-come first-served scheduler\n"
	"===================================\n");
	int n;
	get_int(&n, "input number of processes: ");
	process * ptable = get_ptable(n);

	scheduler_fifo(ptable, n);

	proc_print_report(ptable, n);
	remove_ptable(ptable);
	return 0;
}
