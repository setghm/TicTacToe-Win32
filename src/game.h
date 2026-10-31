#ifndef __GAME_H__
#define __GAME_H__

#include "framework.h"

void InitGame(void);
void StartNewGame(HWND parent);
void CheckInput(DWORD id);
void UpdateBoard(HWND parent);
void LookForWinner(HWND parent);
void UpdateScore(HWND parent, int add);

int GetCellState(int id);

#endif