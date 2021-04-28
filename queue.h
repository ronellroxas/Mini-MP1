/**
 *Structure for an individual queue
 */

#ifndef QUEUE_H_
#define QUEUE_H_

struct queueStruct {
	int queueID;
    int priority;
    int quantum;
};

typedef struct queueStruct Queue;

#endif


