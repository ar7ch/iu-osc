
#ifndef SCHEDULE_H
#define SCHEDULE_H

#ifndef BUF_SIZE
#define BUF_SIZE 50
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int arrival_time;
	int burst_time;
	int completion_time;
	int turnaround_time;
	int waiting_time;
	int done;
} process;

void proc_fill_timedata(process * proc, int * time_passed)
{
	proc->completion_time = *time_passed + proc->burst_time; // time from the very beginning to the process termination
	proc->waiting_time = *time_passed - proc->arrival_time; // time passed before the beginning of execution
	printf("waiting time: %d\n", proc->waiting_time);
	proc->turnaround_time = proc->completion_time - proc->arrival_time;  // time passed from arrival time to completion time
	
	*time_passed = *time_passed + (proc->burst_time);
	proc->done = TRUE;
}

void proc_print_report(process * procs, int n) 
{
	float awt = 0, att = 0;
	printf("\nP#\tAT\tBT\tCT\tTAT\tWT\n");
	for(int i = 0; i < n; i++) {
		int at = procs[i].arrival_time;
		int bt = procs[i].burst_time;
		int ct = procs[i].completion_time;
		int tat = procs[i].turnaround_time;
		int wt = procs[i].waiting_time;
		awt += wt;
		att += tat;
		printf("P%d\t%d\t%d\t%d\t%d\t%d\n", i, at, bt, ct, tat, wt);
	}
	awt = awt / n;
	att = att / n;
	printf("Average Turnaround Time = %f\nAverage Waiting Time = %f\n", att, awt);
}

void get_int(int * n, char * prompt) {
	printf("%s", prompt);
	scanf("%d", n);
}


process * get_ptable(int n) {
	char buf[BUF_SIZE];
	process * ptable = malloc(sizeof(process) * n);
	if (ptable == NULL) {
		perror("Allocation error");
		exit(EXIT_FAILURE);
	}
	for(int i = 0; i < n; i++) {
		int at, bt;
		sprintf(buf,"input arrival time of process #%d: ", i);
		get_int(&at, buf);
		sprintf(buf,"input burst time of process #%d: ", i);
		get_int(&bt, buf);

		ptable[i].completion_time = 0;
		ptable[i].turnaround_time = 0;
		ptable[i].waiting_time = 0;
		ptable[i].arrival_time = at;
		ptable[i].burst_time = bt;
		ptable[i].done = FALSE;
	}
	return ptable;
}

void remove_ptable(process * ptable) {
	free(ptable);
}

int comp_by_burst_time(const void * a, const void * b) {
	process * p1 = (process *) a;
	process * p2 = (process *) b;
	return p1->burst_time - p2->burst_time;
}

int comp_by_arrival_time(const void * a, const void * b) {
	process * p1 = (process *) a;
	process * p2 = (process *) b;
	return p1->arrival_time - p2->arrival_time;
}

#endif
