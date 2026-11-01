#ifndef __GAME_H__
#define __GAME_H__

#include "framework.h"

void GameInit(void);
void GameStartNew(HWND parent);
void GameInputCheck(DWORD id);
void GameBoardUpdate(HWND parent);
void GameWinnerCheck(HWND parent);
void GameScoreUpdate(HWND parent, int add);

int GameCellGetState(int id);

#endif