/**
 *Structure for an individual process
 * processID is input a
 * arrivalTime is input b
 * executionTime is input c
 */

#ifndef PROCESS_H_
#define PROCESS_H_

struct ProcessStruct {
	int processID;
	int arrivalTime;
	int executionTime;
	int *startTimes;	//dynamic array for starting times
	int *endTimes;		//dynamic array for starting times
	int *queueTimes;
	int waitingTime;
	int turnAroundTime;
	int timeSize;		//size of start and end time arrays
	int ioBurst;		//I
	int ioInterval;		//J
};

typedef struct ProcessStruct Process;

#endif


