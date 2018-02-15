#ifndef SHORTEST_JOB
#define SHORTEST_JOB

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "process.h"

//Performs the scheduling function
void sjf(process *queue, int processCount, int runFor);
//Detects the next process
int sjfSelect(struct process *queue, int size);
//Updates the wait times after a given burst
void updateWait(process *queue, int *wait, int burstSize, int currentProcess, int limit);

void sjf(process *queue, int *order, int processCount, int runFor)
{
	//I believe that our output is a file. He says the name is supposed to be constant. I'm not sure
	FILE *f = fopen("Processes.out", "w");
	fprintf(f, "processes %d\n", processCount);
	fprintf(f, "Using Shortest Job First (Pre)\n\n");
	
	int time = 0;
	
	//I use -1 to indicate the idle state
	int currentProcess = -1;
	//This is the index of the next process to arrive in the queue
	int nextArrival = 0;
	//I keep track of the burst to update the wait array when there is a context switch.
	//That way I do not have to go through all of the processes at every time unit.
	int currentBurst = 0;
	
	int *wait;
	int *turnaround;
	
	//I use calloc here since I want everything to be initialized to 0
	wait = calloc(processCount, sizeof(int));
	turnaround = calloc(processCount, sizeof(int));
	
	for(time = 0; time < runFor; time++)
	{	
		//I check for new processes in a loop just in case there are multiple processes with the same arrival time
		while(nextArrival < processCount && queue[nextArrival].arrival == time)
		{
			int nextProcess;
			fprintf(f, "Time %d: %s arrived\n", time, queue[nextArrival].name); 
			nextProcess = sjfSelect(queue, nextArrival + 1);
			if(nextProcess != currentProcess)
			{
				if(nextProcess != -1)
				{
					updateWait(queue, wait, currentBurst, currentProcess, nextArrival);
					currentBurst = 0;
					currentProcess = nextProcess;
					
					fprintf(f, "Time %d: %s selected (burst %d)\n", time, queue[currentProcess].name, queue[currentProcess].burst);
					turnaround[currentProcess] -= time;	
				}
			}
			nextArrival++;
		}
		//A process with a burst time of 0 is considered done
		if (queue[currentProcess].burst == 0)
		{
			fprintf(f, "Time %d: %s finished\n", time, queue[currentProcess].name);
			turnaround[currentProcess] += time;
			updateWait(queue, wait, currentBurst, currentProcess, nextArrival);
			currentBurst = 0;
			currentProcess = sjfSelect(queue, nextArrival);
			if(currentProcess != -1)
			{
				fprintf(f, "Time %d: %s selected (burst %d)\n", time, queue[currentProcess].name, queue[currentProcess].burst);
			}
		}
		if(currentProcess != -1)
		{
		queue[currentProcess].burst--;
		currentBurst++;
		}
		else
		{
			fprintf(f, "Time %d: IDLE\n", time);
		}
	}
	
	fprintf(f, "Finished at time %d\n\n", time);
	
	int x;
	for(x = 0; x < processCount; x++)
	{
		fprintf(f, "%s wait %d turnaround %d\n", queue[order[x]].name, wait[order[x]], turnaround[order[x]]);
	}
	
	fclose(f);
	free(wait);
	free(turnaround);
}

int sjfSelect(struct process *queue, int size)
{
	int minBurst = INT_MAX;
	int select = -1;
	int x;
	
	for(x = 0; x < size; x++)
	{
		if(queue[x].burst < minBurst && queue[x].burst > 0)
		{
			minBurst = queue[x].burst;
			select = x;
		}
	}
	return select;
}

void updateWait(process *queue, int *wait, int burstSize, int currentProcess, int limit)
{
	int x;
		for(x = 0; x < limit; x++)
		{
			if(x != currentProcess && queue[x].burst != 0)
				wait[x] += burstSize;
		}
}

#endif
