#pragma once
#include "afxwin.h"


// CImageRePack �Ի���

class CImageRePack : public CDialog
{
	DECLARE_DYNAMIC(CImageRePack)

public:
	CImageRePack(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CImageRePack();

// �Ի�������
	enum { IDD = IDD_REPACK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
