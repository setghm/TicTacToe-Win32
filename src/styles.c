#include "framework.h"
#include "resource.h"
#include "game.h"
#include "styles.h"

#define FONT_FACE "Soup of Justice"
#define FONT_CACHE_LIMIT 16

static HANDLE hFontResource = NULL;

static struct {
	int size;
	HFONT hFont;
} fontCache[FONT_CACHE_LIMIT];

static int fontCacheLength = 0;

void StylesFontInit(void) {
	HINSTANCE hInstance = GetModuleHandle(NULL);
	HRSRC hRes = FindResource(hInstance, MAKEINTRESOURCE(IDR_FONT_GAME), RT_RCDATA);
	if (!hRes) return;

	HGLOBAL hMem = LoadResource(hInstance, hRes);
	if (!hMem) return;

	void *data = LockResource(hMem);
	DWORD size = SizeofResource(hInstance, hRes);
	if (!data || !size) return;

	DWORD fontsInstalled = 0;
	hFontResource = AddFontMemResourceEx(data, size, NULL, &fontsInstalled);
}

HFONT StylesFontGet(int size) {
	for (int i = 0; i < fontCacheLength; i++) {
		if (fontCache[i].size == size) {
			return fontCache[i].hFont;
		}
	}

	HDC hdc = GetDC(NULL);
	int lfHeight = -MulDiv(size, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	ReleaseDC(NULL, hdc);

	LOGFONT lf = {0};
	lf.lfHeight = lfHeight;
	lf.lfWeight = FW_NORMAL;
	lf.lfCharSet = ANSI_CHARSET;
	lf.lfOutPrecision = OUT_TT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = CLEARTYPE_QUALITY;
	lf.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
	lstrcpy(lf.lfFaceName, FONT_FACE);

	HFONT hFont = CreateFontIndirect(&lf);

	if (hFont && fontCacheLength < FONT_CACHE_LIMIT) {
		fontCache[fontCacheLength].size = size;
		fontCache[fontCacheLength].hFont = hFont;
		fontCacheLength++;
	}

	return hFont;
}

void StylesFontCleanup(void) {
	for (int i = 0; i < fontCacheLength; i++) {
		DeleteObject(fontCache[i].hFont);
	}
	fontCacheLength = 0;

	if (hFontResource) {
		RemoveFontMemResourceEx(hFontResource);
		hFontResource = NULL;
	}
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

void StylesCustomizeTitleBar(HWND hWnd) {
	if (!hWnd) return;

	BOOL dark = TRUE;
	COLORREF caption = COLOR_BG;
	COLORREF text = COLOR_TEXT;
	COLORREF border = COLOR_BG;

	DwmSetWindowAttribute(hWnd, DWMA_USE_IMMERSIVE_DARK_MODE, &dark, sizeof(dark));
	DwmSetWindowAttribute(hWnd, DWMA_CAPTION_COLOR, &caption, sizeof(caption));
	DwmSetWindowAttribute(hWnd, DWMA_TEXT_COLOR, &text, sizeof(text));
	DwmSetWindowAttribute(hWnd, DWMA_BORDER_COLOR, &border, sizeof(border));
}
