// ProcessgDlg.cpp : ʵ���ļ�
//
#include "BindImage.h"
#include "ProcessgDlg.h"


// CProcessgDlg �Ի���

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
	pProgCtrl->SetRange(0,iAllCount);//���ý�������Χ
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
	pProgCtrl->SetRange(0,iAllCount);//���ý�������Χ
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


// CProcessgDlg ��Ϣ�������

BOOL CProcessgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//CProgressCtrl *pProgCtrl=(CProgressCtrl*)GetDlgItem(IDC_PROGRESSBAR);

	//pProgCtrl->SetRange(0,100);//���ý�������Χ
 //   pProgCtrl->StepIt();
	//pProgCtrl->StepIt();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
    //pProgCtrl
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CProcessgDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	if(bShow)
		GetParent()->GetDlgItem(IDC_TABALL)->ShowWindow(SW_HIDE);
	else
        GetParent()->GetDlgItem(IDC_TABALL)->ShowWindow(SW_SHOW);
	// TODO: �ڴ˴������Ϣ����������
}
