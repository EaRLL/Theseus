#include "stdafx.h"
#include "Theseus.h"
#include "TheseusApp.h"


/*#include "include/bass.h"
#pragma comment(lib,"include/bass.lib")
DWORD timer = 0;
HRECORD chan;	// recording channel
BYTE *specbuf;*/


BEGIN_MESSAGE_MAP ( CTheseusView, CFrameWnd )
	ON_BN_CLICKED ( IDC_B_OPTIONS, OnBOptionsClick )
	ON_BN_CLICKED ( IDC_B_CLOSEAPP, OnBCloseAppClick )
	ON_BN_CLICKED ( IDC_B_HIDEAPP, OnBHideAppClick )
	ON_BN_CLICKED ( IDC_B_TITLE, OnBTitleClick)
	ON_BN_CLICKED ( IDC_B_COMANNDLINERECORD, OnBTheseusCommandLineRecordClick)
	ON_BN_CLICKED(IDC_B_LearnWindowClose, OnBLearnWindowCloseClick)
	ON_BN_CLICKED(IDC_B_LearnWindowFileDialog, OnBLearnWindowFileDialogClick)
	ON_BN_CLICKED(IDC_B_LearnWindowAddCommand, OnBLearnWindowAddCommandClick)
	ON_WM_PAINT ( )
	ON_WM_ERASEBKGND ( )
	ON_WM_SIZE()
	ON_WM_CTLCOLOR ( )
	ON_WM_NCHITTEST ( )
	ON_WM_QUERYDRAGICON ( )
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_DESTROY()
	ON_EN_SETFOCUS(IDC_E_COMMANDLINE, OnCMDSetFocus)
	ON_EN_KILLFOCUS(IDC_E_COMMANDLINE, OnCMDKillFocus)
	ON_MESSAGE(WM_HOTKEY, OnHotKey)
END_MESSAGE_MAP ( )

void ReplaceColor(HDC hDC, CRect rcReplaceArea, COLORREF clrColorReplace, COLORREF clrColorFill)
{
	CDC* pDC = CDC::FromHandle(hDC);
	CPoint pt = rcReplaceArea.TopLeft();
	CDC memDCMonoChrome;
	memDCMonoChrome.CreateCompatibleDC(pDC);
	CBitmap bmpMonoChrome;
	bmpMonoChrome.CreateCompatibleBitmap(&memDCMonoChrome, rcReplaceArea.Width(), rcReplaceArea.Height());
	CBitmap* pOldMonoBitmap = memDCMonoChrome.SelectObject(&bmpMonoChrome);

	COLORREF nOldBkColor = pDC->SetBkColor(clrColorReplace);
	memDCMonoChrome.BitBlt(0, 0, rcReplaceArea.Width(), rcReplaceArea.Height(), pDC, pt.x, pt.y, SRCCOPY);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rcReplaceArea.Width(), rcReplaceArea.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bmp);

	COLORREF nOldMemDCBkColor = memDC.SetBkColor(clrColorFill);
	COLORREF nOldMemDCTextColor = memDC.SetTextColor(RGB(255, 255, 255));
	memDC.BitBlt(0, 0, rcReplaceArea.Width(), rcReplaceArea.Height(), &memDCMonoChrome, 0, 0, SRCCOPY);

	pDC->BitBlt(pt.x, pt.y, rcReplaceArea.Width(), rcReplaceArea.Height(), &memDC, 0, 0, SRCAND);

	memDC.SetTextColor(RGB(0, 0, 0));
	memDC.BitBlt(0, 0, rcReplaceArea.Width(), rcReplaceArea.Height(), &memDCMonoChrome, 0, 0, SRCCOPY);

	pDC->BitBlt(pt.x, pt.y, rcReplaceArea.Width(), rcReplaceArea.Height(), &memDC, 0, 0, SRCPAINT);

	memDC.SetTextColor(nOldMemDCTextColor);
	memDC.SetBkColor(nOldMemDCBkColor);
	pDC->SetBkColor(nOldBkColor);

	memDCMonoChrome.SelectObject(pOldMonoBitmap);
	memDC.SelectObject(pOldBitmap);
}

/*void Error(const char *es)
{
	char mes[200];
	sprintf_s(mes, "%s\n(error code: %d)", es, BASS_ErrorGetCode());
	MessageBox(0, (LPCWSTR)mes, 0, 0);
}

BOOL CALLBACK DuffRecording(HRECORD handle, const void *buffer, DWORD length, void *user)
{
	return TRUE;
}*/

void CTheseusView::CreateChildControls ( void )
{
	xCreateFastFont ( f_TitleButBig, 16, 400, _T ( "Tahoma" ) );
	xCreateFastFont ( f_TitleButSmall, 16, 900, _T ( "Tahoma" ) );
	xCreateFastFont ( f_CommLineTx, 18, 600, _T ( "Tahoma" ) );

	b_CloseApp.Create ( L"X", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, CRect (theApp.pMainFrame->WindowWidth - 30, 1, theApp.pMainFrame->WindowWidth-1, 30 ), this, IDC_B_CLOSEAPP );
	b_HideApp.Create ( L"_", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, CRect (theApp.pMainFrame->WindowWidth - 60, 1, theApp.pMainFrame->WindowWidth - 30, 30 ), this, IDC_B_HIDEAPP );
	b_Options.Create (__string_TheseusApp_Options, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, CRect (theApp.pMainFrame->WindowWidth - 150, 1, theApp.pMainFrame->WindowWidth - 60, 30 ), this, IDC_B_OPTIONS );
	b_Title.Create ( theApp.app_title, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, CRect ( 1, 1, theApp.pMainFrame->WindowWidth - 150, 30 ), this, IDC_B_TITLE );
	b_TheseusCommandLineRecord.Create( L"R", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, CRect(theApp.pMainFrame->WindowWidth - 40, theApp.pMainFrame->WindowHeight - 40, theApp.pMainFrame->WindowWidth - 1, theApp.pMainFrame->WindowHeight - 1), this, IDC_B_COMANNDLINERECORD);
	b_HideApp.SetFont ( &f_TitleButSmall );
	b_CloseApp.SetFont ( &f_TitleButSmall );
	b_Options.SetFont ( &f_TitleButBig );
	b_Title.SetFont(&f_TitleButBig);
	b_Title.Draggable = true;

	b_TheseusCommandLineRecord.SetFont(&f_TitleButBig);
	b_TheseusCommandLineRecord.CheckOpt = true;

	l_TheseusCommandLineSharp.Create ( L"#", WS_CHILD | WS_VISIBLE, CRect ( 10, theApp.pMainFrame->WindowHeight - 29, 30, theApp.pMainFrame->WindowHeight - 9 ), this, IDC_L_COMMANDLINESHARP);
	l_TheseusCommandLineSharp.SetFont ( &f_CommLineTx );

	l_TheseusResponseLine.Create(L"", WS_CHILD | WS_VISIBLE, CRect(10, 40, theApp.pMainFrame->WindowWidth - 10, theApp.pMainFrame->WindowHeight - 40), this, IDC_L_RESPONSELINE);
	l_TheseusResponseLine.SetFont(&f_TitleButBig);

	TheseusCommandLine.Create ( ES_MULTILINE | ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE, CRect ( 25, theApp.pMainFrame->WindowHeight - 30, theApp.pMainFrame->WindowWidth - 50, theApp.pMainFrame->WindowHeight - 10 ), this, IDC_E_COMMANDLINE);
	TheseusCommandLine.SetFont ( &f_CommLineTx );
	TheseusCommandLine.SetFocus ( );

	/*if (!BASS_RecordInit(-1))
		Error("Can't initialize device");
	// start recording (44100hz mono 16-bit)
	if (!(chan = BASS_RecordStart(44100, 1, 0, &DuffRecording, 0)))
		Error("Can't start recording");

	if (HIWORD(BASS_GetVersion()) != BASSVERSION)
		MessageBox(0, L"An incorrect version of BASS.DLL was loaded");*/
	
	return;
}

void CTheseusView::LearnCommandWindow (CString Command)
{
	pLearnWindowFrame = new CTheseusView;
	ASSERT_VALID(pLearnWindowFrame);

	CRect rcWinPos = xGetMonitorWorkArea(theApp.pMainFrame);

	pLearnWindowFrame->WindowWidth = theApp.pMainFrame->WindowWidth;
	pLearnWindowFrame->WindowHeight = theApp.pMainFrame->WindowHeight;
	pLearnWindowFrame->WindowLeft = (int)round((rcWinPos.right - pLearnWindowFrame->WindowWidth) / 2);
	pLearnWindowFrame->WindowTop = (int)round((theApp.pMainFrame->WindowTop - 100 < 100) ? 100 : theApp.pMainFrame->WindowTop - 100);

	BOOL rc = pLearnWindowFrame->Create(NULL, __string_TheseusApp_LearnWindow_Title, WS_POPUP | WS_EX_DLGMODALFRAME, CRect(0, 0, 0, 0), this);
	
	b_LearnWindowTitle.Create(__string_TheseusApp_LearnWindow_Title, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, CRect(1, 1, pLearnWindowFrame->WindowWidth - 30, 30), pLearnWindowFrame, IDC_B_LearnWindowTitle);
	b_LearnWindowClose.Create(L"X", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, CRect(pLearnWindowFrame->WindowWidth - 30, 1, pLearnWindowFrame->WindowWidth - 1, 30), pLearnWindowFrame, IDC_B_LearnWindowClose);
	b_LearnWindowFileDialog.Create(L"...", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, CRect(pLearnWindowFrame->WindowWidth - 40, 170, pLearnWindowFrame->WindowWidth - 10, 190), pLearnWindowFrame, IDC_B_LearnWindowFileDialog);
	b_LearnWindowAddCommand.Create(__string_TheseusApp_LearnWindow_LearnOk, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, CRect(pLearnWindowFrame->WindowWidth - 200, 200, pLearnWindowFrame->WindowWidth - 10, 240), pLearnWindowFrame, IDC_B_LearnWindowAddCommand);

	b_LearnWindowAddCommand.SetFont(&f_TitleButSmall);
	b_LearnWindowFileDialog.SetFont(&f_TitleButSmall);
	b_LearnWindowClose.SetFont(&f_TitleButSmall);
	b_LearnWindowTitle.SetFont(&f_TitleButBig);
	b_LearnWindowTitle.Draggable = true;

	l_LearnWindowStartCaption.Create(__string_TheseusApp_LearnWindow_Caption_ShouldI, WS_CHILD | WS_VISIBLE | DT_CENTER, CRect(10, 40, pLearnWindowFrame->WindowWidth - 10, 60), pLearnWindowFrame, IDC_L_LearnWindowCaption);
	l_LearnWindowStartCaption.SetFont(&f_TitleButBig);

	l_LearnWindowActionCaption.Create(__string_TheseusApp_LearnWindow_Caption_WhtIDo, WS_CHILD | WS_VISIBLE | DT_CENTER, CRect(10, 130, pLearnWindowFrame->WindowWidth - 10, 165), pLearnWindowFrame, IDC_L_LearnWindowCaption);
	l_LearnWindowActionCaption.SetFont(&f_TitleButBig);

	l_LearnWindowPoint1Caption.Create(L">", WS_CHILD | WS_VISIBLE, CRect(25, 80, 35, 100), pLearnWindowFrame, IDC_L_LearnWindowCaption);
	l_LearnWindowPoint1Caption.SetFont(&f_TitleButBig);
	l_LearnWindowPoint2Caption.Create(L">", WS_CHILD | WS_VISIBLE, CRect(25, 170, 35, 190), pLearnWindowFrame, IDC_L_LearnWindowCaption);
	l_LearnWindowPoint2Caption.SetFont(&f_TitleButBig);

	l_LearnWindowComandBox.Create(ES_MULTILINE | ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE, CRect(35, 80, pLearnWindowFrame->WindowWidth - 50, 100), pLearnWindowFrame, IDC_L_LearnWindowCommandBox);
	l_LearnWindowComandBox.SetFont(&f_CommLineTx);
	l_LearnWindowComandBox.SetWindowText(Command);

	l_LearnWindowActionBox.Create(ES_MULTILINE | ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE, CRect(35, 170, pLearnWindowFrame->WindowWidth - 50, 190), pLearnWindowFrame, IDC_L_LearnWindowActionBox);
	l_LearnWindowActionBox.SetFont(&f_CommLineTx);
	l_LearnWindowActionBox.SetWindowText(L"");

	pLearnWindowFrame->SetWindowPos(NULL, pLearnWindowFrame->WindowLeft, pLearnWindowFrame->WindowTop, pLearnWindowFrame->WindowWidth, pLearnWindowFrame->WindowHeight, SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSENDCHANGING);
	pLearnWindowFrame->ShowWindow(SW_SHOW);
	pLearnWindowFrame->UpdateWindow();
	
	return afx_msg void();
}

void CTheseusView::OnBLearnWindowCloseClick(void)
{
	BOOL rc = DestroyWindow();
	return afx_msg void();
}

void CTheseusView::OnBLearnWindowAddCommandClick(void)
{
	theApp.pMainFrame->l_LearnWindowComandBox.GetWindowText(theApp.pMainFrame->l_LearnWindowComandBoxText);
	theApp.pMainFrame->l_LearnWindowActionBox.GetWindowText(theApp.pMainFrame->l_LearnWindowActionBoxText);
	if (!theApp.pMainFrame->l_LearnWindowComandBoxText.IsEmpty() && !theApp.pMainFrame->l_LearnWindowActionBoxText.IsEmpty())
	{
		CWinThread *pThread = AfxBeginThread(_Thread_LearnWIndow_LearnOk, GetSafeHwnd(), THREAD_PRIORITY_NORMAL, 0);

		theApp.pMainFrame->l_TheseusResponseLine.SetWindowText(__string_TheseusApp_LearnWindow_Response);

		theApp.pMainFrame->RedrawWindow();

		theApp.pMainFrame->TheseusCommandLine.SetWindowText(so.xRunCommand_LastCommandText);
		theApp.pMainFrame->TheseusCommandLine.UpdateWindow();
		int TextLength = so.xRunCommand_LastCommandText.GetLength();
		theApp.pMainFrame->TheseusCommandLine.SetSel(TextLength, TextLength);
		theApp.pMainFrame->TheseusCommandLine.SetFocus();

		OnBLearnWindowCloseClick();
	}
}

UINT _Thread_LearnWIndow_LearnOk(LPVOID param)
{
	std::wofstream fout(so.OPT_APP_CONFIG_FOLDER + so.OPT_APP_CONFIG_LEARNBASE, std::ios_base::app);

	if (fout.is_open() && !fout.fail())
	{
		const std::locale empty_locale = std::locale::empty();
		typedef std::codecvt_utf8<wchar_t> converter_type;
		const converter_type* converter = new converter_type;
		const std::locale utf8_locale = std::locale(empty_locale, converter);
		fout.imbue(utf8_locale);

		CString sRSubParams = L"";
		int I = 0;
		while (AfxExtractSubString(sRSubParams, theApp.pMainFrame->l_LearnWindowComandBoxText, I, '|'))
		{
			std::wstring ws = L"\n[run_" + sRSubParams + L"] = " + theApp.pMainFrame->l_LearnWindowActionBoxText + L";";

			so.xOptions_Command_Run.insert(std::make_pair(sRSubParams, theApp.pMainFrame->l_LearnWindowActionBoxText));

			fout << ws;
			sRSubParams.Empty();
			I++;
		}
		fout.close();
	}
	AfxEndThread(0);
	return 0;
}

void CTheseusView::OnBLearnWindowFileDialogClick(void)
{
	CWinThread *pThread = AfxBeginThread(_Thread_LearnWIndow_ShowFileDialog, GetSafeHwnd());
	return afx_msg void();
}

UINT _Thread_LearnWIndow_ShowFileDialog(LPVOID param)
{
	TCHAR szFilters[] = _T("All Files (*.*)|*.*||");
	CFileDialog fileDlg(TRUE, _T("*"), _T("*.*"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	if (fileDlg.DoModal() == IDOK)
	{
		CString pathName = fileDlg.GetPathName();
		theApp.pMainFrame->l_LearnWindowActionBox.SetWindowText(pathName);
		theApp.pMainFrame->l_LearnWindowActionBox.UpdateWindow();
	}

	AfxEndThread(0);
	return 0;
}

void CTheseusView::OnBOptionsClick(void)
{
	CTheseusView *pFrame;
	pFrame = new CTheseusView;
	ASSERT_VALID(pFrame);

	CRect rcWinPos = xGetMonitorWorkArea(theApp.pMainFrame);
	pFrame->WindowWidth = theApp.pMainFrame->WindowWidth;
	pFrame->WindowHeight = theApp.pMainFrame->WindowHeight;
	pFrame->WindowLeft = (int)round((rcWinPos.right - pFrame->WindowWidth) / 2);
	pFrame->WindowTop = (int)round((theApp.pMainFrame->WindowTop - 100 < 100) ? 100 : theApp.pMainFrame->WindowTop - 100);

	BOOL rc = pFrame->Create(NULL, __string_TheseusApp_LearnWindow_Title, WS_POPUPWINDOW | WS_DLGFRAME | WS_EX_TOPMOST, CRect(0, 0, 0, 0), this);

	pFrame->SetWindowPos(NULL, pFrame->WindowLeft, pFrame->WindowTop, pFrame->WindowWidth, pFrame->WindowHeight, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSENDCHANGING);
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return afx_msg void();
}

void CTheseusView::OnBCloseAppClick ( void )
{
	BOOL rc = DestroyWindow ( );
	return afx_msg void ( );
}

void CTheseusView::OnBHideAppClick(void)
{
	ShowWindow(SW_MINIMIZE);
	isWindowMinimized = true;
}

void CTheseusView::OnBTheseusCommandLineRecordClick(void)
{
	if (b_TheseusCommandLineRecord.Checked)
	{
		int count = waveInGetNumDevs();
		CString _RESPONSE = L"";
		wchar_t TName;

		for (int i = 0; i < count; i++) {
			WAVEINCAPS wavInCaps;
			waveInGetDevCaps(i, &wavInCaps, sizeof(WAVEINCAPS));
			TName = *wavInCaps.szPname;
			_RESPONSE += wavInCaps.szPname;
		}

		int mmres;

		wavform.wFormatTag = WAVE_FORMAT_PCM;   //Используемый формат
		wavform.nChannels = 2;                  //Кол-во каналов
		wavform.nSamplesPerSec = 44100;         //Записей в секунду (дискретизация, Гц)
		wavform.nAvgBytesPerSec = 176400;       //Байт/с
		wavform.nBlockAlign = 4;                // Bits / 8 * Channels
		wavform.wBitsPerSample = 16;            //Разрядность 8 или 16
		wavform.cbSize = sizeof(wavform);       //Размер структуры

		mmres = waveInOpen(&hwi, WAVE_MAPPER, &wavform, NULL, 0, CALLBACK_WINDOW);
		if (mmres != MMSYSERR_NOERROR)
			_RESPONSE += L"waveInOpen fail/";

		memset(&wavbuf, 0, sizeof(wavbuf));
		wavbuf.lpData = (LPSTR)malloc(176400 * 10);
		wavbuf.dwBufferLength = 176400 * 10;
		wavbuf.dwFlags = 0;

		mmres = waveInPrepareHeader(hwi, &wavbuf, sizeof(wavbuf));
		if (mmres != MMSYSERR_NOERROR)
			_RESPONSE += L"waveInPrepareHeader fail/";

		if (waveInAddBuffer(hwi, &wavbuf, sizeof(wavbuf)) != MMSYSERR_NOERROR)
			_RESPONSE += L"waveInAddBuffer fail/";

		if (waveInStart(hwi) != MMSYSERR_NOERROR)
			_RESPONSE += L"waveInStart fail/";

		recording = true;

		l_TheseusResponseLine.SetWindowText(__string_TheseusApp_SayMagicWords /*_RESPONSE*/);
		RedrawWindow();
	}
	else
	{
		if (!recording)
			return;

		waveInReset(hwi);
		waveInStop(hwi);
		waveInUnprepareHeader(hwi, &wavbuf, sizeof(wavbuf));
		waveInClose(hwi);
		recorded = true;
		recording = false;

		wavbuf.lpData = (LPSTR)realloc(wavbuf.lpData, wavbuf.dwBytesRecorded);
		wavbuf.dwBufferLength = wavbuf.dwBytesRecorded;

		HMMIO hmmio = mmioOpenA("speech.wav", 0, MMIO_CREATE | MMIO_WRITE);

		MMCKINFO mmCkInfo, mmckinfoFormatChunk;
		mmCkInfo.ckid = mmioFOURCC('R', 'I', 'F', 'F');
		mmCkInfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		mmCkInfo.dwFlags = MMIO_DIRTY;
		mmioCreateChunk(hmmio, &mmCkInfo, MMIO_CREATERIFF);

		mmckinfoFormatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
		mmckinfoFormatChunk.dwFlags = MMIO_DIRTY;
		mmckinfoFormatChunk.cksize = sizeof(wavform);
		mmioCreateChunk(hmmio, &mmckinfoFormatChunk, NULL);
		mmioWrite(hmmio, (char*)&wavform, sizeof(wavform));

		mmCkInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
		mmCkInfo.cksize = wavbuf.dwBufferLength;
		mmCkInfo.dwFlags = MMIO_DIRTY;
		mmioCreateChunk(hmmio, &mmCkInfo, NULL);
		mmioWrite(hmmio, wavbuf.lpData, wavbuf.dwBufferLength);

		mmioClose(hmmio, 0);
		
		CString rundUUID = xMakeRand(32);
		if (so.OPT_APP_CONFIG_MAGICKEY.GetLength() == 36)
		{
			ShellExecute(NULL, L"open", L"curl.exe", L"-X POST -H \"Content-Type: audio/x-wav\" --data-binary \"@speech.wav\" \"https://asr.yandex.net/asr_xml?uuid=" + rundUUID + "&key=" + so.OPT_APP_CONFIG_MAGICKEY + "&topic=queries\" -o \"speech.xml\"", NULL, SW_HIDE);

			while (!boost::filesystem::exists(L"speech.xml")) { // TODO: rewrite THIS
				//wait
			}

			while (boost::filesystem::file_size("speech.xml") < 38) { // TODO: rewrite THIS
				//wait
			}

			std::wifstream optFile(L"speech.xml");

			if (optFile.is_open() && !optFile.fail())
			{
				boost::property_tree::ptree ptr;

				try {
					boost::property_tree::read_xml("speech.xml", ptr);

					const boost::property_tree::ptree& formats = ptr.get_child("recognitionResults");

					int counts = ptr.get<int>("recognitionResults.<xmlattr>.success");

					if (counts != 0)
					{
						const std::locale empty_locale = std::locale::empty();
						typedef std::codecvt_utf8<wchar_t> converter_type;
						const converter_type* converter = new converter_type;
						const std::locale utf8_locale = std::locale(empty_locale, converter);
						optFile.imbue(utf8_locale);

						CString sOptLine = _T("");
						std::wstring _s_ReadLine;

						boost::tmatch xRexpMatch;

						while (getline(optFile, _s_ReadLine))
						{
							sOptLine = _s_ReadLine.c_str();
							sOptLine = sOptLine.Trim();

							if (boost::regex_match(sOptLine, xRexpMatch, boost::tregex(L"<variant confidence=\"([^\"]*)\">([^<]*)</variant>", boost::regex_constants::icase)))
							{
								const CString xStrIncFilePath = __T("$2");
								so.xRunCommand_SpeechCommandText = boost::regex_replace(sOptLine, boost::tregex(L"<variant confidence=\"([^\"]*)\">([^<]*)</variant>"), xStrIncFilePath);

								theApp.pMainFrame->TheseusCommandLine.SetWindowText(L"");
								xRunCommand(so.xRunCommand_SpeechCommandText);
								theApp.pMainFrame->TheseusCommandLine.UpdateWindow();
								theApp.pMainFrame->TheseusCommandLine.SetFocus();

								break;
							}
						}
					}
					else
					{
						theApp.pMainFrame->l_TheseusResponseLine.SetWindowText(__string_TheseusApp_SayAgain);
					}

				}
				catch (const boost::property_tree::xml_parser_error&) {

					std::wifstream ppoptFile(so.OPT_APP_FOLDER + "speech.xml");
					
					std::wstring _s_ReadLine;
					CString _w_ReadText;
					CString sOptLine = _T("");
					while (getline(ppoptFile, _s_ReadLine))
					{
						sOptLine = _s_ReadLine.c_str();
						sOptLine.Trim();
						if (!sOptLine.IsEmpty())
						{
							_w_ReadText =+ sOptLine;
						}
					}

					ppoptFile.close();

					theApp.pMainFrame->l_TheseusResponseLine.SetWindowText(__string_TheseusApp_SayError + " : " + _w_ReadText);
				}

				optFile.close();
			}
			else
			{
				theApp.pMainFrame->l_TheseusResponseLine.SetWindowText(__string_TheseusApp_SayAgainOneMore);
			}
		}
		else
		{
			theApp.pMainFrame->l_TheseusResponseLine.SetWindowText(__string_TheseusApp_SayNotMagic);
		}

		theApp.pMainFrame->RedrawWindow();
		remove("speech.xml");
		remove("speech.wav");
	}
}

void CTheseusView::OnBTitleClick ( void )
{
	return afx_msg void ( );
}

void CTheseusView::OnPaint ( void )
{
	SetIcon ( m_hIcon, TRUE );
	SetIcon ( m_hIcon, FALSE );

	m_nIDHotKeyRecord = GlobalAddAtom(L"PressingCtrlStar");
	TRACE("m_nIDHotKeyRecord = %d\n", m_nIDHotKeyRecord);
	RegisterHotKey(m_hWnd, m_nIDHotKeyRecord, MOD_CONTROL, VK_MULTIPLY);

	return afx_msg void ( );
}

BOOL CTheseusView::PreCreateWindow ( CREATESTRUCT& cs )
{
	if ( !CFrameWnd::PreCreateWindow ( cs ) )
		return FALSE;
	cs.dwExStyle &= ~( WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE | WS_CAPTION | WS_THICKFRAME | WS_SYSMENU );

	return TRUE;
}

void CTheseusView::OnDestroy()
{
	CFrameWnd::OnDestroy();
	UnregisterHotKey(m_hWnd, m_nIDHotKeyRecord);
}

LRESULT CTheseusView::OnHotKey(WPARAM wp, LPARAM lp)
{
	TRACE(_T("CAltTabDlg::OnHotKey\n"));
	if (wp == m_nIDHotKeyRecord)
	{
		b_TheseusCommandLineRecord.Checked = (b_TheseusCommandLineRecord.Checked ? false : true);
		OnBTheseusCommandLineRecordClick();

		if (so.OPT_APP_CONFIG_RECORD_STAYONTOP)
		{
			theApp.pMainFrame->SendMessage(NULL, WM_SYSCOMMAND, SC_RESTORE);
			theApp.pMainFrame->SetForegroundWindow();
			theApp.pMainFrame->SetActiveWindow();
		}
		theApp.pMainFrame->UpdateWindow();
	}

	return 0;
}

void CTheseusView::OnSize(UINT nType, int cx, int cy)
{
	switch (nType)
	{
	case SIZE_MAXIMIZED:
		// window was maximized
		break;
	case SIZE_MINIMIZED:
		// window was minimized
		break;
	case SIZE_RESTORED:
		UpdateWindow();
		isWindowMinimized = false;
		break;
	default:
		// 
		break;
	}
}

void CTheseusView::OnWindowPosChanging ( WINDOWPOS* lpwndpos )
{
	CFrameWnd::OnWindowPosChanging ( lpwndpos );
	if (!isWindowMinimized)
	{
		CRect rcMonitor;
		this->GetWindowRect(&rcMonitor);
		this->WindowTop = rcMonitor.top;
		this->WindowLeft = rcMonitor.left;
	}
}

BOOL CTheseusView::OnEraseBkgnd ( CDC* pDC )
{
	CBrush backBrush ( so.SKIN_COLOR_APP_BG );
	CBrush* pOldBrush = pDC->SelectObject ( &backBrush );
	CRect rect;
	pDC->GetClipBox ( &rect );
	pDC->PatBlt ( rect.left, rect.top, rect.Width ( ), rect.Height ( ), PATCOPY );
	pDC->SelectObject ( pOldBrush );
	pDC->FrameRect ( rect, &so.SKIN_COLOR_APP_BORDER );

	return TRUE;
}

HBRUSH CTheseusView::OnCtlColor ( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
	UINT id = pWnd->GetDlgCtrlID ( );
	if ( id == IDC_L_COMMANDLINESHARP)
	{
		pDC->SetTextColor ( so.SKIN_COLOR_CMDRESPONSELINE_TEXT);
		pDC->SetBkColor ( so.SKIN_COLOR_CMDRESPONSELINE_BG);
		return ( HBRUSH )::GetStockObject ( NULL_BRUSH );
	}
	else if (id == IDC_E_COMMANDLINE)
	{
		pDC->SetTextColor(so.SKIN_COLOR_COMMANDLINE_TEXT);
		pDC->SetBkColor(so.SKIN_COLOR_COMMANDLINE_BG);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	else if (id == IDC_L_RESPONSELINE)
	{
		pDC->SetTextColor(so.SKIN_COLOR_CMDRESPONSELINE_TEXT);
		pDC->SetBkColor(so.SKIN_COLOR_CMDRESPONSELINE_BG);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	else if (id == IDC_L_LearnWindowCaption)
	{
		pDC->SetTextColor(so.SKIN_COLOR_CMDRESPONSELINE_TEXT);
		pDC->SetBkColor(so.SKIN_COLOR_CMDRESPONSELINE_BG);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	else if (id == IDC_L_LearnWindowCommandBox || id == IDC_L_LearnWindowActionBox)
	{
		pDC->SetTextColor(so.SKIN_COLOR_COMMANDLINE_TEXT);
		pDC->SetBkColor(so.SKIN_COLOR_COMMANDLINE_BG);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	else
	{
		return ( HBRUSH )::GetStockObject ( NULL_BRUSH );
	}
}

LRESULT CTheseusView::OnNcHitTest ( CPoint point ) // move window
{
	UINT ret = (UINT)CFrameWnd::OnNcHitTest ( point );
	if ( ret == HTCLIENT )
		return HTCAPTION;
	return ret;
}

