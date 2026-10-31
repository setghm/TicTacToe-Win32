#include "framework.h"
#include "resource.h"
#include "game.h"

short gameBoard[] = {
    0, 0, 0,
    0, 0, 0,
    0, 0, 0
};

struct {
    unsigned int player1;
    unsigned int player2;
} score;

PLAYER_NUM player;

unsigned short available = 0;

void InitGame(void) {
    score.player1 = 0;
    score.player2 = 0;
    player = PLAYER_ONE;
}

void StartNewGame(HWND parent) {
    memset(gameBoard, 0, 9 * sizeof(short));
    available = 9;

    UpdateBoard(parent);
}

void CheckInput(DWORD id) {
    if (id >= UI_CELL && id < UI_STATUSBAR) {
        if (gameBoard[id - UI_CELL] == 0) {
            gameBoard[id - UI_CELL] = player;
            player = (player == PLAYER_ONE) ? PLAYER_TWO : PLAYER_ONE;
            available--;
        }
    }
}

void UpdateBoard(HWND parent) {

    for (int i=0; i<9; i++) {
        switch (gameBoard[i]) {
            case -1:
                SetDlgItemText(parent, UI_CELL + i, "X");
                break;
            case 1:
                SetDlgItemText(parent, UI_CELL + i, "O");
                break;
            default:
                SetDlgItemText(parent, UI_CELL + i, " ");
        }
    }

    if (player == PLAYER_ONE) {
        SetDlgItemText(parent, UI_STATUSBAR, "red turn");
    } else {
        SetDlgItemText(parent, UI_STATUSBAR, "blue turn");
    }
}

void LookForWinner(HWND parent) {
    // -3 => Red wins, 3 => Blue wins
    signed int add, line, k;
    
    line = 0;

    for (int i=0; i<3; i++) {
        add = 0;
        for (int j=0; j<3; j++) {
            add += gameBoard[j + (i * 3)];
            if (i == j) {
                line += gameBoard[j + (i * 3)];
            }
        }
        if (add == 3 || add == -3) break;
    }

    if (line == 3 || line == -3) {
        UpdateScore(parent, line);
    } else {
        UpdateScore(parent, add);
    }

    line = 0;
    k = 2;

    for (int j=0; j<3; j++) {
        add = 0;
        for (int i=0; i<3; i++) {
            add += gameBoard[j + (i * 3)];
            if (i == k) {
                line += gameBoard[j + (i * 3)];
            }
        }
        k--;
        if (add == 3 || add == -3) break;
    }

    if (line == 3 || line == -3) {
        UpdateScore(parent, line);
    } else {
        UpdateScore(parent, add);
    }

    if (available == 0) {
        SetDlgItemText(parent, UI_STATUSBAR, "NO WINNER");
        Sleep(1000);
        StartNewGame(parent);
    }
}

void UpdateScore(HWND parent, int add) {
    char sscore[512];
    BOOL winner = FALSE;
    
    if (add == -3) {
        score.player1++;

        sprintf(sscore, "%d", score.player1);

        SetDlgItemText(parent, UI_COUNTERP1, sscore);
        SetDlgItemText(parent, UI_STATUSBAR, "RED WINS");

        winner = TRUE;
    } else if (add == 3) {
        score.player2++;

        sprintf(sscore, "%d", score.player2);

        SetDlgItemText(parent, UI_COUNTERP2, sscore);
        SetDlgItemText(parent, UI_STATUSBAR, "BLUE WINS");

        winner = TRUE;
    }

    if (winner) {
        Sleep(1000);
        StartNewGame(parent);
    }
}

int GetCellState(int id) {
	if ((id - UI_CELL) >= 0 && (id - UI_CELL) < 9) {
		return gameBoard[id - UI_CELL];
	}
	return 0;
}
