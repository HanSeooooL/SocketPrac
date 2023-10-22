#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#define NUM_THREAD 100

void* thread_inc(void* arg);
void* thread_des(void* arg);

long long num = 0;
pthread_mutex_t mutex;

int main() {
    pthread_t thread_id[NUM_THREAD];
    int i;

    //mutex(권한키 생성)
    //pthread_mutex_init(mutex변수 주소, 대상 변수의 특성);
    pthread_mutex_init(&mutex, NULL);
    for(i = 0; i < NUM_THREAD; i++) {
        if(i % 2)
            //홀수에 쓰레드 생성
            pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
        else
            //짝수에 쓰레드 생성
            pthread_create(&(thread_id[i]), NULL, thread_des, NULL);
    }

    //쓰레드 종료될때까지 대기
    for(i = 0 ; i < NUM_THREAD; i++)
        pthread_join(thread_id[i], NULL);
    
    printf("result: %lld \n", num);
    pthread_mutex_destroy(&mutex);
    return 0;
}

void* thread_inc(void* arg) {
    int i ;
    //접근권한 락
    pthread_mutex_lock(&mutex);
    printf("doing inc\n");
    for(i = 0; i < 5000000; i++) num += 1;
    //접근권한 락 헤제
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* thread_des(void* arg) {
    int i;
    for(i = 0; i < 5000000; i++) {
        //접근권한 락
        pthread_mutex_lock(&mutex);
        if(i == 0) printf("doing des\n");
        num -= 1;
        //접근권한 락 헤제
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}