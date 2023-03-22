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
    bool isKingMove, isTakingMove, isKingBecomingMove;
};

struct Board {
    BoardCellState boardRender[8][8];
    Checker blacks[12], whites[12];
    int blacksCount, whitesCount;
    int rmCount, tmCount, kmCount;
    Move regularMoves[32], takingMoves[32], kingMoves[16];
    //todo replace to another structure
};

struct GameSituation {
    Board board;
    int rmCount, tmCount, kmCount;
    Color playerSide;
    Move regularMoves[32], takingMoves[32], kingMoves[10];
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
    newBoard.rmCount = 0; newBoard.tmCount = 0; newBoard.kmCount = 0;
    /*for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            newBoard.boardRender[j][i] = ((i + j) % 2 == 0) ? EMPTY_BLACK : EMPTY_WHITE;
        }
    }*/
    /*for (int i = 0; i < 12; i++) {
        initCPiece(&initBoard.blacks[i]);
        initCPiece(&initBoard.whites[i]);
    }*/
    /*int valueOfInitedPairs = 0;
    for (short j = 0; j < 3; j++) {
        for (short k = 0; k < 8; k += 2) {
            if (playerSide == White) {
                initiateChecker(&newBoard.whites[valueOfInitedPairs], k + (j % 2), j, White);
                initiateChecker(&newBoard.blacks[valueOfInitedPairs++], k + 1 - (j % 2), 7 - j, Black);
            }
            else {
                initiateChecker(&newBoard.blacks[valueOfInitedPairs], k + (j % 2), j, Black);
                initiateChecker(&newBoard.whites[valueOfInitedPairs++], k + 1 - (j % 2), 7 - j, White);
            }
        }
    }*/
    int valueOfInitedPairs = 0;
    for (short j = 0; j < 3; j++) {
        for (short k = 0; k < 8; k += 2) {
            initiateChecker(&newBoard.whites[valueOfInitedPairs], k + (j % 2), j, White);
            initiateChecker(&newBoard.blacks[valueOfInitedPairs++], k + 1 - (j % 2), 7 - j, Black);
        }
    }
    updateBoardRender(&newBoard);
    //for (int i = 0)
    return newBoard;
}

GameSituation makeNullGameSituation(Color playerSide) {
    GameSituation gameSituation;
    gameSituation.board = createANewBoard();
    gameSituation.playerSide = playerSide;
    gameSituation.rmCount = 0; gameSituation.tmCount = 0; gameSituation.kmCount = 0;
}

Game createANewGame(Color playerSide, Color firstMove, GameType type) {
    Game newGame;
    newGame.situation = makeNullGameSituation(playerSide);
    newGame.state = (playerSide == firstMove) ? PlayerMove : ((type == RvsR) ? EnemyMoveReal : EnemyMoveComputer);
}

void findAllRegularMoves(GameSituation* situation, Color forWhichSide) {
    int foundMoves;
    if (forWhichSide == Black) {
        //for (int i )
    }
    else if (forWhichSide == White) {
        for (int i = 0; i < situation->board.whitesCount; i++) {
            Move move;
            move.isKingMove = false; move.isTakingMove = false;
            int ex = situation->board.whites[i].coordinates.x;
            int ey = situation->board.whites[i].coordinates.y;
            if (ey < 7) {
                switch (ex) {
                    case 0: {
                        if ((situation->board.boardRender[ey+1][ex+1] == REG_BLACK) || (situation->board.boardRender[ey+1][ex+1] == KING_BLACK)) {
                            Move move;
                            move.source.y = ey; move.source.x = ex;
                            move.destination.y = ey + 1; move.destination.x = ex + 1;
                        }
                        break;
                    }
                    case 7: {
                        break;
                    }
                    default: {
                        break;
                    }
                }
            }
        }
    }
    /*else if (forWhichSide == White) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (situation->board.boardRender[j][i] == REG_WHITE) {

                }
            }
        }
    }*/
}

/*cvoid findAllRegularMoves(Board* board, Checker checker, Color playerSide, Move* arrayTo) {
}

void findAllTakingMoves(Board* board, Checker checker, Move* arrayTo) {

}

void findAllKingMoves(Board* board, Checker checker, Move* arrayTo) {

}

void findAllMoves(Board* board, Color forWho, Color playerSide) {
    // reg -> tak -> king
    if (forWho == White && playerSide == White) {
        for (int i = 0; i < board->whitesCount; i++) {
            if (board->whites[i].type == Regular) {

            }
        }
    }
    else if (forWho == White && playerSide == Black) {

    }
    else if (forWho == Black && playerSide == White) {

    }
    else if (forWho == Black && playerSide == Black) {

    }
}/*

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
*/

#endif CHECKERS_CLCHECKERS_H

