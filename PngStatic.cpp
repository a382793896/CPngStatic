#include "stdafx.h"
#include "PngStatic.h"
#include "..\LCPublicServers.h"
#define  ReleaseRes(lpRes) if(lpRes){delete lpRes; lpRes = NULL;}

CPngStatic::CPngStatic()
{
	isGetBj = FALSE;
	isSetClickDownImage = FALSE;
	isSetClickUpImage = FALSE;
	isSetMouseOnImage = FALSE;
	isSetInitImage = FALSE;
	isMouseOnPic  =FALSE;
	ClickDownImage = NULL;
	ClickUpImage = NULL;
	MouseOnImage = NULL;
	m_BgImage = NULL;
	InitImage = NULL;
	m_CheckedImage = NULL;
	m_image = NULL;
	m_bCheck = FALSE;
	parentCDC = NULL;

	isSetCheckedImage = FALSE;
	m_text.Empty();
	m_leftBottomText.Empty();
	m_rightBottomText.Empty();
	m_font = LCGetPmsFont(FONT_WARING);
	m_leftFont = LCGetPmsFont(FONT_TICKET_PRICE);
	m_rightFont = LCGetPmsFont(FONT_TICKET_PRICE);
}
CPngStatic::~CPngStatic()
{
	ReleaseRes(InitImage);
	ReleaseRes(MouseOnImage);
	ReleaseRes(ClickUpImage);
	ReleaseRes(ClickDownImage);
	ReleaseRes(m_CheckedImage);
	
	ReleaseRes(parentCDC);

}

void CPngStatic::SetClickDownPngPic(int id)
{
	ReleaseRes(ClickDownImage);
	ClickDownImage = new CMyImage(id);
	isSetClickDownImage = TRUE;
}
void CPngStatic::SetClickUpPngPic(int id)
{
	ReleaseRes(ClickUpImage);
	ClickUpImage = new CMyImage(id);
	isSetClickUpImage = TRUE;

}
void CPngStatic::SetMouseOnPngPic(int id)
{
	ReleaseRes(MouseOnImage);
	MouseOnImage = new CMyImage(id);
	isSetMouseOnImage = TRUE;
}
void CPngStatic::SetInitPngPic(int id)
{
	ModifyStyle(0,SS_NOTIFY);

	this->GetParent()->GetWindowRect(&rp);
	this->GetWindowRect(&lp);
	ReleaseRes(InitImage);
	InitImage = new CMyImage(id);
	
	isSetInitImage = TRUE;
	
	SetBitmap(InitImage);
	
}

void CPngStatic::SetCheckPicture(LPCTSTR CheckedImagePath)
{
	ReleaseRes(m_CheckedImage);
	m_bCheck = FALSE;
	if( CheckedImagePath != NULL && CheckedImagePath != "" )
	{
		isSetCheckedImage = LoadPicture(CheckedImagePath,&m_CheckedImage);
	}

}
void CPngStatic::RefreshUi()
{
	if(InitImage)
		SetBitmap(InitImage);
	return ;
}
void CPngStatic::SetCheck(BOOL bCheck)
{
	m_bCheck = bCheck;
}
void CPngStatic::SetButtonPicture(LPCTSTR m_strMouseNormal,LPCSTR m_strMouseDown,LPCSTR m_strMouseUp,LPCSTR m_strMouseHover,LPCSTR bgImg)
{

	ReleaseRes(InitImage);
	ReleaseRes(MouseOnImage);
	ReleaseRes(ClickUpImage);
	ReleaseRes(ClickDownImage);

	isSetClickDownImage = FALSE;
	isSetClickUpImage = FALSE;
	isSetMouseOnImage = FALSE;
	isSetInitImage = FALSE;
	isMouseOnPic  =FALSE;

	ModifyStyle(0,SS_NOTIFY);

	this->GetParent()->GetWindowRect(&rp);
	this->GetWindowRect(&lp);
	//InitImage = new CMyImage(id);
	if( m_strMouseNormal != NULL && m_strMouseNormal != "" )
	{
		isSetInitImage = LoadPicture(m_strMouseNormal,&InitImage);
		
	}
	if( m_strMouseHover != NULL && m_strMouseHover != "" )
	{
		isSetMouseOnImage = LoadPicture(m_strMouseHover,&MouseOnImage);
		
	}
	if( m_strMouseDown != NULL && m_strMouseDown != "" )
	{
		isSetClickDownImage = LoadPicture(m_strMouseDown,&ClickDownImage);
		
	}
	if( m_strMouseUp != NULL && m_strMouseUp != "" )
	{
		isSetClickUpImage = LoadPicture(m_strMouseUp,&ClickUpImage);
		
	}

	if( bgImg != NULL && bgImg != "" )
	{
		LoadPicture(bgImg,&m_BgImage);
	}
	
	if(isSetInitImage)
		SetBitmap(InitImage);

}
BOOL CPngStatic::LoadPicture(LPCTSTR szFileName,Image **m_pPicture/*,CImage *m_pImage*/)
{
	CString strfilename;

	strfilename = GetFullFilePath(szFileName);
	char * buf = strfilename.GetBuffer();
	int len = static_cast<int>(strlen(buf));

	int wlen = MultiByteToWideChar(CP_ACP,0,(LPCSTR)buf,-1,NULL,0);
	if( wlen > 0 )
	{
		WCHAR * wbuf = new WCHAR[wlen * sizeof(WCHAR)];
		MultiByteToWideChar(CP_ACP,0,(LPCSTR)buf,-1,wbuf,wlen);
		if((*m_pPicture) != NULL)
		{
			delete (*m_pPicture);
			(*m_pPicture) = NULL;
		}
		(*m_pPicture) = Image::FromFile(wbuf);
		delete[] wbuf;
		if( (*m_pPicture) == NULL )
			return 0;
	}
	else
	{
		return 0;
	}
	return 1;
}
//设置图片资源根路径、
CString CPngStatic::GetFullFilePath(CString Ini_FileName)
{
	int filelen,file_index;
	char FileName[MAX_PATH];
	CString FileNameS,strtemp;
	filelen = ::GetModuleFileName(NULL,FileName,MAX_PATH);
	if(filelen <= 0 )
	{
		strtemp="";
		return strtemp;
	}
	for(int i = 0;i < filelen; i ++)
	{
		strtemp=(CString)FileName[i];
		FileNameS=FileNameS+strtemp;
	}
	file_index=FileNameS.ReverseFind('\\');
	FileNameS=FileNameS.Left(file_index);
	FileNameS += "\\";
	FileNameS+= "IconLib\\" + Ini_FileName;

	return FileNameS;
}
void CPngStatic::OnPaint()
{
	
	if(isGetBj)
	{
		if(m_image != NULL)
		{
			//CDC* bgDC = NULL;
			CDC* pFgndMemDC = NULL;
			pFgndMemDC = CreateMixDC(parentCDC,m_image);
			
			AlphaGradientDraw(this,parentCDC,pFgndMemDC,CPoint(0,0),m_bjRect.right,m_bjRect.bottom,AC_SRC_ALPHA);	
			ReleaseRes(pFgndMemDC);
			DrawText();
		}
	   
		CStatic::OnPaint();
	}
	else
	{
		SetBj();
	}
}
void CPngStatic::DrawText()
{
	if(m_text.IsEmpty())
	{
		return ;
	}
	CDC * pDc = GetDC();

	CFont* hOldFont;
	
	//照原样先拷贝一份,用双缓冲，防止闪烁
	CDC* pMemDCDst = new CDC;
	pMemDCDst->CreateCompatibleDC(pDc);
	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDc, m_image->GetWidth(), m_image->GetHeight());
	pMemDCDst->SelectObject(&memBitmap);
	pMemDCDst->BitBlt(0, 0, m_image->GetWidth(),  m_image->GetHeight(), pDc, 0, 0, SRCCOPY);

	if(m_font != NULL)
		hOldFont = pMemDCDst->SelectObject(m_font);
	pMemDCDst->SetTextColor( RGB(0,0,0));
	CSize szExtent = pMemDCDst->GetTextExtent(m_text, lstrlen(m_text));

	CPoint pt,LeftPt,rightPt;
	if(m_TextAlign == LEFT)
	{
		CPoint pt1( m_bjRect.left, m_bjRect.top + (m_bjRect.Height() - szExtent.cy) / 2);
		pt = pt1;
	}
	else if(m_TextAlign == MIDDLE)
	{
		CPoint pt1( m_bjRect.left + (m_bjRect.Width() - szExtent.cx) / 2, m_bjRect.top + (m_bjRect.Height() - szExtent.cy) / 2);
		pt = pt1;
	}
	else if(m_TextAlign == RIGHT)
	{
		CPoint pt1( m_bjRect.left + (m_bjRect.Width() - szExtent.cx), m_bjRect.top + (m_bjRect.Height() - szExtent.cy) / 2);
		pt = pt1;
	}
	else
	{
		CPoint pt1( m_bjRect.left, m_bjRect.top + (m_bjRect.Height() - szExtent.cy) / 2);
		pt = pt1;
	}
	
	
	pt.x += 0;
	int nMode = pMemDCDst->SetBkMode(TRANSPARENT);
	pMemDCDst->DrawState(pt, szExtent, m_text, DSS_NORMAL, TRUE, 0, (HBRUSH)NULL);
	if(m_font != NULL)
		pMemDCDst->SelectObject(hOldFont);
	if(!m_leftBottomText.IsEmpty())
	{
		if(m_leftFont != NULL)
			hOldFont =  pMemDCDst->SelectObject(m_leftFont);
		pMemDCDst->SetTextColor(RGB(255,255,255));
		CSize szLeftExtent = pMemDCDst->GetTextExtent(m_leftBottomText, m_leftBottomText.GetLength());
		if(!m_leftBottomText.IsEmpty())
		{

			LeftPt.x = m_bjRect.left;
			LeftPt.y = m_bjRect.bottom - szLeftExtent.cy - 2;
		}
		pMemDCDst->DrawState(LeftPt, szLeftExtent, m_leftBottomText, DSS_NORMAL, TRUE, 0, (HBRUSH)NULL);
		if(m_leftFont != NULL )
			pMemDCDst->SelectObject(hOldFont);
		
	}
	if(!m_rightBottomText.IsEmpty())
	{
		if(m_rightFont != NULL)
			hOldFont =  pMemDCDst->SelectObject(m_leftFont);
		pMemDCDst->SetTextColor(RGB(255,255,255));
		CSize szRightExtent = pMemDCDst->GetTextExtent(m_rightBottomText, m_rightBottomText.GetLength());
		if(!m_rightBottomText.IsEmpty())
		{
			CPoint pt2( m_bjRect.left + (m_bjRect.Width() - szRightExtent.cx-2), m_bjRect.bottom - szRightExtent.cy-2 );
			rightPt = pt2;
		}
		pMemDCDst->DrawState(rightPt, szRightExtent, m_rightBottomText, DSS_NORMAL, TRUE, 0, (HBRUSH)NULL);
		if(m_rightFont != NULL )
			pMemDCDst->SelectObject(hOldFont);
	}
	pMemDCDst->SetBkMode(nMode);
	nMode = pDc->SetBkMode(TRANSPARENT);
	pDc->BitBlt(0,0,m_image->GetWidth(),m_image->GetHeight(),pMemDCDst,0,0,SRCCOPY);
	pDc->SetBkMode(nMode);
	pMemDCDst->DeleteDC();
	delete pMemDCDst;
	pMemDCDst = NULL;
	ReleaseDC(pDc);
}

void CPngStatic::SetText(LPCTSTR text,int TextAlign)
{
	m_text = text;
	m_TextAlign = TextAlign;
	return ;
}
void CPngStatic::SetLeftBottomText(LPCTSTR text)
{
	m_leftBottomText = text;
	return ;
}
void CPngStatic::SetRightBottomText(LPCTSTR text)
{
	m_rightBottomText = text;
	return ;
}
void CPngStatic::SetFont(CFont* pFont)
{
	m_font = pFont;
	return ;
}
void CPngStatic::SetLeftBottomFont(CFont* pFont)
{
	m_leftFont = pFont;
	return ;
}
void CPngStatic::SetRightBottomFont(CFont* pFont)
{
	m_leftFont = pFont;
	return ;
}
CDC* CPngStatic::GetParentBkgnd(CWnd *pWnd, CPoint ptDrawStart, int nWidth,int nHeight)
{
	//CDC* pDC = pWnd->GetDC();
	CWnd* pWndParent = pWnd->GetParent();
	CDC* pParentDC = pWndParent->GetDC();

	//得到图像绘制开始点在父窗口上的坐标
	pWnd->ClientToScreen(&ptDrawStart);
	pWndParent->ScreenToClient(&ptDrawStart);

	//初始化内存DC
	CDC* pMemDC = new CDC;
	pMemDC->CreateCompatibleDC(pParentDC);
	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pParentDC, nWidth, nHeight);
	pMemDC->SelectObject(&memBitmap);

	//拷贝父窗口背景到内存DC
	pMemDC->BitBlt(0, 0, nWidth, nHeight, pParentDC, ptDrawStart.x, 
		ptDrawStart.y, SRCCOPY);

	//释放	
	pWndParent->ReleaseDC(pParentDC);
	//pWnd->ReleaseDC(pDC);

	return pMemDC;
}

//提取图片所在的背景
CDC* CPngStatic::CreateBgDC()
{

	int nWidth = GetSystemMetrics(SM_CXSCREEN);
	int nHeight = GetSystemMetrics(SM_CYSCREEN);
	
	//照原样先拷贝一份
	CDC* pMemDCDst = new CDC;
	CDC *pDc = this->GetDC();
	pMemDCDst->CreateCompatibleDC(pDc);
	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDc, nWidth, nHeight);
	pMemDCDst->SelectObject(&memBitmap);

	//画上图像
	Graphics memGraph(pMemDCDst->m_hDC);
	memGraph.DrawImage(m_BgImage, 0, 0, nWidth, nHeight);
	memGraph.ReleaseHDC(pMemDCDst->m_hDC);

	pDc->DeleteDC();

	return pMemDCDst;

}

CDC* CPngStatic::CreateMixDC(CDC* pParentMemDC, Image* image)
{
	int nWidth = image->GetWidth(),
		nHeight =image->GetHeight();

	//照原样先拷贝一份
	CDC* pMemDCDst = new CDC;
	pMemDCDst->CreateCompatibleDC(pParentMemDC);
	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pParentMemDC, nWidth, nHeight);
	pMemDCDst->SelectObject(&memBitmap);

	CRect tempRect;
	this->GetWindowRect(&tempRect);
	pMemDCDst->BitBlt(0, 0, nWidth, nHeight, pParentMemDC, tempRect.left, tempRect.top, SRCCOPY);

	//画上图像

	Graphics memGraph(pMemDCDst->m_hDC);
	memGraph.DrawImage(image, 0, 0, nWidth, nHeight);
	memGraph.ReleaseHDC(pMemDCDst->m_hDC);

	return pMemDCDst;
}

void CPngStatic::AlphaGradientDraw(CWnd* pWnd, CDC* pFgndMemDC, CDC* pBkgndMemDC,
	CPoint ptDrawStart, int nWidth, int nHeight, int nTransparence)
{
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = 0;
	bf.SourceConstantAlpha = nTransparence;

	CDC* pDC = pWnd->GetDC();
	//创建内存DC
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC, nWidth, nHeight);
	memDC.SelectObject(&memBitmap);
	//在内存中先画好
	//memDC.BitBlt(0, 0, nWidth, nHeight, pDC, 0, 0, SRCCOPY);
	memDC.BitBlt(0, 0, nWidth, nHeight, pBkgndMemDC, 0, 0, SRCCOPY);
	AlphaBlend(memDC.m_hDC, 0, 0, nWidth, nHeight, pFgndMemDC->m_hDC,0, 0, nWidth, nHeight, bf);
	
	//直接输出
	
	pDC->BitBlt(ptDrawStart.x, ptDrawStart.y, nWidth, nHeight, &memDC, 
		0, 0, SRCCOPY);
	
	pWnd->ReleaseDC(pDC);


}

BEGIN_MESSAGE_MAP(CPngStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


void CPngStatic::SetBj()
{
	if(!isGetBj)
	{
		if(m_BgImage != NULL)
			parentCDC = CreateBgDC();
		else
			parentCDC = GetParentBkgnd(this,CPoint(0,0),m_bjRect.right,m_bjRect.bottom);
		isGetBj = TRUE;	
	}
}

void CPngStatic::SetBitmap(Image *image)
{
	if(!isSetClickDownImage&&!isSetClickUpImage&&!isSetMouseOnImage&&!isSetInitImage)
	    delete(m_image);
	if(m_bCheck && isSetCheckedImage && m_CheckedImage != NULL)
	{
		m_image = m_CheckedImage;
	}else
	{
		m_image = image;
	}
	this->GetParent()->GetWindowRect(&rp);
	this->GetWindowRect(&lp);
	m_rect.left = lp.left - rp.left;
	m_rect.top  = lp.top - rp.top;
	m_rect.right = m_rect.left+image->GetWidth();
	m_rect.bottom = m_rect.top+image->GetHeight();
	this->MoveWindow(&m_rect);
	m_bjRect.top =0;
	m_bjRect.left=0;
	m_bjRect.right=image->GetWidth();
	m_bjRect.bottom=image->GetHeight();
	if(isGetBj)
	   OnPaint();
}


BOOL CPngStatic::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(!isMouseOnPic)
	{
		isMouseOnPic = TRUE;
	  if(isSetMouseOnImage)
			 SetBitmap(MouseOnImage);
	}
	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}


void CPngStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(isSetClickDownImage)
	SetBitmap(ClickDownImage);
	CStatic::OnLButtonDown(nFlags, point);
}


void CPngStatic::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bCheck = TRUE;
	if(isSetClickUpImage)
	SetBitmap(ClickUpImage);
	if(m_bCheck && isSetCheckedImage && m_CheckedImage != NULL )
	{
		SetBitmap(m_CheckedImage);
	}
	GetParent()->SendMessage(PNG_M_CLICKED,GetDlgCtrlID(),0);
	CStatic::OnLButtonUp(nFlags, point);
}


void CPngStatic::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	isMouseOnPic = FALSE;
	if(isSetInitImage)
	 SetBitmap(InitImage);
	CStatic::OnMouseLeave();
}

 
void CPngStatic::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TRACKMOUSEEVENT   *stTRACKMOUSEEVENT = new TRACKMOUSEEVENT; 
	stTRACKMOUSEEVENT->cbSize   =   sizeof(TRACKMOUSEEVENT); 
	stTRACKMOUSEEVENT->hwndTrack=m_hWnd; 
	stTRACKMOUSEEVENT->dwFlags=TME_LEAVE; 
	_TrackMouseEvent(stTRACKMOUSEEVENT); 
	ReleaseRes(stTRACKMOUSEEVENT);
	CStatic::OnMouseMove(nFlags, point);
}
int CPngStatic::GetWidth()
{
	return m_bjRect.Width();
}

int CPngStatic::GetHeight()
{
	return m_bjRect.Height();
}


CRect CPngStatic::GetRect()
{
	return m_rect;
}

void CPngStatic::SetData(const DWORD_PTR lpData)
{
	m_data = lpData;
	return ;
}

DWORD_PTR CPngStatic::GetData() const
{
	return m_data;
}
