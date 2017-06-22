#include "stdafx.h"
#include "BindImage.h"
#include "BindCore.h"
#include "string"
#include "tinyxml/tinyxml.h"
#include "optipng/optipng.h"
//extern "C" int errno()
//{
//	return GetLastError();
//}
#ifdef DEBUG
extern "C" { 
	//FILE _iob[3] = {__iob_func()[0], __iob_func()[1], __iob_func()[2]}; 
	//const unsigned short* _pctype = __pctype_func();

	//int  __mb_cur_max = ___mb_cur_max_func();
}
#endif
CBindCore::CBindCore(void)
{
    
}
CBindCore::~CBindCore(void)
{
 
}
void CBindCore::Reset()
{
	m_RemainList.clear();
	for (int i=0;i<m_ImgList.size();i++)
		delete m_ImgList[i];
	m_ImgList.clear();

	if(!m_ImgBuffer.IsNull())
		m_ImgBuffer.Destroy();
}
void CBindCore::Init(int _Width,int  _Height,int _iSpace)
{
	m_rcBuffer.x=m_rcBuffer.y=0;
	m_sizeMax(_Width,_Height);	
	m_iSpace=_iSpace;
}

BImageInfo* CBindCore::QueryOne()
{
	
	{
		for (int i=m_ImgList.size()-1;i>=0;i--)
		{
			if(m_ImgList[i]->bUser==false)
				return m_ImgList[i];
		}
	}
	return 0;
}
#define  left_top      0x01
#define  right_top     0x02
#define  left_bottom   0x04
#define  right_bottom  0x08
bool CBindCore::CheckClipRect(CfRect& rect,CfRect& crect)
{
     //CNgeRect crect=m_RemainList[clipindex];
	// m_RemainList.Delete(clipindex);
    
	 char value=0;
     if(crect.InCollide(rect.x,rect.y))
         value|=left_top;
	 if(crect.InCollide(rect.x+rect.cx,rect.y))
		 value|=right_top;
	 if(crect.InCollide(rect.x,rect.y+rect.cy))
		 value|=left_bottom;
	 if(crect.InCollide(rect.x+rect.cx,rect.y+rect.cy))
		 value|=right_bottom;
    CfRect  trect;
	CNgePoint point;
	CfSize  size;
	switch(value)
	{
	case left_top:
		size(rect.x-crect.x,crect.cy);
		trect(crect.x,crect.y,size.cx,size.cy);
		if(trect.cx>=1.f&&trect.cy>=1.f)
			m_RemainList.push_back(trect);

		size(crect.cx,rect.y-crect.y);
		trect(crect.x,crect.y,size.cx,size.cy);
		if(trect.cx>=1.f&&trect.cy>=1.f)
			m_RemainList.push_back(trect);


		break;
	case right_top:
		size(crect.cx,rect.y-crect.y);
		trect(crect.x,crect.y,size.cx,size.cy);
		if(trect.cx>=1.f&&trect.cy>=1.f)
			m_RemainList.push_back(trect);


		size(crect.x+crect.cx-rect.x-rect.cx,crect.cy);
		trect(rect.x+rect.cx,crect.y,size.cx,size.cy);
		if(trect.cx>=1.f&&trect.cy>=1.f)
			m_RemainList.push_back(trect);


		break;
	case left_bottom:

		size(rect.x-crect.x,crect.cy);
		trect(crect.x,crect.y,size.cx,size.cy);
		if(trect.cx>=1.f&&trect.cy>=1.f)
			m_RemainList.push_back(trect);

		size(crect.cx,rect.y-crect.y);
		trect(crect.x,crect.y,size.cx,size.cy);
		if(trect.cx>=1.f&&trect.cy>=1.f)
			m_RemainList.push_back(trect);


		break;
	case right_bottom:
		

		size(crect.x+crect.cx-rect.x-rect.cx,crect.cy);
		trect(rect.x+rect.cx,crect.y,size.cx,size.cy);
		if(trect.cx>=1.f&&trect.cy>=1.f)
			m_RemainList.push_back(trect);

		size(crect.cx,crect.y+crect.cy-rect.y-rect.cy);
		trect(crect.x,rect.y+rect.cy,size.cx,size.cy);
		if(trect.cx>=1.f&&trect.cy>=1.f)
			m_RemainList.push_back(trect);
		break;
	case left_top|right_top:
		
		size(rect.x-crect.x,crect.cy);
		trect(crect.x,crect.y,size.cx,size.cy);
		if(trect.cx>=1.f&&trect.cy>=1.f)
			m_RemainList.push_back(trect);

		size(crect.cx,rect.y-crect.y);
		trect(crect.x,crect.y,size.cx,size.cy);
		if(trect.cx>=1.f&&trect.cy>=1.f)
			m_RemainList.push_back(trect);


		size(crect.x+crect.cx-rect.x-rect.cx,crect.cy);
		trect(rect.x+rect.cx,crect.y,size.cx,size.cy);
		if(trect.cx>=1.f&&trect.cy>=1.f)
			m_RemainList.push_back(trect);

		break;
	case left_top|left_bottom:
		
		size(rect.x-crect.x,crect.cy);
		trect(crect.x,crect.y,size.cx,size.cy);
		if(trect.cx>=1.f&&trect.cy>=1.f)
			m_RemainList.push_back(trect);

		size(crect.cx,rect.y-crect.y);
		trect(crect.x,crect.y,size.cx,size.cy);
		if(trect.cx>=1.f&&trect.cy>=1.f)
			m_RemainList.push_back(trect);

		size(crect.cx,crect.y+crect.cy-rect.y-rect.cy);
		trect(crect.x,rect.y+rect.cy,size.cx,size.cy);
		if(trect.cx>=1.f&&trect.cy>=1.f)
			m_RemainList.push_back(trect);
		break;
	case right_top|right_bottom:

		size(crect.cx,rect.y-crect.y);
		trect(crect.x,crect.y,size.cx,size.cy);
		if(trect.cx>=1.f&&trect.cy>=1.f)
			m_RemainList.push_back(trect);


		size(crect.x+crect.cx-rect.x-rect.cx,crect.cy);
		trect(rect.x+rect.cx,crect.y,size.cx,size.cy);
		if(trect.cx>=1.f&&trect.cy>=1.f)
			m_RemainList.push_back(trect);

		size(crect.cx,crect.y+crect.cy-rect.y-rect.cy);
		trect(crect.x,rect.y+rect.cy,size.cx,size.cy);
		if(trect.cx>=1.f&&trect.cy>=1.f)
			m_RemainList.push_back(trect);
		break;
	case left_bottom|right_bottom:
		size(rect.x-crect.x,crect.cy);
		trect(crect.x,crect.y,size.cx,size.cy);
		if(trect.cx>=1.f&&trect.cy>=1.f)
			m_RemainList.push_back(trect);



		size(crect.x+crect.cx-rect.x-rect.cx,crect.cy);
		trect(rect.x+rect.cx,crect.y,size.cx,size.cy);
		if(trect.cx>=1.f&&trect.cy>=1.f)
			m_RemainList.push_back(trect);

			size(crect.cx,crect.y+crect.cy-rect.y-rect.cy);
		trect(crect.x,rect.y+rect.cy,size.cx,size.cy);
		if(trect.cx>=1.f&&trect.cy>=1.f)
			m_RemainList.push_back(trect);

		break;
	case left_bottom|right_bottom|left_top|right_top:
		size(rect.x-crect.x,crect.cy);
		trect(crect.x,crect.y,size.cx,size.cy);
		if(trect.cx>=1.f&&trect.cy>=1.f)
			m_RemainList.push_back(trect);

		size(crect.cx,rect.y-crect.y);
		trect(crect.x,crect.y,size.cx,size.cy);
		if(trect.cx>=1.f&&trect.cy>=1.f)
			m_RemainList.push_back(trect);


		size(crect.x+crect.cx-rect.x-rect.cx,crect.cy);
		trect(rect.x+rect.cx,crect.y,size.cx,size.cy);
		if(trect.cx>=1.f&&trect.cy>=1.f)
			m_RemainList.push_back(trect);

		size(crect.cx,crect.y+crect.cy-rect.y-rect.cy);
		trect(crect.x,rect.y+rect.cy,size.cx,size.cy);
		if(trect.cx>=1.f&&trect.cy>=1.f)
			m_RemainList.push_back(trect);

		break;
	}
	return true;
}
bool CBindCore::CheckSameImage(BImageInfo* _pTempinfo)
{
	if(!m_bUsedSame)
		return false;
	for (int i=m_ImgList.size()-1;i>=0;i--)
	{
		if(m_ImgList[i]->bUser==true){
			CfRect rect = m_ImgList[i]->crect;
			if(rect.cx == _pTempinfo->crect.cx&&\
				rect.cy == _pTempinfo->crect.cy)
			{
				if(CheckImageMatter(_pTempinfo,m_ImgList[i]))
				{
					_pTempinfo->bUser = true;
					_pTempinfo->crect = rect;
					return true;
				}
			}
		}
	}
	return false;
}
bool CBindCore::SingleCount()
{
	BImageInfo* tempinfo;
	while(tempinfo=QueryOne())
	{	 
		int i=0;
		CfRect rect;
		RectList::iterator iter=m_RemainList.begin();
		if(CheckSameImage(tempinfo))
			continue;
		for (;iter<m_RemainList.end();iter++)
		{
			rect=(*iter);
			if(rect.cx>tempinfo->crect.cx&&rect.cy>tempinfo->crect.cy)
				break;
		}
		if(iter==m_RemainList.end())
			return false;
		m_RemainList.erase(iter);
		tempinfo->bUser=true;
		tempinfo->position(rect.x,rect.y);
		tempinfo->crect.x=rect.x;
		tempinfo->crect.y=rect.y;
		CfRect tInRect=tempinfo->crect;
        tInRect.cx+=m_iSpace;
		tInRect.cy+=m_iSpace;
		CheckClipRect(tInRect,rect);
		iter=m_RemainList.begin();
		for (i=0;i<m_RemainList.size();i++)
		{
			iter=m_RemainList.begin()+i;
			CfRect trect=m_RemainList[i];
			if(trect.InCollide(tInRect))
			{
				m_RemainList.erase(iter);
				CheckClipRect(tInRect,trect);
				i--;
			}
		}
		if(m_RemainList.size()>0)
			QuickRect(m_RemainList,0,m_RemainList.size()-1);
		iter=m_RemainList.begin();
		for (int i=0;i<m_RemainList.size();i++)
		{
			iter=m_RemainList.begin()+i;
			for (int j=i+1;j<m_RemainList.size();j++)
			{
				if(m_RemainList[i].InRect(m_RemainList[j]))
				{
					m_RemainList.erase(iter);
					i--;
					break;
				}
			}
		}
		if(m_RemainList.size()>0)
			QuickRect(m_RemainList,0,m_RemainList.size()-1);
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

void CBindCore::setFilePath(CString _strPath){
	m_strPath=_strPath.Left(_strPath.ReverseFind('\\')+1);
}
bool CBindCore::AutoBind(CString& filename,bool _bOptimize,bool _b2m)
{
  if(m_ImgList.size()<=0){
	AfxMessageBox("文件夹没有图片！");
	return false;
  }
  QuickSort(m_ImgList,0,m_ImgList.size()-1);
  m_bBegin=false;
  CfRect rectFirst,rectNext;
  if(!CheckMin(rectFirst,true)){
	  	AfxMessageBox("创建失败,图片超出最大范围！");
		return false;
  }
   
  if(!CheckMin(rectNext,false))
  {
	  AfxMessageBox("创建失败,图片超出最大范围！");
	  return false;
  }

  if(rectFirst.cx*rectFirst.cy<rectNext.cx*rectNext.cy)
  {
	  CheckMin(rectFirst,true);
  }
  
  m_ImgBuffer.Create( m_rcBuffer.cx,m_rcBuffer.cy,32,CImage::createAlphaChannel);
  for (int i=0;i<m_ImgList.size();i++)
  {
	  BImageInfo* tempinfo=m_ImgList[i];
	 tempinfo->image.BitBlt(m_ImgBuffer.GetDC(),tempinfo->crect.x,tempinfo->crect.y,tempinfo->crect.cx,tempinfo->crect.cy,\
		 -tempinfo->srect.x,-tempinfo->srect.y,SRCPAINT);
	  m_ImgBuffer.ReleaseDC();
  }
  CfRect crect,srect;
  srect = crect = m_rcBuffer;
  //if(!_b2m)
  AnalyseImage(m_ImgBuffer,crect,srect,false);
  if(_b2m)
  {
	  CfSize  rectTemp(64,64);
	  while ((rectTemp.cx*=2)<crect.cx){};
	  while ((rectTemp.cy*=2)<crect.cy){};
	  crect.cx = rectTemp.cx;
	  crect.cy = rectTemp.cy;
  }
  m_sizeImage.cx=crect.cx;
  m_sizeImage.cy=crect.cy;
  
  CImage timage;
  timage.Create(crect.cx,crect.cy,32,CImage::createAlphaChannel);
  //DWORD dwBitmasks[] = {8, 8, 8};  
  //timage.CreateEx(crect.cx,crect.cy,24,BI_BITFIELDS,dwBitmasks);
  m_ImgBuffer.BitBlt(timage.GetDC(),0,0,crect.cx,crect.cy,crect.x,crect.y,SRCCOPY);
  timage.ReleaseDC();
  timage.Save((m_strPath+filename+".png").GetBuffer(),Gdiplus::ImageFormatPNG);
  timage.Destroy();
  
  if(_bOptimize){
	  struct opng_ui ui;
	  /* Initialize the optimization engine. */
	  ui.printf_fn      = app_printf;
	  ui.print_cntrl_fn = app_print_cntrl;
	  ui.progress_fn    = app_progress;
	  ui.panic_fn       = panic;
	  ProcessDlg.ShowWindow(SW_SHOW);
	  ProcessDlg.SetProcess(9);
	  ProcessDlg.SetProcessSub(9);
	  static struct opng_options options;
	  memset(&options, 0, sizeof(options));
	  options.optim_level = -1;
	  options.interlace = -1;
	  if (opng_initialize(&options, &ui) != 0)
		  printf("Can't initialize optimization engine");

	  opng_optimize((m_strPath+filename+".png").GetBuffer());
	  if (opng_finalize() != 0)
		  printf("Can't finalize optimization engine");
	  ProcessDlg.ShowWindow(SW_HIDE);
  }
  
  
  return true;
  
}
bool CBindCore::ChangeImageBPP(CImage& src)
{
	
	int  nPitch = src.GetPitch( ) , nBPP = src.GetBPP( );
	bool bHaveTable = src.IsIndexed();
	int  MaxColors = src.GetMaxColorTableEntries();
	RGBQUAD* ColorTable = 0;
	if(bHaveTable)
	{
		ColorTable = new RGBQUAD[MaxColors];
		src.GetColorTable(0,MaxColors,ColorTable);
	}
	CImage des;
	des.Create(src.GetWidth(),src.GetHeight(),32,CImage::createAlphaChannel);
	LPBYTE lpBits = reinterpret_cast< LPBYTE>( src.GetBits( ));
	LPBYTE lpTBits= reinterpret_cast< LPBYTE>( des.GetBits( ));
	for( int yPos = 0 ; yPos < src.GetHeight() ; yPos ++ )
	{
		LPBYTE lpBytes = lpBits + ( yPos * nPitch );
		LPBYTE lptBytes = lpTBits + ( yPos * des.GetPitch());
		LPBYTE lpLines = reinterpret_cast< LPBYTE >( lpBytes );
		PDWORD tlpLines= reinterpret_cast< PDWORD >( lptBytes );
		for( int xPos = 0;xPos < src.GetWidth() ; xPos ++ )
		{
			//DWORD data;=*((DWORD*)(lpLines+xPos*(nBPP/8)));
			if(!bHaveTable)
			{
				memcpy(tlpLines+xPos,lpLines+xPos*(nBPP/8),nBPP/8);
			}else
			{
				memcpy(tlpLines+xPos,(void*)&ColorTable[*(lpLines+xPos*(nBPP/8))],4);
			}
			tlpLines[xPos]|=0xff000000;
		}
	}
	if(ColorTable)
		delete[] ColorTable;
    src.Destroy();
	src.Attach(des.Detach());
	return true;
}


bool CBindCore::CheckImageMatter(BImageInfo* _pInfo1,BImageInfo* _pInfo2)
{
	if(_pInfo1==0||_pInfo2==0)
		AfxMessageBox("比较错误!",MB_OK|MB_ICONINFORMATION);
	if(_pInfo1->image.GetBPP()!=_pInfo2->image.GetBPP())
		return false;
	int nPitch1 = _pInfo1->image.GetPitch();
	int nPitch2 = _pInfo2->image.GetPitch();
	int nAllBPP = _pInfo1->image.GetBPP()/8;
	LPBYTE lpBits1 = reinterpret_cast< LPBYTE>( _pInfo1->image.GetBits());
	LPBYTE lpBits2 = reinterpret_cast< LPBYTE>( _pInfo2->image.GetBits());
	
	CfRect TmpRect = _pInfo1->crect;

	CfRect rectSrc1 = _pInfo1->srect;

	CfRect rectSrc2 = _pInfo2->srect;
	
	for( int yPos = 0 ; yPos < TmpRect.cy ; yPos ++ )
	{
		LPBYTE lpBytes1 = lpBits1 + ((yPos-rectSrc1.y)* nPitch1 ) -rectSrc1.x*nAllBPP;
		LPBYTE lpBytes2 = lpBits2 + ((yPos-rectSrc2.y)* nPitch2 ) - rectSrc2.x*nAllBPP;
		PDWORD lpLines1 = reinterpret_cast< PDWORD >( lpBytes1 );
		PDWORD lpLines2 = reinterpret_cast< PDWORD >( lpBytes2 );
		
		for( int xPos = 0;xPos < TmpRect.cx ; xPos ++ )
		{
			if(lpLines1[xPos]!=lpLines2[xPos])
				return false;
		}
	}
	return true;
}

bool CBindCore::AnalyseImage(CImage& img,CfRect& crect,CfRect& srect,bool bFixed)
{
	int nPitch = img.GetPitch( ) , nBPP = img.GetBPP( );
	LPBYTE lpBits = reinterpret_cast< LPBYTE>( img.GetBits( ) );
	CRect TmpRect;
	int iPlace=nBPP-8;
	TmpRect.left=img.GetWidth();
	TmpRect.right=0;
	TmpRect.top=img.GetHeight();
	TmpRect.bottom=0;
	bool top=false;
	bool bottom=false;
	for( int yPos = 0 ; yPos < img.GetHeight() ; yPos ++ )
	{
		LPBYTE lpBytes = lpBits + ( yPos * nPitch );
		PDWORD lpLines = reinterpret_cast< PDWORD >( lpBytes );
		bool left=false;
		bool right=false;
		for( int xPos = 0;xPos < img.GetWidth() ; xPos ++ )
		{
			if(!left)
			if((lpLines[ xPos ] >> iPlace)!=0)
			{
			   if(xPos<TmpRect.left)
                  TmpRect.left=xPos;
			   left=true;
			}
			if(!right)
			if((lpLines[img.GetWidth()-1-xPos]>>iPlace)!=0)
			{
			    if(img.GetWidth()-xPos>TmpRect.right)
				     TmpRect.right=img.GetWidth()-xPos;
				right=true;
			} 
			if(left&&right)	  
			{
				if(!top)
				{
					TmpRect.top=yPos;
					top=true;
				}  
				break;
			}
			
			
		}
		int  Currenty=img.GetHeight()-1-yPos;
		left=false;
		right=false;
		lpBytes = lpBits + ( Currenty * nPitch );
		lpLines = reinterpret_cast< PDWORD >( lpBytes );
		for( int xPos = 0 ; xPos < img.GetWidth() ; xPos ++ )
		{
		    if(!left)
				if((lpLines[ xPos ]>>iPlace)!=0)
				{
					if(xPos<TmpRect.left)
						TmpRect.left=xPos;
					left=true;
				}
			if(!right)
				if((lpLines[img.GetWidth()-1-xPos]>>iPlace)!=0)
				{
					if(img.GetWidth()-xPos>TmpRect.right)
						TmpRect.right=img.GetWidth()-xPos;
					right=true;
				} 
				if(left&&right)
				{
					if(!bottom)
					{
                        TmpRect.bottom=Currenty+1;
						bottom=true;
					}
					break;
				}
			
		}
	}

	crect.x = 0;
	crect.y = 0;
	crect.cx=TmpRect.Width();
	crect.cy=TmpRect.Height();

	srect.cx = img.GetWidth();
	srect.cy = img.GetHeight();
	if (bFixed) {
		crect.x = TmpRect.left;
		crect.y = TmpRect.top;
		if ((srect.cx - crect.cx) % 2)
		{
			crect.cx += 1;
			crect.x -= 1;
		}
		if ((srect.cy - crect.cy) % 2)
		{
			crect.cy += 1;
			crect.y -= 1;
		}
	}

	srect.x = -crect.x;
	srect.y = -crect.y;
	
	return true;
	
}
bool CBindCore::CheckMin(CfRect& rect,bool _bFirst){

	m_rcBuffer.cx=32;
	m_rcBuffer.cy=32;
	m_bFirst=_bFirst;
	m_iWBase=64;
    m_iHBase=64;
	while(true)
	{
		m_RemainList.clear();
		for (int i=0;i<m_ImgList.size();i++)
			m_ImgList[i]->bUser=false;
		m_RemainList.push_back(m_rcBuffer);
		if(SingleCount())
			break;
		if(!m_bFirst)
		{
			m_rcBuffer.cx+=16;
			if(m_rcBuffer.cx>=m_iWBase){
				m_iWBase*=2;
				m_bFirst=true; 
			}
			
		}else
		{
			m_rcBuffer.cy+=16;
			if(m_rcBuffer.cy>=m_iHBase){
				m_iHBase*=2;
				m_bFirst=false; 
			}	
		}
		
		if(m_rcBuffer.cx>m_sizeMax.cx+1||m_rcBuffer.cy>m_sizeMax.cy+1)
			return false;
		if(m_rcBuffer.cx==m_sizeMax.cx)
			m_rcBuffer.cx++;
		if(m_rcBuffer.cy==m_sizeMax.cy)
			m_rcBuffer.cy++;
		
	}  
    rect=m_rcBuffer;
    return true;
}
void  CBindCore::QuickRect(RectList& list, int first, int end)
{
	int i=first,j=end;
	CfRect temp=list[first];
	/*while(i<j)
	{
		while(i<j&&list[j].cx*list[j].cy>=temp.cx*temp.cy)
			j--;
		list[i]=list[j];
		while(i<j&&list[i].cx*list[i].cy<=temp.cx*temp.cy)
			i++;
		list[j]=list[i];
	}*/
	/*while(i<j)
	{
		while(i<j&&list[j].y>=temp.y)
			j--;
		list[i]=list[j];
		while(i<j&&list[i].y<=temp.y)
			i++;
		list[j]=list[i];
	}*/
	if(m_bFirst){
		while(i<j)
		{
			while(i<j&&list[j].x>=temp.x)
				j--;
			list[i]=list[j];
			while(i<j&&list[i].x<=temp.x)
				i++;
			list[j]=list[i];
		}
	}else{
		while(i<j)
		{
			while(i<j&&list[j].y>=temp.y)
				j--;
			list[i]=list[j];
			while(i<j&&list[i].y<=temp.y)
				i++;
			list[j]=list[i];
		}
	}
	/*while(i<j)
	{
		while(i<j&&list[j].y+list[j].x>=temp.y+temp.x)
			j--;
		list[i]=list[j];
		while(i<j&&list[i].y+list[i].x<=temp.y+temp.x)
			i++;
		list[j]=list[i];
	}*/
	list[i]=temp;
	if(first<i-1)
		QuickRect(list,first,i-1);
	if(end>i+1)		
		QuickRect(list,i+1,end);
}
void  CBindCore::QuickSort(InfoList& list, int first, int end)
{
	int i=first,j=end;
	BImageInfo* temp=list[first];
	while(i<j)
	{
		while(i<j&&list[j]->crect.cx+list[j]->crect.cy>=temp->crect.cx+temp->crect.cy)
			j--;
		list[i]=list[j];
		while(i<j&&list[i]->crect.cx+list[i]->crect.cy<=temp->crect.cx+temp->crect.cy)
			i++;
		list[j]=list[i];
	}
    /*while(i<j)
	{
		while(i<j&&list[j]->crect.cx>=temp->crect.cx)
			j--;
		list[i]=list[j];
		while(i<j&&list[i]->crect.cx<=temp->crect.cx)
			i++;
		list[j]=list[i];
	}*/
	list[i]=temp;
	if(first<i-1)
		QuickSort(list,first,i-1);
	if(end>i+1)		
		QuickSort(list,i+1,end);
}
bool CBindCore::AddImage(CString& str,CString& name)
{
	BImageInfo* info=new BImageInfo;
	info->bUser=false;
	info->image.Load(str);
	info->name=name;
	if(info->image.GetBPP()!=32)
	   ChangeImageBPP(info->image);
	AnalyseImage(info->image,info->crect,info->srect);
	m_ImgList.push_back(info);
	return true;
}

void CBindCore::enableUsedSame( bool _bUsed )
{
	m_bUsedSame = _bUsed;
}
