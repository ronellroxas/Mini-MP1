/**
 *Structure for an individual queue
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include"process.h"

struct queueStruct {
	int queueID;
    int priority;
    int quantum;
    int length;
    Process *processList;   //process array list
    Process *head;
};

typedef struct queueStruct Queue;

#endif


