// BindImageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BindImage.h"
#include "BindImageDlg.h"
#include ".\bindimagedlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CBindImageDlg 对话框



CBindImageDlg::CBindImageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBindImageDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBindImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABALL, m_Table);
	 
}

BEGIN_MESSAGE_MAP(CBindImageDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABALL, OnTcnSelchangeTaball)
END_MESSAGE_MAP()


// CBindImageDlg 消息处理程序

BOOL CBindImageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	//m_ImageView.EnableScrollBar(WS_CHILD|WS_VISIBLE|SBS_HORZ,);
	
	LONG nStyle= GetWindowLong( m_ImageView.GetSafeHwnd(), GWL_STYLE);
	LONG newStyle= nStyle| WS_HSCROLL|WS_VSCROLL; // 添加或删除一些属性
    SetWindowLong( m_ImageView.GetSafeHwnd(),GWL_STYLE,newStyle);
	
	m_Pack.Create(IDD_PACK,&m_Table);
	m_RePack.Create(IDD_REPACK,&m_Table);
	m_Optimize.Create(IDD_OPTIMIZE,&m_Table);
	ProcessDlg.Create(IDD_PROCESS,this);
	CRect rect;
	GetWindowRect(&rect);
	ProcessDlg.CenterWindow();
    
	m_Table.InsertItem(0,_T("合成"));
	m_Table.InsertItem(1,_T("分解"));
	m_Table.InsertItem(2,_T("容量优化"));
	m_Table.ShowWindow(SW_SHOW);
	
	m_Table.GetClientRect(&rect); 
	rect.top+=35;
	rect.bottom-=10; 
	rect.left+=10; 
	rect.right-=10; 

	m_Pack.MoveWindow(&rect); 
	m_RePack.MoveWindow(&rect);
	m_Optimize.MoveWindow(&rect);
	m_Pack.ShowWindow(SW_SHOW);
	m_RePack.ShowWindow(SW_HIDE);
	m_Optimize.ShowWindow(SW_HIDE);
    m_Pack.GetDlgItem(IDOK)->SetFocus();
   // m_Table.ShowWindow(SW_HIDE);
	/*ProcessDlg.ShowWindow(SW_SHOW);
	ProcessDlg.SetProcess(8);
	ProcessDlg.AddCount(1);
	ProcessDlg.AddCount(1);
	ProcessDlg.AddCount(1);
	ProcessDlg.AddCount(1);
	ProcessDlg.AddCount(1);
	ProcessDlg.AddCount(1);
	ProcessDlg.AddCount(1);
	ProcessDlg.AddCount(1);
	ProcessDlg.AddCount(1);*/
	return FALSE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CBindImageDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CBindImageDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CBindImageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBindImageDlg::OnBnClickedOk()
{
	
	
}
void CBindImageDlg::OnTcnSelchangeTaball(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	switch(m_Table.GetCurSel())
	{
	case 0:
		m_RePack.ShowWindow(SW_HIDE);
		m_Optimize.ShowWindow(SW_HIDE);
		m_Pack.ShowWindow(SW_SHOW);
		
		m_Pack.SetFocus();
		break;
	case 1:
		m_Pack.ShowWindow(SW_HIDE);
		m_Optimize.ShowWindow(SW_HIDE);
		m_RePack.ShowWindow(SW_SHOW);
		m_RePack.SetFocus();
		break;
	case 2:
		m_Pack.ShowWindow(SW_HIDE);
		m_RePack.ShowWindow(SW_HIDE);
		m_Optimize.ShowWindow(SW_SHOW);
		m_Optimize.SetFocus();
		break;
	}
	*pResult = 0;
}
