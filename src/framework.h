#ifndef __FRAMEWORK_H__
#define __FRAMEWORK_H__

#define _WIN32_WINNT	0x0601
#define _WIN32_IE		0x0900

#include <Windows.h>
#include <Commctrl.h>
#include <dwmapi.h>
#include <stdlib.h>
#include <stdio.h>

/* MinGW DWM API polyfills */
#ifndef DWMA_USE_IMMERSIVE_DARK_MODE
#define DWMA_USE_IMMERSIVE_DARK_MODE 20
#endif
#ifndef DWMA_BORDER_COLOR
#define DWMA_BORDER_COLOR 34
#endif
#ifndef DWMA_CAPTION_COLOR
#define DWMA_CAPTION_COLOR 35
#endif
#ifndef DWMA_TEXT_COLOR
#define DWMA_TEXT_COLOR 36
#endif

/* Global app values */
#define APPNAME     "Tic-Tac-Toe"
#define WINSTYLE    (WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX))
#define WINWIDTH    400
#define WINHEIGHT   400

#define WIDTH(rc) (rc.right - rc.left)
#define HEIGHT(rc) (rc.bottom - rc.top)
#define S(dpi, x) MulDiv((x), (dpi), 96)

typedef enum {
    PLAYER_TURN_ONE = -1,
    PLAYER_TURN_TWO = 1
} PlayerTurn;

#endif