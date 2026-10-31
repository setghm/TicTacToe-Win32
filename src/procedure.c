#include "framework.h"
#include "resource.h"
#include "game.h"
#include "styles.h"

BOOL UICreatePlayerCounter(HWND hWnd,
                           HINSTANCE hInstance,
                           int number,
                           int x,
                           int y)
{
    const int xsize = 120, ysize = 72;
    const int paddx = 16, paddy = 8;

    char label[] = "Player 0";
    label[7] = number + 0x30;

    /* Create the player label */
    if (CreateWindow(
        "STATIC",
        label,
        SS_CENTER | WS_CHILD | WS_VISIBLE,
        x + paddx + 10, y + paddy,
        xsize - 20, 24,
        hWnd,
        (HMENU) (UI_LABELP1 + (number - 1)),
        hInstance,
        NULL
    ) == NULL) {
        return FALSE;
    }
	
	SET_GAME_FONT(GetDlgItem(hWnd, (UI_LABELP1 + (number - 1))), 14);

    /* Create the score counter */
    if (CreateWindow(
        "STATIC",
        "0",
        SS_CENTER | WS_CHILD | WS_VISIBLE,
        x + paddx + 24, 44,
        72, 24,
        hWnd,
        (HMENU) (UI_COUNTERP1 + (number - 1)),
        hInstance,
        NULL
    ) == NULL) {
        return FALSE;
    }
	
	SET_GAME_FONT(GetDlgItem(hWnd, (UI_COUNTERP1 + (number - 1))), 16);

    return TRUE;
}

BOOL CreateWindowUI(HWND hWnd, HINSTANCE hInstance) {

    const int   cellwh = 72,    // Cell width/height
                cellpa = 8;     // Cell padding right/bottom
    int x, y;
    // Calculate the left margin for the cells
    int cellma = (WINWIDTH - (cellwh * 3 + (cellpa * 2))) / 2;

    /* Create the score counters  */
    UICreatePlayerCounter(hWnd, hInstance, 1, 16, 8);
    UICreatePlayerCounter(hWnd, hInstance, 2, 226, 8);

    /* Create the cells */
    y = 100;
    for (int cy = 0; cy < 3; cy++) {
        for (int cx = 0; cx < 3; cx++) {
            x = (cx == 0) ? cellma : cellma + (cx * cellpa);
            
            if (CreateWindow(
                "BUTTON",
                "X",
                BS_CENTER | WS_CHILD | WS_VISIBLE,
                x + (cellwh * cx),
                y,
                cellwh, cellwh,
                hWnd,
                (HMENU) (UI_CELL + cx + (3 * cy)),
                hInstance,
                NULL
            ) == NULL) {
                return FALSE;
            }
			
			SET_GAME_FONT(GetDlgItem(hWnd, (UI_CELL + cx + (3 * cy))), 36);
			
        }
        y += cellwh + cellpa;
    }

    /* Create the status bar */
    if (CreateWindow(
        "STATIC",
        "blue turn",
        SS_CENTER | WS_CHILD | WS_VISIBLE,
        8, WINHEIGHT - 36,
        WINWIDTH - 16, 24,
        hWnd,
        (HMENU) UI_STATUSBAR,
        hInstance,
        NULL
    ) == NULL) {
        return FALSE;
    }
	
	SET_GAME_FONT(GetDlgItem(hWnd, UI_STATUSBAR), 18);

    return TRUE;
}

LRESULT CALLBACK WinProc(HWND hWnd,
                         UINT uMsg,
                         WPARAM wParam,
                         LPARAM lParam)
{
    static HINSTANCE hInstance;

    switch (uMsg) {
        case WM_CREATE: {
			StylesFontInit();
			
            hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
            if (!CreateWindowUI(hWnd, hInstance)) {
                MessageBox(0, "Unable to create the window UI", 0, MB_ICONERROR);
                exit(EXIT_FAILURE);
            }

            InitGame();
            StartNewGame(hWnd);
            break;
        }
        case WM_COMMAND: {
            CheckInput(wParam);
            UpdateBoard(hWnd);
            LookForWinner(hWnd);
            break;
        }
		case WM_NOTIFY: {
			LPNMHDR nmhdr = (LPNMHDR) lParam;
			if (nmhdr->code & NM_CUSTOMDRAW) {
				return StylesDrawControls((LPNMCUSTOMDRAW)nmhdr);
			}
            break;
		}
		case WM_CTLCOLORSTATIC: {
			HDC hdcStatic = (HDC) wParam;
			
			switch (GetDlgCtrlID((HWND)lParam)) {
				case UI_LABELP1:
				case UI_COUNTERP1:
					SetTextColor(hdcStatic, COLOR_TEXT_RED);
					break;
				case UI_LABELP2:
				case UI_COUNTERP2:
					SetTextColor(hdcStatic, COLOR_TEXT_BLUE);
					break;
				default:
					SetTextColor(hdcStatic, COLOR_TEXT);
			}
			
			SetBkMode(hdcStatic, TRANSPARENT);
			return (LRESULT) CreateSolidBrush(COLOR_BG);
		}
		case WM_CTLCOLORBTN: {
			return (LRESULT) CreateSolidBrush(COLOR_BG);
		}
        case WM_DESTROY: {
			StylesFontCleanup();
            PostQuitMessage(0);
            break;
		}
        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return FALSE;
}