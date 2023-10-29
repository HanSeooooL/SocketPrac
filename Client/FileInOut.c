
#define MAX_CHAR_PER_LINE 1024

#define PARKINGCARINFOROUTE "Parkinginfo.txt"
#define COMMUTERINFOROUTE "Commuterinfo.txt"
#define PLACEINFOROUTE "Placeinfo.txt"

#include "Parking_client.h"
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
int is_empty(void) { return front == NULL; }    //ť�� ������� Ȯ��
void init_queue(void) { front = rear = NULL; }  //ť �ʱ�ȭ
int size(void); //ť ������ Ȯ��
void enqueue(Element e);    //������ ����
Parkcar dequeue(void);  //������ ��������(ť���� ����)
Element peek(void);     //������ ��������(ť���� ����X)
void destroy_queue(void);   //ť �ʱ�ȭ
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
    if(access(PARKINGCARINFOROUTE, F_OK) != -1) { //������ �ִ°��
        fp = fopen(PARKINGCARINFOROUTE, "a"); //���Ͽ� ������ �߰�
        fprintf(fp, "%s", newone.carnumber);
        fprintf(fp, "%s", newone.phonenumber);
        fprintf(fp, "%s", newone.intime);
        fprintf(fp, "\n");
        
        
    } else {    //������ ���� ���
        fp = fopen(PARKINGCARINFOROUTE, "w"); //���� ���� ����
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
    if(access(PARKINGCARINFOROUTE, F_OK) != -1) { //������ �ִ� ���
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
    else error("������ �����ϴ�");
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
    else error("������ �����ϴ�");
    
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
            
            return searching;   //�����ϴ� ��� ����ü ��ȯ �� count�����͸� ���� ���� ��ȯ
        }
    }
    
    *count = FAIL; //������ ã�� ���߽��ϴ�.
    return searching;
}



int size(void)  //ť ũ�� Ȯ��
{
    Node *p;
    int count = 0;
    for ( p = front; p != NULL; p = p -> link) count++;
    return count;
}

void enqueue(Element e) {
    Node *p = (Node*)malloc(sizeof(Node));  //���ο� ��� ����(�޸� �Ҵ�)
    p -> data = e;  //����� �����͸���� e ����
    p -> link = NULL;   //����� link����� NULL ����
    
    if(is_empty()) front = rear = p;    //���࿡ ť�� ����ִٸ�, front�� rear ��� p ����ü�� ����Ű���� ����
    else {  //�ƴ϶��
        rear -> link = p;  //rear�� ���� ����Ű�� �ִ� ����� link�� ���ο� ���� �����ϰ�
        rear = p;   //rear�� ���ο� ��带 ����Ű���� ����
    }
}

Parkcar dequeue(void) {
    Node *p;
    Parkcar e;
    
    if(is_empty())
        error("ť ���� ����");
    p = front; //p�� front�� ����Ǿ��ִ� �ּ� ����
    front = front -> link;  //front�� ���� front�� ����Ű�� �ִ� ����ü�� link ����� ����� �ּ� ����
    if(front == NULL) rear = NULL;  //���࿡ ����� �ּҰ� null�̶�� rear�� null�� �ʱ�ȭ
    e = p -> data;  //e�� p�� ����Ű�� ����ü�� ����� data��������� �� ����
    free(p);
    return e;
}

Element peek(void)
{
    if(is_empty())
        error("ť ���� ����");
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
