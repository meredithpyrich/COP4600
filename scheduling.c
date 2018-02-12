#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ShortestJob.h"
#include "process.h"

#define MAX_INPUT_SIZE 1000
#define MAX_NAMING_SIZE 100
#define DEBUG 1

int processCount = 0;
int runFor = 0;
int quantum = 0;

process * processes;

void bubbleSort();
void fcfs();
void sjf();
void rr();

// argc - The number of arguments
// argv - Array of the contents of each argument
int main(int argc, char ** argv)
{
	// Check if an input file was given at the command line.
	if (argc != 2)
	{
		printf("Invalid parameters.  The correct syntax is:\n\n");
		printf("./scheduling processes.in\n\n");
	}

	// Take the filename from the arguments and open the file
	char * filename = argv[1];
	FILE * ipf = NULL;
	ipf = fopen(filename, "r");

	int fcfsOn = 0;
	int sjfOn = 0;
	int rrOn = 0;
	int currentProcess = 0;

	if (ipf != NULL)
	{
		// char holder - carries one line of the input file at a time.
		char holder[MAX_INPUT_SIZE];
		char * token;
		while(fgets(holder, MAX_INPUT_SIZE, ipf) != NULL)
		{
			// Print the input file (remove this later)
			//fprintf(stdout, "%s", holder);

			token = strtok(holder, " ");
			//printf("%s\n", token);

			int ranIntoHashtag = 0;

			while (token != NULL)
			{
				if (strcmp(token, "#") == 0)
				{
					ranIntoHashtag = 1;
				}

				if (ranIntoHashtag)
				{
					break;
				}
				else if (strcmp(token, "processcount") == 0)
				{
					token = strtok(NULL, " ");
					processCount = atoi(token);

					processes = malloc(sizeof(process) * processCount);
				}
				else if (strcmp(token, "runfor") == 0)
				{
					token = strtok(NULL, " ");
					runFor = atoi(token);
				}
				else if (strcmp(token, "use") == 0)
				{
					token = strtok(NULL, " ");

					if (strcmp(token, "fcfs") == 0)
						fcfsOn = 1;
					else if (strcmp(token, "sjf") == 0)
						sjfOn = 1;
					else
						rrOn = 1;
				}
				else if (strcmp(token, "quantum") == 0)
				{
					token = strtok(NULL, " ");
					quantum = atoi(token);
				}
				else if (strcmp(token, "process") == 0)
				{
					char programName[MAX_NAMING_SIZE];

					token = strtok(NULL, " ");
					token = strtok(NULL, " ");

					// Get program name
					strcpy(processes[currentProcess].name, token);

					token = strtok(NULL, " ");
					token = strtok(NULL, " ");

					// Get Arrival
					processes[currentProcess].arrival = atoi(token);

					token = strtok(NULL, " ");
					token = strtok(NULL, " ");

					// Get Burst
					processes[currentProcess].burst = atoi(token);

					currentProcess++;
				}
				else
				{
					break;
				}

				token = strtok(NULL, " ");

			}
		}

		// Bubble Sort
		bubbleSort();

		if (DEBUG)
		{
			printf("%s %d\n", "processCount = ", processCount);
			printf("%s %d\n", "runFor = ", runFor);
                        if (quantum != 0)
			    printf("%s %d\n\n", "quantum = ", quantum);

			int j;
			for (j = 0; j < processCount; j++)
			{
				printf("%s %s\n%s %d\n%s %d\n\n", "Process Name:", processes[j].name, "Arrival:", processes[j].arrival, "Burst:", processes[j].burst);
			}
		}

		fclose(ipf);
	}

	if (fcfsOn)
	{
		fcfs();
	}
	else if (sjfOn)
	{
		sjf(processes, processCount, runFor);
	}
	else if (rrOn)
	{
		rr();
	}
	free(processes);

	return 0;
}
// use bubble sort to sort array by arrival time
void bubbleSort()
{
    int curProcess1, curProcess2;
    for (curProcess1 = 0; curProcess1 < processCount; curProcess1++)
    {
	for (curProcess2 = 0; curProcess2 < processCount-curProcess1-1; curProcess2++)
	{
	    if (processes[curProcess2].arrival > processes[curProcess2+1].arrival)
	    {
		process temp = processes[curProcess2];
		processes[curProcess2] = processes[curProcess2+1];
		processes[curProcess2+1] = temp;
	    }
	}
    }
}

// First Come First Served
void fcfs()
{
    printf("%d processes\n", processCount);
    printf("Using First Come First Served\n\n");

    int curTime, i, j, nextArrival = 0, curProcess = -1, curBurst = 0;
    int *waitTime, *turnaroundTime;

    waitTime = calloc(processCount, sizeof(int));
    turnaroundTime = calloc(processCount, sizeof(int));

    for (curTime = 0; curTime < runFor; curTime++)
    {
        // while loop is to make sure multiple files coming in
        // at the same time will be counted individually (good idea)
        while (nextArrival < processCount && processes[nextArrival].arrival == curTime)
        {
            printf("Time %d: %s arrived\n", curTime, processes[nextArrival].name);
            // next process is one after current process
            int nextProcess = fcfsSelect(nextArrival+1);

            if (nextProcess != curProcess)
            {
                // while program is not idle
                if (nextProcess != -1)
                {
                    curBurst = 0;
                    curProcess = nextProcess;
                    printf("Time %d: %s selected (burst %d)\n", curTime, processes[curProcess].name, processes[curProcess].burst);
                }
            }
            nextArrival++;
        }
        // if process is finished running
        if (processes[curProcess].burst == 0)
        {
            printf("Time %d: %s finished\n", curTime, processes[curProcess].name);
            turnaroundTime[curProcess] = (curTime-processes[curProcess].arrival);
            // calculate wait time
            waitTime[curProcess] = (curTime-processes[curProcess].arrival-curBurst);
            curBurst = 0;
            curProcess = fcfsSelect(nextArrival);
            if (curProcess != -1)
                printf("Time %d: %s selected (burst %d)\n", curTime, processes[curProcess].name, processes[curProcess].burst);
        }
        if (curProcess != -1)
        {
            // decrement the burst time in the queue and increment the burst for wait time calculation
            processes[curProcess].burst--;
            curBurst++;
            if (curTime == runFor-1)
            {
                printf("Time %d: %s finished\n", curTime+1, processes[curProcess].name);
                waitTime[curProcess] = ((curTime+1)-processes[curProcess].arrival-curBurst);
                turnaroundTime[curProcess] += ((curTime+1)-processes[curProcess].arrival);
            }
        }
        else
            printf("Time %d: IDLE\n", curTime);
    }
    printf("Finished at time %d\n\n", curTime);

    for (i = 0; i < processCount; i++)
    {
        printf("%s wait %d turnaround %d\n", processes[i].name, waitTime[i], turnaroundTime[i]);
    }

    free(waitTime);
    free(turnaroundTime);
}
int fcfsSelect(int nextProcessAfter)
{
    int selectProcess = -1;
    int curProcessSelect;
    for (curProcessSelect = 0; curProcessSelect < nextProcessAfter; curProcessSelect++)
    {
        if (processes[curProcessSelect].burst > 0)
        {
            selectProcess = curProcessSelect;
            break;
        }
    }
    return selectProcess;
}
// Round Robin
void rr()
{

}
