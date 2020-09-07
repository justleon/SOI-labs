#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <string.h>
#include "queue.c"

#define QUEUE_SIZE 50
#define QUEUE_NUM 3

char *semMutexName[QUEUE_NUM] = {"mutex1", "mutex2", "mutex3"};
char *semEmptyName[QUEUE_NUM] = {"empty1", "empty2", "empty3"};
char *semFullName[QUEUE_NUM] = {"full1", "full2", "full3"};

sem_t *semMutex[QUEUE_NUM];
sem_t *semEmpty[QUEUE_NUM];
sem_t *semFull[QUEUE_NUM];

int queueBuff_id[QUEUE_NUM];
int queue_id[QUEUE_NUM];
int *queueBuff[QUEUE_NUM];
QueueP q[QUEUE_NUM];

typedef enum{
    NORM = 0, HIGH = 1
} priority;

int pr = 0;
const char letters[] = "ABC";

void initSem(){
    for(int i = 0; i < QUEUE_NUM; i++){
        semMutex[i] = sem_open(semMutexName[i], O_CREAT | O_EXCL, 0666, 1);
        semEmpty[i] = sem_open(semEmptyName[i], O_CREAT | O_EXCL, 0666, QUEUE_SIZE);
        semFull[i] = sem_open(semFullName[i], O_CREAT | O_EXCL, 0666, 0);
    }
}

void closeSem(){
    for(int i = 0; i < QUEUE_NUM; i++){
        sem_unlink(semMutexName[i]);
        sem_unlink(semEmptyName[i]);
        sem_unlink(semFullName[i]);
        sem_close(semMutex[i]);
        sem_close(semEmpty[i]);
        sem_close(semFull[i]);
    }
}

void produce(QueueP *q, int i, int message, int pri){
    sem_wait(semEmpty[i]);
    sem_wait(semMutex[i]);
    push(q[i], message, pri);
    //printQueue(q[i]);
    sem_post(semMutex[i]);
    sem_post(semFull[i]);
}

char *consume(QueueP *q, int i){
    char *rt;
    sem_wait(semFull[i]);
    sem_wait(semMutex[i]);
    rt = pop(q[i]);
    //printQueue(q[i]);
    sem_post(semMutex[i]);
    sem_post(semEmpty[i]);
    return rt;
}

int main(){
    FILE *results;
    sem_t *s;
    int i;
    int procNr = 7;
    pid_t pid;

    results = fopen("results.txt", "w");
    initSem();
    s = sem_open("pro", O_CREAT | O_EXCL, 0666, 1);

    queueBuff_id[0] = shm_open("/aqueuebuff", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
    queue_id[0] = shm_open("/aqueue", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
    queueBuff_id[1] = shm_open("/bqueuebuff", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
    queue_id[1] = shm_open("/bqueue", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
    queueBuff_id[2] = shm_open("/cqueuebuff", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
    queue_id[2] = shm_open("/cqueue", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
    for(int i = 0; i < QUEUE_NUM; i++){
        ftruncate(queueBuff_id[i], QUEUE_SIZE * sizeof(int));
        ftruncate(queue_id[i], sizeof(PriQueue));
        queueBuff[i] = (int *) mmap(NULL, sizeof(int) * QUEUE_SIZE,
            PROT_READ | PROT_WRITE, MAP_SHARED, queueBuff_id[i], 0);
        q[i] = (QueueP) mmap(NULL, sizeof(PriQueue),
            PROT_READ | PROT_WRITE, MAP_SHARED, queue_id[i], 0);

        q[i] = init(q[i], queueBuff[i], QUEUE_SIZE);
    }

    for(i = 0; i < procNr; i++){
        pid = fork();
        if(pid < 0){
            sem_unlink("pro");
            sem_close(s);
            puts("Something went wrong!");
        } else if(pid == 0) break;
        usleep(500000);
    }

    if(pid == 0){
        sem_wait(s);
        if(i == 0 || i == 1 || i == 2) printf("PRODUCER NR %d\n", i);
        else if(i == 3 || i == 4 || i == 5) printf("CONSUMER NR %d\n", i);
        else if(i == 6) printf("SPECIAL PRODUCER\n");
        sem_post(s);

        time_t measure;
        struct tm *locTime;

        switch(i){
            case 0:
            case 1:
            case 2:
                srand(time(NULL) ^ (getpid() << 16));
                printf("Producer nr %d\n", i);
                for(int j = 0; j < 20; j++){
                    int id = shmget(1500 + j, 4 * sizeof(char), IPC_CREAT | 0666);
                    char *msg = (char *)shmat(id, NULL, 0);
                    for(int k = 0; k < 3; k++){
                        *(msg + k) = letters[rand() % 3];
                    }
                    *(msg + 3) = '\0';

                    produce(q, i, id, NORM);
                    sem_wait(s);
                    time(&measure);
                    locTime = localtime(&measure);
                    printf("(%02d:%02d:%02d) Producer %d added %s to queue %d.\n",
                        locTime->tm_hour, locTime->tm_min, locTime->tm_sec, i, msg, i);
                    fprintf(results, "(%02d:%02d:%02d) Producer %d added %s to queue %d.\n",
                        locTime->tm_hour, locTime->tm_min, locTime->tm_sec, i, msg, i);
                    sem_post(s);
                    sleep(3);
                }
                exit(0);
                break;
            case 3:
            case 4:
            case 5:
                srand(time(NULL) ^ (getpid() << 16));
                printf("Consumer nr %d\n", i % 3);
                for(int j = 0; j < 20; j++){
                    char *temp;
                    temp = consume(q, i % 3);
                    sem_wait(s);
                    time(&measure);
                    locTime = localtime(&measure);
                    printf("(%02d:%02d:%02d) Consumed %s from %d.\n",
                        locTime->tm_hour, locTime->tm_min, locTime->tm_sec, temp, i % 3);
                    fprintf(results, "(%02d:%02d:%02d) Consumed %s from %d.\n",
                        locTime->tm_hour, locTime->tm_min, locTime->tm_sec, temp, i % 3);
                    sem_post(s);

                    if(temp == NULL){
                        continue;
                    }

                    usleep(1000 * 500);

                    if(strlen(temp) != 0){
                        int nr = temp[0] - 'A';
                        temp++;
                        int id = shmget(2500 + j, 4 * sizeof(char), IPC_CREAT | 0666);
                        char *msg = (char *)shmat(id, NULL, 0);
                        strcpy(msg, temp);

                        int prob = rand() % 100;
                        if(prob < pr){
                            *(msg + 2) = letters[rand() % 3];
                            *(msg + 3) = '\0';
                        }
                        sem_wait(s);
                        produce(q, nr, id, NORM);
                        time(&measure);
                        locTime = localtime(&measure);
                        printf("(%02d:%02d:%02d) Consumer %d added %s to queue %d\n",
                            locTime->tm_hour, locTime->tm_min, locTime->tm_sec, i % 3, msg, nr);
                        fprintf(results, "(%02d:%02d:%02d) Consumer %d added %s to queue %d\n",
                            locTime->tm_hour, locTime->tm_min, locTime->tm_sec, i % 3, msg, nr);
                        sem_post(s);
                    }
                    sleep(3);
                }
                exit(0);
                break;
            case 6:
                srand(time(NULL) ^ (getpid() << 16));
                printf("Super Producer\n");
                for(int j = 0; j < 20; j++){
                    int nr = rand() % QUEUE_NUM;

                    int id = shmget(3500 + j, 4 * sizeof(char), IPC_CREAT | 0666);
                    char *msg = (char *)shmat(id, NULL, 0);
                    for(int k = 0; k < 3; k++){
                        *(msg + k) = letters[rand() % 3];
                    }
                    *(msg + 3) = '\0';
                    produce(q, nr, id, HIGH);
                    sem_wait(s);
                    time(&measure);
                    locTime = localtime(&measure);
                    printf("(%02d:%02d:%02d) Super producer added %s to queue %d\n",
                        locTime->tm_hour, locTime->tm_min, locTime->tm_sec, msg, nr);
                    fprintf(results, "(%02d:%02d:%02d) Super producer added %s to queue %d\n",
                        locTime->tm_hour, locTime->tm_min, locTime->tm_sec, msg, nr);
                    sem_post(s);
                    sleep(3);
                }
                exit(0);
                break;
        }
    }

    pid_t w_pid;
    int status;
    while((w_pid = wait(&status)) > 0);

    printf("STARTED CLEANUP\n");
    clearQueue(q[0]);
    clearQueue(q[1]);
    clearQueue(q[2]);
    for(int i = 0; i < QUEUE_NUM; i++){
        munmap(queueBuff[i], sizeof(int) * QUEUE_SIZE);
        munmap(q[i], sizeof(PriQueue));
        close(queueBuff_id[i]);
        close(queue_id[i]);
    }
    shm_unlink("/aqueuebuff");
    shm_unlink("/aqueue");
    shm_unlink("/bqueuebuff");
    shm_unlink("/bqueue");
    shm_unlink("/cqueuebuff");
    shm_unlink("/cqueue");
    printf("CLEANUP SUCCESSFULLY FINISHED\n");

    puts("Exiting programme...");
    fclose(results);

    closeSem();
    sem_unlink("pro");
    sem_close(s);
    exit(0);
}
