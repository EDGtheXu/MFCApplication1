#pragma once

#include<vector>
#include <functional>

using namespace std;

typedef struct ET {
	const int ymax;
	const int xmin;
	const float m;
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
	CClientDC* dc;
	HDC hdc;
	COLORREF color;


public:
	void setColor(COLORREF c) { color = c; }
	void setDC(CClientDC* dc) { this->dc = dc;this->hdc = *dc; }
	MyDrawer(CClientDC* dc, COLORREF color) :dc(dc), color(color) {}

	void drawLine_DDA(int x1, int y1, int x2, int y2);
	void drawLine_DDA(CPoint st, CPoint en) { drawLine_DDA(st.x, st.y, en.x, en.y); }

	void drawLine_Mid(int x1, int y1, int x2, int y2);
	void drawLine_Mid(CPoint st, CPoint en) { drawLine_Mid(st.x, st.y, en.x, en.y); }

	void drawCircle_Mid(int x0, int y0, int r);
	void drawCircle_Bresenham(int x0, int y0, int r);

	void drawEllipse_Mid(int x0, int y0, int a, int b);


	inline void setPixel(int x, int y) { SetPixel(hdc, x, y, color); }

	void fillBmp(vector<CPoint> points, int IDB_BMP);
	void fillSolid(vector<CPoint> points, COLORREF color);


	void clip(vector<CPoint> points, int x0, int y0, int x3, int y3);
	void clip(vector<CPoint> points, CPoint p1, CPoint p2) { clip(points,p1.x, p1.y, p2.x, p2.y); }

	void hermite(vector<CPoint> points);
	void bezier3(vector<CPoint> points);
	void bspline4(vector<CPoint> points);



private:
	void fill(vector<CPoint> points, std::function<COLORREF(int, int)> setcolorRecall);
};

