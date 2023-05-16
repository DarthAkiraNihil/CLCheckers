//
// Created by EgrZver on 11.05.2023.
//

#include "clcengine_types.h"
#include "clcengine_auxiliary.h"

#ifndef CHECKERS_CLCENGINE_INITS_H
#define CHECKERS_CLCENGINE_INITS_H

SeqContainer getNullContainer(Color side) {
    SeqContainer container;
    container.eval = getVictoryEvalFor(negateColor(side));
    container.seqNumberToDo = -1;
    container.takingSequence.tmsCount = 0;
    container.regMoveSequence.rmsCount = 0;
    //container.mixedSequence.takingSequence.tmsCount = 0;
    return container;
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

void resetPathMap(Board* board) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board->pathMap[j][i] = NoMove;
        }
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
    resetPathMap(&newBoard);
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
    newGame.type = type;
    newGame.situation.playerSide = playerSide;
    return newGame;
}

#endif //CHECKERS_CLCENGINE_INITS_H
