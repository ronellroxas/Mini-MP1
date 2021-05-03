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
		total += processList[i].remExeTime;
	}
	return total;
}

//adds process to a queue
void enqueue(Queue *q, Process *pr) {
	if (q->length == q->capacity) //return if already full 
		return;
	q->tail = (q->tail + 1) % q->capacity;
	q->processList[q->tail] = pr;
	q->length++;	
}

//removes process from the head of a queue
Process* dequeue(Queue *q) {
	Process *pr = q->processList[q->head];
	q->head = (q->head + 1) % q->capacity;
	q->length--;
	return pr;
}

//get process at the head of a queue
Process* get_head(Queue *q) {
	return q->processList[q->head];
}

//get process at the tail of a queue
Process* get_tail(Queue *q) {
	return q->processList[q->tail];
}


//move head process in q1 to q2
/*
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
*/

//finds if a processID is in another queue (q)
int processInQueue(Queue *queueList, int processID, int x) {
    int i = 0, z = 0;
    for(i = 0; i < x; i++) {
    	if(queueList[i].length > 0) {
	        for(z = 0; z < queueList[i].length; z++) {
	            if(queueList[i].processList[z]->processID == processID) {
	                return i;
	            }
	        }
	    }
    }
    return -1;
}

//main mlfq function
void mlfq(Queue *queueList, Process *processList, int x, int y, int s) {
    int i = 0, z = 0;

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
        for(z = i; z < x; z++) {
            if(queueList[i].priority > queueList[z].priority) {
                Queue temp;
                temp = queueList[i];
                queueList[i] = queueList[z];
                queueList[z] = temp;
            }
        }
    }

    //initialize variables and allocate memory for IO queue
    	queueList[x].queueID = -1;
	queueList[x].priority = -1;
	queueList[x].quantum = -1;
	queueList[x].capacity = y;
	queueList[x].head = 0;
	queueList[x].length = 0;
	queueList[x].tail = y - 1;
	queueList[x].processList =  (Process**)malloc((y) * sizeof(Process*));
	

	printf("prio boost %d \n", s);
	
	
    int total = totalRemaining(processList, y);
    int currTime = processList[0].arrivalTime;
    Queue *currQueue = &queueList[0];//initialize starting queue
    int currQueueIndex = 0;
    Process dummy; //initialize starting process as dummy data (to indicate that the 1st process hasn't arrived yet)
    dummy.processID = -23;
    Process *currProcess = &dummy;	
    int c = 0; //will act as the counter for new processes going into the queue
    int prioBoost = s;	//act as counter for priority boost
    //per second loop
    while(total > 0) {    	
    	printf("TIME: %d\n", currTime);
    	//put new processes into the top priority queue if their arrival time == current time
    	for(c = c; c < y; c++) {
	    	if (processList[c].arrivalTime == currTime) {
	    		processList[c].quantum = queueList[0].quantum; //set process quantum to top queue's quantum
	   			processList[c].lastqueue = 0; //set process current queue to the index of top queue
	   			enqueue(&queueList[0], &processList[c]); //add process to top queue
	   			printf("Process %d added for the first time .\n", processList[c].processID);
			} else {
				break;
			}
    	}
    	
    	//15 - move finished IO back to original queue
    			int len_io = queueList[x].length;
			for(i = 0; i < len_io; i++) {
				Queue *IOqueue = &queueList[x];
				Process *temp = get_head(IOqueue);
				printf("IO LOOKING AT PROCESS: %d when io len is %d\n", temp->processID, IOqueue->length);
				if(temp->endTimes[temp->timeSize - 1] == currTime) {		//check if an IO process finished, based on last endTime = currTime (endTime set when process was moved to IO)
					temp->nextio = temp->ioInterval;	//reset IO 
					temp->inProcess = 0;	//set to 0 to mark that it left IO, adds start time
					dequeue(IOqueue);
					enqueue(&queueList[temp->lastqueue], temp);	//return back to last queue
					//check queue prio happens on next loop
					printf("Process %d finished IO burst, adding back to queue %d .\n", temp->processID, temp->lastqueue + 1);
					printf("queue %d SIZE is %d\n", queueList[temp->lastqueue].queueID, queueList[temp->lastqueue].length);
				}
			}
		
    	//5 - check if currqueue is not highest prio
    	for(i = 0; i < x; i++) {
    		if(currQueue->queueID != queueList[i].queueID) {	//check first if it is not the same queue
    			//if currQueue has lower priority and other queue has a process
    			if(currQueue->priority > queueList[i].priority && queueList[i].length > 0) {
    				if (currProcess->inProcess) { //if there's a process currently running finish it abruptly
    					currProcess->endTimes[currProcess->timeSize - 1] = currTime;	//set endTime
					printf("Process %d ended CPU burst. Halted abruptly\n", currProcess->processID);
					currProcess->inProcess = 0;
					dequeue(currQueue);
					enqueue(currQueue, currProcess);
				}
    				currQueue = &queueList[i];	//Change currQueue to higher priority(move to higher prio queue)
    				currQueueIndex = i;
    				printf("Current Queue is %d found higher prio queue.\n", currQueue->queueID);
    				
			}
		}
	}
	
	//if currQueue is the top queue and is empty, check if lower queues have a process
	if (currQueue->queueID == queueList[0].queueID && currQueue->length == 0) {
		for (i = 1; i < x;i++) {
			if (queueList[i].length > 0) {
				currQueue = &queueList[i];	//Change currQueue to higher priority(move to higher prio queue)
    				currQueueIndex = i;
    				printf("Current Queue is %d found in lower prio queue.\n", currQueue->queueID);
    				break;
			}	
		}
	}
    	
    	//make the head of the current queue the current process if its arrival time <= curr time and queue isn't empty
    	if (currQueue->length == 0) {
		dummy.processID = -11; //create a dummy process that tells the loop that there aren't any processes available yet
		currProcess = &dummy;
	} else if (get_head(currQueue)->arrivalTime <= currTime) {
    		currProcess = get_head(currQueue);
    		printf("Current process is %d.\n", currProcess->processID);
    	}
    	
		
		if (currProcess->processID == -23) {
			// do nothing if the current process is still the dummy/no processes have arrived yet
			printf("No process has arrived yet\n");
		} else  { //process execution as long as the current queue has processes left
		
		
		if (currProcess->processID == -11) {
			// do nothing if the current process is still the dummy/no processes have arrived yet
			printf("No process has arrived yet a");
			currTime++;
		} else {
			//if start of cpu burst, set startTime and queue time
			//printf("%d %d\n", currProcess->quantum, currQueue->quantum);
			if(currProcess->inProcess == 0 || currProcess->quantum == currQueue->quantum) {
				currProcess->startTimes[currProcess->timeSize] = currTime;
				currProcess->queueTimes[currProcess->timeSize] = currQueue->queueID;
				currProcess->timeSize++;
				currProcess->inProcess = 1;
				printf("Process %d started CPU burst.\n", currProcess->processID);
			}
					
			//decrease the exe time, quantum, and next io burst times by 1;
			currProcess->remExeTime--;
			currProcess->quantum--;
			currProcess->nextio--;		
			//increase current time by 1
			currTime++;
			printf("TIME: %d\n", currTime);
					
			//if either exe time, quantum, or next io burst time is 0, record end time
			if(currProcess->nextio == 0 || currProcess->remExeTime == 0 || currProcess->quantum == 0) {
				currProcess->endTimes[currProcess->timeSize - 1] = currTime;	//set endTime
				printf("Process %d ended CPU burst.\n", currProcess->processID);
				currProcess->inProcess = 0; //process no longer in process
			}
					
			//if the next io burst time is 0 move process to io queue and start io burst (not applied when process is already finished)
			if(currProcess->nextio == 0 && currProcess->remExeTime > 0) {
				if (currProcess->quantum == 0) { //if quantum goes to 0 at the same time then make process's last queue to the lower prio queue; same queue if already the lowest prio queue
					if (currProcess->lastqueue < x - 1) {
						printf("Diff queue after IO\n");
						currProcess->lastqueue = currQueueIndex + 1;
						currProcess->quantum = queueList[currProcess->lastqueue].quantum; //reset quantum to new queue's quantum
					} else if (currProcess->lastqueue == x - 1) {
						printf("Same queue after IO\n");
						currProcess->quantum = queueList[currProcess->lastqueue].quantum; //reset quantum to the same queue's quantum
					}
				}
				currProcess->nextio = currProcess->ioInterval; //reset next io burst time
				//move currProcess from currQueue to io queue
				enqueue(&queueList[x], dequeue(currQueue));
				//record start and end times of io burst
				currProcess->startTimes[currProcess->timeSize] = currTime;
				currProcess->queueTimes[currProcess->timeSize] = x + 1;
				currProcess->endTimes[currProcess->timeSize] = currTime + currProcess->ioBurst;
				currProcess->timeSize++;
				
				printf("Process %d started IO burst. ", currProcess->processID);
				printf("will finish at %d ms\n", currTime + currProcess->ioBurst);
			}		
			//if quantum goes to 0, move to lower prio queue (not applied when process is already finished)
			else if(currProcess->quantum == 0 && currProcess->remExeTime > 0) {
				Queue *nextQueue;
				if (currProcess->lastqueue < x - 1) {
					nextQueue = &queueList[currQueueIndex + 1];	//get next queue
					currProcess->lastqueue = currQueueIndex + 1; //change process's last queue to the new queue's index
				} else if (currProcess->lastqueue == x - 1) { //if lowest prio queue already just move process to the tail end of the queue
					nextQueue = currQueue;	//get the same queue as the "new queue"
				}
				currProcess->quantum = nextQueue->quantum; //reset quantum to new queue's quantum
				//moves curr process from the old queue to the new queue
				dequeue(currQueue);
				enqueue(nextQueue, currProcess);
				//currProcess = get_head(currQueue);
				printf("Process %d moved to queue %d\n", currProcess->processID, nextQueue->queueID);
				printf("QUEUE %d LEN: %d \n", nextQueue->queueID, nextQueue->length);
			}
			//remove process from queue if it's finished
			else if (currProcess->remExeTime == 0) {
				dequeue(currQueue);
				currProcess = get_head(currQueue);
				printf("Process %d FINISHED\n", currProcess->processID);
			}
			
			
			//PRIORITY BOOST
			if( (currTime >= prioBoost) && (currProcess->inProcess == 0)) {
				for(i = 1; i < x; i++) {		//loop to every queue except highest prio (0)
					int len = queueList[i].length;
					printf("LEN IS: %d\n", len);
					for(z = 0; z < len; z++) {	//loop to process of each queue
						Process *temp = dequeue(&queueList[i]);
						temp->quantum = queueList[0].quantum;
						temp->lastqueue = 0;
						enqueue(&queueList[0], temp);	//enqueue everything to highest prio queue
						printf("Process %d BOOSTED.\n", temp->processID);
					}
				}
				prioBoost += s;
				printf("PRIO BOOST TIME. next boost at %d \n", prioBoost);
			}
			
		}
			
			
			//change queue if curr queue is empty (will not go through if other queues are empty as well)
			
			if (currQueue->length == 0) {
				for (i = 0; i < x; i++) {
					if (queueList[i].length != 0) {
						currQueueIndex = (currQueueIndex + 1)%x;
						currQueue = &queueList[currQueueIndex];
						printf("Queue changed to %d. Older queue ran out of processes.\n", currQueue->queueID);
						break;
					}
				}
			}
			
			//calculate total remaining execution time for all processes
			total = totalRemaining(processList, y);
			
		}
		/*
		printf("Q[%d] QU:%d LEN:%d P:%d EX:%d TS:%d\n", currQueue->queueID, currProcess->quantum, currQueue->length, currProcess->processID, currProcess->remExeTime, currProcess->timeSize);
		for(i = 0; i < currProcess->timeSize; i++)
			printf("Q:%d S:%d E:%d\n", currProcess->queueTimes[i], currProcess->startTimes[i], currProcess->endTimes[i]);
		
		//sav currProcess to processList
		for(i = 0; i < y; i++) {	//find currProcess index in processList
			if(currProcess->processID == processList[i].processID) {
				processList[i] = *currProcess;	//save or copy process(not addressed)
			}
		}
		*/
    }

}
