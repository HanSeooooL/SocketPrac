#include "Parking_client.h"

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

void error_handling(char *message)
{
   fputs(message, stderr);
   fputc('\n', stderr);
   exit(1);
}

//차량 조회에 대해 리스트 자료구조를 구성해서 페이지마다 10개씩 꽂아넣자