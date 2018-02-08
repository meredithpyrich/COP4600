#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1000

int processCount;
int runFor;
int quantum;

struct process
{
	char * name;
	int arrival;
	int burst;
};

// argc - The number of arguments
// argv - Array of the contents of each argument
main(int argc, char ** argv)
{
	int i;
	
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
	
	if (ipf != NULL)
	{
		// char holder - carries one line of the input file at a time.
		char holder[MAX_INPUT_SIZE];
		while(fgets(holder, MAX_INPUT_SIZE, ipf) != NULL)
		{
			// Print the input file (remove this later)
			fprintf(stdout, "%s", holder);

		} 
		
		fclose(ipf);
	}
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
