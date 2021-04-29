/********************************************
 *	Names: 		CHUA CHIACO, Ronn Christian
 *				ROXAS, Ronell John 
 *	Group:		26 
 *	Section: 	S-16
 ********************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"process.h"
#include"queue.h"

//orders processes by process time for output
void orderByStartingTime(Process *processList, int y) {
	int i, x;
	Process temp;	
	
	for(i = 0; i < y - 1; i++) {
		for(x = i; x < y; x++) {
			if(processList[i].waitingTime > processList[x].waitingTime) {	//sort by starting time of first executions
				temp = processList[x];
				processList[x] = processList[i];
				processList[i] = temp;
			}
		}
	}
}

//basic printing function following spec format
void printOutput(Process *processList, int y) {
	int i = 0;
	float total_wait = 0;
	
	orderByStartingTime(processList, y);	//order by starting time
	
	for(i = 0; i < y; i++) {
		printf("P[%d]\n", processList[i].processID);
		int z = 0;
		for(z = 0; z < processList[i].timeSize; z++) {
			if(processList[i].startTimes[z] + processList[i].endTimes[z] > 0) {
				printf("Start time: %-2d End time: %-2d\n", processList[i].startTimes[z], processList[i].endTimes[z]);
			}
		}
		printf("Waiting time: %d\n", processList[i].waitingTime);
		printf("Turnaround time: %d\n", processList[i].turnAroundTime);
		total_wait+=(float)processList[i].waitingTime;
		printf("****************************\n");
	}
	printf("Average waiting time: %.2f\n", total_wait/(float)y);
	
}

//Input validation. Check if value is within min max (inclusive)
int valid(int value, int min, int max) {
	return value >= min && value <= max;
}

int main() {
	FILE *fp;
	int x, y, s;
	char filename[21];		//21 characters filename
	Process *processList;	//address for dynamic array of Processes
	Queue *queueList;
	
	//get filename
	printf("Filename: ");
	scanf("%s", filename);
	
	if(strcmp(".txt", filename + (strlen(filename) - 4))) {	//check if last 4 characters is ".txt"
		strcat(filename, ".txt");							//add .txt to end if not in input filename	
	}

	//open file
	fp = fopen(filename , "r");
	
	if(fp != NULL) {											//if file found
		fscanf(fp, "%d %d %d\n", &x, &y, &s);	

		//input range validation
		if(valid(x, 2, 5) && valid(y, 3, 100)) {
			queueList = (Queue*) malloc(x*sizeof(Queue));
			processList = (Process*) malloc(y * sizeof(Process));	//create processList array of size y

			int it = 0;
			int a, b, c;
			//get queues
			for(it= 0; it < x; it++) {
				fscanf(fp, "%d %d %d\n", &a, &b, &c);

				queueList[it].queueID = a;
				queueList[it].priority = b;
				queueList[it].quantum = c;
			}
			
			int f, g, h, i, j;
			//get processes
			for(it = 0; it < y; it++) {
				fscanf(fp, "%d %d %d %d %d\n", &f, &g, &h, &i, &j);
				
				//store to array entry
				processList[it].processID = f;
				processList[it].arrivalTime = g;
				processList[it].executionTime = h;
				processList[it].ioBurst = i;
				processList[it].ioInterval = j;
			}
			//free child mallocs then struct malloc
			// int c = 0;
			// for(c = 0; c < y; c++) {	//free space of times per process
			// 	Process process = processList[c];
			// 	free(process.startTimes);
			// 	free(process.endTimes);
			// }
			free(queueList);
			free(processList);
		}
		else {
			printf("Invalid input format from file!\n");
		}
	}
	else {	//if filename not found
		printf("File %s not found!", filename);
	}
	
	fclose(fp);
	
	return 0;
}
