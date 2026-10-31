#ifndef __FRAMEWORK_H__
#define __FRAMEWORK_H__

#define _WIN32_WINNT	0x0601
#define _WIN32_IE		0x0900

#include <Windows.h>
#include <Commctrl.h>
#include <stdlib.h>
#include <stdio.h>

#define APPNAME     "Tic Tac Toe"
#define WINSTYLE    (WS_OVERLAPPEDWINDOW ^ (WS_THICKFRAME | WS_MAXIMIZEBOX))
#define WINWIDTH    400
#define WINHEIGHT   400

#define WIDTH(rc) (rc.right - rc.left)
#define HEIGHT(rc) (rc.bottom - rc.top)

typedef enum _PLAYER_NUM {
    PLAYER_ONE = -1, PLAYER_TWO = 1
} PLAYER_NUM;

#endif