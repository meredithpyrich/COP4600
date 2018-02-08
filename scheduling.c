#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1000

int processCount;
int runFor;
int quantum;

void fcfs();
void sjf();
void rr();
void bubblesort(int[]);
void printArray(int[]);

// process is divided into name, arrival, and burst time
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
void fcfs(int arr[])
{
    // step 1: arrivalTime global variable is used to determine which process runs first
    // step 2: use passed in array and sort processes using bubble sort
    bubblesort(arr);
    // step 3:
}
// Shortest Job First
void sjf()
{

}
// Round Robin
void rr()

{

}
// use bubble sort to sort array by arrivaltime
void bubblesort(int arr[])
{
    int curProcess1, curProcess2;
    for (curProcess1 = 0; curProcess1 < processCount; curProcess1++)
    {
        for (curProcess2 = 0; curProcess2 < processCount-curProcess1-1; curProcess2++)
        {
            if (arr[curProcess2] > arr[curProcess2+1])
            {
                int temp = arr[curProcess2];
                arr[curProcess2] = arr[curProcess2+1];
                arr[curProcess2+1] = temp;
            }
        }
    }
    printArray(arr);
}
//
void printArray(int arr[])
{
    int curProcess;
    for (curProcess = 0; curProcess < processCount; curProcess++)
    {
        printf("%d", arr[curProcess]);

        if(curProcess < processCount - 1)
            printf(" ");
    }
    printf("\n");
}
