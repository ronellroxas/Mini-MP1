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

/**
 *Structure for an individual process
 * processID is input a
 * arrivalTime is input b
 * executionTime is input c
 */
struct ProcessStruct {
	int processID;
	int arrivalTime;
	int executionTime;
};

typedef struct ProcessStruct Process; 

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
		print(x, y, z);											//test function for import (from fcfs.c)
		
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
		
		for(i = 0; i < y; i++) {	//Print processes input for checking
			printf("ProcessID: %d || Arrival: %d || Execution: %d\n", processList[i].processID, processList[i].arrivalTime, processList[i].executionTime);
		}
		
		switch(x) {
			case 0:	
				//FCFS
				break;
			case 1:	
				//NSJF
				break;
			case 2:	
				//PSJF
				break;
			case 3:	
				//RR
				break;
			default:
				printf("Invalid algorithm input!\n");
				break;
		}
		
		
		free(processList);	//free malloc
	}
	else {	//if filename not found
		printf("File %s not found!", filename);
	}
	
	fclose(fp);
	
	return 0;
}
