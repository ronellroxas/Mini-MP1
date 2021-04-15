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
};

typedef struct ProcessStruct Process;

#endif


