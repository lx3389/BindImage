// BindImage.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "BindImage.h"
#include "BindImageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBindImageApp

BEGIN_MESSAGE_MAP(CBindImageApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CBindImageApp ����

CBindImageApp::CBindImageApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CBindImageApp ����

CBindImageApp theApp;

CProcessgDlg  ProcessDlg;


// CBindImageApp ��ʼ��

BOOL CBindImageApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CBindImageDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȷ�������ر�
		//�Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//�Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	// ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
CString GetFloderPath(HWND _hWnd)
{
	CString FileDir;
	char szDisplayName[_MAX_PATH];
	BROWSEINFO bi; 
	ZeroMemory(&bi, sizeof(BROWSEINFO)); 
	bi.hwndOwner = _hWnd; 
	//bi.ulFlags   = BIF_RETURNONLYFSDIRS;
	bi.pszDisplayName = szDisplayName; 
	bi.lpszTitle = "ѡ��һ���ļ���";

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi); 
	BOOL bRet = FALSE; 
	TCHAR szFolder[MAX_PATH*2]; 
	szFolder[0] = _T('\0'); 
	if (pidl) 
	{ 
		if (SHGetPathFromIDList(pidl, szFolder))    
			bRet = TRUE; 
		IMalloc *pMalloc = NULL; 
		if (SUCCEEDED(SHGetMalloc(&pMalloc)) && pMalloc) 
		{   
			pMalloc->Free(pidl);   
			pMalloc->Release(); 
		} 
	} 

	return szFolder;
}