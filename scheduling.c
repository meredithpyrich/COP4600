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

void fcfs(process * processes, int * order, int processCount, int runFor);
int fcfsSelect(process *processes, int nextProcessAfter);

//Performs the scheduling function
void sjf(process *queue, int * order, int processCount, int runFor);
//Detects the next process
int sjfSelect(process *queue, int size);
//Updates the wait times after a given burst
void updateWait(process *queue, int *wait, int burstSize, int currentProcess, int limit);

void rr(process * processes, int processCount, int runFor, int quantum);

void bubbleSort(process * processes, int * order, int processCount);

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
	int * order;
	
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
					order = malloc(sizeof(int) * processCount);
					
					int count;
					for (count = 0; count < processCount; count++)
					{
						order[count] = count;
					}
				}
				else if (strcmp(token, "runfor") == 0)
				{
					token = strtok(NULL, " ");
					runFor = atoi(token);
				}
				else if (strcmp(token, "use") == 0)
				{
					token = strtok(NULL, " ");

					if (token[0] == 'f')
						fcfsOn = 1;
					else if (token[0] == 's')
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
		bubbleSort(processes, order, processCount);
		fcfs(processes, order, processCount, runFor);
	}
	else if (sjfOn)
	{
		bubbleSort(processes, order, processCount);
		sjf(processes, order, processCount, runFor);
	}
	else if (rrOn)
	{
		rr(processes, processCount, runFor, quantum);
	}

	free(processes);
	free(order);
}

// First Come First Served
void fcfs(process * processes, int * order, int processCount, int runFor)
{
    FILE *fout = fopen("processes.out", "w");

    fprintf(fout, "%d processes\n", processCount);
    fprintf(fout, "Using First Come First Served\n\n");

    int curTime, curIdx, nextArrival = 0, curProcess = -1, curBurst = 0, idle = -1;
    int *waitTime = calloc(processCount, sizeof(int));
    int *turnaroundTime = calloc(processCount, sizeof(int));

    for (curTime = 0; curTime < runFor; curTime++)
    {
        // while loop is to make sure multiple files coming in
        // at the same time will be counted individually (good idea)
        while (nextArrival < processCount && processes[nextArrival].arrival == curTime)
        {
            fprintf(fout, "Time %d: %s arrived\n", curTime, processes[nextArrival].name);
            // next process is one after current process if current process is done
            int nextProcess = fcfsSelect(processes, nextArrival+1);

            if (nextProcess != curProcess)
            {
                // while process is not idle
                if (nextProcess != -1)
                {
                    curBurst = 0;
                    curProcess = nextProcess;
                    fprintf(fout, "Time %d: %s selected (burst %d)\n", curTime, processes[curProcess].name, processes[curProcess].burst);
                }
            }
            nextArrival++;
        }
        // if a process is finished running
        if (processes[curProcess].burst == 0)
        {
            fprintf(fout, "Time %d: %s finished\n", curTime, processes[curProcess].name);
            // calculate turnaround time
            turnaroundTime[curProcess] = (curTime-processes[curProcess].arrival);
            // calculate wait time
            waitTime[curProcess] = (curTime-processes[curProcess].arrival-curBurst);
            // select next process or identify as idle
            curBurst = 0;
            curProcess = fcfsSelect(processes, nextArrival);
            // select the next process to run
            if (curProcess != -1)
                fprintf(fout, "Time %d: %s selected (burst %d)\n", curTime, processes[curProcess].name, processes[curProcess].burst);
        }
        // if a process is not finished running
        if (curProcess != -1)
        {
            // decrement the burst time in processes and increment the burst for wait time calculation
            processes[curProcess].burst--;
            curBurst++;
            // if a process is running in the last time interval
            if (curTime == runFor-1)
            {
                // check if process finishes and if yes, output finish time
                if (processes[curProcess].burst == 0)
                {
                    fprintf(fout, "Time %d: %s finished\n", curTime+1, processes[curProcess].name);
                    waitTime[curProcess] = ((curTime+1)-processes[curProcess].arrival-curBurst);
                    turnaroundTime[curProcess] += ((curTime+1)-processes[curProcess].arrival);
                }
                else
                    fprintf(fout, "Time %d: %s terminated\n", curTime+1, processes[curProcess].name);
            }
        }
        else
            fprintf(fout, "Time %d: IDLE\n", curTime);
    }
    fprintf(fout, "Finished at time %d\n\n", curTime);
    // print wait time and turnaround time for each process
    for (curIdx = 0; curIdx < processCount; curIdx++)
    {
        fprintf(fout, "%s wait %d turnaround %d\n", processes[order[curIdx]].name, waitTime[order[curIdx]], turnaroundTime[order[curIdx]]);
    }

    fclose(fout);
    free(waitTime);
    free(turnaroundTime);
}

int fcfsSelect(process *processes, int nextProcessAfter)
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

// Shortest Job First
void sjf(process *queue, int * order, int processCount, int runFor)
{
	FILE *f = fopen("processes.out", "w");
	fprintf(f, "%d processes \n", processCount);
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
			updateWait(queue, wait, currentBurst, currentProcess, nextArrival);
			currentBurst = 0;
			turnaround[nextArrival] -= time;
			
			int nextProcess;
			fprintf(f, "Time %d: %s arrived\n", time, queue[nextArrival].name); 
			nextProcess = sjfSelect(queue, nextArrival + 1);
			if(nextProcess != -1 && nextProcess != currentProcess)
			{
				currentProcess = nextProcess;
				fprintf(f, "Time %d: %s selected (burst %d)\n", time, queue[currentProcess].name, queue[currentProcess].burst);	
			}
			nextArrival++;
		}
		//A process with a burst time of 0 is considered done
		if (currentProcess > -1 && queue[currentProcess].burst == 0)
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

int sjfSelect(process *queue, int size)
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

// Round Robin
void rr(process * processes, int processCount, int runFor, int quantum)
{
	FILE * f = fopen("processes.out", "w");
	fprintf(f, "%d processes \n", processCount);
	fprintf(f, "Using Round-Robin\n");
	fprintf(f, "Quantum %d", quantum);
	fprintf(f, "\n\n");

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
				fprintf(f, "%s %d: %s %s\n", "Time", i, processes[j].name, "arrived");
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
				fprintf(f, "%s %d: %s\n", "Time", i, "IDLE");
			}
			else
			{
				fprintf(f, "%s %d: %s %s (%s %d)\n", "Time", i, processes[cycle].name, "selected", "burst", burstTime[cycle]);
			}			

			if (burstTime[cycle] < quantum && finished[cycle] != 1)	
			{
				remainingBurst = burstTime[cycle];
				burstTime[cycle] = 0;
				fprintf(f, "%s %d: %s %s\n", "Time", i+remainingBurst, processes[cycle].name, "finished");
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
	fprintf(f, "%s %d\n", "Finished at time", runFor);	

	fprintf(f, "\n");
	for (i = 0; i < processCount; i++)
	{
		int turnaround = (finishedTime[i]-arrivedTime[i])+1;
		
		fprintf(f, "%s %s %d %s %d\n", processes[i].name, "wait", (turnaround - processes[i].burst), "turnaround", turnaround);
	}
}

void bubbleSort(process * processes, int * order, int processCount)
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

		int orderTemp = order[curProcess2];
		order[curProcess2] = order[curProcess2+1];
		order[curProcess2+1] = orderTemp;
	    }
	}
    }
}
