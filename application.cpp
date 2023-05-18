#include <cstring>
#include <memory.h>
#include <tchar.h>
#include <ctime>
//#include <random>
#include <cassert>
#include <cstdlib>
#include <cstdio>
//#include <cstdio>
//#include "conio2.h"
//#include "interface.h"
#include "clcengine/clcengine.h"
#include "appconsts.h"
#include "resources/resources.h"
#include "defs.h"
#include "gamemanager.h"
#include "graphic_subsystem.h"


Game game;
HWND buttons[10], difficultySelect, sideSelectorBlack, sideSelectorWhite, sideSelectorCaption, difficultySelectCaption, whoMovesCaption;
bool isGameBegun = false; Difficulty computerDifficulty;
HBRUSH brushBG = NULL;
Coordinates boardCursor = {0, 0}, selectedSource, selectedDestination;

#define UPDATE_RENDER renderBoard(&game.situation.board, game.situation.playerSide, handler, boardCursor, boardPasteX, boardPasteY); renderPathMapMarkers(&game.situation.board, game.situation.playerSide, handler, boardCursor, boardPasteX, boardPasteY)

inline bool in(int var, int leftBorder, int rightBorder) {
    return (var > leftBorder) && (var < rightBorder);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR args, int cmdShow) {
    srand(time(nullptr));
    player = Black;
    //game = createANewGame(player, White, RvsC);
    //game.situation.playerSide = player;
    //ascendChecker(&game.situation.board.checkers[Black][8]);
    updateBoardRender(&game.situation.board);
    WNDCLASSEX mainClass = newWindowClass((HBRUSH) COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), instance, LoadIcon(NULL, IDI_APPLICATION), applicationProcessor);
    Coordinates pasteCoordinates = getPasteWindowCoordinates();
    mainClass.lpszClassName = _TEXT(APPLICATION_MAIN_CLASS_NAME);
    mainClass.hIcon = (HICON) LoadImage(instance, MAKEINTRESOURCE(IDI_APP_BIG_ICON), IMAGE_ICON, 128, 128, 0);
    mainClass.hIconSm = (HICON) LoadImage(instance, MAKEINTRESOURCE(IDI_APP_WIN_ICON), IMAGE_ICON, 16, 16, 0);
    //mainClass.hbrBackground = (HBRUSH) CreateSolidBrush(RGB(255, 255, 255));
    if (!RegisterClassEx(&mainClass)) {return -1;}

    MSG applicationMSG = {0};
    gInstance = instance;

    HWND mainWindow = CreateWindowExA(
        0,
        mainClass.lpszClassName,
        APPLICATION_TITLE,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE,
        pasteCoordinates.x,
        pasteCoordinates.y,
        WINDOW_SIZE_LENGTH,
        WINDOW_SIZE_HEIGHT,
        NULL,
        NULL,
        NULL,
        NULL
    );

    buttons[buttonStartGameVsReal] = CreateWindowW(L"button", L"Играть против человека", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 540, 20, 200, 40, mainWindow, (HMENU)10000, instance, NULL);
    buttons[buttonStartGameVsComp] = CreateWindowW(L"button", L"Играть против компьютера", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 540, 70, 200, 40, mainWindow, (HMENU)10000, instance, NULL);
    buttons[buttonLoadGame] = CreateWindowW(L"button", L"Загрузить игру", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 540, 220, 200, 40, mainWindow, (HMENU)10000, instance, NULL);
    buttons[buttonSaveGame] = CreateWindowW(L"button", L"Сохранить игру", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 540, 270, 200, 40, mainWindow, (HMENU)10000, instance, NULL);
    buttons[buttonAbout] = CreateWindowW(L"button", L"О программе", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 540, 320, 200, 40, mainWindow, (HMENU)10000, instance, NULL);
    buttons[buttonGiveUp] = CreateWindowW(L"button", L"Сдаться", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 540, 370, 200, 40, mainWindow, (HMENU)10000, instance, NULL);
    buttons[buttonGameDraw] = CreateWindowW(L"button", L"Предложить ничью", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 540, 420, 200, 40, mainWindow, (HMENU)10000, instance, NULL);
    buttons[buttonHelp] = CreateWindowW(L"button", L"Как ходить?", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 540, 470, 200, 40, mainWindow, (HMENU)10000, instance, NULL);
    buttons[buttonRules] = CreateWindowW(L"button", L"Правила игры", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 540, 520, 200, 40, mainWindow, (HMENU)10000, instance, NULL);
    //buttons[buttonAbout] = CreateWindowW(L"button", L"О программе", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 540, 320, 200, 40, mainWindow, (HMENU)10000, instance, NULL);

    difficultySelect = CreateWindowW(
        L"combobox",
        L"",
        WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | CBS_HASSTRINGS | CBS_AUTOHSCROLL,
        540,
        140,
        200,
        200,
        mainWindow,
        (HMENU) 60000,
        instance,
        nullptr
    );

    //LPCSTR fuck = "Dumbass\0";
    //SendMessage(difficultySelect, CB_ADDSTRING, 0,  (LPARAM) fuck);
    //howWindow(difficultySelect, 60000);
    //SendMessage(difficultySelect, CB_ADDSTRING, (WPARAM) 0,  (LPARAM) "Easy");
    SendMessageW(difficultySelect, CB_ADDSTRING, 0, (LPARAM) (LPCWSTR) L"Придурок");
    SendMessageW(difficultySelect, CB_ADDSTRING, 0, (LPARAM) (LPCWSTR) L"Легко");
    SendMessageW(difficultySelect, CB_ADDSTRING, 0, (LPARAM) (LPCWSTR) L"Нормально");
    SendMessageW(difficultySelect, CB_ADDSTRING, 0, (LPARAM) (LPCWSTR) L"Сложно");
    SendMessageW(difficultySelect, CB_ADDSTRING, 0, (LPARAM) (LPCWSTR) L"Безумно");
    SendMessageW(difficultySelect, CB_ADDSTRING, 0, (LPARAM) (LPCWSTR) L"Экстремально");
    SendMessageW(difficultySelect, CB_SETCURSEL, 0, 0);

    sideSelectorCaption = CreateWindowW(L"static", L"Уровень сложности", WS_CHILD | WS_VISIBLE, 540, 120, 200, 20, mainWindow, (HMENU) 10000, instance, nullptr);
    sideSelectorCaption = CreateWindowW(L"static", L"Ваша сторона", WS_CHILD | WS_VISIBLE, 540, 170, 200, 20, mainWindow, (HMENU) 10000, instance, nullptr);
    sideSelectorBlack = CreateWindowW(L"button", L"Чёрные", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTORADIOBUTTON, 540, 190, 80, 20, mainWindow, (HMENU) 12345, instance, NULL);
    sideSelectorWhite = CreateWindowW(L"button", L"Белые", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTORADIOBUTTON, 620, 190, 100, 20, mainWindow, (HMENU)10000, instance, NULL);
    whoMovesCaption = CreateWindowW(L"static", L"Никто не ходит", WS_CHILD | WS_VISIBLE, 20, 540, 400, 20, mainWindow, (HMENU)10000, instance, NULL);
    //SendMessageW(whoMovesCaption, WM_SETTEXT, 0, (LPARAM) (LPCWSTR) L"P E N I S");
    /*for (int i = 0; i < 6; i++) {
        SendMessageW(difficultySelect, CB_ADDSTRING, 0, (LPARAM) difficultyNames[i]);
    }*/
    //SendMessage(difficultySelect, CB_ADDSTRING, 0, (LPARAM) (LPCSTR) difficultyNames[1]);
    //SendMessage(difficultySelect, CB_ADDSTRING, 0,(LPARAM) (LPCSTR) "FUCK YOU");
    ShowWindow(mainWindow, SW_SHOWNORMAL);
    UpdateWindow(mainWindow);
    //SendMessageW(fuck, WM_CTLCOLORBTN, (WPARAM) fuck2, 0);
    while (GetMessage(&applicationMSG, NULL, NULL, NULL)) {
        TranslateMessage(&applicationMSG);
        DispatchMessageA(&applicationMSG);
    }
}

WNDCLASSEX newWindowClass(HBRUSH backgroundColor, HCURSOR cursor, HINSTANCE instance, HICON icon/*, LPCWSTR name*/, WNDPROC procedureToCall) {
    WNDCLASSEX newClass;
    memset(&newClass, 0, sizeof(newClass));
    newClass.cbSize = sizeof(WNDCLASSEX);
    newClass.hCursor = cursor;
    newClass.hInstance = instance;
    newClass.hbrBackground = backgroundColor;
    newClass.lpfnWndProc = procedureToCall;
    return newClass;
}


LRESULT CALLBACK applicationProcessor(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT  paintStructure;
    HDC handler;
    handler = GetDC(window);
    switch (message) {
        case WM_INITDIALOG: {
            //TCHAR strToAdd[40]=_T("ABC");
            //SendMessage(difficultySelect, CB_ADDSTRING, 0,(LPARAM) (LPCSTR) "FUCK YOU");

            break;
        }
        case WM_CREATE: {
            loadBoardTextures();
            boardBorder = loadImage("resources\\border.bmp");
            defineOpenFile(window);
            break;
        }
        case WM_COMMAND: {
            if (lParam == (LPARAM) buttons[buttonStartGameVsComp]) {
                if (!isGameBegun) {
                    TCHAR buffer[256];
                    Color firstMove;
                    int selected = (int) SendMessageW(difficultySelect, CB_GETCURSEL, 0, 0);
                    float rsc = (float) rand() / (float) RAND_MAX;
                    bool blackChosen = SendMessage(sideSelectorBlack, BM_GETCHECK, 0, 0) == BST_CHECKED,
                        whiteChosen = SendMessage(sideSelectorWhite, BM_GETCHECK, 0, 0) == BST_CHECKED;
                    if (blackChosen) player = Black;
                    else if (whiteChosen) player = White;
                    //else MessageBoxW(nullptr, L"Вы не выбрали, за кого играть", L"Saatana vittu perkele", 0);
                    if (blackChosen || whiteChosen) {//SendMessageW(window, WM_COMMAND, 0, 100);;
                        if (rsc < 0.5) firstMove = Black; else firstMove = White;
                        //(TCHAR) SendMessageW(difficultySelect, CB_GETLBTEXT, (WPARAM) selected, (LPARAM) buffer);
                        //MessageBoxW(window, (LPCWSTR) buffer , L"govnuk", 0);
                        computerDifficulty = getDifficultyByNumber(selected);
                        //printf("%d", difficulty);
                        game = createANewGame(player, firstMove, RvsC);
                        //for (int i = 0; i < 10; i++) removeChecker(&game.situation.board, 0, negateColor(player));

                        isGameBegun = true;
                        sprintf(buffer, "%d %d %d", computerDifficulty, blackChosen, whiteChosen);
                        switch (computerDifficulty) {
                            case Dumbass: {
                                MessageBoxW(nullptr, L"\"Уга-буга? Буга-уга-уга-бука! Абуга!\"\n\n"
                                                     "Похоже, вы решили сразиться с самым неумелым игроком в шашки, что есть на белом свете. Что ж, ничего не могу сказать. Должно быть просто. Удачи :)\n\n"
                                                     "Но всё же не недооценивайте его, порой его ходы довольно опасны.", L"Противник: неандерталец Абуган Бунганов Уганович", MB_ICONINFORMATION);//SendMessageW(window, WM_COMMAND, 0, 100);
                                break;
                            }
                            case Easy: {
                                MessageBoxW(nullptr, L"\"Я только начал играть в шашки. Пока плохо получается.\"\n\n"
                                                     "Вы  выбрали не самого умелого игрока, однако он уже дальше смотрит на игровую ситуацию, потому его ходы немного опаснее, чем у предыдущего.\n\n"
                                                     "Однако уверен, его просто одолеть. Удачи :)", L"Противник: Третьеклассник Немагнус Карлсен", MB_ICONINFORMATION);
                                break;
                            }
                            case Normal: {
                                MessageBoxW(nullptr, L"\"Откуда ты? Из обычной школы? Хм, да ты никто по сравнению со мной. Я играй в шашки с -1 года, а потому уверен, что ты проиграешь\"\n\n"
                                                     "Хм, а вот это уже из категории умелых. Довольно зазнавшийся, однако его слова полностью подтверждаются его умением играть. Его ходы могут сыграть свою роль очень нескоро, однако его комбинаторное зрение развито значительно лучше.\n\n"
                                                     "Его непросто будет одолеть. И всё же, удачи! :)", L"Противник: Лицеист Титов Дмитро Полковников", MB_ICONINFORMATION);
                                break;
                            }
                            case Hard: {
                                MessageBoxW(nullptr, L"\"Наконец-то, достойный соперник. Наша схватка будет легендарной!\"\n\n"
                                                     "Он просидел в темнице более 10 лет, оттачивая мастерство игры в шашки. Потому он способен просчитывать всё далеко вперёд, дальше, чем предыдущий противник.\n\n"
                                                     "Будьте начеку, когда делаете ходы, ведь они могут с лёгкостью подставить вас в критическое положение.\n\n"
                                                     "Будьте осторожны! Удачи!", L"Противник: Тайлунг", MB_ICONWARNING);
                                break;
                            }
                            case Insane: {
                                MessageBoxW(nullptr, L"\"Ставлю душу своей матери на то, что я одолею тебя\"\n\n"
                                                     "Крайне опасный противник, который настолько уверен в себе, что вводит своих оппонентов в ступор. Поистине великолепно играет в шашки, и умеет смотреть далеко вперёд, просчитывая миллионы ходов за несколько секунд.\n\n"
                                                     "Не поддайтесь его невозмутимости, так как он этим воспользуется и одолеет вас в считанные минуты.\n\n"
                                                     "Удачи!", L"Противник: Джотаро Куджо", MB_ICONWARNING);
                                break;
                            }
                            case Extreme: {
                                MessageBoxW(nullptr, L"\"ХА-ХА-ХА-ХА! ТЫ ОПОЗДАЛ СОНИК! ТЕПЕРЬ Я СЕРТИФИЦИРОВАННАЯ ПРОГРАММА ДЛЯ ИГРЫ В РУССКИЕ ШАШКИ! БУАХА-ХА-ХА-ХА!\"\n\n"
                                                     "Вы выбрали самого опасного врага, хотя, говорят, есть и опаснее...\n\n"
                                                     "В любом случае, IQ Доктора равен 300 баллам, потому его ходы настолько гениальны, что вы ни за что их не просчитаете. Его комбинаторное зрение совершенно, оно не знает изъянов. Хотя сам доктор долго думает.\n\n"
                                                     "Он уже давно хотел вызвать кого-нибудь на поединок в шашки, потому ваша задача - одолеть его, ведь иначе он всех зверей превратит в бандиков.\n\n"
                                                     "Покажите ему, кто здесь истинный мастер игры в русские шашки!", L"Противник: Доктор Айво \"Эггман\" Роботник", MB_ICONSTOP);
                                break;
                            }
                        }
                        //MessageBoxA(nullptr, "S O S I   B L A C K   D I C K", buffer, 0);//SendMessageW(window, WM_COMMAND, 0, 100);
                        UPDATE_RENDER;
                        setWhoMovesCaption(whoMovesCaption, firstMove);
                        if (firstMove != player) {
                            flushSequenceLists(&game.situation);
                            SeqContainer bestMove = getBestMove(game.situation, negateColor(player), computerDifficulty);
                            Sleep(moveMakingDelay / 5);
                            switch (bestMove.seqNumberToDo) {
                                case 1: {
                                    makeARegMoveSequenceWithDelay(&game.situation, bestMove.regMoveSequence,
                                                                  moveMakingDelay, handler);
                                    break;
                                }
                                case 2: {
                                    makeATakingSequenceWithDelay(&game.situation, bestMove.takingSequence,
                                                                 moveMakingDelay, handler);
                                    break;
                                }
                                    /*case 3: {
                                        makeAMixedSequenceWithDelay(&game.situation, bestMove.mixedSequence,
                                                                    moveMakingDelay, handler);
                                        break;
                                    }*/
                            }
                            removeMarkedForDeath(&game.situation, player);
                            updateBoardRender(&game.situation.board);
                            flushSequenceLists(&game.situation);
                            flushBuffers();
                            UPDATE_RENDER;
                            setWhoMovesCaption(whoMovesCaption, player);
                        }
                    }
                    else MessageBoxW(nullptr, L"Вы не выбрали, за кого играть", L"Saatana vittu perkele", MB_ICONERROR);
                }
                else {
                    MessageBoxW(nullptr, L"Вы не можете начать новую игру, пока не закончите текущую!", L"Saatana vittu perkele", MB_ICONERROR);
                }
            }
            else if (lParam == (LPARAM) buttons[buttonStartGameVsReal]) {
                if (!isGameBegun) {
                    TCHAR buffer[256];
                    Color firstMove;
                    //int selected = (int) SendMessageW(difficultySelect, CB_GETCURSEL, 0, 0);
                    float rsc = (float) rand() / (float) RAND_MAX;
                    bool blackChosen = SendMessage(sideSelectorBlack, BM_GETCHECK, 0, 0) == BST_CHECKED,
                        whiteChosen = SendMessage(sideSelectorWhite, BM_GETCHECK, 0, 0) == BST_CHECKED;
                    if (blackChosen) player = Black;
                    else if (whiteChosen) player = White;
                    //else MessageBoxW(nullptr, L"Вы не выбрали, за кого играть", L"Saatana vittu perkele", 0);
                    if (blackChosen || whiteChosen) {//SendMessageW(window, WM_COMMAND, 0, 100);;
                        if (rsc < 0.5) firstMove = Black; else firstMove = White;
                        //(TCHAR) SendMessageW(difficultySelect, CB_GETLBTEXT, (WPARAM) selected, (LPARAM) buffer);
                        //MessageBoxW(window, (LPCWSTR) buffer , L"govnuk", 0);
                        //computerDifficulty = getDifficultyByNumber(selected);
                        //printf("%d", difficulty);
                        game = createANewGame(player, firstMove, RvsR);

                        isGameBegun = true;
                        sprintf(buffer, "%d %d %d", computerDifficulty, blackChosen, whiteChosen);
                        MessageBoxA(nullptr, "S O S I   B L A C K   D I C K", buffer, 0);//SendMessageW(window, WM_COMMAND, 0, 100);
                        UPDATE_RENDER;
                        setWhoMovesCaption(whoMovesCaption, firstMove);
                    }
                    else MessageBoxW(nullptr, L"Вы не выбрали, за кого играть", L"Saatana vittu perkele", MB_ICONERROR);
                    //game = createANewGame(Black, Black, RvsR);
                    //isGameBegun = true;
                    //SendMessageW(window, WM_COMMAND, 0, 100);
                    //UPDATE_RENDER;
                }
                else {
                    MessageBoxW(nullptr, L"Вы не можете начать новую игру, пока не закончите текущую!", L"Saatana vittu perkele", MB_ICONERROR);
                }

            }
            else if (lParam == (LPARAM) buttons[buttonLoadGame]) {
                //OPENFILENAME test;
                if (!isGameBegun) {
                    BOOL success = GetOpenFileNameW(&openFile);
                    if (success) {
                        MessageBoxW(window, fileName, L"The truth", 0);
                        FILE *load = _wfopen(openFile.lpstrFile, L"rb");
                        if (load != nullptr) {
                            fread(&computerDifficulty, sizeof(Difficulty), 1, load);
                            fread(&player, sizeof(Color), 1, load);
                            fread(&game, sizeof(Game), 1, load);
                            fclose(load);
                            isGameBegun = true;
                            UPDATE_RENDER;
                        } else
                            MessageBoxW(nullptr, L"Нет такого файла, дурачок", L"Saatana vittu perkele", MB_ICONERROR);
                    }
                }
                else MessageBoxW(nullptr, L"Вы что, решили просто взять, ливнуть из этой игры в другую?! Ну уж нет, я вам не позволю! Закончите сначала текущую игру.", L"Saatana vittu perkele", MB_ICONERROR);
            }
            else if (lParam == (LPARAM) buttons[buttonSaveGame]) {
                if (isGameBegun) {
                    BOOL success = GetSaveFileNameW(&openFile);
                    if (success) {
                        MessageBoxW(window, fileName, L"The truth", 0);
                        FILE *save = _wfopen(openFile.lpstrFile, L"wb+");

                        fwrite(&computerDifficulty, sizeof(Difficulty), 1, save);
                        fwrite(&player, sizeof(Color), 1, save);
                        fwrite(&game, sizeof(Game), 1, save);
                        fclose(save);
                    }
                }
                else MessageBoxW(nullptr, L"Как ты сохранишь игру, которой нет, а?!", L"Saatana vittu perkele", MB_ICONERROR);
                //MessageBox(window, "SUCK A DICK!", "OOOOO MA GAD", 0);
            }
            else if (lParam == (LPARAM) buttons[buttonAbout]) {
                MessageBoxW(window, L"CLCheckers ver. 1.0.\nCLCheckers - Made in CLion\nАвтор: Егор \"TheSwagVader\" Зверев (github.com/TheSwagVader)\nРепозиторий проекта: github.com/TheSwagVader/CLCheckers\nПроект лицензирован Apache 2.0, License", L"О программе", MB_ICONINFORMATION);
            }
            else if (lParam == (LPARAM) buttons[buttonGiveUp]) {
                if (isGameBegun) {
                    if (MessageBoxW(nullptr, L"Вы действительно хотите сдаться?", L"Одумайтесь!", MB_ICONQUESTION | MB_YESNO) == IDYES) {
                        if (player == game.situation.playerSide) {
                            MessageBoxW(window, L"YOU SUCK", L"Лошара ёбаный", MB_ICONINFORMATION);
                        }
                        else {
                            MessageBoxW(window, L"YOU ROCK", L"Победитель долбанный", MB_ICONINFORMATION);
                        }

                        renderEmptyBoard(handler, boardPasteX, boardPasteY);
                        isGameBegun = false;
                        moveHasBeenMade = false;
                        movesHaveBeenFound = false;
                    }
                }
                else {
                    MessageBoxW(nullptr, L"Ай-яй! Вы не можете сдаться, ведь у вас нет игры!", L"Saatana vittu perkele", MB_ICONERROR);
                }
            }
            else if (lParam == (LPARAM) buttons[buttonGameDraw]) {
                if (isGameBegun) {
                    if (MessageBoxW(nullptr, L"Вы предлагаете ничью. Ваш оппонент согласен на неё?", L"Предложение ничьи", MB_ICONQUESTION | MB_YESNO) == IDYES) {
                        MessageBoxW(nullptr, L"Партия закончилась ничьей! Победила дружба.", L"Ничьи", MB_ICONINFORMATION);
                        renderEmptyBoard(handler, boardPasteX, boardPasteY);
                        isGameBegun = false;
                        moveHasBeenMade = false;
                        movesHaveBeenFound = false;
                    }
                    else {
                        MessageBoxW(nullptr, L"Ваш оппонент не принял предложение ничьи :(", L"Предложение ничьи", MB_ICONWARNING);
                    }
                }
                else {
                    MessageBoxW(nullptr, L"Ай-яй! Вы не можете предложить ничью, ведь у вас нет игры!", L"Saatana vittu perkele", MB_ICONERROR);
                }
            }
            else if (lParam == (LPARAM) buttons[buttonHelp]) {
                MessageBoxW(window, L"Выбор шашки производится с помощью курсора, указанным на доске. Его перемещение осуществляется нажатием \"стрелочек\" на клавиатуре.\n\n"
                                    "Выбор шашки происходит через нажатие клавиши Enter. Если выбранной шашки есть доступные поля, к которым можно переместиться, они будут подсвечены зелёным цветом.\n\n"
                                    "Если одно из таких полей будет выбрано, также через нажатие Enter, будет сделал ход той шашкой, которую вы выбрали до этого. ", L"Как ходить", MB_ICONINFORMATION);
            }
            else if (lParam == (LPARAM) buttons[buttonRules]) {
                MessageBoxW(window, L"Правила:\n"
                                    "1) Простая шашка ходит по диагонали вперёд на одну клетку.\n"
                                    "2) Дамка ходит по диагонали на любое свободное поле как вперёд, так и назад.\n"
                                    "3) При выделении шашки ей необходимо сделать ход, за исключением случаев, когда шашка не имеет ходов.\n"
                                    "4) Взятие обязательно.\n"
                                    "5) Простая шашка, находящаяся рядом с шашкой соперника, за которой имеется свободное поле, переносится"
                                    " через эту шашку на это свободное поле. Если есть возможность продолжить взятие других шашек соперника,"
                                    " то это взятие продолжается, пока бьющая шашка не достигнет положения, из которого бой невозможен."
                                    " Взятие простой шашкой производится как вперёд, так и назад.\n"
                                    "6) Дамка бьёт по диагонали, как вперёд, так и назад, и становится на любое свободное поле после побитой шашки. \n"
                                    "   Аналогично, дамка может бить несколько фигур соперника и должна бить до тех пор, пока это возможно.\n"
                                    "7) При бое через дамочное поле простая шашка превращается в дамку и продолжает бой по правилам дамки.\n"
                                    "8) При нескольких вариантах взятия, например, одну шашку или две, игрок выбирает вариант взятия по своему усмотрению.\n"
                                    "9) Партия считается выигранной в следующих случаях: \n"
                                    "    - если у одного из соперников побиты все шашки;\n"
                                    "    - если один из участников заявил о том, что сдаётся;\n"
                                    "    - если шашки одного из участников заперты и он не может сделать очередной ход.\n"
                                    "10) Партия считается закончившейся вничью, если один из участников предлагает ничью, а другой её принимает.", L"Правила игры", MB_ICONINFORMATION);
            }
            break;
        }
        case WM_PAINT: {
            handler = BeginPaint(window, &paintStructure);
            renderBoardBorder(boardPasteX, boardPasteY, handler);
            renderEmptyBoard(handler, boardPasteX, boardPasteY);//renderBoard(&game.situation.board, player, handler, boardCursor, boardPasteX, boardPasteY);
            EndPaint(window, &paintStructure);
            break;
        }
        case WM_KEYDOWN : {
            //if (isGameBegun) {
                switch (wParam) {
                    case VK_UP: {
                        if (boardCursor.y > 0 && isGameBegun) {
                            boardCursor.y--;
                            UPDATE_RENDER;
                        }
                        break;
                    }
                    case VK_DOWN: {
                        if (boardCursor.y < 7 && isGameBegun) {
                            boardCursor.y++;
                            UPDATE_RENDER;
                        }
                        break;
                    }
                    case VK_LEFT: {
                        if (boardCursor.x > 0 && isGameBegun) {
                            boardCursor.x--;
                            UPDATE_RENDER;
                        }
                        break;
                    }
                    case VK_RIGHT: {
                        if (boardCursor.x < 7 && isGameBegun) {
                            boardCursor.x++;
                            UPDATE_RENDER;
                        }
                        break;

                    }
                    case VK_RETURN: {
                        if (isGameBegun) {
                            Coordinates converted = cursorToBoardCoord(boardCursor, game.situation.playerSide);
                            PathMapMarkers marker = game.situation.board.pathMap[converted.y][converted.x];

                            if (!movesHaveBeenFound) {
                                flushSequenceLists(&game.situation);
                                findAllMoves(&game.situation, player);
                                copyLevelOneMovesToBuffers(&game.situation);
                                movesHaveBeenFound = true;
                            }
                            /*if (lostByMoves(&game.situation)) {
                                MessageBoxW(window, L"YOU SUCK", L"Лошара ёбаный", MB_ICONINFORMATION);
                            }
                            else {

                            }*/
                            if (marker == Destination) {
                                selectedDestination = converted;
                                if (game.situation.tmBufferLen != 0) {
                                    TakingMove extracted;
                                    for (int i = 0; i < game.situation.tmBufferLen; i++) {
                                        Coordinates src = game.situation.takingMovesBuffer[i].source,
                                            dest = game.situation.takingMovesBuffer[i].destination;
                                        if (isCoordinatesEqual(selectedSource, src) &&
                                            isCoordinatesEqual(selectedDestination, dest)) {
                                            extracted = game.situation.takingMovesBuffer[i];
                                            break;
                                        }
                                    }
                                    resetPathMap(&game.situation.board);
                                    flushMoveBuffers(&game.situation);
                                    makeATakingMove(&game.situation, extracted);
                                    game.situation.board.pathMap[extracted.victim.y][extracted.victim.x] = NoMove;
                                    updateBoardRender(&game.situation.board);
                                } else {
                                    Move extracted;
                                    for (int i = 0; i < game.situation.rmBufferLen; i++) {
                                        Coordinates src = game.situation.regularMovesBuffer[i].source,
                                            dest = game.situation.regularMovesBuffer[i].destination;
                                        if (isCoordinatesEqual(selectedSource, src) &&
                                            isCoordinatesEqual(selectedDestination, dest)) {
                                            extracted = game.situation.regularMovesBuffer[i];
                                            break;
                                        }
                                    }
                                    resetPathMap(&game.situation.board);
                                    flushMoveBuffers(&game.situation);
                                    makeAMove(&game.situation, extracted);
                                    updateBoardRender(&game.situation.board);
                                }
                                if (game.situation.rmBufferLen + game.situation.tmBufferLen == 0) {
                                    moveHasBeenMade = true;
                                    movesHaveBeenFound = false;
                                    removeMarkedForDeath(&game.situation, negateColor(player));
                                    updateBoardRender(&game.situation.board);
                                    resetPathMap(&game.situation.board);
                                }
                            } else {
                                selectedSource = converted;
                                resetPathMap(&game.situation.board);
                                fillPathMap(&game.situation, converted);

                            }

                            UPDATE_RENDER;
                            if (moveHasBeenMade) {
                                setWhoMovesCaption(whoMovesCaption, negateColor(player));
                                flushSequenceLists(&game.situation);
                                findAllMoves(&game.situation, negateColor(player));
                                if (lostByMoves(&game.situation)) {
                                    MessageBoxW(window, L"YOU ROCK", L"Победитель долбанный", MB_ICONINFORMATION);
                                    renderEmptyBoard(handler, boardPasteX, boardPasteY);
                                    isGameBegun = false;
                                    moveHasBeenMade = false;
                                    movesHaveBeenFound = false;
                                }
                                else {
                                    if (game.type == RvsC) {
                                        flushSequenceLists(&game.situation);
                                        findAllMoves(&game.situation, negateColor(player));

                                        flushSequenceLists(&game.situation);
                                        SeqContainer bestMove = getBestMove(game.situation, negateColor(player), computerDifficulty);
                                        Sleep(moveMakingDelay / 5);
                                        switch (bestMove.seqNumberToDo) {
                                            case 1: {
                                                makeARegMoveSequenceWithDelay(&game.situation, bestMove.regMoveSequence,
                                                                              moveMakingDelay, handler);
                                                break;
                                            }
                                            case 2: {
                                                makeATakingSequenceWithDelay(&game.situation, bestMove.takingSequence,
                                                                             moveMakingDelay, handler);
                                                break;
                                            }
                                            default: {
                                                flushSequenceLists(&game.situation);
                                                findAllMoves(&game.situation, negateColor(player));
                                                float moveSector = (float) rand() / (float) RAND_MAX;
                                                if (game.situation.tmsCount != 0) {
                                                    if (moveSector < 0.5) {
                                                        int moveIndex = (int) ((float) rand() / (float) RAND_MAX) * game.situation.rmsCount;
                                                        makeARegMoveSequenceWithDelay(&game.situation, game.situation.regMoveSequences[moveIndex],
                                                                                      moveMakingDelay, handler);
                                                    }
                                                    else {
                                                        int moveIndex = (int) ((float) rand() / (float) RAND_MAX) * game.situation.tmsCount;
                                                        makeATakingSequenceWithDelay(&game.situation, game.situation.takingSequences[moveIndex],
                                                                                     moveMakingDelay, handler);
                                                    }
                                                }
                                                else {
                                                    int moveIndex = (int) ((float) rand() / (float) RAND_MAX) * game.situation.rmsCount;
                                                    makeARegMoveSequenceWithDelay(&game.situation, game.situation.regMoveSequences[moveIndex],
                                                                                  moveMakingDelay, handler);
                                                }
                                                break;
                                            }
                                                /*case 3: {
                                                    makeAMixedSequenceWithDelay(&game.situation, bestMove.mixedSequence,
                                                                                moveMakingDelay, handler);
                                                    break;
                                                }*/
                                        }
                                        removeMarkedForDeath(&game.situation, player);
                                        updateBoardRender(&game.situation.board);
                                        flushSequenceLists(&game.situation);
                                        flushBuffers();
                                        moveHasBeenMade = false;
                                        renderBoard(&game.situation.board, player, handler, boardCursor, boardPasteX,
                                                    boardPasteY);
                                        findAllMoves(&game.situation, player);
                                        if (lostByMoves(&game.situation)) {
                                            MessageBoxW(window, L"YOU SUCK", L"Лошара ёбаный", MB_ICONINFORMATION);
                                            renderEmptyBoard(handler, boardPasteX, boardPasteY);
                                            isGameBegun = false;
                                            moveHasBeenMade = false;
                                            movesHaveBeenFound = false;
                                        }
                                        flushSequenceLists(&game.situation);
                                        setWhoMovesCaption(whoMovesCaption, player);
                                    } else {
                                        player = negateColor(player);
                                        moveHasBeenMade = false;
                                        flushSequenceLists(&game.situation);
                                        findAllMoves(&game.situation, player);
                                        if (lostByMoves(&game.situation)) {
                                            MessageBoxW(window, L"YOU SUCK", L"Лошара ёбаный", MB_ICONINFORMATION);
                                            renderEmptyBoard(handler, boardPasteX, boardPasteY);
                                            isGameBegun = false;
                                            moveHasBeenMade = false;
                                            movesHaveBeenFound = false;
                                        }
                                        flushSequenceLists(&game.situation);
                                        setWhoMovesCaption(whoMovesCaption, player);
                                    }
                                }
                            }
                        }
                        break;
                    }
                }
            break;
        }
        case WM_CTLCOLORBTN: {
            HDC staticHDC = (HDC) wParam;
            SetBkColor(staticHDC, RGB(255, 255, 255));
            if (brushBG == nullptr) brushBG = CreateSolidBrush((RGB(255, 255, 255)));
            return (INT_PTR) brushBG;
        }
        case WM_DESTROY : {
            PostQuitMessage(0);
            return 0;
        }
        case WM_CLOSE: {
            DestroyWindow(window);
        }
        default: {
            return DefWindowProc(window, message, wParam, lParam);
        }
    }
    ReleaseDC(window, handler);
}

Difficulty getDifficultyByNumber(int number) {
    switch (number) {
        case 0: return Dumbass;
        case 1: return Easy;
        case 2: return Normal;
        case 3: return Hard;
        case 4: return Insane;
        case 5: return Extreme;
        case 6: return Diabolic;
        case 7: return Invincible;
        default: return Dumbass;
    }
}

void defineOpenFile(HWND ofWindow) {
    memset(&openFile, 0, sizeof(openFile));
    openFile.lStructSize = sizeof(openFile);
    openFile.hwndOwner = ofWindow;
    openFile.nMaxFile = sizeof(fileName);
    openFile.lpstrFile = fileName;
    openFile.lpstrDefExt = L"sav";
    openFile.lpstrFilter = L"Файлы сохранений (.sav)\0*.sav\0";
    //openFile.nFilterIndex = 1;
}
void setWhoMovesCaption(HWND caption, Color color) {
    if (color == Black) {
        SendMessageW(caption, WM_SETTEXT, 0, (LPARAM) (LPCWSTR) L"Ход чёрных");
    } else {
        SendMessageW(caption, WM_SETTEXT, 0, (LPARAM) (LPCWSTR) L"Ход белых");
    }
}