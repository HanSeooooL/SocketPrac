#include "Parking_server.h"
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

extern void savethecar(Parkcar newone);
extern void readParkinginfo(void);
extern Parkcar searchtheCar(char *targetcarnumber, int *count);
extern void deletetheCar(int count);

extern void print_queue(char *msg);

char* returntimetoday(void);
char* returntimetomin(void);
int Stringtoint(char *str); //문자열 -> 정수변환
char* substring(char *str, int start, int length); //문자열 부분 자르기(자를 문자열, 시작지점, 길이)
int checkMaxDayofMonth(int year, int month); //특정 년월의 일수 체크
int checkParkingdays(char *starttime);    //주차기간 체크
int checkBill(Parkcar selected); //요금 체크

void error_handling(char *message)
{
   fputs(message, stderr);
   fputc('\n', stderr);
   exit(1);
}


void Carin(Parkcar newone) {    //작동
    strcpy(newone.intime, returntimetomin());
    savethecar(newone);
}

void Carread(void) {    //작동
    readParkinginfo();
    print_queue("");
}

void Carout(char *carnumber) {  //작동
    int count = 0;
    searchtheCar(carnumber, &count);
    if(count == FAIL) {printf("찾지 못했습니다\n");}
    else deletetheCar(count);
}

void addCommuter(char *addcar) {
    
}

char* returntimetomin(void) {
    //분단위까지 문자열 형태로 반환
    time_t currtime;
    time(&currtime);
    char *res = NULL;
    res = (char*)malloc(sizeof(char) * INTIMESIZE);
    
    struct tm nowtime;
    nowtime = *localtime(&currtime);
    res[0] = ((nowtime.tm_year + 1900) % 100) / 10 + 48;
    res[1] = ((nowtime.tm_year + 1900) % 10) + 48;
    res[2] = ((nowtime.tm_mon + 1) / 10) + 48;
    res[3] = ((nowtime.tm_mon + 1) % 10) + 48;
    res[4] = ((nowtime.tm_mday) / 10) + 48;
    res[5] = ((nowtime.tm_mday) % 10) + 48;
    res[6] = ((nowtime.tm_hour) / 10) + 48;
    res[7] = ((nowtime.tm_hour) % 10) + 48;
    res[8] = ((nowtime.tm_min) / 10) + 48;
    res[9] = ((nowtime.tm_min) % 10) + 48;
    res[10] = '\0';
    
    return res;
}

char* returntimetoday(void) {
    //오늘 날짜까지 문자열 형태로 반환
    time_t currtime;
    time(&currtime);
    char *res = NULL;
    res = (char*)malloc(sizeof(char) * LIMITTIMESIZE);
    
    struct tm nowtime;
    nowtime = *localtime(&currtime);
    res[0] = ((nowtime.tm_year + 1900) % 100) / 10 + 48;
    res[1] = ((nowtime.tm_year + 1900) % 10) + 48;
    res[2] = ((nowtime.tm_mon + 1) / 10) + 48;
    res[3] = ((nowtime.tm_mon + 1) % 10) + 48;
    res[4] = ((nowtime.tm_mday) / 10) + 48;
    res[5] = ((nowtime.tm_mday) % 10) + 48;
    res[6] = '\0';
    
    return res;
}

int checkBill(Parkcar selected) {
    
    int res = 0, days, min;
    
    //주차시간 분으로 변환
    days = checkParkingdays(selected.intime);
    min = days * 24 * 60;
    
    
    return res;
}

int checkParkingdays(char *starttime) {
    int res = -1, year = 0, *mon, day = 0, hour = 0, min = 0, minw = 0, hourw = 0, dayw = 0, monw = 0, yearw = 0;
    char* now;
    now = returntimetomin();
    
    //연산
    //분
    if(Stringtoint(substring(starttime, MININTIME, TIMESECTORLEN)) > Stringtoint(substring(now, MININTIME,TIMESECTORLEN))) {
        hourw -= 1;
        minw += 60;
    }
    min = (Stringtoint(substring(now, MININTIME, TIMESECTORLEN)) + minw) - Stringtoint(substring(starttime, MININTIME, TIMESECTORLEN));
    
    //시
    if(Stringtoint(substring(starttime, HOURINTIME, TIMESECTORLEN)) > (Stringtoint(substring(now, HOURINTIME, TIMESECTORLEN)) + hourw)) {
        dayw -= 1;
        hourw += 24;
    }
    hour = (Stringtoint(substring(now, HOURINTIME, TIMESECTORLEN)) + hourw) - Stringtoint(substring(starttime, HOURINTIME, TIMESECTORLEN));
    
    //일
    if(Stringtoint(substring(starttime, DAYINTIME, TIMESECTORLEN)) > Stringtoint(substring(now, DAYINTIME, TIMESECTORLEN)) + dayw) {
        monw -= 1;
        dayw += checkMaxDayofMonth(Stringtoint(substring(now, YEARINTIME, TIMESECTORLEN)), Stringtoint(substring(now, MONINTIME, TIMESECTORLEN)) + monw);
    }
    day = (Stringtoint(substring(now, DAYINTIME, TIMESECTORLEN)) + dayw) - Stringtoint(substring(starttime, DAYINTIME, TIMESECTORLEN));
    
    //월
    if(Stringtoint(substring(starttime, MONINTIME, TIMESECTORLEN)) > Stringtoint(substring(now, MONINTIME, TIMESECTORLEN)) + monw) {
        int monthnum, l;
        yearw -= 1;
        monthnum = (Stringtoint(substring(now, MONINTIME, TIMESECTORLEN)) + monw + 12) - Stringtoint(substring(starttime, MONINTIME, TIMESECTORLEN));
        mon = (int*)malloc(sizeof(int) * monthnum);
        for(int i = 0; i < monthnum; i++) {
            l = (Stringtoint(substring(starttime, MONINTIME, TIMESECTORLEN)) + i) % 12;
            if(l == 0) l = 12;
            mon[i] = l;
        }
    }
    else {
        int monthnum, l;
        monthnum = (Stringtoint(substring(now, MONINTIME, TIMESECTORLEN)) + monw) - Stringtoint(substring(starttime, MONINTIME, TIMESECTORLEN));
        mon = (int*)malloc(sizeof(int) * monthnum);
        for(int i = 0; i < monthnum; i++) {
            l = (Stringtoint(substring(starttime, MONINTIME, TIMESECTORLEN)) + i) % 12;
            if(l == 0) l = 12;
            mon[i] = l;
        }
    }
    
    //년
    if(Stringtoint(substring(starttime, YEARINTIME, TIMESECTORLEN)) > Stringtoint(substring(now, YEARINTIME, TIMESECTORLEN)) + yearw) {
        printf("잘못된 입력입니다\n");
        exit(1);
    }
    else {
        year = (Stringtoint(substring(now, YEARINTIME, TIMESECTORLEN)) + yearw) - Stringtoint(substring(starttime, YEARINTIME, TIMESECTORLEN));
    }
    
    return res;
}


int checkMaxDayofMonth(int year, int month) {
    int res = 0;
    
    if (month < 8) {
        if(month % 2 == 1) {
            res = 31;
        }
        else {
            if(month == 2) {
                if(year % 4 == 0) res = 29;
                else res = 28;
            }
            else res = 30;
        }
    }
    else {
        if (month % 2 == 0) res = 30;
        else res = 31;
    }
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

char* substring(char *str, int start, int length) {
    char *res;
    res = (char*)malloc(sizeof(char) * length + 1);
    strncpy(res, &str[start], length);
    res[length] = '\0';
    return res;
}

void requestsavetheCardata(char* message) {
   FILE* file;
   char *str;
   if (access("Cars.txt", 0) != -1) {  //이미 존재하는 파일인 경우 내용 추가
        file = fopen("Cars.txt", "a");
    }
    else    //없는 경우 생성
        file = fopen("Cars.txt", "w");
   str = substring(message, 1, strlen(message));
   fprintf(file, "%s", str);
   

   fclose(file);
}

void requestchangethedata(int *clnt_sock, char* message) {
   FILE* fp;
   char str[BUF_SIZE];
   if (access("Cars.txt", 0) != -1) {  //이미 존재하는 파일인 경우 내용 추가
        fp = fopen("Cars.txt", "r+");
    }
    else {
      error_handling("File not exist!!");
    }
    while(fgets(str, BUF_SIZE, fp)) {
      
    }

}

void requestdeletethedata(char* message) {
   
}

void requestgivethedata(char* message, int page) {
   int fd;
   char buf[BUF_SIZE];
   int file_read_len, chk_write;
   
   fd = open("test.txt", O_RDONLY);
   while(1) {
      memset(buf, 0x00, BUF_SIZE);
      file_read_len = read(fd, buf, BUF_SIZE);
      printf("\nread: %s", buf);
      chk_write = sendthedata(buf, file_read_len);
      if(file_read_len == EOF | file_read_len == 0) {
         //chk_write = write(*clnt_sock, "+", BUF_SIZE);
         printf("finish file\n");
         break;
      }
   }
   close(fd);
}