#define _CRT_SECURE_NO_WARNINGS
#include "Parking_client.h"
#include <Windows.h>
#include <math.h>
#include <conio.h>
#include <stdarg.h>

//UI관련 상수
#define SCREENCOLS 138
#define SCREENLINES 42
#define BUTTONHOR 26
#define BUTTONVER 3
#define ELEMENTBUTTONHOR 15
#define ELEMENTBUTTONVER 3
#define HORIZONAL 0
#define VERTICAL 1

INPUT_RECORD rec;
DWORD dwNOER;
HANDLE CIN = 0;

//Click체크용 좌표 기록
int *menux, *menuy, saved;
int *elementx, *elementy, elementsaved;

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

void click(int *xx, int *yy, int *lr) {
    while(1) {
        ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &rec, 1, &dwNOER);
        if(rec.EventType == MOUSE_EVENT) {
            if(rec.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
                int mouse_x = rec.Event.MouseEvent.dwMousePosition.X;
                int mouse_y = rec.Event.MouseEvent.dwMousePosition.Y;
                *xx = mouse_x;
                *yy = mouse_y;
                *lr = 1;

                break;
            }
            else if(rec.Event.MouseEvent.dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED) {
                int mouse_x = rec.Event.MouseEvent.dwMousePosition.X;
                int mouse_y = rec.Event.MouseEvent.dwMousePosition.Y;
                *xx = mouse_x;
                *yy = mouse_y;
                *lr = 2;

                break;
            }
        }
    }
}

void printText(int x, int y, char* text) {
    gotoxy(x, y);
    printf("%s", text);
}

void defaultscreensetting() {
    system("mode con cols=138 lines=42");
	system("주차장 정산관리 프로그램");
    SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);
    incursor(0);
}

void printScreenTitle(char* title) {
    //입력된 문자열을 화면의 가장 위 정중앙에 출력하는 함수
    int str_len, halfscreen;
    str_len = strlen(title);
    halfscreen = getCenterofhorizonal(0, SCREENCOLS, str_len);
    printText(halfscreen, 1, title);
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

int getCenterofVertical(int vero, int vert, int ylen) {
    int res;
    if(vero == vert) return vero;
    if(ylen != 0) ylen = ylen >> 1;
    res = ((vert - vero) >> 1) + vero;
    res -= ylen;
    if (res < 0) return FAIL;
    return res;
}

void printTexttoCenter(int horo, int vero, int hort, int vert, char* text) {
    //x축과 y축을 받아서 중심좌표에 text출력
    int destination, horres, verres;
    horres = getCenterofhorizonal(horo, hort, strlen(text));
    verres = getCenterofVertical(vero, vert, 0);
    if(horres == FAIL) error_handling("공간이 너무 작습니다.(printTexttoCenter)");
    printText(horres, verres, text);
}

void printDataList(int x, int y, int colgap, int rawgap, int datacount, void* data, short datatype) {
    //x, y = 시작 좌표 colgap = 가로 간격 rawgap = 세로 간격 datacount = 데이터 개수 data = 출력할 데이터 주소 datatype = 출력할 데이터의 타입
    if(datatype == DT_PARKCAR) {
        printText(x, y, "차량번호");
        printText(x + colgap, y, "전화번호");
        printText(x + (colgap * 2), y, "입차시각");
        y += rawgap;

        for(int i = 0; i < datacount; i++) {
            printText(x, y, ((Parkcar*)data)[i].carnumber);
            printText(x + colgap, y, ((Parkcar*)data)[i].phonenumber);
            printText(x + (colgap * 2), y, ((Parkcar*)data)[i].intime);
            y += rawgap;
        }
    }
}

void printGraph(int x, int y, int alldata, int successdata) {
    float percentage = (float)successdata / (float)alldata;
    int key = ((float)successdata / (float)alldata) * 100;
    if(percentage * 100 + 5 == (key % 10) + 10) key += 10;
    key = key % 10;
    for(int i = 0; i < key; i++) {
        x += i * 2;
        printText(x, y, "?");
    }
    key = 10 - key;
    for(int i = 0; i < key; i++) {
        x += i * 2;
        printText(x, y, "?");
    }
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

void makeButtonClickEvent(int x, int y) {
    x = menux[saved];
    y = menuy[saved];
    saved += 1;
}

void makeElementButtonClickEvent(int x, int y) {
    x = elementx[elementsaved];
    y = elementy[elementsaved];
    saved += 1;
}

void Button(int x, int y, char* name) {
    makeBox(x, y, BUTTONHOR, BUTTONVER);
    printTexttoCenter(x, y, x + BUTTONHOR, y + BUTTONVER, name);
    makeButtonClickEvent(x, y);
}

void ElementButton(int x, int y, char* name) {
    makeBox(x, y, ELEMENTBUTTONHOR, ELEMENTBUTTONVER);
    printTexttoCenter(x, y, x + ELEMENTBUTTONHOR, y + ELEMENTBUTTONVER, name);
    makeElementButtonClickEvent(x, y);
}

int checkClickEvent(int xx, int yy, int lr) {
    for(int i = 0; i < saved; i++) {
        if((menux[i] <= xx <= (menux[i] + BUTTONHOR)) &&
            (menuy[i] <= yy <= (menuy[i] + BUTTONVER))) {
                return i;
            }
    }
}

void Layout1(char* title) {
    system("cls");
    printScreenTitle(title);
    //makeBox(2, 4, 100, 34);
    //makeBox(106, 4, 50, 12);
    //makeBox(106, 17, 50, 21);
    makeBox(2, 4, (((SCREENCOLS - 4) / 3) * 2) - 2, SCREENLINES - 5);
    makeBox(2 + (((SCREENCOLS - 4) / 3) * 2) + 1, 4, (SCREENCOLS - 4) / 3 - 2, (SCREENLINES - 6) / 3 + 1);
    makeBox(2 + (((SCREENCOLS - 4) / 3) * 2) + 1, (4 + (SCREENLINES - 6) / 3 + 1) + 1, (SCREENCOLS - 4) / 3 - 2, ((SCREENLINES - 6) / 3 * 2) - 1);
}

void Layout2(char* title, int Boxsizex, int BoxsizeY) {
    system("cls");
    printScreenTitle(title);
    makeBox(getCenterofhorizonal(0, SCREENCOLS, Boxsizex), getCenterofVertical(4, SCREENLINES, BoxsizeY), Boxsizex, BoxsizeY);
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

void Layout4(char* title, int leftboxx, int leftboxy, int rightboxx, int rightboxy) {
    //ppt 43
    system("cls");
    printScreenTitle(title);
    makeBox(getCenterofhorizonal(0, SCREENCOLS, leftboxx) - (leftboxx / 2), getCenterofVertical (4, SCREENLINES, leftboxy), leftboxx, leftboxy);
    makeBox(getCenterofhorizonal(0, SCREENCOLS, rightboxx) + (leftboxx / 2), getCenterofVertical(4, SCREENLINES, rightboxy), rightboxx, rightboxy);
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

void Layout1_menuButtons(int choice, int n, char **names) {
    int *yarr, x;
    free(menux);
    free(menuy);
    saved = 0;
    if(choice == 0) {
        menux = (int*)malloc(sizeof(int) * n);
        menuy = (int*)malloc(sizeof(int) * n);
    }
  
    else if(choice == 1) {
        menux = (int*)malloc(sizeof(int) * (n + 1));
        menuy = (int*)malloc(sizeof(int) * (n + 1));
    }

    yarr = (int*)malloc(sizeof(int) * (n + 1));
    x = (((SCREENLINES - 6) / 3 * 2) - 1 - 4) / n;
    yarr[0] = 0;

    for(int i = 1 ; i < n; i ++) {
        yarr[i] = x * i;
    }
    yarr[n] = ((SCREENLINES - 6) / 3 * 2) - 1 - 4;

    for(int i = 0; i < n; i++) {
        Button(getCenterofhorizonal(2 + (((SCREENCOLS - 4) / 3) * 2) + 1, 2 + (((SCREENCOLS - 4) / 3) * 2) + 1 + (SCREENCOLS - 4) / 3 - 2, BUTTONHOR),
        getCenterofVertical(yarr[i], yarr[i + 1], 0) + (4 + (SCREENLINES - 6) / 3 + 1) + 1, names[i]);
        
    }

    free(names);
    free(yarr);
}

void Layout2_Buttons(int n, char **names) {

}

void Layout3_Buttons(int n, char **names) {

}

void Layout4_Buttons(int n, char **names) {

}

void UI_1100() {

}

void UI_0100() {

}

void UI_0003() {

}

void UI_0002() {

}

void UI_0001() {

}

void UI_0000() {
    int xx, yy, lr, choice;
    Parkcar *array;
    array = (Parkcar*)malloc(sizeof(Parkcar) * 10);
    Layout1("주차 정산관리 프로그램");
    Layout1_menuButtons(1, 3, makeButtonNameArray(3, "입차", "주차차량검색", "정기권 관리"));
    readParkinginfo();
    for(int i = 0; i < 2; i++) {
        array[i] = dequeue();
    }
    printDataList(8, 6, 20, 2, 2, (void*)array, DT_PARKCAR);
    while(1) {
        click(&xx, &yy, &lr);
        choice = checkClickEvent(xx, yy, lr);
        switch(choice) {
            case 0:
                UI_0001();
                break;
            case 1:
                UI_0002();
                break;
            case 2:
                UI_0100();
                break;
            case 3:
                UI_1100();
                break;
            default:
                break;
        }
    }
}

void ProgramStart() {
    defaultscreensetting();
    UI_0000();
}
