#pragma once

#include "Theseus.strings.h"
#include "resource.h"
#include "CFlatButton.h"
#include "CMainPROMT.h"
#include "xCFGParser.h"
#include "xRunCommand.h"

CRect     xGetMonitorWorkArea ( const CPoint& point );
CRect     xGetMonitorWorkArea ( CWnd* pWnd );
CRect     xGetMonitorRect(CWnd* pWnd);
void      xCreateFastFont ( CFont& pFont, INT nHeight, INT nWeight, CString nFontName );
BOOL      xIsRunAsAdmin ( );
long int  xGetUnixTime();
CString   xGetTime(int FormatType = 0, CString DateTimeInfo = "Текущее время %Y, %A, %B %d, %X");
int       xGetDays(int month, int year);
bool      xCheckIsLeapYear(int year);
CString   xGetStrTime(int intTime, CString TimeType);
void      xInitEnvironment ( );
CString   xMakeLower(CString str);
CString   xMakeRand(int count);
CString   xGetFolder(int type);

class TheseusOptions
{
public:
	
	COLORREF SKIN_COLOR_APP_BG = RGB ( 45, 45, 48 ); // CTheseusApp
	CBrush   SKIN_COLOR_APP_BORDER = RGB ( 0, 122, 204 ); // CTheseusApp
	COLORREF SKIN_COLOR_SPLASH_BG = RGB ( 0, 122, 204 ); // CSplashScreen
	COLORREF SKIN_COLOR_SPLASH_TEXT = RGB (131, 176, 229); // CSplashScreen
	COLORREF SKIN_COLOR_COMMANDLINE_BG = RGB ( 45, 45, 48 ); // CTheseusApp
	COLORREF SKIN_COLOR_COMMANDLINE_TEXT = RGB ( 0, 122, 204 ); // CTheseusApp
	COLORREF SKIN_COLOR_CMDRESPONSELINE_BG = RGB ( 45, 45, 48 ); // CTheseusApp
	COLORREF SKIN_COLOR_CMDRESPONSELINE_TEXT = RGB ( 0, 122, 204 ); // CTheseusApp
	CBrush   SKIN_COLOR_FBTN_BORDER_NORMAL = RGB ( 0, 122, 204 ); // CFlatButton
	COLORREF   SKIN_COLOR_FBTN_FILL_NORMAL = RGB ( 0, 122, 204 ); // CFlatButton
	COLORREF   SKIN_COLOR_FBTN_TEXT_NORMAL = RGB ( 245, 245, 245 ); // CFlatButton
	CBrush   SKIN_COLOR_FBTN_BORDER_HOVER = RGB ( 10, 132, 214 ); // CFlatButton
	COLORREF   SKIN_COLOR_FBTN_FILL_HOVER = RGB ( 10, 132, 214 ); // CFlatButton
	COLORREF   SKIN_COLOR_FBTN_TEXT_HOVER = RGB ( 255, 255, 255 ); // CFlatButton
	CBrush   SKIN_COLOR_FBTN_BORDER_CDOWN = RGB ( 0, 112, 194 ); // CFlatButton
	COLORREF   SKIN_COLOR_FBTN_FILL_CDOWN = RGB ( 0, 112, 194 ); // CFlatButton
	COLORREF   SKIN_COLOR_FBTN_TEXT_CDOWN = RGB ( 235, 235, 235 ); // CFlatButton

	CBrush   SKIN_COLOR_FBTN_BORDER_CHCKD_NORMAL = RGB(15, 157, 88); // CFlatButton
	COLORREF   SKIN_COLOR_FBTN_FILL_CHCKD_NORMAL = RGB(15, 157, 88); // CFlatButton
	COLORREF   SKIN_COLOR_FBTN_TEXT_CHCKD_NORMAL = RGB(47, 47, 49); // CFlatButton
	CBrush   SKIN_COLOR_FBTN_BORDER_CHCKD_HOVER = RGB(25, 167, 98); // CFlatButton
	COLORREF   SKIN_COLOR_FBTN_FILL_CHCKD_HOVER = RGB(25, 167, 98); // CFlatButton
	COLORREF   SKIN_COLOR_FBTN_TEXT_CHCKD_HOVER = RGB(57, 57, 59); // CFlatButton
	CBrush   SKIN_COLOR_FBTN_BORDER_CHCKD_CDOWN = RGB(5, 147, 78); // CFlatButton
	COLORREF   SKIN_COLOR_FBTN_FILL_CHCKD_CDOWN = RGB(5, 147, 78); // CFlatButton
	COLORREF   SKIN_COLOR_FBTN_TEXT_CHCKD_CDOWN = RGB(37, 37, 39); // CFlatButton

	CString  STR_APP_TITLE = L"Theseus"; // CTheseus
	CString  STR_APP_ADM_PREF = L"[A] "; // CTheseus

	BOOL     OPT_FBTN_SET_CROSS_ONMOVE = TRUE; // CFlatButton
	INT      OPT_APP_WIDTH  = 450; // CTheseus
	INT      OPT_APP_HEIGHT = 250; // CTheseus
	CString  OPT_APP_SHOWPOS = L"center"; // CTheseus //center, top-left, top-center, top-right, bottom-left, bottom-center, bottom-right
	CString  OPT_APP_FOLDER = L"";
	CString  OPT_APP_CONFIG_FOLDER = L"conf/";
	CString  OPT_APP_CONFIG_FILE_EXT = L".xcfg";
	CString  OPT_APP_CONFIG_FILE = L"xConfig" + OPT_APP_CONFIG_FILE_EXT;
	CString  OPT_APP_CONFIG_LEARNBASE = L"xConfig.learnbase" + OPT_APP_CONFIG_FILE_EXT;
	bool     OPT_APP_CONFIG_RECORD_STAYONTOP = true;
	CString  OPT_APP_CONFIG_UPDATECFG_ALIASES = L"^(обновить|refresh|update)$";
	CString  OPT_APP_CONFIG_MAGICKEY = L"0";
	INT      OPT_SPLASH_WIDTH  = 450; // CTheseus
	INT      OPT_SPLASH_HEIGHT = 250; // CTheseus
	BOOL     OPT_SPLASH_BCGR_BITMAP = TRUE; // CSplashScreen; if LOAD BITMAP = true, if LOAD COLOR(COLOR_SPLASH_BG) = false
	CString   OPT_REGEX_RGB = L"RGB([ ]*)\\(([ ]*)([0-2]?[0-9]?[0-9]),([ ]*)([0-2]?[0-9]?[0-9]),([ ]*)([0-2]?[0-9]?[0-9])([ ]*)\\)"; // TheseusCore
	CString   OPT_REGEX_HEX = L"\\#([ ]*)([a-fA-F0-9]{1})([ ]*)([a-fA-F0-9]{1})([ ]*)([a-fA-F0-9]{1})([ ]*)([a-fA-F0-9]{1})([ ]*)([a-fA-F0-9]{1})([ ]*)([a-fA-F0-9]{1})"; // TheseusCore
	CString   OPT_REGEX_INC = L"\\@([ ]*)include([ ]*)\\((.*)\\)([ ]*)\\;"; // TheseusCore

	std::map <int, CString> F_OPT_LINES;
	std::map <int, CString> x_bParBlock;

	/** COMMANDS | OPTIONS START **/
	CString   OPT_COMMANDALS_RUN = L"run|open|открыть|запустить";
	std::map <CString, CString> xOptions_Command_Run;

	CString   OPT_COMMANDALS_INTSEARCH = L"search|поиск";
	std::map <CString, CString> xOptions_Command_InternetSearch;

	CString   OPT_COMMANDALS_STOP = L"stop|остановить";
	std::map <CString, CString> xOptions_Command_Stop;
	/** COMMANDS | OPTIONS END   **/

	/** FOR PROMT CONTROL | OPTIONS START **/
	CString   xRunCommand_FirstCommandText = L"";
	CString   xRunCommand_LastCommandText = L"";
	CString   xRunCommand_SpeechCommandText = L"";
	/** FOR PROMT CONTROL | OPTIONS END **/

	CString   m_sPath;
	CString   m_sVersion;
	WORD      m_nVersion[ 4 ];

	bool timerClocks = true;

};


extern TheseusOptions so;
