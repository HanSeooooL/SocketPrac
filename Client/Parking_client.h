
#ifndef Parking_client_h
#define Parking_client_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FAIL -1
#define BUF_SIZE 1024   //버퍼 사이즈
#define PAGEDATACOUNT 10 //페이지당 차량 개수

//데이터 크기
#define PARKCARRECORDSIZE 32    //주차차량 레코드 크기

#define CARNUMBERSIZE 10        //차량번호크기
#define PHONENUMBERSIZE 14      //전화번호크기
#define INTIMESIZE 11           //입차시간크기
#define SIXDAYSIZE 7            //만료날짜크기

#define COMMUTERNAMESIZE 21         //정기권 이름 크기

#define YEARINTIME 0            //시간 데이터에서의 연도 위치
#define MONINTIME 2             //시간 데이터에서의 월 위치
#define DAYINTIME 4             //시간 데이터에서의 일 위치
#define HOURINTIME 6            //시간 데이터에서의 시간 위치
#define MININTIME 8             //시간 데이터에서의 분 위치
#define TIMESECTORLEN 2         //시간 데이터에서의 각 구간 길이

//기능 번호
#define GETALLPARKINGPLACECOUNT '0'     //주차장 총 주차대수 가져오기
#define SAVEONEPARKCARDATA '1'          //1개의 차량 데이터 저장(입차)
#define CALCULATEONEPARKCARDATA '2'     //1개의 차량 데이터 정산(출차)
#define GETPARKCARLISTOFPAGE '3'        //선택 페이지 차량 데이터 가져오기
#define GETNOWPARKINGCARCOUNT '4'       //현재 주차중인 차량 대수 가져오기
#define SAVEONECOMMUTERCAR '5'          //정기권 차량 등록
#define ADDONECOMMUTERCARTIME '6'       //정기권 차량 기간 연장
#define UPDATECOMMUTERCARNUMBER '7'     //정기권 차량 차량번호 수정
#define DELETEONECOMMUTERCAR '8'        //정기권 차량 삭제

//데이터 타입 코드
#define DT_PARKCAR 3
#define DT_COMCAR 4
#define DT_COMINFO 5
#define DT_SALEDATA 6

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

void error_handling(char *message); //오류 출력

//programinside
int Stringtoint(char *str);
char* Inttostring(int num);
char* substring(char *str, int start, int length);
int get_page();     //페이지 번호 가져오기
void next_page();   //페이지 다음 번호로 이동
void prev_page();   //페이지 이전 번호로 이동
void get_pagelist(char *data, int str_len); //구조체 배열에 선택 페이지의 데이터 저장
void print_pagelist();  //저장된 차량 데이터 출력

//Network
void ConnectNetwork();  //서버 연결
char* requesttheData(char funccharacter, ...); //데이터 요청 후 정제되지 않은 데이터 문자열 반환
Parkcar* requestParkcarlist(int page); //주차차량 목록 요청

//UI
void ProgramStart();    //UI실행


//FileInOut
void readParkinginfo(void);
Parkcar dequeue(void);

#endif