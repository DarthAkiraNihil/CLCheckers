//
// Created by EgrZver on 11.05.2023.
//
#include <random>
#include "clcengine_types.h"
#include "clcengine_inits.h"
#include "clcengine_sequence_searching.h"
#include "clcengine_moving.h"

#ifndef CHECKERS_CLCENGINE_ANALYZER_H
#define CHECKERS_CLCENGINE_ANALYZER_H

std::random_device analyzerRD;
std::mt19937 agen(analyzerRD());
std::uniform_int_distribution<> analyzerDistribution(1, 1000);

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
        /*MixedSequence* msBackup = new MixedSequence[situation->mmsCount];
        for (int i = 0; i < situation->mmsCount; i++) msBackup[i] = situation->mixedSequences[i];
        int savedMSS = situation->mmsCount;
        RegMoveSequence* rmsBackup = new RegMoveSequence[situation->rmsCount];
        for (int i = 0; i < situation->rmsCount; i++) rmsBackup[i] = situation->regMoveSequences[i];
        int savedRMSS = situation->rmsCount;

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
        delete [] msBackup;*/
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
            else if (deepMoveAnalyzed.eval == topBorder) {
                if (analyzerDistribution(agen) < 500) {
                    //topBorder = deepMoveAnalyzed.eval;
                    //toReturn.eval = topBorder;
                    toReturn.regMoveSequence = rmsBackup[i];
                    toReturn.seqNumberToDo = 1;
                }
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
            else if (deepMoveAnalyzed.eval == topBorder) {
                if (analyzerDistribution(agen) < 500) {
                    //topBorder = deepMoveAnalyzed.eval;
                    //toReturn.eval = topBorder;
                    toReturn.takingSequence = tsBackup[i];
                    toReturn.seqNumberToDo = 2;
                }
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

#endif //CHECKERS_CLCENGINE_ANALYZER_H
