#pragma once

class CMainPROMT : public CEdit
{
public:
	CMainPROMT ( );
	virtual ~CMainPROMT ( );

protected:
	UINT OnGetDlgCode ( );
	void OnChar ( UINT nChar, UINT nRepCnt, UINT nFlags );
	DECLARE_MESSAGE_MAP ( )
};
