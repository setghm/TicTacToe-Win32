#include "framework.h"
#include "styles.h"
#include "resource.h"

extern LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    HWND hWnd = NULL;
    WNDCLASSEX wcx = { 0 };
    RECT wrc = { 0 };
    MSG msg = { 0 };

    /* Create and register window class */
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
        MessageBox(0, "Can't register window class", 0, MB_ICONERROR);
        exit(EXIT_FAILURE);
    }

    /* Adjust the size of the window */
    wrc.left = 0;
    wrc.top = 0;
    wrc.right = WINWIDTH;
    wrc.bottom = WINHEIGHT;
    
    AdjustWindowRect(&wrc, WINSTYLE, FALSE);

    /* Create the main window */
    hWnd = CreateWindow(
        APPNAME,
        APPNAME,
        WINSTYLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        WIDTH(wrc),
        HEIGHT(wrc),
        NULL,
        0,
        hInstance,
        NULL
    );

    if (hWnd == NULL) {
        MessageBox(0, "Unable to create the main window", 0, MB_ICONERROR);
        exit(EXIT_FAILURE);
    }

    /* Apply a custom color to title bar (Windows 10 and Windows 11 only) */
    StylesCustomizeTitleBar(hWnd);

    /* Show main window and update it */
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    /* Execute the message loop */
    while (GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}