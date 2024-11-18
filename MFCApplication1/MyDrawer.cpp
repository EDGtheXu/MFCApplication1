#include "pch.h"
#include "MyDrawer.h"
#include "math.h"
using namespace std;


void MyDrawer::setDC(HDC dc) {
	this->dc = dc;
}

void swap(int* a, int* b) {
	int t = *a;*a = *b;*b = t;
}

// Ïß
void MyDrawer::drawLine_DDA(int x1, int y1, int x2, int y2)
{
	double dx, dy, e, x, y;
	dx = x2 - x1;
	dy = y2 - y1;
	e = max(fabs(dx), fabs(dy));
	dx /= e;
	dy /= e;
	x = x1;
	y = y1;
	for (int i = 0; i <= e; i++) {
		setPixel( x + 0.5, y + 0.5);
		x += dx;
		y += dy;
	}
}


void MyDrawer::drawLine_Mid(int x0, int y0, int x1, int y1)
{
	int dx = abs(x1 - x0), dy = abs(y1 - y0);
	int sx = x0 < x1 ? 1 : -1, sy = y0 < y1 ? 1 : -1;
	int err = dx - dy, e2;

	while (true) {
		setPixel(x0, y0);
		if (x0 == x1 && y0 == y1) break;
		e2 = 2 * err;
		if (e2 > -dy) { err -= dy; x0 += sx; }
		if (e2 < dx) { err += dx; y0 += sy; }
	}

}



// Ô²

void MyDrawer::drawCircle_Mid(int x, int y, int r) {
	int x0 = 0, y0 = r,
		d = 1 - r,
		dx = 3, dy = 2 - r - r;


	setPixel(x0, y0);
	while (x0 < y0) {
		if (d < 0) {
			d += dx;
			dx += 2;
			x0++;
		}
		else {
			d += (dx + dy);
			dx += 2;dy += 2;
			x0++;
			y0--;
		}

		setPixel( x0 + x,  y0 + y);
		setPixel(-x0 + x, -y0 + y);
		setPixel( x0 + x, -y0 + y);
		setPixel(-x0 + x,  y0 + y);

		setPixel( y0 + x,  x0 + y);
		setPixel(-y0 + x, -x0 + y);
		setPixel(-y0 + x,  x0 + y);
		setPixel( y0 + x, -x0 + y);
	}

}

