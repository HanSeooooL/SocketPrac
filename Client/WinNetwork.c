#include "Parking_client.h"
#include <winsock2.h>
#include <conio.h>
#include <stdarg.h>
#define BUFSIZE 1024
#define Port 1209
#define IP "127.0.0.1"

#pragma comment (lib, "ws2_32.lib")

SOCKET servSock;
WSADATA wsaData;

static void init_socket();
static void connecttoServer();
char* requesttheData(char funccharacter, ...);


Parkcar* requestParkcarlist(int page) {
    Parkcar* res;
    char *data, charpage[3];
    if(page / 10 == 0) {
        charpage[0] = '0';
        charpage[1] = page + '0';
        charpage[2] = '\0';
    }
    else {
        strcpy(charpage, Inttostring(page));
    }

    data = requesttheData(GETPARKCARLISTOFPAGE, charpage);

    res = (Parkcar*)malloc(sizeof(Parkcar) * (strlen(data) / PARKCARRECORDSIZE));
    for(int i = 0; i < (strlen(data) / PARKCARRECORDSIZE); i++) {
        strcpy(res[i].carnumber, substring(data, PARKCARRECORDSIZE * i, CARNUMBERSIZE - 1));
        strcpy(res[i].phonenumber, substring(data, PARKCARRECORDSIZE * i + CARNUMBERSIZE - 1, PHONENUMBERSIZE));
        strcpy(res[i].intime, substring(data, PARKCARRECORDSIZE * i + CARNUMBERSIZE - 1 + PHONENUMBERSIZE - 1, INTIMESIZE));
    }

    return res;
}

void ErrorHandling(char *message) {
    WSACleanup();
    fputs(message, stderr);
    exit(1);
}

void init_socket() {
    if((servSock = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        WSACleanup();
        ErrorHandling("socketinit error!");
    }
}

void connecttoServer() {
    SOCKADDR_IN servAddr;
    char MYIP[12];
    strcpy(MYIP, IP);

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(MYIP);
    servAddr.sin_port = htons(Port);
    if (connect(servSock, (struct sockaddr*)&servAddr, sizeof(servAddr)) == -1)
        ErrorHandling("connect() error!");

}

char* requesttheData(char funccharacter, ...) {
    int count = 0, nRcv = 0, check, i = 0;
    char message[BUFSIZE];
    char *data, *tmp[10];
    va_list argptr;

    message[0] = funccharacter;
    message[1] = '\0';

    va_start(argptr, count);
    for(int i = 0; i < count; i++) {
        strcat(message, va_arg(argptr, char*));
    }

    send(servSock, message, (int)strlen(message), 0);

    do {
        tmp[i] = (char*)malloc(sizeof(char) * BUF_SIZE);
        nRcv = recv(servSock, tmp[i], sizeof(tmp[i]) - 1, 0);
        if(nRcv == SOCKET_ERROR) perror("Receive Error!\n");
        tmp[i][nRcv] = '\0';
        ++i;
        //ioctl(*servSock, FIONREAD, &check);
        check = 1;
    } while(check > 0);

    data = (char*)malloc(sizeof(char) * (BUF_SIZE * i));
    strcpy(data, tmp[0]);
    for(int j = 1; j < i; j++) {
        strcat(data, tmp[i]);
    }

    return data;
}

void ConnectNetwork() {
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) printf("초기화 에러");

    init_socket();
    connecttoServer();

}