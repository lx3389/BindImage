// ImageRePack.cpp : 实现文件
//

#include "stdafx.h"
#include "BindImage.h"
#include "ImageRePack.h"
#include ".\imagerepack.h"
#include <atlimage.h>
#include "tinyxml/tinyxml.h"
#include "optipng/optipng.h"
#include "json/json.h"
#include "ImageExt.h"
#include <vector>
#include <map>
using namespace std;
// CImageRePack 对话框

IMPLEMENT_DYNAMIC(CImageRePack, CDialog)
CImageRePack::CImageRePack(CWnd* pParent /*=NULL*/)
	: CDialog(CImageRePack::IDD, pParent)
{
     
}

CImageRePack::~CImageRePack()
{
}

void CImageRePack::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITPACKIN, m_EditSrc);
	DDX_Control(pDX, IDC_EDITPACKOUT, m_EditDes);
}


BEGIN_MESSAGE_MAP(CImageRePack, CDialog)
	ON_BN_CLICKED(IDC_BUTTONSRC, OnBnClickedButtonSrc)
	ON_BN_CLICKED(IDC_BUTTONOUT, OnBnClickedButtonOut)
	ON_BN_CLICKED(IDC_BUTTONBUILD, OnBnClickedButtonBuild)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CImageRePack 消息处理程序

void CImageRePack::OnBnClickedButtonSrc()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("XML PLIST|*.xml;*.plist;*.json|"), NULL);

	if(dlg.DoModal()==IDOK)	
		m_strSrc = dlg.GetPathName();

	
	GetDlgItem(IDC_EDITPACKIN)->SetWindowText(m_strSrc);
}

void CImageRePack::OnBnClickedButtonOut()
{
	m_strDes = GetFloderPath(GetSafeHwnd());
	if(m_strDes=="")
		return;
	GetDlgItem(IDC_EDITPACKOUT)->SetWindowText(m_strDes);
	// TODO: 在此添加控件通知处理程序代码
}
class CfRect
{
public:

	CfRect()
	{
		this->x=0;
		this->y=0;
		this->cx=0;
		this->cy=0;
	}
	CfRect(float x,float y,float cx,float cy)
	{
		this->x=x;
		this->y=y;
		this->cx=cx;
		this->cy=cy;

	}
	inline bool  IntoRect(float x,float y)
	{
		if(x>=this->x&&x<=this->x+this->cx&&y>=this->y&&y<=this->y+this->cy)
			return true;
		return false;
	}
	inline bool  IntoRect(const CfRect& rect)
	{
		if(x>=rect.x&&y>=rect.y&&x+cx<=rect.x+rect.cx&&y+cy<=rect.y+rect.cy)
			return true;
		return false;
	}
	inline bool  Collide(const CfRect& rect)
	{
		if(x+cx>rect.x&&x<rect.x+rect.cx&&y+cy>rect.y&&y<rect.y+rect.cy)
			return true;
		return false;
	}
	void operator()(float x,float y,float cx,float cy)
	{
		this->x=x;
		this->y=y;
		this->cx=cx;
		this->cy=cy;
	}
public:
	float x;
	float y;
	float cx;
	float cy;
};
class _tagTexXmlInfo
{
public:
	_tagTexXmlInfo(){
		bRotate = false;
	};
	_tagTexXmlInfo(_tagTexXmlInfo& tmp){
		operator=(tmp);
	}
	CString strName;
	CString strImgName;
	CString strPackName;
	CfRect    rcCurrent;
	CfRect    rcSource;
	bool	  bRotate;
	void operator=(_tagTexXmlInfo& tmp)
	{
		strName=tmp.strName;
		strImgName=tmp.strImgName;
		strPackName=tmp.strPackName;
		rcCurrent=tmp.rcCurrent;
		rcSource=tmp.rcSource;
		bRotate = tmp.bRotate;
	}
};
bool  IsFilePresent(CString _strFloder){
	WIN32_FIND_DATA fd;
	bool ret = FALSE;
	HANDLE hFind = FindFirstFile(_strFloder, &fd);
	if ((hFind != INVALID_HANDLE_VALUE) )
		ret = TRUE;    
	FindClose(hFind);
	return ret;
}
bool  CreateFloder(CString _strFloder){

	if (_strFloder == "") return false;
	if(!CreateDirectory(_strFloder,NULL)&&!IsFilePresent(_strFloder)){
		int nIndex = _strFloder.ReverseFind('/');
		if(nIndex == -1) nIndex = _strFloder.ReverseFind('\\');
		CString strTemp=_strFloder.Left(nIndex);
		if(!CreateFloder(strTemp))
            return false;
		CreateDirectory(_strFloder,NULL);
	}
	return true;
}
static void app_printf(const char *fmt, ...){}
static void app_print_cntrl(int cntrl_code){}
static void app_progress(unsigned long current_step, unsigned long total_steps){
	ProcessDlg.AddCount(1);
	ProcessDlg.AddCountSub(1);
}
static void panic(const char *msg){}
void CImageRePack::OnBnClickedButtonBuild()
{
	m_EditDes.GetWindowText(m_strDes);
	m_EditSrc.GetWindowText(m_strSrc);
	if(m_strDes==""||m_strSrc==""){
		AfxMessageBox("目录为空!",MB_OK|MB_ICONINFORMATION);
		return;
	}
	if(!CreateFloder(m_strDes)){
        AfxMessageBox("目录错误!",MB_OK|MB_ICONINFORMATION);
		return;
	}
	CString strType = m_strSrc.Right(m_strSrc.GetLength()-m_strSrc.ReverseFind('.'));
	if(strType==".plist")
		onPlistParse();
	if (strType == ".json")
		onJsonParse();
	else
		onXmlParse();
}
void CImageRePack::onJsonParse() {

	CImage m_ImgBuffer;
	CString strTemp = m_strSrc.Left(m_strSrc.Find(".")) + ".png";
	m_ImgBuffer.Load(strTemp);

	FILE* file = fopen(m_strSrc.GetString(), "rb");
	if (!file)
	{
		AfxMessageBox("读取文件失败!", MB_OK | MB_ICONINFORMATION);
		return;
	}
	fseek(file, 0, SEEK_END);
	long const size = ftell(file);
	unsigned long const usize = static_cast<unsigned long>(size);
	fseek(file, 0, SEEK_SET);
	JSONCPP_STRING text;
	char* buffer = new char[size + 1];
	buffer[size] = 0;
	if (fread(buffer, 1, usize, file) == usize)
		text = buffer;
	fclose(file);
	delete[] buffer;

	Json::Reader reader;
	Json::Value json_object;
	if (!reader.parse(text.data(), json_object)) {
		AfxMessageBox("解析文件失败!", MB_OK | MB_ICONINFORMATION);
		return;
	}
		
	int nCount = json_object["SubTexture"].size();
	

	ProcessDlg.ShowWindow(SW_SHOW);
	ProcessDlg.SetProcess(9 * nCount);

	struct opng_ui ui;
	/* Initialize the optimization engine. */
	ui.printf_fn = app_printf;
	ui.print_cntrl_fn = app_print_cntrl;
	ui.progress_fn = app_progress;
	ui.panic_fn = panic;

	static struct opng_options options;
	memset(&options, 0, sizeof(options));
	options.optim_level = -1;
	options.interlace = -1;
	if (opng_initialize(&options, &ui) != 0)
		printf("Can't initialize optimization engine");
	for (int i = 0;i < nCount; i++)
	{
		Json::Value json_item = json_object["SubTexture"][i];
		_tagTexXmlInfo pInfo;
		pInfo.strName = json_item["name"].asCString();
		pInfo.rcCurrent.x = json_item["x"].asFloat();
		pInfo.rcCurrent.y = json_item["y"].asFloat();
		pInfo.rcCurrent.cx = json_item["width"].asFloat();
		pInfo.rcCurrent.cy = json_item["height"].asFloat();
		pInfo.rcSource.x = pInfo.rcSource.y = 0;
		pInfo.rcSource.cx = pInfo.rcCurrent.cx;
		pInfo.rcSource.cy = pInfo.rcCurrent.cy;
		CImage m_imgTemp;
		m_imgTemp.Create(pInfo.rcSource.cx, pInfo.rcSource.cy, 32, CImage::createAlphaChannel);
		m_ImgBuffer.BitBlt(m_imgTemp.GetDC(), -pInfo.rcSource.x, -pInfo.rcSource.y, pInfo.rcCurrent.cx, pInfo.rcCurrent.cy, pInfo.rcCurrent.x, pInfo.rcCurrent.y, SRCCOPY);
		m_imgTemp.ReleaseDC();
		ProcessDlg.SetProcessSub(9);
		CString str123 = m_strDes + "\\" + pInfo.strName + ".png";
		m_imgTemp.Save(str123, Gdiplus::ImageFormatPNG);
		m_imgTemp.Destroy();
		if (((CButton*)GetDlgItem(IDC_REOPTIMIZE))->GetCheck())
			opng_optimize(str123.GetBuffer());
	}
	if (opng_finalize() != 0)
		printf("Can't finalize optimization engine");
	ProcessDlg.ShowWindow(SW_HIDE);
	m_ImgBuffer.Destroy();
	AfxMessageBox("分解成功!", MB_OK | MB_ICONINFORMATION);
}
void CImageRePack::onXmlParse(){

	CImage m_ImgBuffer;
	CString strTemp=m_strSrc.Left(m_strSrc.Find("."))+".png";
	m_ImgBuffer.Load(strTemp);

	TiXmlDocument doc;	
	if(!doc.LoadFile(m_strSrc)){
		AfxMessageBox("读取文件失败!",MB_OK|MB_ICONINFORMATION);
		return;
	}
	TiXmlNode* RootNode=doc.RootElement();
	TiXmlNode* RectNode;
	int        iCount=0;
	RootNode->ToElement()->QueryIntAttribute("ImageCount",&iCount);
	ProcessDlg.ShowWindow(SW_SHOW);
	ProcessDlg.SetProcess(9*iCount);

	struct opng_ui ui;
	/* Initialize the optimization engine. */
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
	for (RectNode=RootNode->FirstChild();RectNode;RectNode=RectNode->NextSibling())
	{
		if(RectNode->Type()==TiXmlNode::COMMENT)
			continue;
		_tagTexXmlInfo pInfo;
		pInfo.strName=RectNode->ToElement()->Attribute("Name");
		RectNode->ToElement()->QueryFloatAttribute("X",&pInfo.rcCurrent.x);
		RectNode->ToElement()->QueryFloatAttribute("Y",&pInfo.rcCurrent.y);
		RectNode->ToElement()->QueryFloatAttribute("CX",&pInfo.rcCurrent.cx);
		RectNode->ToElement()->QueryFloatAttribute("CY",&pInfo.rcCurrent.cy);
		RectNode->ToElement()->QueryFloatAttribute("SX",&pInfo.rcSource.x);
		RectNode->ToElement()->QueryFloatAttribute("SY",&pInfo.rcSource.y);
		RectNode->ToElement()->QueryFloatAttribute("SCX",&pInfo.rcSource.cx);
		RectNode->ToElement()->QueryFloatAttribute("SCY",&pInfo.rcSource.cy);

		CImage m_imgTemp;
		m_imgTemp.Create(pInfo.rcSource.cx,pInfo.rcSource.cy,32,CImage::createAlphaChannel);
		m_ImgBuffer.BitBlt(m_imgTemp.GetDC(),-pInfo.rcSource.x,-pInfo.rcSource.y,pInfo.rcCurrent.cx,pInfo.rcCurrent.cy,pInfo.rcCurrent.x,pInfo.rcCurrent.y,SRCCOPY);
		m_imgTemp.ReleaseDC();
		ProcessDlg.SetProcessSub(9);
		CString str123=m_strDes+"\\"+pInfo.strName+".png";
		m_imgTemp.Save(str123,Gdiplus::ImageFormatPNG);
		m_imgTemp.Destroy();
		if(((CButton*)GetDlgItem(IDC_REOPTIMIZE))->GetCheck())
			opng_optimize(str123.GetBuffer());
	}
	if (opng_finalize() != 0)
		printf("Can't finalize optimization engine");
	ProcessDlg.ShowWindow(SW_HIDE);
	m_ImgBuffer.Destroy();
	AfxMessageBox("分解成功!",MB_OK|MB_ICONINFORMATION);
	doc.Clear();
}

void CImageRePack::onPlistParse(){

	CImage m_ImgBuffer;
	CString strTemp=m_strSrc.Left(m_strSrc.Find("."))+".png";
	m_ImgBuffer.Load(strTemp);

	TiXmlDocument doc;	
	if(!doc.LoadFile(m_strSrc)){
		AfxMessageBox("读取文件失败!",MB_OK|MB_ICONINFORMATION);
		return;
	}
	vector<_tagTexXmlInfo*> vecTexXmlInfo;
	TiXmlNode* RootNode=doc.RootElement();
	TiXmlNode* RectNode,*InfoNode, *DescribeNode;
	int        iCount=0;
	string strKey,strValue,strFormat;
	map<string, string>  formatMap;
	//RootNode->ToElement()->QueryIntAttribute("ImageCount",&iCount);

	
    RootNode = RootNode->FirstChild()->FirstChild();

	for (DescribeNode = RootNode; DescribeNode; DescribeNode = DescribeNode->NextSibling())
	{
		string value = DescribeNode->FirstChild()->Value();
		if (value == "metadata") {
			DescribeNode = DescribeNode->NextSibling();
			for (DescribeNode = DescribeNode->FirstChild(); DescribeNode; DescribeNode = DescribeNode->NextSibling())
			{
				string strKey = DescribeNode->FirstChild()->Value();
				strFormat = DescribeNode->NextSibling()->FirstChild()->Value();
				break;
			}
			break;
		}
	}

    RootNode = RootNode->NextSibling();
	if (strFormat == "2") {
		formatMap["frame"] = "frame";
		formatMap["offset"] = "offset";
		formatMap["rotated"] = "rotated";
		formatMap["sourceColorRect"] = "sourceColorRect";
		formatMap["sourceSize"] = "sourceSize";
	}
	else if(strFormat == "3")
	{
		formatMap["frame"] = "textureRect";
		formatMap["offset"] = "spriteOffset";
		formatMap["rotated"] = "textureRotated";
		formatMap["sourceColorRect"] = "textureRect";
		formatMap["sourceSize"] = "spriteSourceSize";
	}

	for (RectNode=RootNode->FirstChild();RectNode;RectNode=RectNode->NextSibling())
	{
		if(RectNode->Type()==TiXmlNode::COMMENT)
			continue;
		_tagTexXmlInfo* pInfo = new _tagTexXmlInfo;
		pInfo->strName=RectNode->FirstChild()->Value();
		RectNode = RectNode->NextSibling();
		int  x = 0,y = 0,ox = 0,oy = 0,cx = 0,cy = 0,sx = 0,sy = 0,scx = 0,scy =0;
		
		bool bRotated = false;
        for (InfoNode = RectNode->FirstChild();InfoNode;InfoNode=InfoNode->NextSibling())
        {
			if(InfoNode->Type()==TiXmlNode::COMMENT)
				continue;
			strKey = InfoNode->FirstChild()->Value();
			InfoNode = InfoNode->NextSibling();
			
			
			if(strcmp(strKey.c_str(), formatMap["frame"].c_str())==0)
			{
				strValue = InfoNode->FirstChild()->Value();
				sscanf(strValue.c_str(),"{{%d,%d},{%d,%d}}",&x,&y,&cx,&cy);
				
			}
			else if (strcmp(strKey.c_str(), formatMap["offset"].c_str()) == 0)
			{
				strValue = InfoNode->FirstChild()->Value();
				sscanf(strValue.c_str(), "{%d,%d}", &ox, &oy);
			}
			else if(strcmp(strKey.c_str(), formatMap["rotated"].c_str())==0)
			{
				strValue = InfoNode->Value();
				if(strcmp(strValue.c_str(),"true")==0)
					bRotated = true;
			}
			else if(strcmp(strKey.c_str(), formatMap["sourceColorRect"].c_str())==0)
			{
				strValue = InfoNode->FirstChild()->Value();
				sscanf(strValue.c_str(),"{{%d,%d},{%d,%d}}",&sx,&sy,&cx,&cy);
			}
			else if(strcmp(strKey.c_str(), formatMap["sourceSize"].c_str())== 0){
				strValue = InfoNode->FirstChild()->Value();
				sscanf(strValue.c_str(),"{%d,%d}",&scx,&scy);
			}
        }
		if (strFormat == "3") {
			sx = (scx - cx) / 2 + ox;
			sy = (scy - cy) / 2 - oy;
		}
		pInfo->rcCurrent(x, y,cx,cy);
		pInfo->rcSource(sx,sy,scx,scy);
			
	   
		pInfo->bRotate = bRotated;
		vecTexXmlInfo.push_back(pInfo);
		
	}
	ProcessDlg.ShowWindow(SW_SHOW);
	ProcessDlg.SetProcess(9*vecTexXmlInfo.size());

	struct opng_ui ui;
	/* Initialize the optimization engine. */
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
	for (int i=0;i<vecTexXmlInfo.size();i++)
	{
		_tagTexXmlInfo* pInfo = vecTexXmlInfo[i];
		CImage m_imgTemp;
		if (pInfo->bRotate) {
			m_imgTemp.Create(pInfo->rcSource.cy,pInfo->rcSource.cx, 32, CImage::createAlphaChannel);
			m_ImgBuffer.BitBlt(m_imgTemp.GetDC(), pInfo->rcSource.cy - pInfo->rcSource.y- pInfo->rcCurrent.cy,pInfo->rcSource.x, pInfo->rcCurrent.cy, pInfo->rcCurrent.cx, pInfo->rcCurrent.x, pInfo->rcCurrent.y, SRCCOPY);
		}
		else
		{
			m_imgTemp.Create(pInfo->rcSource.cx, pInfo->rcSource.cy, 32, CImage::createAlphaChannel);
			m_ImgBuffer.BitBlt(m_imgTemp.GetDC(), pInfo->rcSource.x, pInfo->rcSource.y, pInfo->rcCurrent.cx, pInfo->rcCurrent.cy, pInfo->rcCurrent.x, pInfo->rcCurrent.y, SRCCOPY);
		}
		m_imgTemp.ReleaseDC();
		ProcessDlg.SetProcessSub(9);
		CString strPath=m_strDes+"\\"+pInfo->strName;
		CreateFloder(strPath.Left(strPath.ReverseFind('/')));
		if(pInfo->bRotate)
		{
			RotateCimage(m_imgTemp, strPath);//顺时针90度
		}else
		{
			m_imgTemp.Save(strPath,Gdiplus::ImageFormatPNG);
		}
		m_imgTemp.Destroy();
		
		
		if(((CButton*)GetDlgItem(IDC_REOPTIMIZE))->GetCheck())
			opng_optimize(strPath.GetBuffer());
		delete pInfo;
	}
	if (opng_finalize() != 0)
		printf("Can't finalize optimization engine");
	ProcessDlg.ShowWindow(SW_HIDE);
	m_ImgBuffer.Destroy();
	vecTexXmlInfo.clear();
	AfxMessageBox("分解成功!",MB_OK|MB_ICONINFORMATION);
	doc.Clear();
}

BOOL CImageRePack::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return false;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CImageRePack::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
     
	// TODO: 在此处添加消息处理程序代码
}
