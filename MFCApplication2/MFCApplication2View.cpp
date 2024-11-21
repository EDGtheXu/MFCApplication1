
// MFCApplication2View.cpp: CMFCApplication2View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCApplication2.h"
#endif

#include "MFCApplication2Doc.h"
#include "MFCApplication2View.h"
#include <vector>
#include "Cube.h"
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CMFCApplication2View

IMPLEMENT_DYNCREATE(CMFCApplication2View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication2View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCApplication2View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_32771, &CMFCApplication2View::OnFillSolid)
	ON_COMMAND(ID_32772, &CMFCApplication2View::OnFillBmp)
	ON_COMMAND(ID_CREATE_CUBE, &CMFCApplication2View::OnCreateCube)
	ON_COMMAND(ID_PROJ_TYPE1, &CMFCApplication2View::OnProjType1)
	ON_COMMAND(ID_PROJ_TYPE2, &CMFCApplication2View::OnProjType2)
END_MESSAGE_MAP()

// CMFCApplication2View 构造/析构

vector<CPoint> pointArr;
CPoint startPoint;
CPoint endPoint;
bool lpress = false;

int distance(CPoint a, CPoint b) {
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

Cube cube1("cube1",Vec3(100,100,0),40);
Matrix proj(4, 4);
Matrix view(4, 4);

CMFCApplication2View::CMFCApplication2View() noexcept
{
	// TODO: 在此处添加构造代码
	m_cursor = AfxGetApp()->LoadStandardCursor(IDC_CROSS);
	SetCursor(m_cursor);

	drawState = DrawingState::DRAW;
	drawType = DrawType::LINE;
	fillType = FillType::FILL_BITMAP;

	
}

CMFCApplication2View::~CMFCApplication2View()
{
}

BOOL CMFCApplication2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMFCApplication2View 绘图

void CMFCApplication2View::OnDraw(CDC* /*pDC*/)
{
	CMFCApplication2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CMFCApplication2View 打印


void CMFCApplication2View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFCApplication2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMFCApplication2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMFCApplication2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CMFCApplication2View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCApplication2View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFCApplication2View 诊断

#ifdef _DEBUG
void CMFCApplication2View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication2Doc* CMFCApplication2View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication2Doc)));
	return (CMFCApplication2Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFCApplication2View 消息处理程序


void CMFCApplication2View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetCursor(m_cursor);

	this->SetCapture();
	startPoint = point;
	endPoint = point;
	lpress = true;
	pointArr.push_back(point);

	CClientDC dc(this);

	if (drawType == DrawType::ELLIPSE) {
		if (pointArr.size() == 2) {
			int a = distance(pointArr.at(0), startPoint);
			int b = distance(pointArr.at(0), endPoint);
			getDraw(dc)->drawEllipse_Mid(pointArr.at(0).x, pointArr.at(0).y, a, b);

		}
		else if (pointArr.size() == 3)
			pointArr.clear();
	}


	CView::OnLButtonDown(nFlags, point);
}



void CMFCApplication2View::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CView::OnRButtonDown(nFlags, point);
}


void CMFCApplication2View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetCursor(m_cursor);
	ReleaseCapture();
	CView::OnLButtonUp(nFlags, point);
}


void CMFCApplication2View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CClientDC dc(this);
	switch (nChar)
	{
	case 'L':drawType = DrawType::LINE; break;
	case 'C':drawType = DrawType::CIRCLE; break;
	case 'E':drawType = DrawType::ELLIPSE; break;
	case 'R':drawType = DrawType::RECTANGLE; break;
	case 'W':
	{
		dc.SetROP2(R2_NOT);
		cube1.render(getDraw(dc), &proj, &view);
		cube1.translate(0, 10, 0);
		cube1.render(getDraw(dc), &proj, &view);
		break;

	}
	case 'S':
	{
		dc.SetROP2(R2_NOT);
		cube1.render(getDraw(dc), &proj, &view);
		cube1.translate(0, -10, 0);
		cube1.render(getDraw(dc), &proj, &view);
		break;

	}
	case 'A':
	{
		dc.SetROP2(R2_NOT);
		cube1.render(getDraw(dc), &proj, &view);
		cube1.translate(-10, 0, 0);
		cube1.render(getDraw(dc), &proj, &view);
		break;

	}
	case 'D':
	{
		dc.SetROP2(R2_NOT);
		cube1.render(getDraw(dc), &proj, &view);
		cube1.translate(10, 0, 0);
		cube1.render(getDraw(dc), &proj, &view);
		break;

	}
	case 'F':
	{
		//getDraw(dc)->fillSolid(pointArr, RGB(255, 50, 50));
		if (pointArr.empty()) break;
		if (fillType == FillType::SOLID)
			getDraw(dc)->fillSolid(pointArr, RGB(0, 255, 0));
		else if (fillType == FillType::FILL_BITMAP)
			getDraw(dc)->fillBmp(pointArr, IDB_MY_NUMBER);
		break;
	}
	case 'Y':
	{
		
		dc.SetROP2(R2_NOT);
		cube1.render(getDraw(dc), &proj, &view);
		cube1.setYRot(cube1.getYRot() + 10);
		proj.set(2, 2, 0);

		cube1.render(getDraw(dc), &proj, &view);
		break;
	}
	case 'X':
	{

		dc.SetROP2(R2_NOT);
		cube1.render(getDraw(dc), &proj, &view);
		cube1.setXRot(cube1.getXRot() + 10);
		cube1.render(getDraw(dc), &proj, &view);
		break;
	}
	default:

		dc.SetROP2(R2_NOT);
		if (drawType == DrawType::LINE) {
			getDraw(dc)->drawLine_DDA(startPoint, endPoint);
		}
		else if (drawType == DrawType::ELLIPSE) {

		}
		else if (drawType == DrawType::CIRCLE) {
			int x = startPoint.x;
			int y = startPoint.y;
			int r = sqrt((endPoint.x - x) * (endPoint.x - x) + (endPoint.y - y) * (endPoint.y - y));
			getDraw(dc)->drawCircle_Mid(x, y, r);
		}
		else if (drawType == DrawType::RECTANGLE) {
			dc.MoveTo(startPoint);
			dc.LineTo(startPoint.x, endPoint.y);
			dc.LineTo(endPoint);
			dc.LineTo(endPoint.x, startPoint.y);
			dc.LineTo(startPoint);
		}

		lpress = false;
		drawType = DrawType::NONE;
		pointArr.clear();
		break;
	}


	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}



void CMFCApplication2View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetCursor(m_cursor);
	CClientDC dc(this);
	if (lpress) {

		if (drawType == DrawType::LINE) {
			dc.SetROP2(R2_NOT);
			getDraw(dc)->drawLine_Mid(startPoint, endPoint);
			getDraw(dc)->drawLine_Mid(startPoint, point);
			endPoint = point;
		}
		else if (drawType == DrawType::CIRCLE) {
			dc.SetROP2(R2_NOT);
			int x = startPoint.x;
			int y = startPoint.y;
			int r = distance(endPoint, startPoint);
			getDraw(dc)->drawCircle_Mid(x, y, r);

			r = sqrt((point.x - x) * (point.x - x) + (point.y - y) * (point.y - y));
			getDraw(dc)->drawCircle_Mid(x, y, r);
			endPoint = point;
		}
		else if (drawType == DrawType::ELLIPSE) {
			if (pointArr.size() == 1) {
				dc.SetROP2(R2_NOT);
				getDraw(dc)->drawLine_Mid(startPoint, endPoint);
				getDraw(dc)->drawLine_Mid(startPoint, point);
				endPoint = point;
			}
			else if (pointArr.size() == 2) {

				getDraw(dc)->drawLine_Mid(pointArr.at(0), startPoint);
				dc.SetROP2(R2_NOT);
				getDraw(dc)->drawLine_Mid(pointArr.at(0), endPoint);
				getDraw(dc)->drawLine_Mid(pointArr.at(0), point);

				int a = distance(pointArr.at(0), startPoint);
				int b = distance(pointArr.at(0), endPoint);
				getDraw(dc)->drawEllipse_Mid(pointArr.at(0).x, pointArr.at(0).y, a, b);

				b = distance(pointArr.at(0), point);
				getDraw(dc)->drawEllipse_Mid(pointArr.at(0).x, pointArr.at(0).y, a, b);

				endPoint = point;
			}

		}
		else if (drawType == DrawType::RECTANGLE) {
			dc.SetROP2(R2_NOT);
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


MyDrawer* CMFCApplication2View::getDraw(HDC dc) {
	if (drawer == nullptr) drawer = new MyDrawer(dc, RGB(0, 0, 0));
	else drawer->setDC(dc);
	return drawer;
}

void CMFCApplication2View::OnFillSolid()
{
	// TODO: 在此添加命令处理程序代码
	fillType = FillType::SOLID;
	drawState = DrawingState::FILL;
}


void CMFCApplication2View::OnFillBmp()
{
	// TODO: 在此添加命令处理程序代码

	fillType = FillType::FILL_BITMAP;
	drawState = DrawingState::FILL;
}




void CMFCApplication2View::OnCreateCube()
{
	// TODO: 在此添加命令处理程序代码


}


void CMFCApplication2View::OnProjType1()
{
	// TODO: 在此添加命令处理程序代码
	proj = Matrix(4, 4);
	proj.set(2, 2, 0);
}


void CMFCApplication2View::OnProjType2()
{
	// TODO: 在此添加命令处理程序代码
	CClientDC dc(this);
	dc.SetROP2(R2_NOT);
	cube1.render(getDraw(dc), &proj, &view);
	float d = 150;
	//proj.set(2,0,cube1.position.x / d);
	//proj.set(2,1,cube1.position.y / d);
	proj.set(2, 2, 0);
	proj.set(2, 3, 1 / d);
	cube1.render(getDraw(dc), &proj, &view);

}
