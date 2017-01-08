#include "graphtypes.h"
#include <stdint.h>

void plotPixel8(int x, int y, uint8_t *buffer, int memPitch, uint8_t color) {
	buffer[(y*memPitch) + x] = color;
}

void plotPixel32(int x, int y, uint32_t *buffer, int memPitch, uint32_t color) {
	buffer[(y * memPitch >> 2) + x] = color;
}

void drawLine(Vec3<int> start, Vec3<int> end) {
	// first, only works in the first quadrant
	Vec3<int> diff = end - start;
	int error = 0;

	for (int i = 0; i < diff.x(); i++) {
		plotPixel32(start.x() + i, start.y());
	}
}