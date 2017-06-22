// BindImageDlg.h : 头文件
//

#pragma once


#include "afxwin.h"
#include "afxcmn.h"
#include "ImagePack.h"
#include "ImageRePack.h"
#include "ImageOptimize.h"
#include "ProcessgDlg.h"
// CBindImageDlg 对话框
class CBindImageDlg : public CDialog
{
// 构造
public:
	CBindImageDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_BINDIMAGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
  
	// 生成的消息映射函数
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
