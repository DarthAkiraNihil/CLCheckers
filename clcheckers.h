//
// Created by EgrZver on 20.03.2023.
//

#ifndef CHECKERS_CLCHECKERS_H
#define CHECKERS_CLCHECKERS_H

enum BoardCellState {EMPTY_BLACK, EMPTY_WHITE, REG_BLACK, REG_WHITE, KING_BLACK, KING_WHITE};
enum Color {Black, White};
enum CheckerType {Regular, King};
enum GameState {PlayerMove, EnemyMoveReal, EnemyMoveComputer, BlackWon, WhiteWon};
enum GameType {RvsR, RvsC}; //RvsR = Real versus Real, RvsC = Real vs Computer
enum Difficulty {Easy = 2, Normal = 3, Hard = 4, Insane = 5, Extreme = 6, Diabolic = 8};
// may delete last two


const int regularCheckerCost = 100;
const int kingCheckerCost = 250;

const int qualityFactor[8][8] = {
        {1200, 1200, 1200, 1200, 1200, 1200, 1200, 1200},
        {1150, 1150, 1150, 1150, 1150, 1150, 1150, 1150},
        {1150, 1150, 1150, 1150, 1150, 1150, 1150, 1150},
        {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000},
        {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000},
        {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000},
        {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000},
        {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000}
    };

struct Coordinates {
    int x, y;
};

struct Checker {
    Coordinates coordinates;
    Color color;
    CheckerType type;
    bool markedForDeath;
};

struct Move {
    Coordinates source, destination;
    bool isKingMove, isKingBecomingMove;
    // tod remove king/taking types
};

struct TakingMove {
    Coordinates source, destination, victim;
    Color takingSide;
    CheckerType victimType;
    bool isASpecialMove;
};

struct RegMoveSequence {
    Move regularMoves[2];
    int rmsCount;
};

struct TakingSequence {
    TakingMove takingMoves[16];
    int tmsCount;
};

struct Board {
    BoardCellState boardRender[8][8];
    Checker checkers[2][12];
    int checkersCount[2];
};

struct GameSituation {
    Board board;
    //int rmCount, krmCount, kbmCount, rtmCount, ktmCount;
    int rmCount, tmCount;
    Color playerSide;
    //Move regularMoves[32], kingRegularMoves[64], kingBecomingMoves[16];
    Move regularMoves[128];
    Move lastKingBecomingMove;
    //Move regularMoves[128], kingBecomingMoves[16];
    //TakingMove regularTakingMoves[64], kingTakingMoves[32];
    int tmsCount, rmsCount;
    TakingMove takingMoves[100];
    TakingSequence lastTakingSequence, takingSequences[16];
    RegMoveSequence regMoveSequences[32];
};

struct MoveContainer {
    Move move;
    TakingMove takingMove;
    TakingSequence takingSequence;
    int whatToTake;
};

struct Game {
    GameSituation situation;
    GameState state;
    GameType type;
};

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

Board createANewBoard(/*Color playerSide*/) {
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
    gameSituation.rmCount = 0; //gameSituation.krmCount = 0; gameSituation.kbmCount = 0;
    gameSituation.tmCount = 0;
    gameSituation.tmsCount = 0;
    gameSituation.rmsCount = 0;
    gameSituation.lastTakingSequence.tmsCount = 0;
    for (int i = 0; i < 16; i++) gameSituation.takingSequences[i].tmsCount = 0;
    Color turnOf;
    //gameSituation.rtmCount = 0; gameSituation.ktmCount = 0;
    return gameSituation;
}

Game createANewGame(Color playerSide, Color firstMove, GameType type) {
    Game newGame;
    newGame.situation = makeNullGameSituation(playerSide);
    newGame.state = (playerSide == firstMove) ? PlayerMove : ((type == RvsR) ? EnemyMoveReal : EnemyMoveComputer);
    return newGame;
}

Checker getCheckerByCoords(GameSituation* situation, int cx, int cy) {
    for (int i = 0; i < situation->board.checkersCount[White]; i++) {
        if (situation->board.checkers[White][i].coordinates.x == cx && situation->board.checkers[White][i].coordinates.y == cy) {
            return situation->board.checkers[White][i];
        }
    }
    for (int i = 0; i < situation->board.checkersCount[Black]; i++) {
        if (situation->board.checkers[Black][i].coordinates.x == cx && situation->board.checkers[Black][i].coordinates.y == cy) {
            return situation->board.checkers[Black][i];
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


bool isMarkedForDeath(GameSituation* situation, Color checkerColor, int mx, int my) {
    int index = getCheckerIndexByCoordsAndColor(situation, mx, my, checkerColor);
    return situation->board.checkers[checkerColor][index].markedForDeath;
}

bool isAVictim(GameSituation* situation, Color forWhichSide, int tx, int ty) {
    //bool lock = isMarkedForDeath(situation, negateColor(forWhichSide), getCheckerIndexByCoordsAndColor(situation, tx, ty, forWhichSide));
    BoardCellState tInfo = situation->board.boardRender[ty][tx];
    //if (lock) return false;
    //else {
        if (forWhichSide == Black) {
            return (tInfo == REG_WHITE || tInfo == KING_WHITE);
        } else if (forWhichSide == White) {
            return (tInfo == REG_BLACK || tInfo == KING_BLACK);
        } else return false;
    //}
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



void findAllKBMovesForOne(GameSituation* situation, Color checkerColor, int checkerIndex) {
    if (situation->board.checkers[checkerColor][checkerIndex].type != King) {
        Move move;
        move.isKingMove = false; move.isKingBecomingMove = true;
        int ex = situation->board.checkers[checkerColor][checkerIndex].coordinates.x;
        int ey = situation->board.checkers[checkerColor][checkerIndex].coordinates.y;
        move.source.x = ex; move.source.y = ey;

        if (checkerColor == Black ? ey == 1 : ey == 6) {
            if (ex < 7) {
                if ((situation->board.boardRender[(checkerColor == Black ? ey - 1 : ey + 1)][ex+1] == EMPTY_BLACK)) {
                    move.destination.y = (checkerColor == Black ? ey - 1 : ey + 1); move.destination.x = ex + 1;
                    situation->regularMoves[situation->rmCount++] = move;
                }
            }
            if (ex > 0) {
                if ((situation->board.boardRender[(checkerColor == Black ? ey - 1 : ey + 1)][ex-1] == EMPTY_BLACK)) {
                    move.destination.y = (checkerColor == Black ? ey - 1 : ey + 1); move.destination.x = ex - 1;
                    situation->regularMoves[situation->rmCount++] = move;
                }
            }
        }
    }
}

void findAllKingMovesForOne(GameSituation* situation, Color checkerColor, int checkerIndex) {
    if (situation->board.checkers[checkerColor][checkerIndex].type == King) {
        Move move;
        move.isKingMove = false;
        move.isKingBecomingMove = false;
        int ex = situation->board.checkers[checkerColor][checkerIndex].coordinates.x;
        int ey = situation->board.checkers[checkerColor][checkerIndex].coordinates.y;
        move.source.x = ex;
        move.source.y = ey;
        bool flag = !(ey == 7 || ex == 7);
        for (int j = 1; flag; j++) {
            if (situation->board.boardRender[ey + j][ex + j] == EMPTY_BLACK) {
                move.destination.x = ex + j;
                move.destination.y = ey + j;
                situation->regularMoves[situation->rmCount++] = move;
                if (ey + j == 7 || ex + j == 7) flag = false;
            } else {
                flag = false;
            }
        }
        flag = !(ey == 7 || ex == 0);
        for (int j = 1; flag; j++) {
            if (situation->board.boardRender[ey + j][ex - j] == EMPTY_BLACK) {
                move.destination.x = ex - j;
                move.destination.y = ey + j;
                situation->regularMoves[situation->rmCount++] = move;
                if (ey + j == 7 || ex - j == 0) flag = false;
            } else {
                flag = false;
            }
        }
        flag = !(ey == 0 || ex == 7);
        for (int j = 1; flag; j++) {
            if (situation->board.boardRender[ey - j][ex + j] == EMPTY_BLACK) {
                move.destination.x = ex + j;
                move.destination.y = ey - j;
                situation->regularMoves[situation->rmCount++] = move;
                if (ey - j == 0 || ex + j == 7) flag = false;
            } else {
                flag = false;
            }
        }
        flag = !(ey == 0 || ex == 0);
        for (int j = 1; flag; j++) {
            if (situation->board.boardRender[ey - j][ex - j] == EMPTY_BLACK) {
                move.destination.x = ex - j;
                move.destination.y = ey - j;
                situation->regularMoves[situation->rmCount++] = move;
                if (ey - j == 7 || ex - j == 7) flag = false;
            } else {
                flag = false;
            }
        }
    }
}

void findAllRegularMovesForOne(GameSituation* situation, Color checkerColor, int checkerIndex) {
    if (situation->board.checkers[checkerColor][checkerIndex].type != King) {
        Move move;
        move.isKingMove = false;
        move.isKingBecomingMove = false;
        int ex = situation->board.checkers[checkerColor][checkerIndex].coordinates.x;
        int ey = situation->board.checkers[checkerColor][checkerIndex].coordinates.y;
        move.source.x = ex;
        move.source.y = ey;
        bool flag = checkerColor == Black ? ey > 1 : ey < 6;
        if (flag) {
            if (ex < 7) {
                if ((situation->board.boardRender[checkerColor == Black ? ey - 1 : ey + 1][ex + 1] == EMPTY_BLACK)) {
                    move.destination.y = checkerColor == Black ? ey - 1 : ey + 1;
                    move.destination.x = ex + 1;
                    situation->regularMoves[situation->rmCount++] = move;
                    //situation->rmCount++;
                }
            }
            if (ex > 0) {
                if ((situation->board.boardRender[checkerColor == Black ? ey - 1 : ey + 1][ex - 1] == EMPTY_BLACK)) {
                    move.destination.y = checkerColor == Black ? ey - 1 : ey + 1;
                    move.destination.x = ex - 1;
                    situation->regularMoves[situation->rmCount++] = move;
                    //situation->rmCount++;
                }
            }
        }
    }
}

void findAllRegularMoves(GameSituation* situation, Color forWhichSide) {
    for (int i = 0; i < situation->board.checkersCount[forWhichSide]; i++) {
        findAllRegularMovesForOne(situation, forWhichSide, i);
    }
}

void findAllKingBecomingMoves(GameSituation* situation, Color forWhichSide) {
    for (int i = 0; i < situation->board.checkersCount[forWhichSide]; i++) {
        findAllKBMovesForOne(situation, forWhichSide, i);
    }
}

void findAllKingMoves(GameSituation* situation, Color forWhichSide) {
    for (int i = 0; i < situation->board.checkersCount[forWhichSide]; i++) {
        findAllKingMovesForOne(situation, forWhichSide, i);
    }
}



void findAllTakingMovesForOne(GameSituation* situation, Color checkerColor, int checkerIndex/*, TakingMove previousMove*/) {
    if (situation->board.checkers[checkerColor][checkerIndex].type != King) {
        TakingMove takingMove;
        int ex = situation->board.checkers[checkerColor][checkerIndex].coordinates.x;
        int ey = situation->board.checkers[checkerColor][checkerIndex].coordinates.y;
        takingMove.source.x = ex;
        takingMove.source.y = ey;
        //int vx = previousMove.victim.x, vy = previousMove.victim.y;
        if (ey < 6) {
            if (ex < 6) {
                //bool farIsFree = false;

                if ((situation->board.boardRender[ey + 2][ex + 2] == EMPTY_BLACK) && isAVictim(situation, checkerColor, ex + 1, ey + 1) && (!isMarkedForDeath(situation,negateColor(checkerColor), ex + 1, ey + 1))/* && ((ex + 1 != vx) || (ey + 1 != vy))*/) {
                    takingMove.destination.y = ey + 2;
                    takingMove.destination.x = ex + 2;
                    takingMove.victim.x = ex + 1;
                    takingMove.victim.y = ey + 1;
                    takingMove.takingSide = negateColor(checkerColor);
                    takingMove.victimType = getCheckerTypeOnBoard(situation, ex + 1, ey + 1);
                    takingMove.isASpecialMove = (checkerColor == White && ey == 5);
                    situation->takingMoves[situation->tmCount++] = takingMove;
                    //situation->rmCount++;
                }
            }
            if (ex > 1) {
                if ((situation->board.boardRender[ey + 2][ex - 2] == EMPTY_BLACK) && isAVictim(situation, checkerColor, ex - 1, ey + 1) && !isMarkedForDeath(situation,
                                                                                                                                                             negateColor(checkerColor), ex - 1, ey + 1)/* && ((ex - 1 != vx) || (ey + 1 != vy))*/) {
                    takingMove.destination.y = ey + 2;
                    takingMove.destination.x = ex - 2;
                    takingMove.victim.x = ex + 1;
                    takingMove.victim.y = ey - 1;
                    takingMove.takingSide = negateColor(checkerColor);
                    takingMove.victimType = getCheckerTypeOnBoard(situation, ex - 1, ey + 1);
                    takingMove.isASpecialMove = (checkerColor == White && ey == 5);
                    situation->takingMoves[situation->tmCount++] = takingMove;
                    //situation->rmCount++;
                }
            }
        }
        if (ey > 1) {
            if (ex < 6) {
                if ((situation->board.boardRender[ey - 2][ex + 2] == EMPTY_BLACK) && isAVictim(situation, checkerColor, ex + 1, ey - 1) && !isMarkedForDeath(situation,negateColor(checkerColor), ex + 1, ey - 1)/* && ((ex + 1 != vx) || (ey - 1 != vy))*/) {
                    takingMove.destination.y = ey - 2;
                    takingMove.destination.x = ex + 2;
                    takingMove.victim.x = ex + 1;
                    takingMove.victim.y = ey - 1;
                    takingMove.takingSide = negateColor(checkerColor);
                    takingMove.victimType = getCheckerTypeOnBoard(situation, ex + 1, ey - 1);
                    takingMove.isASpecialMove = (checkerColor == Black && ey == 2);
                    situation->takingMoves[situation->tmCount++] = takingMove;
                    //situation->rmCount++;
                }
            }
            if (ey > 1) {
                if ((situation->board.boardRender[ey - 2][ex - 2] == EMPTY_BLACK) && isAVictim(situation, checkerColor, ex - 1, ey - 1) && !isMarkedForDeath(situation,negateColor(checkerColor), ex - 1, ey - 1)/* && ((ex - 1 != vx) || (ey - 1 != vy))*/) {
                    takingMove.destination.y = ey - 2;
                    takingMove.destination.x = ex - 2;
                    takingMove.victim.x = ex - 1;
                    takingMove.victim.y = ey - 1;
                    takingMove.takingSide = negateColor(checkerColor);
                    takingMove.victimType = getCheckerTypeOnBoard(situation, ex - 1, ey - 1);
                    takingMove.isASpecialMove = (checkerColor == Black && ey == 2);
                    situation->takingMoves[situation->tmCount++] = takingMove;
                    //situation->rmCount++;
                }
            }
        }
    }
}


void findAllKingTakingMovesForOne(GameSituation* situation, Color checkerColor, int checkerIndex) {
    if (situation->board.checkers[checkerColor][checkerIndex].type == King) {
        TakingMove takingMove;
        takingMove.isASpecialMove = false;
        takingMove.takingSide = negateColor(checkerColor);
        int ex = situation->board.checkers[checkerColor][checkerIndex].coordinates.x;
        int ey = situation->board.checkers[checkerColor][checkerIndex].coordinates.y;
        takingMove.source.x = ex;
        takingMove.source.y = ey;
        int primalShift;
        bool flag = true, victimFound = false;
        for (primalShift = 1; flag; primalShift++) {
            if (isAVictim(situation, checkerColor, ex + primalShift, ey + primalShift) && !isMarkedForDeath(situation,negateColor(checkerColor), ex + primalShift, ey + primalShift)) {
                takingMove.victim.x = ex + primalShift;
                takingMove.victim.y = ey + primalShift;
                takingMove.victimType = getCheckerTypeOnBoard(situation, ex + primalShift, ey + primalShift);
                victimFound = true;
                flag = false;
            } else if (situation->board.boardRender[ey + primalShift][ex + primalShift] == EMPTY_BLACK) {
                //if (ex + primalShift)
                continue;
            } else if (isAFriend(situation, checkerColor, ex + primalShift, ey + primalShift) ||
                       (ex + primalShift == 7) || (ey + primalShift == 7) || isMarkedForDeath(situation,negateColor(checkerColor), ex + primalShift, ey + primalShift)) {
                flag = false;
            }
        }

        if (victimFound && (ex + primalShift - 1 != 7) && (ey + primalShift - 1 != 7)) {
            flag = true;
            for (int j = primalShift; flag; j++) {
                if (ex + j == 8 || ey + j == 8) {
                    flag = false;
                } else if (situation->board.boardRender[ey + j][ex + j] == EMPTY_BLACK) {
                    takingMove.destination.x = ex + j;
                    takingMove.destination.y = ey + j;
                    situation->takingMoves[situation->tmCount++] = takingMove;
                } else flag = false;
            }
        }

        flag = true, victimFound = false;
        for (primalShift = 1; flag; primalShift++) {
            if (isAVictim(situation, checkerColor, ex - primalShift, ey + primalShift) && !isMarkedForDeath(situation,negateColor(checkerColor), ex - primalShift, ey + primalShift)) {
                takingMove.victim.x = ex - primalShift;
                takingMove.victim.y = ey + primalShift;
                takingMove.victimType = getCheckerTypeOnBoard(situation, ex - primalShift, ey + primalShift);
                victimFound = true;
                flag = false;
            } else if (situation->board.boardRender[ey + primalShift][ex - primalShift] == EMPTY_BLACK) {
                //if (ex + primalShift)
                continue;
            } else if (isAFriend(situation, checkerColor, ex - primalShift, ey + primalShift) ||
                       (ex - primalShift == 0) || (ey + primalShift == 7) || isMarkedForDeath(situation,negateColor(checkerColor), ex - primalShift, ey + primalShift)) {
                flag = false;
            }
        }

        if (victimFound && (ex - primalShift  + 1 != 0) && (ey + primalShift - 1 != 7)) {
            flag = true;
            for (int j = primalShift; flag; j++) {
                if (ex - j == -1 || ey + j == 8) {
                    flag = false;
                } else if (situation->board.boardRender[ey + j][ex - j] == EMPTY_BLACK) {
                    takingMove.destination.x = ex - j;
                    takingMove.destination.y = ey + j;
                    situation->takingMoves[situation->tmCount++] = takingMove;
                } else flag = false;
            }
            // добавлять пустье поля пока не дойдём до непустой клетки
        }

        flag = true, victimFound = false;
        for (primalShift = 1; flag; primalShift++) {
            if (isAVictim(situation, checkerColor, ex + primalShift, ey - primalShift) && !isMarkedForDeath(situation,negateColor(checkerColor), ex + primalShift, ey - primalShift)) {
                takingMove.victim.x = ex + primalShift;
                takingMove.victim.y = ey - primalShift;
                takingMove.victimType = getCheckerTypeOnBoard(situation, ex + primalShift, ey - primalShift);
                victimFound = true;
                flag = false;
            } else if (situation->board.boardRender[ey - primalShift][ex + primalShift] == EMPTY_BLACK) {
                //if (ex + primalShift)
                continue;
            } else if (isAFriend(situation, checkerColor, ex + primalShift, ey - primalShift) ||
                       (ex + primalShift == 7) || (ey - primalShift == 0) || isMarkedForDeath(situation,negateColor(checkerColor), ex + primalShift, ey - primalShift)) {
                flag = false;
            }
        }

        if (victimFound && (ex + primalShift - 1 != 7) && (ey - primalShift + 1 != 0)) {
            flag = true;
            for (int j = primalShift; flag; j++) {
                if (ex + j == 8 || ey - j == -1) {
                    flag = false;
                } else if (situation->board.boardRender[ey - j][ex + j] == EMPTY_BLACK) {
                    takingMove.destination.x = ex + j;
                    takingMove.destination.y = ey - j;
                    situation->takingMoves[situation->tmCount++] = takingMove;
                } else flag = false;
            }
            // добавлять пустье поля пока не дойдём до непустой клетки
        }

        flag = true, victimFound = false;
        for (primalShift = 1; flag; primalShift++) {
            if (isAVictim(situation, checkerColor, ex - primalShift, ey - primalShift) && !isMarkedForDeath(situation,negateColor(checkerColor), ex - primalShift, ey - primalShift)) {
                takingMove.victim.x = ex - primalShift;
                takingMove.victim.y = ey - primalShift;
                takingMove.victimType = getCheckerTypeOnBoard(situation, ex - primalShift, ey - primalShift);
                victimFound = true;
                flag = false;
            } else if (situation->board.boardRender[ey - primalShift][ex - primalShift] == EMPTY_BLACK) {
                //if (ex + primalShift)
                continue;
            } else if (isAFriend(situation, checkerColor, ex - primalShift, ey - primalShift) ||
                       (ex - primalShift - 1== 0) || (ey - primalShift - 1== 0) && isMarkedForDeath(situation,negateColor(checkerColor), ex - primalShift, ey - primalShift)) {
                flag = false;
            }
        }

        if (victimFound && (ex - primalShift + 1!= 0) && (ey - primalShift + 1!= 0)) {
            flag = true;
            for (int j = primalShift; flag; j++) {
                if (ex - j == -1 || ey - j == -1) {
                    flag = false;
                } else if (situation->board.boardRender[ey - j][ex - j] == EMPTY_BLACK) {
                    takingMove.destination.x = ex - j;
                    takingMove.destination.y = ey - j;
                    situation->takingMoves[situation->tmCount++] = takingMove;
                } else flag = false;
            }
            // добавлять пустье поля пока не дойдём до непустой клетки
        }
    }

    
}

void findAllRegularTakingMoves(GameSituation* situation, Color forWhichSide) {
    for (int i = 0; i < situation->board.checkersCount[forWhichSide]; i++) {
        findAllTakingMovesForOne(situation, forWhichSide, i);
    }
}

void findAllKingTakingMoves(GameSituation* situation, Color forWhichSide) {
    for (int i = 0; i < situation->board.checkersCount[forWhichSide]; i++) {
        findAllKingTakingMovesForOne(situation, forWhichSide, i);
    }
}

inline void findAllTakingSequencesForOne(GameSituation* situation, Checker* checker, int found);

inline void clearLastTakingSequence(GameSituation* situation) {
    situation->lastTakingSequence.tmsCount = 0;
}

inline void clearRegularMoveList(GameSituation* situation) {
    situation->rmCount = 0;
}

inline void clearTakingMoveList(GameSituation* situation) {
    situation->tmCount = 0;
}

inline void clearMoveLists(GameSituation* situation) {
    clearRegularMoveList(situation);
    clearTakingMoveList(situation);
    //situation->tmCount = 0;
    //situation->krmCount = 0;
    //situation->kbmCount = 0;
    //situation->rtmCount = 0;
    //situation->ktmCount = 0;
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
    situation->board.checkers[movedColor][movedIndex].coordinates.x = move.destination.x;
    situation->board.checkers[movedColor][movedIndex].coordinates.y = move.destination.y;
    if (move.isKingBecomingMove) ascendChecker(&(situation->board.checkers[movedColor][movedIndex]));
    //updateBoardRender(&(situation->board));
    updateBoardRender(&(situation->board));
    clearMoveLists(situation);
    if (move.isKingBecomingMove) {
        situation->lastKingBecomingMove = move;
        findAllKingTakingMovesForOne(situation, movedColor, movedIndex);
        if (situation->tmCount == 0) {
            findAllKingMovesForOne(situation, movedColor, movedIndex);
            return 2;
        }
        return 1;
    } else return 0;
    //situation->board.boardRender[move.destination.y][move.destination.x] = situation->board.boardRender[move.source.y][move.source.x];
    //situation->board.boardRender[move.source.y][move.source.x] = EMPTY_BLACK;
}

void addToLastTakingSequence(GameSituation* situation, TakingMove move) {
    situation->lastTakingSequence.takingMoves[situation->lastTakingSequence.tmsCount++] = move;
}

int makeATakingMove(GameSituation* situation, TakingMove move) {

    Color movedColor = negateColor(move.takingSide);
    int movedIndex = getCheckerIndexByCoordsAndColor(situation, move.source.x, move.source.y, movedColor);
    Color victimColor = move.takingSide;
    int victimIndex = getCheckerIndexByCoordsAndColor(situation, move.victim.x, move.victim.y, victimColor);
    situation->board.checkers[movedColor][movedIndex].coordinates.x = move.destination.x;
    situation->board.checkers[movedColor][movedIndex].coordinates.y = move.destination.y;
    if (move.isASpecialMove) ascendChecker(&(situation->board.checkers[movedColor][movedIndex]));

    situation->board.checkers[victimColor][victimIndex].markedForDeath = true;
    clearMoveLists(situation);
    updateBoardRender(&(situation->board));
    if (situation->board.checkers[movedColor][movedIndex].type == King) {
        findAllKingTakingMovesForOne(situation, movedColor, movedIndex);
    }
    else {
        findAllTakingMovesForOne(situation, movedColor, movedIndex);
    }
    return situation->tmCount;
    //if (situation->tmCount != 0) {
        //situation->lastTakingSequence.takingMoves[situation->lastTakingSequence.tmsCount++] = move;
    //    return -1;
    //}
    //removeChecker(&(situation->board), victimIndex, victimColor);
    //updateBoardRender(&(situation->board));

    //clearMoveLists(situation);
    //if (move.isASpecialMove) return 1; else return 0;
}



int cancelAMove(GameSituation* situation, Move move) {
    Color movedColor = sideColor(situation, move.destination.x, move.destination.y);
    int movedIndex = getCheckerIndexByCoordsAndColor(situation, move.destination.x, move.destination.y, movedColor);
    situation->board.checkers[movedColor][movedIndex].coordinates.x = move.source.x;
    situation->board.checkers[movedColor][movedIndex].coordinates.y = move.source.y;
    if (move.isKingBecomingMove) descendChecker(&(situation->board.checkers[movedColor][movedIndex]));
    updateBoardRender(&(situation->board));
    if (move.isKingBecomingMove) return -1; else return 0;
    //situation->board.boardRender[move.destination.y][move.destination.x] = situation->board.boardRender[move.source.y][move.source.x];
    //situation->board.boardRender[move.source.y][move.source.x] = EMPTY_BLACK;
}

int cancelATakingMove(GameSituation* situation, TakingMove move) {
    Color movedColor = negateColor(move.takingSide);
    int movedIndex = getCheckerIndexByCoordsAndColor(situation, move.destination.x, move.destination.y, movedColor);
    //Color victimColor = move.takingSide;
    //int victimIndex = getCheckerIndexByCoordsAndColor(situation, move.victim.x, move.victim.y, victimColor);
    situation->board.checkers[movedColor][movedIndex].coordinates.x = move.source.x;
    situation->board.checkers[movedColor][movedIndex].coordinates.y = move.source.y;
    if (move.isASpecialMove) descendChecker(&(situation->board.checkers[movedColor][movedIndex]));
    Checker regeneratedVictim;
    regeneratedVictim.coordinates.x = move.victim.x;
    regeneratedVictim.coordinates.y = move.victim.y;
    regeneratedVictim.type = move.victimType;
    regeneratedVictim.color = move.takingSide;
    situation->board.checkers[move.takingSide][situation->board.checkersCount[move.takingSide]++] = regeneratedVictim;
    //removeChecker(&(situation->board), victimIndex, victimColor);
    //updateBoardRender(&(situation->board));
    updateBoardRender(&(situation->board));
    //clearMoveLists(situation);
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

    if (situation->rmCount != 0) {
        Move* buffer = new Move[situation->rmCount];
        for (int i = 0; i < situation->rmCount; i++) buffer[i] = situation->regularMoves[i];
        int savedRMs = situation->rmCount;
        for (int i = 0; i < savedRMs; i++) {
            Move extracted = buffer[i];
            if (extracted.isKingBecomingMove) {
                //int insertIndex = situation->rmsCount;
                clearMoveLists(situation);
                int insertIndex;
                makeAMove(situation, extracted);
                if (situation->tmCount == 0 && situation->rmCount != 0) {
                    for (int j = 0; j < situation->rmCount; j++) {
                        insertIndex = situation->rmsCount;
                        situation->regMoveSequences[insertIndex].regularMoves[0] = extracted;
                        situation->regMoveSequences[insertIndex].regularMoves[1] = situation->regularMoves[j];
                        situation->regMoveSequences[insertIndex].rmsCount = 2;
                        situation->rmsCount++;
                    }
                }
                cancelAMove(situation, extracted);
                //situation->regMoveSequences[insertIndex].regularMoves[situation->regMoveSequences[insertIndex].rmsCount++] = extracted;
                //situation->rmsCount++;
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
        clearMoveLists(situation);
    }
}

void makeARegMoveSequence(GameSituation* situation, RegMoveSequence regMoveSequence) {
    for (int i = 0; i < regMoveSequence.rmsCount; i++) makeAMove(situation, regMoveSequence.regularMoves[i]);
}

void findAllTakingSequencesForOne(GameSituation* situation, Color checkerColor, int checkerIndex) {
    findAllKingTakingMovesForOne(situation, checkerColor, checkerIndex);
    findAllTakingMovesForOne(situation, checkerColor, checkerIndex);

    TakingMove* buffer = new TakingMove[situation->tmCount];
    for (int i = 0; i < situation->tmCount; i++) buffer[i] = situation->takingMoves[i];
    int savedTMs = situation->tmCount;
    for (int i = 0; i < savedTMs; i++) {
        TakingMove extracted = buffer[i];
        int stat = makeATakingMove(situation, extracted);
        int insertIndex = situation->tmsCount, toWhere = situation->takingSequences[insertIndex].tmsCount;
        situation->takingSequences[insertIndex].takingMoves[toWhere] = extracted; // maybe 0
        toWhere++;//situation->takingSequences[insertIndex].tmsCount++;
        //situation->rmsCount++;
        if (stat != 0) {
            for (int i = situation->tmsCount; i < situation->tmsCount + stat; i++) {
                situation->takingSequences[i].takingMoves
            }
            findAllTakingSequencesForOne(situation, checkerColor, checkerIndex);
        }
        else situation->tmsCount++;
        cancelATakingMove(situation, extracted);
    }
    delete [] buffer;
}
// топо стек надо написать но это не стек при обходе храни текущий путь если листь удали если нечего смотреть удали короч так


inline void findAllMoves(GameSituation* situation, Color forWhichSide) {
    //findAllKingTakingMoves(situation, forWhichSide);
    //findAllRegularTakingMoves(situation, forWhichSide);
    if (situation->tmCount == 0) {
        findAllRegularMoveSequences(situation, forWhichSide);
        //findAllKingBecomingMoves(situation, forWhichSide);
        //findAllKingMoves(situation, forWhichSide);
        //findAllRegularMoves(situation, forWhichSide);
    }
    //findAllKingTakingMoves(situation, forWhichSide);
}

void clearAllSequencesLists(GameSituation* situation) {
    situation->rmsCount = 0;
    situation->tmsCount = 0;
}



void cancelLastTakingSequence(GameSituation* situation) {
    for (int i = situation->lastTakingSequence.tmsCount - 1; i > -1; i--) {
        cancelATakingMove(situation, situation->lastTakingSequence.takingMoves[i]);
    }
    clearLastTakingSequence(situation);
}

bool won(GameSituation* situation, Color who) {
    return situation->board.checkersCount[negateColor(who)] == 0;
}

int evaluateQuality(GameSituation* situation, Color forWhichSide) {
    int whiteEval = 0, blackEval = 0;
    if (won(situation, forWhichSide)) {
        return 10000000;
    }
    for (int i = 0; i < situation->board.checkersCount[White]; i++) {
        if (situation->board.checkers[White][i].type == King)  {
            whiteEval += kingCheckerCost * qualityFactor[situation->board.checkers[White][i].coordinates.y][situation->board.checkers[White][i].coordinates.x];
        }
        else {
            whiteEval += regularCheckerCost * qualityFactor[situation->board.checkers[White][i].coordinates.y][situation->board.checkers[White][i].coordinates.x];
        }
    }
    for (int i = 0; i < situation->board.checkersCount[Black]; i++) {
        if (situation->board.checkers[Black][i].type == King)  {
            blackEval += kingCheckerCost * qualityFactor[situation->board.checkers[Black][i].coordinates.y][situation->board.checkers[Black][i].coordinates.x];
        }
        else {
            blackEval += regularCheckerCost * qualityFactor[situation->board.checkers[Black][i].coordinates.y][situation->board.checkers[Black][i].coordinates.x];
        }
    }
    return (forWhichSide == White) ? whiteEval - blackEval : blackEval - whiteEval;
}



int analyysi(GameSituation* situation, Color side, int currentDepth, Difficulty maxDepth, MoveContainer* container) {
    int eval = evaluateQuality(situation, side);
    findAllMoves(situation, side);
    /*
     * do {

                            if (test.situation.tmCount == 0) {
                                Move t = test.situation.regularMoves[move];
                                stat = makeAMove(&test.situation, t);
                            }
                            else {
                                TakingMove t = test.situation.takingMoves[move];
                                stat = makeATakingMove(&test.situation, t);
                                addToLastTakingSequence(&test.situation, t);
                                //stat = 1;
                            }
                            renderBoard(&test.situation.board, White, 4, 4, true);
                            if (stat == 1 || stat == -1) {
                                //findAllKingTakingMovesForOne(&test.situation, forWho)
                                //renderBoard(&test.situation.board, White, 4, 4, true);

                                for (int i = 0; i < test.situation.tmCount; i++) {
                                    gotoxy(39, 2 + i);
                                    printf("%d) [%d : %d] -> [%d : %d] -> [%d : %d]",
                                           i,
                                           test.situation.takingMoves[i].source.x,
                                           test.situation.takingMoves[i].source.y,
                                           test.situation.takingMoves[i].victim.x,
                                           test.situation.takingMoves[i].victim.y,
                                           test.situation.takingMoves[i].destination.x,
                                           test.situation.takingMoves[i].destination.y);
                                }
                                for (int i = 0; i < test.situation.lastTakingSequence.tmsCount; i++) {
                                    gotoxy(39, 10 + i);
                                    printf("%d) [%d : %d] -> [%d : %d] -> [%d : %d]",
                                           i,
                                           test.situation.lastTakingSequence.takingMoves[i].source.x,
                                           test.situation.lastTakingSequence.takingMoves[i].source.y,
                                           test.situation.lastTakingSequence.takingMoves[i].victim.x,
                                           test.situation.lastTakingSequence.takingMoves[i].victim.y,
                                           test.situation.lastTakingSequence.takingMoves[i].destination.x,
                                           test.situation.lastTakingSequence.takingMoves[i].destination.y);
                                }
                                gotoxy(15, 13);
                                printf("%d", test.situation.tmCount);

                            } else if (stat == 2) {
                                for (int i = 0; i < test.situation.rmCount; i++) {
                                    gotoxy(16, 2 + i);
                                    printf("%d) [%d : %d] -> [%d : %d]",
                                           i,
                                           test.situation.regularMoves[i].source.x,
                                           test.situation.regularMoves[i].source.y,
                                           test.situation.regularMoves[i].destination.x,
                                           test.situation.regularMoves[i].destination.y);
                                }
                            }
                            //renderBoard(&test.situation.board, White, 4, 4, true);
                            //waitForKey(13);
                            //cancelAMove(&test.situation, t);
                            //makeAMove(&test.situation, test.situation.regularMoves[move]);
                            if (stat != 0) scanf("%d", &move);
                        } while (stat != 0);
                        removeMarkedForDeath(&test.situation, negateColor(forWho));
     */
    int bestMoveIndex = -1, bestMoveType = 0;
    if (situation->tmCount != 0) {
        //... todo
    }
    else if (situation->rmCount != 0) {
        int status;
        for (int i = 0; i < situation->rmCount; i++) {
            Move backup = situation->regularMoves[i];
            do {
                if (situation->tmCount == 0) {
                    status = makeAMove(situation, backup);
                }
                else {
                    TakingMove newSelected = situation->takingMoves[0];
                    status = makeATakingMove(situation, newSelected);
                    addToLastTakingSequence(situation, newSelected);
                }
            } while (status != 0);
            cancelAMove(situation, backup);
        }

    }
    else {
        //...todo idk but this is a failure for the analyzable side
    }
    clearMoveLists(situation);
    findAllMoves(situation, negateColor(side));
}

int getBestMove(GameSituation situation, Color side, Difficulty depth, MoveContainer* container) {
    GameSituation copyOfGS = situation;
    analyysi(&copyOfGS, side, 0, depth, container);

}

#endif CHECKERS_CLCHECKERS_H

