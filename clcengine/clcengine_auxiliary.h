//
// Created by EgrZver on 11.05.2023.
//

#include "clcengine_types.h"

#ifndef CHECKERS_CLCENGINE_AUXILIARY_H
#define CHECKERS_CLCENGINE_AUXILIARY_H

bool isCoordinatesEqual(Coordinates arg1, Coordinates arg2) {
    return arg1.x == arg2.x && arg1.y == arg2.y;
}

void appendToATakingSequence(TakingSequence* sequence, TakingMove move) {
    sequence->takingMoves[sequence->tmsCount++] = move;
}

void deleteLastFromATakingSequence(TakingSequence* sequence) {
    sequence->tmsCount--;
}

void copyToAnotherTakingSequence(TakingSequence* source, TakingSequence* destination) {
    for (int i = 0; i < source->tmsCount; i++) {
        destination->takingMoves[i] = source->takingMoves[i];
    }
    destination->tmsCount = source->tmsCount;
}

float getVictoryEvalFor(Color side) {
    if (side == Black) return BLACK_VICTORY; else return WHITE_VICTORY;
}

bool lostByMoves(GameSituation* situation) {
    return (situation->tmsCount + situation->rmsCount + situation->mmsCount) == 0;
}

void copyLevelOneMovesToBuffers(GameSituation* situation) {
    for (int i = 0; i < situation->rmsCount; i++) {
        situation->regularMovesBuffer[situation->rmBufferLen++] = situation->regMoveSequences[i].regularMoves[0];
    }
    /*for (int i = 0; i < situation->mmsCount; i++) {
        situation->regularMovesBuffer[situation->rmBufferLen++] = situation->mixedSequences[i].kingBecomingMove;
    }*/
    for (int i = 0; i < situation->tmsCount; i++) {
        situation->takingMovesBuffer[situation->tmBufferLen++] = situation->takingSequences[i].takingMoves[0];
    }
}

void flushSequenceLists(GameSituation* situation) {
    situation->rmsCount = 0;
    situation->tmsCount = 0;
    situation->mmsCount = 0;
}

bool replaceConditionFor(float nEval, float oEval, Color side) {
    if (side == White) return (nEval < oEval); else return (nEval > oEval);
}

float evalQuality(GameSituation* situation) {
    float eval = 0;
    for (int i = 0; i < situation->board.checkersCount[White]; i++) {
        if (situation->board.checkers[White][i].type == King)  {
            eval -= kingCheckerCost;
        }
        else {
            eval -= regularCheckerCost * qualityFactor[7 - situation->board.checkers[White][i].coordinates.y][7 - situation->board.checkers[White][i].coordinates.x];
        }
    }
    for (int i = 0; i < situation->board.checkersCount[Black]; i++) {
        if (situation->board.checkers[Black][i].type == King)  {
            eval += kingCheckerCost;
        }
        else {
            eval += regularCheckerCost * qualityFactor[situation->board.checkers[Black][i].coordinates.y][situation->board.checkers[Black][i].coordinates.x];
        }

    }
    return eval;
}

PathMapMarkers getVictimMarker(Color victimColor, CheckerType victimType) {
    if (victimColor == Black) {
        if (victimType == Regular) {
            return VictimRB;
        }
        else {
            return VictimKB;
        }
    }
    else {
        if (victimType == Regular) {
            return VictimRW;
        }
        else {
            return VictimKW;
        }
    }
}

void updateBoardRender(Board* board) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board->boardRender[j][i] = ((i + j) % 2 == 0) ? EMPTY_BLACK : EMPTY_WHITE;
        }
    }
    for (int i = 0; i < board->checkersCount[Black]; i++) {
        int rcx = board->checkers[Black][i].coordinates.x, rcy = board->checkers[Black][i].coordinates.y;
        board->boardRender[rcy][rcx] = (board->checkers[Black][i].type == King) ? KING_BLACK : REG_BLACK;
    }
    for (int i = 0; i < board->checkersCount[White]; i++) {
        int rcx = board->checkers[White][i].coordinates.x, rcy = board->checkers[White][i].coordinates.y;
        board->boardRender[rcy][rcx] = (board->checkers[White][i].type == King) ? KING_WHITE : REG_WHITE;
    }
}

void fillPathMap(GameSituation* situation, Coordinates source) {
    if (situation->tmBufferLen != 0) {
        for (int i = 0; i < situation->tmBufferLen; i++) {
            if (isCoordinatesEqual(situation->takingMovesBuffer[i].source, source)) {
                Coordinates dest = situation->takingMovesBuffer[i].destination,
                    victim = situation->takingMovesBuffer[i].victim;
                situation->board.pathMap[dest.y][dest.x] = Destination;
                situation->board.pathMap[victim.y][victim.x] = getVictimMarker(situation->takingMovesBuffer[i].takingSide, situation->takingMovesBuffer[i].victimType);
            }
        }
    }
    else {
        for (int i = 0; i < situation->rmBufferLen; i++) {
            if (isCoordinatesEqual(situation->regularMovesBuffer[i].source, source)) {
                Coordinates dest = situation->regularMovesBuffer[i].destination;
                situation->board.pathMap[dest.y][dest.x] = Destination;
            }
        }
    }
}

int getCheckerIndexByCoordsAndColor(GameSituation* situation, int cx, int cy, Color color) {
    for (int i = 0; i < situation->board.checkersCount[color]; i++) {
        if (situation->board.checkers[color][i].coordinates.x == cx && situation->board.checkers[color][i].coordinates.y == cy) {
            return i;
        }
    }
}

Color sideColor(GameSituation* situation, int cx, int cy) {
    BoardCellState tInfo = situation->board.boardRender[cy][cx];
    if (tInfo == REG_WHITE || tInfo == KING_WHITE) {
        return White;
    }
    else if (tInfo == REG_BLACK || tInfo == KING_BLACK) {
        return Black;
    }
}

Color negateColor(Color color) {
    if (color == White) return Black;
    else if (color == Black) return White;
}

CheckerType getCheckerTypeOnBoard(GameSituation* situation, int cx, int cy) {
    BoardCellState tInfo = situation->board.boardRender[cy][cx];
    if (tInfo == REG_WHITE || tInfo == REG_BLACK) {
        return Regular;
    }
    else if (tInfo == KING_BLACK || tInfo == KING_WHITE) {
        return King;
    }
}

bool isAFriend(GameSituation* situation, Color forWhichSide, int tx, int ty) {
    BoardCellState tInfo = situation->board.boardRender[ty][tx];
    if (forWhichSide == White) {
        return tInfo == REG_WHITE || tInfo == KING_WHITE;
    }
    else if (forWhichSide == Black) {
        return tInfo == REG_BLACK || tInfo == KING_BLACK;
    }
}

bool isMarkedForDeath(GameSituation* situation, Color checkerColor, int mx, int my) {
    if (situation->board.boardRender[my][mx] == EMPTY_BLACK) return false;
    int index = getCheckerIndexByCoordsAndColor(situation, mx, my, checkerColor);
    return situation->board.checkers[checkerColor][index].markedForDeath;
}

bool isAVictim(GameSituation* situation, Color forWhichSide, int tx, int ty) {
    BoardCellState tInfo = situation->board.boardRender[ty][tx];
    if (forWhichSide == Black) {
        return (tInfo == REG_WHITE || tInfo == KING_WHITE);
    } else if (forWhichSide == White) {
        return (tInfo == REG_BLACK || tInfo == KING_BLACK);
    } else return false;
}

bool isNotOnKBBorder(int y, Color side) {
    return (side == White) ? y < 6 : y > 1;
}

bool longMoveLookingFlag(int x, int y, Coordinates direction) {
    if (direction.x == NORTH_EAST.x && direction.y == NORTH_EAST.y) {
        return !(x >= 7 || y >= 7);
    }
    else if (direction.x == NORTH_WEST.x && direction.y == NORTH_WEST.y) {
        return !(x <= 0 || y >= 7);
    }
    else if (direction.x == SOUTH_EAST.x && direction.y == SOUTH_EAST.y) {
        return !(x >= 7 || y <= 0);
    }
    else if (direction.x == SOUTH_WEST.x && direction.y == SOUTH_WEST.y) {
        return !(x <= 0 || y <= 0);
    }
}

bool regMoveLookingFlag(int x, int y, Coordinates direction) {
    if (direction.x == NORTH_EAST.x) {
        return x < 7;
    }
    else if (direction.x == NORTH_WEST.x) {
        return x > 0;
    }
}

int findDistanceToVictim(GameSituation* situation, int x, int y, Color forWhichSide, Coordinates direction) {
    int shift = 1;
    while (longMoveLookingFlag(x + shift*direction.x, y + shift * direction.y, direction)) {
        if (isAFriend(situation, forWhichSide, x + shift*direction.x, y + shift * direction.y) || isMarkedForDeath(situation, negateColor(forWhichSide), x + shift*direction.x, y + shift * direction.y)) return -1;
        else if (isAFriend(situation, forWhichSide, x + shift*direction.x, y + shift * direction.y)) return -1;
        else if (isAVictim(situation, forWhichSide, x + shift * direction.x, y + shift * direction.y)) {
            return shift;
        }
        else if (situation->board.boardRender[y + shift * direction.y][x + shift*direction.x] == EMPTY_BLACK) shift++;
    }
    return -1;
}

inline void flushMoveBuffers(GameSituation* situation) {
    situation->tmBufferLen = 0;
    situation->rmBufferLen = 0;
}




void removeChecker(Board* board, int index, Color color) {
    if (index < board->checkersCount[color]) {
        for (int i = index + 1; i < board->checkersCount[color];i++) {
            board->checkers[color][i - 1] = board->checkers[color][i];
        }
        board->checkersCount[color]--;
    }
}

void descendChecker(Checker* checker) {
    checker->type = Regular;
}

void ascendChecker(Checker* checker) {
    checker->type = King;
}

void removeMarkedForDeath(GameSituation* situation, Color inWhere) {
    for (int i = 0; i < situation->board.checkersCount[inWhere]; i++) {
        if (situation->board.checkers[inWhere][i].markedForDeath) {
            removeChecker(&(situation->board), i, inWhere);
            i--;
        }
    }
    updateBoardRender(&(situation->board));
}

#endif //CHECKERS_CLCENGINE_AUXILIARY_H
