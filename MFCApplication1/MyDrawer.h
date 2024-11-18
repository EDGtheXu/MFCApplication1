#pragma once
class MyDrawer
{
private:
	HDC dc;
	COLORREF color;

public:
	void setDC(HDC dc);
	MyDrawer(HDC dc, COLORREF color) :dc(dc),color(color) {}

	void drawLine_DDA(int x1,int y1,int x2,int y2);
	void drawLine_DDA(CPoint st, CPoint en){drawLine_DDA(st.x, st.y, en.x, en.y);}

	void drawLine_Mid(int x1, int y1, int x2, int y2);
	void drawLine_Mid(CPoint st, CPoint en) { drawLine_Mid(st.x, st.y, en.x, en.y); }

	void drawCircle_Mid(int x, int y, int r);




	inline void setPixel(int x, int y) { SetPixel(dc, x, y, color); }
};

