//
// Created by EgrZver on 20.03.2023.
//

#ifndef CHECKERS_CLCHECKERS_H
#define CHECKERS_CLCHECKERS_H

enum BoardCellState {EMPTY_BLACK, EMPTY_WHITE, REG_BLACK, REG_WHITE, KING_BLACK, KING_WHITE};
enum Color {Black, White};
enum CheckerType {Regular, King};

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
    bool isKingMove, isTakingMove;
};

struct Board {
    BoardCellState boardRender[8][8];
    Checker blacks[12], whites[12];
    int blacksCount, whitesCount;
    int rmCount, tmCount, kmCount;
    Move regularMoves[32], takingMoves[32], kingMoves[16];
};

void initiateChecker(Checker* checker, int x, int y) {
    checker->coordinates.x = x;
    checker->coordinates.y = y;

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

Board createANewBoard(Color playerSide) {
    Board newBoard;
    newBoard.blacksCount = 12; newBoard.whitesCount = 12;
    newBoard.rmCount = 0; newBoard.tmCount = 0; newBoard.kmCount = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            newBoard.boardRender[j][i] = ((i + j) % 2 == 0) ? EMPTY_BLACK : EMPTY_WHITE;
        }
    }
    /*for (int i = 0; i < 12; i++) {
        initCPiece(&initBoard.blacks[i]);
        initCPiece(&initBoard.whites[i]);
    }*/
    int valueOfInitedPairs = 0;
    for (short j = 0; j < 3; j++) {
        for (short k = 0; k < 8; k += 2) {
            if (playerSide == White) {
                initiateChecker(&newBoard.whites[valueOfInitedPairs], k + (j % 2), j);
                initiateChecker(&newBoard.blacks[valueOfInitedPairs++], k + 1 - (j % 2), 7 - j);
            }
            else {
                initiateChecker(&newBoard.blacks[valueOfInitedPairs], k + (j % 2), j);
                initiateChecker(&newBoard.whites[valueOfInitedPairs++], k + 1 - (j % 2), 7 - j);
            }
        }
    }
    //}
    updateBoardRender(&newBoard);
    //for (int i = 0)
    return newBoard;
}

void findAllRegularMoves(Board* board, Checker checker, Color playerSide, Move* arrayTo) {
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

struct Game {

};
#endif CHECKERS_CLCHECKERS_H

