#include <ddraw.h>
#include <stdint.h>
#include "graphtypes.h"

class Renderer2D {
public:
	Renderer2D(HWND hwnd, uint32_t screenWidth, uint32_t screenHeight);
	~Renderer2D();
	void plotPixel(int x, int y, uint32_t* vidmem, int lPitch, uint32_t color);
	void drawLine(Vec3<int> start, Vec3<int> end, uint32_t color);
	void drawTriangle(Vec3<int> v1, Vec3<int> v2, Vec3<int> v3);
	uint32_t* Renderer2D::lockSurface(int& lPitch);
	void Renderer2D::unlockSurface();
	void Flip();

private:
	uint32_t _screenWidth;
	uint32_t _screenHeight;
	
	// DX stuff
	LPDIRECTDRAW lpdd = NULL;
	LPDIRECTDRAW4 lpdd4 = NULL;
	LPDIRECTDRAWSURFACE4 lpddsprimary = NULL;
	LPDIRECTDRAWSURFACE4 lpddsback = NULL;
};