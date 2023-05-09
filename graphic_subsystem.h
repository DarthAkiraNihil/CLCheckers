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

inline Coordinates transformXYToBoardXY(int x, int y, Color relativeSide) {
    if (relativeSide == Black) {
        return {7 - (x - 33 - boardPasteX) / 56, (y - 33 - boardPasteY) / 56};
    }
    else {
        return { (x - 33 - boardPasteX) / 56, 7 - (y - 33 - boardPasteY) / 56};
    }
}

inline Coordinates getPasteCoords(int x, int y, Color playerSide) {
    if (playerSide == Black) {
        return {33 + boardPasteX + 56 * (7 - x), 33 + boardPasteY + 56 * y};
    }
    else {
        return {33 + boardPasteX + 56 * x, 33 + boardPasteY + 56 * (7 - y)};
    }
}

inline Coordinates getNearestCorner(int x, int y) {
    return {((x - 33 - boardPasteX) / 56) * 56 + 33 + boardPasteX, ((y - 33 - boardPasteY) / 56) * 56 + 33 + boardPasteY};
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

void renderRegularMoveSequence(RegMoveSequence sequence, int moveNumber, HDC handler) {
    Coordinates renderPlace = getPasteCoords(sequence.regularMoves[moveNumber].destination.x, sequence.regularMoves[moveNumber].destination.y, player);
    renderBoardTexture(renderPlace.x, renderPlace.y, 6, handler);
}

void renderTakingSequence(TakingSequence sequence, int moveNumber, HDC handler) {
    Coordinates renderPlaceDest = getPasteCoords(sequence.takingMoves[moveNumber].destination.x, sequence.takingMoves[moveNumber].destination.y, player);
    Coordinates renderPlaceVictim = getPasteCoords(sequence.takingMoves[moveNumber].victim.x, sequence.takingMoves[moveNumber].victim.y, player);
    renderBoardTexture(renderPlaceDest.x, renderPlaceDest.y, 6, handler);
    renderBoardTexture(renderPlaceVictim.x, renderPlaceVictim.y, 7 + 2*sequence.takingMoves[moveNumber].victimType +  sequence.takingMoves[moveNumber].takingSide, handler);
}

void renderKBMove(Move kingBecomingMove, HDC handler) {
    Coordinates renderPlace = getPasteCoords(kingBecomingMove.destination.x, kingBecomingMove.destination.y, player);
    renderBoardTexture(renderPlace.x, renderPlace.y, 6, handler);
}
//6
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

/*
 * removeMarkedForDeath(&game.situation, player);

                    flushSequenceLists(&game.situation);
                    moveHasBeenMade = false;

 */

void makeATakingSequenceWithDelay(GameSituation* situation, TakingSequence takingSequence, int mSecDelay, HDC handler) {
    for (int i = 0; i < takingSequence.tmsCount; i++) {
        makeATakingMove(situation, takingSequence.takingMoves[i]);
        removeMarkedForDeath(situation, player);
        updateBoardRender(&(situation->board));
        renderBoard(&(situation->board), player, handler, {-1, -1}, boardPasteX, boardPasteY);
        Sleep(mSecDelay);
    }
}

void makeAMixedSequenceWithDelay(GameSituation* situation, MixedSequence mixedSequence, int mSecDelay, HDC handler) {
    makeAMove(situation, mixedSequence.kingBecomingMove);
    updateBoardRender(&(situation->board));
    renderBoard(&(situation->board), player, handler, {-1, -1}, boardPasteX, boardPasteY);
    Sleep(mSecDelay);
    makeATakingSequenceWithDelay(situation, mixedSequence.takingSequence, mSecDelay, handler);
}

void renderBoard2(Board* board, Color playerSide, short drawX, short drawY, bool pasteCoordinates) {
    drawFrame(10, 10, drawX, drawY);
    if (pasteCoordinates) {
        gotoxy(drawX + 1, drawY - 1); printf("abcdefgh");
        gotoxy(drawX + 1, drawY + 10); printf("abcdefgh");
        for (int i = 0; i < 8; i++) {
            gotoxy(drawX - 1, drawY + 1 + i);
            printf("%d", i + 1);
            gotoxy(drawX + 10, drawY + 1 + i);
            printf("%d", i + 1);
        }
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            short bStat = board->boardRender[j][i];
            if (playerSide == Black) {
                gotoxy(drawX + 8 - i, drawY + 1 + j);
            }
            else if (playerSide == White) {
                gotoxy(drawX + 1 + i, drawY + 8 - j);
            }
            switch(bStat) {
                case EMPTY_BLACK: {
                    textbackground(0);
                    printf(" ");
                    break;
                }
                case EMPTY_WHITE: {
                    textbackground(15);
                    printf(" ");
                    break;
                }
                case REG_BLACK: {
                    textcolor(12);
                    printf("O");
                    break;
                }
                case REG_WHITE: {
                    textcolor(15);
                    printf("O");
                    break;
                }
                case KING_BLACK: {
                    textcolor(12);
                    printf("W");
                    break;
                }
                case KING_WHITE: {
                    textcolor(15);
                    printf("W");
                    break;
                }

            }
            textbackground(0);
        }
    }
    textbackground(0);
    textcolor(15);
}

#endif //CHECKERS_GRAPHIC_SUBSYSTEM_H
