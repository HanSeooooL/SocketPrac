
#define MAX_CHAR_PER_LINE 1024

#define PARKINGCARINFOROUTE "Parkinginfo.txt"
#define COMMUTERINFOROUTE "Commuterinfo.txt"
#define PLACEINFOROUTE "Placeinfo.txt"

#include "Parking_server.h"
#include <unistd.h>

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

void readParkinginfo(void) {
    FILE *fp;
    char line[MAX_CHAR_PER_LINE];
    Parkcar newone;
    if(access(PARKINGCARINFOROUTE, F_OK) != -1) { //파일이 있는 경우
        destroy_queue();
        fp = fopen(PARKINGCARINFOROUTE, "r");
        while(fgets(line, MAX_CHAR_PER_LINE, fp)) {
            strncpy(newone.carnumber, line, CARNUMBERSIZE - 1);
            newone.carnumber[CARNUMBERSIZE - 1] = '\0';
            strncpy(newone.phonenumber, &line[CARNUMBERSIZE - 1], PHONENUMBERSIZE - 1);
            newone.phonenumber[PHONENUMBERSIZE - 1] = '\0';
            strncpy(newone.intime, &line[(CARNUMBERSIZE - 1) + (PHONENUMBERSIZE - 1)], INTIMESIZE - 1);
            newone.intime[INTIMESIZE - 1] = '\0';
            enqueue(newone);
        }
    }
    else error("파일이 없습니다");
    fclose(fp);
}

void deletetheCar(int count) {
    FILE *fp;
    Parkcar one;
    int limit = size();
    if(access(PARKINGCARINFOROUTE, F_OK) != -1) {
        readParkinginfo();
        fp = fopen(PARKINGCARINFOROUTE, "w");
        for(int i = 0; i <= limit; i++) {
            count -= 1;
            one = dequeue();
            if(count == 0) {
                count = -1;
            }
            else {
                fprintf(fp, "%s", one.carnumber);
                fprintf(fp, "%s", one.phonenumber);
                fprintf(fp, "%s", one.intime);
                fprintf(fp, "\n");
            }
        }
        
    }
    else error("파일이 없습니다");
    
    fclose(fp);
}

Parkcar searchtheCar(char *targetcarnumber, int *count) {
    Parkcar searching;
    destroy_queue();
    readParkinginfo();
    
    for(int i = 0; i <= size(); i++) {
        
        *count += 1;
        searching = dequeue();
        if(!strcmp(searching.carnumber, targetcarnumber)) {
            
            return searching;   //존재하는 경우 구조체 반환 및 count포인터를 통한 순서 반환
        }
    }
    
    *count = FAIL; //차량을 찾지 못했습니다.
    return searching;
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
