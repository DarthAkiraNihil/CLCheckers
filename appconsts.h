#include <windows.h>

#ifndef CHECKERS_APPCONSTS_H
#define CHECKERS_APPCONSTS_H

const int WINDOW_SIZE_LENGTH = 765;
const int WINDOW_SIZE_HEIGHT = 610;

const int buttonStartGameVsReal = 0;
const int buttonStartGameVsComp = 1;
const int buttonLoadGame = 2;
const int buttonSaveGame= 3;
const int buttonAbout = 4;
const int buttonGiveUp = 5;
const int buttonGameDraw = 6;
const int buttonHelp = 7;
const int buttonRules = 8;

const int boardPasteX = 20;
const int boardPasteY = 20;

const int moveMakingDelay = 500;


char APPLICATION_MAIN_CLASS_NAME[] = "CLCMain";
const char APPLICATION_TITLE[] = "CLCheckers by TheSwagVader";

const char* textures[22] = {
    "resources\\emptyBlack.bmp",
    "resources\\emptyWhite.bmp",
    "resources\\regBlack.bmp",
    "resources\\regWhite.bmp",
    "resources\\kingBlack.bmp",
    "resources\\kingWhite.bmp",
    "resources\\emptyToMove.bmp",
    "resources\\regBlackVictim.bmp",
    "resources\\regWhiteVictim.bmp",
    "resources\\kingBlackVictim.bmp",
    "resources\\kingWhiteVictim.bmp",
    "resources\\emptyBlackSelected.bmp",
    "resources\\emptyWhiteSelected.bmp",
    "resources\\regBlackSelected.bmp",
    "resources\\regWhiteSelected.bmp",
    "resources\\kingBlackSelected.bmp",
    "resources\\kingWhiteSelected.bmp",
    "resources\\emptyToMoveSelected.bmp",
    "resources\\regBlackVictimSelected.bmp",
    "resources\\regWhiteVictimSelected.bmp",
    "resources\\kingBlackVictimSelected.bmp",
    "resources\\kingWhiteVictimSelected.bmp"
};

#endif //CHECKERS_APPCONSTS_H
