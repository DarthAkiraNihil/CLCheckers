//
// Created by EgrZver on 20.03.2023.
//

#include "clcengine_types.h"

#ifndef CHECKERS_CLCHECKERS_H
#define CHECKERS_CLCHECKERS_H



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

TakingSequence getNullPath() {
    TakingSequence path;
    path.tmsCount = 0;
    return path;
}

void initiateChecker(Checker* checker, int x, int y, Color color) {
    checker->coordinates.x = x;
    checker->coordinates.y = y;
    checker->color = color;
    checker->type = Regular;
    checker->markedForDeath = false;
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

Board createANewBoard() {
    Board newBoard;
    newBoard.checkersCount[Black] = 12; newBoard.checkersCount[White] = 12;
    int valueOfInitedPairs = 0;
    for (short j = 0; j < 3; j++) {
        for (short k = 0; k < 8; k += 2) {
            initiateChecker(&newBoard.checkers[White][valueOfInitedPairs], k + (j % 2), j, White);
            initiateChecker(&newBoard.checkers[Black][valueOfInitedPairs++], k + 1 - (j % 2), 7 - j, Black);
        }
    }
    updateBoardRender(&newBoard);
    return newBoard;
}

GameSituation makeNullGameSituation(Color playerSide) {
    GameSituation gameSituation;
    gameSituation.board = createANewBoard();
    gameSituation.playerSide = playerSide;
    gameSituation.rmBufferLen = 0;
    gameSituation.tmBufferLen = 0;
    gameSituation.tmsCount = 0;
    gameSituation.rmsCount = 0;
    gameSituation.mmsCount = 0;
    for (int i = 0; i < 16; i++) gameSituation.takingSequences[i].tmsCount = 0;
    //Color turnOf;
    return gameSituation;
}

Game createANewGame(Color playerSide, Color firstMove, GameType type) {
    Game newGame;
    newGame.situation = makeNullGameSituation(playerSide);
    newGame.state = (playerSide == firstMove) ? PlayerMove : ((type == RvsR) ? EnemyMoveReal : EnemyMoveComputer);
    return newGame;
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

void findAllKingTakingMovesForOne(GameSituation* situation, Color checkerColor, int checkerIndex) {
    if (situation->board.checkers[checkerColor][checkerIndex].type == King) {
        findAllKingTakingMovesForOneOnDir(situation, checkerColor, checkerIndex, NORTH_EAST);
        findAllKingTakingMovesForOneOnDir(situation, checkerColor, checkerIndex, NORTH_WEST);
        findAllKingTakingMovesForOneOnDir(situation, checkerColor, checkerIndex, SOUTH_EAST);
        findAllKingTakingMovesForOneOnDir(situation, checkerColor, checkerIndex, SOUTH_WEST);
    }
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

int makeAMove(GameSituation* situation, Move move) {
    Color movedColor = sideColor(situation, move.source.x, move.source.y);
    int movedIndex = getCheckerIndexByCoordsAndColor(situation, move.source.x, move.source.y, movedColor);
    situation->board.checkers[movedColor][movedIndex].coordinates = {move.destination.x, move.destination.y};
    if (move.isKingBecomingMove) ascendChecker(&(situation->board.checkers[movedColor][movedIndex]));
    updateBoardRender(&(situation->board));
    flushMoveBuffers(situation);
    if (move.isKingBecomingMove) {
        findAllKingTakingMovesForOne(situation, movedColor, movedIndex);
        if (situation->tmBufferLen == 0) {
            findAllKingMovesForOne(situation, movedColor, movedIndex);
            return 2;
        }
        return 1;
    } else return 0;
}

int makeATakingMove(GameSituation* situation, TakingMove move) {
    Color movedColor = negateColor(move.takingSide);
    int movedIndex = getCheckerIndexByCoordsAndColor(situation, move.source.x, move.source.y, movedColor);
    Color victimColor = move.takingSide;
    int victimIndex = getCheckerIndexByCoordsAndColor(situation, move.victim.x, move.victim.y, victimColor);
    situation->board.checkers[movedColor][movedIndex].coordinates = {move.destination.x, move.destination.y};
    if (move.isASpecialMove) ascendChecker(&(situation->board.checkers[movedColor][movedIndex]));
    situation->board.checkers[victimColor][victimIndex].markedForDeath = true;
    flushMoveBuffers(situation);
    updateBoardRender(&(situation->board));
    if (situation->board.checkers[movedColor][movedIndex].type == King) {
        findAllKingTakingMovesForOne(situation, movedColor, movedIndex);
    }
    else {
        findAllTakingMovesForOne(situation, movedColor, movedIndex);
    }
    return situation->tmBufferLen;
}



int cancelAMove(GameSituation* situation, Move move) {
    Color movedColor = sideColor(situation, move.destination.x, move.destination.y);
    int movedIndex = getCheckerIndexByCoordsAndColor(situation, move.destination.x, move.destination.y, movedColor);
    situation->board.checkers[movedColor][movedIndex].coordinates = {move.source.x, move.source.y};
    if (move.isKingBecomingMove) descendChecker(&(situation->board.checkers[movedColor][movedIndex]));
    updateBoardRender(&(situation->board));
    if (move.isKingBecomingMove) return -1; else return 0;
}

int cancelATakingMove(GameSituation* situation, TakingMove move, bool emulated = false) {
    Color movedColor = negateColor(move.takingSide);
    int movedIndex = getCheckerIndexByCoordsAndColor(situation, move.destination.x, move.destination.y, movedColor);
    situation->board.checkers[movedColor][movedIndex].coordinates = {move.source.x, move.source.y};
    if (move.isASpecialMove) descendChecker(&(situation->board.checkers[movedColor][movedIndex]));
    if (!emulated) {
        Checker regeneratedVictim;
        regeneratedVictim.coordinates = {move.victim.x, move.victim.y};
        regeneratedVictim.type = move.victimType;
        regeneratedVictim.color = move.takingSide;
        regeneratedVictim.markedForDeath = false;
        situation->board.checkers[move.takingSide][situation->board.checkersCount[move.takingSide]++] = regeneratedVictim;
    }
    else {
        int falseVictimIndex = getCheckerIndexByCoordsAndColor(situation, move.victim.x, move.victim.y, move.takingSide);
        situation->board.checkers[move.takingSide][falseVictimIndex].markedForDeath = false;
    }
    updateBoardRender(&(situation->board));
    if (move.isASpecialMove) return -1; else return 0;
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

void findRegularMoveSequenceForOne(GameSituation* situation, Color checkerColor, int checkerIndex) {
    findAllKBMovesForOne(situation, checkerColor, checkerIndex);
    findAllRegularMovesForOne(situation, checkerColor, checkerIndex);
    findAllKingMovesForOne(situation, checkerColor, checkerIndex);

    if (situation->rmBufferLen != 0) {
        Move* buffer = new Move[situation->rmBufferLen];
        for (int i = 0; i < situation->rmBufferLen; i++) buffer[i] = situation->regularMovesBuffer[i];
        int savedRMs = situation->rmBufferLen;
        for (int i = 0; i < savedRMs; i++) {
            Move extracted = buffer[i];
            if (extracted.isKingBecomingMove) {
                flushMoveBuffers(situation);
                int insertIndex;
                makeAMove(situation, extracted);
                if (situation->tmBufferLen == 0 && situation->rmBufferLen != 0) {
                    for (int j = 0; j < situation->rmBufferLen; j++) {
                        insertIndex = situation->rmsCount;
                        situation->regMoveSequences[insertIndex].regularMoves[0] = extracted;
                        situation->regMoveSequences[insertIndex].regularMoves[1] = situation->regularMovesBuffer[j];
                        situation->regMoveSequences[insertIndex].rmsCount = 2;
                        situation->rmsCount++;
                    }
                }
                cancelAMove(situation, extracted);
            }
            else {
                int insertIndex = situation->rmsCount;
                situation->regMoveSequences[insertIndex].regularMoves[0] = extracted; // maybe 0
                situation->regMoveSequences[insertIndex].rmsCount = 1;
                situation->rmsCount++;
            }
        }
        delete [] buffer;
    }
}

void findAllRegularMoveSequences(GameSituation* situation, Color forWhichSide) {
    for (int i = 0; i < situation->board.checkersCount[forWhichSide]; i++) {
        findRegularMoveSequenceForOne(situation, forWhichSide, i);
        flushMoveBuffers(situation);
    }
}

void makeARegMoveSequence(GameSituation* situation, RegMoveSequence regMoveSequence) {
    for (int i = 0; i < regMoveSequence.rmsCount; i++) makeAMove(situation, regMoveSequence.regularMoves[i]);
}

void cancelARegMoveSequence(GameSituation* situation, RegMoveSequence regMoveSequence) {
    for (int i = regMoveSequence.rmsCount - 1; i > -1; i--) cancelAMove(situation, regMoveSequence.regularMoves[i]);
}

void findAllTakingSequencesForOne(GameSituation* situation, Color checkerColor, int checkerIndex, TakingSequence* currentPath, bool afterMove = false) {
    if (!afterMove) {
        findAllKingTakingMovesForOne(situation, checkerColor, checkerIndex);
        findAllTakingMovesForOne(situation, checkerColor, checkerIndex);
    }
    TakingMove* buffer = new TakingMove[situation->tmBufferLen];
    for (int i = 0; i < situation->tmBufferLen; i++) buffer[i] = situation->takingMovesBuffer[i];
    int savedTMs = situation->tmBufferLen;
    for (int i = 0; i < savedTMs; i++) {
        TakingMove extracted = buffer[i];
        appendToATakingSequence(currentPath, extracted);
        int stat = makeATakingMove(situation, extracted);
        if (stat == 0) {
            copyToAnotherTakingSequence(currentPath, &(situation->takingSequences[situation->tmsCount++]));
        }
        else {
            findAllTakingSequencesForOne(situation, checkerColor, checkerIndex, currentPath, true);
        }
        cancelATakingMove(situation, extracted, ENABLED_EMULATION);
        deleteLastFromATakingSequence(currentPath);
    }
    delete [] buffer;
}



void makeATakingMoveSequence(GameSituation* situation, TakingSequence takingSequence) {
    for (int i = 0; i < takingSequence.tmsCount; i++) makeATakingMove(situation, takingSequence.takingMoves[i]);
}

void cancelATakingMoveSequence(GameSituation* situation, TakingSequence takingSequence) {
    for (int i = takingSequence.tmsCount - 1; i > -1; i--) cancelATakingMove(situation, takingSequence.takingMoves[i]);
}

void findAllTakingMoveSequences(GameSituation* situation, Color forWhichSide) {
    for (int i = 0; i < situation->board.checkersCount[forWhichSide]; i++) {
        TakingSequence path = getNullPath();
        findAllTakingSequencesForOne(situation, forWhichSide, i, &path);
        flushMoveBuffers(situation);
    }
}

void findAllTakingSequencesForOne(GameSituation* situation, Color checkerColor, int checkerIndex, TakingSequence* currentPath, Move kingBecomingMoveToInsert, bool afterMove = false) {
    if (!afterMove) {
        findAllKingTakingMovesForOne(situation, checkerColor, checkerIndex);
        findAllTakingMovesForOne(situation, checkerColor, checkerIndex);
    }
    TakingMove* buffer = new TakingMove[situation->tmBufferLen];
    for (int i = 0; i < situation->tmBufferLen; i++) buffer[i] = situation->takingMovesBuffer[i];
    int savedTMs = situation->tmBufferLen;
    for (int i = 0; i < savedTMs; i++) {
        TakingMove extracted = buffer[i];
        appendToATakingSequence(currentPath, extracted);
        int stat = makeATakingMove(situation, extracted);
        if (stat == 0) {
            situation->mixedSequences[situation->mmsCount].kingBecomingMove = kingBecomingMoveToInsert;
            copyToAnotherTakingSequence(currentPath, &(situation->mixedSequences[situation->mmsCount++].takingSequence));
        }
        else {
            findAllTakingSequencesForOne(situation, checkerColor, checkerIndex, currentPath, true);
        }
        cancelATakingMove(situation, extracted, ENABLED_EMULATION);
        deleteLastFromATakingSequence(currentPath);
    }
    delete [] buffer;
}

void findAllMixedSequencesForOne(GameSituation* situation, Color checkerColor, int checkerIndex) {
    findAllKBMovesForOne(situation, checkerColor, checkerIndex);
    int savedTMs = 0;
    if (situation->rmBufferLen != 0) {
        Move* buffer = new Move[situation->rmBufferLen];
        for (int i = 0; i < situation->rmBufferLen; i++) buffer[i] = situation->regularMovesBuffer[i];
        int savedRMs = situation->rmBufferLen;
        for (int i = 0; i < savedRMs; i++) {
            Move extracted = buffer[i];
            situation->mixedSequences[situation->mmsCount].kingBecomingMove = extracted;
            if (extracted.isKingBecomingMove) {
                flushMoveBuffers(situation);
                makeAMove(situation, extracted);
                if (situation->tmBufferLen != 0) {
                    savedTMs = situation->tmBufferLen;
                    flushMoveBuffers(situation);
                    TakingSequence null = getNullPath();
                    findAllTakingSequencesForOne(situation, checkerColor, checkerIndex, &null, extracted);
                }
                cancelAMove(situation, extracted);
            }
        }
        delete [] buffer;
    }
}

void findAllMixedSequences(GameSituation* situation, Color forWhichSide) {
    for (int i = 0; i < situation->board.checkersCount[forWhichSide]; i++) {
        findAllMixedSequencesForOne(situation, forWhichSide, i);
        flushMoveBuffers(situation);
    }
}

void makeAMixedSequence(GameSituation* situation, MixedSequence mixedSequence) {
    makeAMove(situation, mixedSequence.kingBecomingMove);
    makeATakingMoveSequence(situation, mixedSequence.takingSequence);
}

void cancelAMixedSequence(GameSituation* situation, MixedSequence mixedSequence) {
    cancelATakingMoveSequence(situation, mixedSequence.takingSequence);
    cancelAMove(situation, mixedSequence.kingBecomingMove);
}

inline void findAllMoves(GameSituation* situation, Color forWhichSide) {
    findAllTakingMoveSequences(situation, forWhichSide);
    if (situation->tmsCount == 0) {
        findAllMixedSequences(situation, forWhichSide);
        findAllRegularMoveSequences(situation, forWhichSide);
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

float getVictoryEvalFor(Color side) {
    if (side == Black) return BLACK_VICTORY; else return WHITE_VICTORY;
}

bool lostByMoves(GameSituation* situation) {
    return (situation->tmsCount + situation->rmsCount + situation->mmsCount) == 0;
}

SeqContainer getNullContainer(Color side) {
    SeqContainer container;
    container.eval = getVictoryEvalFor(negateColor(side));
    container.seqNumberToDo = -1;
    container.takingSequence.tmsCount = 0;
    container.regMoveSequence.rmsCount = 0;
    container.mixedSequence.takingSequence.tmsCount = 0;
    return container;
}

SeqContainer analyze(GameSituation* situation, Color side, int currentDepth, Difficulty maxDepth) {
    SeqContainer toReturn = getNullContainer(side);
    float topBorder = getVictoryEvalFor(negateColor(side)), soFuckingDeepEval;//container->eval = getVictoryEvalFor(negateColor(side));
    findAllMoves(situation, side);
    if (lostByMoves(situation)) {
        toReturn.eval = topBorder;
        return toReturn;
    }
    if (currentDepth == maxDepth) {
        toReturn.eval = evalQuality(situation);
        return toReturn;
    }
    if (situation->tmsCount == 0) {
        MixedSequence* msBackup = new MixedSequence[situation->mmsCount];
        for (int i = 0; i < situation->mmsCount; i++) msBackup[i] = situation->mixedSequences[i];
        int savedMSS = situation->mmsCount;
        for (int i = 0; i < savedMSS; i++) {
            makeAMixedSequence(situation, msBackup[i]);
            removeMarkedForDeath(situation, negateColor(side));
            flushSequenceLists(situation);
            SeqContainer deepMoveAnalyzed = analyze(situation, negateColor(side), currentDepth + 1, maxDepth);
            cancelAMixedSequence(situation, msBackup[i]);
            if (replaceConditionFor(deepMoveAnalyzed.eval, topBorder, side)) {
                topBorder = deepMoveAnalyzed.eval;
                toReturn.eval = topBorder;
                toReturn.mixedSequence = msBackup[i];
                toReturn.seqNumberToDo = 3;
            }
        }
        delete [] msBackup;
        RegMoveSequence* rmsBackup = new RegMoveSequence[situation->rmsCount];
        for (int i = 0; i < situation->rmsCount; i++) rmsBackup[i] = situation->regMoveSequences[i];
        int savedRMSS = situation->rmsCount;
        for (int i = 0; i < savedRMSS; i++) {
            makeARegMoveSequence(situation, rmsBackup[i]);
            flushSequenceLists(situation);
            SeqContainer deepMoveAnalyzed = analyze(situation, negateColor(side), currentDepth + 1, maxDepth);
            cancelARegMoveSequence(situation, rmsBackup[i]);
            if (replaceConditionFor(deepMoveAnalyzed.eval, topBorder, side)) {
                topBorder = deepMoveAnalyzed.eval;
                toReturn.eval = topBorder;
                toReturn.regMoveSequence = rmsBackup[i];
                toReturn.seqNumberToDo = 1;
            }
        }
        delete [] rmsBackup;
        return toReturn;
    }
    else {
        TakingSequence* tsBackup = new TakingSequence[situation->tmsCount];
        for (int i = 0; i < situation->tmsCount; i++) tsBackup[i] = situation->takingSequences[i];
        int savedTSS = situation->tmsCount;
        for (int i = 0; i < savedTSS; i++) {
            makeATakingMoveSequence(situation, tsBackup[i]);
            removeMarkedForDeath(situation, negateColor(side));
            flushSequenceLists(situation);
            SeqContainer deepMoveAnalyzed = analyze(situation, negateColor(side), currentDepth + 1, maxDepth);
            cancelATakingMoveSequence(situation, tsBackup[i]);
            if (replaceConditionFor(deepMoveAnalyzed.eval, topBorder, side)) {
                topBorder = deepMoveAnalyzed.eval;
                toReturn.eval = topBorder;
                toReturn.takingSequence = tsBackup[i];
                toReturn.seqNumberToDo = 2;
            }
        }
        delete [] tsBackup;
        return toReturn;
    }
}

SeqContainer getBestMove(GameSituation situation, Color side, Difficulty depth) {
    GameSituation copyOfGS = situation;
    return analyze(&copyOfGS, side, 0, depth);
}

#endif CHECKERS_CLCHECKERS_H

