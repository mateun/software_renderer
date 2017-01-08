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

/**
 * This method is taken from Andre LaMothes Book Tricks of the Windows Game Programming Gurus, 
 * all kudos to him!
 */
void Renderer2D::drawLine(Vec3<int> start, Vec3<int> end, uint32_t* vidmem, int lpitch, uint32_t color) {
	int x0, y0; // starting position 
	int x1, y1; // ending position

	x0 = start.x();
	y0 = start.y();
	x1 = end.x();
	y1 = end.y();

	lpitch = lpitch >> 2;
	
	// this function draws a line from xo,yo to x1,y1 using differential error
	// terms (based on Bresenahams work)

	int dx,             // difference in x's
		dy,             // difference in y's
		dx2,            // dx,dy * 2
		dy2,
		x_inc,          // amount in pixel space to move during drawing
		y_inc,          // amount in pixel space to move during drawing
		error,          // the discriminant i.e. error i.e. decision variable
		index;          // used for looping

	// pre-compute first pixel address in video buffer
	vidmem = vidmem + x0 + (y0*lpitch);

	// compute horizontal and vertical deltas
	dx = x1 - x0;
	dy = y1 - y0;

	// test which direction the line is going in i.e. slope angle
	if (dx >= 0)
	{
		x_inc = 1;

	} // end if line is moving right
	else
	{
		x_inc = -1;
		dx = -dx;  // need absolute value

	} // end else moving left

	// test y component of slope

	if (dy >= 0)
	{
		y_inc = lpitch;
	} // end if line is moving down
	else
	{
		y_inc = -lpitch;
		dy = -dy;  // need absolute value

	} // end else moving up

	// compute (dx,dy) * 2
	dx2 = dx << 1;
	dy2 = dy << 1;

	// now based on which delta is greater we can draw the line
	if (dx > dy)
	{
		// initialize error term
		error = dy2 - dx;

		// draw the line
		for (index = 0; index <= dx; index++)
		{
			// set the pixel
			*vidmem = color;

			// test if error has overflowed
			if (error >= 0)
			{
				error -= dx2;

				// move to next line
				vidmem += y_inc;

			} // end if error overflowed

			// adjust the error term
			error += dy2;

			// move to the next pixel
			vidmem += x_inc;

		} // end for

	} // end if |slope| <= 1
	else
	{
		// initialize error term
		error = dx2 - dy;

		// draw the line
		for (index = 0; index <= dy; index++)
		{
			// set the pixel
			*vidmem = color;

			// test if error overflowed
			if (error >= 0)
			{
				error -= dy2;

				// move to next line
				vidmem += x_inc;

			} // end if error overflowed

			// adjust the error term
			error += dx2;

			// move to the next pixel
			vidmem += y_inc;

		} // end for

	} // end else |slope| > 1

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
