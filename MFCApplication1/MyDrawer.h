#pragma once
class MyDrawer
{
private:
	HDC dc;
	COLORREF color;

public:
	MyDrawer(HDC dc, COLORREF color) :dc(dc),color(color) {}
	void drawLine_DDA(int x1, int y1, int x2, int y2);
	void drawLine_Mid(int x1, int y1, int x2, int y2);



};

