#pragma once


// CImageOptimize �Ի���

class CImageOptimize : public CDialog
{
	DECLARE_DYNAMIC(CImageOptimize)

public:
	CImageOptimize(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CImageOptimize();

// �Ի�������
	enum { IDD = IDD_OPTIMIZE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	void		 FindInAll(const char* lpszPath);
};
