
// MFCApplication2View.h: CMFCApplication2View 类的接口
//

#pragma once

#include "MyDrawer.h"
enum class DrawingState {
	DRAW, FILL
};
enum class DrawType {
	NONE, LINE, RECTANGLE, CIRCLE, ELLIPSE
};
enum class FillType {
	SOLID, SHADOW, FILL_BITMAP
};

class CMFCApplication2View : public CView
{
protected: // 仅从序列化创建
	CMFCApplication2View() noexcept;
	DECLARE_DYNCREATE(CMFCApplication2View)

// 特性
public:
	CMFCApplication2Doc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMFCApplication2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	MyDrawer* getDraw(HDC dc);
	MyDrawer* drawer = nullptr;

	HCURSOR m_cursor;
	DrawingState drawState;
	DrawType drawType;
	FillType fillType;
	afx_msg void OnFillSolid();
	afx_msg void OnFillBmp();
	afx_msg void OnCreateCube();
	afx_msg void OnProjType1();
	afx_msg void OnProjType2();
};

#ifndef _DEBUG  // MFCApplication2View.cpp 中的调试版本
inline CMFCApplication2Doc* CMFCApplication2View::GetDocument() const
   { return reinterpret_cast<CMFCApplication2Doc*>(m_pDocument); }
#endif

