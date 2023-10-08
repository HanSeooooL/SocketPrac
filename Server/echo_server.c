//TCP방식 소켓통신
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUF_SIZE 1024   //버퍼 사이즈
#define MAX_CHAR_PER_LINE 1024
void error_handling(char *message); //오류 출력
void requestsavethedata(char* message); //데이터 저장 요청
void requestchangethedata(char* message); //데이터 수정 요청
void requestdeletethedata(char* message); //데이터 삭제 요청
void requestgivethedata(char* message, int* clnt_sock); //데이터 리스트 요청

char* substring(char *str, int start, int length); //문자열 부분 자르기(자를 문자열, 시작지점, 길이)
int Stringtoint(char *str);   //문자열 정수형 변환

int main(int argc, char *argv[])
{
   int serv_sock, clnt_sock;
   char message[BUF_SIZE];
   int str_len, i, fd_max, fd_num;
   fd_set reads, cpy_reads;

   //sockaddr_in : IPv4주소체계(AF_INET)에 사용하는 구조체
   //sockaddr_in6 : IPv6주소체계(AF_INET6)에 사용하는 구조체
   struct sockaddr_in serv_adr, clnt_adr;
   struct timeval timeout;
   socklen_t clnt_adr_sz;

    //입력받은 포트번호가 잘못되었을 경우 메뉴얼을 출력하고 종료.
   if(argc != 2)
   {
      printf("Usage : %s <port>\n", argv[0]);
      exit(1);
   }

    //소켓(디스크립터) 생성
   serv_sock = socket(PF_INET, SOCK_STREAM, 0);
   if(serv_sock == -1)
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
   //INADDR_ANY == 컴퓨터에 지정된 어느 아이피에서 오는 데이터든 다 받는다.
   //INADDR_ANY자리에 내 컴퓨터에 할당된 IP를 입력해도 가능
   serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);

   //포트 지정
   //sin_port: port번호
   //argv[2] 인자로 입력받은 포트.
   serv_adr.sin_port = htons(atoi(argv[1]));
   
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
                     requestsavethedata(message);
                     write(clnt_sock, "Save done!\n", 11);
                  }
                  
                  else if(message[0] == '1') {
                     requestchangethedata(message);
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

   /*
   for(i = 0; i < 5; i++)
   {
      //accept함수로 클라이언트의 접속 요청 허락, 커널이 소켓 생성(클라이언트 소켓 생성)
      clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
      if(clnt_sock == -1)
         error_handling("accept() error");
      else
         printf("Connected client %d \n", i + 1);
      //read(읽어들이는 위치(클라이언트 소켓)), 저장 위치, 버퍼 사이즈)
      //데이터의 길이를 반환
      while((str_len = read(clnt_sock, message, BUF_SIZE)) != 0) {
         //write(전송위치(디스크립터), 전송내용, 길이);
         write(clnt_sock, message, str_len);
         message[str_len] = '\0';
         savethedata(message);
      }

      close(clnt_sock);
   }
   */

   close(serv_sock);
   return 0;
}

void error_handling(char *message)
{
   fputs(message, stderr);
   fputc('\n', stderr);
   exit(1);
}

void requestsavethedata(char* message) {
   FILE* file;
   char *str;
   if (access("test.txt", 0) != -1) {  //이미 존재하는 파일인 경우 내용 추가
        file = fopen("test.txt", "a");
    }
    else    //없는 경우 생성
        file = fopen("test.txt", "w");
   str = substring(message, 1, strlen(message));
   fprintf(file, "%s", str);

   fclose(file);
}

void requestchangethedata(char* message) {

}

void requestdeletethedata(char* message) {
   
}

void requestgivethedata(char* message, int* clnt_sock) {
   int fd;
   char buf[BUF_SIZE];
   int file_read_len, chk_write;
   fd = open("test.txt", O_RDONLY);
   while(1) {
      memset(buf, 0x00, BUF_SIZE);
      file_read_len = read(fd, buf, BUF_SIZE);
      printf("\nread: %s", buf);
      chk_write = write(*clnt_sock, buf, BUF_SIZE);
      if(file_read_len == EOF | file_read_len == 0) {
         printf("finish file\n");
         break;
      }
   }
   close(fd);
}

char* substring(char *str, int start, int length) {
    char *res;
    res = (char*)malloc(sizeof(char) * length + 1);
    strncpy(res, &str[start], length);
    res[length] = '\0';
    return res;
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