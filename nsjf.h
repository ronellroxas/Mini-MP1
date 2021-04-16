#include<math.h>
#include<stdlib.h>
#include"process.h"
#include<stdio.h>

/**
 * Non-preemptive Shortest-Job First (NSJF)
 */
 
  struct id_time2 { //process struct but only the index num, arrival time, and execution time
 	int num;
 	int time;
 	int e_time;
 };
 
  void nsjf(Process *processList, int y) {
 	int i = 0; //counters
 	int j = 0;
 	int k = 0;
 	struct id_time2 temp; //temporary struct var for sorting
 	int curr_time = 0; //current cpu time
 	struct id_time2 pos[y]; //id_time array, basically a representation of processList so that the original doesn't need to be sorted later
 	
 	for (i = 0; i < y; i++) { //fills in the array of id_times with the processList index and arrival time values
 		pos[i].num = i;
 		pos[i].time = processList[i].arrivalTime;
 		pos[i].e_time = processList[i].executionTime;
	 }
 		
 	for (i = 0; i < y; i++) { //go through each process in the list and update values
 		
	 	for (j = i; j < y - 1; j++) { //bubble sort the id_times array from smallest to largest arrival time
	 		for (k = i; k < (y - 1 - j); k++) {
	 			if (pos[k].time > pos[k+1].time) {
	 				temp = pos[k];
	 				pos[k] = pos[k+1];
	 				pos[k+1] = temp;
				}
			 }
		}
		
		if (curr_time < pos[i].time) //if current time is less than the process with the smallest arrival time, change it to that
 			curr_time = pos[i].time;
		
 		if (i != (y - 1))
	 		for (j = i + 1; j < y; j++) { //find the available process with the smallest execution time
	 			if ((pos[j].time <= curr_time) && (pos[j].e_time < pos[i].e_time)) {
	 				temp = pos[i];
	 				pos[i] = pos[j];
	 				pos[j] = temp;
				 }
			}

 		Process *process = &processList[pos[i].num]; //new process pointer that contains the address of the current process
 		process->waitingTime = abs(curr_time - process->arrivalTime); //set waiting time
 		process->startTimes = (int*) malloc(sizeof(int)); //dynamically allocate start time array
		process->endTimes = (int*) malloc(sizeof(int)); //dynamically allocate end time array
		process->startTimes[0] = curr_time; //set start time
		process->endTimes[0] = curr_time + process->executionTime; //set end time
		curr_time =  process->endTimes[0]; //set new current time
		process->turnAroundTime = process->waitingTime + process->executionTime; //set turnaround time
 		process->timeSize = 1; //set timesize
	}
 }
