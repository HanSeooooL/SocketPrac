#define _CRT_SECURE_NO_WARNINGS
#include "Parking_client.h"
#include <Windows.h>
#include <math.h>
#include <conio.h>
#include <stdarg.h>

//데이터 타입 코드
#define DT_PARKCAR 3

//데이터 타입 코드
#define SCREENCOLS 160
#define SCREENLINES 40
#define BUTTONHOR 26
#define BUTTONVER 5

void gotoxy(int x, int y)	//gotoxy
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void textcolor(int color_number)	//글자 색
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_number);
}

void incursor(int n)
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	ConsoleCursor.bVisible = n;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

void printText(int x, int y, char* text) {
    gotoxy(x, y);
    printf("%s", text);
}

void defaultscreensetting() {
    system("mode con cols=160 lines=40");
	system("주차장 정산관리 프로그램");
    incursor(0);
}

int getCenterofhorizonal(int horo, int hort, int textlen) {
    int res;
    if(horo == hort) return horo;
    if(textlen != 0) textlen = textlen >> 1;
    res = ((hort - horo) >> 1) + horo;
    res -= textlen;
    if (res < 0) return FAIL;
    return res;
}

int getCenterofVertical(int vero, int vert) {
    int res;
    if(vero == vert) return vero;
    res = (vert - vero) >> 1;
    res += vero;
    return res;
}

void printTexttoCenter(int horo, int vero, int hort, int vert, char* text) {
    //x축과 y축을 받아서 중심좌표에 text출력
    int destination, horres, verres;
    horres = getCenterofhorizonal(horo, hort, strlen(text));
    verres = getCenterofVertical(vero, vert);
    if(horres == FAIL) error_handling("공간이 너무 작습니다.(printTexttoCenter)");
    printText(horres, verres, text);
}

void printDataList(int x, int y, int colgap, int rawgap, int datacount, void* data, short datatype) {
    //x, y = 시작 좌표 colgap = 가로 간격 rawgap = 세로 간격 datacount = 데이터 개수 data = 출력할 데이터 주소 datatype = 출력할 데이터의 타입
    if(datatype == DT_PARKCAR) {
        for(int i = 0; i < datacount; i++) {
            printText(x, y, ((Parkcar*)data)[i].carnumber);
            printText(x + colgap, y, ((Parkcar*)data)[i].phonenumber);
            printText(x + (colgap * 2), y, ((Parkcar*)data)[i].intime);
            y += rawgap;
        }
    }
}

void printScreenTitle(char* title) {
    //입력된 문자열을 화면의 가장 위 정중앙에 출력하는 함수
    int str_len, halfscreen;
    str_len = strlen(title);
    halfscreen = getCenterofhorizonal(0, SCREENCOLS, str_len);
    printText(halfscreen, 1, title);
}

void makeBox(int x, int y, int hor, int ver) {
    //hor = 가로 ver = 세로
    //(x, y) = 시작좌표(왼쪽 꼭짓점 좌표)
    //입력된 가로세로 크기의 상자를 출력하는 함수
    for (int i = 0; i < ver; i++) {
        gotoxy(x, y);
        if(i == 0) {
            printf("┏");
            for(int i = 0; i < hor - 2; i++) {
                printf("━");
            }
            printf("┓");
        }
        else if(i == ver - 1) {
            printf("┗");
            for(int i = 0; i < hor - 2; i++) {
                printf("━");
            }
            printf("┛");  
        }
        else {
            printf("┃");
            gotoxy(x + hor - 1, y);
            printf("┃");
        }
        y += 1;
    }
}

void Button(int x, int y, char* name) {
    makeBox(x, y, BUTTONHOR, BUTTONVER);
    printTexttoCenter(x, y, x + BUTTONHOR, y + BUTTONVER, name);
}

void Layout1(char* title) {
    system("cls");
    printScreenTitle(title);
    makeBox(2, 4, 100, 34);
    makeBox(106, 4, 50, 12);
    makeBox(106, 17, 50, 21);
}

void Layout2(char* title) {
    system("cls");
    printScreenTitle(title);
    makeBox(getCenterofhorizonal(0, SCREENCOLS, 60), 10, 60, 15);
}

void Layout3(char* title) {
    system("cls");
    printScreenTitle(title);
    makeBox(2, 4, SCREENCOLS - 5, SCREENLINES - 6);
    makeBox(4, 5, ((SCREENCOLS - 9) / 3), (SCREENLINES - 8) / 2);
    makeBox(4 + ((SCREENCOLS - 9) / 3) + 1, 5, ((SCREENCOLS - 9) / 3), (SCREENLINES - 8) / 2);
    makeBox(4, 5 + (SCREENLINES - 8) / 2, ((SCREENCOLS - 9) / 3), (SCREENLINES - 8) / 2);
    makeBox(4 + ((SCREENCOLS - 9) / 3) + 1, 5 + (SCREENLINES - 8) / 2, ((SCREENCOLS - 9) / 3), (SCREENLINES - 8) / 2);
    makeBox(4 + ((SCREENCOLS - 9) / 3 * 2) + 2, 5, ((SCREENCOLS - 9) / 3) - 1, SCREENLINES - 8);
}

void Layout4(char* title) {
    //ppt 43
    system("cls");
    printScreenTitle(title);
    makeBox(getCenterofhorizonal(0, SCREENCOLS, 60) - 30, 10, 60, 15);
    makeBox(getCenterofhorizonal(0, SCREENCOLS, 60) + 30, 10, 50, 15);
}

char** makeButtonNameArray(int count, ...) {
    //count = 문자열 개수, 그 뒤에 문자열 입력
    va_list ap;
    int i;
    char** strarr = NULL;
    char* str = NULL;

    va_start(ap, count);
    strarr = (char**)malloc(sizeof(char*) * count);

    for(int i = 0; i < count; i++) {
        str = va_arg(ap, char*);
        strarr[i] = (char*)malloc(sizeof(char) * strlen(str));
        strcpy(strarr[i], str);
    }

    return strarr;
    
}

void Layout1_Buttons(int n, char **names) {
    int x, y;
    
    for(int i; i < n; i++) {

    }
}

void Layout2_Buttons(int n, char **names) {

}

void Layout3_Buttons(int n, char **names) {

}

void Layout4_Buttons(int n, char **names) {

}

void UI_0000() {
    Layout4("주차 정산관리 프로그램");
}

void UI_0001() {

}

void UI_0002() {

}

void UI_0003() {

}

void ProgramStart() {
    defaultscreensetting();
    UI_0000();
    while(1) {

    }
}
