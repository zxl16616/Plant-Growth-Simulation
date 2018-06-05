
// FractalTreeView.h : CFractalTreeView 类的接口
//
#include "OpenGL.h"
#pragma once


class CFractalTreeView : public CView
{
protected: // 仅从序列化创建
	CFractalTreeView();
	DECLARE_DYNCREATE(CFractalTreeView)
	COpenGL opengl;
// 特性
public:
	CFractalTreeDoc* GetDocument() const;

// 操作
public:
	void OnInitialUpdate();
	
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
	virtual ~CFractalTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetdata();
	afx_msg void OnReset();
};

#ifndef _DEBUG  // FractalTreeView.cpp 中的调试版本
inline CFractalTreeDoc* CFractalTreeView::GetDocument() const
   { return reinterpret_cast<CFractalTreeDoc*>(m_pDocument); }
#endif

