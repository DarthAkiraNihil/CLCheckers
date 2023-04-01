//
// Created by EgrZver on 20.03.2023.
//

#ifndef CHECKERS_CLCHECKERS_H
#define CHECKERS_CLCHECKERS_H

enum BoardCellState {EMPTY_BLACK, EMPTY_WHITE, REG_BLACK, REG_WHITE, KING_BLACK, KING_WHITE};
enum Color {Black, White};
enum CheckerType {Regular, King};
enum GameState {PlayerMove, EnemyMoveReal, EnemyMoveComputer};
enum GameType {RvsR, RvsC}; //RvsR = Real versus Real, RvsC = Real vs Computer

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
    TakingMove takingMoves[100];
    TakingSequence lastTakingSequence;
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
    gameSituation.lastTakingSequence.tmsCount = 0;
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

inline void findAllRegularMoves(GameSituation* situation, Color forWhichSide) {
    for (int i = 0; i < situation->board.checkersCount[forWhichSide]; i++) {
        if (situation->board.checkers[forWhichSide][i].type != King) {
            Move move;
            move.isKingMove = false;
            move.isKingBecomingMove = false;
            int ex = situation->board.checkers[forWhichSide][i].coordinates.x;
            int ey = situation->board.checkers[forWhichSide][i].coordinates.y;
            move.source.x = ex;
            move.source.y = ey;
            bool flag = forWhichSide == Black ? ey > 1 : ey < 6;
            if (flag) {
                if (ex < 7) {
                    if ((situation->board.boardRender[forWhichSide == Black ? ey - 1 : ey + 1][ex + 1] == EMPTY_BLACK)) {
                        move.destination.y = forWhichSide == Black ? ey - 1 : ey + 1;
                        move.destination.x = ex + 1;
                        situation->regularMoves[situation->rmCount++] = move;
                        //situation->rmCount++;
                    }
                }
                if (ex > 0) {
                    if ((situation->board.boardRender[forWhichSide == Black ? ey - 1 : ey + 1][ex - 1] == EMPTY_BLACK)) {
                        move.destination.y = forWhichSide == Black ? ey - 1 : ey + 1;
                        move.destination.x = ex - 1;
                        situation->regularMoves[situation->rmCount++] = move;
                        //situation->rmCount++;
                    }
                }
            }
        }
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


inline void findAllKingBecomingMoves(GameSituation* situation, Color forWhichSide) {
    for (int i = 0; i < situation->board.checkersCount[forWhichSide]; i++) {
        if (situation->board.checkers[forWhichSide][i].type != King) {
            Move move;
            move.isKingMove = false; move.isKingBecomingMove = true;
            int ex = situation->board.checkers[forWhichSide][i].coordinates.x;
            int ey = situation->board.checkers[forWhichSide][i].coordinates.y;
            move.source.x = ex; move.source.y = ey;

            if (forWhichSide == Black ? ey == 1 : ey == 6) {
                if (ex < 7) {
                    if ((situation->board.boardRender[(forWhichSide == Black ? ey - 1 : ey + 1)][ex+1] == EMPTY_BLACK)) {
                        move.destination.y = (forWhichSide == Black ? ey - 1 : ey + 1); move.destination.x = ex + 1;
                        situation->regularMoves[situation->rmCount++] = move;
                    }
                }
                if (ex > 0) {
                    if ((situation->board.boardRender[(forWhichSide == Black ? ey - 1 : ey + 1)][ex-1] == EMPTY_BLACK)) {
                        move.destination.y = (forWhichSide == Black ? ey - 1 : ey + 1); move.destination.x = ex - 1;
                        situation->regularMoves[situation->rmCount++] = move;
                    }
                }
            }
        }
    }
}

inline void findAllRegularKingMoves(GameSituation* situation, Color forWhichSide) {
    for (int i = 0; i < situation->board.checkersCount[forWhichSide]; i++) {
        if (situation->board.checkers[forWhichSide][i].type == King) {
            Move move;
            move.isKingMove = false; move.isKingBecomingMove = false;
            int ex = situation->board.checkers[forWhichSide][i].coordinates.x;
            int ey = situation->board.checkers[forWhichSide][i].coordinates.y;
            move.source.x = ex; move.source.y = ey;
            bool flag = !(ey == 7 || ex == 7);
            for (int j = 1; flag; j++) {
                if (situation->board.boardRender[ey + j][ex + j] == EMPTY_BLACK) {
                    move.destination.x = ex + j; move.destination.y = ey + j;
                    situation->regularMoves[situation->rmCount++] = move;
                    if (ey + j == 7 || ex + j == 7) flag = false;
                }
                else {
                    flag = false;
                }
            }
            flag = !(ey == 7 || ex == 0);
            for (int j = 1; flag; j++) {
                if (situation->board.boardRender[ey + j][ex - j] == EMPTY_BLACK) {
                    move.destination.x = ex - j; move.destination.y = ey + j;
                    situation->regularMoves[situation->rmCount++] = move;
                    if (ey + j == 7 || ex - j == 0) flag = false;
                }
                else {
                    flag = false;
                }
            }
            flag = !(ey == 0 || ex == 7);
            for (int j = 1; flag; j++) {
                if (situation->board.boardRender[ey - j][ex + j] == EMPTY_BLACK) {
                    move.destination.x = ex + j; move.destination.y = ey - j;
                    situation->regularMoves[situation->rmCount++] = move;
                    if (ey - j == 0 || ex + j == 7) flag = false;
                }
                else {
                    flag = false;
                }
            }
            flag = !(ey == 0 || ex == 0);
            for (int j = 1; flag; j++) {
                if (situation->board.boardRender[ey - j][ex - j] == EMPTY_BLACK) {
                    move.destination.x = ex - j; move.destination.y = ey - j;
                    situation->regularMoves[situation->rmCount++] = move;
                    if (ey - j == 7 || ex - j == 7) flag = false;
                }
                else {
                    flag = false;
                }
            }

        }
    }
}

inline void findAllRegularTakingMoves(GameSituation* situation, Color forWhichSide) {
    for (int i = 0; i < situation->board.checkersCount[forWhichSide]; i++) {
        if (situation->board.checkers[forWhichSide][i].type != King) {
            TakingMove takingMove;
            int ex = situation->board.checkers[forWhichSide][i].coordinates.x;
            int ey = situation->board.checkers[forWhichSide][i].coordinates.y;
            takingMove.source.x = ex;
            takingMove.source.y = ey;
            if (ey < 6) {
                if (ex < 6) {
                    //bool farIsFree = false;

                    if ((situation->board.boardRender[ey + 2][ex + 2] == EMPTY_BLACK) && isAVictim(situation, forWhichSide, ex + 1, ey + 1)) {
                        takingMove.destination.y = ey + 2;
                        takingMove.destination.x = ex + 2;
                        takingMove.victim.x = ex + 1;
                        takingMove.victim.y = ey + 1;
                        takingMove.takingSide = negateColor(forWhichSide);
                        takingMove.victimType = getCheckerTypeOnBoard(situation, ex + 1, ey + 1);
                        takingMove.isASpecialMove = (forWhichSide == White && ey == 5);
                        situation->takingMoves[situation->tmCount++] = takingMove;
                        //situation->rmCount++;
                    }
                }
                if (ex > 1) {
                    if ((situation->board.boardRender[ey + 2][ex - 2] == EMPTY_BLACK) && isAVictim(situation, forWhichSide, ex - 1, ey + 1)) {
                        takingMove.destination.y = ey + 2;
                        takingMove.destination.x = ex - 2;
                        takingMove.victim.x = ex + 1;
                        takingMove.victim.y = ey - 1;
                        takingMove.takingSide = negateColor(forWhichSide);
                        takingMove.victimType = getCheckerTypeOnBoard(situation, ex - 1, ey + 1);
                        takingMove.isASpecialMove = (forWhichSide == White && ey == 5);
                        situation->takingMoves[situation->tmCount++] = takingMove;
                        //situation->rmCount++;
                    }
                }
            }
            if (ey > 1) {
                if (ex < 6) {
                    if ((situation->board.boardRender[ey - 2][ex + 2] == EMPTY_BLACK) && isAVictim(situation, forWhichSide, ex + 1, ey - 1)) {
                        takingMove.destination.y = ey - 2;
                        takingMove.destination.x = ex + 2;
                        takingMove.victim.x = ex + 1;
                        takingMove.victim.y = ey - 1;
                        takingMove.takingSide = negateColor(forWhichSide);
                        takingMove.victimType = getCheckerTypeOnBoard(situation, ex + 1, ey - 1);
                        takingMove.isASpecialMove = (forWhichSide == Black && ey == 2);
                        situation->takingMoves[situation->tmCount++] = takingMove;
                        //situation->rmCount++;
                    }
                }
                if (ey > 1) {
                    if ((situation->board.boardRender[ey - 2][ex - 2] == EMPTY_BLACK) && isAVictim(situation, forWhichSide, ex - 1, ey - 1)) {
                        takingMove.destination.y = ey - 2;
                        takingMove.destination.x = ex - 2;
                        takingMove.victim.x = ex - 1;
                        takingMove.victim.y = ey - 1;
                        takingMove.takingSide = negateColor(forWhichSide);
                        takingMove.victimType = getCheckerTypeOnBoard(situation, ex - 1, ey - 1);
                        takingMove.isASpecialMove = (forWhichSide == Black && ey == 2);
                        situation->takingMoves[situation->tmCount++] = takingMove;
                        //situation->rmCount++;
                    }
                }
            }
        }
    }

}

inline void findAllKingTakingMoves(GameSituation* situation, Color forWhichSide) {
    for (int i = 0; i < situation->board.checkersCount[forWhichSide]; i++) {
        if (situation->board.checkers[forWhichSide][i].type == King) {
            TakingMove takingMove;
            takingMove.isASpecialMove = false;
            takingMove.takingSide = negateColor(forWhichSide);
            int ex = situation->board.checkers[forWhichSide][i].coordinates.x;
            int ey = situation->board.checkers[forWhichSide][i].coordinates.y;
            takingMove.source.x = ex;
            takingMove.source.y = ey;
            int primalShift;
            bool flag = true, victimFound = false;
            for (primalShift = 1; flag; primalShift++) {
                if (isAVictim(situation, forWhichSide, ex + primalShift, ey + primalShift)) {
                    takingMove.victim.x = ex + primalShift;
                    takingMove.victim.y = ey + primalShift;
                    takingMove.victimType = getCheckerTypeOnBoard(situation, ex + primalShift, ey + primalShift);
                    victimFound = true;
                    flag = false;
                } else if (situation->board.boardRender[ey + primalShift][ex + primalShift] == EMPTY_BLACK) {
                    //if (ex + primalShift)
                    continue;
                } else if (isAFriend(situation, forWhichSide, ex + primalShift, ey + primalShift) ||
                           (ex + primalShift == 7) || (ey + primalShift == 7)) {
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
                // добавлять пустье поля пока не дойдём до непустой клетки
            }

            flag = true, victimFound = false;
            for (primalShift = 1; flag; primalShift++) {
                if (isAVictim(situation, forWhichSide, ex - primalShift, ey + primalShift)) {
                    takingMove.victim.x = ex - primalShift;
                    takingMove.victim.y = ey + primalShift;
                    takingMove.victimType = getCheckerTypeOnBoard(situation, ex - primalShift, ey + primalShift);
                    victimFound = true;
                    flag = false;
                } else if (situation->board.boardRender[ey + primalShift][ex - primalShift] == EMPTY_BLACK) {
                    //if (ex + primalShift)
                    continue;
                } else if (isAFriend(situation, forWhichSide, ex - primalShift, ey + primalShift) ||
                           (ex - primalShift == 0) || (ey + primalShift == 7)) {
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
                if (isAVictim(situation, forWhichSide, ex + primalShift, ey - primalShift)) {
                    takingMove.victim.x = ex + primalShift;
                    takingMove.victim.y = ey - primalShift;
                    takingMove.victimType = getCheckerTypeOnBoard(situation, ex + primalShift, ey - primalShift);
                    victimFound = true;
                    flag = false;
                } else if (situation->board.boardRender[ey - primalShift][ex + primalShift] == EMPTY_BLACK) {
                    //if (ex + primalShift)
                    continue;
                } else if (isAFriend(situation, forWhichSide, ex + primalShift, ey - primalShift) ||
                           (ex + primalShift == 7) || (ey - primalShift == 0)) {
                    flag = false;
                }
            }

            if (victimFound && (ex + primalShift - 1 != 7) && (ey - primalShift + 1!= 0)) {
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
                if (isAVictim(situation, forWhichSide, ex - primalShift, ey - primalShift)) {
                    takingMove.victim.x = ex - primalShift;
                    takingMove.victim.y = ey - primalShift;
                    takingMove.victimType = getCheckerTypeOnBoard(situation, ex - primalShift, ey - primalShift);
                    victimFound = true;
                    flag = false;
                } else if (situation->board.boardRender[ey - primalShift][ex - primalShift] == EMPTY_BLACK) {
                    //if (ex + primalShift)
                    continue;
                } else if (isAFriend(situation, forWhichSide, ex - primalShift, ey - primalShift) ||
                           (ex - primalShift - 1== 0) || (ey - primalShift - 1== 0)) {
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
}
//void findAllRegularMovesForOne(GameSituation* situation, Color checkerColor, int checkerIndex);

void findAllRegularKingMovesForOne(GameSituation* situation, Color checkerColor, int checkerIndex) {

        if (situation->board.checkers[checkerColor][checkerIndex].type == King) {
            Move move;
            move.isKingMove = false; move.isKingBecomingMove = false;
            int ex = situation->board.checkers[checkerColor][checkerIndex].coordinates.x;
            int ey = situation->board.checkers[checkerColor][checkerIndex].coordinates.y;
            move.source.x = ex; move.source.y = ey;
            bool flag = !(ey == 7 || ex == 7);
            for (int j = 1; flag; j++) {
                if (situation->board.boardRender[ey + j][ex + j] == EMPTY_BLACK) {
                    move.destination.x = ex + j; move.destination.y = ey + j;
                    situation->regularMoves[situation->rmCount++] = move;
                    if (ey + j == 7 || ex + j == 7) flag = false;
                }
                else {
                    flag = false;
                }
            }
            flag = !(ey == 7 || ex == 0);
            for (int j = 1; flag; j++) {
                if (situation->board.boardRender[ey + j][ex - j] == EMPTY_BLACK) {
                    move.destination.x = ex - j; move.destination.y = ey + j;
                    situation->regularMoves[situation->rmCount++] = move;
                    if (ey + j == 7 || ex - j == 0) flag = false;
                }
                else {
                    flag = false;
                }
            }
            flag = !(ey == 0 || ex == 7);
            for (int j = 1; flag; j++) {
                if (situation->board.boardRender[ey - j][ex + j] == EMPTY_BLACK) {
                    move.destination.x = ex + j; move.destination.y = ey - j;
                    situation->regularMoves[situation->rmCount++] = move;
                    if (ey - j == 0 || ex + j == 7) flag = false;
                }
                else {
                    flag = false;
                }
            }
            flag = !(ey == 0 || ex == 0);
            for (int j = 1; flag; j++) {
                if (situation->board.boardRender[ey - j][ex - j] == EMPTY_BLACK) {
                    move.destination.x = ex - j; move.destination.y = ey - j;
                    situation->regularMoves[situation->rmCount++] = move;
                    if (ey - j == 7 || ex - j == 7) flag = false;
                }
                else {
                    flag = false;
                }
            }

        }

}

void findAllTakingMovesForOne(GameSituation* situation, Color checkerColor, int checkerIndex, TakingMove previousMove) {
    if (situation->board.checkers[checkerColor][checkerIndex].type != King) {
        TakingMove takingMove;
        int ex = situation->board.checkers[checkerColor][checkerIndex].coordinates.x;
        int ey = situation->board.checkers[checkerColor][checkerIndex].coordinates.y;
        takingMove.source.x = ex;
        takingMove.source.y = ey;
        int vx = previousMove.victim.x, vy = previousMove.victim.y;
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
inline void findAllMoves(GameSituation* situation, Color forWhichSide) {
    findAllKingTakingMoves(situation, forWhichSide);
    findAllRegularTakingMoves(situation, forWhichSide);
    if (situation->tmCount == 0) {
        findAllKingBecomingMoves(situation, forWhichSide);
        findAllRegularKingMoves(situation, forWhichSide);
        findAllRegularMoves(situation, forWhichSide);
    }
    //findAllKingTakingMoves(situation, forWhichSide);
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
            findAllRegularKingMovesForOne(situation, movedColor, movedIndex);
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
        findAllTakingMovesForOne(situation, movedColor, movedIndex, move);
    }

    if (situation->tmCount != 0) {
        //situation->lastTakingSequence.takingMoves[situation->lastTakingSequence.tmsCount++] = move;
        return -1;
    }
    //removeChecker(&(situation->board), victimIndex, victimColor);
    //updateBoardRender(&(situation->board));

    //clearMoveLists(situation);
    if (move.isASpecialMove) return 1; else return 0;
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
void cancelLastTakingSequence(GameSituation* situation) {
    for (int i = situation->lastTakingSequence.tmsCount - 1; i > -1; i--) {
        cancelATakingMove(situation, situation->lastTakingSequence.takingMoves[i]);
    }
    clearLastTakingSequence(situation);
}

int analyysi();

#endif CHECKERS_CLCHECKERS_H

