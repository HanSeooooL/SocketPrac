#ifndef Parking_server_h
#define Parking_server_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FAIL -1
#define BUF_SIZE 1024   //버퍼 사이즈
#define PAGEDATACOUNT 10 //페이지당 차량 개수

#define PARKCARRECORDSIZE 32    //주차차량 레코드 크기

#define CARNUMBERSIZE 11        //차량번호크기
#define PHONENUMBERSIZE 14      //전화번호크기
#define INTIMESIZE 11           //입차시간크기
#define COMMUTERNAMESIZE 21     //정기권명 크기
#define SIXDAYSIZE 7       //연월일 데이터 크기

#define YEARINTIME 0            //시간 데이터에서의 연도 위치
#define MONINTIME 2             //시간 데이터에서의 월 위치
#define DAYINTIME 4             //시간 데이터에서의 일 위치
#define HOURINTIME 6            //시간 데이터에서의 시간 위치
#define MININTIME 8             //시간 데이터에서의 분 위치
#define TIMESECTORLEN 2         //시간 데이터에서의 각 구간 길이

//데이터 타입 코드
#define DT_PARKCAR 3
#define DT_COMCAR 4
#define DT_COMINFO 5
#define DT_SALEDATA 6

void error_handling(char *message);

typedef struct _parkcar {
    char carnumber[CARNUMBERSIZE];  //차량번호
    char phonenumber[PHONENUMBERSIZE];  //전화번호
    char intime[INTIMESIZE];        //입차시간
} Parkcar;

typedef struct _commuterinfo {
    char commutername[COMMUTERNAMESIZE];    //정기권명
    char lastestsaleday[SIXDAYSIZE];    //최근 구매일
    int bill;   //가격
    int month;  //적용기간
} Commuterinfo;

typedef struct _commutercar {
    char carnumber[CARNUMBERSIZE]; //차량번호
    char limittime[SIXDAYSIZE]; //만료일
} Commutercar;

typedef struct _salesdata {
   char day[SIXDAYSIZE];
   int bill;
   char *type;
   char *name;
} Salesdata;

typedef struct _parkingplaceSetting {
    int MaxCarSize;
    int FirstPee;
    int FirstPeeTime;
    int NextPee;
    int NextPeeTime;
    short GetWeekendPee;
    int GetBigCarPeePercent;
    int ChangePlusMoney;
    int PlusMoneyTime;
    int PlusMoneyPlus;
    int PeeOfDay;
} ParkingplaceSetting;

//Programinside
int Stringtoint(char *str);
char* substring(char *str, int start, int length);
void requestsavetheCardata(char* message);
void requestchangethedata(int *clnt_sock, char* message);
void requestExchangetheCar(char* message);
void requestgivethedata(char* message, int page);

//Network
void init_socket(); //소켓 초기화
void ListenFromtheClient(); //소켓 듣기 상태
int sendthedata(char* data, int datasize); //클라이언트에 데이터 보내기


//FileInOut
void getSetting();  //주차장 설정 불러오기

#endif