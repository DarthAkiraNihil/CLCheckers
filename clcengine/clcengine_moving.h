//
// Created by EgrZver on 11.05.2023.
//

#include "clcengine_types.h"
#include "clcengine_auxiliary.h"
#include "clcengine_move_searching.h"

#ifndef CHECKERS_CLCENGINE_MOVING_H
#define CHECKERS_CLCENGINE_MOVING_H

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

#endif //CHECKERS_CLCENGINE_MOVING_H
