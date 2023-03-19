#include <windows.h>
#include <cstdio>
#include "conio2.h"

#define max(x, y) ((x > y) ? x : y)

#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

struct TextLineWidget {
    int locX, locY, length;
    char text[64];
};


void waitForKey(int code) {
    int key;
    do key = getch(); while (key != code);
    while(kbhit() != 0) key = getch();
}

void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void setInitialText(TextLineWidget* widget, const char* text) {
    strcpy(widget->text, text);
}

void setTextLineText(TextLineWidget widget, const char* text) {
    gotoxy(widget.locX + 1, widget.locY + 1);
    char* null = new char[widget.length - 1];
    null[widget.length - 2] = 0; for (int i = 0; i < widget.length - 2; i++) null[i] = ' ';
    printf(null);
    gotoxy(widget.locX + 1, widget.locY + 1);
    printf(text);
}

void setWindowSize(int columns, int lines) {
    char sysSWS[64];
    sprintf(sysSWS, "mode con cols=%d lines=%d", columns, lines);
    system(sysSWS);
}

void drawFrame(int width, int height, int drawX, int drawY) {
    gotoxy(drawX, drawY);
    char* topBorder = new char[width + 1];
    char* bottomBorder = new char[width + 1];
    char* rlBorder = new char[width + 1];
    topBorder[0] = '#'; bottomBorder[0] = '#'; rlBorder[0] = '|';
    topBorder[width - 1] = '#'; bottomBorder[width - 1] = '#'; rlBorder[width - 1] = '|';
    topBorder[width] = 0; bottomBorder[width] = 0; rlBorder[width] = 0;
    for (int i = 1; i < width - 1; i++) {
        topBorder[i] = '='; bottomBorder[i] = '='; rlBorder[i] = ' ';
    }
    cprintf(topBorder);
    for (int i = 0; i < height - 2; i++) {
        gotoxy(drawX, drawY + i + 1);
        cprintf(rlBorder);
    }
    gotoxy(drawX, drawY + height - 1);
    cprintf(bottomBorder);
}

void drawTextLineWidget(TextLineWidget widget) {
    //gotoxy(widget.locX, widget.locY);
    drawFrame(widget.length, 3, widget.locX, widget.locY);
    gotoxy(widget.locX + 1, widget.locY + 1);
    printf(widget.text);
}

int drawMenu(
        const char* title,
        int activePos,
        const char* menuPoints[],
        int pointsCount,
        const char* menuPointsDescriptions[],
        int MPDCount,
        int drawX,
        int drawY,
        int alignCenterH,
        int alignCenterV,
        short activePointColor
    ) {
    int key;
    gotoxy(drawX, drawY);
    int maxLen = -1;
    if (pointsCount != MPDCount) return -1;
    for (int i = 0; i < pointsCount; i++) if ((int) strlen(menuPoints[i]) > maxLen) maxLen = max((int) strlen(menuPoints[i]), maxLen);
    TextLineWidget menuPointDescription;
    menuPointDescription.length = maxLen + alignCenterH*2 + 2;
    menuPointDescription.locX = drawX; menuPointDescription.locY = drawY + pointsCount + alignCenterV * 2;

    drawFrame(maxLen + alignCenterH*2 + 2, pointsCount + alignCenterV * 2 + 2, drawX, drawY);
    gotoxy(drawX + alignCenterH + maxLen / 2 - (int) strlen(title) / 2 + 1,drawY);
    printf(title);
    for (int i = 0; i < pointsCount; i++) {
        gotoxy(drawX + alignCenterH + 1, drawY + alignCenterV + 1 + i);
        printf(menuPoints[i]);
    }
    textbackground(activePointColor);
    gotoxy(drawX + alignCenterH + 1, drawY + alignCenterV + activePos);
    printf(menuPoints[activePos - 1]);
    textbackground(0);
    drawTextLineWidget(menuPointDescription);
    setTextLineText(menuPointDescription, menuPointsDescriptions[activePos - 1]);
    do {
        key = getch();
        if (key != 13) {
            textbackground(0);
            gotoxy(drawX + alignCenterH + 1, drawY + alignCenterV + activePos);
            printf(menuPoints[activePos - 1]);
            setTextLineText(menuPointDescription, menuPointsDescriptions[activePos - 1]);
            if (key == 224) {
                key = getch();
                if (key == 80) {
                    if (activePos == pointsCount) activePos = 1;
                    else activePos += 1;
                }
                else if (key == 72) {
                    if (activePos == 1) activePos = pointsCount;
                    else activePos -= 1;
                }
            }
            textbackground(activePointColor);
            gotoxy(drawX + alignCenterH + 1, drawY + alignCenterV + activePos);
            printf(menuPoints[activePos - 1]);
            textbackground(0);
            setTextLineText(menuPointDescription, menuPointsDescriptions[activePos - 1]);
        }
    } while (key != 13);
    while (kbhit() != 0) getch();
    textbackground(0);
    return activePos;
}

int drawMenu(
        int activePos,
        const char* menuPoints[],
        int pointsCount,
        //const char* menuPointsDescriptions[],
        //int MPDCount,
        int drawX,
        int drawY,
        short activePointColor
) {
    int key;
    gotoxy(drawX, drawY);
    for (int i = 0; i < pointsCount; i++) {
        gotoxy(drawX, drawY + i);
        printf(menuPoints[i]);
    }
    textbackground(activePointColor);
    gotoxy(drawX, drawY + activePos - 1);
    printf(menuPoints[activePos - 1]);
    textbackground(0);
    do {
        key = getch();
        if (key != 13) {
            textbackground(0);
            gotoxy(drawX, drawY + activePos - 1);
            printf(menuPoints[activePos - 1]);
            if (key == 224) {
                key = getch();
                if (key == 80) {
                    if (activePos == pointsCount) activePos = 1;
                    else activePos += 1;
                }
                else if (key == 72) {
                    if (activePos == 1) activePos = pointsCount;
                    else activePos -= 1;
                }
            }
            textbackground(activePointColor);
            gotoxy(drawX, drawY + activePos - 1);
            printf(menuPoints[activePos - 1]);
            textbackground(0);
        }
    } while (key != 13);
    while (kbhit() != 0) getch();
    textbackground(0);
    return activePos;
}

//void clearField(int x, int y, int length, int heigth) {

//}

#endif // INTERFACE_H_INCLUDED
