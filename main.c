

#include "stdafx.h"
#include "map.h"
#include "display.h"

HDC hDC;						/* device context */
HPALETTE hPalette = 0;			/* custom palette (if needed) */


static struct MINESWEEPER_MAP map;


LONG WINAPI
WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{ 
    static PAINTSTRUCT ps;
    static GLuint    mouseButtonState = 0;	
    static int omx, omy, mx, my;
	static RECT rect;

    switch(uMsg) {
    case WM_PAINT:
		updateDisplay();
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;

    case WM_SIZE:
		GetClientRect (hWnd, &rect);
		changeWindowSize(rect.right, rect.bottom);
		PostMessage(hWnd, WM_PAINT, 0, 0);
		return 0;

    case WM_CHAR:
		switch (wParam) {
		case 27:			/* ESC key */
		    PostQuitMessage(0);
		    break;
		}
		return 0;

    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
	/* if we don't set the capture we won't get mouse move
           messages when the mouse moves outside the window. */
		SetCapture(hWnd);
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (uMsg == WM_LBUTTONDOWN)
		    mouseButtonState |= 1;
		if (uMsg == WM_RBUTTONDOWN)
		    mouseButtonState |= 2;
		mouseButton (uMsg, (short)(LOWORD(lParam)), (short)(HIWORD(lParam)));
		return 0;

    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
	/* remember to release the capture when we are finished. */
		ReleaseCapture();
		mouseButtonState = 0;
		mouseButton (uMsg, (short)(LOWORD(lParam)), (short)(HIWORD(lParam)));
		return 0;

    case WM_MOUSEMOVE:
		if (!mouseButtonState) 
			return 0;

		omx = mx;
		omy = my;
		mx = (short)(LOWORD(lParam));
		my = (short)(HIWORD(lParam));
		if (mouseMove(mouseButtonState, omx - mx, my - omy))
			PostMessage(hWnd, WM_PAINT, 0, 0);
		return 0;

    case WM_PALETTECHANGED:
		if (hWnd == (HWND)wParam)
			break;
			/* fall through to WM_QUERYNEWPALETTE */

    case WM_QUERYNEWPALETTE:
		if (hPalette) {
		    UnrealizeObject(hPalette);
		    SelectPalette(hDC, hPalette, FALSE);
		    RealizePalette(hDC);
		    return TRUE;
		}
		return FALSE;

    case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam); 
} 



HWND
CreateOpenGLWindow(char* title, int x, int y, int width, int height)
{
    int         n, pf;
    HWND        hWnd;
    WNDCLASS    wc;
    LOGPALETTE* lpPal;
    PIXELFORMATDESCRIPTOR pfd;
    static HINSTANCE hInstance = 0;

    /* only register the window class once - use hInstance as a flag. */
    if (!hInstance) {
		hInstance = GetModuleHandle(NULL);
		wc.style         = CS_OWNDC;
		wc.lpfnWndProc   = (WNDPROC)WindowProc;
		wc.cbClsExtra    = 0;
		wc.cbWndExtra    = 0;
		wc.hInstance     = hInstance;
		wc.hIcon         = LoadIcon(NULL, IDI_WINLOGO);
		wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName  = NULL;
		wc.lpszClassName = "WinMine3D";

		if (!RegisterClass(&wc)) {
		    MessageBox(NULL, "RegisterClass() failed:  "
			       "Cannot register window class.", "Error", MB_OK);
		    return NULL;
		}
    }

    hWnd = CreateWindow("WinMine3D", title, WS_OVERLAPPEDWINDOW |
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			x, y, width, height, NULL, NULL, hInstance, NULL);

    if (hWnd == NULL) {
		MessageBox(NULL, "CreateWindow() failed:  Cannot create a window.",
			   "Error", MB_OK);
		return NULL;
    }

    hDC = GetDC(hWnd);

    /* there is no guarantee that the contents of the stack that become
       the pfd are zeroed, therefore _make sure_ to clear these bits. */
    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize        = sizeof(pfd);
    pfd.nVersion     = 1;
    pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | 
		               PFD_GENERIC_ACCELERATED | PFD_DOUBLEBUFFER;
    pfd.iPixelType   = PFD_TYPE_RGBA;
    pfd.cDepthBits   = 32;
    pfd.cColorBits   = 16;

    pf = ChoosePixelFormat(hDC, &pfd);
    if (pf == 0) {
		MessageBox(NULL, "ChoosePixelFormat() failed:  "
		   "Cannot find a suitable pixel format.", "Error", MB_OK); 
		return 0;
    } 
 
    if (SetPixelFormat(hDC, pf, &pfd) == FALSE) {
		MessageBox(NULL, "SetPixelFormat() failed:  "
			   "Cannot set format specified.", "Error", MB_OK);
		return 0;
    } 

    DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    if (pfd.dwFlags & PFD_NEED_PALETTE ||
	pfd.iPixelType == PFD_TYPE_COLORINDEX) {

		n = 1 << pfd.cColorBits;
		if (n > 256) n = 256;

		lpPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) +
					    sizeof(PALETTEENTRY) * n);
		memset(lpPal, 0, sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * n);
		lpPal->palVersion = 0x300;
		lpPal->palNumEntries = n;

		GetSystemPaletteEntries(hDC, 0, n, &lpPal->palPalEntry[0]);
    
		/* if the pixel type is RGBA, then we want to make an RGB ramp,
		   otherwise (color index) set individual colors. */
		if (pfd.iPixelType == PFD_TYPE_RGBA) {
		    int redMask = (1 << pfd.cRedBits) - 1;
		    int greenMask = (1 << pfd.cGreenBits) - 1;
		    int blueMask = (1 << pfd.cBlueBits) - 1;
		    int i;

			/* fill in the entries with an RGB color ramp. */
			for (i = 0; i < n; ++i) {
				lpPal->palPalEntry[i].peRed = 
				    (((i >> pfd.cRedShift)   & redMask)   * 255) / redMask;
				lpPal->palPalEntry[i].peGreen = 
				    (((i >> pfd.cGreenShift) & greenMask) * 255) / greenMask;
				lpPal->palPalEntry[i].peBlue = 
				    (((i >> pfd.cBlueShift)  & blueMask)  * 255) / blueMask;
				lpPal->palPalEntry[i].peFlags = 0;
			}
		} else {
		    lpPal->palPalEntry[0].peRed = 0;
		    lpPal->palPalEntry[0].peGreen = 0;
		    lpPal->palPalEntry[0].peBlue = 0;
		    lpPal->palPalEntry[0].peFlags = PC_NOCOLLAPSE;
		    lpPal->palPalEntry[1].peRed = 255;
		    lpPal->palPalEntry[1].peGreen = 0;
		    lpPal->palPalEntry[1].peBlue = 0;
		    lpPal->palPalEntry[1].peFlags = PC_NOCOLLAPSE;
		    lpPal->palPalEntry[2].peRed = 0;
		    lpPal->palPalEntry[2].peGreen = 255;
		    lpPal->palPalEntry[2].peBlue = 0;
		    lpPal->palPalEntry[2].peFlags = PC_NOCOLLAPSE;
		    lpPal->palPalEntry[3].peRed = 0;
		    lpPal->palPalEntry[3].peGreen = 0;
		    lpPal->palPalEntry[3].peBlue = 255;
		    lpPal->palPalEntry[3].peFlags = PC_NOCOLLAPSE;
		}

		hPalette = CreatePalette(lpPal);
		if (hPalette) {
		    SelectPalette(hDC, hPalette, FALSE);
		    RealizePalette(hDC);
		}
		free(lpPal);
    }

    ReleaseDC(hWnd, hDC);
    return hWnd;
}    



int APIENTRY
WinMain(HINSTANCE hCurrentInst, HINSTANCE hPreviousInst,
	LPSTR lpszCmdLine, int nCmdShow)
{
    HGLRC hRC;							/* opengl context */
    HWND  hWnd;							/* window */
    MSG   msg;							/* message */

    hWnd = CreateOpenGLWindow("WinMine3D", CW_USEDEFAULT, 0, 256, 256);
    if (hWnd == NULL)
		exit(1);

    hDC = GetDC(hWnd);
    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);

	buildIcosahedron (&map);
	//vinsFunction (&map);

	if (!oglInit(map))
		return 1;

    ShowWindow(hWnd, nCmdShow);

    while(GetMessage(&msg, hWnd, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
    }

	oglClose();

    wglMakeCurrent(NULL, NULL);
    ReleaseDC(hWnd, hDC);
    wglDeleteContext(hRC);
    DestroyWindow(hWnd);
    if (hPalette)
		DeleteObject(hPalette);

    return msg.wParam;
}

