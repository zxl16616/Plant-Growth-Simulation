
// FractalTreeView.cpp : CFractalTreeView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "FractalTree.h"
#endif

#include "FractalTreeDoc.h"
#include "FractalTreeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "OpenGL.h"
#include "MyDialog.h"

// CFractalTreeView

IMPLEMENT_DYNCREATE(CFractalTreeView, CView)

BEGIN_MESSAGE_MAP(CFractalTreeView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_setData, &CFractalTreeView::OnSetdata)
	ON_COMMAND(ID_Reset, &CFractalTreeView::OnReset)
END_MESSAGE_MAP()

// CFractalTreeView 构造/析构

CFractalTreeView::CFractalTreeView()
{
	// TODO: 在此处添加构造代码

}

CFractalTreeView::~CFractalTreeView()
{
}

BOOL CFractalTreeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);

}

// CFractalTreeView 绘制

void CFractalTreeView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
 	// TODO: 写入最终选择模式代码之后移除此代码
	//m_pSelection = NULL;    // 初始化所选内容
	opengl.SetupPixelFormat(::GetDC(GetSafeHwnd()));
	opengl.SetupRC();
}


void CFractalTreeView::OnDraw(CDC* /*pDC*/)
{
	CFractalTreeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码

	opengl.RenderSence();  //绘制场景
	//glFlush();
	
}


// CFractalTreeView 打印

BOOL CFractalTreeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CFractalTreeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CFractalTreeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CFractalTreeView 诊断

#ifdef _DEBUG
void CFractalTreeView::AssertValid() const
{
	CView::AssertValid();
}

void CFractalTreeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFractalTreeDoc* CFractalTreeView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFractalTreeDoc)));
	return (CFractalTreeDoc*)m_pDocument;
}
#endif //_DEBUG


// CFractalTreeView 消息处理程序


void CFractalTreeView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	opengl.ChangeSize(cx, cy);
	
}


void CFractalTreeView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int n = 0;
	int m = 0;
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	if (nChar == VK_UP) {
		RedrawWindow();
		opengl.SpecialKeys(nChar, 0, 0);
	}
		

	if (nChar == VK_DOWN) {
		opengl.SpecialKeys(nChar, 0, 0);
		RedrawWindow();
	}

	if (nChar == VK_LEFT) {
		opengl.SpecialKeys(nChar, 0, 0);
		RedrawWindow();
	}

	if (nChar == VK_RIGHT) {
		
		opengl.SpecialKeys(nChar, 0, 0);
		RedrawWindow();
	}

	if (nChar == VK_SPACE) {
		while (n < 6) {
			opengl.tree.setN(n);
			opengl.tree.setbaseTall(n + m);
			if(n < 3)
				m = m + 1;
			RedrawWindow();
			n++;
		}
	}

	if (nChar == 82) {
		opengl.tree.setbaseTall(8);
		opengl.tree.setbaseR(6);
		opengl.tree.setAngle(55);
		opengl.tree.setleaf(9);
		opengl.tree.setM(0.65);
		opengl.tree.setN(5);
		opengl.tree.setQ(0.5);
		Tree::img = opengl.tree.loadPNGTexture("leaf.png");
		Tree::shape = 0;
		RedrawWindow();
	}
	
}


BOOL CFractalTreeView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return true;
}


void CFractalTreeView::OnSetdata()
{
	// TODO: 在此添加命令处理程序代码
	CMyDialog dlg;
	//读取当前的参数
	dlg.m_n = opengl.tree.getN();
	dlg.m_high = opengl.tree.getbaseTall();
	dlg.m_M = opengl.tree.getm();
	dlg.m_angle = opengl.tree.getAngle();
	dlg.m_leaf = opengl.tree.getleaf();
	dlg.m_width = opengl.tree.getbaseR();
	dlg.m_q = opengl.tree.getq();
	UpdateData(false);
	if (CMyDialog::radioState == 1) {
		dlg.m_iRadio = 1;
		//CheckRadioButton(IDC_RADIO3, IDC_RADIO4, IDC_RADIO4);
	}
	

	UpdateData(true);
	if (IDOK == dlg.DoModal()) {
		opengl.tree.setN(dlg.m_n);
		opengl.tree.setbaseTall(dlg.m_high);
		opengl.tree.setM(dlg.m_M);
		opengl.tree.setAngle(dlg.m_angle);
		opengl.tree.setleaf(dlg.m_leaf);
		opengl.tree.setbaseR(dlg.m_width);
		opengl.tree.setQ(dlg.m_q);
	}
	else
		return;
	RedrawWindow();
	opengl.RenderSence();
}


void CFractalTreeView::OnReset()
{
	// TODO: 在此添加命令处理程序代码
	opengl.tree.setbaseTall(8);
	opengl.tree.setbaseR(6);
	opengl.tree.setAngle(55);
	opengl.tree.setleaf(9);
	opengl.tree.setM(0.65);
	opengl.tree.setN(5);
	opengl.tree.setQ(0.5);
	Tree::img = opengl.tree.loadPNGTexture("leaf.png");
	Tree::shape = 0;
	RedrawWindow();
}
