#include<math.h>
#include<stdlib.h>
#include"process.h"
#include<stdio.h>

/**
 * First-Come First-Serve (FCFS)
 */
 
 struct id_time {
 	int num;
 	int time;
 };
 
 void fcfs(Process *processList, int y) {
 	int i = 0;
 	int j = 0;
 	struct id_time temp;
 	int curr_time = 0;
 	struct id_time pos[y];
 	
 	for (i = 0; i < y; i++) {
 		pos[i].num = i;
 		pos[i].time = processList[i].arrivalTime;
	 }
 		
 	
 	for (i = 0; i < y - 1; i++) { //bubble sort the process list from smallest to largest arrival time
 		for (j = 0; j < (y - 1 - i); j++) {
 			if (pos[j].time > pos[j+1].time) {
 				temp = pos[j];
 				pos[j] = pos[j+1];
 				pos[j+1] = temp;
 				
			}
 			
		 }
	}
	
 	
 	for (i = 0; i < y; i++) { //go through each process in the list
 		Process *process = &processList[pos[i].num];
 		if (curr_time < process->arrivalTime)
 			curr_time = process->arrivalTime;
 		process->waitingTime = abs(curr_time - process->arrivalTime);
 		process->startTimes = (int*) malloc(sizeof(int));
		process->endTimes = (int*) malloc(sizeof(int));
		process->startTimes[0] = curr_time;
		process->endTimes[0] = curr_time + process->executionTime;
		curr_time =  process->endTimes[0];
		process->turnAroundTime = process->waitingTime + process->executionTime;
 		process->timeSize = 1;
	}
 }
