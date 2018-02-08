#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1000
#define MAX_NAMING_SIZE 100
#define DEBUG 1

int processCount = 0;
int runFor = 0;
int quantum = 0;

struct process
{
	char name[MAX_NAMING_SIZE];
	int arrival;
	int burst;
};

struct process * processes;

void fcfs();
void sjf();
void rr();

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
					
					processes = malloc(sizeof(struct process) * processCount);
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
		rr();
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
void rr()
{

}
