//
// Created by EgrZver on 11.05.2023.
//
#include "clcengine_types.h"
#include "clcengine_auxiliary.h"

#ifndef CHECKERS_CLCENGINE_MOVE_SEARCHING_H
#define CHECKERS_CLCENGINE_MOVE_SEARCHING_H

void findAllKingMovesForOneOnDir(GameSituation* situation, Color checkerColor, int checkerIndex, Coordinates directionShift) {
    Move move;
    move.isKingBecomingMove = false; move.isKingMove = false;
    int ex = situation->board.checkers[checkerColor][checkerIndex].coordinates.x;
    int ey = situation->board.checkers[checkerColor][checkerIndex].coordinates.y;
    move.source = {ex, ey};
    bool flag = longMoveLookingFlag(ex, ey, directionShift);
    int shift = 1;
    while (flag) {
        if (situation->board.boardRender[ey + shift * directionShift.y][ex + shift*directionShift.x] == EMPTY_BLACK) {
            move.destination = {ex + shift * directionShift.x, ey + shift * directionShift.y};
            situation->regularMovesBuffer[situation->rmBufferLen++] = move;
            flag = longMoveLookingFlag(ex + shift*directionShift.x, ey + shift * directionShift.y, directionShift);
        } else {
            flag = false;
        }
        shift++;
    }

}

void findAllKingMovesForOne(GameSituation* situation, Color checkerColor, int checkerIndex) {
    if (situation->board.checkers[checkerColor][checkerIndex].type == King) {
        findAllKingMovesForOneOnDir(situation, checkerColor, checkerIndex, NORTH_EAST);
        findAllKingMovesForOneOnDir(situation, checkerColor, checkerIndex, NORTH_WEST);
        findAllKingMovesForOneOnDir(situation, checkerColor, checkerIndex, SOUTH_EAST);
        findAllKingMovesForOneOnDir(situation, checkerColor, checkerIndex, SOUTH_WEST);
    }
}

void findAllRegularMovesForOneOnDir(GameSituation* situation, Color checkerColor, int checkerIndex, Coordinates direction) {
    Move move;
    move.isKingBecomingMove = false; move.isKingMove = false;
    int ex = situation->board.checkers[checkerColor][checkerIndex].coordinates.x;
    int ey = situation->board.checkers[checkerColor][checkerIndex].coordinates.y;
    move.source = {ex, ey};
    if (regMoveLookingFlag(ex, ey, direction) && isNotOnKBBorder(ey, checkerColor)) {
        BoardCellState tInfo = situation->board.boardRender[ey + direction.y][ex + direction.x];
        if (tInfo == EMPTY_BLACK) {
            move.destination = {ex + direction.x, ey + direction.y};
            situation->regularMovesBuffer[situation->rmBufferLen++] = move;
        }
    }
}

void findAllRegularMovesForOne(GameSituation* situation, Color checkerColor, int checkerIndex) {
    if (checkerColor == White) {
        findAllRegularMovesForOneOnDir(situation, checkerColor, checkerIndex, NORTH_EAST);
        findAllRegularMovesForOneOnDir(situation, checkerColor, checkerIndex, NORTH_WEST);
    }
    else {
        findAllRegularMovesForOneOnDir(situation, checkerColor, checkerIndex, SOUTH_EAST);
        findAllRegularMovesForOneOnDir(situation, checkerColor, checkerIndex, SOUTH_WEST);
    }
}

void findAllTakingMovesForOne(GameSituation* situation, Color checkerColor, int checkerIndex) {
    if (situation->board.checkers[checkerColor][checkerIndex].type != King) {
        TakingMove takingMove;
        int ex = situation->board.checkers[checkerColor][checkerIndex].coordinates.x;
        int ey = situation->board.checkers[checkerColor][checkerIndex].coordinates.y;
        takingMove.source = {ex, ey};
        if (ey < 6) {
            if (ex < 6) {
                if ((situation->board.boardRender[ey + 2][ex + 2] == EMPTY_BLACK) && isAVictim(situation, checkerColor, ex + 1, ey + 1) && (!isMarkedForDeath(situation,negateColor(checkerColor), ex + 1, ey + 1))/* && ((ex + 1 != vx) || (ey + 1 != vy))*/) {
                    takingMove.destination = {ex + 2, ey + 2};
                    takingMove.victim = {ex + 1, ey + 1};
                    takingMove.takingSide = negateColor(checkerColor);
                    takingMove.victimType = getCheckerTypeOnBoard(situation, ex + 1, ey + 1);
                    takingMove.isASpecialMove = (checkerColor == White && ey == 5);
                    situation->takingMovesBuffer[situation->tmBufferLen++] = takingMove;
                }
            }
            if (ex > 1) {
                if ((situation->board.boardRender[ey + 2][ex - 2] == EMPTY_BLACK) && isAVictim(situation, checkerColor, ex - 1, ey + 1) && !isMarkedForDeath(situation,negateColor(checkerColor), ex - 1, ey + 1)/* && ((ex - 1 != vx) || (ey + 1 != vy))*/) {
                    takingMove.destination = {ex - 2, ey + 2};
                    takingMove.victim = {ex - 1, ey + 1};
                    takingMove.takingSide = negateColor(checkerColor);
                    takingMove.victimType = getCheckerTypeOnBoard(situation, ex - 1, ey + 1);
                    takingMove.isASpecialMove = (checkerColor == White && ey == 5);
                    situation->takingMovesBuffer[situation->tmBufferLen++] = takingMove;
                }
            }
        }
        if (ey > 1) {
            if (ex < 6) {
                if ((situation->board.boardRender[ey - 2][ex + 2] == EMPTY_BLACK) && isAVictim(situation, checkerColor, ex + 1, ey - 1) && !isMarkedForDeath(situation,negateColor(checkerColor), ex + 1, ey - 1)/* && ((ex + 1 != vx) || (ey - 1 != vy))*/) {
                    takingMove.destination = {ex + 2, ey - 2};
                    takingMove.victim = {ex + 1, ey - 1};
                    takingMove.takingSide = negateColor(checkerColor);
                    takingMove.victimType = getCheckerTypeOnBoard(situation, ex + 1, ey - 1);
                    takingMove.isASpecialMove = (checkerColor == Black && ey == 2);
                    situation->takingMovesBuffer[situation->tmBufferLen++] = takingMove;
                }
            }
            if (ex > 1) {
                if ((situation->board.boardRender[ey - 2][ex - 2] == EMPTY_BLACK) && isAVictim(situation, checkerColor, ex - 1, ey - 1) && !isMarkedForDeath(situation,negateColor(checkerColor), ex - 1, ey - 1)/* && ((ex - 1 != vx) || (ey - 1 != vy))*/) {
                    takingMove.destination = {ex - 2, ey - 2};
                    takingMove.victim = {ex - 1, ey - 1};
                    takingMove.takingSide = negateColor(checkerColor);
                    takingMove.victimType = getCheckerTypeOnBoard(situation, ex - 1, ey - 1);
                    takingMove.isASpecialMove = (checkerColor == Black && ey == 2);
                    situation->takingMovesBuffer[situation->tmBufferLen++] = takingMove;
                }
            }
        }
    }
}

void findAllKingTakingMovesForOneOnDir(GameSituation* situation, Color checkerColor, int checkerIndex, Coordinates directionShift) {
    TakingMove takingMove;
    takingMove.isASpecialMove = false;
    takingMove.takingSide = negateColor(checkerColor);
    int ex = situation->board.checkers[checkerColor][checkerIndex].coordinates.x;
    int ey = situation->board.checkers[checkerColor][checkerIndex].coordinates.y;
    takingMove.source = {ex, ey};
    int victimDistance = findDistanceToVictim(situation, ex, ey, checkerColor, directionShift);
    if (victimDistance != -1) {
        takingMove.victim = {ex + victimDistance * directionShift.x, ey + victimDistance * directionShift.y};
        takingMove.victimType = getCheckerTypeOnBoard(situation, ex + victimDistance * directionShift.x, ey + victimDistance * directionShift.y);
        bool flag = longMoveLookingFlag(ex + victimDistance * directionShift.x, ey + victimDistance * directionShift.y, directionShift);
        int additionalShift = 1;
        while (flag) {
            if (situation->board.boardRender[ey + directionShift.y * (victimDistance + additionalShift)][ex + directionShift.x * (victimDistance + additionalShift)] == EMPTY_BLACK) {
                takingMove.destination = {ex + directionShift.x * (victimDistance + additionalShift), ey + directionShift.y * (victimDistance + additionalShift)};
                situation->takingMovesBuffer[situation->tmBufferLen++] = takingMove;
                flag = longMoveLookingFlag(ex + directionShift.x * (victimDistance + additionalShift), ey + directionShift.y * (victimDistance + additionalShift), directionShift);
            } else {
                flag = false;
            }
            additionalShift++;
        }
    }
}

void findAllKBMovesForOne(GameSituation* situation, Color checkerColor, int checkerIndex) {
    if (situation->board.checkers[checkerColor][checkerIndex].type != King) {
        Move move;
        move.isKingMove = false; move.isKingBecomingMove = true;
        int ex = situation->board.checkers[checkerColor][checkerIndex].coordinates.x;
        int ey = situation->board.checkers[checkerColor][checkerIndex].coordinates.y;
        move.source = {ex, ey};

        if (checkerColor == Black ? ey == 1 : ey == 6) {
            if (ex < 7) {
                if ((situation->board.boardRender[(checkerColor == Black ? ey - 1 : ey + 1)][ex+1] == EMPTY_BLACK)) {
                    move.destination = {ex + 1, checkerColor == Black ? ey - 1 : ey + 1};
                    situation->regularMovesBuffer[situation->rmBufferLen++] = move;
                }
            }
            if (ex > 0) {
                if ((situation->board.boardRender[(checkerColor == Black ? ey - 1 : ey + 1)][ex-1] == EMPTY_BLACK)) {
                    move.destination = {ex - 1, checkerColor == Black ? ey - 1 : ey + 1};
                    situation->regularMovesBuffer[situation->rmBufferLen++] = move;
                }
            }
        }
    }
}


void findAllKingTakingMovesForOne(GameSituation* situation, Color checkerColor, int checkerIndex) {
    if (situation->board.checkers[checkerColor][checkerIndex].type == King) {
        findAllKingTakingMovesForOneOnDir(situation, checkerColor, checkerIndex, NORTH_EAST);
        findAllKingTakingMovesForOneOnDir(situation, checkerColor, checkerIndex, NORTH_WEST);
        findAllKingTakingMovesForOneOnDir(situation, checkerColor, checkerIndex, SOUTH_EAST);
        findAllKingTakingMovesForOneOnDir(situation, checkerColor, checkerIndex, SOUTH_WEST);
    }
}

#endif //CHECKERS_CLCENGINE_MOVE_SEARCHING_H
