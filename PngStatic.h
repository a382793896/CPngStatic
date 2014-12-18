/************************************************************
* 类名:CPngStatic
* 作用：在vc dialog中使用picture控件显示png图片，并相应鼠标
*       消息，可做picture button使用
* 注意：如果有背景时，背景控件必须在pngButton控件之下（tab=1）
* 原理：使用控件背景覆盖法实现png图片的重画和透明效果
* 王群  2014年5月26日12:23:52 
*		1、第一次修改
*		2014年11月17日16:01:52
*		2、第二次修改  
*************************************************************/
    
#ifndef _PNGBUTTON_ZZZILI
#define _PNGBUTTON_ZZZILI

#include "stdafx.h"

#include "stdafx.h"
#include  <gdiplus.h>
using namespace Gdiplus;
//////////////////////////////////////////////////////////////////////////
//按钮消息，自定义添加
//////////////////////////////////////////////////////////////////////////


//鼠标单击消息
#define PNG_M_CLICKED			WM_USER + 1000

class CMyImage:public Image
{
public:
	CMyImage(UINT uID)
	{
		// 初始化 Image 成员变量
		nativeImage = NULL;
		lastResult = InvalidParameter;

		// 查找图片资源
		HINSTANCE hInst =  GetModuleHandle(NULL);
		HRSRC hRsrc = FindResource(hInst, MAKEINTRESOURCE(uID), _T("PNG"));
		if (!hRsrc) {
			return;
		}

		// 加载资源并获取其指针
		HGLOBAL hMemRsrc = LoadResource(hInst, hRsrc);
		BYTE * pMemRsrc = (BYTE *)LockResource(hMemRsrc);

		// 为流分配全局内存空间
		size_t sizeRsrc = SizeofResource(hInst, hRsrc);
		HGLOBAL hMemStrm = GlobalAlloc(GMEM_FIXED, sizeRsrc);
		if (!hMemStrm) {
			return;
		}

		// 拷贝图片资源到全局内存空间中
		BYTE * pMemStrm = (BYTE *)GlobalLock(hMemStrm);
		memcpy(pMemStrm, pMemRsrc, sizeRsrc);

		// 使用图片资源流构造 Image 对象
		IStream * pStrm = NULL;
		CreateStreamOnHGlobal(hMemStrm, TRUE, &pStrm);
		lastResult = DllExports::GdipLoadImageFromStream(pStrm, &nativeImage);
		pStrm->Release();

	} 

}; 

class CPngStatic : public CStatic
{
public:
	enum ALIGN_TYPE
	{
		LEFT,
		MIDDLE,
		RIGHT
	};

public:
	CPngStatic();
	~CPngStatic();
	void SetInitPngPic(int id);//设置初始png图片
	void SetClickDownPngPic(int id);//设置鼠标左键按下时png图片
	void SetClickUpPngPic(int id);//设置鼠标左键松开时png图片
	void SetMouseOnPngPic(int id);//设置鼠标在控件上时png图片
	void SetButtonPicture(LPCTSTR m_strMouseNormal ,LPCSTR m_strMouseDown = NULL,LPCSTR m_strMouseUp = NULL ,LPCSTR m_strMouseHover = NULL,LPCSTR bgImg=NULL);
	void SetText(LPCTSTR text,int TextAlign = LEFT);//设置按钮上面的字符串
	void SetFont(CFont* pFont);//设置字体
	int GetWidth();
	int GetHeight();
	CRect GetRect();
	void SetData(const DWORD_PTR lpData);
	DWORD_PTR GetData() const;
	void SetLeftBottomText(LPCTSTR text);//设置左下角字符串
	void SetRightBottomText(LPCTSTR text);//设置右下角文字
	void SetLeftBottomFont(CFont* pFont);//设置左下角字体
	void SetRightBottomFont(CFont* pFont);//设置左下角字体
	void SetCheckPicture(LPCTSTR CheckedImage);//设置选中后的图片
	void SetCheck(BOOL bCheck);
	void RefreshUi();
private:
	RECT rp,lp;
	CDC *parentCDC;
	
	BOOL isGetBj;
	CString pngFile;
	CRect m_rect;
	CRect m_bjRect;
	Image *m_image;
	DWORD_PTR m_data;
	CString m_text,m_leftBottomText,m_rightBottomText;
	CFont * m_font;
	CFont * m_leftFont;
	CFont * m_rightFont;
	int m_TextAlign;
	BOOL m_bCheck;
private:
	CDC* GetParentBkgnd(CWnd *pWnd, CPoint ptDrawStart, int nWidth,int nHeight);
	CDC* CreateMixDC(CDC* pParentMemDC, Image* image);
	CDC* CreateBgDC();
	void AlphaGradientDraw(CWnd* pWnd, CDC* pFgndMemDC, CDC* pBkgndMemDC,CPoint ptDrawStart, int nWidth, int nHeight, int nTransparence);
private:
	Image *ClickDownImage;
	Image *ClickUpImage;
	Image *MouseOnImage;
	Image *InitImage;
	Image *m_CheckedImage;
	Image *m_BgImage;
	BOOL isMouseOnPic;
	BOOL isSetClickDownImage;
	BOOL isSetClickUpImage;
	BOOL isSetMouseOnImage;
	BOOL isSetInitImage;
	BOOL isSetCheckedImage;
private:
	void SetBj();
	void SetBitmap(Image *image);
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	CString GetFullFilePath(CString Ini_FileName);
	BOOL LoadPicture(LPCTSTR szFileName,Image **m_pPicture/*,CImage *m_pImage*/);
	void DrawText();
	

};

#endif