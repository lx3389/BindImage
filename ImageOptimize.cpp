// ImageOptimize.cpp : 实现文件
//

#include "stdafx.h"
#include "BindImage.h"
#include "ImageOptimize.h"
#include ".\imageoptimize.h"
#include "optipng/optipng.h"
#include <vector>
using namespace std;
// CImageOptimize 对话框

IMPLEMENT_DYNAMIC(CImageOptimize, CDialog)
CImageOptimize::CImageOptimize(CWnd* pParent /*=NULL*/)
	: CDialog(CImageOptimize::IDD, pParent)
{
}

CImageOptimize::~CImageOptimize()
{
}

void CImageOptimize::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CImageOptimize, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CImageOptimize 消息处理程序
static void app_printf(const char *fmt, ...){}
static void app_print_cntrl(int cntrl_code){}
static void app_progress(unsigned long current_step, unsigned long total_steps){
    ProcessDlg.AddCount(1);
    ProcessDlg.AddCountSub(1);
}
static void panic(const char *msg){}

static CString m_strPath;
static vector<CString> m_vecName;
void CImageOptimize::FindInAll(const char* lpszPath) 
{

	char szFind[MAX_PATH]; 
	lstrcpy(szFind, lpszPath);
	lstrcat(szFind, "*.*"); 
	WIN32_FIND_DATA wfd; 
	HANDLE hFind = FindFirstFile(szFind, &wfd); 
	if (hFind == INVALID_HANDLE_VALUE) 
		return; 
	do 
	{ 
		if (wfd.cFileName[0] == '.') 
			continue; 
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		{ 
			char szFile[MAX_PATH]; 
			wsprintf(szFile, "%s%s/", lpszPath, wfd.cFileName); 
			FindInAll(szFile);
		} 
		else 
		{ 
			char szFile[MAX_PATH]; 
			CString buffer=wfd.cFileName;
			if(buffer.Mid(buffer.ReverseFind('.')+1)!="png")
				continue;
			wsprintf(szFile, "%s%s", lpszPath, wfd.cFileName); 
			CString strTemp=lpszPath;
			strTemp=strTemp.Mid(m_strPath.GetLength()+1);
			m_vecName.push_back(strTemp+buffer);
			printf("SizeLow:%d\n",wfd.nFileSizeLow); 
			printf("SizeHight:%d\n",wfd.nFileSizeHigh); 
			printf("%s\n",szFile); 

		} 
	} while (FindNextFile(hFind, &wfd)); 
	FindClose(hFind); 

} 
void CImageOptimize::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnOK();


	m_strPath = GetFloderPath(GetSafeHwnd());
	if(m_strPath=="")
		return;
	//m_BindCore.setFilePath(m_strPath);
	CString fullPath=m_strPath+"\\";
	FindInAll(fullPath.GetBuffer());
	//for (int i=0;i<m_vecName.size();i++)
		//m_BindCore.AddImage(fullPath+m_vecName[i],m_vecName[i]);
	
	struct opng_ui ui;
	ui.printf_fn      = app_printf;
	ui.print_cntrl_fn = app_print_cntrl;
	ui.progress_fn    = app_progress;
	ui.panic_fn       = panic;
	static struct opng_options options;
	memset(&options, 0, sizeof(options));
	options.optim_level = -1;
	options.interlace = -1;
	if (opng_initialize(&options, &ui) != 0)
		printf("Can't initialize optimization engine");

	/*POSITION pos = dlg.GetStartPosition();
	vector<CString> strList;
	while (pos != NULL)
	{
		strPath = dlg.GetNextPathName(pos);
		CString   m_filetitle; 
		m_filetitle=strPath; 
		m_filetitle=m_filetitle.Mid(m_filetitle.ReverseFind('\\')+1); 
		strList.push_back(strPath);


	}*/
	ProcessDlg.ShowWindow(SW_SHOW);
	ProcessDlg.SetProcess(m_vecName.size()*9);
	for (int i=0;i<m_vecName.size();i++){
		ProcessDlg.SetProcessSub(9);
		opng_optimize((fullPath+m_vecName[i]).GetBuffer());
	}
	ProcessDlg.ShowWindow(SW_HIDE);
	if (opng_finalize() != 0)
		printf("Can't finalize optimization engine");

	m_vecName.clear();
	AfxMessageBox("优化完毕!",MB_OK|MB_ICONINFORMATION);
	

	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_ALLOWMULTISELECT,_T("文件png|*.png||"),NULL); 
	 CString strPath,str;
	dlg.m_ofn.lpstrFile=str.GetBuffer(1000000);
	str.ReleaseBuffer(); 
	dlg.m_ofn.nMaxFile = 50000;
	
}
