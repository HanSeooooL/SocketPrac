#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


void *p_function(void *data) {
    pid_t pid;
    pthread_t tid;

    pid = getpid();
    tid = pthread_self();
    
    char *thread_name = (char*)data;
    int i = 0;

    while(i < 3) {
        printf("thread name : %s, tid : %x, pid: %u\n", thread_name, (unsigned int)tid, (unsigned int)pid);
        i++;
        sleep(1);
    }
}

int function1() {
    int res;
    res = 1 + 2;
    return res;
}

int function2() {
    int res;
    res = 3 + 4 + 5;
    return res;
}

int main() {
    /*
    pthread_t pthread[2];
    int thr_id;
    int status;
    char p1[] = "thread_1";
    char p2[] = "thread_2";
    char p3[] = "thread_3";

    sleep(1);

    thr_id = pthread_create(&pthread[0], NULL, p_function, (void*)p1);
    if(thr_id < 0) {
        perror("pthread0 create error");
        exit(EXIT_FAILURE);
    } 
    thr_id = pthread_create(&pthread[1], NULL, p_function, (void*)p2);
    if(thr_id < 0) {
        perror("pthread0 create error");
        exit(EXIT_FAILURE);
    } 
    thr_id = pthread_create(&pthread[2], NULL, p_function, (void*)p3);
    if(thr_id < 0) {
        perror("pthread0 create error");
        exit(EXIT_FAILURE);
    } 

    p_function((void *)p3);

    pthread_join(pthread[0], (void **)&status);
    pthread_join(pthread[1], (void **)&status);

    printf("end??\n"); */

    pthread_t pthread[2];
    int result[2];

    if(pthread_create(&pthread[0], NULL, function1, 0) < 0) {
        printf("Error!!");
        exit(1);
    }
    if(pthread_create(&pthread[1], NULL, function2, 0) < 0) {
        printf("Error!!");
        exit(1);
    }

    pthread_join(pthread[0], (void **)&result[0]);
    pthread_join(pthread[1], (void **)&result[1]);
    printf("result1 : %d\n", result[0]);
    printf("result2 : %d\n", result[1]);

    return 0;


}