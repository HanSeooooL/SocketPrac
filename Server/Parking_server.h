#ifndef Parking_server_h
#define Parking_server_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FAIL -1
#define BUF_SIZE 1024   //버퍼 사이즈
#define PAGEDATACOUNT 10 //페이지당 차량 개수

//데이터 크기
#define PARKCARRECORDSIZE 32    //주차차량 레코드 크기

#define CARNUMBERSIZE 11        //차량번호크기
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
#define CALCULATETHECARDATA '2'     //1개의 차량 데이터 요금 확인
#define GETPARKCARLISTOFPAGE '3'        //선택 페이지 차량 데이터 가져오기
#define GETNOWPARKINGCARCOUNT '4'       //현재 주차중인 차량 대수 가져오기
#define SAVEONECOMMUTERCAR '5'          //정기권 차량 등록
#define ADDONECOMMUTERCARTIME '6'       //정기권 차량 기간 연장
#define UPDATECOMMUTERCARNUMBER '7'     //정기권 차량 차량번호 수정
#define DELETEONECOMMUTERCAR '8'        //정기권 차량 삭제

#define EXCHANGETHECARDATA 'a'      //차량 Carinfo에서 삭제하고 Sale데이터로 넘기기

//데이터 타입 코드
#define DT_PARKCAR 3
#define DT_COMCAR 4
#define DT_COMINFO 5
#define DT_SALEDATA 6

//파일 경로
#define PARKINGCARINFOROUTE "Datas/Parkinginfo.bin"
#define COMMUTERCARROUTE "Datas/CommuterCar.bin"
#define COMMUTERINFOROUTE "Datas/Commuterinfo.txt"
#define SALESROUTE "Datas/Sales.txt"
#define FEEOPTIONROUTE "Datas/Feeoptioninfo.txt"
#define PLACEINFOROUTE "Datas/Placeinfo.txt"

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
    int MaxCarSize; //최대 주차가능 차량
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

typedef struct _time {
    int year;
    int month;
    int mon[12];
    int day;
    int hour;
    int min;
} Time;

//Programinside
int Stringtoint(char *str);
char* substring(char *str, int start, int length);
void getSetting(void);  //주차장 설정 불러오기
void saveSetting(void); //주차장 설정 저장하기
void requestsavetheCardata(char* message);
void requestchangethedata(int *clnt_sock, char* message);
void requestExchangetheCar(char *message);
void requestgivethedata(char* message, int page);

//Network
void init_socket(void); //소켓 초기화
void ListenFromtheClient(void); //소켓 듣기 상태
int sendthedata(char* data, int datasize); //클라이언트에 데이터 보내기


//FileInOut
void* readFile(char* filename, int startline, int count, int *n); //파일 읽기
void getPlaceinfo(ParkingplaceSetting *Setting); //세팅 읽기
void getFeeoptioninfo(ParkingplaceSetting *Setting); //세팅 읽기
void savePlaceinfo(ParkingplaceSetting Setting); //세팅 저장
void seveFeeoptioninfo(ParkingplaceSetting Setting); //세팅 저장
Parkcar SearchtheCar(char* number); //차량 검색

#endif