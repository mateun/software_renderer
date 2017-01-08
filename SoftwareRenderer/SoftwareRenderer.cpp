// SoftwareRenderer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "SoftwareRenderer.h"
#include <ddraw.h>

#define MAX_LOADSTRING 100
#define _RGB32BIT(a,r,g,b) ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

// Global Variables:
HINSTANCE hInst;								// current instance
HWND hwnd;
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
LPDIRECTDRAW lpdd = NULL;
LPDIRECTDRAW4 lpdd4 = NULL;
PALETTEENTRY palette[256];
LPDIRECTDRAWPALETTE lpddpal = NULL;
LPDIRECTDRAWSURFACE4 lpddsprimary = NULL;

BOOL initGraphics(HWND hwnd) {
	if (FAILED(DirectDrawCreate(NULL, &lpdd, NULL))){
		OutputDebugString(L"Error creating DDraw object\n");
		exit(1);
	}

	if (FAILED(lpdd->QueryInterface(IID_IDirectDraw4, (LPVOID*)&lpdd4))) {
		OutputDebugString(L"Error creating DD4 object\n");
		exit(1);
	}

	lpdd->Release();
	lpdd = NULL;

	if (FAILED(lpdd4->SetCooperativeLevel(hwnd, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT | DDSCL_ALLOWMODEX))) {
		OutputDebugString(L"Error setting cooperative level\n");
		exit(1);
	}
	
	// Setting palettized 8 bit modes does not work :(
	if (FAILED(lpdd4->SetDisplayMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0))){
		OutputDebugString(L"Error setting display mode\n");
		exit(1);
	}

}

void shutDownGraphics() {

	if (lpddsprimary)
	{
		lpddsprimary->Release();
		lpddsprimary = NULL;
	} // end if

	if (lpdd4) {
		lpdd4->Release();
		lpdd4 = NULL;
	}
}

void initPalette() {
	for (int color = 1; color < 256; color++) {
		palette[color].peRed = rand() % 256;
		palette[color].peGreen = rand() % 256;
		palette[color].peBlue = rand() % 256;
		palette[color].peFlags = PC_NOCOLLAPSE;
	}

	palette[0].peRed = 0;
	palette[0].peGreen = 0;
	palette[0].peBlue = 0;
	palette[0].peFlags = PC_NOCOLLAPSE;

	palette[255].peRed = 255;
	palette[255].peGreen = 255;
	palette[255].peBlue = 255;
	palette[255].peFlags = PC_NOCOLLAPSE;

	if (FAILED(lpdd4->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256 | DDPCAPS_INITIALIZE,
		palette,
		&lpddpal,
		NULL))) {
		OutputDebugString(L"ERROR >>>>>>>>>>> Failed to create palette\n\n");
		exit(1);
	}
}

void initPrimarySurface() {
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));

	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	if (FAILED(lpdd4->CreateSurface(&ddsd, &lpddsprimary, NULL))) {
		OutputDebugString(L"ERROR >>>>>>>>>>> Failed to create primary surface\n\n");
		exit(1);
	}
}

void attachPalette(LPDIRECTDRAWPALETTE palette) {
	// Attach the palette
	if (FAILED((lpddsprimary->SetPalette(palette)))) {
		OutputDebugString(L"ERROR >>>>>>>>>>> Failed to attach palette to primary surface\n\n");
		exit(1);
	}
}

DDSURFACEDESC2 lockSurface(LPDIRECTDRAWSURFACE4 surface) {
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	if (FAILED(surface->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL))) {
		OutputDebugString(L"ERROR >>>>>>>>>>> Failed to lock surface\n\n");
		exit(1);
	}
	return ddsd;
}

void unlockSurface(LPDIRECTDRAWSURFACE4 surface) {
	if (FAILED((surface->Unlock(NULL)))) {
		OutputDebugString(L"ERROR >>>>>>>>>>> Failed to unlock surface\n\n");
		exit(1);
	}
}

void plotPixel8(int x, int y, UCHAR *buffer, int memPitch, UCHAR color) {
	buffer[(y*memPitch) + x] = color;
}

void plotPixel32(int x, int y, UINT32 *buffer, int memPitch, UINT32 color) {
	buffer[(y * memPitch >> 2) + x] = color;
}

void doFrame() {
	DDSURFACEDESC2 ddsd = lockSurface(lpddsprimary);
	plotPixel32(50, 50, (UINT32*) ddsd.lpSurface, ddsd.lPitch, _RGB32BIT(255, 255, 10, 20));
	unlockSurface(lpddsprimary);
}


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SOFTWARERENDERER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SOFTWARERENDERER));

	initGraphics(hwnd);
	//initPalette();
	initPrimarySurface();


	// Main message loop:
	while (true)
	{
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			break;
		}

		doFrame();
	}

	shutDownGraphics();

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SOFTWARERENDERER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
     hInst = hInstance; // Store instance handle in our global variable

   hwnd = CreateWindow(szWindowClass, szTitle, WS_POPUP | WS_VISIBLE,
      0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hInstance, NULL);

   if (!hwnd)
   {
      return FALSE;
   }

   ShowWindow(hwnd, nCmdShow);
   UpdateWindow(hwnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
