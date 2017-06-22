#pragma once


// CImageOptimize 对话框

class CImageOptimize : public CDialog
{
	DECLARE_DYNAMIC(CImageOptimize)

public:
	CImageOptimize(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CImageOptimize();

// 对话框数据
	enum { IDD = IDD_OPTIMIZE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	void		 FindInAll(const char* lpszPath);
};
