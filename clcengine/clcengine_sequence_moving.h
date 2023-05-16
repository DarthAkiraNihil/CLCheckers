//
// Created by EgrZver on 11.05.2023.
//

#include "clcengine_types.h"
#include "clcengine_moving.h"

#ifndef CHECKERS_CLCENGINE_SEQUENCE_MOVING_H
#define CHECKERS_CLCENGINE_SEQUENCE_MOVING_H

void makeATakingMoveSequence(GameSituation* situation, TakingSequence takingSequence) {
    for (int i = 0; i < takingSequence.tmsCount; i++) makeATakingMove(situation, takingSequence.takingMoves[i]);
}



void cancelATakingMoveSequence(GameSituation* situation, TakingSequence takingSequence) {
    for (int i = takingSequence.tmsCount - 1; i > -1; i--) cancelATakingMove(situation, takingSequence.takingMoves[i]);
}

/*
void makeAMixedSequence(GameSituation* situation, MixedSequence mixedSequence) {
    makeAMove(situation, mixedSequence.kingBecomingMove);
    makeATakingMoveSequence(situation, mixedSequence.takingSequence);
}

void cancelAMixedSequence(GameSituation* situation, MixedSequence mixedSequence) {
    cancelATakingMoveSequence(situation, mixedSequence.takingSequence);
    cancelAMove(situation, mixedSequence.kingBecomingMove);
}
*/

#endif //CHECKERS_CLCENGINE_SEQUENCE_MOVING_H
