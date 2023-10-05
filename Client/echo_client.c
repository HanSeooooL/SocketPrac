// echo_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024   //버퍼사이즈
void error_handling(char *message); //오류 출력

int main(int argc, char *argv[])
{
   int sock;
   char message[BUF_SIZE];
   int str_len;
   struct sockaddr_in serv_adr;

   if(argc != 3)
   {
      printf("Usage : %s <IP> <port> \n", argv[0]);
      exit(1);
   }

   //소켓(디스크립터) 생성
   sock = socket(PF_INET, SOCK_STREAM, 0);
   if(sock == -1)
      error_handling("socket() error");
   //소켓 주소 할당
   //메모리 초기화
   memset(&serv_adr, 0, sizeof(serv_adr));
   //인터넷 프로토콜 지정
   //sin_family: 주소체계 구분변수(sockaddr_in에서는 항상 AF_INET설정)
   serv_adr.sin_family = AF_INET;
   //s_addr: 호스트 IP주소
   //htonl == host to network long
   //Host-Byte-Order(Little Endian)로부터 NetworkByteorder 반환
   //해외여행할 때 한국어를 외국어로 번역시키려고 파파고를 키는 것 비슷한 원리
   //argv[1] 인자로 입력받은 주소.
   serv_adr.sin_addr.s_addr = inet_addr(argv[1]);

   //포트 지정
   //sin_port: port번호
   //argv[2] 인자로 입력받은 포트.
   serv_adr.sin_port = htons(atoi(argv[2]));

   //connect(소켓 디스크립터, 주소정보 할당, addr구조체 크기)
   //지정된 주소로 접속 요청
   if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
      error_handling("connect() error!");
  else
      puts("Connect........");

   while(1)
   {
      fputs("Input message(Q to quit) : ", stdout);
      fgets(message, BUF_SIZE, stdin);

      if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
         break;

      //wite(전송위치(디스크립터), 전송내용, 전송내용의 길이);
      write(sock, message, strlen(message));
      //read(읽어들이는위치, 저장위치, 길이);
      //전송된 내용의 크기 길이 반환
      str_len = read(sock, message, BUF_SIZE - 1);
      
      //문자열의 끝 문자 지정
      message[str_len] = 0;
      printf("Message form server : %s", message);
   }

   close(sock);
   return 0;
}

void error_handling(char *message)
{
   fputs(message, stderr);
   fputc('\n', stderr);
   exit(1);
}