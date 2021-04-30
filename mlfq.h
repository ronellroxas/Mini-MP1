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

//main mlfq function
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
		
		queue->processList = (Process*) malloc((y) * sizeof(int));
	}
    int total = totalRemaining(processList, y);

    int currTime = processList[0].arrivalTime;
    
    Queue *currQueue = &queueList[0];//initialize starting queue
    Process dummy; //initialize starting process as dummy data (to indicate that the 1st process hasn't arrived yet)
    dummy.processID = -23;
    Process *currProcess = &dummy;	
    int currQuantum = currQueue->quantum;	//initialize as first quantum
    int currQueueIndex = 0;	//initialize queueIndex
    int currProcessIndex = 0;
    
    currQueue->processList = processList;
    
    
    c = 0; //will act as the counter for new processes going into the queue
    //per second loop
    while(currTime <= total) {    	
    	printf("%d\n", currTime);
    	//put new processes into the top priority queue if their arrival time == current time
    	for(c = c; c < y; c++) {
    		if (processList[c].arrivalTime == currTime) {
    			if(queueList[0].length == 0) {	//if arriving at empty queue
    				queueList[0].head = &processList[c];		
			}
			Process *queueProcessListTail = &(queueList[0].processList[queueList[0].length]);
			queueProcessListTail = &processList[c];	//add to queue.processList
			queueList[0].length++;
		} else {
			break;
		}
    	}
    	
		//process execution
		if(currQueue->length > 0 && currProcess->executionTime > 0 && currProcess->processID != -23) {
			//if there is still quantum on queue
			if(currQueue->quantum > 0) {
				//if first run, set startTime and queue time
				printf("%d %d\n", currQuantum, currQueue->quantum);
				if(currQuantum == currQueue->quantum) {
					printf("in\n");
					currProcess->startTimes[currProcess->timeSize] = currTime;
					currProcess->queueTimes[currProcess->timeSize] = currQueue->queueID;
					currProcess->timeSize++;
				}
				
				currProcess->executionTime--;
				currQueue->quantum--;	
				
				//if quantum or exec run out, set endTime
				if(currQueue->quantum == 0 || currProcess->executionTime == 0) {
					currProcess->endTimes[currProcess->timeSize - 1] = currTime + 1;	//set endTime	
					//currQueue->quantum = currQuantum;
				}
			}
			else {
				//if quantum is 0, and process still not finished
				if(currProcess->executionTime > 0) {
					printf("abc\n");
					Queue *nextQueue = &queueList[(currQueueIndex + 1)%x];	//get next queue
				
					//move process to next queue
					nextQueue->head = currProcess;
					nextQueue->length++;
					
				}
				
				//move to next process
				currProcessIndex = (currProcessIndex + 1)%y;
				currProcess = &processList[currProcessIndex];		//move to next process in queue
				//remove and replace process from current queue
				if(currQueue->length > 0) {
					currQueue->head = currProcess;	//set head to next process
				}
				else {
					currQueue->head = NULL;
				}
				currQueue->length--;					
				
				//reset quantum
				currQueue->quantum = currQuantum;
				currTime--;	//decrement to undo increment later (since if it reaches this part, no process was executed)
			}
		}
		else {
			currProcessIndex = (currProcessIndex + 1)%y;
			currProcess = &processList[currProcessIndex];		//move to next process in queue
			if(currQueue->length > 0) {
				currQueue->head = currProcess;		//change head	
				currQueue->quantum = currQuantum;	//reset quantum	
			}	
			else {
				currQueue->head = NULL;
				currQueue = &queueList[(currQueueIndex + 1)%x];	//move to next queue
				currQuantum = currQueue->quantum;	//change quantum to next queue;
				
				currQueueIndex++;
			}
			currQueue->length--;				//change length
		}
		printf("Q[%d] QU:%d P:%d S:%d E:%d EX:%d\n", currQueue->queueID, currQueue->quantum, currProcess->processID, currProcess->startTimes[0], currProcess->endTimes[0], currProcess->executionTime);
		currTime++;
    }

}
