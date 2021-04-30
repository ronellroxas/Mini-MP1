#include<math.h>
#include<stdlib.h>
#include"process.h"
#include"queue.h"
#include<stdio.h>
#include<string.h>

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
    			processList[c].quantum = queueList[0].quantum; //set process quantum to top queue's quantum
    			processList[c].lastqueue = 0; //set process current queue to the index of top queue
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
    	
		
	if (currProcess->processID != -23) {
		// do nothing if the current process is still the dummy/no processes have arrived yet
	} else if(currQueue->length > 0) { //process execution as long as the current queue has processes left
	
		//if start of cpu burst, set startTime and queue time
		printf("%d %d\n", currProcess->quantum, currQueue->quantum);
		if(currProcess->inProcess == 0) {
			printf("in\n");
			currProcess->startTimes[currProcess->timeSize] = currTime;
			currProcess->queueTimes[currProcess->timeSize] = currQueue->queueID;
			currProcess->timeSize++;
			currProcess->inProcess == 1;
		}
				
		//decrease the exe time, quantum, and next io burst times by 1;
		currProcess->remExeTime--;
		currProcess->quantum--;
		currProcess->nextio--;
				
		//increase current time by 1
		currTime++;
				
		//if either exe time, quantum, or next io burst time is 0, record end time
		if(currProcess->nextio == 0 || currProcess->remExeTime == 0 || currProcess->quantum == 0) {
			currProcess->endTimes[currProcess->timeSize - 1] = currTime;	//set endTime	
		}
				
		//if the next io burst time is 0 move process to io queue and start io burst (not applied when process is already finished)
		if(currProcess->nextio == 0 && currProcess->remExeTime > 0) {
			if (currProcess->quantum == 0) { //if quantum goes to 0 at the same time then make process's last queue to the lower prio queue
				if (currProcess->lastqueue < x - 1) {
					currProcess->lastqueue = currQueueIndex + 1;
					currProcess->quantum = queueList[currProcess->lastqueue].quantum; //reset quantum to new queue's quantum
				} else if (currProcess->lastqueue == x - 1) {
					currProcess->quantum = queueList[currProcess->lastqueue].quantum; //reset quantum to the same queue's quantum
				}
			}
			currProcess->nextio = currProcess->ioInterval; //reset next io burst time
			//record start and end times of io burst
			currProcess->startTimes[currProcess->timeSize] = currTime;
			currProcess->queueTimes[currProcess->timeSize] = x;
			currProcess->endTimes[currProcess->timeSize] = currTime + currProcess->ioBurst;
			currProcess->timeSize++;
		}
				
		//if quantum goes to 0, move to lower prio queue (not applied when process is already finished)
		if(currProcess->quantum == 0 && currProcess->remExeTime > 0) {
			Queue *nextQueue;
			if (currProcess->lastqueue < x - 1) {
				nextQueue = &queueList[(currQueueIndex + 1)%x];	//get next queue
				currProcess->lastqueue = currQueueIndex + 1; //change process's last queue to the new queue's index
			} else if (currProcess->lastqueue == x - 1) { //if lowest prio queue already just move process to the tail end of the queue
				nextQueue = &queueList[(currQueueIndex)%x];	//get the same queue as the "new queue"
			}
			currProcess->quantum = nextQueue->quantum; //reset quantum to new queue's quantum
			if (nextQueue->length == 0) //set the current process to the head of the new queue if it's empty
				nextQueue->head = currProcess;
			moveTopToQueue(currQueue, nextQueue); //moves curr process from the old queue to the new queue
		}
		
		//TO DO: EVERYTHING BELOW THIS COMMENT	
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
		else { //change queue
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
		printf("Q[%d] QU:%d P:%d S:%d E:%d EX:%d\n", currQueue->queueID, currQueue->quantum, currProcess->processID, currProcess->startTimes[0], currProcess->endTimes[0], currProcess->remExeTime);
		
    }

}
