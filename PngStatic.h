/************************************************************
* ����:CPngStatic
* ���ã���vc dialog��ʹ��picture�ؼ���ʾpngͼƬ������Ӧ���
*       ��Ϣ������picture buttonʹ��
* ע�⣺����б���ʱ�������ؼ�������pngButton�ؼ�֮�£�tab=1��
* ԭ��ʹ�ÿؼ��������Ƿ�ʵ��pngͼƬ���ػ���͸��Ч��
* ��Ⱥ  2014��5��26��12:23:52 
*		1����һ���޸�
*		2014��11��17��16:01:52
*		2���ڶ����޸�  
*************************************************************/
    
#ifndef _PNGBUTTON_ZZZILI
#define _PNGBUTTON_ZZZILI

#include "stdafx.h"

#include "stdafx.h"
#include  <gdiplus.h>
using namespace Gdiplus;
//////////////////////////////////////////////////////////////////////////
//��ť��Ϣ���Զ������
//////////////////////////////////////////////////////////////////////////


//��굥����Ϣ
#define PNG_M_CLICKED			WM_USER + 1000

class CMyImage:public Image
{
public:
	CMyImage(UINT uID)
	{
		// ��ʼ�� Image ��Ա����
		nativeImage = NULL;
		lastResult = InvalidParameter;

		// ����ͼƬ��Դ
		HINSTANCE hInst =  GetModuleHandle(NULL);
		HRSRC hRsrc = FindResource(hInst, MAKEINTRESOURCE(uID), _T("PNG"));
		if (!hRsrc) {
			return;
		}

		// ������Դ����ȡ��ָ��
		HGLOBAL hMemRsrc = LoadResource(hInst, hRsrc);
		BYTE * pMemRsrc = (BYTE *)LockResource(hMemRsrc);

		// Ϊ������ȫ���ڴ�ռ�
		size_t sizeRsrc = SizeofResource(hInst, hRsrc);
		HGLOBAL hMemStrm = GlobalAlloc(GMEM_FIXED, sizeRsrc);
		if (!hMemStrm) {
			return;
		}

		// ����ͼƬ��Դ��ȫ���ڴ�ռ���
		BYTE * pMemStrm = (BYTE *)GlobalLock(hMemStrm);
		memcpy(pMemStrm, pMemRsrc, sizeRsrc);

		// ʹ��ͼƬ��Դ������ Image ����
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
	void SetInitPngPic(int id);//���ó�ʼpngͼƬ
	void SetClickDownPngPic(int id);//��������������ʱpngͼƬ
	void SetClickUpPngPic(int id);//�����������ɿ�ʱpngͼƬ
	void SetMouseOnPngPic(int id);//��������ڿؼ���ʱpngͼƬ
	void SetButtonPicture(LPCTSTR m_strMouseNormal ,LPCSTR m_strMouseDown = NULL,LPCSTR m_strMouseUp = NULL ,LPCSTR m_strMouseHover = NULL,LPCSTR bgImg=NULL);
	void SetText(LPCTSTR text,int TextAlign = LEFT);//���ð�ť������ַ���
	void SetFont(CFont* pFont);//��������
	int GetWidth();
	int GetHeight();
	CRect GetRect();
	void SetData(const DWORD_PTR lpData);
	DWORD_PTR GetData() const;
	void SetLeftBottomText(LPCTSTR text);//�������½��ַ���
	void SetRightBottomText(LPCTSTR text);//�������½�����
	void SetLeftBottomFont(CFont* pFont);//�������½�����
	void SetRightBottomFont(CFont* pFont);//�������½�����
	void SetCheckPicture(LPCTSTR CheckedImage);//����ѡ�к��ͼƬ
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