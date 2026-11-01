#include "framework.h"
#include "ui.h"
#include "styles.h"
#include "resource.h"

LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {
    HWND hwnd = NULL;
    WNDCLASSEX wcx = { 0 };
    MSG msg = { 0 };

    wcx.cbSize = sizeof(WNDCLASSEX);
    wcx.style = CS_HREDRAW | CS_VREDRAW;
    wcx.lpfnWndProc = WinProc;
    wcx.cbClsExtra = 0;
    wcx.cbWndExtra = 0;
    wcx.hInstance = hInstance;
    wcx.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPICON));
    wcx.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPICON));
    wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcx.hbrBackground = CreateSolidBrush(COLOR_BG);
    wcx.lpszMenuName = NULL;
    wcx.lpszClassName = APPNAME;

    if (!RegisterClassEx(&wcx)) {
        MessageBox(0, "Cannot register window class", 0, MB_ICONERROR);
        exit(EXIT_FAILURE);
    }

    hwnd = CreateWindow(
        APPNAME,
        APPNAME,
        WINSTYLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        0,
        hInstance,
        NULL);

    if (hwnd == NULL) {
        MessageBox(0, "Unable to create the main window", 0, MB_ICONERROR);
        exit(EXIT_FAILURE);
    }

    StylesCustomizeTitleBar(hwnd);
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HINSTANCE hInstance;
    static UINT dpi;

    switch (uMsg) {
        case WM_CREATE: {
            hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
            dpi = GetDpiForWindow(hwnd);

			StylesFontInit();
			
            UIInitWindowSize(hwnd, dpi);
            UICreateControls(hwnd, hInstance);
            UILayoutControls(hwnd, dpi);

            GameInit();
            GameStartNew(hwnd);
            break;
        }
        case WM_COMMAND: {
            GameInputCheck(wParam);
            GameBoardUpdate(hwnd);
            GameWinnerCheck(hwnd);
            break;
        }
		case WM_NOTIFY: {
			LPNMHDR nmhdr = (LPNMHDR) lParam;
			if (nmhdr->code & NM_CUSTOMDRAW) {
				return UICustomDraw((LPNMCUSTOMDRAW)nmhdr, dpi);
			}
            break;
		}
		case WM_CTLCOLORSTATIC: {
            HDC hdcStatic = (HDC) wParam;
            int controlId = GetDlgCtrlID((HWND)lParam);
			return (LRESULT) UIColorTextControls(controlId, hdcStatic);
		}
		case WM_CTLCOLORBTN: {
			return (LRESULT) UIColorButtonControls();
		}
        case WM_DPICHANGED: {
            dpi = HIWORD(lParam);
            UIReshapeWindow(hwnd, (const RECT *)lParam);
            UILayoutControls(hwnd, dpi);
            break;
        }
        case WM_DESTROY: {
			StylesFontCleanup();
            PostQuitMessage(0);
            break;
		}
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return FALSE;
}