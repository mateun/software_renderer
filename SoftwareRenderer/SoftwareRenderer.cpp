// SoftwareRenderer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "SoftwareRenderer.h"
#include <renderer2d.h>
#include <renderer3d.h>
#include <grapfuncs.h>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define MAX_LOADSTRING 100


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
Renderer2D* renderer;
Renderer3D* renderer3D;

float angle = 0;

void doFrame() {
	int lPitch;
	uint32_t* vidmem = renderer->lockSurface(lPitch);
	renderer->plotPixel(50, 50, vidmem, lPitch, _RGB32BIT(255, 255, 10, 20));
	renderer->plotPixel(100, 50, vidmem, lPitch, _RGB32BIT(255, 255, 10, 20));
	renderer->plotPixel(150, 50, vidmem, lPitch, _RGB32BIT(255, 255, 10, 20));

	// all things rendered with the 2D renderer are in pixel, screen coordinates (top left is 0/0, x grows right, y grows down)
	renderer->drawLine(Vec3<int>(10, 200, 0), Vec3<int>(100, 200, 0), vidmem, lPitch, _RGB32BIT(255, 200, 100, 200));
	renderer->drawLine(Vec3<int>(10, 200, 0), Vec3<int>(70, 120, 0), vidmem, lPitch, _RGB32BIT(255, 200, 100, 200));
	renderer->drawLine(Vec3<int>(70, 120, 0), Vec3<int>(100, 200, 0), vidmem, lPitch, _RGB32BIT(255, 200, 100, 200));

	renderer->unlockSurface();

	// All 3D rendered things have a different coordinate system: x goes from -1 to 1 (left to right), and y from 1 to -y (top to bottom))
	Triangle<float> tri(Vec3<float>(-0.5f, -0.5f, 2.0f), Vec3<float>(0.5f, -0.5f, 2.0f), Vec3<float>(0.0f, 0.5f, 2.0f));
	
	glm::mat4x4 translation = glm::translate(glm::mat4x4(1), glm::vec3(0.75, -0.55, 0));
	glm::mat4x4 rotation = glm::rotate(glm::mat4x4(1), angle, glm::vec3(0, 0, 1));
	tri = renderer3D->transformTri(tri, rotation);
	renderer3D->renderTransformedTriangle(tri, _RGB32BIT(255, 200, 100, 100));
	angle += 9.2f;
	rotation = glm::rotate(glm::mat4x4(1), angle, glm::vec3(0, 0, 1));
	tri = renderer3D->transformTri(tri, rotation);
	renderer3D->renderTransformedTriangle(tri, _RGB32BIT(255, 255, 255, 255));
	
	
	renderer->Flip();

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

	renderer = new Renderer2D(hwnd, SCREEN_WIDTH, SCREEN_HEIGHT);
	renderer3D = new Renderer3D(*renderer);
	

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
	delete(renderer);
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
