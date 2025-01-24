#include "framework.h"
#include "resource.h"
#include "game.h"
#include "styles.h"

void StylesInit(void) {
	AddFontResource("GameFont.ttf");
}

HFONT StylesGetFont(int size) {
	HFONT hGameFont;
	int lfWidth, lfHeight;

	lfWidth = 0;
	lfHeight = -MulDiv(size, GetDeviceCaps(CreateCompatibleDC(NULL), LOGPIXELSY), 72);
	
	LOGFONT LogFont = {
		lfHeight, lfWidth, 0, 0, FW_NORMAL,
		FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY, VARIABLE_PITCH | FF_SWISS,
		"Soup of Justice"
	};
	
	hGameFont = CreateFontIndirect(&LogFont);
	
	return hGameFont;
}

void StylesRelease(void) {
	//RemoveFontMemResourceEx(hFont);
	RemoveFontResource("GameFont.ttf");
}

DWORD StylesDrawControls(LPNMCUSTOMDRAW nmcd) {
	HPEN pen;
	HBRUSH brush;
	char szWndText[50];
	int nTextLength;
	
	if (nmcd->hdr.idFrom >= UI_CELL &&
		nmcd->hdr.idFrom < UI_STATUSBAR) {
		
		pen = CreatePen(PS_NULL, 0, 0);
		
		switch (GetCellState(nmcd->hdr.idFrom)) {
			case PLAYER_ONE: {
				SetTextColor(nmcd->hdc, COLOR_ACCENT_RED);
				brush = CreateSolidBrush(COLOR_CELL_RED);
				break;
			}
			case PLAYER_TWO: {
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
				  WIDTH(nmcd->rc),
				  HEIGHT(nmcd->rc),
				  SIZE_BORDER_RADIUS,
				  SIZE_BORDER_RADIUS);
		
		DeleteObject(pen);
		DeleteObject(brush);

		SetBkMode(nmcd->hdc, TRANSPARENT);		
		nTextLength = GetWindowText(nmcd->hdr.hwndFrom,
									szWndText,
									50);
		if (nTextLength > 0) {
			DrawText(nmcd->hdc,
					 szWndText,
					 nTextLength,
					 &nmcd->rc,
					 FLAGS_TEXT_DRAW);
		}
		
		return CDRF_SKIPDEFAULT;
	}
	
	return CDRF_DODEFAULT;
}
