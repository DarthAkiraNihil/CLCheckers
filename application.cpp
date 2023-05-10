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
HWND buttons[10];
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

    buttons[buttonTheFuck] = CreateWindow("button", "Pososi jopu", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 540, 20, 140, 40, mainWindow, (HMENU)10000, instance, NULL);
    buttons[buttonTheHell] = CreateWindow("button", "DIE!", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 540, 80, 140, 40, mainWindow, (HMENU)10000, instance, NULL);
    buttons[buttonTheShit] = CreateWindow("button", "Axol x Melony 4eva", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 540, 140, 140, 40, mainWindow, (HMENU)10000, instance, NULL);
    buttons[buttonTheDamn] = CreateWindow("button", "SUCK A DICK!", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 540, 200, 140, 40, mainWindow, (HMENU)10000, instance, NULL);
    buttons[buttonTheIdiot] = CreateWindow("button", "zamn", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 540, 260, 140, 40, mainWindow, (HMENU)10000, instance, NULL);




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
                MessageBox(window, "no bitches?", "What a loh", 0);
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
                    //MessageBox(window, "J O P A", "J O P A", 0);
                    if (boardCursor.y > 0) boardCursor.y--; else boardCursor.y = 7;
                    UPDATE_RENDER;
                    break;
                }
                case VK_DOWN: {
                    //MessageBox(window, "Fcking task", "Holy shit", 0);
                    if (boardCursor.y < 7) boardCursor.y++; else boardCursor.y = 0;
                    UPDATE_RENDER;
                    break;
                }
                case VK_LEFT: {
                    //MessageBox(window, "I AM NOW MESSAGEBOX CERTIFIED", "UR TOO LATE SONIC!", 0);
                    if (boardCursor.x > 0) boardCursor.x--; else boardCursor.x = 7;
                    UPDATE_RENDER;
                    break;
                }
                case VK_RIGHT: {
                    //MessageBox(window, "I now am too message box certified", "HAHA, I TOO AM LATE", 0);
                    if (boardCursor.x < 7) boardCursor.x++; else boardCursor.x = 0;
                    UPDATE_RENDER;
                    break;

                }case VK_RETURN: {
                    GameSituation cpy1 = game.situation;
                    Coordinates converted = cursorToBoardCoord(boardCursor, game.situation.playerSide);
                    PathMapMarkers marker = game.situation.board.pathMap[converted.y][converted.x];

                    if (!movesHaveBeenFound) {
                        findAllMoves(&game.situation, player);
                        movesHaveBeenFound = true;
                    }
                    if (lockIsSet) {
                        if (marker == MovingLock) {
                            selectedSource = converted;
                            resetPathMap(&game.situation.board);
                            fillPathMapTakingMoves(&game.situation, converted, numberOfMoveInTakingSequence);
                        }
                        else if (marker == Destination) {
                            selectedDestination = converted;
                            TakingMove takingMove;
                            for (int i = 0; i < game.situation.tmsCount; i++) {
                                Coordinates src = game.situation.takingSequences[i].takingMoves[numberOfMoveInTakingSequence].source,
                                    dest = game.situation.takingSequences[i].takingMoves[numberOfMoveInTakingSequence].destination;
                                if (isCoordinatesEqual(src, selectedSource) && isCoordinatesEqual(dest, selectedDestination)) {
                                    takingMove = game.situation.takingSequences[i].takingMoves[numberOfMoveInTakingSequence];
                                    break;
                                }
                            }
                            makeATakingMove(&game.situation, takingMove);
                            if (noMoreTakingMoves(&game.situation, ++numberOfMoveInTakingSequence, selectedDestination)) {
                                moveHasBeenMade = true;
                                resetPathMap(&game.situation.board);
                                numberOfMoveInTakingSequence = 0;
                                removeMarkedForDeath(&game.situation, negateColor(player));
                                //resetPathMap(&game.situation.board);
                                flushSequenceLists(&game.situation);
                                lockIsSet = false; movesHaveBeenFound = false;
                            }
                        }
                        else {
                            selectedSource = converted;
                            resetPathMap(&game.situation.board);
                            fillPathMapTakingMoves(&game.situation, converted, numberOfMoveInTakingSequence + 1);
                        }
                    }
                    else {
                        if (marker == Destination) {
                            selectedDestination = converted;
                            if (game.situation.tmsCount != 0) {
                                TakingMove takingMove;
                                for (int i = 0; i < game.situation.tmsCount; i++) {
                                    Coordinates src = game.situation.takingSequences[i].takingMoves[0].source,
                                        dest = game.situation.takingSequences[i].takingMoves[0].destination;
                                    if (isCoordinatesEqual(src, selectedSource) && isCoordinatesEqual(dest, selectedDestination)) {
                                        takingMove = game.situation.takingSequences[i].takingMoves[0];
                                        break;
                                    }
                                }
                                makeATakingMove(&game.situation, takingMove);
                                if (noMoreTakingMoves(&game.situation, ++numberOfMoveInTakingSequence, selectedDestination)) {
                                    moveHasBeenMade = true; movesHaveBeenFound = false;
                                }
                                else {
                                    pathMapSetMovingLock(&game.situation.board, selectedDestination);
                                    game.situation.board.pathMap[takingMove.victim.y][takingMove.victim.x] = NoMove;
                                    lockIsSet = true;
                                }
                            }
                            else {
                                Move extracted;
                                for (int i = 0; i < game.situation.rmsCount; i++) {
                                    Coordinates src = game.situation.regMoveSequences[i].regularMoves[0].source,
                                        dest = game.situation.regMoveSequences[i].regularMoves[0].destination;
                                    if (isCoordinatesEqual(src, selectedSource) && isCoordinatesEqual(dest, selectedDestination)) {
                                        extracted = game.situation.regMoveSequences[i].regularMoves[0];
                                        foundRegular = true;
                                        break;
                                    }
                                }
                                if (!foundRegular) {
                                    for (int i = 0; i < game.situation.mmsCount; i++) {
                                        Coordinates src = game.situation.mixedSequences[i].kingBecomingMove.source,
                                            dest = game.situation.mixedSequences[i].kingBecomingMove.destination;
                                        if (isCoordinatesEqual(src, selectedSource) && isCoordinatesEqual(dest, selectedDestination)) {
                                            extracted = game.situation.mixedSequences[i].kingBecomingMove;
                                            //foundRegular = true;
                                            break;
                                        }
                                    }

                                }
                                movesHaveBeenFound = false;
                                makeAMove(&game.situation, extracted);
                                foundRegular = false; moveHasBeenMade = true;
                            }
                            if (!lockIsSet) {
                                removeMarkedForDeath(&game.situation, negateColor(player));
                                resetPathMap(&game.situation.board);
                                flushSequenceLists(&game.situation);
                            }
                            updateBoardRender(&game.situation.board);
                            renderBoard(&game.situation.board, player, handler, boardCursor, boardPasteX, boardPasteY);
                            //flushBuffers();


                            // make a move
                        }

                        else {
                            selectedSource = converted;
                            resetPathMap(&game.situation.board);
                            fillPathMap(&game.situation, converted);
                        }
                    }

                    //Board cpy = game.situation.board;


                    UPDATE_RENDER;
                    if (moveHasBeenMade) {
                        player = negateColor(player);
                        moveHasBeenMade = false;
                    }
                    GameSituation cpy2 = game.situation;
                    /*if (moveHasBeenMade) {
                       //if (game.type == RvsC) {
                            flushSequenceLists(&game.situation);
                            SeqContainer bestMove = getBestMove(game.situation, negateColor(player), Normal);
                            Sleep(moveMakingDelay / 5);
                            //while (bestMove.seqNumberToDo == -1) bestMove = getBestMove(test.situation, forWho, Hard);
                            switch (bestMove.seqNumberToDo) {
                                case 1: {
                                    makeARegMoveSequenceWithDelay(&game.situation, bestMove.regMoveSequence, moveMakingDelay, handler);
                                    break;
                                }
                                case 2: {
                                    makeATakingSequenceWithDelay(&game.situation, bestMove.takingSequence, moveMakingDelay, handler);
                                    //removeMarkedForDeath(&test.situation, forWho);
                                    break;
                                }
                                case 3: {
                                    makeAMixedSequenceWithDelay(&game.situation, bestMove.mixedSequence, moveMakingDelay, handler);
                                    //removeMarkedForDeath(&test.situation, forWho);
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
                        //else {
                        //    player = negateColor(player);
                        //}
*/
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
