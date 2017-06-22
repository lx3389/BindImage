#pragma once

#include "stdafx.h"
// CProcessgDlg �Ի���

class CProcessgDlg : public CDialog
{
	DECLARE_DYNAMIC(CProcessgDlg)

public:
	CProcessgDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProcessgDlg();

// �Ի�������
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

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
