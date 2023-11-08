
#define MAX_CHAR_PER_LINE 1024

#define PARKINGCARINFORECORDSIZE 32
#define COMMUTERCARRECORDSIZE 15

#include "Parking_server.h"
#include "parson.h"
#include <unistd.h>
#include <stdarg.h>

typedef Parkcar Element;
typedef struct LinkedNode {
    Element data;
    struct LinkedNode *link;
} Node;


void error(char *msg)
{
    printf("%s\n", msg);
    exit(1);
}

Node *front = NULL;
Node *rear = NULL;
int is_empty(void) { return front == NULL; }    //큐가 비었는지 확인
void init_queue(void) { front = rear = NULL; }  //큐 초기화
int size(void); //큐 사이즈 확인
void enqueue(Element e);    //데이터 삽입
Element dequeue(void);  //데이터 가져오기(큐에서 삭제)
Element peek(void);     //데이터 가져오기(큐에서 삭제X)
void destroy_queue(void);   //큐 초기화
void print_queue(char *msg);    

void* readFile(char* filename, int startline, int count, int *n);
void* readFiletail(char* filename, FILE *fp, int *count, void* res, int *n);
void savethecar(Parkcar newone);
Parkcar SearchtheCar(char* number);
void readParkinginfo(void);

void init_Parkingcarinfo(void) {
    FILE *fp;
    fp = fopen(PARKINGCARINFOROUTE, "w");
    fclose(fp);
}

void init_Commuterinfo(void) {
    FILE *fp;
    fp = fopen(COMMUTERINFOROUTE, "w");
    fclose(fp);
}

void init_Placeinfo(void) {
    FILE *fp;
    fp = fopen(PLACEINFOROUTE, "w");
    fclose(fp);
}

void savethecar(Parkcar newone) {
    FILE *fp;
    if(access(PARKINGCARINFOROUTE, F_OK) != -1) { //파일이 있는경우
        fp = fopen(PARKINGCARINFOROUTE, "a"); //파일에 데이터 추가
        fprintf(fp, "%s", newone.carnumber);
        fprintf(fp, "%s", newone.phonenumber);
        fprintf(fp, "%s", newone.intime);
        fprintf(fp, "\n");
        
        
    } else {    //파일이 없는 경우
        fp = fopen(PARKINGCARINFOROUTE, "w"); //파일 새로 생성
        fprintf(fp, "%s", newone.carnumber);
        fprintf(fp, "%s", newone.phonenumber);
        fprintf(fp, "%s", newone.intime);
        fprintf(fp, "\n");
    }
    fclose(fp);
}

Parkcar SearchtheCar(char* number) {
    FILE *fp;
    Parkcar* data;
    Parkcar res;
    int i, line;
    char Carnumber[CARNUMBERSIZE];
    if(access(PARKINGCARINFOROUTE, F_OK) != -1) {
        while(!feof(fp)) {
            fp = fopen(PARKINGCARINFOROUTE, "r");
            for(i = 0; i < CARNUMBERSIZE - 1; i++) {
                Carnumber[i] = getc(fp);
            }
            Carnumber[i] = '\0';
            if(strcmp(Carnumber, number) == 0) {
                int n = 0;
                fclose(fp);
                data = (Parkcar*)readFile(PARKINGCARINFOROUTE, line, 1, &n);
                res = *data;
                free(data);
                return res;
            }
        }

        fclose(fp);
    }
}

void* readFiletail(char* filename, FILE *fp, int *count, void* res, int *n) {
    char line[MAX_CHAR_PER_LINE];

    if(fgets(line, MAX_CHAR_PER_LINE, fp) == NULL) {
        if((*count) > 0) {

            if(strcmp(filename, PARKINGCARINFOROUTE) == 0) {
                res = malloc(sizeof(Parkcar) * (*count));
            }
            else if(strcmp(filename, COMMUTERCARROUTE) == 0) {
                res = malloc(sizeof(Commutercar) * (*count));
            }
        }
        return NULL;
    }
    else {
        (*count) += 1;
        readFiletail(filename, fp, count, res, n);
    }
    if(strcmp(filename, PARKINGCARINFOROUTE) == 0) {

        strcpy(((Parkcar*)res)[*count - 1].carnumber, substring(line, 0, CARNUMBERSIZE - 1));
        strcpy(((Parkcar*)res)[*count - 1].phonenumber, substring(line, CARNUMBERSIZE - 1, PHONENUMBERSIZE - 1));
        strcpy(((Parkcar*)res)[*count - 1].intime, substring(line, (CARNUMBERSIZE - 1) + (PHONENUMBERSIZE - 1), INTIMESIZE - 1));

    }
    else if(strcmp(filename, COMMUTERCARROUTE) == 0) {
        strcpy(((Commutercar*)res)[*count - 1].carnumber, substring(line, 0, CARNUMBERSIZE - 1));
        strcpy(((Commutercar*)res)[*count - 1].limittime, substring(line, CARNUMBERSIZE - 1, SIXDAYSIZE - 1));
    }

    if(*count == 0) return res;
    
    --(*count);
    ++(*n);
}

void* readFile(char* filename, int startline, int count, int *n) {
    //filename = 읽을 파일 이름, startline = 읽을 첫번째 라인, count = 첫번째 라인으로부터 몇라인 읽을지
    //NULL 리턴시 파일 이름 오류.
    FILE *fp;
    char line[MAX_CHAR_PER_LINE];
    int i = 0;
    void* res;

    if(access(filename, F_OK) == -1) return NULL;
    fp = fopen(filename, "rb");

    if(strcmp(filename, PARKINGCARINFOROUTE) == 0) {
        fseek(fp, SEEK_SET, PARKINGCARINFORECORDSIZE * startline);
        if(count != 0) {
            res = (Parkcar*)malloc(sizeof(Parkcar) * count);
            while(fgets(line, MAX_CHAR_PER_LINE, fp) && i < count) {
                strcpy(((Parkcar*)res)[i].carnumber, substring(line, 0, CARNUMBERSIZE - 1));
                strcpy(((Parkcar*)res)[i].phonenumber, substring(line, CARNUMBERSIZE - 1, PHONENUMBERSIZE - 1));
                strcpy(((Parkcar*)res)[i].intime, substring(line, (CARNUMBERSIZE - 1) + (PHONENUMBERSIZE - 1), INTIMESIZE - 1));
                printf("carnumber: %s\n", ((Parkcar*)res)[i].carnumber);
                printf("phonenumver: %s\n", ((Parkcar*)res)[i].phonenumber);
                printf("intime: %s\n\n", ((Parkcar*)res)[i].intime);
                ++i;
            }
            *n = i;
        }
        else {
            res = readFiletail(PARKINGCARINFOROUTE, fp, &i, res, n);

        }
    }
    else if(strcmp(filename, COMMUTERCARROUTE) == 0) {
        fseek(fp, SEEK_SET, COMMUTERCARRECORDSIZE * startline);
        if(count != 0) {
            res = (Commutercar*)malloc(sizeof(Commutercar) * count);
            while(fgets(line, MAX_CHAR_PER_LINE, fp) && i < count) {
                strcpy(((Commutercar*)res)[i].carnumber, substring(line, 0, CARNUMBERSIZE - 1));
                strcpy(((Commutercar*)res)[i].limittime, substring(line, CARNUMBERSIZE -1, SIXDAYSIZE));
                ++i;
            }
        }
        else {
            res = readFiletail(COMMUTERCARROUTE, fp, &i, res, n);
        }
    }
    fclose(fp);

    return res;
}


int size(void)  //큐 크기 확인
{
    Node *p;
    int count = 0;
    for ( p = front; p != NULL; p = p -> link) count++;
    return count;
}

void enqueue(Element e) {
    Node *p = (Node*)malloc(sizeof(Node));  //새로운 노드 생성(메모리 할당)
    p -> data = e;  //노드의 데이터멤버에 e 기입
    p -> link = NULL;   //노드의 link멤버에 NULL 기입
    
    if(is_empty()) front = rear = p;    //만약에 큐가 비어있다면, front와 rear 모두 p 구조체를 가리키도록 설정
    else {  //아니라면
        rear -> link = p;  //rear가 현재 가리키고 있는 노드의 link를 새로운 노드로 기입하고
        rear = p;   //rear는 새로운 노드를 가리키도록 설정
    }
}

Element dequeue(void) {
    Node *p;
    Element e;
    
    if(is_empty())
        error("큐 공백 에러");
    p = front; //p에 front에 저장되어있는 주소 기입
    front = front -> link;  //front에 현재 front가 가리키고 있는 구조체의 link 멤버에 저장된 주소 기입
    if(front == NULL) rear = NULL;  //만약에 저장된 주소가 null이라면 rear도 null로 초기화
    e = p -> data;  //e에 p가 가리키는 구조체에 저장된 data멤버변수의 값 기입
    free(p);
    return e;
}

Element peek(void)
{
    if(is_empty())
        error("큐 공백 에러");
    return front -> data;
}

void destroy_queue(void)
{
    while(is_empty() == 0)
        dequeue();
}

void print_queue(char *msg)
{
    Node *p;
    printf("%s[%2d]= ", msg, size());
    for(p = front; p != NULL; p = p -> link)
        printf("%s\n", p -> data.carnumber);
    printf("\n");
}

void getPlaceinfo(ParkingplaceSetting *Setting) {
    FILE *fp;
    char line[MAX_CHAR_PER_LINE];
    fp = fopen(PLACEINFOROUTE, "r");
    fgets(line, MAX_CHAR_PER_LINE, fp);
    printf("%s\n", line);
    Setting -> MaxCarSize = Stringtoint(substring(line, 0, 4));
    Setting -> FirstPee = Stringtoint(substring(line, 3, 5));
    Setting -> FirstPeeTime = Stringtoint(substring(line, 9, 3));
    Setting -> NextPee = Stringtoint(substring(line, 12, 5));
    Setting -> NextPeeTime = Stringtoint(substring(line, 17, 3));
    fclose(fp);
}

void getFeeoptioninfo(ParkingplaceSetting *Setting) {
    FILE *fp;
    char line[MAX_CHAR_PER_LINE];
    fp = fopen(FEEOPTIONROUTE, "r");
    fgets(line, MAX_CHAR_PER_LINE, fp);
    printf("%s\n", line);
    Setting -> GetWeekendPee = Stringtoint(substring(line, 0, 1));
    Setting -> GetBigCarPeePercent = Stringtoint(substring(line, 1, 3));
    Setting -> ChangePlusMoney = Stringtoint(substring(line, 4, 5));
    Setting -> PlusMoneyTime = Stringtoint(substring(line, 9, 3));
    Setting -> PlusMoneyPlus = Stringtoint(substring(line, 12, 3));
    printf("%s\n", (substring(line, 15, 5)));
    Setting -> PeeOfDay = Stringtoint (substring(line, 15, 5));
    fclose(fp);
}

void savePlaceinfo(ParkingplaceSetting Setting) {
    FILE *fp;
    fp = fopen(PLACEINFOROUTE, "w");
    fprintf(fp, "%d", Setting.MaxCarSize);
    fprintf(fp, "%d", Setting.FirstPee);
    fprintf(fp, "%d", Setting.FirstPeeTime);
    fprintf(fp, "%d", Setting.NextPee);
    fprintf(fp, "%d", Setting.NextPeeTime);
    fclose(fp);
}

void seveFeeoptioninfo(ParkingplaceSetting Setting) {
    FILE *fp;
    fp = fopen(FEEOPTIONROUTE, "w");
    fprintf(fp, "%d", Setting.GetWeekendPee);
    fprintf(fp, "%d", Setting.GetBigCarPeePercent);
    fprintf(fp, "%d", Setting.ChangePlusMoney);
    fprintf(fp, "%d", Setting.PlusMoneyTime);
    fprintf(fp, "%d", Setting.PlusMoneyPlus);
    fprintf(fp, "%d", Setting.PeeOfDay);
    fclose(fp);
}

/*
void getSetting() {
    JSON_Value *rootvalue;
    JSON_Object *rootobj;
    
    rootvalue = json_parse_file("setting.json");
    rootobj = json_value_get_object(rootvalue);

    Setting.MaxCarSize = json_object_get_number(rootobj, "MAXCARSIZE");
    Setting.FirstPee = json_object_get_number(rootobj, "FIRSTPEE");
    Setting.FirstPeeTime = json_object_get_number(rootobj, "FIRSTPEETIME");
    Setting.NextPee = json_object_get_number(rootobj, "NEXTPEE");
    Setting.NextPeeTime = json_object_get_number(rootobj, "NEXTPEETIME");
    Setting.GetWeekendPee = json_object_get_number(rootobj, "GETWEEKENDPEE");
    Setting.GetBigCarPeePercent = json_object_get_number(rootobj, "GETBIGCARPEEPERCENT");
    Setting.ChangePlusMoney = json_object_get_number(rootobj, "CHANGEPLUSMONEY");
    Setting.PlusMoneyTime = json_object_get_number(rootobj, "PLUSMONEYTIME");
    Setting.PlusMoneyPlus = json_object_get_number(rootobj, "PLUSMONEYPLUS");
    Setting.PeeOfDay = json_object_get_number(rootobj, "PEEOFDAY");
}
*/