#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "TheseusCore.h"
#include "TheseusApp.h"
#include "CSplashScreen.h"

class CTheseusApp : public CWinApp
{
public:
	CTheseusApp ( );
	virtual BOOL InitInstance ( void );
	virtual int ExitInstance ( );
	CString app_title;
	CTheseusView* pMainFrame;
	CSplashScreen* pFrameSplash;
};

extern CTheseusApp theApp;
