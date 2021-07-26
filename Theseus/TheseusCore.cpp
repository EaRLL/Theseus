#include "stdafx.h"
#include "TheseusCore.h"

TheseusOptions so;

struct MonitorAPI
{
	MonitorAPI ( );
	~MonitorAPI ( );

	struct MonitorInfo
	{
		MonitorInfo ( ) : cbSize ( sizeof( MonitorInfo ) )
		{
		}

		RECT rcMonitor;
		RECT rcWork;
		DWORD dwFlags;
		DWORD cbSize;
	};

	enum
	{
		DefaultToNull = 0,
		DefaultToPrimary = 1,
		DefaultToNearest = 2
	};

	typedef HANDLE ( WINAPI* pfnMonitorFromPoint )( POINT pt, DWORD dwFlags );
	typedef BOOL ( WINAPI* pfnGetMonitorInfo )( HANDLE hMonitor, MonitorInfo* pmi );
	typedef HANDLE ( WINAPI* pfnMonitorFromWindow )( HWND hWnd, DWORD dwFlags );

	pfnMonitorFromPoint pMonitorFromPoint;
	pfnMonitorFromWindow pMonitorFromWindow;
	pfnGetMonitorInfo pGetMonitorInfo;

	bool IsLoaded ( ) const
	{
		return hUser32 != NULL;
	}

	HINSTANCE hUser32;
};

static MonitorAPI theMonitorAPI;

MonitorAPI::MonitorAPI ( )
: pMonitorFromPoint ( NULL ),
pMonitorFromWindow ( NULL ),
pGetMonitorInfo ( NULL )
{
	hUser32 = ::LoadLibrary ( _T ( "user32.dll" ) );
	if ( hUser32 != NULL )
	{
		pMonitorFromPoint = ( pfnMonitorFromPoint ) ::GetProcAddress ( hUser32, "MonitorFromPoint" );
		pMonitorFromWindow = ( pfnMonitorFromWindow ) ::GetProcAddress ( hUser32, "MonitorFromWindow" );
		pGetMonitorInfo = ( pfnGetMonitorInfo ) ::GetProcAddress ( hUser32, "GetMonitorInfoA" );

		if ( pMonitorFromPoint == NULL
			 || pMonitorFromWindow == NULL
			 || pGetMonitorInfo == NULL )
		{
			::FreeLibrary ( hUser32 );
			hUser32 = NULL;
		}
	}
}

MonitorAPI::~MonitorAPI ( )
{
	if ( hUser32 != NULL )
		::FreeLibrary ( hUser32 );
}

CRect xGetMonitorWorkArea ( const CPoint& point )
{
	CRect rcWorkArea;

	if ( !::SystemParametersInfo ( SPI_GETWORKAREA, 0, ( PVOID ) &rcWorkArea, SPIF_SENDWININICHANGE) )
	{
		CSize szScreen ( ::GetSystemMetrics ( SM_CXSCREEN ), ::GetSystemMetrics ( SM_CYSCREEN ) );
		rcWorkArea = CRect ( CPoint ( 0, 0 ), szScreen );
	}

	if ( theMonitorAPI.IsLoaded ( ) )
	{
		MonitorAPI::MonitorInfo mi;
		HANDLE hMonitor = theMonitorAPI.pMonitorFromPoint ( point, MonitorAPI::DefaultToNearest );
		if ( hMonitor != NULL && theMonitorAPI.pGetMonitorInfo ( hMonitor, &mi ) )
			rcWorkArea = mi.rcWork;
	}

	return rcWorkArea;
}

CRect xGetMonitorWorkArea ( CWnd* pWnd )
{
	CRect rcWorkArea;

	if ( !::SystemParametersInfo ( SPI_GETWORKAREA, 0, ( PVOID ) &rcWorkArea, SPIF_SENDWININICHANGE) )
	{
		CSize szScreen ( ::GetSystemMetrics ( SM_CXSCREEN ), ::GetSystemMetrics ( SM_CYSCREEN ) );
		rcWorkArea = CRect ( CPoint ( 0, 0 ), szScreen );
	}

	if ( theMonitorAPI.IsLoaded ( ) )
	{
		MonitorAPI::MonitorInfo mi;
		HANDLE hMonitor = theMonitorAPI.pMonitorFromWindow ( pWnd->GetSafeHwnd ( ), MonitorAPI::DefaultToNearest );
		if ( hMonitor != NULL && theMonitorAPI.pGetMonitorInfo ( hMonitor, &mi ) )
			rcWorkArea = mi.rcWork;
	}

	return rcWorkArea;
}

CRect xGetMonitorRect ( CWnd* pWnd )
{
	CSize szScreen ( ::GetSystemMetrics ( SM_CXSCREEN ), ::GetSystemMetrics ( SM_CYSCREEN ) );
	CRect rcMonitor ( CPoint ( 0, 0 ), szScreen );

	if ( theMonitorAPI.IsLoaded ( ) )
	{
		MonitorAPI::MonitorInfo mi;
		HANDLE hMonitor = theMonitorAPI.pMonitorFromWindow ( pWnd->GetSafeHwnd ( ), MonitorAPI::DefaultToNearest );
		if ( hMonitor != NULL && theMonitorAPI.pGetMonitorInfo ( hMonitor, &mi ) )
			rcMonitor = mi.rcMonitor;
	}

	return rcMonitor;
}

void xCreateFastFont ( CFont& pFont, INT nHeight, INT nWeight, CString nFontName )
{
	VERIFY ( pFont.CreateFont (
		nHeight,                   // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		nWeight,                   // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		CLEARTYPE_QUALITY,         // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		nFontName )
	);
}

BOOL xIsRunAsAdmin ( )
{
	BOOL bIsRunAsAdmin = FALSE;
	PSID pAdministratorsGroup = NULL;
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	if ( AllocateAndInitializeSid ( &NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &pAdministratorsGroup ) )
	{
		CheckTokenMembership ( NULL, pAdministratorsGroup, &bIsRunAsAdmin );
		FreeSid ( pAdministratorsGroup );
	}
	return bIsRunAsAdmin;
}

long int xGetUnixTime()
{
	time_t t = std::time(0);
	long int now = static_cast<long int> (t);
	return now;
}

CString xGetTime (int FormatType, CString DateTimeInfo)
{
	std::map <int, CString> CalendarMapMonth;
	CalendarMapMonth.insert(std::pair<int, CString>({ 1, L"������" }));
	CalendarMapMonth.insert(std::pair<int, CString>({ 2, L"�������" }));
	CalendarMapMonth.insert(std::pair<int, CString>({ 3, L"�����" }));
	CalendarMapMonth.insert(std::pair<int, CString>({ 4, L"������" }));
	CalendarMapMonth.insert(std::pair<int, CString>({ 5, L"���" }));
	CalendarMapMonth.insert(std::pair<int, CString>({ 6, L"����" }));
	CalendarMapMonth.insert(std::pair<int, CString>({ 7, L"����" }));
	CalendarMapMonth.insert(std::pair<int, CString>({ 8, L"�������" }));
	CalendarMapMonth.insert(std::pair<int, CString>({ 9, L"��������" }));
	CalendarMapMonth.insert(std::pair<int, CString>({ 10, L"�������" }));
	CalendarMapMonth.insert(std::pair<int, CString>({ 11, L"������" }));
	CalendarMapMonth.insert(std::pair<int, CString>({ 12, L"�������" }));

	std::map <int, CString> CalendarMapMonthFull;
	CalendarMapMonthFull.insert(std::pair<int, CString>({ 1, L"������" }));
	CalendarMapMonthFull.insert(std::pair<int, CString>({ 2, L"�������" }));
	CalendarMapMonthFull.insert(std::pair<int, CString>({ 3, L"����" }));
	CalendarMapMonthFull.insert(std::pair<int, CString>({ 4, L"������" }));
	CalendarMapMonthFull.insert(std::pair<int, CString>({ 5, L"���" }));
	CalendarMapMonthFull.insert(std::pair<int, CString>({ 6, L"����" }));
	CalendarMapMonthFull.insert(std::pair<int, CString>({ 7, L"����" }));
	CalendarMapMonthFull.insert(std::pair<int, CString>({ 8, L"������" }));
	CalendarMapMonthFull.insert(std::pair<int, CString>({ 9, L"��������" }));
	CalendarMapMonthFull.insert(std::pair<int, CString>({ 10, L"�������" }));
	CalendarMapMonthFull.insert(std::pair<int, CString>({ 11, L"������" }));
	CalendarMapMonthFull.insert(std::pair<int, CString>({ 12, L"�������" }));

	std::map <CString, CString> CalendarMapWeeks;
	CalendarMapWeeks.insert(std::pair<CString, CString>({ L"Monday", L"�����������" }));
	CalendarMapWeeks.insert(std::pair<CString, CString>({ L"Tuesday", L"�������" }));
	CalendarMapWeeks.insert(std::pair<CString, CString>({ L"Wednesday", L"�����" }));
	CalendarMapWeeks.insert(std::pair<CString, CString>({ L"Thursday", L"�������" }));
	CalendarMapWeeks.insert(std::pair<CString, CString>({ L"Friday", L"�������" }));
	CalendarMapWeeks.insert(std::pair<CString, CString>({ L"Saturday", L"�������" }));
	CalendarMapWeeks.insert(std::pair<CString, CString>({ L"Sunday", L"�����������" }));

	int DaysNonVYear[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int DaysVYear[] =    { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	CTime rawtime = CTime::GetCurrentTime();
	CString TimeInfo, TimeInfoTemp, str;
	int TimeInfoDay, TimeInfoMonth;

	if (FormatType == 0)
	{
		TimeInfoMonth = _wtoi(rawtime.Format(L"%m"));
		DateTimeInfo.Replace(L"%B", CalendarMapMonth[TimeInfoMonth]);
		DateTimeInfo.Replace(L"%A", CalendarMapWeeks[rawtime.Format(L"%A")]);

		TimeInfo = rawtime.Format(DateTimeInfo);
	}
	else if (FormatType == 1)
	{
		TimeInfo = xGetStrTime(_wtoi(rawtime.Format(L"%H")), L"h") + L" " + xGetStrTime(_wtoi(rawtime.Format(L"%M")), L"m");
	}
	else if (FormatType == 2 || FormatType == 3 || FormatType == 4)
	{
		DateTimeInfo = L"%A, %d %B";

		if (FormatType == 3)
			rawtime += CTimeSpan(1, 0, 0, 0);
		else if (FormatType == 4)
			rawtime += CTimeSpan(-1, 0, 0, 0);

		TimeInfoMonth = _wtoi(rawtime.Format(L"%m"));
		DateTimeInfo.Replace(L"%B", CalendarMapMonth[TimeInfoMonth]);
		DateTimeInfo.Replace(L"%A", CalendarMapWeeks[rawtime.Format(L"%A")]);
		TimeInfoDay = _wtoi(rawtime.Format(L"%d"));

		TimeInfo.Format(L"%d", TimeInfoDay);

		DateTimeInfo.Replace(L"%d", TimeInfo);

		if (FormatType == 2)
			TimeInfo = L"������� " + rawtime.Format(DateTimeInfo);
		else if (FormatType == 3)
			TimeInfo = L"������ ����� " + rawtime.Format(DateTimeInfo);
		else if (FormatType == 4)
			if (CalendarMapWeeks[rawtime.Format(L"%A")] == L"�����������"
				|| CalendarMapWeeks[rawtime.Format(L"%A")] == L"�������"
				|| CalendarMapWeeks[rawtime.Format(L"%A")] == L"�������")
				TimeInfo = L"����� ��� " + rawtime.Format(DateTimeInfo);
			else if(CalendarMapWeeks[rawtime.Format(L"%A")] == L"�����"
				|| CalendarMapWeeks[rawtime.Format(L"%A")] == L"�������"
				|| CalendarMapWeeks[rawtime.Format(L"%A")] == L"�������")
				TimeInfo = L"����� ���� " + rawtime.Format(DateTimeInfo);
			else
				TimeInfo = L"����� ���� " + rawtime.Format(DateTimeInfo);
	}
	else if (FormatType == 5)
	{
		TimeInfoMonth = _wtoi(rawtime.Format(L"%m"));
		TimeInfo = L"� ���� ������ "
			+ xGetStrTime(
				(
					xCheckIsLeapYear(_wtoi(rawtime.Format(L"%Y")))
						? DaysVYear[TimeInfoMonth - 1]
						: DaysNonVYear[TimeInfoMonth - 1])
				, L"d");
	}
	
	return TimeInfo;
}

int xGetDays(int month, int year)
{
	int days;
	switch (month) {
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			days = 31;
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			days = 30;
			break;
		case 2:
				days = xCheckIsLeapYear(year) ? 29 : 28;
			break;
		default:
			days = -1;
	}
	return days;
}

bool xCheckIsLeapYear(int year)
{
	return (year % 100 == 0) ? (year % 400 == 0) : (year % 4 == 0);
}

CString xGetStrTime(int intTime, CString TimeType)
{
	CString rTime, sTemp;

	CStringArray TimeSec; TimeSec.Add(L"�������"); TimeSec.Add(L"�������"); TimeSec.Add(L"������");
	CStringArray TimeMin; TimeMin.Add(L"������"); TimeMin.Add(L"������"); TimeMin.Add(L"�����");
	CStringArray TimeHour; TimeHour.Add(L"���"); TimeHour.Add(L"����"); TimeHour.Add(L"�����");
	CStringArray TimeDay; TimeDay.Add(L"����"); TimeDay.Add(L"���"); TimeDay.Add(L"����");
	CStringArray TimeYrs; TimeYrs.Add(L"���"); TimeYrs.Add(L"����"); TimeYrs.Add(L"���");

	CStringArray mapTimeType;

	if (TimeType == L"s")
		mapTimeType.Append(TimeSec);
	else if(TimeType == L"m")
		mapTimeType.Append(TimeMin);
	else if(TimeType == L"h")
		mapTimeType.Append(TimeHour);
	else if(TimeType == L"d")
		mapTimeType.Append(TimeDay);
	else if(TimeType == L"y")
		mapTimeType.Append(TimeYrs);

	switch ((intTime >= 20) ? intTime % 10 : intTime)
	{
		case 1:
			rTime += mapTimeType[0];
			break;
		case 2:
		case 3:
		case 4:
			rTime += mapTimeType[1];
			break;
		default:
			rTime += mapTimeType[2];
	}

	sTemp.Format(L"%d " + rTime, intTime);
	return sTemp;
}

void xInitEnvironment ( )
{
	TCHAR szPath[ 260 ];
	DWORD dwSize = 0;

	so.m_nVersion[ 0 ] = so.m_nVersion[ 1 ] = so.m_nVersion[ 2 ] = so.m_nVersion[ 3 ] = 0;

	if ( GetModuleFileName ( NULL, szPath, 260 ) )
	{
		so.m_sPath = szPath;
		dwSize = GetFileVersionInfoSize ( szPath, &dwSize );
	}

	if ( dwSize > 0 )
	{
		BYTE* pBuffer = new BYTE[ dwSize ];

		if ( GetFileVersionInfo ( szPath, NULL, dwSize, pBuffer ) )
		{
			VS_FIXEDFILEINFO* pTable;

			if ( VerQueryValue ( pBuffer, _T ( "\\" ), ( VOID** ) &pTable, ( UINT* ) &dwSize ) )
			{
				so.m_nVersion[ 0 ] = ( WORD ) ( pTable->dwFileVersionMS >> 16 );
				so.m_nVersion[ 1 ] = ( WORD ) ( pTable->dwFileVersionMS & 0xFFFF );
				so.m_nVersion[ 2 ] = ( WORD ) ( pTable->dwFileVersionLS >> 16 );
				so.m_nVersion[ 3 ] = ( WORD ) ( pTable->dwFileVersionLS & 0xFFFF );
			}
		}
		delete[ ] pBuffer;
	}
	so.m_sVersion.Format ( _T ( "%i.%i.%i build %i" ),
						   so.m_nVersion[ 0 ], so.m_nVersion[ 1 ], so.m_nVersion[ 2 ], so.m_nVersion[ 3 ] );
}

CString xMakeRand(int count)
{
	CString randStr, s;
	int r;
	srand((UINT)time(0));

	for (int i = 0; i < count; i++)
	{
		r = 1 + rand() % 9;
		s.Format(L"%d", r);
		randStr += s;
	}
	return randStr;
}

CString xMakeLower(CString str)
{
	str = str.MakeLower();
	CString strret = _T("");
	enum alphabet { eng, rus, no_one };
	for (int i = 0; i<str.GetLength(); i++) {
		int rez;
		wchar_t ch = static_cast<wchar_t> (str[i]);
		if (ch >= 65L && ch <= 90L) rez = eng;
		if (ch >= 1040L && ch <= 1071L) rez = rus;
		if (ch<65L || (ch>90L && ch <1040L) || ch>1071L) rez = no_one;
		switch (rez) {
			case eng: ch += 32; break;
			case rus: ch += 32; break;
			case no_one: ch = ch; break;
			default: break;
		}
		strret += ch;
	}
	return strret;
}
