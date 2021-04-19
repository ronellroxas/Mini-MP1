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

int getEmptyMap(int *map, int length, int start) {	//returns the next empty space in the map (0)
	int z = start;
	while(z < length) {
		if(map[z] == 0) {
			return z;
		}
		
		z++;
	}
}

/**
 * Preemptive Shortest-Job First (PSJF)
 */
void psjf(Process *processList, int y) {
	int totalTime = totalRemaining(processList, y);
	int x, a;
	int *timeIndeces;	//storage for time indices for process
	
	for(x = 0; x < y; x++) {	//sort by arrival time
		for(a = x; a < y; a++) {
			if(processList[x].arrivalTime > processList[a].arrivalTime) {
				Process temp = processList[x];
				processList[x] = processList[a];
				processList[a] = temp;
			}
		}
	}

	//create dynamic time indeces array then set values to 0
	timeIndeces = (int*) malloc(y * sizeof(int));
	memset(timeIndeces, 0, y * sizeof(int));
	
	int c = 0;
	for(c = 0; c < y; c++) {	//dynamically allocate start and end times array for each process
		Process *process = &processList[c];
		//dynamically allocates array based on time slice
		process->timeSize = 0;	//set timeSize (dynamic yere
		process->startTimes	= (int*) malloc(y*2 * sizeof(int));
		process->endTimes = (int*) malloc(y*2 * sizeof(int));
		
		//set times to 0
		memset(process->startTimes, 0, y*2  * sizeof(int));
		memset(process->endTimes, 0, y*2  * sizeof(int));
		process->waitingTime = 0;
		process->turnAroundTime = process->executionTime;	//set to execution time only then add waiting time later
		
	}
	
	int i = 0;
	int *map;		//array for storing time map, for checking if time slot is taken by other processes
	map = malloc(totalTime * sizeof(int));
	memset(map, 0, totalTime * sizeof(int));	//initialize map to 0

	int currIndex = 0;	//current index of process
	
	Process *process = &processList[0];				//current process	
	Process *next = &processList[1];
	process->startTimes[0] = process->arrivalTime;	//set initial start time
	i = process->arrivalTime;
	
	//special case, starting process
	while(currIndex == 0 && process->executionTime > 0) {
		if(next->arrivalTime == i) {
			if(next->executionTime < process->executionTime) {
				currIndex++;
				process->endTimes[0] = i;
				timeIndeces[0]++;
				process->timeSize++;
			}
			else {
				next = (next + 1);
				map[i] = 1;
				process->executionTime--; 	
			}
		}
		else {
			map[i] = 1;
			process->executionTime--; 
		}
		i++;
	}
	
	//per second loop
	while(totalRemaining(processList, y) > 0) {
		Process *process = &processList[currIndex];	//current process	
		
		int z = 0;
		int minIndex = 0;	//index of process with minimum exec time
		int minValue = 9999999;	//minimum execution time	
		int indexCheck = 1;
		//get smallest execution time
		for(z = 0; z < y; z++) {
			if(processList[z].executionTime > 0 && processList[z].executionTime < minValue) {
				minValue = processList[z].executionTime;
				minIndex = z;
			}
		}
		currIndex = minIndex;
		process = &processList[currIndex];

		if(map[process->arrivalTime] == 0) {	//if space in map is still empty
			process->startTimes[timeIndeces[currIndex]] = process->arrivalTime;	//set to normal arrival time
			process->endTimes[timeIndeces[currIndex]] = process->arrivalTime + process->executionTime;	//set end to execution time
			//printf("%d index:%d arrive:%d start:%d end:%d\n", process->timeSize, currIndex, process->arrivalTime, process->startTimes[timeIndeces[currIndex]], process->endTimes[timeIndeces[currIndex]]);	
		}
		else {	//get new arrival time if map space is empty
		 	process->startTimes[timeIndeces[currIndex]] = getEmptyMap(map, totalTime, process->arrivalTime);
			process->endTimes[timeIndeces[currIndex]] = process->startTimes[timeIndeces[currIndex]] + process->executionTime;
		}
		
		//save timeframe to map
		int x = process->startTimes[timeIndeces[currIndex]];
		while(x < process->endTimes[timeIndeces[currIndex]]) {
			if(map[x] == 0) {
				map[x] = 1;
			}
			else {
				process->endTimes[timeIndeces[currIndex]] = x;
				break;
			}
			process->executionTime--;
			x++;
		}
		
		process->timeSize++;	
		timeIndeces[currIndex]++;

	
	}

	//calculate weight	
	for(i = 0; i < y; i++) {
		Process *process = &processList[i];
		for(c = 0; c < process->timeSize; c++) {			//add starting times together
			process->waitingTime += process->startTimes[c];
		}
		for(c = 0; c < process->timeSize - 1; c++) {		//reduce n-1 end times together for waiting
			process->waitingTime -= process->endTimes[c];
		}
		process->waitingTime -= process->arrivalTime;
		process->turnAroundTime += process->waitingTime;
	}
	
	free(map);
	free(timeIndeces);
}
