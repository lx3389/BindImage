#pragma once
#include "afxwin.h"


// CImageRePack 对话框

class CImageRePack : public CDialog
{
	DECLARE_DYNAMIC(CImageRePack)

public:
	CImageRePack(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CImageRePack();

// 对话框数据
	enum { IDD = IDD_REPACK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EditSrc;
	CEdit m_EditDes;
	CString m_strSrc;
	CString m_strDes;
	afx_msg void OnBnClickedButtonSrc();
	afx_msg void OnBnClickedButtonOut();
	afx_msg void OnBnClickedButtonBuild();

	void		 onJsonParse();
	void		 onXmlParse();

	void		 onPlistParse();

	virtual BOOL OnInitDialog();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
