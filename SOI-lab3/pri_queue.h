#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include <pthread.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdbool.h>

typedef struct{
    int *queueID;
    size_t queueMax;
    size_t head;
    size_t tail;
    bool is_full;
} PriQueue;

typedef PriQueue *QueueP;

QueueP init(QueueP pointer, int *queueBuff, size_t size);

void clearQueue(QueueP pointer);

bool push(QueueP pointer,int data, int pri);

char *pop(QueueP pointer);

void moveForw(QueueP pointer);

void moveBack(QueueP pointer);

bool isEmpty(QueueP pointer);

bool isFull(QueueP pointer);

size_t queueCapacity(QueueP pointer);

size_t queueSize(QueueP pointer);

void printQueue(QueueP pointer);

#endif // QUEUE_H_INCLUDED
