#include "Parking_client.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <fcntl.h>

#define Port 1208
#define MYIP "127.0.0.1"

int sock, choice;
int fd_max, fd_num;
struct timeval timeout;
struct sockaddr_in serv_adr;
fd_set reads, cpy_reads;

void init_socket() {
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
   serv_adr.sin_addr.s_addr = inet_addr(MYIP);

   //포트 지정
   //sin_port: port번호
   //argv[2] 인자로 입력받은 포트.
   serv_adr.sin_port = htons(Port);

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
}

void requestsavethedata() {
    int str_len;
    char data[BUF_SIZE], keyboardinput[BUF_SIZE - 1];
    memset(data, 0x00, BUF_SIZE);
    fputs("Input message(Q to quit) : ", stdout);
    rewind(stdin);
    fgets(keyboardinput, BUF_SIZE - 1, stdin);
    data[0] = '0';
    data[1] = '\0';
    //wite(전송위치(디스크립터), 전송내용, 전송내용의 길이);
    strcat(data, keyboardinput);
    write(sock, data, strlen(data));
    //read(읽어들이는위치, 저장위치, 길이);
    //전송된 내용의 크기 길이 반환
    str_len = read(sock, data, BUF_SIZE - 1);
    //문자열의 끝 문자 지정
    data[str_len] = 0;
    printf("Message form server : %s", data);
}

Parkcar* requestgivemethecarList(int page) {
    int str_len, data_count;
    char data[BUF_SIZE];
    Parkcar* res;

    data[0] = '3';
    data[1] = (page / 10) + 48;
    data[2] = (page % 10) + 48;
    write(sock, data, strlen(data));
    memset(data, 0x00, BUF_SIZE);
    str_len = read(sock, data, BUF_SIZE);
    data_count = str_len / 32;
    res = (Parkcar*)malloc(sizeof(Parkcar) * data_count);
    for(int i = 0; i < data_count; i++) {
        *(res + i) -> carnumber = substring(data, ((i - 1) * 32) + i, CARNUMBERSIZE - 1);
        *(res + i) -> phonenumber = substring(data, ((i - 1) * 32) + i + (CARNUMBERSIZE - 1), PHONENUMBERSIZE);
        *(res + i) -> intime = substring(data, ((i - 1) * 32) + i + (CARNUMBERSIZE - 1) + (PHONENUMBERSIZE - 1), PHONENUMBERSIZE);
    }

    return res;
}