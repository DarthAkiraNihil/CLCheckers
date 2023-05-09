//
// Created by EgrZver on 08.04.2023.
//

#ifndef CHECKERS_CLCENGINE_STRUCTS_H
#define CHECKERS_CLCENGINE_STRUCTS_H

enum BoardCellState {EMPTY_BLACK, EMPTY_WHITE, REG_BLACK, REG_WHITE, KING_BLACK, KING_WHITE};
enum PathMapMarkers {NoMove, Source, Destination, VictimRB, VictimRW, VictimKB, VictimKW, MovingLock};
enum Color {Black, White};
enum CheckerType {Regular, King};
enum GameState {PlayerMove, EnemyMoveReal, EnemyMoveComputer, BlackWon, WhiteWon};
enum GameType {RvsR, RvsC}; //RvsR = Real versus Real, RvsC = Real vs Computer
enum Difficulty {Dumbass = 2, Easy, Normal, Hard = 6, Insane, Extreme, Diabolic, Invincible = 11};
// may delete last two


const int regularCheckerCost = 100;
const int kingCheckerCost = 250;
const bool ENABLED_EMULATION = true;

const float BLACK_VICTORY = 100000.0;
const float WHITE_VICTORY = -100000.0;

const float qualityFactor[8][4] = {
        {1.2, 1.2, 1.2, 1.2},
        {1.15, 1.2, 1.2, 1.15},
        {1.15, 1.2, 1.2, 1.13},
        {1.0, 1.2, 1.15, 1.0},
        {1.0, 1.2, 1.2, 1.0},
        {1.0, 1.0, 1.0, 1.0},
        {1.0, 1.0, 1.0, 1.0},
        {1.0, 1.0, 1.0, 1.0},
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
    PathMapMarkers pathMap[8][8];
    Checker checkers[2][12];
    int checkersCount[2];
};

struct GameSituation {
    Board board;
    Color playerSide;
    Move regularMovesBuffer[24];
    int tmsCount, rmsCount, mmsCount;
    TakingMove takingMovesBuffer[24];
    int tmBufferLen, rmBufferLen;
    TakingSequence takingSequences[16];
    MixedSequence mixedSequences[16];
    RegMoveSequence regMoveSequences[32];
};

struct SeqContainer {
    RegMoveSequence regMoveSequence;
    TakingSequence takingSequence;
    MixedSequence mixedSequence;
    int seqNumberToDo;
    float eval;
};

struct Game {
    GameSituation situation;
    GameState state;
    GameType type;
};

#endif //CHECKERS_CLCENGINE_STRUCTS_H
