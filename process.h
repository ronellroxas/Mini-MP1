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
	int quantum;		//time quantum left for a queue, resets when moving from one queue to another
	int lastqueue; 		//the index of the last queue the process was in; used for returning to a queue after an i/o burst
	int nextio; 		//starts off the same number as ioInterval; when decreased to 0, perform an i/o burst, resets back to ioInterval
	int remExeTime;		//remaining execution time left, basically executionTime but this decreases 
	int inProcess;		//determines if the process has recorded the current cpu burst yet or not; 0 if false, 1 if true
};

typedef struct ProcessStruct Process;

#endif


