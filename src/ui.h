#ifndef UI_H
#define UI_H

#include "framework.h"

#define UI_COUNTERP1    101
#define UI_COUNTERP2    102
#define UI_CELL         103
#define UI_STATUSBAR    112
#define UI_LABELP1		113
#define UI_LABELP2		114

void UICreateControls(HWND hwnd, HINSTANCE hInstance);
void UICreatePlayerCounter(HWND hwnd, HINSTANCE hInstance, PlayerTurn player);

void UILayoutControls(HWND hwnd, UINT dpi);

HBRUSH UIColorTextControls(int controlId, HDC hdcStatic);
HBRUSH UIColorButtonControls(void);

DWORD UICustomDraw(LPNMCUSTOMDRAW nmcd, int dpi);

void UIReshapeWindow(HWND hwnd, const RECT *rc);
void UIInitWindow(HWND hwnd, UINT dpi);

#endif // UI_H