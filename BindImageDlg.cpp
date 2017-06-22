// BindImageDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BindImage.h"
#include "BindImageDlg.h"
#include ".\bindimagedlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CBindImageDlg �Ի���



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


// CBindImageDlg ��Ϣ�������

BOOL CBindImageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	//m_ImageView.EnableScrollBar(WS_CHILD|WS_VISIBLE|SBS_HORZ,);
	
	LONG nStyle= GetWindowLong( m_ImageView.GetSafeHwnd(), GWL_STYLE);
	LONG newStyle= nStyle| WS_HSCROLL|WS_VSCROLL; // ��ӻ�ɾ��һЩ����
    SetWindowLong( m_ImageView.GetSafeHwnd(),GWL_STYLE,newStyle);
	
	m_Pack.Create(IDD_PACK,&m_Table);
	m_RePack.Create(IDD_REPACK,&m_Table);
	m_Optimize.Create(IDD_OPTIMIZE,&m_Table);
	ProcessDlg.Create(IDD_PROCESS,this);
	CRect rect;
	GetWindowRect(&rect);
	ProcessDlg.CenterWindow();
    
	m_Table.InsertItem(0,_T("�ϳ�"));
	m_Table.InsertItem(1,_T("�ֽ�"));
	m_Table.InsertItem(2,_T("�����Ż�"));
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
	return FALSE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CBindImageDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CBindImageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBindImageDlg::OnBnClickedOk()
{
	
	
}
void CBindImageDlg::OnTcnSelchangeTaball(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
