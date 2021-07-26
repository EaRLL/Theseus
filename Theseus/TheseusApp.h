#pragma once

class CTheseusView : public CFrameWnd
{
public:
	CTheseusView ( void ){
		m_hIcon = AfxGetApp ( )->LoadIcon ( IDI_MAINICON );
	};
	~CTheseusView ( void ){
	};
	virtual void CreateChildControls ( void );
	virtual BOOL PreCreateWindow ( CREATESTRUCT& cs );

	CFlatButton b_CloseApp, b_HideApp, b_Options, b_Title;
	CStatic l_TheseusCommandLineSharp, l_TheseusResponseLine;
	CFlatButton b_TheseusCommandLineRecord;
	CFont f_TitleButBig, f_TitleButSmall, f_CommLineTx;
	CMainPROMT TheseusCommandLine;
	int caAnchor = 0;

	HWND hwnd;
	HINSTANCE hinstance;
	bool closed;
	bool visible;
	BOOL isWindowMinimized = false;

	CFlatButton b_LearnWindowClose, b_LearnWindowTitle, b_LearnWindowFileDialog, b_LearnWindowAddCommand;
	CEdit l_LearnWindowComandBox, l_LearnWindowActionBox;
	CStatic l_LearnWindowStartCaption, l_LearnWindowActionCaption, l_LearnWindowPoint1Caption, l_LearnWindowPoint2Caption;
	CString l_LearnWindowComandBoxText, l_LearnWindowActionBoxText;
	CTheseusView *pLearnWindowFrame;
	void LearnCommandWindow( CString Command );
	void OnBLearnWindowCloseClick(void);
	void OnBLearnWindowFileDialogClick(void);
	void OnBLearnWindowAddCommandClick(void);

	WAVEHDR wavbuf;
	bool recording = false;
	bool recorded = false;
	HWAVEIN hwi;
	HWAVEOUT hwo;
	WAVEFORMATEX wavform;

	int WindowTop;
	int WindowLeft;
	int WindowWidth;
	int WindowHeight;


protected:
	HICON m_hIcon;
	void OnPaint ( void );
	void OnBOptionsClick(void);
	void OnBCloseAppClick ( void );
	void OnBHideAppClick ( void );
	void OnBTitleClick ( void );
	void OnBTheseusCommandLineRecordClick(void);
	void OnSize(UINT nType, int cx, int cy);
	void OnDestroy();
	BOOL OnEraseBkgnd ( CDC* pDC );
	HBRUSH OnCtlColor ( CDC* pDC, CWnd* pWnd, UINT nCtlColor );
	LRESULT OnNcHitTest ( CPoint point );
	void OnWindowPosChanging ( WINDOWPOS* lpwndpos );

	HCURSOR OnQueryDragIcon ( ){
		return static_cast<HCURSOR>( m_hIcon );
	};
	void OnCMDSetFocus ( ){
		TheseusCommandLine.UpdateWindow ( );
	};
	void OnCMDKillFocus ( ){
		TheseusCommandLine.UpdateWindow ( );
	};

	LRESULT OnHotKey(WPARAM wp, LPARAM lp);
	UINT m_nIDHotKeyRecord;

	DECLARE_MESSAGE_MAP ( );
};

UINT _Thread_LearnWIndow_ShowFileDialog(LPVOID param);
UINT _Thread_LearnWIndow_LearnOk(LPVOID param);
