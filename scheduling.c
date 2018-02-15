#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_INPUT_SIZE 1000
#define MAX_NAMING_SIZE 100
#define DEBUG 0

typedef struct process
{
	char name[MAX_NAMING_SIZE];
	int arrival;
	int burst;
}process;

void fcfs();
void sjf();
void rr(process * processes, int processCount, int runFor, int quantum);

void bubbleSort(process * processes, int processCount);

// argc - The number of arguments
// argv - Array of the contents of each argument
main(int argc, char ** argv)
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

	int processCount = 0;
	int runFor = 0;
	int quantum = 0;

	process * processes;
	
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
					int arrival;
					int burst;

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

		if (DEBUG)
		{
			printf("%s %d\n", "processCount = ", processCount);
			printf("%s %d\n", "runFor = ", runFor);
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
		sjf();
	}
	else if (rrOn)
	{
		rr(processes, processCount, runFor, quantum);
	}

	free(processes);
}

// First Come First Serve
void fcfs()
{

}

// Shortest Job First
void sjf()
{

}

// Round Robin
void rr(process * processes, int processCount, int runFor, int quantum)
{

	printf("%d %s\n", processCount, "processes");
	printf("Using Round Robin\n");
	printf("%s %d\n\n", "Quantum", quantum);
	
	int i, j;
	int arrived[processCount];
	int arrivedTime[processCount];
	int finished[processCount];
	int finishedTime[processCount];
	int burstTime[processCount];
	int cycle = 0;
	int remainingBurst = 0;
	int loopCount = 0;

	for (i = 0; i < processCount; i++)
		arrived[i] = 0;	

	for (i = 0; i < runFor; i++)
	{

		for (j = 0; j < processCount; j++)
		{

			if (processes[j].arrival == i)
			{
				printf("%s %d: %s %s\n", "Time", i, processes[j].name, "arrived");
				arrived[j] = 1;
				arrivedTime[j] = i;
				burstTime[j] = processes[j].burst;
			}			
		}
	
		if (!(remainingBurst > 0))
		{
			loopCount = 0;
			// Cycle through until we have a program that's ready and needs to be run
			while (arrived[cycle] != 1 && burstTime[cycle] != 0)
			{
				cycle++;
				if (cycle == processCount)
				{
					cycle = 0;
				}
				loopCount++;
				if (loopCount == processCount)
					break;
			}
			
			if (burstTime[cycle] == 0)
			{
				printf("%s %d: %s\n", "Time", i, "Idle");
			}
			else
			{
				printf("%s %d: %s %s (%s %d)\n", "Time", i, processes[cycle].name, "selected", "burst", burstTime[cycle]);
			}			

			if (burstTime[cycle] < quantum && finished[cycle] != 1)	
			{
				remainingBurst = burstTime[cycle];
				burstTime[cycle] = 0;
				printf("%s %d: %s %s\n", "Time", i+remainingBurst, processes[cycle].name, "finished");
				finished[cycle] = 1;
				finishedTime[cycle] = i;				
			}		
			else
			{
				remainingBurst = quantum;
				burstTime[cycle] -= quantum;
			}

			cycle++;

			if (cycle == processCount)
			{
				cycle = 0;
			}

		}
		
		remainingBurst--;

	}
	printf("%s %d\n", "Finished at time", runFor);	

	printf("\n");
	for (i = 0; i < processCount; i++)
	{
		int turnaround = (finishedTime[i]-arrivedTime[i])+1;
		
		printf("%s %s %d %s %d\n", processes[i].name, "wait", (turnaround - processes[i].burst), "turnaround", turnaround);
	}
}

void bubbleSort(process * processes, int processCount)
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
