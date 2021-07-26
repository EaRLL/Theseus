#include "stdafx.h"
#include "TheseusCore.h"
#include "Theseus.h"
#include "CFlatButton.h"

IMPLEMENT_DYNAMIC ( CFlatButton, CButton )

BEGIN_MESSAGE_MAP ( CFlatButton, CButton )
	ON_WM_LBUTTONDOWN ( )
	ON_WM_LBUTTONDBLCLK ( )
	ON_WM_LBUTTONUP ( )
	ON_WM_SETCURSOR ( )
	ON_WM_MOUSEMOVE ( )
	ON_MESSAGE ( WM_MOUSELEAVE, OnMouseLeave )
END_MESSAGE_MAP ( )

CFlatButton::CFlatButton ( )
{
	m_bMouseInHouse = false;
	m_bClicked = false;
	Draggable = false;
	m_bMoveWindow = false;
	CheckOpt = false;
	Checked = false;
	aCpl = 0;
	aCpT = 0;
}

CFlatButton::~CFlatButton ( )
{
}

BOOL CFlatButton::OnSetCursor ( CWnd* pWnd, UINT nHitTest, UINT message )
{
	::SetCursor ( AfxGetApp ( )->LoadStandardCursor ( IDC_HAND ) );
	return TRUE;
}

void CFlatButton::OnLButtonDblClk ( UINT nFlags, CPoint point )
{
	if ( m_bMouseInHouse && !m_bClicked )
	{
		m_bClicked = true;
		RedrawWindow ( );
		Invalidate ( );
	}
	CButton::OnLButtonDblClk ( nFlags, point );
}

void CFlatButton::OnLButtonDown ( UINT nFlags, CPoint point )
{
	m_bMoveWindow = ( Draggable ) ? true : false;

	if ( m_bMoveWindow )
	{
		GetCursorPos ( &curPos );

		aCpl = curPos.x - ((CTheseusView *)GetParentFrame())->WindowLeft;
		aCpT = curPos.y - ((CTheseusView *)GetParentFrame())->WindowTop;
	}

	if ( m_bMouseInHouse && !m_bClicked )
	{
		m_bClicked = true;
		RedrawWindow ( );
		Invalidate ( );
	}
	CButton::OnLButtonDown ( nFlags, point );
}

void CFlatButton::OnLButtonUp ( UINT nFlags, CPoint point )
{
	if ( m_bMoveWindow && so.OPT_FBTN_SET_CROSS_ONMOVE )
		::SetCursor ( AfxGetApp ( )->LoadStandardCursor ( IDC_HAND ) );

	m_bMoveWindow = false;
	
	if ( m_bMouseInHouse && m_bClicked )
	{
		if (CheckOpt && !Checked)
			Checked = true;
		else
			Checked = false;
		m_bClicked = false;
		RedrawWindow ( );
		Invalidate ( );
	}

	CButton::OnLButtonUp ( nFlags, point );
}

void CFlatButton::OnMouseMove ( UINT nFlags, CPoint point )
{
	if ( m_bMoveWindow )
	{
		if ( so.OPT_FBTN_SET_CROSS_ONMOVE )
			::SetCursor ( AfxGetApp ( )->LoadStandardCursor ( IDC_SIZEALL ) );

		GetCursorPos ( &curPos );

		CTheseusView *pParentFrame = ((CTheseusView *)GetParentFrame());
		ASSERT_VALID(pParentFrame);

		pParentFrame->WindowLeft = curPos.x - aCpl;
		pParentFrame->WindowTop = curPos.y - aCpT;

		pParentFrame->SetWindowPos(NULL, pParentFrame->WindowLeft, pParentFrame->WindowTop, pParentFrame->WindowWidth, pParentFrame->WindowHeight,
			SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOACTIVATE | SWP_NOSENDCHANGING);
		pParentFrame->UpdateWindow();

	}

	if ( !m_bMouseInHouse )
	{
		m_bMouseInHouse = true;

		TRACKMOUSEEVENT tme = { 0 };
		tme.cbSize = sizeof( tme );
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_hWnd;

		TrackMouseEvent ( &tme );
		RedrawWindow ( );

		Invalidate ( );
	}

	CButton::OnMouseMove ( nFlags, point );
}

LRESULT CFlatButton::OnMouseLeave ( WPARAM wParam, LPARAM lParam )
{
	ASSERT ( m_bMouseInHouse );

	m_bMouseInHouse = false;
	m_bClicked = false;
	RedrawWindow ( );
	Invalidate ( );

	return 0;
}

void CFlatButton::DrawItem ( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
	CDC *pDC = CDC::FromHandle ( lpDrawItemStruct->hDC );
	CRect r = lpDrawItemStruct->rcItem;

	CString text;
	GetWindowText ( text );

	bool bFocus = ( lpDrawItemStruct->itemState & ODS_FOCUS ) != 0;

	if (!Checked)
	{
		if (IsMouseInHouse() == false && IsClicked() == false)
		{
			// normal
			pDC->FillSolidRect(&r, so.SKIN_COLOR_FBTN_FILL_NORMAL);
			pDC->SetTextColor(so.SKIN_COLOR_FBTN_TEXT_NORMAL);
			pDC->DrawText(text, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			pDC->FrameRect(&r, &so.SKIN_COLOR_FBTN_BORDER_NORMAL);

		}
		else if (IsMouseInHouse() && !IsClicked())
		{
			// hover
			pDC->FillSolidRect(&r, so.SKIN_COLOR_FBTN_FILL_HOVER);
			pDC->SetTextColor(so.SKIN_COLOR_FBTN_TEXT_HOVER);
			pDC->DrawText(text, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			pDC->FrameRect(&r, &so.SKIN_COLOR_FBTN_BORDER_HOVER);
		}
		else if (IsMouseInHouse() && IsClicked())
		{
			// clicked
			pDC->FillSolidRect(&r, so.SKIN_COLOR_FBTN_FILL_CDOWN);
			pDC->SetTextColor(so.SKIN_COLOR_FBTN_TEXT_CDOWN);
			pDC->DrawText(text, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			pDC->FrameRect(&r, &so.SKIN_COLOR_FBTN_BORDER_CDOWN);
		}
	}
	else
	{
		if (CheckOpt && Checked && IsMouseInHouse() == false && IsClicked() == false)
		{
			// normal
			pDC->FillSolidRect(&r, so.SKIN_COLOR_FBTN_FILL_CHCKD_NORMAL);
			pDC->SetTextColor(so.SKIN_COLOR_FBTN_TEXT_CHCKD_NORMAL);
			pDC->DrawText(text, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			pDC->FrameRect(&r, &so.SKIN_COLOR_FBTN_BORDER_CHCKD_NORMAL);
		}
		else if (CheckOpt && Checked && IsMouseInHouse() && !IsClicked())
		{
			// hover
			pDC->FillSolidRect(&r, so.SKIN_COLOR_FBTN_FILL_CHCKD_HOVER);
			pDC->SetTextColor(so.SKIN_COLOR_FBTN_TEXT_CHCKD_HOVER);
			pDC->DrawText(text, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			pDC->FrameRect(&r, &so.SKIN_COLOR_FBTN_BORDER_CHCKD_HOVER);
		}
		else if (CheckOpt && Checked && IsMouseInHouse() && IsClicked())
		{
			// clicked
			pDC->FillSolidRect(&r, so.SKIN_COLOR_FBTN_FILL_CHCKD_CDOWN);
			pDC->SetTextColor(so.SKIN_COLOR_FBTN_TEXT_CHCKD_CDOWN);
			pDC->DrawText(text, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			pDC->FrameRect(&r, &so.SKIN_COLOR_FBTN_BORDER_CHCKD_CDOWN);
		}
	}
}
