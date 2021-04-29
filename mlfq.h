#include<math.h>
#include<stdlib.h>
#include"process.h"
#include"queue.h"
#include<stdio.h>

void mlfq(Queue *queueList, Process *processList, int x, int y) {
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
            if(queueList[i].priority < queueList[z].priority) {
                Queue temp;
                temp = queueList[i];
                queueList[i] = queueList[z];
                queueList[z] = temp;
            }
        }
    }
}