//
// Created by EgrZver on 11.05.2023.
//

#include "clcengine_types.h"
#include "clcengine_auxiliary.h"
#include "clcengine_move_searching.h"
#include "clcengine_moving.h"
#include "clcengine_inits.h"

#ifndef CHECKERS_CLCENGINE_SEQUENCE_SEARCHING_H
#define CHECKERS_CLCENGINE_SEQUENCE_SEARCHING_H

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
                else {
                    insertIndex = situation->rmsCount;
                    situation->regMoveSequences[insertIndex].regularMoves[0] = extracted;
                    //situation->regMoveSequences[insertIndex].regularMoves[1] = situation->regularMovesBuffer[j];
                    situation->regMoveSequences[insertIndex].rmsCount = 1;
                    situation->rmsCount++;
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

void findAllTakingMoveSequences(GameSituation* situation, Color forWhichSide) {
    for (int i = 0; i < situation->board.checkersCount[forWhichSide]; i++) {
        TakingSequence path = getNullPath();
        findAllTakingSequencesForOne(situation, forWhichSide, i, &path);
        flushMoveBuffers(situation);
    }
}

/*
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
*/

/*void findAllMixedSequencesForOne(GameSituation* situation, Color checkerColor, int checkerIndex) {
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
*/

/*
void findAllMixedSequences(GameSituation* situation, Color forWhichSide) {
    for (int i = 0; i < situation->board.checkersCount[forWhichSide]; i++) {
        findAllMixedSequencesForOne(situation, forWhichSide, i);
        flushMoveBuffers(situation);
    }
}
*/

inline void findAllMoves(GameSituation* situation, Color forWhichSide) {
    findAllTakingMoveSequences(situation, forWhichSide);
    if (situation->tmsCount == 0) {
        //findAllMixedSequences(situation, forWhichSide);
        findAllRegularMoveSequences(situation, forWhichSide);
    }
}

#endif //CHECKERS_CLCENGINE_SEQUENCE_SEARCHING_H
