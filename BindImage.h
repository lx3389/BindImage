// BindImage.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once


#include "resource.h"		// ������
#include "ProcessgDlg.h"

// CBindImageApp:
// �йش����ʵ�֣������ BindImage.cpp
//
 
class CBindImageApp : public CWinApp
{
public:
	CBindImageApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CBindImageApp theApp;

extern CProcessgDlg  ProcessDlg;

CString GetFloderPath(HWND _hWnd);