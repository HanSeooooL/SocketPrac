#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdarg.h>
#include <unistd.h>
#define MAXTHREADLIMIT 
#define testroute "testroute.txt"

pthread_t pthread[MAXTHREADLIMIT];
pthread_mutex_t mutexo;
pthread_mutex_t mutext;
pthread_mutex_t mutexthree;

void init_mutex();
void* saverthedata(void* data);


int main(void) {

    init_mutex();
    
    return 0;
}

void init_mutex() {
    pthread_mutex_init(&mutexo, NULL);
    pthread_mutex_init(&mutext, NULL);
    pthread_mutex_init(&mutexthree, NULL);
}

void* threadsavethedata(char* data) {
}


void* savethedata(void* data) {
    FILE *fp;

    if(access(testroute, F_OK) != -1) {
        fp = fopen(testroute, "a");
    }
    else fp = fopen(testroute, "w");

    fprintf(fp, "%s\n", data);

    fclose(fp);
}