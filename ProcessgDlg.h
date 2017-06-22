#pragma once

#include "stdafx.h"
// CProcessgDlg 对话框

class CProcessgDlg : public CDialog
{
	DECLARE_DYNAMIC(CProcessgDlg)

public:
	CProcessgDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProcessgDlg();

// 对话框数据
	enum { IDD = IDD_PROCESS };

	void     SetProcess(int iAllCount);

	void     SetProcessSub(int iAllCount);

	void     AddCount(int iCount);

	void     AddCountSub(int iCount);


protected:
	int      m_iAllCount;

	int      m_iCurCount;

	int      m_iAllCountSub;

	int      m_iCurCountSub;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
