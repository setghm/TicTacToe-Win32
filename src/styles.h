#ifndef __STYLES_H__
#define __STYLES_H__

#include "framework.h"

/* 
	Color Blender tool:
	meyerweb.com/eric/tools/color-blend/
*/

#define COLOR_BG			RGB(35, 22, 66)
#define COLOR_TEXT			RGB(255, 255, 255)
#define COLOR_TEXT_RED		RGB(255, 185, 185)
#define COLOR_TEXT_BLUE		RGB(185, 185, 255)
#define COLOR_ACCENT_RED	RGB(255, 155, 155)
#define COLOR_ACCENT_BLUE	RGB(155, 155, 255)
#define COLOR_CELL_NORMAL	RGB(85, 72, 116)
#define COLOR_CELL_ACTIVE	RGB(105, 92, 136)
#define COLOR_CELL_HOVER	RGB(135, 122, 166)
#define COLOR_CELL_RED		RGB(155, 40, 60)
#define COLOR_CELL_BLUE		RGB(46, 40, 169)
#define SIZE_BORDER_RADIUS	12
#define FLAGS_TEXT_DRAW		(DT_CENTER | DT_SINGLELINE | DT_VCENTER)

#define SET_GAME_FONT(hwnd, size) \
	SendMessage(hwnd, WM_SETFONT, (WPARAM) StylesFontGet(size), (LPARAM) TRUE);

void StylesFontInit(void);
HFONT StylesFontGet(int size);
void StylesFontCleanup(void);

DWORD StylesDrawControls(LPNMCUSTOMDRAW nmcd);

void StylesCustomizeTitleBar(HWND hWnd);

#endif