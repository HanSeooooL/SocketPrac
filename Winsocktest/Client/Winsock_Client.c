#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <conio.h>
#define BUFSIZE 1024
#define Port 8000

#pragma comment(lib, "ws2_32.lib");

void ErrorHandling(char *message) {
    WSACleanup();
    fputs(message, stderr);
    exit(1);
}


int main(int argc, char **argv) {
    SetConsoleOutputCP(CP_UTF8);
    SOCKET Sock;
    WSADATA wsaData;
    SOCKADDR_IN servAddr;
    char sAddr[15];
    int sPort = 0, nRcv = 0;
    unsigned int Addr;
    char message[BUFSIZE];
    struct hostent *host;
    printf("서버 주소 : ");
    scanf("%s", sAddr);
    fflush(stdin);

    //WSA(Window Socket Addr) 초기화
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) printf("초기화 에러");
    if((Sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        WSACleanup();
        ErrorHandling("socketinit error!");
    }

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(sAddr);
    servAddr.sin_port = htons(Port);

    if (connect(Sock, (struct sockaddr*)&servAddr, sizeof(servAddr)) == -1)
        ErrorHandling("connect() error!");

    while(1) {
        printf("\n보낼 메세지: ");
        gets(message);
        fflush(stdin);

        send(Sock, message, (int)strlen(message), 0);
        if(strcmp(message, "exit") == 0) break;

        printf("메세지 기다리는 중...\n");
        nRcv = recv(Sock, message, sizeof(message) - 1, 0);
        if(nRcv == SOCKET_ERROR) {
            printf("수신 에러..\n");
            break;
        }
        message[nRcv] = '\0';

        if(strcmp(message, "exit") == 0) {
            printf("서버가 연결을 종료하였습니다.\n");
            break;
        }

        printf("받은 메세지 : %s", message);
        fflush(stdin);
    }

    


    return 0;
}