//TCP방식 소켓통신
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024   //버퍼 사이즈
void error_handling(char *message); //오류 출력

int main(int argc, char *argv[])
{
   int serv_sock, clnt_sock;
   char message[BUF_SIZE];
   int str_len, i;

   struct sockaddr_in serv_adr, clnt_adr;
   socklen_t clnt_adr_sz;

    //입력받은 포트번호가 잘못되었을 경우 오류를 출력하고 종료한다.
   if(argc != 2)
   {
      printf("Usage : %s <port>\n", argv[0]);
      exit(1);
   }

    //소켓 생성
   serv_sock = socket(PF_INET, SOCK_STREAM, 0);
   if(serv_sock == -1)
      error_handling("socket() error");


    //소켓 주소 할당

    //메모리 초기화
   memset(&serv_adr, 0, sizeof(serv_adr));

   //인터넷 프로토콜 지정
   serv_adr.sin_family = AF_INET;

   //htonl == host to network long
   //Host-Byte-Order(Little Endian)로부터 NetworkByteorder 반환
   //해외여행할 때 파파고로 한국어를 외국어로 변환시키는 것과 비슷한 원리
   //INADDR_ANY == 컴퓨터에 지정된 어느 아이피에서 오는 데이터든 다 받는다.
   //INADDR_ANY자리에 내 컴퓨터에 할당된 IP를 입력?
   serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);

   //포트 지정
   serv_adr.sin_port = htons(atoi(argv[1]));
   
   //bind함수를 통해 serv_adr에 입력된 소켓 주소를 serv_sock에 할당
   if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
      error_handling("bind() error");
   if(listen(serv_sock, 5) == -1)
      error_handling("listen() error");

   clnt_adr_sz = sizeof(clnt_adr);

   for(i = 0; i < 5; i++)
   {
      clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
      if(clnt_sock == -1)
         error_handling("accept() error");
      else
         printf("Connected client %d \n", i + 1);
      while((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)
         write(clnt_sock, message, str_len);

      close(clnt_sock);
   }
   close(serv_sock);
   return 0;
}

void error_handling(char *message)
{
   fputs(message, stderr);
   fputc('\n', stderr);
   exit(1);
}