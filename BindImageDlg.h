// BindImageDlg.h : ͷ�ļ�
//

#pragma once


#include "afxwin.h"
#include "afxcmn.h"
#include "ImagePack.h"
#include "ImageRePack.h"
#include "ImageOptimize.h"
#include "ProcessgDlg.h"
// CBindImageDlg �Ի���
class CBindImageDlg : public CDialog
{
// ����
public:
	CBindImageDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_BINDIMAGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
  
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CStatic m_ImageView;
	CEdit m_EditName;
	afx_msg void OnBnClickedRadio1();
	CTabCtrl      m_Table;
	CImagePack      m_Pack;
	CImageRePack    m_RePack;
	CImageOptimize  m_Optimize;
	//CProcessgDlg    m_Process;
	afx_msg void OnTcnSelchangeTaball(NMHDR *pNMHDR, LRESULT *pResult);
};
