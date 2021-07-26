#include "stdafx.h"
#include "CMainPROMT.h"
#include "xRunCommand.h"
#include "TheseusCore.h"

#define WM_EDIT_ENTER (WM_USER+1)

BEGIN_MESSAGE_MAP ( CMainPROMT, CEdit )
	ON_WM_GETDLGCODE ( )
	ON_WM_CHAR ( )
END_MESSAGE_MAP ( )

CMainPROMT::CMainPROMT ( )
{
}

CMainPROMT::~CMainPROMT ( )
{
}

UINT CMainPROMT::OnGetDlgCode ( )
{
	return DLGC_WANTARROWS | DLGC_WANTALLKEYS | DLGC_WANTCHARS;
}

void CMainPROMT::OnChar ( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	CString promtCommand;
	GetWindowText(promtCommand);

	if ( nChar == VK_RETURN )
	{
		if ( !promtCommand.IsEmpty() )
		{
			SetWindowText ( L"" );
			xRunCommand(promtCommand);
			UpdateWindow ( );
			SetFocus ( );
			return;
		}
		else
		{
			SetFocus ( );
			return;
		}
	}
	else if (nChar == VK_TAB && !so.xRunCommand_FirstCommandText.IsEmpty() && !promtCommand.IsEmpty())
	{
		int TextLength = so.xRunCommand_FirstCommandText.GetLength();

		SetWindowText(so.xRunCommand_FirstCommandText);
		UpdateWindow();
		SetFocus();
		SetSel(TextLength, TextLength);
		return;
	}
	else if (nChar == VK_TAB && !so.xRunCommand_LastCommandText.IsEmpty() && promtCommand.IsEmpty())
	{
		int TextLength = so.xRunCommand_LastCommandText.GetLength();

		SetWindowText(so.xRunCommand_LastCommandText);
		UpdateWindow();
		SetFocus();
		SetSel(TextLength, TextLength);
		return;
	}
	else
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);

		GetWindowText(promtCommand);
		xRunCommand_ShowCommandList(promtCommand);
	}
}
