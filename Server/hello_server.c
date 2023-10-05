// hello_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
void error_handling(char *message);

int main(int argc, char *argv[])
{
   int serv_sock;
   int clnt_sock;

   struct sockaddr_in serv_addr;
   struct sockaddr_in clnt_addr;
   socklen_t clnt_addr_size;

   char message[] = "Hello World!";

   if(argc != 2)
   {
      printf("Usage : %s <port>\n", argv[0]);
      exit(1);
   }

   serv_sock=socket(PF_INET, SOCK_STREAM, 0);   //소켓 생성
   if(serv_sock == -1)
      error_handling("Socket() error");

   //소켓 주소 할당(구조체 변수 초기화, bind함수 호출)
   memset(&serv_addr, 0, sizeof(serv_addr));
   serv_addr.sin_family=AF_INET;
   serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   serv_addr.sin_port = htons(atoi(argv[1]));

   if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
      error_handling("bind() error");

   //연결요청 대기상태
   if(listen(serv_sock, 5) == -1)
      error_handling("listen() error");
      
   clnt_addr_size = sizeof(clnt_addr);
   //accpet함수 호출시 대기큐에서 대기중에있는 연결요청 참조, 클라이언트와의 연결 구성, 파일 디스크립터 반환
   //만약 대기 큐가 비어있을 경우 대기큐가 연결될 때까지 accept함수는 반환하지 않는다.
   clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
   if(clnt_sock == -1)
      error_handling("accept() error");
      
   //write함수 호출을 통해 클라이언트에게 데이터를 전송.
   write(clnt_sock, message, sizeof(message));

   //close함수를 통해 연결 끊기
   close(clnt_sock);
   close(serv_sock);
   return 0;
}

void error_handling(char *message)
{
   fputs(message, stderr);
   fputc('\n', stderr);
   exit(1);
}