#pragma once
#include <stdint.h>
#include "renderer2d.h"
#include <vector>
#include "graphtypes.h"
#include <stdexcpt.h>
#include "grapfuncs.h"

class Renderer3D {
public:
	Renderer3D(Renderer2D& renderer2D);

	// We assume the triangle is already transformed 
	// into world and view space
	template<typename T>
	void renderTransformedTriangle(Triangle<T> triangle, uint32_t color) {
		Triangle<float> transformedTri;
		for (int i = 0; i < 3; i++) {
			// make the perspective divide for each vertex
			triangle.vecs[i].z() != 0 ? transformedTri.vecs[i].setX((float)triangle.vecs[i].x() / (float)triangle.vecs[i].z()) : transformedTri.vecs[i].setX(triangle.vecs[i].x());
			triangle.vecs[i].z() != 0 ? (transformedTri.vecs[i].setY((float)triangle.vecs[i].y() / (float)triangle.vecs[i].z())) : transformedTri.vecs[i].setY(triangle.vecs[i].y());

			char buf[100];
			sprintf_s(buf, "vec%d x/y: %f/%f\n",i, transformedTri.vecs[i].x(), transformedTri.vecs[i].y());
			OutputDebugStringA(buf);
		}

		

		if (_renderer2D.GetIsBackBufferLocked() == true) {
			//throw new std::runtime_error("Backbuffer was locked! Only do calls on the 3D renderer when the 2D renderer is not in locked state!");
			OutputDebugString(L"Backbuffer was locked! Only do calls on the 3D renderer when the 2D renderer is not in locked state!");
			exit(1);
		}

		// Before rendering to the screen, the normalized device coordinates need to be transformed to screen coordinates
		Triangle<int> screenTri;
		for (int i = 0; i < 3; i++) {
			screenTri.vecs[i].setX((float)(((float)transformedTri.vecs[i].x() + 1)) * (1024/2));
			screenTri.vecs[i].setY((1 - (float)transformedTri.vecs[i].y()) * (768/2));

			char buf[100];
			sprintf_s(buf, "screen vec%d x/y: %d/%df\n", i, screenTri.vecs[i].x(), screenTri.vecs[i].y());
			OutputDebugStringA(buf);
		}
		
		int lp;
		uint32_t* vidmem = _renderer2D.lockSurface(lp);
		_renderer2D.drawTriangle(screenTri.vecs[0], screenTri.vecs[1], screenTri.vecs[2], vidmem, lp, color);
		_renderer2D.unlockSurface();
	}

	template<typename T>
	void renderTransformedMesh(Mesh<T> mesh) {

	}

private:
	Renderer2D& _renderer2D;
};