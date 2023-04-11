//
// Created by EgrZver on 11.04.2023.
//
#include <tchar.h>
#include <windows.h>
#define _UNICODE


#ifndef CHECKERS_DEFS_H
#define CHECKERS_DEFS_H

WNDCLASSEX newWindowClass(HBRUSH backgroundColor, HCURSOR cursor, HINSTANCE instance, HICON icon/*, TCHAR name*/, WNDPROC procedureToCall);
LRESULT CALLBACK applicationProcessor(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

#endif //CHECKERS_DEFS_H
