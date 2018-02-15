#ifndef FIRST_COME_FIRST_SERVE
#define FIRST_COME_FIRST_SERVE

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "process.h"

//Performs the scheduling function
void fcfs(process *queue, int processCount, int runFor);
//Detects the next process
int fcfsSelect(struct process *queue, int size);

// First Come First Served
void fcfs(process *queue, int processCount, int runFor)
{
    FILE *fout = fopen("processes.out", "w");

    fprintf(fout, "%d processes\n", processCount);
    fprintf(fout, "Using First Come First Served\n\n");

    int curTime, i, j, nextArrival = 0, curProcess = -1, curBurst = 0;
    int *waitTime, *turnaroundTime;

    waitTime = calloc(processCount, sizeof(int));
    turnaroundTime = calloc(processCount, sizeof(int));

    for (curTime = 0; curTime < runFor; curTime++)
    {
        // while loop is to make sure multiple files coming in
        // at the same time will be counted individually (good idea)
        while (nextArrival < processCount && queue[nextArrival].arrival == curTime)
        {
            fprintf(fout, "Time %d: %s arrived\n", curTime, queue[nextArrival].name);
            // next process is one after current process
            int nextProcess = fcfsSelect(queue, nextArrival+1);

            if (nextProcess != curProcess)
            {
                // while program is not idle
                if (nextProcess != -1)
                {
                    curBurst = 0;
                    curProcess = nextProcess;
                    fprintf(fout, "Time %d: %s selected (burst %d)\n", curTime, queue[curProcess].name, queue[curProcess].burst);
                }
            }
            nextArrival++;
        }
        // if process is finished running
        if (queue[curProcess].burst == 0)
        {
            fprintf(fout, "Time %d: %s finished\n", curTime, queue[curProcess].name);
            turnaroundTime[curProcess] = (curTime-queue[curProcess].arrival);
            // calculate wait time
            waitTime[curProcess] = (curTime-queue[curProcess].arrival-curBurst);
            curBurst = 0;
            curProcess = fcfsSelect(queue, nextArrival);
            if (curProcess != -1)
                fprintf(fout, "Time %d: %s selected (burst %d)\n", curTime, queue[curProcess].name, queue[curProcess].burst);
        }
        if (curProcess != -1)
        {
            // decrement the burst time in the queue and increment the burst for wait time calculation
            queue[curProcess].burst--;
            curBurst++;
            if (curTime == runFor-1)
            {
                fprintf(fout, "Time %d: %s finished\n", curTime+1, queue[curProcess].name);
                waitTime[curProcess] = ((curTime+1)-queue[curProcess].arrival-curBurst);
                turnaroundTime[curProcess] += ((curTime+1)-queue[curProcess].arrival);
            }
        }
        else
            fprintf(fout, "Time %d: IDLE\n", curTime);
    }
    fprintf(fout, "Finished at time %d\n\n", curTime);

    for (i = 0; i < processCount; i++)
    {
        fprintf(fout, "%s wait %d turnaround %d\n", queue[i].name, waitTime[i], turnaroundTime[i]);
    }

    fclose(fout);
    free(waitTime);
    free(turnaroundTime);
}
int fcfsSelect(struct process *queue, int nextProcessAfter)
{
    int selectProcess = -1;
    int curProcessSelect;
    for (curProcessSelect = 0; curProcessSelect < nextProcessAfter; curProcessSelect++)
    {
        if (queue[curProcessSelect].burst > 0)
        {
            selectProcess = curProcessSelect;
            break;
        }
    }
    return selectProcess;
}

#endif
