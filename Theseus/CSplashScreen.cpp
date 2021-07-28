#include "stdafx.h"
#include "Theseus.h"
#include "CSplashScreen.h"

BEGIN_MESSAGE_MAP ( CSplashScreen, CFrameWnd )
	ON_BN_CLICKED ( IDC_B_CLOSEAPP2, OnBCloseAppClick )
	ON_WM_ERASEBKGND ( )
	ON_WM_CTLCOLOR ( )
	ON_WM_TIMER ( )
END_MESSAGE_MAP ( )

CSplashScreen::CSplashScreen ( void )
{
	//HICON m_hIcon;
	//m_hIcon = AfxGetApp ( )->LoadIcon ( IDI_MAINICON );
}

CSplashScreen:: ~CSplashScreen ( void )
{
}

BOOL CSplashScreen::PreCreateWindow ( CREATESTRUCT& cs )
{
	if ( !CFrameWnd::PreCreateWindow ( cs ) )
		return FALSE;
	cs.dwExStyle &= ~( WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE | WS_CAPTION | WS_THICKFRAME | WS_SYSMENU );

	return TRUE;
}

void CSplashScreen::InitaliseApp ( void )
{
	if ( so.OPT_SPLASH_BCGR_BITMAP )
	{
		Background.LoadBitmap ( IDB_SPLASHSCR3 );
		BITMAP bm;
		Background.GetBitmap ( &bm );
		bitmapSize = CSize ( bm.bmWidth, bm.bmHeight );
	}
	xCreateFastFont(f_ButBig, 20, 400, _T ("Tahoma"));
	xCreateFastFont(f_ButSmall, 18, 900, _T("Tahoma"));
	xCreateFastFont(f_ButWSmall, 16, 400, _T("Tahoma"));

	/** 4 debug **/
	b_CloseApp.Create ( L"X", WS_CHILD | /*WS_VISIBLE*/WS_DISABLED | BS_OWNERDRAW, CRect ( 5, 5, 35, 35 ), this, IDC_B_CLOSEAPP2 );
	b_CloseApp.SetFont ( &f_ButSmall );
	/** 4 debug **/

	l_cmndShrp.Create (__string_CSplashScreen_Loading, WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, CRect ( 23, so.OPT_SPLASH_HEIGHT - 43, so.OPT_SPLASH_WIDTH - 20, so.OPT_SPLASH_HEIGHT - 23 ), this, IDC_L_SPLASHPROCESS );
	l_cmndShrp.SetFont ( &f_ButBig );
	
	l_cmndShrpVersion.Create(so.m_sVersion, WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, CRect(10, 1, so.OPT_SPLASH_WIDTH, 30), this, IDC_L_SPLASHPROCESSVERSION);
	l_cmndShrpVersion.SetFont(&f_ButWSmall);

	l_cmndShrpLoad.Create(L"", WS_CHILD | WS_VISIBLE, CRect(80, 120, 100, 120), this, IDC_L_SPLASHPROCESSLOAD);
	
	so.OPT_APP_FOLDER = xGetFolder(1);
	so.OPT_APP_FOLDER.Replace(L"\\", L"/");
	so.OPT_APP_CONFIG_FOLDER = so.OPT_APP_FOLDER + "/" + so.OPT_APP_CONFIG_FOLDER;
	so.OPT_APP_CONFIG_FILE = so.OPT_APP_CONFIG_FOLDER + so.OPT_APP_CONFIG_FILE;

	std::wifstream optFile ( so.OPT_APP_CONFIG_FILE );

	l_cmndShrp.SetWindowTextW (__string_CSplashScreen_Parsing);
	RedrawWindow ( );

	if ( optFile.is_open ( ) && !optFile.fail ( ) )
	{
		TimeSec = xGetUnixTime();
		CWinThread *pThread = AfxBeginThread(_Thread_SplashScreenLoad, GetSafeHwnd());
		
		const std::locale empty_locale = std::locale::empty ( );
		typedef std::codecvt_utf8<wchar_t> converter_type;
		const converter_type* converter = new converter_type;
		const std::locale utf8_locale = std::locale ( empty_locale, converter );
		optFile.imbue ( utf8_locale );
		so.F_OPT_LINES = _xInitalizeFile ( optFile );
		_xParseCfg ( so.F_OPT_LINES );

		optFile.close ( );

		l_cmndShrp.SetWindowTextW (__string_CSplashScreen_Initalize);
		RedrawWindow ( );

		SetTimer ( 1, 1050, NULL );
	}
	else
	{
		l_cmndShrp.SetWindowText(__string_CSplashScreen_CfgNotFound);
		RedrawWindow ( );

		SetTimer ( 1, 1000, NULL );
	}
}

UINT _Thread_SplashScreenLoad(LPVOID param)
{
	while (so.timerClocks)
	{
		theApp.pFrameSplash->l_cmndShrpLoad.SetWindowPos(NULL, 60, 153, (300 + ((xGetUnixTime() - theApp.pFrameSplash->TimeSec)*5)), 6, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOACTIVATE | SWP_NOSENDCHANGING);
		theApp.pFrameSplash->l_cmndShrpLoad.UpdateWindow();
		theApp.pFrameSplash->UpdateWindow();
		if (!so.timerClocks)
			break;
	}

	AfxEndThread(0);
	return 0;
}

void CSplashScreen::OnTimer ( UINT_PTR nIDEvent )
{
	so.timerClocks = false;
	OnBCloseAppClick ( );
}

BOOL CSplashScreen::OnEraseBkgnd ( CDC* pDC )
{
	if ( so.OPT_SPLASH_BCGR_BITMAP )
	{
		CDC dcMemory;
		dcMemory.CreateCompatibleDC ( pDC );
		CBitmap* pOldbitmap = dcMemory.SelectObject ( &Background );
		CRect rcClient;
		GetClientRect ( &rcClient );
		const CSize& sbitmap = bitmapSize;
		pDC->BitBlt ( 0, 0, sbitmap.cx, sbitmap.cy, &dcMemory, 0, 0, SRCCOPY );
		dcMemory.SelectObject ( pOldbitmap );
	}
	else
	{
		CBrush backBrush ( so.SKIN_COLOR_SPLASH_BG );
		CBrush* pOldBrush = pDC->SelectObject ( &backBrush );
		CRect rect;
		GetClientRect ( &rect );
		pDC->PatBlt ( rect.left, rect.top, rect.Width ( ), rect.Height ( ), PATCOPY );
		pDC->SelectObject ( pOldBrush );
	}

	return TRUE;
}

HBRUSH CSplashScreen::OnCtlColor ( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
	UINT id = pWnd->GetDlgCtrlID ( );

	if ( id == IDC_L_SPLASHPROCESS || id == IDC_L_SPLASHPROCESSVERSION)
	{
		pDC->SetTextColor ( so.SKIN_COLOR_SPLASH_TEXT );
		pDC->SetBkMode ( TRANSPARENT );
		return ( HBRUSH )::GetStockObject ( NULL_BRUSH );
	}
	else if (id == IDC_L_SPLASHPROCESSLOAD)
	{
		pDC->SetTextColor(so.SKIN_COLOR_SPLASH_TEXT);
		pDC->SetBkMode(OPAQUE);
		return (HBRUSH)::CreateSolidBrush(RGB(131, 176, 229));
	}
	else
		return ( HBRUSH )::GetStockObject ( NULL_BRUSH );
}

void CSplashScreen::OnBCloseAppClick ( void )
{
	if (so.OPT_APP_SHOWPOS == L"top-left")
		theApp.pMainFrame->SetWindowPos(NULL, 0, 0, theApp.pMainFrame->WindowWidth, theApp.pMainFrame->WindowHeight,
			SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOACTIVATE | SWP_NOSENDCHANGING);
	else if (so.OPT_APP_SHOWPOS == L"top-right")
	{
		CRect rcMonitor = xGetMonitorWorkArea(theApp.pMainFrame);
		theApp.pMainFrame->SetWindowPos(NULL, rcMonitor.right - theApp.pMainFrame->WindowWidth, 0, theApp.pMainFrame->WindowWidth, theApp.pMainFrame->WindowHeight,
			SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOACTIVATE | SWP_NOSENDCHANGING);
	}
	else if (so.OPT_APP_SHOWPOS == L"top-center")
	{
		CRect rcMonitor = xGetMonitorWorkArea(theApp.pMainFrame);
		theApp.pMainFrame->SetWindowPos(NULL, (int)round((rcMonitor.right - theApp.pMainFrame->WindowWidth) / 2), 0, theApp.pMainFrame->WindowWidth, theApp.pMainFrame->WindowHeight,
			SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOACTIVATE | SWP_NOSENDCHANGING);
	}
	else if (so.OPT_APP_SHOWPOS == L"bottom-left")
	{
		CRect rcMonitor = xGetMonitorWorkArea(theApp.pMainFrame);
		theApp.pMainFrame->SetWindowPos(NULL, 0, rcMonitor.bottom - theApp.pMainFrame->WindowHeight, theApp.pMainFrame->WindowWidth, theApp.pMainFrame->WindowHeight,
			SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOACTIVATE | SWP_NOSENDCHANGING);
	}
	else if (so.OPT_APP_SHOWPOS == L"bottom-center")
	{
		CRect rcMonitor = xGetMonitorWorkArea(theApp.pMainFrame);
		theApp.pMainFrame->SetWindowPos(NULL, (int)round((rcMonitor.right - theApp.pMainFrame->WindowWidth) / 2), rcMonitor.bottom - theApp.pMainFrame->WindowHeight, theApp.pMainFrame->WindowWidth, theApp.pMainFrame->WindowHeight,
			SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOACTIVATE | SWP_NOSENDCHANGING);
	}
	else if (so.OPT_APP_SHOWPOS == L"bottom-right")
	{
		CRect rcMonitor = xGetMonitorWorkArea(theApp.pMainFrame);
		theApp.pMainFrame->SetWindowPos(NULL, rcMonitor.right - theApp.pMainFrame->WindowWidth, rcMonitor.bottom - theApp.pMainFrame->WindowHeight, theApp.pMainFrame->WindowWidth, theApp.pMainFrame->WindowHeight,
			SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOACTIVATE | SWP_NOSENDCHANGING);
	}
	else
		theApp.pMainFrame->SetWindowPos(NULL, theApp.pMainFrame->WindowLeft, theApp.pMainFrame->WindowTop, theApp.pMainFrame->WindowWidth, theApp.pMainFrame->WindowHeight,
			SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOACTIVATE | SWP_NOSENDCHANGING);
	
	theApp.pMainFrame->CreateChildControls ( );
	theApp.pMainFrame->ShowWindow ( SW_SHOW );
	theApp.pMainFrame->UpdateWindow ( );

	BOOL rc = DestroyWindow ( );
	return afx_msg void ( );
}
