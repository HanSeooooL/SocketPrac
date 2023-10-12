#include "Parking_server.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

#define Port 1208

int serv_sock, clnt_sock;
int str_len, i, fd_max, fd_num;
fd_set reads, cpy_reads;
struct sockaddr_in serv_adr, clnt_adr;
struct timeval timeout;
socklen_t clnt_adr_sz;

void init_socket() {
    //소켓(디스크립터) 생성
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
      error_handling("socket() error");

    //메모리 초기화
    memset(&serv_adr, 0, sizeof(serv_adr));

    //인터넷 프로토콜 지정
    //sin_family: 주소체계 구분변수(sockaddr_in에서는 항상 AF_INET설정)
    serv_adr.sin_family = AF_INET;

    //s_addr: 호스트 IP주소
    //htonl == host to network long
    //Host-Byte-Order(Little Endian)로부터 NetworkByteorder 반환
    //해외여행할 때 한국어를 외국어로 번역시키려고 파파고를 키는 것 비슷한 원리
    //INADDR_ANY == 컴퓨터에 지정된 어느 아이피에서 오는 데이터든 다 받는다.
    //INADDR_ANY자리에 내 컴퓨터에 할당된 IP를 입력해도 가능
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);

    //포트 지정
    //sin_port: port번호
    //argv[2] 인자로 입력받은 포트.
    serv_adr.sin_port = htons(Port);

    //bind(소켓(디스크립터), 주소정보 할당, addr 구조체 크기)
    //bind함수를 통해 serv_adr에 입력된 소켓 주소를 serv_sock에 할당
    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");

    //통신 대기상태로 설정
    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    clnt_adr_sz = sizeof(clnt_adr);

    FD_ZERO(&reads);
    //데이터의 수신여부를 관찰하는 관찰대상에 서버 소켓 포함
    FD_SET(serv_sock, &reads);

    fd_max = serv_sock;
}

void ListenFromtheClient() {
    char message[BUF_SIZE];
    while(1) {
        cpy_reads = reads;
      timeout.tv_sec = 5;
      timeout.tv_usec = 5000;

      if((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1)
         break;

      if(fd_num == 0) continue;

      for(i = 0; i < fd_max + 1; i++) {
         if(FD_ISSET(i, &cpy_reads)) {
            if(i == serv_sock) {
               clnt_adr_sz = sizeof(clnt_adr);
               clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
               FD_SET(clnt_sock, &reads);
               if(fd_max < clnt_sock) fd_max = clnt_sock;
               printf("connected client: %d \n", clnt_sock);
            }
            else {
               str_len = read(i, message, BUF_SIZE);
               if(str_len == 0) {
                  FD_CLR(i, &reads);
                  close(i);
                  printf("closed client: %d \n", i);
               }
               else {
                  if(message[0] == '0') {
                     requestsavetheCardata(message);
                     write(clnt_sock, "Save done!\n", 11);
                  }
                  
                  else if(message[0] == '1') {
                     requestchangethedata(&clnt_sock, message);
                     write(clnt_sock, "Change done!\n", 13);
                  }

                  else if(message[0] == '2') {
                     requestdeletethedata(message);
                     write(clnt_sock, "Delete done!\n", 13);
                  }

                  else if(message[0] == '3') {
                     requestgivethedata(message, &clnt_sock);
                  }

                  else write(clnt_sock, "Sign Error!\n", 12);
               }
            }
         }
      }
    }
    close(serv_sock);
}

int sendthedata(char* data, int datasize) {
    int chk_write;
    data[datasize] = '\0';
    chk_write = write(clnt_sock, data, datasize);
    return chk_write;
    
}