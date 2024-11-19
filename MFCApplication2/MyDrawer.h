#pragma once

#include<vector>
#include <functional>

using namespace std;

typedef struct ET {
	int ymax;
	int xmin;
	float m;
	ET* next;
	ET(int ymax, int xmin, float m, ET* next) :ymax(ymax), xmin(xmin), m(m), next(next) {}
};

typedef struct AET {
	int ymax;
	float x;
	float m;
	AET* next;
	AET(int y, int x, float m, AET* next) :ymax(y), x(x), m(m), next(next) {}
};



class MyDrawer
{
private:
	HDC dc;
	COLORREF color;


public:
	void setDC(HDC dc);
	MyDrawer(HDC dc, COLORREF color) :dc(dc), color(color) {}

	void drawLine_DDA(int x1, int y1, int x2, int y2);
	void drawLine_DDA(CPoint st, CPoint en) { drawLine_DDA(st.x, st.y, en.x, en.y); }

	void drawLine_Mid(int x1, int y1, int x2, int y2);
	void drawLine_Mid(CPoint st, CPoint en) { drawLine_Mid(st.x, st.y, en.x, en.y); }

	void drawCircle_Mid(int x0, int y0, int r);

	void drawEllipse_Mid(int x0, int y0, int a, int b);


	inline void setPixel(int x, int y) { SetPixel(dc, x, y, color); }

	void fillBmp(vector<CPoint> points, int IDB_BMP);
	void fillSolid(vector<CPoint> points, COLORREF color);

private:
	void fill(vector<CPoint> points, std::function<COLORREF(int, int)> setcolorRecall);

	void solidColorFillRecall(int a, int b);
	void fillBmpFillRecall(int a, int b);
};

