#include <cstdio>
#include <cstring>
//#include "conio2.h"
#include "interface.h"
#include "checkerslib.h"
#include <windows.h>

#define enableCP1251 SetConsoleCP(1251); SetConsoleOutputCP(1251)

const int WINDOW_SIZE_LENGTH = 80;
const int WINDOW_SIZE_HEIGTH = 20;

void renderBoard(Board* board, short drawX, short drawY) {
    drawFrame(10, 10, drawX, drawY);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            short bStat = board->redreredBoardTemplate[j][i];
            gotoxy(drawX + 1 + i, drawY + 8 - j);
            switch(bStat) {
                case EMPTY_BLACK_CELL: {
                    textbackground(0);
                    printf(" ");
                    break;
                }
                case EMPTY_WHITE_CELL: {
                    textbackground(15);
                    printf(" ");
                    break;
                }
                case FIGURE_REGULAR_BLACK: {
                    textcolor(12);
                    printf("O");
                    break;
                }
                case FIGURE_REGULAR_WHITE: {
                    textcolor(15);
                    printf("O");
                    break;
                }
                case FIGURE_KING_BLACK: {
                    textcolor(12);
                    printf("W");
                    break;
                }
                case FIGURE_KING_WHITE: {
                    textcolor(15);
                    printf("W");
                    break;
                }
                case AVALIABLE_MOVEMENT_CELL: {
                    textbackground(2);
                    printf(" ");
                    break;
                }
            }
            textbackground(0);
        }
    }
    textbackground(0);
    textcolor(15);
}
//render
int main() {

    clrscr();
    hideCursor();
    enableCP1251;
    setWindowSize(WINDOW_SIZE_LENGTH, WINDOW_SIZE_HEIGTH);
    textcolor(15);
    //printf("AAA");
    //waitForKey(13);
    //drawFrame(WINDOW_SIZE_LENGTH, WINDOW_SIZE_HEIGTH, 1, 1);
    const char* mainMenuPoints[36] = {
        "Начать новую игру",
        "Загрузить игру",
        "Правила игры",
        "Статистика",
        "Выйти из игры"
    };
    const char* mainMenuPointsDescriptions[50] = {
        "Играйте против реального игрока или компьютера",
        "Продолжите сохранённую ранее игру, загрузив её",
        "Почитать о правилах игры",
        "Узнать статистику",
        "Уже уходите?"
    };
    TextLineWidget tlw;
    tlw.length = 29;
    tlw.locX = 4; tlw.locY = 2;
    setInitialText(&tlw, "pidarok");
    //strcpy(tlw.text, "pidarok");
    drawTextLineWidget(tlw);
    //int k = drawMenu(1, mainMenuPoints, 4, )
    int k = drawMenu(1, mainMenuPoints, 5, mainMenuPointsDescriptions, 5, 4, 4, 15, 3, 12);
    if (k == -1) {printf("mpdcount != pointscount");}
    Board gameBoard = initiateGameBoard(true);
    renderBoard(&gameBoard, 4, 4);
    //test1.length = 30; test1.locX = 3; test1.locY = 4;
    //strcpy(test1.text, "EEEEEEEEEEEE");
    //char sysSWS[64];
    //sprintf(sysSWS, "mode con cols=%d lines=%d", 10, 10);
    //gotoxy(1, 2);
    //puts(sysSWS);
    //puts(sysSWS);
    //char* menu[] = {" Aga", "Bebe"};
    //drawTextLineWidget(test1);
    //drawMenu(1, menu, 2, 5, 5, 4, 4, 12);

    //drawFrame(5, 6, 5, 6);
    waitForKey(13);
    //setTextLineText(test1, "EEEAAAAA");
    return 0;
}
