#define _CRT_SECURE_NO_WARNINGS
#include "Parking_client.h"
#include <Windows.h>
#include <math.h>
#include <conio.h>

#define DT_PARKCAR 3

#define SCREENCOLS 160
#define SCREENLINES 40
#define BUTTONHOR 26
#define BUTTONVER 5

void gotoxy(int x, int y)	//gotoxy
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void textcolor(int color_number)	//���� ��
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
	system("������ ������� ���α׷�");
    incursor(1);
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
    //x��� y���� �޾Ƽ� �߽���ǥ�� text���
    int destination, horres, verres;
    horres = getCenterofhorizonal(horo, hort, strlen(text));
    verres = getCenterofVertical(vero, vert);
    if(horres == FAIL) error_handling("������ �ʹ� �۽��ϴ�.(printTexttoCenter)");
    printText(horres, verres, text);
}

void printDataList(int x, int y, int colgap, int rawgap, int datacount, void* data, short datatype) {
    //x, y = ���� ��ǥ colgap = ���� ���� rawgap = ���� ���� datacount = ������ ���� data = ����� ������ �ּ� datatype = ����� �������� Ÿ��
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
    //�Էµ� ���ڿ��� ȭ���� ���� �� ���߾ӿ� ����ϴ� �Լ�
    int str_len, halfscreen;
    str_len = strlen(title);
    halfscreen = getCenterofhorizonal(0, SCREENCOLS, str_len);
    printText(halfscreen, 1, title);
}

void makeBox(int x, int y, int hor, int ver) {
    //hor = ���� ver = ����
    //(x, y) = ������ǥ
    //�Էµ� ���μ��� ũ���� ���ڸ� ����ϴ� �Լ�
    for (int i = 0; i < ver; i++) {
        gotoxy(x, y);
        if(i == 0) {
            printf("��");
            for(int i = 0; i < hor - 2; i++) {
                printf("��");
            }
            printf("��");
        }
        else if(i == ver - 1) {
            printf("��");
            for(int i = 0; i < hor - 2; i++) {
                printf("��");
            }
            printf("��");  
        }
        else {
            printf("��");
            gotoxy(x + hor - 1, y);
            printf("��");
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
}

void UI_0000() {
    Layout3("���� ������� ���α׷�");
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
