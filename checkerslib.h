#ifndef CHECKERSLIB_H_INCLUDED
#define CHECKERSLIB_H_INCLUDED

enum boardCellStates {EMPTY_BLACK_CELL, EMPTY_WHITE_CELL, FIGURE_REGULAR_WHITE, FIGURE_REGULAR_BLACK, FIGURE_KING_BLACK, FIGURE_KING_WHITE, AVALIABLE_MOVEMENT_CELL};

struct CheckersPiece {
    bool isKing, isThreating/*, isWhite*/;
    short boardX, boardY;
};

//Checker
//CheckersPiece nullCheckersPiece;
//nullCheckersPiece.isKing = false;

struct Board {
    short int renderedBoardTemplate[8][8];
    CheckersPiece blacks[12];
    CheckersPiece whites[12];
    short blacksCount = 12, whitesCount = 12;
};
//void inline getRenderedChar;

void initCPiece(CheckersPiece* piece) {
    piece->isThreating = false;
    piece->isKing = false;
}

void updateBoardRender(Board* board) {

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board->renderedBoardTemplate[j][i] = ((i + j) % 2 == 0) ? EMPTY_BLACK_CELL : EMPTY_WHITE_CELL;
        }
    }
    for (int i = 0; i < board->blacksCount; i++) {
        short placeX = board->blacks[i].boardX, placeY = board->blacks[i].boardY;
        board->renderedBoardTemplate[placeY][placeX] = (board->blacks[i].isKing) ? FIGURE_KING_BLACK : FIGURE_REGULAR_BLACK;
    }
    for (int i = 0; i < board->whitesCount; i++) {
        short placeX = board->whites[i].boardX, placeY = board->whites[i].boardY;
        board->renderedBoardTemplate[placeY][placeX] = (board->whites[i].isKing) ? FIGURE_KING_WHITE : FIGURE_REGULAR_WHITE;
    }
}

void setCheckersPieceCoordinates(/*Board* board, */CheckersPiece* piece, short newX, short newY) {
    //short oldX = piece->boardX, oldY = piece->boardY;
    piece->boardX = newX;
    piece->boardY = newY;
    //updateBoardRender(board);
    //board->redreredBoardTemplate[oldY][oldX] = ((oldX + oldY) % 2 == 0) ? EMPTY_BLACK_CELL : EMPTY_WHITE_CELL;
    //board->redreredBoardTemplate[newY][newX] = (piece->isKing) ?
}

void promoteAChecker(CheckersPiece* checker) {
    checker->isKing = true;
}

Board initiateGameBoard(bool playerSideIsWhite) {
    Board initBoard;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            initBoard.renderedBoardTemplate[j][i] = ((i + j) % 2 == 0) ? EMPTY_BLACK_CELL : EMPTY_WHITE_CELL;
        }
    }
    for (int i = 0; i < 12; i++) {
        initCPiece(&initBoard.blacks[i]);
        initCPiece(&initBoard.whites[i]);
    }
    int valueOfInitedPairs = 0;
    for (short j = 0; j < 3; j++) {
        for (short k = 0; k < 8; k += 2) {
            if (playerSideIsWhite) {
                setCheckersPieceCoordinates(&initBoard.whites[valueOfInitedPairs], k + (j % 2), j);
                setCheckersPieceCoordinates(&initBoard.blacks[valueOfInitedPairs++], k + 1 - (j % 2), 7 - j);
                    //initBoard.redreredBoardTemplate[j][k + (j % 2)] = FIGURE_REGULAR_WHITE;
                    //initBoard.redreredBoardTemplate[7-j][k + 1 - (j % 2)] = FIGURE_REGULAR_BLACK;
            }
            else {
                setCheckersPieceCoordinates(&initBoard.blacks[valueOfInitedPairs], k + (j % 2), j);
                setCheckersPieceCoordinates(&initBoard.whites[valueOfInitedPairs++], k + 1 - (j % 2), 7 - j);
                    //initBoard.redreredBoardTemplate[j][k + (j % 2)] = FIGURE_REGULAR_BLACK;
                    //initBoard.redreredBoardTemplate[7-j][k + 1 - (j % 2)] = FIGURE_REGULAR_WHITE;
            }
        }
    }
    //}
    updateBoardRender(&initBoard);
        //for (int i = 0)
    return initBoard;
}



void checkKingAppears(Board* board, bool playerSideIsWhite) {
    if (playerSideIsWhite) {
        for (int i = 0; i < board->blacksCount; i++) {
            if (board->blacks[i].boardY == 0) promoteAChecker(&(board->blacks[i]));
        }
        for (int i = 0; i < board->whitesCount; i++) {
            if (board->whites[i].boardY == 7) promoteAChecker(&(board->whites[i]));
        }
    }
    else {
        for (int i = 0; i < board->blacksCount; i++) {
            if (board->blacks[i].boardY == 7) promoteAChecker(&(board->blacks[i]));
        }
        for (int i = 0; i < board->whitesCount; i++) {
            if (board->whites[i].boardY == 0) promoteAChecker(&(board->whites[i]));
        }
    }
}
/*bool checkPossibilityToKill(Board board, int figureX, int figureY) {
    if (board.board[figureX][figureY] == NO_FIGURE_BLACK || board.board[figureX][figureY] == NO_FIGURE_WHITE)
        return false;
    else {
        if
    }
}*/
int getCheckerNumber(Board* board, int figureX, int figureY, bool color) {
    if (color) {
        for (int i = 0; i < board->whitesCount; i++) {
            if ((board->whites[i].boardX == figureX) && (board->whites[i].boardY == figureY)) return i;
        }
    }
    else {
        for (int i = 0; i < board->blacksCount; i++) {
            if ((board->blacks[i].boardX == figureX) && (board->blacks[i].boardY == figureY)) return i;
        }
    }
}
void getAvailableFields(Board board, int figureX, int figureY, bool playerSideIsWhite) {

}

#endif // CHECKERSLIB_H_INCLUDED
