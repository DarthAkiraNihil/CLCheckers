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
};

struct Move {
    Coordinates source, destination;
    bool isKingMove, isKingBecomingMove;
    // todo remove king/taking types
};

struct TakingMove {
    Coordinates source, destination, victim;
    Color takingSide;
};

struct Board {
    BoardCellState boardRender[8][8];
    Checker blacks[12], whites[12];
    int blacksCount, whitesCount;
    //int rmCount, tmCount, kmCount;
    //Move regularMoves[32], takingMoves[32], kingMoves[16];
    //todo replace to another structure
};

struct GameSituation {
    Board board;
    int rmCount, krmCount, kbmCount, rtmCount, ktmCount;
    Color playerSide;
    Move regularMoves[32], kingRegularMoves[32], kingBecomingMoves[16];
    //Move regularMoves[128], kingBecomingMoves[16];
    TakingMove regularTakingMoves[64], kingTakingMoves[32];
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
}

void updateBoardRender(Board* board) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board->boardRender[j][i] = ((i + j) % 2 == 0) ? EMPTY_BLACK : EMPTY_WHITE;
        }
    }
    for (int i = 0; i < board->blacksCount; i++) {
        int rcx = board->blacks[i].coordinates.x, rcy = board->blacks[i].coordinates.y;
        board->boardRender[rcy][rcx] = (board->blacks[i].type == King) ? KING_BLACK : REG_BLACK;
    }
    for (int i = 0; i < board->whitesCount; i++) {
        int rcx = board->whites[i].coordinates.x, rcy = board->whites[i].coordinates.y;
        board->boardRender[rcy][rcx] = (board->whites[i].type == King) ? KING_WHITE : REG_WHITE;
    }
}

Board createANewBoard(/*Color playerSide*/) {
    Board newBoard;
    newBoard.blacksCount = 12; newBoard.whitesCount = 12;
    int valueOfInitedPairs = 0;
    for (short j = 0; j < 3; j++) {
        for (short k = 0; k < 8; k += 2) {
            initiateChecker(&newBoard.whites[valueOfInitedPairs], k + (j % 2), j, White);
            initiateChecker(&newBoard.blacks[valueOfInitedPairs++], k + 1 - (j % 2), 7 - j, Black);
        }
    }
    updateBoardRender(&newBoard);
    return newBoard;
}

GameSituation makeNullGameSituation(Color playerSide) {
    GameSituation gameSituation;
    gameSituation.board = createANewBoard();
    gameSituation.playerSide = playerSide;
    gameSituation.rmCount = 0; gameSituation.krmCount = 0; gameSituation.kbmCount = 0;
    gameSituation.rtmCount = 0; gameSituation.ktmCount = 0;
    return gameSituation;
}

Game createANewGame(Color playerSide, Color firstMove, GameType type) {
    Game newGame;
    newGame.situation = makeNullGameSituation(playerSide);
    newGame.state = (playerSide == firstMove) ? PlayerMove : ((type == RvsR) ? EnemyMoveReal : EnemyMoveComputer);
    return newGame;
}

void findAllRegularMoves(GameSituation* situation, Color forWhichSide) {
    if (forWhichSide == Black) {
        for (int i = 0; i < situation->board.blacksCount; i++) {
            if (situation->board.blacks[i].type != King) {
                Move move;
                move.isKingMove = false; move.isKingBecomingMove = false;
                int ex = situation->board.blacks[i].coordinates.x;
                int ey = situation->board.blacks[i].coordinates.y;
                move.source.x = ex; move.source.y = ey;
                if (ey > 1) {
                    if (ex < 7) {
                        if ((situation->board.boardRender[ey-1][ex+1] == EMPTY_BLACK)) {
                            move.destination.y = ey - 1; move.destination.x = ex + 1;
                            situation->regularMoves[situation->rmCount++] = move;
                        }
                    }
                    if (ex > 0) {
                        if ((situation->board.boardRender[ey-1][ex-1] == EMPTY_BLACK)) {
                            move.destination.y = ey - 1; move.destination.x = ex - 1;
                            situation->regularMoves[situation->rmCount++] = move;
                        }
                    }
                }
            }
        }
    }
    else if (forWhichSide == White) {
        for (int i = 0; i < situation->board.whitesCount; i++) {
            if (situation->board.whites[i].type != King) {
                Move move;
                move.isKingMove = false;
                move.isKingBecomingMove = false;
                int ex = situation->board.whites[i].coordinates.x;
                int ey = situation->board.whites[i].coordinates.y;
                move.source.x = ex;
                move.source.y = ey;
                if (ey < 6) {
                    if (ex < 7) {
                        if ((situation->board.boardRender[ey + 1][ex + 1] == EMPTY_BLACK)) {
                            move.destination.y = ey + 1;
                            move.destination.x = ex + 1;
                            situation->regularMoves[situation->rmCount++] = move;
                            //situation->rmCount++;
                        }
                    }
                    if (ex > 0) {
                        if ((situation->board.boardRender[ey + 1][ex - 1] == EMPTY_BLACK)) {
                            move.destination.y = ey + 1;
                            move.destination.x = ex - 1;
                            situation->regularMoves[situation->rmCount++] = move;
                            //situation->rmCount++;
                        }
                    }
                }
            }
        }
    }
}

void findAllKingBecomingMoves(GameSituation* situation, Color forWhichSide) {
    if (forWhichSide == Black) {
        for (int i = 0; i < situation->board.blacksCount; i++) {
            if (situation->board.blacks[i].type != King) {
                Move move;
                move.isKingMove = false; move.isKingBecomingMove = true;
                int ex = situation->board.blacks[i].coordinates.x;
                int ey = situation->board.blacks[i].coordinates.y;
                move.source.x = ex; move.source.y = ey;
                if (ey == 1) {
                    if (ex < 7) {
                        if ((situation->board.boardRender[ey-1][ex+1] == EMPTY_BLACK)) {
                            move.destination.y = ey - 1; move.destination.x = ex + 1;
                            situation->kingBecomingMoves[situation->kbmCount++] = move;
                        }
                    }
                    if (ex > 0) {
                        if ((situation->board.boardRender[ey-1][ex-1] == EMPTY_BLACK)) {
                            move.destination.y = ey - 1; move.destination.x = ex - 1;
                            situation->kingBecomingMoves[situation->kbmCount++] = move;
                        }
                    }
                }
            }
        }
    }
    else if (forWhichSide == White) {
        for (int i = 0; i < situation->board.whitesCount; i++) {
            if (situation->board.whites[i].type != King) {
                Move move;
                move.isKingMove = false;
                move.isKingBecomingMove = true;
                int ex = situation->board.whites[i].coordinates.x;
                int ey = situation->board.whites[i].coordinates.y;
                move.source.x = ex;
                move.source.y = ey;
                if (ey < 6) {
                    if (ex < 7) {
                        if ((situation->board.boardRender[ey + 1][ex + 1] == EMPTY_BLACK)) {
                            move.destination.y = ey + 1;
                            move.destination.x = ex + 1;
                            situation->kingBecomingMoves[situation->kbmCount++] = move;
                            //situation->rmCount++;
                        }
                    }
                    if (ex > 0) {
                        if ((situation->board.boardRender[ey + 1][ex - 1] == EMPTY_BLACK)) {
                            move.destination.y = ey + 1;
                            move.destination.x = ex - 1;
                            situation->kingBecomingMoves[situation->kbmCount++] = move;
                            //situation->rmCount++;
                        }
                    }
                }
            }
        }
    }
}

void findAllRegularKingMoves(GameSituation* situation, Color forWhichSide) {
    if (forWhichSide == Black) {
        for (int i = 0; i < situation->board.blacksCount; i++) {
            if (situation->board.blacks[i].type == King) {
                Move move;
                move.isKingMove = false; move.isKingBecomingMove = false;
                int ex = situation->board.blacks[i].coordinates.x;
                int ey = situation->board.blacks[i].coordinates.y;
                move.source.x = ex; move.source.y = ey;
                bool flag = !(ey == 7 || ex == 7);
                for (int i = 1; flag; i++) {
                    if (situation->board.boardRender[ey + i][ex + i] == EMPTY_BLACK) {
                        move.destination.x = ex + i; move.destination.y = ey + i;
                        situation->kingRegularMoves[situation->krmCount++] = move;
                        if (ey + i == 7 || ex + i == 7) flag = false;
                    }
                    else {
                        flag = false;
                    }
                }
                flag = !(ey == 7 || ex == 0);
                for (int i = 1; flag; i++) {
                    if (situation->board.boardRender[ey + i][ex - i] == EMPTY_BLACK) {
                        move.destination.x = ex - i; move.destination.y = ey + i;
                        situation->kingRegularMoves[situation->krmCount++] = move;
                        if (ey + i == 7 || ex - i == 0) flag = false;
                    }
                    else {
                        flag = false;
                    }
                }
                flag = !(ey == 0 || ex == 7);
                for (int i = 1; flag; i++) {
                    if (situation->board.boardRender[ey - i][ex + i] == EMPTY_BLACK) {
                        move.destination.x = ex + i; move.destination.y = ey - i;
                        situation->kingRegularMoves[situation->krmCount++] = move;
                        if (ey - i == 0 || ex + i == 7) flag = false;
                    }
                    else {
                        flag = false;
                    }
                }
                flag = !(ey == 0 || ex == 0);
                for (int i = 1; flag; i++) {
                    if (situation->board.boardRender[ey - i][ex - i] == EMPTY_BLACK) {
                        move.destination.x = ex - i; move.destination.y = ey - i;
                        situation->kingRegularMoves[situation->krmCount++] = move;
                        if (ey - i == 7 || ex - i == 7) flag = false;
                    }
                    else {
                        flag = false;
                    }
                }

            }
        }
    }
    else if (forWhichSide == White) {
        for (int i = 0; i < situation->board.whitesCount; i++) {
            if (situation->board.whites[i].type == King) {
                Move move;
                move.isKingMove = false; move.isKingBecomingMove = false;
                int ex = situation->board.whites[i].coordinates.x;
                int ey = situation->board.whites[i].coordinates.y;
                move.source.x = ex; move.source.y = ey;
                bool flag = !(ey == 7 || ex == 7);
                for (int i = 1; flag; i++) {
                    if (situation->board.boardRender[ey + i][ex + i] == EMPTY_BLACK) {
                        move.destination.x = ex + i; move.destination.y = ey + i;
                        situation->kingRegularMoves[situation->krmCount++] = move;
                        if (ey + i == 7 || ex + i == 7) flag = false;
                    }
                    else {
                        flag = false;
                    }
                }
                flag = !(ey == 7 || ex == 0);
                for (int i = 1; flag; i++) {
                    if (situation->board.boardRender[ey + i][ex - i] == EMPTY_BLACK) {
                        move.destination.x = ex - i; move.destination.y = ey + i;
                        situation->kingRegularMoves[situation->krmCount++] = move;
                        if (ey + i == 7 || ex - i == 0) flag = false;
                    }
                    else {
                        flag = false;
                    }
                }
                flag = !(ey == 0 || ex == 7);
                for (int i = 1; flag; i++) {
                    if (situation->board.boardRender[ey - i][ex + i] == EMPTY_BLACK) {
                        move.destination.x = ex + i; move.destination.y = ey - i;
                        situation->kingRegularMoves[situation->krmCount++] = move;
                        if (ey - i == 0 || ex + i == 7) flag = false;
                    }
                    else {
                        flag = false;
                    }
                }
                flag = !(ey == 0 || ex == 0);
                for (int i = 1; flag; i++) {
                    if (situation->board.boardRender[ey - i][ex - i] == EMPTY_BLACK) {
                        move.destination.x = ex - i; move.destination.y = ey - i;
                        situation->kingRegularMoves[situation->krmCount++] = move;
                        if (ey - i == 7 || ex - i == 7) flag = false;
                    }
                    else {
                        flag = false;
                    }
                }
            }
        }
    }
}

void findAllRegularTakingMoves(GameSituation* situation, Color forWhichSide) {

}

void findAllKingTakingMoves(GameSituation* situation, Color forWhichSide) {

}

void findAllMoves(GameSituation* situation, Color forWhichSide) {
    findAllRegularMoves(situation, forWhichSide);
    findAllKingBecomingMoves(situation, forWhichSide);
    findAllRegularKingMoves(situation, forWhichSide);
    //findAllRegularTakingMoves(situation, forWhichSide);
    //findAllKingTakingMoves(situation, forWhichSide);
}

void clearMoveLists(GameSituation* situation) {
    situation->rmCount = 0;
    situation->krmCount = 0;
    situation->kbmCount = 0;
    situation->rtmCount = 0;
    situation->ktmCount = 0;
}

void removeChecker(Board* board, int index, Color color) {
    if (color == Black && index < board->blacksCount) {
        for (int i = index + 1; i < board->blacksCount;i++) {
            board->blacks[i - 1] = board->blacks[i];
        }
        board->blacksCount--;
    }
    else if (color == White && index < board->whitesCount) {
        for (int i = index + 1; i < board->whitesCount;i++) {
            board->whites[i - 1] = board->whites[i];
        }
        board->whitesCount--;
    }
}

void ascendChecker(Checker* checker) {
    checker->type = King;
}

void makeAMove(Move move);

void makeATakingMove(TakingMove move);


#endif CHECKERS_CLCHECKERS_H

