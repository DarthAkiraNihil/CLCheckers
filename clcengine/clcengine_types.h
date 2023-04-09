//
// Created by EgrZver on 08.04.2023.
//

#ifndef CHECKERS_CLCENGINE_STRUCTS_H
#define CHECKERS_CLCENGINE_STRUCTS_H

enum BoardCellState {EMPTY_BLACK, EMPTY_WHITE, REG_BLACK, REG_WHITE, KING_BLACK, KING_WHITE};
enum Color {Black, White};
enum CheckerType {Regular, King};
enum GameState {PlayerMove, EnemyMoveReal, EnemyMoveComputer, BlackWon, WhiteWon};
enum GameType {RvsR, RvsC}; //RvsR = Real versus Real, RvsC = Real vs Computer
enum Difficulty {Easy = 2, Normal = 3, Hard = 4, Insane = 5, Extreme = 6, Diabolic = 8};
// may delete last two


const int regularCheckerCost = 100;
const int kingCheckerCost = 250;
bool ENABLED_EMULATION = true;

const int qualityFactor[8][8] = {
        {1200, 1200, 1200, 1200, 1200, 1200, 1200, 1200},
        {1150, 1150, 1150, 1150, 1150, 1150, 1150, 1150},
        {1150, 1150, 1150, 1150, 1150, 1150, 1150, 1150},
        {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000},
        {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000},
        {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000},
        {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000},
        {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000}
};

struct Coordinates {
    int x, y;
};

const Coordinates NORTH_EAST = {1, 1};
const Coordinates NORTH_WEST = {-1, 1};
const Coordinates SOUTH_EAST = {1, -1};
const Coordinates SOUTH_WEST = {-1, -1};

struct Checker {
    Coordinates coordinates;
    Color color;
    CheckerType type;
    bool markedForDeath;
};

struct Move {
    Coordinates source, destination;
    bool isKingMove, isKingBecomingMove;
    // tod remove king/taking types
};

struct TakingMove {
    Coordinates source, destination, victim;
    Color takingSide;
    CheckerType victimType;
    bool isASpecialMove;
};

struct RegMoveSequence {
    Move regularMoves[2];
    int rmsCount;
};

struct TakingSequence {
    TakingMove takingMoves[16];
    int tmsCount;
};

struct MixedSequence {
    Move kingBecomingMove;
    TakingSequence takingSequence;
};

struct Board {
    BoardCellState boardRender[8][8];
    Checker checkers[2][12];
    int checkersCount[2];
};

struct GameSituation {
    Board board;
    //int rmCount, krmCount, kbmCount, rtmCount, ktmCount;
    int rmCount, tmCount;
    Color playerSide;
    //Move regularMoves[32], kingRegularMoves[64], kingBecomingMoves[16];
    Move regularMoves[128];
    Move lastKingBecomingMove;
    //Move regularMoves[128], kingBecomingMoves[16];
    //TakingMove regularTakingMoves[64], kingTakingMoves[32];
    int tmsCount, rmsCount, mmsCount;
    TakingMove takingMoves[100];
    TakingSequence lastTakingSequence, takingSequences[16];
    MixedSequence mixedSequences[16];
    RegMoveSequence regMoveSequences[32];
};

struct MoveContainer {
    Move move;
    TakingMove takingMove;
    TakingSequence takingSequence;
    int whatToTake;
};

struct Game {
    GameSituation situation;
    GameState state;
    GameType type;
};

#endif //CHECKERS_CLCENGINE_STRUCTS_H
