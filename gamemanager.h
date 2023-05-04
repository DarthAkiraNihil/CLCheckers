//
// Created by Егор on 04.05.2023.
//
#include "clcengine/clcengine_main.h"


#ifndef CHECKERS_GAMEMANAGER_H
#define CHECKERS_GAMEMANAGER_H

Game currentGame; Color player; bool movesHaveBeenFound = false;
Coordinates lastChosen;

RegMoveSequence rmsForChosen[20]; TakingSequence tsForChosen[20]; MixedSequence msForChosen[20];
int sizeOfRMSFC = 0, sizeOfTSFC = 0, sizeOfMSFC = 0;
Coordinates coordinatesOfAvailableMoves[20]; int sizeOfCOAM = 0;

bool isCoordinatesEqual(Coordinates arg1, Coordinates arg2) {
    return arg1.x == arg2.x && arg1.y == arg2.y;
}

void addRMSToBuffer(RegMoveSequence* buffer, RegMoveSequence sequence) {
    buffer[sizeOfRMSFC++] = sequence;
}

void addTSToBuffer(TakingSequence* buffer, TakingSequence sequence) {
    buffer[sizeOfTSFC++] = sequence;
}

void addMSToBuffer(MixedSequence* buffer, MixedSequence sequence) {
    buffer[sizeOfMSFC++] = sequence;
}

void flushBuffers() {
    sizeOfRMSFC = 0;
    sizeOfTSFC = 0;
    sizeOfMSFC = 0;
}

void addAnAvailableCoordinate(Coordinates* buffer, Coordinates what) {
    buffer[sizeOfCOAM++] = what;
}

void flushCOAMBuffer() {
    sizeOfCOAM = 0;
}

bool isAvailable(Coordinates* buffer, Coordinates what) {
    for (int i = 0; i < sizeOfCOAM; i++) {
        if (buffer[i].x == what.x && buffer[i].y == what.y) return true;
    }
    return false;
}

bool destinationInRMSBuffer(RegMoveSequence* buffer, Coordinates destination) {
    for (int i = 0; i < sizeOfRMSFC; i++) {
        if (isCoordinatesEqual(buffer[i].regularMoves[0].destination, destination)) return true;
    }
    return false;
}

int getAvailableIndex(Coordinates* buffer, Coordinates what) {
    for (int i = 0; i < sizeOfCOAM; i++) {
        if (isCoordinatesEqual(buffer[i], what)) return i;
    }
    return -1;
}

Move extractMoveFromBuffer(RegMoveSequence* buffer, Coordinates destination) {
    for (int i = 0; i < sizeOfRMSFC; i++) {
        if (isCoordinatesEqual(buffer[i].regularMoves[0].destination, destination)) return buffer[i].regularMoves[0];
    }
}
#endif //CHECKERS_GAMEMANAGER_H
