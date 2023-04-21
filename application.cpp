#include <cstring>
#include <memory.h>
//#include "conio2.h"
#include "interface.h"
#include "clcengine/clcengine_main.h"
#include "appconsts.h"
#include "resources/resources.h"
#include "defs.h"
HBITMAP boardTextures[7], boardBorder;
HINSTANCE gInstance;
Game game;
HWND buttons[10];
#define enableCP1251 SetConsoleCP(1251); SetConsoleOutputCP(1251)

Coordinates getPasteCoordinates() {
    RECT screen;
    GetWindowRect(GetDesktopWindow(), &screen);
    return {(screen.right - WINDOW_SIZE_LENGTH) / 2, (screen.bottom - WINDOW_SIZE_HEIGHT) / 2};
}

void loadBoardTextures() {
    char fPath[255], *fName;
    for (int i = 0; i < 7; i++) {
        GetFullPathNameA(textures[i], 255, fPath, &fName);
        boardTextures[i] = (HBITMAP) LoadImage(gInstance, fPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    }
}

HBITMAP loadImage(const char* path) {
    char fPath[255], *fName;
    GetFullPathNameA(path, 255, fPath, &fName);
        return (HBITMAP) LoadImage(gInstance, fPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

void renderBoardTexture(int x, int y, int textureNumber, HDC handler) {
    HDC handlerMemory;
    BITMAP textureBitmap;
    HGDIOBJ oldBitmap;
    handlerMemory = CreateCompatibleDC(handler);
    oldBitmap = SelectObject(handlerMemory, boardTextures[textureNumber]);
    GetObject(boardTextures[textureNumber], sizeof(textureBitmap), &textureBitmap);
    BitBlt(handler, x, y, textureBitmap.bmWidth, textureBitmap.bmHeight, handlerMemory, 0, 0, SRCCOPY);
    SelectObject(handlerMemory, oldBitmap);
    DeleteDC(handlerMemory);
}

void renderBoardBorder(int x, int y, HDC handler) {
    HDC handlerMemory;
    BITMAP textureBitmap;
    HGDIOBJ oldBitmap;
    handlerMemory = CreateCompatibleDC(handler);
    oldBitmap = SelectObject(handlerMemory, boardBorder);
    GetObject(boardBorder, sizeof(textureBitmap), &textureBitmap);
    BitBlt(handler, x, y, textureBitmap.bmWidth, textureBitmap.bmHeight, handlerMemory, 0, 0, SRCCOPY);
    SelectObject(handlerMemory, oldBitmap);
    DeleteDC(handlerMemory);
}

void renderBoard(Board* board, Color playerSide, HDC handler, int x = 0, int y = 0) {
    renderBoardBorder(x, y, handler);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            short bStat = board->boardRender[j][i];
            if (playerSide == Black) {
                renderBoardTexture(33 + x + 56 * (7 - i), 33 + y + 56 * j, bStat, handler); //56 x 56
            }
            else if (playerSide == White) {
                renderBoardTexture(33 + x + 56 * i, 33 + y + 56 * (7- j), bStat, handler);
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

    switch (message) {
        case WM_CREATE: {
            loadBoardTextures();
            boardBorder = loadImage("resources\\border.bmp");
            break;
        }
        case WM_COMMAND: {
            if (lParam == (LPARAM) buttons[buttonTheFuck]) {
                MessageBox(window, "Chego ty zhdesh?", "govnuk", 0);
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
            PAINTSTRUCT  paintStructure;
            HDC handler;
            handler = BeginPaint(window, &paintStructure);
            renderBoard(&game.situation.board, Black, handler, 20, 20);
            EndPaint(window, &paintStructure);
            break;
        }
        case WM_LBUTTONUP: {
            POINT p;
            GetCursorPos(&p);
            ScreenToClient(window, &p);
            char pr[50];
            sprintf(pr, "x: %ld, y: %ld", p.x, p.y);

            MessageBoxA(window, pr, "ASS", 0);
            break;
        }
        case WM_RBUTTONUP: {
            break;
        }
        case WM_SETFONT : {
            break;
        }
        case WM_DESTROY : {
            PostQuitMessage(0);
            return 0;
        }
        case WM_CLOSE: {
            DestroyWindow(window);
        }
        case WM_QUIT: {
            break;
        }
        default: {
            return DefWindowProc(window, message, wParam, lParam);
        }
    }
}
