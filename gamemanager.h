//
// Created by Егор on 04.05.2023.
//
#include "clcengine/clcengine.h"


#ifndef CHECKERS_GAMEMANAGER_H
#define CHECKERS_GAMEMANAGER_H

Game currentGame; Color player; bool movesHaveBeenFound = false, moveHasBeenMade = false;
int numberOfCurrentTSMove = 0;
Coordinates lastChosen;

RegMoveSequence rmsForChosen[20]; TakingSequence tsForChosen[20];// MixedSequence msForChosen[20];
int sizeOfRMSFC = 0, sizeOfTSFC = 0, sizeOfMSFC = 0;

Coordinates coordinatesOfAvailableMoves[20]; int sizeOfCOAM = 0;


//добавь карту путей как рендер доски но только с доступнымы для хода полями и жертвы
void flushBuffers() {
    sizeOfRMSFC = 0;
    sizeOfTSFC = 0;
    sizeOfMSFC = 0;
}



bool destinationInRMSBuffer(RegMoveSequence* buffer, Coordinates destination) {
    for (int i = 0; i < sizeOfRMSFC; i++) {
        if (isCoordinatesEqual(buffer[i].regularMoves[0].destination, destination)) return true;
    }
    return false;
}

bool destinationInTSBuffer(TakingSequence* buffer, Coordinates destination, int seqMove) {
    for (int i = 0; i < sizeOfTSFC; i++) {
        if (isCoordinatesEqual(buffer[i].takingMoves[seqMove].destination, destination)) return true;
    }
    return false;
}



#endif //CHECKERS_GAMEMANAGER_H
