#include "pch.h"
#include "MyDrawer.h"
#include "math.h"
#include <algorithm>
#include <unordered_map>
#include "resource.h"





void swap(int* a, int* b) {
	int t = *a;*a = *b;*b = t;
}

// 线
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
		setPixel(x + 0.5, y + 0.5);
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



// 圆

void MyDrawer::drawCircle_Mid(int x0, int y0, int r) {
	int x = 0, y = r,
		d = 1 - r,
		dx = 3, dy = 2 - r - r;


	setPixel(x, y);
	while (x < y) {
		if (d < 0) {
			d += dx;
			dx += 2;
			x++;
		}
		else {
			d += (dx + dy);
			dx += 2;dy += 2;
			x++;
			y--;
		}

		setPixel(x + x0, y + y0);
		setPixel(-x + x0, -y + y0);
		setPixel(x + x0, -y + y0);
		setPixel(-x + x0, y + y0);

		setPixel(y + x0, x + y0);
		setPixel(-y + x0, -x + y0);
		setPixel(-y + x0, x + y0);
		setPixel(y + x0, -x + y0);
	}

}


void MyDrawer::drawEllipse_Mid(int x0, int y0, int a, int b) {
	int x = 0, y = b;
	double d1 = b * b + a * a * (-b + 0.25);
	while (b * b * (x + 1) < a * a * (y - 0.5)) {
		if (d1 < 0) {
			d1 += b * b * (2 * x + 3);
			x++;
		}
		else {
			d1 += ((b * b * 2 * x + 3) + a * a * (-2 * y + 2));
			x++;y--;
		}
		setPixel(x + x0, y + y0);
		setPixel(x + x0, -y + y0);
		setPixel(-x + x0, y + y0);
		setPixel(-x + x0, -y + y0);
	}

	double d2 = b * b * (x + 0.5) * (x + 0.5) + a * a * (y - 1) * (y - 1) - a * a * b * b;
	while (y > 0) {
		if (d2 < 0) {
			d2 += b * b * (2 * x + 2) + a * a * (-2 * y + 3);
			x++;
			y--;
		}
		else {
			d2 += a * a * (-2 * y + 3);
			y--;
		}
		setPixel(x + x0, y + y0);
		setPixel(x + x0, -y + y0);
		setPixel(-x + x0, y + y0);
		setPixel(-x + x0, -y + y0);
	}
}

void bubbleSort(AET* head, int n) {
	if (!head || !head->next) return;
	for (int i = 0; i < n; ++i) {
		auto pre = head;
		auto cur = head->next;
		bool flag = 0;
		for (int j = 0; j < n - i - 1; ++j) {
			if (cur != nullptr && pre->x > cur->x) {
				swap(pre->x, cur->x);
				swap(pre->m, cur->m);
				swap(pre->ymax, cur->ymax);

				flag = 1;
			}
			pre = pre->next;
			cur = cur->next;
		}
		if (!flag) break;
	}
}
AET* sortList(AET* head) {
	auto p = head;
	int n = 0; // 记录节点个数
	while (p != nullptr) {
		++n;
		p = p->next;
	}
	bubbleSort(head, n);
	return head;
}

void MyDrawer::fill(vector<CPoint> points, std::function<COLORREF(int, int)> setcolorRecall) {

	int _ymin = INT_MAX;
	int _ymax = INT_MIN;
	// 建立ET表
	int count = points.size();
	unordered_map<int, ET*> bucket = unordered_map<int, ET*>();
	CPoint last = points.at(count - 1);
	for (int i = 0;i < count;i++) {
		CPoint now = points.at(i);
		int x1, y1, x2, y2;
		if (last.x > now.x) {
			x1 = now.x;y1 = now.y;x2 = last.x;y2 = last.y;
		}
		else {
			x1 = last.x;y1 = last.y;x2 = now.x;y2 = now.y;
		}
		int ymin = min(y1, y2);
		int ymax = max(y1, y2);
		_ymin = min(ymin, _ymin);
		_ymax = max(ymax, _ymax);
		int x_ymin = y1 > y2 ? x2 : x1;
		float m = (x2 - x1) * 1.0 / (y2 - y1);
		if (bucket.find(ymin) == bucket.end()) {
			bucket.insert(pair<int, ET*>(ymin, new ET(ymax, x_ymin, m, nullptr)));
		}
		else {
			ET* head = bucket.at(ymin);
			while (head->next != nullptr) head = head->next;
			head->next = new ET(ymax, x_ymin, m, nullptr);
		}
		last = now;
	}

	// 扫描

	AET* aet = nullptr;
	for (int i = _ymin;i <= _ymax;i++) {
		AET* rear = aet;
		ET* head = nullptr;
		if (bucket.find(i) != bucket.end())
			head = bucket.at(i);


		while (head) {
			if (aet == nullptr) {
				aet = new AET(head->ymax, head->xmin, head->m, nullptr);
				rear = aet;
			}
			else {
				while (rear->next) rear = rear->next;
				rear->next = new AET(head->ymax, head->xmin, head->m, nullptr);
			}
			head = head->next;
		}
		//排序
		aet = sortList(aet);
		//更新AET
		rear = aet;
		AET* pre = nullptr;
		while (rear) {
			if (rear->ymax < i) {
				rear = rear->next;
				if (pre != nullptr) pre->next = rear;
				else aet = rear;
				continue;
			}
			pre = rear;
			rear = rear->next;
		}

		rear = aet;
		while (rear) {
			rear->x = rear->x + rear->m;
			rear = rear->next;
		}

		// 点亮
		bool set = false;
		rear = aet;
		float s = aet->x - 1;
		while (rear) {
			if (s > rear->x) {
				rear = rear->next;
				set = !set;
				continue;
			}
			if (set) {

				SetPixel(hdc, s, i, setcolorRecall(s, i));
			}
			s++;
		}
	}
}


void MyDrawer::fillSolid(vector<CPoint> points, COLORREF color) {
	fill(points, [color](int, int) {return color;});
}

void MyDrawer::fillBmp(vector<CPoint> points, int IDB_BMP) {
	CBitmap cbmp;
	cbmp.LoadBitmap(IDB_BMP);
	CWindowDC dcDesktop(NULL);
	BITMAP bmp;
	cbmp.GetBitmap(&bmp);

	CDC dcTmp;
	dcTmp.CreateCompatibleDC(&dcDesktop);
	CBitmap* pOldBmp = dcTmp.SelectObject(&cbmp);

	int w = bmp.bmWidth;
	int h = bmp.bmHeight;
	fill(points, [&](int x, int y) {return dcTmp.GetPixel(x % w, y % h);});
}

int getPosEncode(int x,int y,int x0,int y0,int x3,int y3) {
	int code = 0;
	if (y > y3) code |= 8;
	if (y < y0) code |= 4;
	if (x > x3) code |= 2;
	if (x < x0) code |= 1;
	return code;
}

void MyDrawer::clip(vector<CPoint> points, int x0,int y0,int x3,int y3) {
	int size = points.size();
	if (size == 0) return;
	if (x0 == y3 || y0 == y3) return;
	if (x0 > x3) swap(x0, x3);
	if (y0 > y3) swap(y0, y3);

	CPoint* last = &points.at(points.size() - 1);
	int x1 = last->x, y1 = last->y, x2, y2;
	int code1 = getPosEncode(x1, y1, x0, y0, x3, y3);
	int code2;
	COLORREF incolor = RGB(0,255,0);
	COLORREF outcolor = RGB(0, 0, 255);

	for (int i = 0; i < size; i++) {
		CPoint* now = &points.at(i);
		x2 = now->x;
		y2 = now->y;
		code2 = getPosEncode(x2, y2, x0, y0, x3, y3);

		int c1 = code1, c2 = code2, t1 = x1, t2 = y1, t3 = x2, t4 = y2;
		while (1) {
			if ((c1 & c2) != 0) {
				color = outcolor;
				drawLine_Mid(x1, y1, x2, y2);
				break;
			}

			if (c1 == 0 && c2 == 0) {
				color = incolor;
				drawLine_Mid(x1, y1, x2, y2);

				break;
			}
			int code = c1 ? c1 : c2;
			
			int xIntersect, yIntersect;
			if (code & 1) { // left
				xIntersect = x0;
				yIntersect = t2 + (t4 - t2) * (x0 - t1) / (t3 - t1);
			}
			else if (code & 2) { // right
				xIntersect = x3;
				yIntersect = t2 + (t4 - t2) * (x3 - t1) / (t3 - t1);
			}
			else if (code & 4) { // bottom
				yIntersect = y0;
				xIntersect = t1 + (t3 - t1) * (y0 - t2) / (t4 - t2);
			}
			else { // top
				yIntersect = y3;
				xIntersect = t1 + (t3 - t1) * (y3 - t2) / (t4 - t2);
			}

			if (code == c1)
			{
				t1 = xIntersect;
				t2 = yIntersect;
				c1 = getPosEncode(t1, t2, x0, y0, x3, y3);
			}
			else
			{
				t3 = xIntersect;
				t4 = yIntersect;
				c2 = getPosEncode(t3, t4, x0, y0, x3, y3);
			}

			if (c1 == 0 && c2 == 0) {
				
				color = RGB(255,255,255);
				drawLine_Mid(x1, y1, x2, y2);
				
				color = incolor;
				drawLine_Mid(t1, t2, t3, t4);

				color = outcolor;
				drawLine_Mid(x1, y1, t1, t2);

				color = outcolor;
				drawLine_Mid(x2, y2, t3, t4);

				break;
			}
		}

		code1 = code2;
		last = now;
		x1 = x2;
		y1 = y2;
	}

	color = RGB(0, 0, 0);
}
