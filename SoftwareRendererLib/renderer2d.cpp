#include "stdafx.h"
#include "renderer2d.h"

Renderer2D::Renderer2D(HWND hwnd, uint32_t screenWidth, uint32_t screenHeight) : _screenWidth(screenWidth), _screenHeight(screenHeight) {
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
	if (FAILED(lpdd4->SetDisplayMode(_screenWidth, _screenHeight, 32, 0, 0))){
		OutputDebugString(L"Error setting display mode\n");
		exit(1);
	}

	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));

	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.dwBackBufferCount = 1;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_COMPLEX | DDSCAPS_FLIP;
	if (FAILED(lpdd4->CreateSurface(&ddsd, &lpddsprimary, NULL))) {
		OutputDebugString(L"ERROR >>>>>>>>>>> Failed to create primary surface\n\n");
		exit(1);
	}

	ddsd.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;
	if (FAILED(lpddsprimary->GetAttachedSurface(&ddsd.ddsCaps, &lpddsback))) {
		OutputDebugString(L"ERROR >>>>>>>>>>> Failed to create primary surface\n\n");
		exit(1);
	}

}

uint32_t* Renderer2D::lockSurface(int& lPitch) {
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	if (FAILED(lpddsback->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL))) {
		OutputDebugString(L"ERROR >>>>>>>>>>> Failed to lock surface\n\n");
		exit(1);
	}
	lPitch = ddsd.lPitch;
	return (uint32_t*) ddsd.lpSurface;
}



void Renderer2D::unlockSurface() {
	if (FAILED((lpddsback->Unlock(NULL)))) {
		OutputDebugString(L"ERROR >>>>>>>>>>> Failed to unlock surface\n\n");
		exit(1);
	}
}
void Renderer2D::plotPixel(int x, int y, uint32_t* vidmem, int lPitch, uint32_t color) {
	vidmem[(y * lPitch >> 2) + x] = color;
}

void Renderer2D::Flip() {
	while (FAILED(lpddsprimary->Flip(NULL, DDFLIP_WAIT)));
}

Renderer2D::~Renderer2D() {

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
