#ifndef CHECKERS_APPCONSTS_H
#define CHECKERS_APPCONSTS_H

const int WINDOW_SIZE_LENGTH = 600;
const int WINDOW_SIZE_HEIGHT = 600;

TCHAR APPLICATION_MAIN_CLASS_NAME[] = "CLCMain";
const char APPLICATION_TITLE[] = "CLCheckers by TheSwagVader";

const char* mainMenuPoints[36] = {
        "Начать новую игру",
        "Загрузить игру",
        "Правила игры",
        "Статистика",
        "Выйти из игры"
};
const char* mainMenuPointsDescriptions[50] = {
        "Играйте против реального игрока или компьютера",
        "Продолжите сохранённую ранее игру, загрузив её",
        "Почитать о правилах игры",
        "Узнать статистику",
        "Уже уходите?"
};

#endif //CHECKERS_APPCONSTS_H
