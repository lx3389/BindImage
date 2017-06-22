// ImagePack.cpp : 实现文件
//

#include "stdafx.h"
#include "BindImage.h"
#include "ImagePack.h"
#include "tinyxml/tinyxml.h"
#include "lua/lua.hpp"

CString  GetFilePath()
{ 
	char  exepath[MAX_PATH];
	CString  strdir,tmpdir; 
	memset(exepath,0,MAX_PATH); 
	GetModuleFileName(NULL,exepath,MAX_PATH); 
	tmpdir=exepath; 
	strdir=tmpdir.Left(tmpdir.ReverseFind('\\'));
	return strdir; 
} 
// CImagePack 对话框

VecHistory CImagePack::m_vecHistory;

int CImagePack::m_nSwitchOption = 0;

int CImagePack::m_nLoopIndex = 0;

int CImagePack::m_nCurIndex = 0;

int CImagePack::m_nMaxSize = 0;

int CImagePack::m_nSize = 0;

IMPLEMENT_DYNAMIC(CImagePack, CDialog)
CImagePack::CImagePack(CWnd* pParent /*=NULL*/)
	: CDialog(CImagePack::IDD, pParent)
{
	m_nMaxComboxWidth = 0;
}

CImagePack::~CImagePack()
{
	/*m_pDCCombo->SelectObject(m_pOldFontCombo);
	m_HostoryCombo.ReleaseDC(m_pDCCombo);*/
}

void CImagePack::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPACE, m_SpaceEdit);
	DDX_Control(pDX, IDC_MAXW, m_MaxWEdit);
	DDX_Control(pDX, IDC_MAXH, m_MaxHEdit);
	DDX_Control(pDX, IDC_FILEEDIT, m_EditName);
	DDX_Control(pDX, IDC_COMBOTYPE, m_listType);
	DDX_Control(pDX, IDC_FILEHOSTORY, m_HistoryCombo);
}


BEGIN_MESSAGE_MAP(CImagePack, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDOPFILE, &CImagePack::OnBnClickedOpfile)
	ON_CBN_SELCHANGE(IDC_FILEHOSTORY, &CImagePack::OnCbnSelchangeFilehostory)
	ON_CBN_DBLCLK(IDC_FILEHOSTORY, &CImagePack::OnCbnDblclkFilehostory)
	ON_CBN_SETFOCUS(IDC_FILEHOSTORY, &CImagePack::OnCbnSetfocusFilehostory)
	ON_BN_CLICKED(IDC_LUACORE, &CImagePack::OnBnClickedLuacore)
END_MESSAGE_MAP()


// CImagePack 消息处理程序

BOOL CImagePack::OnInitDialog()
{
	CDialog::OnInitDialog();
     
	// TODO:  在此添加额外的初始化
    m_MaxWEdit.SetWindowText("1024");
	m_MaxHEdit.SetWindowText("1024");
    m_SpaceEdit.SetWindowText("1");
	GetDlgItem(IDOK)->SetFocus();
	m_listType.SetCurSel(1);
	m_HistoryCombo.LimitText(MAX_PATH);

	m_pDCCombo = m_HistoryCombo.GetDC();
	m_pFontCombo = m_HistoryCombo.GetFont();
	m_pOldFontCombo = m_pDCCombo->SelectObject(m_pFontCombo);
	m_pDCCombo->GetTextMetrics(&m_tmCombo);
	m_nExtendWidth = ::GetSystemMetrics(SM_CXVSCROLL) + 2*::GetSystemMetrics(SM_CXEDGE);
	LoadConfig();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CImagePack::LoadConfig(){

	if(m_nMaxSize ==0 )
	{
		CString strSize,strMaxSize,strCurIndex,strLoopIndex,strOption;
		CString strTemp,strHistory;
		CString strConfig = GetFilePath()+"\\Config.ini";
		GetPrivateProfileString("Info","Size","0",strSize.GetBuffer(MAX_PATH),MAX_PATH,strConfig); 
		GetPrivateProfileString("Info","MaxSize","50",strMaxSize.GetBuffer(MAX_PATH),MAX_PATH,strConfig); 
		GetPrivateProfileString("Info","CurIndex","0",strCurIndex.GetBuffer(MAX_PATH),MAX_PATH,strConfig);
		GetPrivateProfileString("Info","LoopIndex","0",strLoopIndex.GetBuffer(MAX_PATH),MAX_PATH,strConfig);
		GetPrivateProfileString("Info","switchOption","0",strOption.GetBuffer(MAX_PATH),MAX_PATH,strConfig);
		m_nSize = atoi(strSize.GetString());
		m_nMaxSize = atoi(strMaxSize.GetString());
		m_nCurIndex = atoi(strCurIndex.GetString());
		m_nLoopIndex = atoi(strLoopIndex.GetString());
		m_nSwitchOption = atoi(strOption.GetString());
		m_vecHistory.reserve(m_nSize);
		tagHistoryInfo tempInfo;
		for (int i=m_nLoopIndex;i<m_nSize+m_nLoopIndex;i++)
		{
			strHistory.Format("History%d",(i%m_nSize)+1);
			GetPrivateProfileString(strHistory.GetString(),"space","",strTemp.GetBuffer(MAX_PATH),MAX_PATH,strConfig);
			tempInfo.nSpace = atoi(strTemp.GetString());
			GetPrivateProfileString(strHistory.GetString(),"maxWidth","",strTemp.GetBuffer(MAX_PATH),MAX_PATH,strConfig);
			tempInfo.nMaxWidth = atoi(strTemp.GetString());
			GetPrivateProfileString(strHistory.GetString(),"maxHeight","",strTemp.GetBuffer(MAX_PATH),MAX_PATH,strConfig);
			tempInfo.nMaxHeight = atoi(strTemp.GetString());
			GetPrivateProfileString(strHistory.GetString(),"dataType","0",strTemp.GetBuffer(MAX_PATH),MAX_PATH,strConfig);
			tempInfo.nDataType = atoi(strTemp.GetString());
			GetPrivateProfileString(strHistory.GetString(),"fileName","",strTemp.GetBuffer(MAX_PATH),MAX_PATH,strConfig);
			tempInfo.strFileName = strTemp.GetString();
			GetPrivateProfileString(strHistory.GetString(),"filePath","",strTemp.GetBuffer(MAX_PATH),MAX_PATH,strConfig);
			tempInfo.strFilePath = strTemp.GetString();
			m_vecHistory.push_back(tempInfo);
			insertComboBox(tempInfo.strFilePath);
		}
		((CButton*)GetDlgItem(IDC_SUBITEMUSE))->SetCheck(m_nSwitchOption&SWITCH_OPTION_SAME);
		((CButton*)GetDlgItem(IDC_OPTIMIZE))->SetCheck(m_nSwitchOption&SWITCH_OPTION_AUTO);
		((CButton*)GetDlgItem(IDC_2MI))->SetCheck(m_nSwitchOption&SWITCH_OPTION_TOWMI);
		((CButton*)GetDlgItem(IDC_LUACORE))->SetCheck(m_nSwitchOption&SWITCH_OPTION_LUA);
		OnBnClickedLuacore();
	}
	if(m_nSize>0)
	{
		if(m_nCurIndex>=0&&m_nCurIndex<m_vecHistory.size()){
			setCurInfo(m_vecHistory[m_nCurIndex]);
			m_HistoryCombo.SetCurSel(m_nCurIndex);
		}
	}
}


void CImagePack::SaveConfig( const tagHistoryInfo& _info )
{
	CString strConfig = GetFilePath()+"\\Config.ini";
	CString strTemp;
	tagHistoryInfo tempInfo;
	bool    bFindSame = false;
	int     curUpdateIndex;
	m_nSwitchOption = 0;
	m_nSwitchOption|=((CButton*)GetDlgItem(IDC_SUBITEMUSE))->GetCheck()?SWITCH_OPTION_SAME:0;
	m_nSwitchOption|=((CButton*)GetDlgItem(IDC_OPTIMIZE))->GetCheck()?SWITCH_OPTION_AUTO:0;
	m_nSwitchOption|=((CButton*)GetDlgItem(IDC_2MI))->GetCheck()?SWITCH_OPTION_TOWMI:0;
	m_nSwitchOption|=((CButton*)GetDlgItem(IDC_LUACORE))->GetCheck()?SWITCH_OPTION_LUA:0;
	strTemp.Format("%d",m_nSwitchOption);
	WritePrivateProfileString("Info","switchOption",strTemp.GetString(),strConfig);
	for (int i=0;i<m_vecHistory.size();i++)
	{
		if(m_vecHistory[i].strFilePath == _info.strFilePath){
			bFindSame = true;
			m_vecHistory[i].nSpace = _info.nSpace;
			m_vecHistory[i].nMaxHeight = _info.nMaxHeight;
			m_vecHistory[i].nMaxWidth = _info.nMaxWidth;
			m_vecHistory[i].strFileName = _info.strFileName;

			tempInfo.nSpace = _info.nSpace;
			tempInfo.nMaxHeight = _info.nMaxHeight;
			tempInfo.nMaxWidth = _info.nMaxWidth;
			tempInfo.strFileName = _info.strFileName;
			tempInfo.strFilePath = _info.strFilePath;
			tempInfo.nDataType = _info.nDataType;
			curUpdateIndex = i+1;
			break;
		}
	}

	
	if(!bFindSame)
	{
		if(m_nLoopIndex>=m_nMaxSize)
			m_nLoopIndex = 0;
		
		if(m_nLoopIndex>=m_vecHistory.size())
		{

			tempInfo.nSpace = _info.nSpace;
			tempInfo.nMaxHeight = _info.nMaxHeight;
			tempInfo.nMaxWidth = _info.nMaxWidth;
			tempInfo.strFileName = _info.strFileName;
			tempInfo.strFilePath = _info.strFilePath;
			tempInfo.nDataType = _info.nDataType;
			m_vecHistory.push_back(tempInfo);
			m_nLoopIndex++;
			m_nSize = m_vecHistory.size();
			insertComboBox(tempInfo.strFilePath);
			
			curUpdateIndex = m_nLoopIndex;
		}else
		{
			tagHistoryInfo &tempInfo = m_vecHistory[m_nLoopIndex];
			tempInfo.nSpace = _info.nSpace;
			tempInfo.nMaxHeight = _info.nMaxHeight;
			tempInfo.nMaxWidth = _info.nMaxWidth;
			tempInfo.strFileName = _info.strFileName;
			tempInfo.strFilePath = _info.strFilePath;
			tempInfo.nDataType = _info.nDataType;
			
			insertComboBox(tempInfo.strFilePath);
			m_nLoopIndex++;
			curUpdateIndex = m_nLoopIndex;
		}
	}
	
	CString strHistory;
	strHistory.Format("History%d",curUpdateIndex);
	strTemp.Format("%d",tempInfo.nSpace);
	WritePrivateProfileString(strHistory,"space",strTemp.GetString(),strConfig); 
	strTemp.Format("%d",tempInfo.nMaxHeight);
	WritePrivateProfileString(strHistory,"maxWidth",strTemp.GetString(),strConfig);
	strTemp.Format("%d",tempInfo.nMaxWidth);
	WritePrivateProfileString(strHistory,"maxHeight",strTemp.GetString(),strConfig);
	strTemp.Format("%d",tempInfo.nDataType);
	WritePrivateProfileString(strHistory,"dataType",strTemp.GetString(),strConfig);
	WritePrivateProfileString(strHistory,"fileName",tempInfo.strFileName.GetString(),strConfig);
	WritePrivateProfileString(strHistory,"filePath",tempInfo.strFilePath.GetString(),strConfig);

	strTemp.Format("%d",m_nSize);
	WritePrivateProfileString("Info","Size",strTemp.GetString(),strConfig); 
	strTemp.Format("%d",m_nMaxSize);
	WritePrivateProfileString("Info","MaxSize",strTemp.GetString(),strConfig);
	strTemp.Format("%d",m_nCurIndex);
	WritePrivateProfileString("Info","CurIndex",strTemp.GetString(),strConfig);
	strTemp.Format("%d",m_nLoopIndex);
	WritePrivateProfileString("Info","LoopIndex",strTemp.GetString(),strConfig);

	

	
}

void CImagePack::FindInAll(const char* lpszPath) 
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
			//printf("SizeLow:%d\n",wfd.nFileSizeLow); 
			//printf("SizeHight:%d\n",wfd.nFileSizeHigh); 
			//printf("%s\n",szFile); 

		} 
	} while (FindNextFile(hFind, &wfd)); 
	FindClose(hFind); 

} 
void CImagePack::OnBnClickedOpfile()
{
	m_strPath = GetFloderPath(GetSafeHwnd());
	if(m_strPath=="")
		return;

	GetDlgItem(IDC_FILEHOSTORY)->SetWindowText(m_strPath);
}

void CImagePack::OnBnClickedOk()
{
	int  iW,iH,iSpace;
	CString strTemp;
    m_MaxWEdit.GetWindowText(strTemp);
	iW=atoi(strTemp.GetBuffer());
	m_MaxHEdit.GetWindowText(strTemp);
	iH=atoi(strTemp.GetBuffer());
	m_SpaceEdit.GetWindowText(strTemp);
	iSpace=atoi(strTemp.GetBuffer());
	if(iW<=0||iH<=0){
		AfxMessageBox("最大宽高错误!",MB_OK|MB_ICONINFORMATION);
		return;
	}else if(iSpace<0){
		AfxMessageBox("空隙不能小于0!",MB_OK|MB_ICONINFORMATION);
		return;
	}
	

	
	CString strPath;
	GetDlgItem(IDC_FILEHOSTORY)->GetWindowText(strPath);
	if(strPath=="")
	{
		AfxMessageBox("没有找到路径!",MB_OK|MB_ICONINFORMATION);
		return;
	}
	m_BindCore.setFilePath(strPath);
	m_strPath = strPath;
	strPath+="\\";
	FindInAll(strPath.GetBuffer());

	m_BindCore.Init(iW,iH,iSpace);
	m_BindCore.Reset();
	for (int i=0;i<m_vecName.size();i++)
		m_BindCore.AddImage(strPath+m_vecName[i],m_vecName[i]);
    m_vecName.clear();
	
	CString filename;
	m_EditName.GetWindowText(filename);

	m_BindCore.enableUsedSame(((CButton*)GetDlgItem(IDC_SUBITEMUSE))->GetCheck());
	if(!m_BindCore.AutoBind(filename,((CButton*)GetDlgItem(IDC_OPTIMIZE))->GetCheck(),((CButton*)GetDlgItem(IDC_2MI))->GetCheck()))
		return;
	GetDlgItem(IDC_FILEHOSTORY)->GetWindowText(strPath);
	tagHistoryInfo info;
	info.nSpace = iSpace;
	info.nMaxWidth = iW;
	info.nMaxHeight = iH;
	info.strFilePath = strPath;
	info.strFileName = filename;
	info.nDataType = m_listType.GetCurSel();
	SaveConfig(info);
	m_strPath = strPath;
	DescribeBuild(filename);
	AfxMessageBox("创建成功!",MB_OK|MB_ICONINFORMATION);
	if(((CButton*)GetDlgItem(IDC_LUACORE))->GetCheck())
		luaProcess(filename);
}
void  CImagePack::luaProcess(CString filename)
{
	printf("程序根目录存在main.lua会自动执行,不存在将会创建再自动执行！\n");
	lua_State* lua = luaL_newstate();
	//加载所有标准类库
	luaL_openlibs(lua);
	//把函数传入栈中
	//把宿主中的结构体转换为表数据
	lua_newtable(lua); //创建一个表 
	int idx = -3;
	//压入key-value

	InfoList& list=m_BindCore.GetImageList();
	CfSize size=m_BindCore.GetImageSize();
	CString strTemp;
	lua_pushstring(lua, "path"); // table["id"] = c_xp.id
	lua_pushstring(lua, m_strPath.GetBuffer());
	lua_settable(lua, idx);
	lua_pushstring(lua, "name"); // table["id"] = c_xp.id
	lua_pushstring(lua, filename.GetBuffer());
	lua_settable(lua, idx);
	lua_pushstring(lua, "width"); // table[1] = c_xp.score[0]
	lua_pushnumber(lua, size.cx);
	lua_settable(lua, idx);
	lua_pushstring(lua, "height"); // table[1] = c_xp.score[0]
	lua_pushnumber(lua, size.cy);
	lua_settable(lua, idx);
	lua_pushstring(lua, "count"); // table[1] = c_xp.score[0]
	lua_pushnumber(lua, list.size());
	lua_settable(lua, idx);


	for (int i=0;i<list.size();i++)
	{
		BImageInfo* Info=list[i];

		lua_pushinteger(lua,i+1); 
		lua_newtable(lua);
		lua_pushstring(lua, "name"); 
		lua_pushstring(lua, (Info->name+".png").GetBuffer());
		lua_settable(lua, idx);

		lua_pushstring(lua, "x"); 
		lua_pushnumber(lua, Info->position.x);
		lua_settable(lua, idx);
		lua_pushstring(lua, "y"); 
		lua_pushnumber(lua, Info->position.y);
		lua_settable(lua, idx);
		lua_pushstring(lua, "w"); 
		lua_pushnumber(lua, Info->crect.cx);
		lua_settable(lua, idx);
		lua_pushstring(lua, "h"); 
		lua_pushnumber(lua, Info->crect.cy);
		lua_settable(lua, idx);
		lua_pushstring(lua, "sx"); 
		lua_pushnumber(lua, Info->srect.x);
		lua_settable(lua, idx);
		lua_pushstring(lua, "sy"); 
		lua_pushnumber(lua, Info->srect.y);
		lua_settable(lua, idx);
		lua_pushstring(lua, "sw"); 
		lua_pushnumber(lua, Info->srect.cx);
		lua_settable(lua, idx);
		lua_pushstring(lua, "sh"); 
		lua_pushnumber(lua, Info->srect.cy);
		lua_settable(lua, idx);
		lua_settable(lua, idx);
	}
	
	lua_setglobal(lua, "BIInfo");
	//把表导入到解析器, 并命名为c_xp
	

	FILE* pFile = fopen("main.lua","r");
	if(!pFile){
		pFile = fopen("main.lua","w");
		strTemp = "function main()\n"
			      "    print(\"************************************************\")\n"
				  "    print(string.format(\"outPath:%s\",BIInfo.path))\n"
				  "    print(string.format(\"outName:%s\",BIInfo.name))\n"
			      "    print(string.format(\"count:%d\",BIInfo.count))\n"
			      "    print(string.format(\"width:%d\",BIInfo.width))\n"
				  "    print(string.format(\"height:%d\",BIInfo.height))\n"
			      "    for i=1,BIInfo.count do\n"
				  "    	   print(string.format(\"itemName:%s x=%d y=%d w=%d h=%d sx=%d sy=%d sw=%d sh=%d\",BIInfo[i].name,\n"
			      "                              BIInfo[i].x,BIInfo[i].y,BIInfo[i].w,BIInfo[i].h,\n"
				  "                              BIInfo[i].sx,BIInfo[i].sy,BIInfo[i].sw,BIInfo[i].sh))\n"
			      "    end\n"
			      "    print(\"************************************************\")\n"
			      "end\n"
			      "main()";
		fwrite(strTemp.GetString(),strTemp.GetLength(),1,pFile);
		fclose(pFile);
	}else
		fclose(pFile);
	if(luaL_dofile(lua, "main.lua"))
		printf("%s\n", lua_tostring(lua, -1));
	lua_close(lua);
}
void  CImagePack::DescribeBuild(CString filename){

	m_strPath=m_strPath.Left(m_strPath.ReverseFind('\\')+1);
	InfoList& list=m_BindCore.GetImageList();
	CfSize size=m_BindCore.GetImageSize();
	switch(m_listType.GetCurSel())
	{
	case 1:
		{
			TiXmlDocument  *myDoc=new TiXmlDocument();
			TiXmlDeclaration * xmlDec = new TiXmlDeclaration("1.0","UTF-8","");
			myDoc->LinkEndChild(xmlDec);
			TiXmlElement *elmImageSet=new TiXmlElement("ImageSet");
			elmImageSet->SetAttribute("Name",filename.GetBuffer());
			elmImageSet->SetAttribute("Imagefile",(filename+".png").GetBuffer());
			elmImageSet->SetAttribute("ImageW",size.cx);
			elmImageSet->SetAttribute("ImageH",size.cy);
			elmImageSet->SetAttribute("ImageCount",list.size());
			myDoc->LinkEndChild(elmImageSet);

			for(int j=0;j<list.size();j++)
			{        
				BImageInfo* Info=list[j];
				TiXmlElement *elmImageItem=new TiXmlElement("Image");
				elmImageSet->LinkEndChild(elmImageItem);
				Info->name.Replace(".png","");
				elmImageItem->SetAttribute("Name",Info->name.GetBuffer());
				elmImageItem->SetAttribute("X",Info->position.x);
				elmImageItem->SetAttribute("Y",Info->position.y);
				elmImageItem->SetAttribute("CX",Info->crect.cx);
				elmImageItem->SetAttribute("CY",Info->crect.cy);
				elmImageItem->SetAttribute("SX",Info->srect.x);
				elmImageItem->SetAttribute("SY",Info->srect.y);
				elmImageItem->SetAttribute("SCX",Info->srect.cx);
				elmImageItem->SetAttribute("SCY",Info->srect.cy);
			}

			myDoc->SaveFile((m_strPath+filename+".xml").GetBuffer());
			myDoc->Clear();
			delete myDoc;
		}
		break;
	case 0:
		{
			TiXmlDocument  *myDoc=new TiXmlDocument();
			TiXmlDeclaration * xmlDec = new TiXmlDeclaration("1.0","UTF-8","");
			myDoc->LinkEndChild(xmlDec);

			TiXmlElement *elmPlistRoot=new TiXmlElement("plist");
			elmPlistRoot->SetAttribute("version","1.0");
			myDoc->LinkEndChild(elmPlistRoot);

			TiXmlElement *elmDictRoot=new TiXmlElement("dict");
			elmPlistRoot->LinkEndChild(elmDictRoot);

			TiXmlElement *elmFrameRoot=new TiXmlElement("key");
			elmFrameRoot->LinkEndChild(new TiXmlText("frames"));
			elmDictRoot->LinkEndChild(elmFrameRoot);

			TiXmlElement *elmValueRoot=new TiXmlElement("dict");
			elmDictRoot->LinkEndChild(elmValueRoot);


			for(int j=0;j<list.size();j++)
			{        
				BImageInfo* Info=list[j];
				TiXmlElement *elmKeyItem=new TiXmlElement("key");
				elmValueRoot->LinkEndChild(elmKeyItem);
				elmKeyItem->LinkEndChild(new TiXmlText(Info->name.GetBuffer()));
				TiXmlElement *elmValueItem=new TiXmlElement("dict");
				elmValueRoot->LinkEndChild(elmValueItem);

				TiXmlElement *elmKey,*elmValue;
				CString       strTemp;
				int	colorX, colorY, colorW, colorH,sW,sH;
				colorX = -(int)Info->srect.x;
				colorY = -(int)Info->srect.y;
				colorW = (int)Info->crect.cx;
				colorH = (int)Info->crect.cy;
				sW = (int)Info->srect.cx;
				sH = (int)Info->srect.cy;
				elmKey=new TiXmlElement("key");
				elmValue=new TiXmlElement("string");
				elmValueItem->LinkEndChild(elmKey);
				elmValueItem->LinkEndChild(elmValue);

				elmKey->LinkEndChild(new TiXmlText("frame"));
				strTemp.Format("{{%d,%d},{%d,%d}}",(int)Info->position.x,(int)Info->position.y,(int)Info->crect.cx,(int)Info->crect.cy);
				elmValue->LinkEndChild(new TiXmlText(strTemp.GetBuffer()));

				elmKey=new TiXmlElement("key");
				elmValue=new TiXmlElement("string");
				elmValueItem->LinkEndChild(elmKey);
				elmValueItem->LinkEndChild(elmValue);

				elmKey->LinkEndChild(new TiXmlText("offset"));
				strTemp.Format("{%d,%d}", colorX + (colorW - sW) / 2, (sH - colorH) / 2 - colorY);
				elmValue->LinkEndChild(new TiXmlText(strTemp.GetBuffer()));

				elmKey=new TiXmlElement("key");
				elmValue=new TiXmlElement("false");
				elmValueItem->LinkEndChild(elmKey);
				elmValueItem->LinkEndChild(elmValue);

				elmKey->LinkEndChild(new TiXmlText("rotated"));
				
				elmKey=new TiXmlElement("key");
				elmValue=new TiXmlElement("string");
				elmValueItem->LinkEndChild(elmKey);
				elmValueItem->LinkEndChild(elmValue);

				elmKey->LinkEndChild(new TiXmlText("sourceColorRect"));
				strTemp.Format("{{%d,%d},{%d,%d}}", colorX, colorY, colorW, colorH);
				elmValue->LinkEndChild(new TiXmlText(strTemp.GetBuffer()));


				elmKey=new TiXmlElement("key");
				elmValue=new TiXmlElement("string");
				elmValueItem->LinkEndChild(elmKey);
				elmValueItem->LinkEndChild(elmValue);

				elmKey->LinkEndChild(new TiXmlText("sourceSize"));
				strTemp.Format("{%d,%d}",(int)Info->srect.cx,(int)Info->srect.cy);
				elmValue->LinkEndChild(new TiXmlText(strTemp.GetBuffer()));

			}
			TiXmlElement *elmMetadata=new TiXmlElement("key");
			elmMetadata->LinkEndChild(new TiXmlText("metadata"));
			elmDictRoot->LinkEndChild(elmMetadata);


			TiXmlElement *elmMetadataRoot=new TiXmlElement("dict");
			elmDictRoot->LinkEndChild(elmMetadataRoot);

			TiXmlElement *elmKey,*elmValue;
			CString       strTemp;
			elmKey=new TiXmlElement("key");
			elmValue=new TiXmlElement("string");
			elmMetadataRoot->LinkEndChild(elmKey);
			elmMetadataRoot->LinkEndChild(elmValue);

			elmKey->LinkEndChild(new TiXmlText("format"));
			elmValue->LinkEndChild(new TiXmlText("2"));

			elmKey=new TiXmlElement("key");
			elmValue=new TiXmlElement("string");
			elmMetadataRoot->LinkEndChild(elmKey);
			elmMetadataRoot->LinkEndChild(elmValue);

			elmKey->LinkEndChild(new TiXmlText("realTextureFileName"));
			elmValue->LinkEndChild(new TiXmlText((filename+".png").GetBuffer()));

			elmKey=new TiXmlElement("key");
			elmValue=new TiXmlElement("string");
			elmMetadataRoot->LinkEndChild(elmKey);
			elmMetadataRoot->LinkEndChild(elmValue);

			elmKey->LinkEndChild(new TiXmlText("size"));
			strTemp.Format("{%d,%d}",(int)size.cx,(int)size.cy);
			elmValue->LinkEndChild(new TiXmlText(strTemp.GetBuffer()));

			elmKey=new TiXmlElement("key");
			elmValue=new TiXmlElement("string");
			elmMetadataRoot->LinkEndChild(elmKey);
			elmMetadataRoot->LinkEndChild(elmValue);

			elmKey->LinkEndChild(new TiXmlText("smartupdate"));
			elmValue->LinkEndChild(new TiXmlText("BindImage2.4"));

			elmKey=new TiXmlElement("key");
			elmValue=new TiXmlElement("string");
			elmMetadataRoot->LinkEndChild(elmKey);
			elmMetadataRoot->LinkEndChild(elmValue);

			elmKey->LinkEndChild(new TiXmlText("textureFileName"));
			elmValue->LinkEndChild(new TiXmlText((filename+".png").GetBuffer()));

			myDoc->SaveFile((m_strPath+filename+".plist").GetBuffer());
			myDoc->Clear();
			delete myDoc;
		}
		break;
	}
	
	
}

void CImagePack::setCurInfo( const tagHistoryInfo& _info )
{
	GetDlgItem(IDC_FILEHOSTORY)->SetWindowText(_info.strFilePath);
	GetDlgItem(IDC_FILEEDIT)->SetWindowText(_info.strFileName);
	m_listType.SetCurSel(_info.nDataType);
	CString strTemp;
	strTemp.Format("%d",_info.nMaxWidth);
	m_MaxWEdit.SetWindowText(strTemp.GetString());
	strTemp.Format("%d",_info.nMaxHeight);
	m_MaxHEdit.SetWindowText(strTemp.GetString());
	strTemp.Format("%d",_info.nSpace);
	m_SpaceEdit.SetWindowText(strTemp.GetString());
}

void CImagePack::insertComboBox( const CString& _str )
{
	if(m_HistoryCombo.GetCount()>=m_nMaxSize)
		m_HistoryCombo.DeleteString(0);
	CSize size = m_pDCCombo->GetTextExtent(_str);
	size.cx += m_tmCombo.tmAveCharWidth;
	if(size.cx>m_nMaxComboxWidth)
		m_nMaxComboxWidth = size.cx;
	m_HistoryCombo.AddString(_str);
	
	m_HistoryCombo.SetDroppedWidth(m_nMaxComboxWidth+m_nExtendWidth);
}
void CImagePack::OnCbnSelchangeFilehostory()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nCurIndex = m_HistoryCombo.GetCurSel();
	setCurInfo(m_vecHistory[m_nCurIndex]);
}


void CImagePack::OnCbnDblclkFilehostory()
{
	
	// TODO: 在此添加控件通知处理程序代码
}


void CImagePack::OnCbnSetfocusFilehostory()
{
	// TODO: 在此添加控件通知处理程序代码
	m_HistoryCombo.ShowDropDown();
}


void CImagePack::OnBnClickedLuacore()
{
	// TODO: 在此添加控件通知处理程序代码
	if(((CButton*)GetDlgItem(IDC_LUACORE))->GetCheck())
	{
		AllocConsole();  
		freopen("CONOUT$","w+t",stdout);  
		freopen("CONIN$","r+t",stdin);  
	}else
	{
		fclose(stdout);
		fclose(stdin);
		FreeConsole();
	}

}
