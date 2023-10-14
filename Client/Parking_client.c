// echo_client.c
#include "Parking_client.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <fcntl.h>

#define BUF_SIZE 1024   //버퍼사이즈

int main(int argc, char *argv[])
{
   int sock, choice;
   char message[BUF_SIZE], keyboardinput[BUF_SIZE - 1];
   int str_len, fd_max, fd_num;
   struct timeval timeout;
   struct sockaddr_in serv_adr;
   fd_set reads, cpy_reads;

   //입력받은 IP와 port가 잘못되었을때, 메뉴얼 출력 후 종료.
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

   FD_ZERO(&reads);
   FD_SET(0, &reads);
   FD_SET(sock, &reads);

   fd_max = sock;

   while(1)
   {
      cpy_reads = reads;
      timeout.tv_sec = 3;
      timeout.tv_usec = 0;
      printf("주차차량 저장 1 데이터 변경 2 데이터 삭제 3 데이터 가져오기 4 나가기 5>>");
      scanf("%d", &choice);
      memset(message, 0x00, BUF_SIZE);

      if(choice == 2) {
         memset(message, 0x00, BUF_SIZE);
         int count;
         fputs("몇번째 데이터를 지우시겠습니까? : ", stdout);
         rewind(stdin);
         scanf("%d", &count);
         rewind(stdin);
         
      }

      else if(choice == 3) {
         
      }

      else if(choice == 5) break;
      
      else printf("다시 입력해주세요.\n");
   }

   close(sock);
   return 0;
}