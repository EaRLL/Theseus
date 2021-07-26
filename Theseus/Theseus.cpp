#include "stdafx.h"
#include "Theseus.h"

#pragma comment( user, "Compiled on " __DATE__ " at " __TIME__ )
#pragma comment( compiler )

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTheseusApp::CTheseusApp ( )
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

CTheseusApp theApp;

BOOL CTheseusApp::InitInstance ( void )
{

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof( InitCtrls );
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx ( &InitCtrls );

	CWinApp::InitInstance ( );

	EnableTaskbarInteraction ( TRUE );

	SetRegistryKey ( _T ( "TheseusTerminal" ) );

	pMainFrame = new CTheseusView;
	xInitEnvironment ( );
	app_title = ( xIsRunAsAdmin ( ) ) ? so.STR_APP_ADM_PREF + so.STR_APP_TITLE : so.STR_APP_TITLE;
	app_title = app_title + L" v" + so.m_sVersion;

	BOOL rc = pMainFrame->Create ( NULL, app_title, WS_POPUP, CRect ( 0, 0, 0, 0 ) );
	CRect rcMonitor = xGetMonitorWorkArea (pMainFrame);

	pMainFrame->WindowWidth = so.OPT_APP_WIDTH;
	pMainFrame->WindowHeight = so.OPT_APP_HEIGHT;
	pMainFrame->WindowTop  = ( int ) round ( ( rcMonitor.bottom - pMainFrame->WindowHeight ) / 2 );
	pMainFrame->WindowLeft = ( int ) round ( ( rcMonitor.right  - pMainFrame->WindowWidth  ) / 2 );

	/********************/
		pFrameSplash = new CSplashScreen;
		BOOL rcSplash = pFrameSplash->Create ( NULL, app_title, WS_POPUP, CRect ( 0, 0, 0, 0 ) );

		int SplashWindowWidth = so.OPT_SPLASH_WIDTH;
		int SplashWindowHeight = so.OPT_SPLASH_HEIGHT;
		int SplashWindowTop = ( int ) round ( ( rcMonitor.bottom - SplashWindowHeight ) / 2 );
		int SplashWindowLeft = ( int ) round ( ( rcMonitor.right - SplashWindowWidth ) / 2 );

		pFrameSplash->SetWindowPos ( NULL, SplashWindowLeft, SplashWindowTop, SplashWindowWidth, SplashWindowHeight, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOACTIVATE | SWP_NOSENDCHANGING );
		pFrameSplash->ShowWindow ( SW_SHOW );
		pFrameSplash->InitaliseApp ( );
		pFrameSplash->UpdateWindow ( );
	/***********************/

	m_pMainWnd = pMainFrame;
	return TRUE;
}

int CTheseusApp::ExitInstance ( )
{
	return CWinApp::ExitInstance ( );
}
