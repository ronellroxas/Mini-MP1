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
    int length; //current size of queue
    int head; //index of head process
    int tail; //index of last process
    int capacity; //max size of queue
    Process **processList;   //process array list
};

typedef struct queueStruct Queue;

#endif


