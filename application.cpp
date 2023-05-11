#include <cstring>
#include <memory.h>
//#include "conio2.h"
#include "interface.h"
#include "clcengine/clcengine_main.h"
#include "appconsts.h"
#include "resources/resources.h"
#include "defs.h"
#include "gamemanager.h"
#include "graphic_subsystem.h"


Game game;
HWND buttons[10], difficultySelect;
Coordinates boardCursor = {0, 5}, selectedSource, selectedDestination;
int numberOfMoveInTakingSequence = 0;
bool foundRegular = false, foundMixed = false, newKingAppeared = false, lockIsSet = false;
bool findMoves = false;
#define enableCP1251 SetConsoleCP(1251); SetConsoleOutputCP(1251)

#define UPDATE_RENDER renderBoard(&game.situation.board, game.situation.playerSide, handler, boardCursor, boardPasteX, boardPasteY); renderPathMapMarkers(&game.situation.board, game.situation.playerSide, handler, boardCursor, boardPasteX, boardPasteY)

inline bool in(int var, int leftBorder, int rightBorder) {
    return (var > leftBorder) && (var < rightBorder);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR args, int cmdShow) {
    player = Black;
    game = createANewGame(player, White, RvsC);
    game.situation.playerSide = player;
    //ascendChecker(&game.situation.board.checkers[Black][8]);
    updateBoardRender(&game.situation.board);
    WNDCLASSEX mainClass = newWindowClass((HBRUSH) COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), instance, LoadIcon(NULL, IDI_APPLICATION), applicationProcessor);
    Coordinates pasteCoordinates = getPasteWindowCoordinates();
    mainClass.lpszClassName = _TEXT(APPLICATION_MAIN_CLASS_NAME);
    mainClass.hIcon = (HICON) LoadImage(instance, MAKEINTRESOURCE(IDI_APP_BIG_ICON), IMAGE_ICON, 128, 128, 0);
    mainClass.hIconSm = (HICON) LoadImage(instance, MAKEINTRESOURCE(IDI_APP_WIN_ICON), IMAGE_ICON, 16, 16, 0);
    mainClass.hbrBackground = (HBRUSH) CreateSolidBrush(RGB(255, 255, 255));
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

    buttons[buttonTheFuck] = CreateWindowW(L"button", L"Играть против человека", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 540, 20, 140, 40, mainWindow, (HMENU)10000, instance, NULL);
    buttons[buttonTheHell] = CreateWindowW(L"button", L"Играть против компьютера", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 540, 80, 140, 40, mainWindow, (HMENU)10000, instance, NULL);
    buttons[buttonTheShit] = CreateWindowW(L"button", L"Загрузить игру", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 540, 140, 140, 40, mainWindow, (HMENU)10000, instance, NULL);
    buttons[buttonTheDamn] = CreateWindowW(L"button", L"Сохранить игру", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 540, 200, 140, 40, mainWindow, (HMENU)10000, instance, NULL);
    buttons[buttonTheIdiot] = CreateWindowW(L"button", L"О программе", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 540, 260, 140, 40, mainWindow, (HMENU)10000, instance, NULL);

    difficultySelect = CreateWindow(
        "combobox",
        "",
        WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | CBS_HASSTRINGS, 540, 300, 140, 50,  mainWindow, (HMENU) 60000, instance, nullptr
    );
    //LPCSTR fuck = "Dumbass\0";
    //SendMessage(difficultySelect, CB_ADDSTRING, 0,  (LPARAM) fuck);
    //howWindow(difficultySelect, 60000);
    //SendMessage(difficultySelect, CB_ADDSTRING, (WPARAM) 0,  (LPARAM) "Easy");
    SendMessage(difficultySelect, CB_ADDSTRING, 0,(LPARAM) (LPCSTR) "FUCK YOU");
    ShowWindow(mainWindow, SW_SHOWNORMAL);
    UpdateWindow(mainWindow);
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
            SendMessage(difficultySelect, CB_ADDSTRING, 0,(LPARAM) (LPCSTR) "FUCK YOU");

            break;
        }
        case WM_CREATE: {
            loadBoardTextures();
            boardBorder = loadImage("resources\\border.bmp");
            break;
        }
        case WM_COMMAND: {
            if (lParam == (LPARAM) buttons[buttonTheFuck]) {
                MessageBox(window, "Chego ty zhdesh?", "govnuk", 0);
                //PostQuitMessage(3221225477);
            }
            else if (lParam == (LPARAM) buttons[buttonTheHell]) {
                MessageBox(window, "really", "do it if you want", 0);
            }
            else if (lParam == (LPARAM) buttons[buttonTheShit]) {
                MessageBox(window, "And that's the fact!", "The truth", 0);
            }
            else if (lParam == (LPARAM) buttons[buttonTheDamn]) {
                MessageBox(window, "SUCK A DICK!", "OOOOO MA GAD", 0);
            }
            else if (lParam == (LPARAM) buttons[buttonTheIdiot]) {
                MessageBoxW(window, L"CLCheckers ver. 1.0.\nCLCheckers - Made in CLion\nАвтор: Егор \"TheSwagVader\" Зверев (github.com/TheSwagVader)\nРепозиторий проекта: github.com/TheSwagVader/CLCheckers\nПроект лицензирован Apache 2.0, License", L"О программе", 0);
            }
            break;
        }
        case WM_PAINT: {
            handler = BeginPaint(window, &paintStructure);
            renderBoardBorder(boardPasteX, boardPasteY, handler);
            renderBoard(&game.situation.board, player, handler, boardCursor, boardPasteX, boardPasteY);
            EndPaint(window, &paintStructure);
            break;
        }
        case WM_KEYDOWN : {
            switch (wParam) {
                case VK_UP: {
                    if (boardCursor.y > 0) {
                        boardCursor.y--;
                        UPDATE_RENDER;
                    }
                    break;
                }
                case VK_DOWN: {
                    if (boardCursor.y < 7) {
                        boardCursor.y++;
                        UPDATE_RENDER;
                    }
                    break;
                }
                case VK_LEFT: {
                    if (boardCursor.x > 0) {
                        boardCursor.x--;
                        UPDATE_RENDER;
                    }
                    break;
                }
                case VK_RIGHT: {
                    if (boardCursor.x < 7) {
                        boardCursor.x++;
                        UPDATE_RENDER;
                    }
                    break;

                }
                case VK_RETURN: {
                    Coordinates converted = cursorToBoardCoord(boardCursor, game.situation.playerSide);
                    PathMapMarkers marker = game.situation.board.pathMap[converted.y][converted.x];

                    if (!movesHaveBeenFound) {
                        flushSequenceLists(&game.situation);
                        findAllMoves(&game.situation, player);
                        copyLevelOneMovesToBuffers(&game.situation);
                        movesHaveBeenFound = true;
                    }
                    if (marker == Destination) {
                        selectedDestination = converted;
                        if (game.situation.tmBufferLen != 0) {
                            TakingMove extracted;
                            for (int i = 0; i < game.situation.tmBufferLen; i++) {
                                Coordinates src = game.situation.takingMovesBuffer[i].source,
                                        dest = game.situation.takingMovesBuffer[i].destination;
                                if (isCoordinatesEqual(selectedSource, src) && isCoordinatesEqual(selectedDestination, dest)) {
                                    extracted = game.situation.takingMovesBuffer[i];
                                    break;
                                }
                            }
                            resetPathMap(&game.situation.board);
                            flushMoveBuffers(&game.situation);
                            makeATakingMove(&game.situation, extracted);
                            game.situation.board.pathMap[extracted.victim.y][extracted.victim.x] = NoMove;
                            updateBoardRender(&game.situation.board);
                        }
                        else {
                            Move extracted;
                            for (int i = 0; i < game.situation.rmBufferLen; i++) {
                                Coordinates src = game.situation.regularMovesBuffer[i].source,
                                            dest = game.situation.regularMovesBuffer[i].destination;
                                if (isCoordinatesEqual(selectedSource, src) && isCoordinatesEqual(selectedDestination, dest)) {
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
                    }
                    else {
                        selectedSource = converted;
                        resetPathMap(&game.situation.board);
                        fillPathMap(&game.situation, converted);

                    }

                    UPDATE_RENDER;
                    if (moveHasBeenMade) {
                        if (game.type == RvsC) {
                            flushSequenceLists(&game.situation);
                            SeqContainer bestMove = getBestMove(game.situation, negateColor(player), Normal);
                            Sleep(moveMakingDelay / 5);
                            switch (bestMove.seqNumberToDo) {
                                case 1: {
                                    makeARegMoveSequenceWithDelay(&game.situation, bestMove.regMoveSequence, moveMakingDelay, handler);
                                    break;
                                }
                                case 2: {
                                    makeATakingSequenceWithDelay(&game.situation, bestMove.takingSequence, moveMakingDelay, handler);
                                    break;
                                }
                                case 3: {
                                    makeAMixedSequenceWithDelay(&game.situation, bestMove.mixedSequence, moveMakingDelay, handler);
                                    break;
                                }
                            }
                            removeMarkedForDeath(&game.situation, player);
                            updateBoardRender(&game.situation.board);
                            flushSequenceLists(&game.situation);
                            flushBuffers();
                            moveHasBeenMade = false;
                            renderBoard(&game.situation.board, player, handler, boardCursor, boardPasteX, boardPasteY);
                        }
                        else {
                            player = negateColor(player);
                            moveHasBeenMade = false;
                        }

                    }
                    break;
                }
            }
            break;
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
