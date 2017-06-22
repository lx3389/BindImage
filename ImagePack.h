#pragma once
#include "afxwin.h"

#include "BindCore.h"
// CImagePack 对话框
enum 
{
	SWITCH_OPTION_TOWMI= 0x01,
	SWITCH_OPTION_AUTO = 0x02,
	SWITCH_OPTION_SAME = 0x04,
	SWITCH_OPTION_LUA  = 0x08,
};
typedef struct _tagHistoryInfo
{
	int      nSpace;
	int      nMaxWidth;
	int      nMaxHeight;
	int      nDataType;
	CString  strFileName;
	CString  strFilePath;
}tagHistoryInfo;

typedef vector<_tagHistoryInfo> VecHistory;

class CImagePack : public CDialog
{
	DECLARE_DYNAMIC(CImagePack)

public:
	typedef vector<CString> VecString;
	CImagePack(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CImagePack();

// 对话框数据
	enum { IDD = IDD_PACK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	void		 FindInAll(const char* lpszPath);

	void         DescribeBuild(CString filename);

	DECLARE_MESSAGE_MAP()

public:
	CEdit		 m_SpaceEdit;

	CEdit		 m_MaxWEdit;

	CEdit		 m_MaxHEdit;

	CComboBox    m_HistoryCombo;

	TEXTMETRIC   m_tmCombo;

	CDC*         m_pDCCombo;

	CFont*       m_pFontCombo;

	CFont*       m_pOldFontCombo;

	int			 m_nMaxComboxWidth;

	int			 m_nExtendWidth;

	CBindCore    m_BindCore;

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();

	CEdit		 m_EditName;

	VecString    m_vecName;

	CString      m_strPath;

	CComboBox	 m_listType;

	afx_msg void OnBnClickedOpfile();

	void		 LoadConfig();

	void		 SaveConfig(const tagHistoryInfo& _info);

	void		 setCurInfo(const tagHistoryInfo& _info);

	void         insertComboBox(const CString& _str);

	void		 luaProcess(CString filename);

private:

	static int          m_nSize;

	static int          m_nMaxSize;

	static int          m_nCurIndex;

	static int          m_nLoopIndex;

	static int			m_nSwitchOption;

	static VecHistory   m_vecHistory;
public:
	afx_msg void OnCbnSelchangeFilehostory();
	afx_msg void OnCbnDblclkFilehostory();
	afx_msg void OnCbnSetfocusFilehostory();

	afx_msg void OnBnClickedLuacore();
};
