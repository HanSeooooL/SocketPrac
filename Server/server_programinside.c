#include "Parking_server.h"
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>


typedef struct _charnode {
    char* data;
    struct _charnode* next;
} Charnode;


extern void savethecar(Parkcar newone);
extern void readParkinginfo(void);
extern Parkcar searchtheCar(char *targetcarnumber, int *count);
extern void deletetheCar(int count);

extern void print_queue(char *msg);

//Charnode queue
int isEmpty_Charnode(Charnode *head, Charnode *tail);
void enqueue_Charnode(char *newone, Charnode *head, Charnode *tail);
char* dequeue_Charnode(Charnode *head, Charnode *tail);


int intlen(int x);
char* returntimetoday(void);
char* returntimetomin(void);
int Stringtoint(char *str); //문자열 -> 정수변환
char* makeIntotoString(int num);
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
    res = (char*)malloc(sizeof(char) * SIXDAYSIZE);
    
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

int intlen(int x) {
    int res;

    for(res = 0; x > 0; res++) {
        x = x / 10;
    }

    return res;
}

char* makeIntotoString(int num) {
    char* res;
    res = (char*)malloc(sizeof(char) * (intlen(num) + 1));
    
    while(num > 0) {
        for(int i = 1; i < intlen(num) + 1; i++) {
            res[i - 1] = num / (int)pow(10, (intlen(num) - i)) % (int)pow(10, (intlen(num) - i + 1)) + '0';
        }
    }
    res[intlen(num)] = '\0';
    return res;
}

char* makeMessagefromData(void* data, int datatype, int count) {
    char* res;

    if(datatype == DT_PARKCAR) {
        res = (char*)malloc(sizeof(char) * (count * PARKCARRECORDSIZE));
        res[0] = '\0';
        for(int i = 0; i < count; i++) {
            strcat(res, ((Parkcar*)data)[i].carnumber);
            strcat(res, ((Parkcar*)data)[i].phonenumber);
            strcat(res, ((Parkcar*)data)[i].intime);
        }
    }

    if(datatype == DT_COMCAR) {

    }

    if(datatype == DT_COMINFO) {
        char *tmp = NULL, *tmp1 = NULL, *tmp2 = NULL;
        int *size, fullsize = 0;
        Charnode *head = NULL, *tail = NULL;
        size = (int*)malloc(sizeof(int) * count);
        for(int i = 0; i < count; i++) {
            tmp1 = makeIntotoString(((Commuterinfo*)data)[i].bill);
            tmp2 = makeIntotoString(((Commuterinfo*)data)[i].month);

            size[i] = sizeof(char) * (COMMUTERNAMESIZE + SIXDAYSIZE + 
            intlen(((Commuterinfo*)data)[i].bill) + 1 + intlen(((Commuterinfo*)data)[i].month) + 1);
            
            tmp = (char*)malloc(size[i]);
            

            tmp[0] = '\0';
            strcat(tmp, ((Commuterinfo*)data)[i].commutername);
            strcat(tmp, ((Commuterinfo*)data)[i].lastestsaleday);
            strcat(tmp, tmp1);
            strcat(tmp, "&\0");
            strcat(tmp, tmp2);
            enqueue_Charnode(tmp, head, tail);

            free(tmp1);
            free(tmp2);
            tmp = NULL;
        }

        for(int i = 0; i < count; i++) {
            fullsize += size[i];
        }
        free(size);

        res = (char*)malloc(fullsize);
        res[0] = '\0';

        for(int i = 0; i < count; i++) {
            strcat(res, dequeue_Charnode(head, tail));
        }
    }

    if(datatype == DT_SALEDATA) {

    }
    

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
   str = substring(message, 1, strlen(message) - 2);
   fprintf(file, "%s\n", str);
   

   fclose(file);
}

void requestgivethedata(char* message, int page) {
   int fd;
   char buf[BUF_SIZE];
   int file_read_len, chk_write;
   
   fd = open("Cars.txt", O_RDONLY);
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

void server_console() {
    int choice;
    while(1) {
        printf("서버를 종료하려면 0번을 입력 >> ");
        scanf("%d", &choice);

        if(choice == 0) {
            break;
        }

        else printf("다시 입력해주세요.\n");
    }
}

int isEmpty_Charnode(Charnode *head, Charnode *tail) {
    return (head == NULL);
}

void enqueue_Charnode(char *newone, Charnode *head, Charnode *tail) {
    Charnode *tmp = (Charnode*)malloc(sizeof(Charnode));
    tmp -> data = newone;
    tmp -> next = NULL;
    if (isEmpty_Charnode(head, tail)) head = tail = tmp;
    
    else {
        tail -> next = tmp;
        tail = tmp;
    }
}

char* dequeue_Charnode(Charnode *head, Charnode *tail) {
    char* res;
    Charnode *tmp;
    if(isEmpty_Charnode(head, tail)) perror("큐 공백 에러");

    else {
        tmp = head;
        head = head -> next;
        if(head == NULL) tail = NULL;
        res = tmp -> data;
        free(tmp);
    }
    return res;
}