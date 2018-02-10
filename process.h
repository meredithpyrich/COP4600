#ifndef PROCESS_STRUCT
#define PROCESS_STRUCT

#ifndef MAX_INPUT_SIZE
#define MAX_INPUT_SIZE 1000
#endif

//I perform a typedef. You do not need to put the word 'struct' when defining a new process 
typedef struct process
{
	char name[MAX_INPUT_SIZE];
	int arrival;
	int burst;
} process;

#endif