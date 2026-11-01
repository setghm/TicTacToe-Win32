#include "framework.h"
#include "resource.h"
#include "game.h"
#include "styles.h"

#define FONT_FACE "Soup of Justice"
#define FONT_CACHE_LIMIT 16

static HANDLE hFontResource = NULL;

static struct {
	int size;
	UINT dpi;
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

HFONT StylesFontGet(UINT dpi, int size) {
	for (int i = 0; i < fontCacheLength; i++) {
		if (fontCache[i].size == size && fontCache[i].dpi == dpi) {
			return fontCache[i].hFont;
		}
	}

	LOGFONT lf = {0};
	lf.lfHeight = -MulDiv(size, dpi, 72);
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
		fontCache[fontCacheLength].dpi = dpi;
		fontCache[fontCacheLength].hFont = hFont;
		fontCacheLength++;
	}

	return hFont;
}

void StylesFontSet(HWND hwnd, UINT dpi, int size) {
	SendMessage(hwnd, WM_SETFONT, (WPARAM) StylesFontGet(dpi, size), (LPARAM) TRUE);
}

void StylesCustomizeTitleBar(HWND hwnd) {
	if (!hwnd) return;

	BOOL dark = TRUE;
	COLORREF caption = COLOR_BG;
	COLORREF text = COLOR_TEXT;
	COLORREF border = COLOR_BG;

	DwmSetWindowAttribute(hwnd, DWMA_USE_IMMERSIVE_DARK_MODE, &dark, sizeof(dark));
	DwmSetWindowAttribute(hwnd, DWMA_CAPTION_COLOR, &caption, sizeof(caption));
	DwmSetWindowAttribute(hwnd, DWMA_TEXT_COLOR, &text, sizeof(text));
	DwmSetWindowAttribute(hwnd, DWMA_BORDER_COLOR, &border, sizeof(border));
}
