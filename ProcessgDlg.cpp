// ProcessgDlg.cpp : 实现文件
//
#include "BindImage.h"
#include "ProcessgDlg.h"


// CProcessgDlg 对话框

IMPLEMENT_DYNAMIC(CProcessgDlg, CDialog)
CProcessgDlg::CProcessgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProcessgDlg::IDD, pParent)
{

}

CProcessgDlg::~CProcessgDlg()
{
}

void CProcessgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CProcessgDlg::SetProcess(int iAllCount){
    m_iAllCount=iAllCount;
	m_iCurCount=0;
	CProgressCtrl *pProgCtrl=(CProgressCtrl*)GetDlgItem(IDC_PROGRESSBAR);
    pProgCtrl->SetPos(0);
	pProgCtrl->SetRange(0,iAllCount);//设置进度条范围
	pProgCtrl->SetStep(1);
}

void CProcessgDlg::AddCount(int iCount){
   m_iCurCount+=iCount;
   CProgressCtrl *pProgCtrl=(CProgressCtrl*)GetDlgItem(IDC_PROGRESSBAR);
   for (int i=0;i<iCount;i++)
      pProgCtrl->StepIt();
}

void CProcessgDlg::SetProcessSub(int iAllCount){
	m_iAllCountSub=iAllCount;
	m_iCurCountSub=0;
	CProgressCtrl *pProgCtrl=(CProgressCtrl*)GetDlgItem(IDC_PROGRESSBARSUB);
	pProgCtrl->SetPos(0);
	pProgCtrl->SetRange(0,iAllCount);//设置进度条范围
	pProgCtrl->SetStep(1);
}


void CProcessgDlg::AddCountSub(int iCount){
	m_iCurCountSub+=iCount;
	CProgressCtrl *pProgCtrl=(CProgressCtrl*)GetDlgItem(IDC_PROGRESSBARSUB);
	for (int i=0;i<iCount;i++)
		pProgCtrl->StepIt();
}

BEGIN_MESSAGE_MAP(CProcessgDlg, CDialog)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CProcessgDlg 消息处理程序

BOOL CProcessgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//CProgressCtrl *pProgCtrl=(CProgressCtrl*)GetDlgItem(IDC_PROGRESSBAR);

	//pProgCtrl->SetRange(0,100);//设置进度条范围
 //   pProgCtrl->StepIt();
	//pProgCtrl->StepIt();
	// TODO:  在此添加额外的初始化
    //pProgCtrl
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CProcessgDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	if(bShow)
		GetParent()->GetDlgItem(IDC_TABALL)->ShowWindow(SW_HIDE);
	else
        GetParent()->GetDlgItem(IDC_TABALL)->ShowWindow(SW_SHOW);
	// TODO: 在此处添加消息处理程序代码
}
