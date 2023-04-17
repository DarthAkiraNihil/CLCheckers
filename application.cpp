#include <cstdio>
#include <cstring>
#include <unistd.h>
//#include <tchar.h>
#include <memory.h>
//#include "conio2.h"
#include "interface.h"
//#include "checkerslib.h"
#include "clcengine/clcengine_main.h"
#include "appconsts.h"
#include "resources/resources.h"
#include "defs.h"
//#include <windows.h>
HBITMAP boardTextures[6];// = NULL;
HBITMAP test;
HINSTANCE gInstance;
HWND mainHandle;
Game game;
char pathos[255];
#define enableCP1251 SetConsoleCP(1251); SetConsoleOutputCP(1251)
char testPath[255], *testName[255]; const char *file = "resources\\test.bmp";

Coordinates getPasteCoordinates() {
    RECT screen;
    GetWindowRect(GetDesktopWindow(), &screen);
    return {(screen.right - WINDOW_SIZE_LENGTH) / 2, (screen.bottom - WINDOW_SIZE_HEIGHT) / 2};
}

void loadBoardTextures() {
    char fPath[255], *fName;
    //char cwdcpy[255];
    for (int i = 0; i < 6; i++) {
        //const char *texture = texturePaths[i];
        GetFullPathNameA(textures[i], 255, fPath, &fName);
        //puts(fPath);
        boardTextures[i] = (HBITMAP) LoadImage(gInstance, fPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    }
    //strcpy(cwdcpy, pathos);
    //strcat(cwdcpy, "\\resources\\test.bpm");
    //getcwd(pathos, sizeof(pathos));
    //chdir(pathos);

    //GetFullPathNameA(file, 255, testPath, testName);
    //test = (HBITMAP)LoadImage(gInstance, testPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    //test = (HBITMAP)LoadImage(gInstance, "resources\\test.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    //break;
}

void renderBoardTexture(int x, int y, int textureNumber, HDC handler) {
    //PAINTSTRUCT paintStructure;
    HDC /*handler,*/ handlerMemory;
    BITMAP textureBitmap;
    HGDIOBJ oldBitmap;

    handlerMemory = CreateCompatibleDC(handler);
    oldBitmap = SelectObject(handlerMemory, boardTextures[textureNumber]);

    GetObject(boardTextures[textureNumber], sizeof(textureBitmap), &textureBitmap);
    BitBlt(handler, x, y, textureBitmap.bmWidth, textureBitmap.bmHeight, handlerMemory, 0, 0, SRCCOPY);

    SelectObject(handlerMemory, oldBitmap);
    DeleteDC(handlerMemory);
}

void renderBoard(Board* board, Color playerSide, HDC handler, int x = 0, int y = 0) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            short bStat = board->boardRender[j][i];
            if (playerSide == Black) {
                renderBoardTexture(x + 56 * (7 - i), y + 56 * j, bStat, handler); //56 x 56
                //gotoxy(drawX + 8 - i, drawY + 1 + j);
            }
            else if (playerSide == White) {
                renderBoardTexture(x + 56 * i, y + 56 * (7- j), bStat, handler);
                //gotoxy(drawX + 1 + i, drawY + 8 - j);
            }
        }
    }
}


int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR args, int cmdShow) {
    game = createANewGame(White, White, RvsC);
    ascendChecker(&game.situation.board.checkers[White][8]);
    updateBoardRender(&game.situation.board);
    WNDCLASSEX mainClass = newWindowClass((HBRUSH) COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), instance, LoadIcon(NULL, IDI_APPLICATION), applicationProcessor);
    Coordinates pasteCoordinates = getPasteCoordinates();
    mainClass.lpszClassName = _TEXT(APPLICATION_MAIN_CLASS_NAME);
    mainClass.hIcon = (HICON) LoadImage(instance, MAKEINTRESOURCE(IDI_APP_BIG_ICON), IMAGE_ICON, 128, 128, 0);
    mainClass.hIconSm = (HICON) LoadImage(instance, MAKEINTRESOURCE(IDI_APP_WIN_ICON), IMAGE_ICON, 16, 16, 0);
    if (!RegisterClassEx(&mainClass)) {return -1;}

    MSG applicationMSG = {0};
    gInstance = instance;

    getcwd(pathos, sizeof(pathos));
    chdir(pathos);
    //test = LoadBitmap(gInstance, MAKEINTRESOURCE(IDB_TEST));
    //CreateWindowA
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
    //HBITMAP hBitmap=LoadBitmap(instance, "resources/test.bmp");


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

    //newClass.hIcon = icon;
    newClass.hCursor = cursor;
    newClass.hInstance = instance;
    /*newClass.lpszClassName = (LPCSTR) (name);*/
    newClass.hbrBackground = backgroundColor;
    newClass.lpfnWndProc = procedureToCall;
    return newClass;
}


LRESULT CALLBACK applicationProcessor(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {

    switch (message) {
        case WM_CREATE: {
            //loadBoardTextures();
            //char cwdcpy[255];
            //strcpy(cwdcpy, pathos);
            //strcat(cwdcpy, "\\resources\\test.bpm");
            //getcwd(pathos, sizeof(pathos));
            //chdir(pathos);
            char testPath[255], *testName[255];
            GetFullPathNameA(file, 255, testPath, testName);
            test = (HBITMAP)LoadImage(gInstance, testPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            loadBoardTextures();
            /*GetFullPathNameA(textures[0], 255, testPath, testName);
            boardTextures[0] = (HBITMAP)LoadImage(gInstance, testPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            GetFullPathNameA(textures[1], 255, testPath, testName);
            boardTextures[1] = (HBITMAP)LoadImage(gInstance, testPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);*/
            //test = (HBITMAP)LoadImage(gInstance, "resources\\test.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);*/
            break;
        }
        case WM_PAINT: {
            PAINTSTRUCT     ps;
            HDC             hdc;
            BITMAP          bitmap;
            HDC             hdcMem;
            HGDIOBJ         oldBitmap;

            hdc = BeginPaint(window, &ps);
            renderBoard(&game.situation.board, Black, hdc, 20, 20);
            //renderBoardTexture(100, 100, 0, hdc);
           //renderBoardTexture(100, 180, 5, hdc);
            //hdcMem = CreateCompatibleDC(hdc);
            //oldBitmap = SelectObject(hdcMem, boardTextures[0]);

            //GetObject(test, sizeof(bitmap), &bitmap);
            //BitBlt(hdc, 0, 90, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
            //oldBitmap = SelectObject(hdcMem, boardTextures[0]);
            /*SelectObject(hdcMem, boardTextures[0]);
            GetObject(boardTextures[0], sizeof(bitmap), &bitmap);
            BitBlt(hdc, 100, 100, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);*/

            //SelectObject(hdcMem, oldBitmap);
            //DeleteDC(hdcMem);

            EndPaint(window, &ps);
            /*PAINTSTRUCT paintStruct;
            HDC handle = BeginPaint(window, &paintStruct);
            HDC mdc = CreateCompatibleDC(handle);
            BitBlt(handle, 0, 0, 128, 128, mdc, 0, 0, SRCCOPY);
            EndPaint(window, &paintStruct);*/
            break;
        }
        case WM_LBUTTONUP: {
            //MessageBox(window, testPath, "OOOOO MA GAD", 0);
            break;
        }
        case WM_RBUTTONUP: {
            //printf("Aha!");
            break;
        }
        case WM_DESTROY : {
            PostQuitMessage(0);
            return 0;
            //applicationProcessor(window, WM_CLOSE, NULL, NULL);
            //return 0;
            break;
        }
        case WM_CLOSE: {
            DestroyWindow(window);
            //return 0;
        }
        case WM_QUIT: {
            break;
            //return 0;
        }
        default: {
            return DefWindowProc(window, message, wParam, lParam);
        }
    }
}

/*void renderBoard(Board* board, Color playerSide, short drawX, short drawY, bool pasteCoordinates) {
    drawFrame(10, 10, drawX, drawY);
    if (pasteCoordinates) {
        gotoxy(drawX + 1, drawY - 1); printf("abcdefgh");
        gotoxy(drawX + 1, drawY + 10); printf("abcdefgh");
        for (int i = 0; i < 8; i++) {
            gotoxy(drawX - 1, drawY + 1 + i);
            printf("%d", i + 1);
            gotoxy(drawX + 10, drawY + 1 + i);
            printf("%d", i + 1);
        }
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            short bStat = board->boardRender[j][i];
            if (playerSide == Black) {
                gotoxy(drawX + 8 - i, drawY + 1 + j);
            }
            else if (playerSide == White) {
                gotoxy(drawX + 1 + i, drawY + 8 - j);
            }
            switch(bStat) {
                case EMPTY_BLACK: {
                    textbackground(0);
                    printf(" ");
                    break;
                }
                case EMPTY_WHITE: {
                    textbackground(15);
                    printf(" ");
                    break;
                }
                case REG_BLACK: {
                    textcolor(12);
                    printf("O");
                    break;
                }
                case REG_WHITE: {
                    textcolor(15);
                    printf("O");
                    break;
                }
                case KING_BLACK: {
                    textcolor(12);
                    printf("W");
                    break;
                }
                case KING_WHITE: {
                    textcolor(15);
                    printf("W");
                    break;
                }

            }
            textbackground(0);
        }
    }
    textbackground(0);
    textcolor(15);
}

void printRegMoveSeq(RegMoveSequence sequence, Coordinates at) {
    gotoxy(at.x, at.y);
    if (sequence.rmsCount == 2) {
        printf("[%d:%d][%d:%d]->[%d:%d][%d:%d]",
               sequence.regularMoves[0].source.x,
               sequence.regularMoves[0].source.y,
               sequence.regularMoves[0].destination.x,
               sequence.regularMoves[0].destination.y,
               sequence.regularMoves[1].source.x,
               sequence.regularMoves[1].source.y,
               sequence.regularMoves[1].destination.x,
               sequence.regularMoves[1].destination.y
               );
    }
    else {
        printf("[%d:%d][%d:%d]",
               sequence.regularMoves[0].source.x,
               sequence.regularMoves[0].source.y,
               sequence.regularMoves[0].destination.x,
               sequence.regularMoves[0].destination.y
        );
    }
}

void printTakingSeq(TakingSequence sequence, Coordinates at) {
    gotoxy(at.x, at.y);


    for (int j = 0; j < sequence.tmsCount; j++) {
        printf("[%d:%d][%d:%d][%d:%d]|",
               sequence.takingMoves[j].source.x,
               sequence.takingMoves[j].source.y,
               sequence.takingMoves[j].victim.x,
               sequence.takingMoves[j].victim.y,
               sequence.takingMoves[j].destination.x,
               sequence.takingMoves[j].destination.y);
    }


}

void printMixedSeq(MixedSequence sequence, Coordinates at) {
    gotoxy(at.x, at.y);

        printf("[%d:%d][%d:%d]>", sequence.kingBecomingMove.source.x,sequence.kingBecomingMove.source.y,sequence.kingBecomingMove.destination.x,sequence.kingBecomingMove.destination.y);
    printTakingSeq(sequence.takingSequence, {at.x+12, at.y});




}

int main() {

    clrscr();
    hideCursor();
    enableCP1251;
    //Board test_board = initiateGameBoard(false);


    setWindowSize(WINDOW_SIZE_LENGTH, WINDOW_SIZE_HEIGTH);
    textcolor(15);
    //printf("AAA");
    //waitForKey(13);
    drawFrame(WINDOW_SIZE_LENGTH, WINDOW_SIZE_HEIGTH, 1, 1);
    int choice;
    Game test = createANewGame(White, White, RvsC);
    //test.situation = makeNullGameSituation(White);
    test.situation.board = createANewBoard();
    TextLineWidget tlw;
    tlw.length = 29;
    tlw.locX = 4; tlw.locY = 2;
    Color player = White;
    setInitialText(&tlw, "pidarok");
    //drawTextLineWidget(tlw);
    drawMenu(1, mainMenuPoints, 5, 2, 2, 12);
    do {
        choice = drawMenu(APPLICATION_TITLE, 1, mainMenuPoints, 5, mainMenuPointsDescriptions, 5, 16, 4, 15, 3, 12);
        switch (choice) {
            case 1: {

                Color forWho = player;
                while (true) {
                    drawFrame(WINDOW_SIZE_LENGTH, WINDOW_SIZE_HEIGTH, 1, 1);
                    renderBoard(&test.situation.board, player, 4, 4, true);

                    if (forWho == player) {
                        findAllMoves(&test.situation, forWho);
                        for (int i = 0; i < test.situation.rmsCount; i++) {
                            gotoxy(16, 2 + i);
                            printf("%d)", i);
                            printRegMoveSeq(test.situation.regMoveSequences[i], {20, 2+ i});
                        }
                        for (int i = 0; i < test.situation.mmsCount; i++) {
                            gotoxy(16, 2 + i);
                            printf("%d)", i);
                            printMixedSeq(test.situation.mixedSequences[i], {20, 15 + i});
                        }
                        for (int i = 0; i < test.situation.tmsCount; i++) {
                            gotoxy(16, 2 + i);
                            printf("%d)", i);
                            printTakingSeq(test.situation.takingSequences[i], {20, 2+ i});
                        }
                        gotoxy(2, 15);
                        int move;
                        scanf("%d", &move);
                        if (move >= 0) {
                            if (test.situation.tmsCount > 0) {
                                makeATakingMoveSequence(&test.situation, test.situation.takingSequences[move]);
                                //removeMarkedForDeath(&test.situation, forWho);
                            }
                            else {
                                makeARegMoveSequence(&test.situation, test.situation.regMoveSequences[move]);
                            }
                        }
                        else {
                            makeAMixedSequence(&test.situation, test.situation.mixedSequences[-move + 1]);
                            //removeMarkedForDeath(&test.situation, forWho);
                        }
                    }
                    else {
                        SeqContainer bestMove = getBestMove(test.situation, forWho, Insane);
                        //while (bestMove.seqNumberToDo == -1) bestMove = getBestMove(test.situation, forWho, Hard);
                        switch (bestMove.seqNumberToDo) {
                            case 1: {
                                makeARegMoveSequence(&test.situation, bestMove.regMoveSequence);
                                break;
                            }
                            case 2: {
                                makeATakingMoveSequence(&test.situation, bestMove.takingSequence);
                                //removeMarkedForDeath(&test.situation, forWho);
                                break;
                            }
                            case 3: {
                                makeAMixedSequence(&test.situation, bestMove.mixedSequence);
                                //removeMarkedForDeath(&test.situation, forWho);
                                break;
                            }
                        }
                        //printRegMoveSeq(bestMove.regMoveSequence, {15, 10});
                   }

                    //updateBoardRender(&test.situation.board);
                    updateBoardRender(&test.situation.board);
                    forWho = negateColor(forWho);
                    removeMarkedForDeath(&test.situation, forWho);
                    clrscr();

                    flushSequenceLists(&test.situation);

                }
                break;
            }
            case 2: {
                break;
            }
            case 3: {
                break;
            }
            case 4: {
                break;
            }
            case -1: {
                printf("mpdcount != pointscount");
                break;
            }
        }
        //Board gameBoard = initiateGameBoard(true);
        //renderBoard(&gameBoard, 4, 4);
    } while (choice != 5);
    return 0;
}
*/