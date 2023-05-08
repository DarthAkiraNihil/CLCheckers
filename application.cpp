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
Coordinates boardCursor = {0, 5};

bool findMoves = false;
#define enableCP1251 SetConsoleCP(1251); SetConsoleOutputCP(1251)



inline bool in(int var, int leftBorder, int rightBorder) {
    return (var > leftBorder) && (var < rightBorder);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR args, int cmdShow) {
    player = Black;
    game = createANewGame(player, White, RvsC);
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
        case WM_LBUTTONUP: {
            /*POINT p;
            GetCursorPos(&p);
            ScreenToClient(window, &p);
            //char pr[50];
            if (in((int) p.x, boardPasteX + 33, boardPasteX + 8 * 56 + 33) && in((int) p.y, boardPasteY + 33, boardPasteY + 8*56 + 33)) {

                //Coordinates b = transformXYToBoardXY(p.x, p.y, playerSide);
                Coordinates placeToRender = getNearestCorner(p.x, p.y), boardPos = transformXYToBoardXY(p.x, p.y, player);
                if (!movesHaveBeenFound) {
                    findAllMoves(&game.situation, player);
                    movesHaveBeenFound = true;
                }
                //BeginPaint(window, &paintStructure);
                renderBoard(&game.situation.board, player, handler, boardCursor, boardPasteX, boardPasteY);
                switch (inWhatBufferIsDestination(boardPos, 0)) {
                    case 1: {
                        Move extracted = extractMoveFromRSBuffer(rmsForChosen, boardPos);
                        makeAMove(&game.situation, extracted);
                        updateBoardRender(&game.situation.board);
                        renderBoard(&game.situation.board, player, handler, boardCursor, boardPasteX, boardPasteY);
                        flushBuffers();
                        movesHaveBeenFound = false; moveHasBeenMade = true;
                        break;
                    }
                    case 2: {
                        //printf("ASS");
                        TakingMove extracted = extractTakingMoveFromTSBuffer(tsForChosen, boardPos, 0);
                        makeATakingMove(&game.situation, extracted);
                        //renderBoard2(&game.situation.board, player, 0, 0, false);
                        updateBoardRender(&game.situation.board);
                        removeMarkedForDeath(&game.situation, negateColor(player));
                        renderBoard(&game.situation.board, player, handler, boardCursor, boardPasteX, boardPasteY);
                        numberOfCurrentTSMove++;
                        //if (!isThereAnyOtherTakingMoves(tsForChosen, boardPos, numberOfCurrentTSMove)) {
                            flushBuffers();
                            movesHaveBeenFound = false; moveHasBeenMade = true;
                        //}
                        //movesHaveBeenFound = false; moveHasBeenMade = true; flushBuffers();
                        break;
                    }
                    case 3: {
                        Move extracted = extractMoveFromMSBuffer(msForChosen, boardPos);
                        makeAMove(&game.situation, extracted);
                        updateBoardRender(&game.situation.board);
                        renderBoard(&game.situation.board, player, handler, boardCursor, boardPasteX, boardPasteY);
                        movesHaveBeenFound = false; moveHasBeenMade = true; flushBuffers();
                        break;
                    }
                    default: {
                        flushBuffers();
                        //if (numberOfCurrentTSMove == 0) flushBuffers();
                        if (game.situation.tmsCount != 0) {
                            for (int i = 0; i < game.situation.tmsCount; i++) {
                                if (isCoordinatesEqual(game.situation.takingSequences[i].takingMoves[0].source, boardPos)) {
                                    renderTakingSequence(game.situation.takingSequences[i], 0, handler);
                                    addTSToBuffer(tsForChosen, game.situation.takingSequences[i]);
                                }
                            }
                        }
                        else {
                            for (int i = 0; i < game.situation.mmsCount; i++) {
                                if (isCoordinatesEqual(game.situation.mixedSequences[i].kingBecomingMove.source, boardPos)) {
                                    renderRegularMoveSequence(game.situation.regMoveSequences[i], 0, handler);
                                    addMSToBuffer(msForChosen, game.situation.mixedSequences[i]);
                                }
                            }
                            for (int i = 0; i < game.situation.rmsCount; i++) {
                                if (isCoordinatesEqual(game.situation.regMoveSequences[i].regularMoves[0].source, boardPos)) {
                                    renderRegularMoveSequence(game.situation.regMoveSequences[i], 0, handler);
                                    addRMSToBuffer(rmsForChosen, game.situation.regMoveSequences[i]);
                                }
                            }
                        }
                    }

                }
                if (moveHasBeenMade) {
                    flushSequenceLists(&game.situation);
                    SeqContainer bestMove = getBestMove(game.situation, negateColor(player), Invincible);
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
                if (destinationInRMSBuffer(rmsForChosen, boardPos)) {

                    //int index = getAvailableIndex(coordinatesOfAvailableMoves, boardPos);
                    Move extractedMove = extractMoveFromRSBuffer(rmsForChosen, boardPos);
                    makeAMove(&game.situation, extractedMove);
                    updateBoardRender(&game.situation.board);
                    renderBoard(&game.situation.board, player, handler, boardPasteX, boardPasteY);
                    flushBuffers();
                    flushSequenceLists(&game.situation);
                    movesHaveBeenFound = false;
                    SeqContainer bestMove = getBestMove(game.situation, negateColor(player), Normal);
                    //while (bestMove.seqNumberToDo == -1) bestMove = getBestMove(test.situation, forWho, Hard);
                    switch (bestMove.seqNumberToDo) {
                        case 1: {
                            makeARegMoveSequenceWithDelay(&game.situation, bestMove.regMoveSequence, 2000);
                            break;
                        }
                        case 2: {
                            makeATakingSequenceWithDelay(&game.situation, bestMove.takingSequence, 2000);
                            //removeMarkedForDeath(&test.situation, forWho);
                            break;
                        }
                        case 3: {
                            makeAMixedSequenceWithDelay(&game.situation, bestMove.mixedSequence, 2000);
                            //removeMarkedForDeath(&test.situation, forWho);
                            break;
                        }
                    }
                    updateBoardRender(&game.situation.board);
                    removeMarkedForDeath(&game.situation, player);
                    removeMarkedForDeath(&game.situation, negateColor(player));
                    flushSequenceLists(&game.situation);
                    renderBoard(&game.situation.board, player, handler, boardPasteX, boardPasteY);
                }
                else {
                    flushBuffers();
                    if (game.situation.tmsCount != 0) {
                        for (int i = 0; i < game.situation.tmsCount; i++) {
                            if (isCoordinatesEqual(game.situation.takingSequences[i].takingMoves[0].source, boardPos)) {
                                renderTakingSequence(game.situation.takingSequences[i], 0, handler);
                                addTSToBuffer(tsForChosen, game.situation.takingSequences[i]);
                            }
                        }
                    }
                    else {
                        for (int i = 0; i < game.situation.mmsCount; i++) {
                            if (isCoordinatesEqual(game.situation.mixedSequences[i].kingBecomingMove.source, boardPos)) {
                                renderRegularMoveSequence(game.situation.regMoveSequences[i], 0, handler);
                                addMSToBuffer(msForChosen, game.situation.mixedSequences[i]);
                            }
                        }
                        for (int i = 0; i < game.situation.rmsCount; i++) {
                            if (isCoordinatesEqual(game.situation.regMoveSequences[i].regularMoves[0].source, boardPos)) {
                                renderRegularMoveSequence(game.situation.regMoveSequences[i], 0, handler);
                                addRMSToBuffer(rmsForChosen, game.situation.regMoveSequences[i]);
                            }
                        }
                    }
                }
                ReleaseDC(window, handler);
            }*/
            break;
        }
        case WM_KEYDOWN : {
            switch (wParam) {
                case VK_UP: {
                    //MessageBox(window, "J O P A", "J O P A", 0);
                    if (boardCursor.y > 0) boardCursor.y--; else boardCursor.y = 7;
                    renderBoard(&game.situation.board, player, handler, boardCursor, boardPasteX, boardPasteY);
                    break;
                }
                case VK_DOWN: {
                    //MessageBox(window, "Fcking task", "Holy shit", 0);
                    if (boardCursor.y < 7) boardCursor.y++; else boardCursor.y = 0;
                    renderBoard(&game.situation.board, player, handler, boardCursor, boardPasteX, boardPasteY);
                    break;
                }
                case VK_LEFT: {
                    //MessageBox(window, "I AM NOW MESSAGEBOX CERTIFIED", "UR TOO LATE SONIC!", 0);
                    if (boardCursor.x > 0) boardCursor.x--; else boardCursor.x = 7;
                    renderBoard(&game.situation.board, player, handler, boardCursor, boardPasteX, boardPasteY);
                    break;
                }
                case VK_RIGHT: {
                    //MessageBox(window, "I now am too message box certified", "HAHA, I TOO AM LATE", 0);
                    if (boardCursor.x < 7) boardCursor.x++; else boardCursor.x = 0;
                    renderBoard(&game.situation.board, player, handler, boardCursor, boardPasteX, boardPasteY);
                    break;

                }case VK_RETURN: {
                    Coordinates converted = cursorToBoardCoord(boardCursor, player);
                    if (!movesHaveBeenFound) {
                        findAllMoves(&game.situation, player);
                        movesHaveBeenFound = true;
                    }
                    flushBuffers();
                    if (game.situation.tmsCount != 0) {
                        for (int i = 0; i < game.situation.tmsCount; i++) {
                            if (isCoordinatesEqual(game.situation.takingSequences[i].takingMoves[0].source, converted)) {
                                renderTakingSequence(game.situation.takingSequences[i], 0, handler);
                                addTSToBuffer(tsForChosen, game.situation.takingSequences[i]);
                            }
                        }
                    }
                    else {
                        for (int i = 0; i < game.situation.mmsCount; i++) {
                            if (isCoordinatesEqual(game.situation.mixedSequences[i].kingBecomingMove.source, converted)) {
                                renderRegularMoveSequence(game.situation.regMoveSequences[i], 0, handler);
                                addMSToBuffer(msForChosen, game.situation.mixedSequences[i]);
                            }
                        }
                        for (int i = 0; i < game.situation.rmsCount; i++) {
                            if (isCoordinatesEqual(game.situation.regMoveSequences[i].regularMoves[0].source, converted)) {
                                renderRegularMoveSequence(game.situation.regMoveSequences[i], 0, handler);
                                addRMSToBuffer(rmsForChosen, game.situation.regMoveSequences[i]);
                            }
                        }
                    }
                    //MessageBox(window, "A S S", "J O P A", 0);
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
}
