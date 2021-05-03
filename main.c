/********************************************
 *	Names: 		CHUA CHIACO, Ronn Christian
 *				ROXAS, Ronell John 
 *	Group:		26 
 *	Section: 	S-16
 ********************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"mlfq.h"
#include"process.h"
#include"queue.h"

//orders processes by process time for output
void orderByStartingTime(Process *processList, int y) {
	int i, x;
	Process temp;	
	
	for(i = 0; i < y - 1; i++) {
		for(x = i; x < y; x++) {
			if(processList[i].waitingTime > processList[x].waitingTime) {	//sort by waitingTimes
				temp = processList[x];
				processList[x] = processList[i];
				processList[i] = temp;
			}
		}
	}
}

//calculates the waiting time and turnaround time of the processes 
void calcOutput(Process *processList, int y, int q_count) {
	int i = 0;
	for(i = 0; i < y; i++) {
		int wait_time = 0;
		int x = 0;
		int total_io = 0; //total io burst count
		for (x = 0; x < processList[i].timeSize; x++) {
			if (processList[i].queueTimes[x] == (q_count + 1)) { //only cpu bursts are counted, i/o bursts don't
				total_io++;
			} else { 
				if (x == 0) { //calculate waiting time for first cpu burst
					wait_time = wait_time + (processList[i].startTimes[x] - processList[i].arrivalTime);
				} else if (x == processList[i].timeSize - 1) { //calculate waiting time for last cpu burst
					wait_time = wait_time + (processList[i].startTimes[x] - processList[i].endTimes[x-1]);
				} else { //calculate waiting time for other cpu bursts
					wait_time = wait_time + (processList[i].startTimes[x] - processList[i].endTimes[x-1]);
				}
			}
		}
		processList[i].waitingTime = wait_time;
		processList[i].turnAroundTime = wait_time + processList[i].executionTime + (total_io * processList[i].ioBurst);		
	}
}

//basic printing function following spec format
void printOutput(Process *processList, int y, int x) {
	int i = 0;
	float total_wait = 0;
	
	orderByStartingTime(processList, y);	//order by starting time
	
	for(i = 0; i < y; i++) {
		printf("P[%d]\n", processList[i].processID);
		int z = 0;

		for(z = 0; z < processList[i].timeSize; z++) {
			if(processList[i].queueTimes[z] == (x + 1)) {		
				printf("[IO] Start time: %-2d End time: %-2d\n",  processList[i].startTimes[z], processList[i].endTimes[z]);
			} else {
				printf("Q[%d] Start time: %-2d End time: %-2d\n",  processList[i].queueTimes[z],processList[i].startTimes[z], processList[i].endTimes[z]);
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
			queueList = (Queue*) malloc((x+1)*sizeof(Queue));
			processList = (Process*) malloc((y) * sizeof(Process));	//create processList array of size y

			int it = 0;
			int a, b, c;
			
			//get queues
			for(it= 0; it < x; it++) {
				fscanf(fp, "%d %d %d\n", &a, &b, &c);

				queueList[it].queueID = a;
				queueList[it].priority = b;
				queueList[it].quantum = c;
				queueList[it].capacity = y;
				queueList[it].head = 0;
				queueList[it].length = 0;
				queueList[it].tail = y - 1;
				queueList[it].processList = (Process**)malloc((y) * sizeof(Process*));
			}
			
			int f, g, h, i, j;
			//get processes
			for(it = 0; it < y; it++) {
				fscanf(fp, "%d %d %d %d %d\n", &f, &g, &h, &i, &j);
			
				//store to array entry
				processList[it].processID = f;
				processList[it].arrivalTime = g;
				processList[it].executionTime = h;
				processList[it].remExeTime = h;
				processList[it].ioBurst = i;
				processList[it].ioInterval = j;
				processList[it].inProcess = 0;
				if (i == 0 || j == 0) {
					processList[it].nextio = -1;	//set next io burst time to -1 since the process won't be needing it anyway
				} else {
					processList[it].nextio = j;	
				}
				processList[it].timeSize = 0;
				//dynamically allocate arrays
				processList[it].startTimes = (int*) malloc(h*2 * sizeof(int)); //allocate h * 2 for the times arrays since the number of times recorded would be exactly h*2 if the io burst freq. is 1
				processList[it].endTimes = (int*) malloc(h*2 * sizeof(int));
				processList[it].queueTimes = (int*) malloc(h*2 * sizeof(int));
				//set times to 0
				memset(processList[it].startTimes, 0, h*2  * sizeof(int));
				memset(processList[it].endTimes, 0, h*2  * sizeof(int));
        			memset(processList[it].queueTimes, 0, h*2  * sizeof(int));
				processList[it].waitingTime = 0;
				processList[it].turnAroundTime = 0;
			}

			//run mlfq
			mlfq(queueList, processList, x, y, s);
			//calculate the waiting and turnaround times of the processes
			calcOutput(processList, y, x);
			//print the process details
			printOutput(processList, y, x);

			//free child mallocs then struct malloc
			for(c = 0; c < y; c++) {	//free space of times per process
				Process process = processList[c];
				free(process.startTimes);
				free(process.endTimes);
				free(process.queueTimes);
			}
			
			//free queue.processList
			for(c = 0; c < x; c++) {	//free space of times per queue
				free(queueList[c].processList);
			}
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
