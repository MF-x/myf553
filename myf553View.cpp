// myf553View.cpp : implementation of the CMyf553View class
//

#include "stdafx.h"
#include "myf553.h"

#include "myf553Doc.h"
#include "myf553View.h"

#include "Bmp.h"
#include "DlgHistogram.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyf553View

IMPLEMENT_DYNCREATE(CMyf553View, CScrollView)

BEGIN_MESSAGE_MAP(CMyf553View, CScrollView)
	//{{AFX_MSG_MAP(CMyf553View)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_GRAY, OnGray)
	ON_UPDATE_COMMAND_UI(ID_GRAY, OnUpdateGray)
	ON_COMMAND(ID_HISTOGRAM, OnHistogram)
	ON_UPDATE_COMMAND_UI(ID_HISTOGRAM, OnUpdateHistogram)
	ON_COMMAND(ID_HISTOGRAM_SUPER, OnHistogramSuper)
	ON_UPDATE_COMMAND_UI(ID_HISTOGRAM_SUPER, OnUpdateHistogramSuper)
	ON_COMMAND(ID_LINEARTRANS, OnLineartrans)
	ON_UPDATE_COMMAND_UI(ID_LINEARTRANS, OnUpdateLineartrans)
	ON_COMMAND(ID_EQUALIZE, OnEqualize)
	ON_UPDATE_COMMAND_UI(ID_EQUALIZE, OnUpdateEqualize)
	ON_COMMAND(ID_FOURIER, OnFourier)
	ON_UPDATE_COMMAND_UI(ID_FOURIER, OnUpdateFourier)
	ON_COMMAND(ID_INVERT_FOURIER, OnInvertFourier)
	ON_UPDATE_COMMAND_UI(ID_INVERT_FOURIER, OnUpdateInvertFourier)
	ON_COMMAND(ID_FAST_FOURIER, OnFastFourier)
	ON_UPDATE_COMMAND_UI(ID_FAST_FOURIER, OnUpdateFastFourier)
	ON_COMMAND(ID_INVERT_FAST_FOURIER, OnInvertFastFourier)
	ON_UPDATE_COMMAND_UI(ID_INVERT_FAST_FOURIER, OnUpdateInvertFastFourier)
	ON_COMMAND(ID_AVERAGE_FILTER, OnAverageFilter)
	ON_UPDATE_COMMAND_UI(ID_AVERAGE_FILTER, OnUpdateAverageFilter)
	ON_COMMAND(ID_MEDIAN_FILTER, OnMedianFilter)
	ON_UPDATE_COMMAND_UI(ID_MEDIAN_FILTER, OnUpdateMedianFilter)
	ON_COMMAND(ID_LAPLACE_SHARP, OnLaplaceSharp)
	ON_UPDATE_COMMAND_UI(ID_LAPLACE_SHARP, OnUpdateLaplaceSharp)
	ON_COMMAND(ID_GRADIENT_SHARPEN, OnGradientSharpen)
	ON_UPDATE_COMMAND_UI(ID_GRADIENT_SHARPEN, OnUpdateGradientSharpen)
	ON_COMMAND(ID_ILPF, OnIlpf)
	ON_UPDATE_COMMAND_UI(ID_ILPF, OnUpdateIlpf)
	ON_COMMAND(ID_IHPF, OnIhpf)
	ON_UPDATE_COMMAND_UI(ID_IHPF, OnUpdateIhpf)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyf553View construction/destruction

CMyf553View::CMyf553View()
{
	// TODO: add construction code here

}

CMyf553View::~CMyf553View()
{
}

BOOL CMyf553View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMyf553View drawing
void CMyf553View::OnDraw(CDC* pDC)
{
	CMyf553Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	if(lpBitsInfo != NULL) {
		LPVOID lpBits = (LPVOID)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
		StretchDIBits(
			pDC->GetSafeHdc(),
			0, 0,
			lpBitsInfo->bmiHeader.biWidth,
			lpBitsInfo->bmiHeader.biHeight,
			0, 0,
			lpBitsInfo->bmiHeader.biWidth,
			lpBitsInfo->bmiHeader.biHeight,
			lpBits,
			lpBitsInfo,
			DIB_RGB_COLORS,
			SRCCOPY
		);
	}

	if(lpBitsInfoFT != NULL) {
		LPVOID lpBitsFT = (LPVOID)&lpBitsInfoFT->bmiColors[lpBitsInfoFT->bmiHeader.biClrUsed];
		StretchDIBits(
			pDC->GetSafeHdc(),
			lpBitsInfo->bmiHeader.biWidth + 30, 0,
			lpBitsInfoFT->bmiHeader.biWidth,
			lpBitsInfoFT->bmiHeader.biHeight,
			0, 0,
			lpBitsInfoFT->bmiHeader.biWidth,
			lpBitsInfoFT->bmiHeader.biHeight,
			lpBitsFT,
			lpBitsInfoFT,
			DIB_RGB_COLORS,
			SRCCOPY
		);
	}

	if(lpBitsInfoFFT != NULL) {
		LPVOID lpBitsFFT = (LPVOID)&lpBitsInfoFFT->bmiColors[lpBitsInfoFFT->bmiHeader.biClrUsed];
		StretchDIBits(
			pDC->GetSafeHdc(),
			lpBitsInfo->bmiHeader.biWidth + 30, 0,
			lpBitsInfoFFT->bmiHeader.biWidth,
			lpBitsInfoFFT->bmiHeader.biHeight,
			0, 0,
			lpBitsInfoFFT->bmiHeader.biWidth,
			lpBitsInfoFFT->bmiHeader.biHeight,
			lpBitsFFT,
			lpBitsInfoFFT,
			DIB_RGB_COLORS,
			SRCCOPY
		);
	}

	if(showHistogram){
		RECT r;
		r.left = 30;
		r.top = lpBitsInfo->bmiHeader.biHeight + 30;
		r.right = r.left + 256 + 2;
		r.bottom = r.top + 130;
		LPCRECT rectangle= &r;
		pDC->Rectangle(rectangle);

		int i;
		int max = 0;
		for(i = 0; i < 256; i++) {
			if(H[i] > max) {
				max = H[i];
			}
		}

		for(i = 0; i < 256; i++) {
			POINT b = {r.left + i + 1, r.bottom - 1};
			POINT e = {r.left + i + 1, r.bottom - H[i] * (r.bottom - r.top - 20) / max - 1};
			pDC->MoveTo(b);
			pDC->LineTo(e);
		}
	}

}

void CMyf553View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CMyf553View printing

BOOL CMyf553View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMyf553View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMyf553View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMyf553View diagnostics

#ifdef _DEBUG
void CMyf553View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMyf553View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CMyf553Doc* CMyf553View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyf553Doc)));
	return (CMyf553Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyf553View message handlers

void CMyf553View::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(lpBitsInfo != NULL && point.y < lpBitsInfo->bmiHeader.biHeight && point.x < lpBitsInfo->bmiHeader.biWidth) {
		char str[50];
		pixel(point.y, point.x, str);
		AfxMessageBox(str);
	}
	CScrollView::OnLButtonDown(nFlags, point);
}

void CMyf553View::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	char rgb[64] = {0, };
	if(lpBitsInfo != NULL && point.y < lpBitsInfo->bmiHeader.biHeight && point.x < lpBitsInfo->bmiHeader.biWidth) {
		pixel(point.y, point.x, rgb);
	}
	
	char str[64] = {0, };
	sprintf(str, "  x:%4d  y:%4d    %s", point.x, point.y, rgb);

	((CFrameWnd *)GetParent())->SetMessageText(str);


	CScrollView::OnMouseMove(nFlags, point);
}

void CMyf553View::OnGray() 
{
	// TODO: Add your command handler code here
	gray();
	Invalidate();
}

void CMyf553View::OnUpdateGray(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo != NULL && lpBitsInfo->bmiHeader.biBitCount == 24);
}

void CMyf553View::OnHistogram() 
{
	// TODO: Add your command handler code here
	histogram();
	CDlgHistogram dlg;
	dlg.DoModal();
}

void CMyf553View::OnUpdateHistogram(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(isGray == TRUE);
}

void CMyf553View::OnHistogramSuper() 
{
	// TODO: Add your command handler code here
	histogram();
	showHistogram = !showHistogram;
	Invalidate();
}

void CMyf553View::OnUpdateHistogramSuper(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(isGray == TRUE);
}

void CMyf553View::OnLineartrans() 
{
	// TODO: Add your command handler code here
	linearTrans(1.1,-10);
	Invalidate();
}

void CMyf553View::OnUpdateLineartrans(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(isGray == TRUE);
}

void CMyf553View::OnEqualize() 
{
	// TODO: Add your command handler code here
	equalize();
	Invalidate();
}

void CMyf553View::OnUpdateEqualize(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(isGray == TRUE);
}


void CMyf553View::OnFourier() 
{
	// TODO: Add your command handler code here
	fourier();
	Invalidate();
}

void CMyf553View::OnUpdateFourier(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(isGray == TRUE);
}

void CMyf553View::OnInvertFourier() 
{
	// TODO: Add your command handler code here
	invertFourier();
	Invalidate();
}

void CMyf553View::OnUpdateInvertFourier(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(isGray == TRUE && lpBitsInfoFT != NULL);
}

void CMyf553View::OnFastFourier() 
{
	// TODO: Add your command handler code here
	fastFourier();
	Invalidate();
}

void CMyf553View::OnUpdateFastFourier(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(isGray == TRUE);
}

void CMyf553View::OnInvertFastFourier() 
{
	// TODO: Add your command handler code here
	invertFastFourier();
	Invalidate();
}

void CMyf553View::OnUpdateInvertFastFourier(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(isGray == TRUE && lpBitsInfoFFT != NULL);
}


void CMyf553View::OnAverageFilter() 
{
	// TODO: Add your command handler code here
	averageFilter();
	Invalidate();
}

void CMyf553View::OnUpdateAverageFilter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(isGray == TRUE);
}

void CMyf553View::OnMedianFilter() 
{
	// TODO: Add your command handler code here
	medianFilter();
	Invalidate();
}

void CMyf553View::OnUpdateMedianFilter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(isGray == TRUE);
}

void CMyf553View::OnGradientSharpen() 
{
	// TODO: Add your command handler code here
	gradientSharpen();
	Invalidate();
}

void CMyf553View::OnUpdateGradientSharpen(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(isGray == TRUE);
}

void CMyf553View::OnLaplaceSharp() 
{
	// TODO: Add your command handler code here
	laplaceSharpen();
	Invalidate();
}

void CMyf553View::OnUpdateLaplaceSharp(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(isGray == TRUE);
}



void CMyf553View::OnIlpf() 
{
	// TODO: Add your command handler code here
	frequencyDomainFilter(70);
	Invalidate();
}

void CMyf553View::OnUpdateIlpf(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(isGray == TRUE && gFD != NULL);
}

void CMyf553View::OnIhpf() 
{
	// TODO: Add your command handler code here
	frequencyDomainFilter(-30);
	Invalidate();
}

void CMyf553View::OnUpdateIhpf(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(isGray == TRUE && gFD != NULL);
}
