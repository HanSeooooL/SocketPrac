#include "Parking_client.h"

Parkcar *nowparkcar;
int page = 1;

void error_handling(char *message)
{
   fputs(message, stderr);
   fputc('\n', stderr);
   exit(1);
}

int Stringtoint(char *str) {
    int res = 0, w = 1;
    unsigned long length;
    length = strlen(str);
    for (int i = (int)length - 1; i >= 0; i--) {
        res += (str[i] - 48) * w;
        w *= 10;
    }
    return res;
}

char* substring(char *str, int start, int length) {
    char *res;
    res = (char*)malloc(sizeof(char) * length + 1);
    strncpy(res, &str[start], length);
    res[length] = '\0';
    return res;
}

void next_page() {page += 1;}
void prev_page() {page -= 1;}
int get_page() {return page;}
void get_pagelist(char *data, int str_len) {
    int data_count;
    free(nowparkcar);
    data_count = str_len / (PARKCARRECORDSIZE + 2);
    nowparkcar = (Parkcar*)malloc(sizeof(Parkcar) * data_count);
    for(int i = 0; i < data_count; i++) {
        strcpy(nowparkcar[i].carnumber, substring(data, (i * PARKCARRECORDSIZE) + (2 * i), CARNUMBERSIZE - 1));
        strcpy(nowparkcar[i].phonenumber, substring(data, (i * PARKCARRECORDSIZE) + (2 * i) + (CARNUMBERSIZE - 1), PHONENUMBERSIZE - 1));
        strcpy(nowparkcar[i].intime, substring(data, (i * PARKCARRECORDSIZE) + (2 * i) + (CARNUMBERSIZE - 1) + (PHONENUMBERSIZE - 1), INTIMESIZE - 1));
    }
}

void print_pagelist() {
    printf("\n%d Page\n", page);
    for(int i = 0; i < sizeof(nowparkcar) / sizeof(Parkcar); i++) {
        printf("%s\n", nowparkcar[i].carnumber);
        printf("%s\n", nowparkcar[i].phonenumber);
        printf("%s\n", nowparkcar[i].intime);
    }
}

//차량 조회에 대해 리스트 자료구조를 구성해서 페이지마다 10개씩 꽂아넣자