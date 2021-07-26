#pragma once

class CFlatButton : public CButton
{
	DECLARE_DYNAMIC ( CFlatButton )

protected:
	void OnMouseMove ( UINT nFlags, CPoint point );
	LRESULT OnMouseLeave ( WPARAM wParam, LPARAM lParam );
	void OnLButtonDown ( UINT nFlags, CPoint point );
	void OnLButtonDblClk ( UINT nFlags, CPoint point );
	void OnLButtonUp ( UINT nFlags, CPoint point );
	BOOL OnSetCursor ( CWnd* pWnd, UINT nHitTest, UINT message );
	DECLARE_MESSAGE_MAP ( )

public:
	CFlatButton ( );
	virtual ~CFlatButton ( );
	virtual void DrawItem ( LPDRAWITEMSTRUCT );

	bool m_bMouseInHouse;
	bool m_bClicked;

	bool CheckOpt;
	bool Checked;

	bool Draggable; // drag window {
	bool m_bMoveWindow;
	CPoint curPos;
	int aCpl;
	int aCpT; // } drag window

	bool IsClicked ( ) const
	{
		return m_bClicked;
	}
	bool IsMouseInHouse ( ) const
	{
		return m_bMouseInHouse;
	}
};
