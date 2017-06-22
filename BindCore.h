#pragma once
#include <atlimage.h>
#include <CString>
#include <vector>
#include <Windows.h>

using namespace std;
struct CfRect
{
	int x;
	int y;
	int cx;
	int cy;
public:
	CfRect()
	{

	}
	CfRect(int x,int y,int cx,int cy)
	{
		this->x=x;
		this->y=y;
		this->cx=cx;
		this->cy=cy;

	}
	void operator()(int x,int y,int cx,int cy)
	{
		this->x=x;
		this->y=y;
		this->cx=cx;
		this->cy=cy;
	}
	inline bool  InCollide(CfRect& rect)
	{
		/*if(x==rect.x&&y==rect.y&&cx==rect.cx&&cy==rect.cy)
			return false;
		if(x==rect.x)
		{
			x=x;
		}*/
		if(x+cx>rect.x&&x<rect.x+rect.cx&&y+cy>rect.y&&y<rect.y+rect.cy)
			return true;
		return false;
	}
	inline bool  InCollide(float x,float y)
	{
		if(x>=this->x&&x<=this->x+this->cx&&y>=this->y&&y<=this->y+this->cy)
			return true;
		return false;
	}
	inline bool  InRect(CfRect& rect)
	{
		if(x>=rect.x&&y>=rect.y&&x+cx<=rect.x+rect.cx&&y+cy<=rect.y+rect.cy)
			return true;
		return false;
	}
};
struct CNgePoint
{
	int x;
	int y;
public:
	CNgePoint()
	{
		x=0;
		y=0;
	}
	CNgePoint(int x,int y)
	{
		this->x=x;
		this->y=y;
	}
	void operator()(int x,int y)
	{
		this->x=x;
		this->y=y;
	}
};
struct CfSize
{
	int cx;
	int cy;
public:
	CfSize()
	{
		cx=0;
		cy=0;
	}
	CfSize(int cx,int cy)
	{
		this->cx=cx;
		this->cy=cy;
	}
	void operator()(int cx,int cy)
	{
		this->cx=cx;
		this->cy=cy;
	}
};
struct BImageInfo
{
	CImage    image;
	CfRect  crect;
	CfRect  srect;
	CNgePoint position;
	CString   name;
	bool      bUser;
};
typedef  vector<BImageInfo*> InfoList;
typedef  vector<CfRect>   RectList;
class CBindCore
{
public:
	CBindCore(void);
	~CBindCore(void);
	bool       AnalyseImage(CImage& img,CfRect& crect,CfRect& srect,bool bFixed = true);
	bool	   CheckImageMatter(BImageInfo* _pInfo1,BImageInfo* _pInfo2);
	bool       ChangeImageBPP(CImage& src);
	void       Init(int _Width,int  _Height,int _iSpace);
	void       setFilePath(CString _strPath);
	void       Reset();
	bool       AutoBind(CString& filename,bool _bOptimize,bool _b2m);
	bool       SingleCount();
	bool       CheckClipRect(CfRect& rect,CfRect& crect);
	BImageInfo* QueryOne();
	CImage*    GetImgBuffer(){return &m_ImgBuffer;};
	bool       AddImage(CString& str,CString& name);
	void       QuickSort(InfoList& list, int first, int end);
	void       QuickRect(RectList& list, int first, int end);
	bool       CheckMin(CfRect& rect,bool _bFirst);

	void	   enableUsedSame(bool _bUsed);

    inline     InfoList&  GetImageList(){return m_ImgList;}
	 inline    CfSize     GetImageSize(){return m_sizeImage;}
	 bool				  CheckSameImage(BImageInfo* _pTempinfo);
private:
    InfoList        m_ImgList;
	RectList        m_RemainList;
	CImage          m_ImgBuffer;
	CfRect			m_rcBuffer;
	CfSize			m_sizeImage;
	CfSize			m_sizeMax;
	int				m_iWBase;
	int				m_iHBase;
	CString         m_strPath;
	int             m_iSpace;
	bool            m_bBegin;
	CPoint          m_ptSize;
	CPoint          m_ptCurrentV;
	DWORD           m_dwLeftY;
	DWORD           m_dwRightY;
	bool            m_bFirst;
	bool			m_bUsedSame;
};
