#include "pch.h"
#include "MyDrawer.h"
#include "math.h"

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
		SetPixel(dc, x + 0.5, y + 0.5,color);
		x += dx;
		y += dy;
	}
}

void MyDrawer::drawLine_Mid(int x1, int y1, int x2, int y2)
{
}
