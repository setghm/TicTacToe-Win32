#include "framework.h"
#include "styles.h"

extern LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    MSG msg;
    HWND hWnd;
    WNDCLASS wc;
    RECT wrc;

    /* Create and register window class */
    wc.style = 0;
    wc.lpfnWndProc = WinProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(COLOR_BG);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = APPNAME;

    if (!RegisterClass(&wc)) {
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