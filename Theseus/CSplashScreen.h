#pragma once

#include "TheseusCore.h"

class CSplashScreen : public CFrameWnd
{
public:
	CSplashScreen ( void );
	~CSplashScreen ( void );
	virtual BOOL PreCreateWindow ( CREATESTRUCT& cs );
	void InitaliseApp ( void );
	CFlatButton b_CloseApp;
	CStatic l_cmndShrp, l_cmndShrpVersion, l_cmndShrpLoad;
	CFont f_ButBig, f_ButSmall, f_ButWSmall;
	CBitmap Background;
	CSize bitmapSize;
	CBrush BrushHol;
	
	int TimeSec;
	int m_timer = 0;
	void OnTimer ( UINT_PTR nIDEvent );

protected:
	BOOL OnEraseBkgnd ( CDC* pDC );
	HBRUSH OnCtlColor ( CDC* pDC, CWnd* pWnd, UINT nCtlColor );
	void OnBCloseAppClick ( void );

	DECLARE_MESSAGE_MAP ( );
};

UINT _Thread_SplashScreenLoad(LPVOID param);
