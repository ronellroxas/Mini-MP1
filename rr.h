#include<math.h>
#include<stdlib.h>
#include"process.h"

/**
 * Calculates total remaining execution time
 */
int totalRemaining(Process *processList, int y) {
	int i = 0;
	int total = 0;
	for(i = 0; i < y; i++) {
		total += processList[i].executionTime;
	}
	return total;
}

/**
 * Round-Robin (RR)
 */
void rr(Process *processList, int y, int z) {
	int i = 0;
	int *timeIndeces;	//storage for time indices for process
	int currTime = 0;
	
	//create dynamic time indeces array then set values to 0
	timeIndeces = (int*) malloc(y * sizeof(int));
	memset(timeIndeces, 0, y * sizeof(int));
	
	
	int c = 0;
	for(c = 0; c < y; c++) {	//dynamically allocate start and end times array for each process
		Process *process = &processList[c];
		//dynamically allocates array based on time slice
		process->timeSize = (int) ceil((float)totalRemaining(processList, y)/(float)z);	//set timeSize
		process->startTimes	= (int*) malloc(process->timeSize * sizeof(int));
		process->endTimes = (int*) malloc(process->timeSize * sizeof(int));
		
		//set times to 0
		memset(process->startTimes, 0, process->timeSize  * sizeof(int));
		memset(process->endTimes, 0, process->timeSize  * sizeof(int));
		process->waitingTime = 0;
		process->turnAroundTime = process->executionTime;	//set to execution time only then add waiting time later
	}
	
	
	//while there is still process with remaining time
	while(totalRemaining(processList, y) > 0) {
		
		Process *process = &processList[i];
		
		//printf("ID:%d\n", process->processID);
		//if process has remaining time
		if(process->executionTime > 0) {
			//set start time
			process->startTimes[timeIndeces[i]]= currTime;
			
			process->waitingTime += currTime;							//waiting time formula part 1 (Summation(startTimes) - Summation(endTimes[i - 1]))
	
			if(process->executionTime > z) {							//if time remaining is longer than time slice
				process->endTimes[timeIndeces[i]] = currTime + z;		//add z to current time for end time
				currTime += z;
				process->executionTime -= z;							//reduce execution time of process
			}
			else {																			//if time remaining is less than or equal to time slice
				process->endTimes[timeIndeces[i]] = currTime + process->executionTime;		//add exec time to curr time for end time
				currTime += process->executionTime;
				process->executionTime = 0;													//set exec time to 0 since process is finished
			}
			
			//if not last process, subtract to waiting time
			if(process->executionTime > 0) {							//waiting time formula part 2 (Summation(startTimes) - Summation(endTimes[i - 1]))
				process->waitingTime -= process->endTimes[timeIndeces[i]];
			}
			else {
				process->turnAroundTime += process->waitingTime;	//add waiting time to turnAroundTime
			}
			//printf("%d %d\n", process->startTimes[timeIndeces[i]], process->endTimes[timeIndeces[i]]);
			timeIndeces[i]++;
		}
		
		i++;	
		i = i%y;	//round robin back to start
	}
	
	free(timeIndeces);
}
