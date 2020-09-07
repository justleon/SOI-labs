#include <stdio.h>
#include "pri_queue.h"

QueueP init(QueueP pointer, int *queueBuff, size_t size){
    pointer->queueMax = size;
    pointer->queueID = queueBuff;
    pointer->head = 0;
    pointer->tail = 0;
    pointer->is_full = false;

    return pointer;

    /*semNames[0] = semMutex;
    semNames[1] = semEmpty;
    semNames[2] = semFull;
    sMutex = sem_open(semNames[0], O_CREAT | O_EXCL, 0666, 1);
    sEmpty = sem_open(semNames[1], O_CREAT | O_EXCL, 0666, queueCap);
    sFull = sem_open(semNames[2], O_CREAT | O_EXCL, 0666, 0);
    queueID = (int*) shmget(key, sizeof(int) * queueCap, IPC_CREAT | 0666);*/
}

void clearQueue(QueueP pointer){
    if(!isEmpty(pointer)){
        for(int i = pointer->tail; i < pointer->tail + queueSize(pointer); i++){
            int id = pointer->queueID[i % pointer->queueMax];
            shmctl(id, IPC_RMID, NULL);
        }
    }
    /*sem_close(sMutex);
    sem_close(sEmpty);
    sem_close(sFull);
    sem_unlink(semNames[0]);
    sem_unlink(semNames[1]);
    sem_unlink(semNames[2]);*/
}

bool push(QueueP pointer, int data, int pri){
    if(!isFull(pointer)){
        if(pri == 1){
            size_t new_tail;
            if(pointer->tail != 0)
                new_tail = pointer->tail - 1;
            else
                new_tail = pointer->queueMax - 1;
            pointer->queueID[new_tail] = data;
            pointer->tail = new_tail;
            pointer->is_full = (pointer->head == pointer->tail);

            return true;
        } else{
            pointer->queueID[pointer->head] = data;
            moveForw(pointer);
            return true;
        }
    }

    return false;
}

char *pop(QueueP pointer){
    char *data;

    if(!isEmpty(pointer)){
        int id = pointer->queueID[pointer->tail];
        data = (char *) shmat(id, NULL, 0);
        shmctl(id, IPC_RMID, NULL);
        moveBack(pointer);
    }

    return data;
}

void moveForw(QueueP pointer){
    if(isFull(pointer))
        pointer->tail = (pointer->tail + 1) % pointer->queueMax;

    pointer->head = (pointer->head + 1) % pointer->queueMax;
    pointer->is_full = (pointer->head == pointer->tail);
}

void moveBack(QueueP pointer){
    pointer->is_full = false;
    pointer->tail = (pointer->tail + 1) % pointer->queueMax;
}

bool isEmpty(QueueP pointer){
    return (!pointer->is_full && (pointer->head == pointer->tail));
}

bool isFull(QueueP pointer){
    return pointer->is_full;
}

size_t queueCapacity(QueueP pointer){
    return pointer->queueMax;
}

size_t queueSize(QueueP pointer){
    size_t s = pointer->queueMax;

    if(!isFull(pointer)){
        if(pointer->head >= pointer->tail)
            s = pointer->head - pointer->tail;
        else
            s = pointer->queueMax + pointer->head - pointer->tail;
    }

    return s;
}

void printQueue(QueueP pointer){
    if(!isEmpty(pointer)){
        for(int i = pointer->tail; i < pointer->tail + queueSize(pointer); i++){
            int id = pointer->queueID[i % pointer->queueMax];
            char *str = (char *)shmat(id, NULL, 0);
            printf("%s <- ", str);
        }
        printf("\n");
    } else{
        puts("The queue is empty!");
    }
}
