
// MFCApplication1View.cpp: CMFCApplication1View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCApplication1.h"
#endif

#include "MFCApplication1Doc.h"
#include "MFCApplication1View.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <vector>
using namespace std;


// CMFCApplication1View
enum DrawingState {
	DRAW, FILL
};

enum FillType
{
	SOLID, SHADOW, FILL_BITMAP
};
DrawingState drawState = DRAW;
FillType fillType = SOLID;


IMPLEMENT_DYNCREATE(CMFCApplication1View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication1View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCApplication1View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_SOLID, &CMFCApplication1View::OnSolid)

	ON_COMMAND(ID_LINE, &CMFCApplication1View::OnLine)
	ON_COMMAND(ID_CIRCLE, &CMFCApplication1View::OnCircle)
	ON_COMMAND(ID_ELLISEP, &CMFCApplication1View::OnEllisep)
	ON_COMMAND(ID_RECTANGLE, &CMFCApplication1View::OnRectangle)
	ON_COMMAND(ID_NONE, &CMFCApplication1View::OnNone)
	ON_COMMAND(ID_SHADOW, &CMFCApplication1View::OnShadow)
	ON_COMMAND(ID_BITMAP, &CMFCApplication1View::OnBitmap)
END_MESSAGE_MAP()

// CMFCApplication1View 构造/析构

vector<CPoint> pointArr;
CMFCApplication1View::CMFCApplication1View() noexcept
{
	// TODO: 在此处添加构造代码
	m_cursor = AfxGetApp()->LoadStandardCursor(IDC_CROSS);
	SetCursor(m_cursor);
	drawType = LINE;

}

CMFCApplication1View::~CMFCApplication1View()
{
}

BOOL CMFCApplication1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	return CView::PreCreateWindow(cs);
}

// CMFCApplication1View 绘图

void CMFCApplication1View::OnDraw(CDC* /*pDC*/)
{
	CMFCApplication1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CMFCApplication1View 打印


void CMFCApplication1View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFCApplication1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMFCApplication1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMFCApplication1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CMFCApplication1View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCApplication1View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFCApplication1View 诊断

#ifdef _DEBUG
void CMFCApplication1View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication1Doc* CMFCApplication1View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication1Doc)));
	return (CMFCApplication1Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFCApplication1View 消息处理程序
CPoint startPoint;
CPoint endPoint;
bool lpress = false;

void CMFCApplication1View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetCursor(m_cursor);
	CClientDC dc(this);
	if (lpress) {
		dc.SetROP2(R2_NOT);
		if (drawType == LINE) {
			getDraw(dc)->drawLine_Mid(startPoint, endPoint);
			getDraw(dc)->drawLine_Mid(startPoint, point);
			endPoint = point;
		}
		else if (drawType == ELLIPSE) {
			int x = startPoint.x;
			int y = startPoint.y;
			int r = sqrt((endPoint.x - x) * (endPoint.x - x) + (endPoint.y - y) * (endPoint.y - y));
			getDraw(dc)->drawCircle_Mid(x, y, r);

			r = sqrt((point.x - x) * (point.x - x) + (point.y - y) * (point.y - y));
			getDraw(dc)->drawCircle_Mid(x, y, r);
			/*
			dc.Arc((int)startPoint.x, (int)startPoint.y, (int)endPoint.x, (int)endPoint.y,
				(int)startPoint.x, (int)startPoint.y, (int)endPoint.x, (int)endPoint.y);
			dc.Arc((int)startPoint.x, (int)startPoint.y, (int)endPoint.x, (int)endPoint.y,
				(int)endPoint.x, (int)endPoint.y,(int)startPoint.x, (int)startPoint.y);


			dc.Arc((int)startPoint.x, (int)startPoint.y, (int)point.x, (int)point.y,
				(int)startPoint.x, (int)startPoint.y,(int)point.x, (int)point.y);
			dc.Arc((int)startPoint.x, (int)startPoint.y, (int)point.x, (int)point.y,
				(int)point.x, (int)point.y, (int)startPoint.x, (int)startPoint.y);
			*/
			endPoint = point;
		}
		else if (drawType == RECTANGLE) {
			dc.MoveTo(startPoint);
			dc.LineTo(startPoint.x, endPoint.y);
			dc.LineTo(endPoint);
			dc.LineTo(endPoint.x, startPoint.y);
			dc.LineTo(startPoint);

			dc.MoveTo(startPoint);
			dc.LineTo(startPoint.x, point.y);
			dc.LineTo(point);
			dc.LineTo(point.x, startPoint.y);
			dc.LineTo(startPoint);
			endPoint = point;

		}

	}

	CView::OnMouseMove(nFlags, point);
}


void CMFCApplication1View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//ptArray.Add(point);
	//m_Newpoint = point;
	//m_nCount++;

	SetCursor(m_cursor);

	this->SetCapture();
	startPoint = point;
	endPoint = point;
	lpress = true;
	pointArr.push_back(point);


	CView::OnLButtonDown(nFlags, point);
}

void CMFCApplication1View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetCursor(m_cursor);
	ReleaseCapture();

	/*
	CClientDC dc(this);
	if (drawType == LINE) {
		dc.MoveTo(startPoint);
		dc.LineTo(endPoint);
	}
	*/

	CView::OnLButtonUp(nFlags, point);
}


void CMFCApplication1View::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值


	CView::OnRButtonDown(nFlags, point);
}




void CMFCApplication1View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CClientDC dc(this);
	switch (nChar)
	{
	case 'L':drawType = LINE; break;
	case 'C':drawType = CIRCLE; break;
	case 'E':drawType = ELLIPSE; break;
	case 'R':drawType = RECTANGLE; break;

	case 'T':
		getDraw(dc)->drawLine_DDA(100,100,200,400);
	default:

		dc.SetROP2(R2_NOT);
		if (drawType == LINE) {
			getDraw(dc)->drawLine_DDA(startPoint, endPoint);
		}else if (drawType == ELLIPSE) {
			int x = startPoint.x;
			int y = startPoint.y;
			int r = sqrt((endPoint.x - x) * (endPoint.x - x) + (endPoint.y - y) * (endPoint.y - y));
			getDraw(dc)->drawCircle_Mid(x, y, r);
		}
		else if (drawType == RECTANGLE) {
			dc.MoveTo(startPoint);
			dc.LineTo(startPoint.x, endPoint.y);
			dc.LineTo(endPoint);
			dc.LineTo(endPoint.x, startPoint.y);
			dc.LineTo(startPoint);
		}

		lpress = false;
		drawType = NONE;
		pointArr.clear();
		break;
	}


	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}






void CMFCApplication1View::OnLine()
{
	// TODO: 在此添加命令处理程序代码
	drawType = LINE;
	drawState = DRAW;

}


void CMFCApplication1View::OnCircle()
{
	// TODO: 在此添加命令处理程序代码
	drawType = CIRCLE;
	drawState = DRAW;

}


void CMFCApplication1View::OnEllisep()
{
	// TODO: 在此添加命令处理程序代码
	drawType = ELLIPSE;
	drawState = DRAW;

}


void CMFCApplication1View::OnRectangle()
{
	// TODO: 在此添加命令处理程序代码
	drawType = RECTANGLE;
	drawState = DRAW;

}


void CMFCApplication1View::OnNone()
{
	// TODO: 在此添加命令处理程序代码
	drawType = NONE;
	drawState = DRAW;
}




void CMFCApplication1View::OnSolid()
{
	// TODO: 在此添加命令处理程序代码
	fillType = SOLID;
	drawState = FILL;
}

void CMFCApplication1View::OnShadow()
{
	// TODO: 在此添加命令处理程序代码
	fillType = SHADOW;
	drawState = FILL;

}


void CMFCApplication1View::OnBitmap()
{
	// TODO: 在此添加命令处理程序代码
	fillType = FILL_BITMAP;
	drawState = FILL;

}

MyDrawer* CMFCApplication1View::getDraw(HDC dc) {
	if (drawer == nullptr) drawer = new MyDrawer(dc, RGB(0, 0, 0));
	else drawer->setDC(dc);
	return drawer;
}