// MyDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "FractalTree.h"
#include "MyDialog.h"
#include "afxdialogex.h"
#include "OpenGL.h"
#include "resource.h"

// CMyDialog 对话框

IMPLEMENT_DYNAMIC(CMyDialog, CDialog)

int CMyDialog::radioState = 0;

CMyDialog::CMyDialog(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG1, pParent)
	, m_n(5)
	, m_high(0)
	, m_M(0)
	, m_angle(0)
	, m_leaf(0)
	, m_q(0)
	, m_width(0)
	, m_iRadio(0)
{

}

CMyDialog::~CMyDialog()
{
}

void CMyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_N, m_n);
	DDV_MinMaxInt(pDX, m_n, 0, 7);
	DDX_Text(pDX, IDC_orgHigh, m_high);
	DDV_MinMaxInt(pDX, m_high, 1, 10);
	DDX_Text(pDX, IDC_M, m_M);
	DDV_MinMaxFloat(pDX, m_M, 0, 1);
	DDX_Text(pDX, IDC_Angle, m_angle);
	DDV_MinMaxFloat(pDX, m_angle, 0, 180);
	DDX_Text(pDX, IDC_leaf, m_leaf);
	DDV_MinMaxInt(pDX, m_leaf, 1, 10);
	DDX_Text(pDX, IDC_Q, m_q);
	DDV_MinMaxFloat(pDX, m_q, 0, 1);
	DDX_Text(pDX, IDC_width, m_width);
	DDV_MinMaxFloat(pDX, m_width, 1, 10);
	DDX_Control(pDX, IDC_leaf1, m_Btn1);
	DDX_Control(pDX, IDC_leaf2, m_Btn2);
	DDX_Control(pDX, IDC_leaf3, m_Btn3);
	DDX_Control(pDX, IDC_leaf4, m_Btn4);
	DDX_Radio(pDX, IDC_RADIO3, m_iRadio);
}


BEGIN_MESSAGE_MAP(CMyDialog, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_leaf1, &CMyDialog::OnBnClickedleaf1)
	ON_BN_CLICKED(IDC_leaf2, &CMyDialog::OnBnClickedleaf2)
	ON_BN_CLICKED(IDC_leaf3, &CMyDialog::OnBnClickedleaf3)
	ON_BN_CLICKED(IDC_leaf4, &CMyDialog::OnBnClickedleaf4)
	ON_BN_CLICKED(IDC_RADIO3, &CMyDialog::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CMyDialog::OnBnClickedRadio4)
END_MESSAGE_MAP()


// CMyDialog 消息处理程序


void CMyDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	GetDlgItem(IDC_N)->SetFocus();
	((CEdit*)GetDlgItem(IDC_N))->SetSel(0, -1);
}


void CMyDialog::OnBnClickedleaf1()
{
	// TODO: 在此添加控件通知处理程序代码
	Tree tree;
	Tree::img = tree.loadPNGTexture("leaf.png");
	
}


void CMyDialog::OnBnClickedleaf2()
{
	// TODO: 在此添加控件通知处理程序代码
	Tree tree;
	Tree::img = tree.loadPNGTexture("leaf1.png");
}


void CMyDialog::OnBnClickedleaf3()
{
	// TODO: 在此添加控件通知处理程序代码
	Tree tree;
	Tree::img = tree.loadPNGTexture("leaf2.png");
}


void CMyDialog::OnBnClickedleaf4()
{
	// TODO: 在此添加控件通知处理程序代码
	Tree tree;
	Tree::img = tree.loadPNGTexture("leaf3.png");
}


BOOL CMyDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
 
	// TODO:  在此添加额外的初始化
	m_Btn1.LoadBitmaps(IDB_BITMAP1, IDB_BITMAP2);
	m_Btn1.SizeToContent();
	m_Btn2.LoadBitmaps(IDB_BITMAP5, IDB_BITMAP6);
	m_Btn2.SizeToContent();
	m_Btn3.LoadBitmaps(IDB_BITMAP7, IDB_BITMAP8);
	m_Btn3.SizeToContent();
	m_Btn4.LoadBitmaps(IDB_BITMAP3, IDB_BITMAP4);
	m_Btn4.SizeToContent();
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CMyDialog::OnBnClickedRadio3()
{
	// TODO: 在此添加控件通知处理程序代码
	Tree tree;
	Tree::shape = 0;
	CMyDialog::radioState = 0;
}


void CMyDialog::OnBnClickedRadio4()
{
	// TODO: 在此添加控件通知处理程序代码
	
	Tree tree;
	Tree::shape = 1;
	CMyDialog::radioState = 1;
}
