#include<math.h>
#include<stdlib.h>
#include"process.h"
#include"queue.h"
#include<stdio.h>

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

//move head process in q1 to q2
void moveTopToQueue(Queue *q1, Queue *q2) {
    Process *q2List = &q2->processList[q2->length];
    q2List = q1->head;
    q2->length++;
    q1->length--;
    if(q1->length > 0) {
        q1->head = &q1->processList[1];
    }
    else {
        q1->head = NULL;
    }

}

//finds if a processID is in another queue (q)
int processInQueue(Queue *queueList, int processID, int x) {
    int i = 0, z = 0;
    for(i = 0; i < x; i++) {
    	if(queueList[i].length > 0) {
	        for(z = 0; z < queueList[i].length; z++) {
	            if(queueList[i].processList[z].processID == processID) {
	                return i;
	            }
	        }
	    }
    }
    return -1;
}

void mlfq(Queue *queueList, Process *processList, int x, int y) {
    int i = 0, z = 0;
    int *timeIndeces;	//storage for time indices for process

    //sort processes arrival time
    for(i = 0; i < y; i++) {
        for(z = i; z < y; z++) {
            if(processList[i].arrivalTime > processList[z].arrivalTime) {
                Process temp;
                temp = processList[i];
                processList[i] = processList[z];
                processList[z] = temp;
            }
        }
    }
	
    //sort queue by priority
    for(i = 0; i < x; i++) {    
        queueList[i].length = 0;    //initialize lengths
        for(z = i; z < x; z++) {
            if(queueList[i].priority > queueList[z].priority) {
                Queue temp;
                temp = queueList[i];
                queueList[i] = queueList[z];
                queueList[z] = temp;
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
        process->queueTimes = (int*) malloc(y*2 * sizeof(int));
		
		//set times to 0
		memset(process->startTimes, 0, y*2  * sizeof(int));
		memset(process->endTimes, 0, y*2  * sizeof(int));
        memset(process->queueTimes, 0, y*2  * sizeof(int));
		process->waitingTime = 0;
		process->turnAroundTime = process->executionTime;	//set to execution time only then add waiting time later
		
	}

	for(c = 0; c < x + 1; c++) {	//allocate memory for queue.processList (+1 for the additional IO queue)
		Queue *queue = &queueList[c];
		
		queue->processList = (Process*) malloc(y * sizeof(int));
	}
    int total = totalRemaining(processList, y);

    int currTime = processList[0].arrivalTime;
    int currProcessIndex = 0;
    int currQueueIndex = 0;
    
    //per second loop
    while(currTime <= total) {
        Process *process = &processList[currProcessIndex];
        int timeIndex = timeIndeces[currProcessIndex];
        
        //if process has remaining time
        if(process->executionTime > 0) {
            Queue *queue = &queueList[currQueueIndex];

            Process *queueProcess = queue->processList + queue->length;
            queueProcess = process;  //add process to queue
            queue->length++;
                
            if(queue->head == NULL) {    //for top queue, initial insert
                queue->head = process;
            }
	
            process->startTimes[timeIndex] = currTime;          //set process start time
            process->queueTimes[timeIndex] = queue->queueID;    //set queue time
            if(process->executionTime > queue->quantum) {       //if execution is greater than quantum
                process->executionTime -= queue->quantum;       //execute time
                Queue *nextQueue = &queueList[(currQueueIndex + 1)%x];
                moveTopToQueue(queue, nextQueue);               //move process to next queue
            }
            else {
                process->executionTime = 0;
                queue->length--;
            }
            currTime += queue->quantum;
            if(queue->length = 0) {
                queue->head = NULL;
            }
            process->endTimes[timeIndex] = currTime;    //set end time
            process->timeSize++;
            timeIndeces[currProcessIndex]++;            //increase time index	
            
            //check if round robin
            if(currProcessIndex == y - 1) { //will only trigger if process already round-robined---
                currQueueIndex++;                                       //--- which means we should move to next queue
            }
    	}

        currTime++;
        currProcessIndex = (currProcessIndex+1)%y;
    }

}
