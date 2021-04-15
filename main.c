/********************************************
 *	Names: 		CHUA CHIACO, Ronn Christian
 *				ROXAS, Ronell John 
 *	Group:		26 
 *	Section: 	S-16
 ********************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"fcfs.h"
#include"nsjf.h"
#include"psjf.h"
#include"rr.h"
#include"process.h"

//basic printing function following spec format
void printOutput(Process *processList, int y) {
	int i = 0;
	float total_wait = 0;
	for(i = 0; i < y; i++) {
		printf("P[%d]\n", processList[i].processID);
		int z = 0;
		for(z = 0; z < processList[i].timeSize; z++) {
			if(processList[i].startTimes[z] + processList[i].endTimes[z] > 0) {
				printf("Start time: %d End time: %d\n", processList[i].startTimes[z], processList[i].endTimes[z]);
			}
		}
		printf("Waiting time: %d\n", processList[i].waitingTime);
		printf("Turnaround time: %d\n", processList[i].turnAroundTime);
		total_wait+=(float)processList[i].waitingTime;
		printf("***********************\n");
	}
	printf("Average waiting time: %.2f\n", total_wait/(float)y);
	
}

int main() {
	FILE *fp;
	int x, y, z;
	char filename[21];		//21 characters filename
	Process *processList;	//address for dynamic array of Processes
	
	//get filename
	printf("Filename: ");
	scanf("%s", filename);
	
	if(strcmp(".txt", filename + (strlen(filename) - 4))) {	//check if last 4 characters is ".txt"
		strcat(filename, ".txt");							//add .txt to end if not in input filename	
	}

	//open file
	fp = fopen(filename , "r");
	
	if(fp != NULL) {											//if file found
		fscanf(fp, "%d %d %d\n", &x, &y, &z);	
		
		processList = (Process*) malloc(y * sizeof(Process));	//create processList array of size y
		
		int i;
		for(i = 0; i < y; i++) {					//Get all process infos
			int a, b, c;							//Process value holders
			fscanf(fp, "%d %d %d\n", &a, &b, &c);	//Get individual process info (format: A B C\n)
			
			//store to array entry
			processList[i].processID = a;
			processList[i].arrivalTime = b;
			processList[i].executionTime = c;
		}
		
//		for(i = 0; i < y; i++) {	//Print processes input for checking
//			printf("ProcessID: %d || Arrival: %d || Execution: %d\n", processList[i].processID, processList[i].arrivalTime, processList[i].executionTime);
//		}
		
		switch(x) {
			case 0:	
				//FCFS
				fcfs(processList, y);
				printOutput(processList, y);
				break;
			case 1:	
				//NSJF
				break;
			case 2:	
				//PSJF
				break;
			case 3:	
				//RR
				rr(processList, y, z);
				printOutput(processList, y);
				break;
			default:
				printf("Invalid algorithm input!\n");
				break;
		}
		
		//free child mallocs then struct malloc
		int c = 0;
		for(c = 0; c < y; c++) {	//free space of times per process
			Process process = processList[c];
			free(process.startTimes);
			free(process.endTimes);
		}
		free(processList);
		
	}
	else {	//if filename not found
		printf("File %s not found!", filename);
	}
	
	fclose(fp);
	
	return 0;
}
