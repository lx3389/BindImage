// BindImage.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once


#include "resource.h"		// 主符号
#include "ProcessgDlg.h"

// CBindImageApp:
// 有关此类的实现，请参阅 BindImage.cpp
//
 
class CBindImageApp : public CWinApp
{
public:
	CBindImageApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CBindImageApp theApp;

extern CProcessgDlg  ProcessDlg;

CString GetFloderPath(HWND _hWnd);