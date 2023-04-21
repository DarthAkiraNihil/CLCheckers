#ifndef CHECKERS_APPCONSTS_H
#define CHECKERS_APPCONSTS_H

const int WINDOW_SIZE_LENGTH = 700;
const int WINDOW_SIZE_HEIGHT = 700;

const int buttonTheFuck = 0;
const int buttonTheHell = 1;
const int buttonTheShit = 2;
const int buttonTheDamn = 3;
const int buttonTheIdiot = 4;


TCHAR APPLICATION_MAIN_CLASS_NAME[] = "CLCMain";
const char APPLICATION_TITLE[] = "CLCheckers by TheSwagVader";

const char* mainMenuPoints[36] = {
        "������ ����� ����",
        "��������� ����",
        "������� ����",
        "����������",
        "����� �� ����"
};
const char* mainMenuPointsDescriptions[50] = {
        "������� ������ ��������� ������ ��� ����������",
        "���������� ���������� ����� ����, �������� �",
        "�������� � �������� ����",
        "������ ����������",
        "��� �������?"
};

/*const char *emptyBlackTexture = "resources\\emptyBlack.bmp";
const char *emptyWhiteTexture = "resources\\emptyWhite.bmp";
const char *regBlackTexture = "resources\\regBlack.bmp";
const char *regWhiteTexture = "resources\\regWhite.bmp";
const char *kingBlackTexture = "resources\\kingBlack.bmp";
const char *kingWhiteTexture = "resources\\kingWhite.bmp";*/
const char* textures[7] = {
    "resources\\emptyBlack.bmp",
    "resources\\emptyWhite.bmp",
    "resources\\regBlack.bmp",
    "resources\\regWhite.bmp",
    "resources\\kingBlack.bmp",
    "resources\\kingWhite.bmp",
    "resources\\emptyToMove.bmp"
};
/*const char* texturePaths[50] = {
    "resource/emptyBlack.bmp",
    "resource/emptyWhite.bmp",
    "resource/regBlack.bmp",
    "resource/regWhite.bmp",
    "resource/kingBlack.bmp",
    "resource/kingWhite.bmp"
};*/
#endif //CHECKERS_APPCONSTS_H
