#include "ui.h"
#include "resource.h"
#include "game.h"
#include "styles.h"

/* -------------------------------------------------------------------------- */
/*                                Create logic                                */
/* -------------------------------------------------------------------------- */

void UICreateControls(HWND hwnd, HINSTANCE hInstance) {
    const int labelStyle = SS_CENTER | WS_CHILD | WS_VISIBLE;
    const int buttonStyle = BS_CENTER | WS_CHILD | WS_VISIBLE;

    CreateWindow("STATIC", "Player 1", labelStyle, 0, 0, 0, 0, hwnd, (HMENU) UI_LABELP1, hInstance, NULL);
    CreateWindow("STATIC", "Player 2", labelStyle, 0, 0, 0, 0, hwnd, (HMENU) UI_LABELP2, hInstance, NULL);
    CreateWindow("STATIC", "0", labelStyle, 0, 0, 0, 0, hwnd, (HMENU) UI_COUNTERP1, hInstance, NULL);
    CreateWindow("STATIC", "0", labelStyle, 0, 0, 0, 0, hwnd, (HMENU) UI_COUNTERP2, hInstance, NULL);
    CreateWindow("STATIC", "Blue turn", labelStyle, 0, 0, 0, 0, hwnd, (HMENU) UI_STATUSBAR, hInstance, NULL);

    for (int i = 0; i < 9; i++) {
        CreateWindow("BUTTON", "X", buttonStyle, 0, 0, 0, 0, hwnd, (HMENU) (UI_CELL + i), hInstance, NULL);
    }
}

/* -------------------------------------------------------------------------- */
/*                                Layout logic                                */
/* -------------------------------------------------------------------------- */

static inline void UILayoutPlayerCounter(HWND hwnd, UINT dpi, int playerNumber) {
    static const POINT pos[2] = { { 16, 8 }, { 226, 8 } };
    static const POINT size = { 120, 72 };
    static const POINT pad = { 16, 8 };

    int i = playerNumber - 1;

    HWND hLabel = GetDlgItem(hwnd, UI_LABELP1 + i);
    HWND hCounter = GetDlgItem(hwnd, UI_COUNTERP1 + i);

    SetWindowPos(hLabel, NULL,
        S(dpi, pos[i].x + pad.x + 10),
        S(dpi, pos[i].y + pad.y),
        S(dpi, size.x - 20),
        S(dpi, 24),
        SWP_NOZORDER | SWP_NOACTIVATE);
    
    SetWindowPos(hCounter, NULL,
        S(dpi, pos[i].x + pad.x + 24),
        S(dpi, 44),
        S(dpi, 72),
        S(dpi, 24),
        SWP_NOZORDER | SWP_NOACTIVATE);

    StylesFontSet(hLabel, dpi, 14);
    StylesFontSet(hCounter, dpi, 16);
}

static inline void UILayoutCells(HWND hwnd, UINT dpi) {
    static const int size = 72;
    static const int gap = 8;
    static const int count = 3;

    int margin = (WINWIDTH - (size * count + (gap * count - 1))) / 2;
    POINT pos = { 0, 100 };
    
    for (int cy = 0; cy < count; cy++) {
        for (int cx = 0; cx < count; cx++) {
            pos.x = (cx == 0) ? margin : margin + (cx * gap);
            
            HWND hCell = GetDlgItem(hwnd, UI_CELL + cx + (count * cy));

            SetWindowPos(hCell, NULL,
                S(dpi, pos.x + (size * cx)),
                S(dpi, pos.y),
                S(dpi, size),
                S(dpi, size),
                SWP_NOZORDER | SWP_NOACTIVATE);

            StylesFontSet(hCell, dpi, 36);
        }
        pos.y += size + gap;
    }
}

static inline void UILayoutStatusBar(HWND hwnd, UINT dpi) {
    HWND hStatusBar = GetDlgItem(hwnd, UI_STATUSBAR);

    SetWindowPos(hStatusBar, NULL,
        S(dpi, 8),
        S(dpi, WINHEIGHT - 36),
        S(dpi, WINWIDTH - 16),
        S(dpi, 24),
        SWP_NOZORDER | SWP_NOACTIVATE);
    
    StylesFontSet(hStatusBar, dpi, 18);
}

void UILayoutControls(HWND hwnd, UINT dpi) {
    UILayoutPlayerCounter(hwnd, dpi, 1);
    UILayoutPlayerCounter(hwnd, dpi, 2);
    UILayoutCells(hwnd, dpi);
    UILayoutStatusBar(hwnd, dpi);
}

/* -------------------------------------------------------------------------- */
/*                                 Color logic                                */
/* -------------------------------------------------------------------------- */

HBRUSH UIColorTextControls(int controlId, HDC hdcStatic) {
    switch (controlId) {
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
    return CreateSolidBrush(COLOR_BG);
}

HBRUSH UIColorButtonControls(void) {
    return CreateSolidBrush(COLOR_BG);
}

/* -------------------------------------------------------------------------- */
/*                           Custom control drawing                           */
/* -------------------------------------------------------------------------- */

DWORD UICustomDraw(LPNMCUSTOMDRAW nmcd, int dpi) {
    static const UINT bufferSize = 50;
    HPEN pen;
	HBRUSH brush;
	char szWndText[bufferSize];
	int nTextLength;
	
	if (nmcd->hdr.idFrom >= UI_CELL &&
		nmcd->hdr.idFrom < UI_STATUSBAR) {
		
		pen = CreatePen(PS_NULL, 0, 0);
		
		switch (GameCellGetState(nmcd->hdr.idFrom)) {
			case PLAYER_TURN_ONE: {
				SetTextColor(nmcd->hdc, COLOR_ACCENT_RED);
				brush = CreateSolidBrush(COLOR_CELL_RED);
				break;
			}
			case PLAYER_TURN_TWO: {
				SetTextColor(nmcd->hdc, COLOR_ACCENT_BLUE);
				brush = CreateSolidBrush(COLOR_CELL_BLUE);
				break;
			}
			default: {
				if (nmcd->uItemState & CDIS_SELECTED) {
					brush = CreateSolidBrush(COLOR_CELL_ACTIVE);
				}
				else if (nmcd->uItemState & CDIS_HOT) {
					brush = CreateSolidBrush(COLOR_CELL_HOVER);
				} else {
					brush = CreateSolidBrush(COLOR_CELL_NORMAL);
				}
			}
		}

		SelectObject(nmcd->hdc, pen);
		SelectObject(nmcd->hdc, brush);
		RoundRect(nmcd->hdc,
            0, 0,
            WIDTH(nmcd->rc),    // Already scaled
            HEIGHT(nmcd->rc),   // Already scaled
            S(dpi, SIZE_BORDER_RADIUS),
            S(dpi, SIZE_BORDER_RADIUS));
		
		DeleteObject(pen);
		DeleteObject(brush);

		SetBkMode(nmcd->hdc, TRANSPARENT);		
        nTextLength = GetWindowText(nmcd->hdr.hwndFrom, szWndText, bufferSize);
        
		if (nTextLength > 0) {
			DrawText(nmcd->hdc, szWndText, nTextLength, &nmcd->rc, FLAGS_TEXT_DRAW);
		}
		
		return CDRF_SKIPDEFAULT;
	}
	
	return CDRF_DODEFAULT;
}

/* -------------------------------------------------------------------------- */
/*                                Other helpers                               */
/* -------------------------------------------------------------------------- */

void UIReshapeWindow(HWND hWindow, const RECT *rc) {
    SetWindowPos(hWindow, NULL,
        rc->left,
        rc->top,
        rc->right - rc->left,
        rc->bottom - rc->top,
        SWP_NOZORDER | SWP_NOACTIVATE);
}

void UIInitWindowSize(HWND hwnd, UINT dpi) {
    RECT rc = { 0, 0, S(dpi, WINWIDTH), S(dpi, WINHEIGHT) };

    DWORD style = (DWORD) GetWindowLongPtr(hwnd, GWL_STYLE);
    DWORD exStyle = (DWORD) GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    
    AdjustWindowRectExForDpi(&rc, style, FALSE, exStyle, dpi);

    SetWindowPos(hwnd, NULL,
        rc.left,
        rc.top,
        rc.right - rc.left,
        rc.bottom - rc.top,
        SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}
