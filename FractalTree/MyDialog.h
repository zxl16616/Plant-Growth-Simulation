#pragma once
#include "afxwin.h"


// CMyDialog 对话框

class CMyDialog : public CDialog
{
	DECLARE_DYNAMIC(CMyDialog)

public:
	CMyDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 迭代次数
	int m_n;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	
	// 初始高度
	int m_high;
	
	// 高度缩放比例
	float m_M;
	// 树枝开合角度
	float m_angle;
	// 叶片相对大小
	int m_leaf;
	// 树枝粗度缩放比例
	float m_q;
	// 初始树枝粗度
	float m_width;
	afx_msg void OnBnClickedleaf1();
	afx_msg void OnBnClickedleaf2();
	afx_msg void OnBnClickedleaf3();
	afx_msg void OnBnClickedleaf4();
	CBitmapButton m_Btn1;
	CBitmapButton m_Btn2;
	CBitmapButton m_Btn3;
	CBitmapButton m_Btn4;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	int m_iRadio;

	static int radioState;
};
