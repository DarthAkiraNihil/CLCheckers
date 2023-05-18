//
// Created by EgrZver on 05.05.2023.
//

#include <windows.h>

#ifndef CHECKERS_GRAPHIC_SUBSYSTEM_H
#define CHECKERS_GRAPHIC_SUBSYSTEM_H

HBITMAP boardTextures[22], boardBorder;
HINSTANCE gInstance;

Coordinates cursorToBoardCoord(Coordinates cursor, Color playerSide) {
    if (playerSide == Black) return {7 - cursor.x, cursor.y};
    else return {cursor.x, 7 - cursor.y};
}

Coordinates getPasteWindowCoordinates() {
    RECT screen;
    GetWindowRect(GetDesktopWindow(), &screen);
    return {(screen.right - WINDOW_SIZE_LENGTH) / 2, (screen.bottom - WINDOW_SIZE_HEIGHT) / 2};
}

inline Coordinates getPasteCoords(int x, int y, Color playerSide) {
    if (playerSide == Black) {
        return {33 + boardPasteX + 56 * (7 - x), 33 + boardPasteY + 56 * y};
    }
    else {
        return {33 + boardPasteX + 56 * x, 33 + boardPasteY + 56 * (7 - y)};
    }
}

void loadBoardTextures() {
    char fPath[255], *fName;
    for (int i = 0; i < 22; i++) {
        GetFullPathNameA(textures[i], 255, fPath, &fName);
        boardTextures[i] = (HBITMAP) LoadImage(gInstance, fPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    }
}

HBITMAP loadImage(const char* path) {
    char fPath[255], *fName;
    GetFullPathNameA(path, 255, fPath, &fName);
    return (HBITMAP) LoadImage(gInstance, fPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

void renderBoardTexture(int x, int y, int textureNumber, HDC handler) {
    HDC handlerMemory;
    BITMAP textureBitmap;
    HGDIOBJ oldBitmap;
    handlerMemory = CreateCompatibleDC(handler);
    oldBitmap = SelectObject(handlerMemory, boardTextures[textureNumber]);
    GetObject(boardTextures[textureNumber], sizeof(textureBitmap), &textureBitmap);
    BitBlt(handler, x, y, textureBitmap.bmWidth, textureBitmap.bmHeight, handlerMemory, 0, 0, SRCCOPY);
    SelectObject(handlerMemory, oldBitmap);
    DeleteDC(handlerMemory);
}

void renderBoardBorder(int x, int y, HDC handler) {
    HDC handlerMemory;
    BITMAP textureBitmap;
    HGDIOBJ oldBitmap;
    handlerMemory = CreateCompatibleDC(handler);
    oldBitmap = SelectObject(handlerMemory, boardBorder);
    GetObject(boardBorder, sizeof(textureBitmap), &textureBitmap);
    BitBlt(handler, x, y, textureBitmap.bmWidth, textureBitmap.bmHeight, handlerMemory, 0, 0, SRCCOPY);
    SelectObject(handlerMemory, oldBitmap);
    DeleteDC(handlerMemory);
}

void renderBoard(Board* board, Color playerSide, HDC handler, Coordinates cursor, int x = 0, int y = 0) {
    //renderBoardBorder(x, y, handler);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            short bStat = board->boardRender[j][i]; short selectShift = 0;
            if (isCoordinatesEqual(cursorToBoardCoord(cursor, playerSide), {i, j})) selectShift = 11;
            if (playerSide == Black) {
                renderBoardTexture(33 + x + 56 * (7 - i), 33 + y + 56 * j, bStat + selectShift, handler); //56 x 56
            }
            else if (playerSide == White) {
                renderBoardTexture(33 + x + 56 * i, 33 + y + 56 * (7 - j), bStat + selectShift, handler);
            }
        }
    }
}

void renderEmptyBoard(HDC handler, int x, int y) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((i + j) % 2 == 0) {
                renderBoardTexture(33 + x + 56 * (7 - i), 33 + y + 56 * j, 0, handler);
            }
            else {
                renderBoardTexture(33 + x + 56 * (7 - i), 33 + y + 56 * j, 1, handler);
            }
        }
    }
}

void renderPathMapMarkers(Board* board, Color playerSide, HDC handler, Coordinates cursor, int x = 0, int y = 0) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            //Coordinates renderPos = getPasteCoords(i, j, playerSide);
            PathMapMarkers marker = board->pathMap[j][i]; short selectShift = 0;
            if (marker != Source && marker != NoMove && marker != MovingLock) {
                if (isCoordinatesEqual(cursorToBoardCoord(cursor, playerSide), {i, j})) selectShift = 11;
                if (playerSide == Black) {
                    renderBoardTexture(33 + x + 56 * (7 - i), 33 + y + 56 * j, 4 + marker + selectShift, handler); //56 x 56
                } else if (playerSide == White) {
                    renderBoardTexture(33 + x + 56 * i, 33 + y + 56 * (7 - j), 4 + marker + selectShift, handler);
                }
            }
        }
    }
}

void makeARegMoveSequenceWithDelay(GameSituation* situation, RegMoveSequence regMoveSequence, int mSecDelay, HDC handler) {
    for (int i = 0; i < regMoveSequence.rmsCount; i++) {
        makeAMove(situation, regMoveSequence.regularMoves[i]);
        updateBoardRender(&(situation->board));
        renderBoard(&(situation->board), player, handler, {-1, -1}, boardPasteX, boardPasteY);
        Sleep(mSecDelay);
    }
}

void makeATakingSequenceWithDelay(GameSituation* situation, TakingSequence takingSequence, int mSecDelay, HDC handler) {
    for (int i = 0; i < takingSequence.tmsCount; i++) {
        makeATakingMove(situation, takingSequence.takingMoves[i]);
        removeMarkedForDeath(situation, player);
        updateBoardRender(&(situation->board));
        renderBoard(&(situation->board), player, handler, {-1, -1}, boardPasteX, boardPasteY);
        Sleep(mSecDelay);
    }
}
/*
void makeAMixedSequenceWithDelay(GameSituation* situation, MixedSequence mixedSequence, int mSecDelay, HDC handler) {
    makeAMove(situation, mixedSequence.kingBecomingMove);
    updateBoardRender(&(situation->board));
    renderBoard(&(situation->board), player, handler, {-1, -1}, boardPasteX, boardPasteY);
    Sleep(mSecDelay);
    makeATakingSequenceWithDelay(situation, mixedSequence.takingSequence, mSecDelay, handler);
}
*/

#endif //CHECKERS_GRAPHIC_SUBSYSTEM_H
