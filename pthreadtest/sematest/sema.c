#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>


void* readnum(void* arg);
void* accunum(void* arg);
static sem_t *sem_one;
static sem_t *sem_two;
int num = 2;


int main() {
    pthread_t id_t1, id_t2;
    sem_one = sem_open("sem_one", O_CREAT, 0644, 0);

    sem_two = sem_open("sem_two", O_CREAT, 0644, 1);

    pthread_create(&id_t1, NULL, readnum, NULL);
    pthread_create(&id_t2, NULL, accunum, NULL);

    pthread_join(id_t1, NULL);
    pthread_join(id_t2, NULL);


    if(!sem_close(sem_one)) {
        perror("semone init");
        return -1;
    }
    if(!sem_close(sem_two)) {
        perror("semtwo init");
        return -1;
    }
    sem_unlink("sem_one");
    sem_unlink("sem_two");
    return 0;
}

void* readnum(void* arg) {
    int i;
    for(i = 0; i < 5; i++) {
        fputs("Input num: ", stdout);
        sem_wait(sem_two);

        scanf("%d", &num);
        sem_post(sem_one);

    }
    return NULL;
}

void* accunum(void* arg) {
    int sum = 0, i;
    for(i = 0; i < 5; i++) {
        sem_wait(sem_one);

        sum += num;
        sem_post(sem_two);

    }
    printf("Result : %d \n", sum);
    return NULL;
}