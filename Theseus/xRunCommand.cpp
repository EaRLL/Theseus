#include "stdafx.h"
#include "xRunCommand.h"
#include "Theseus.h"

bool xRunCommand_CheckAlses(CString OptAlses, CString CheckCommand)
{
	int i = 0;
	CString sInput = xMakeLower(OptAlses);
	CString sToken = L"";
	while (AfxExtractSubString(sToken, sInput, i, '|'))
	{
		if (xMakeLower(sToken) == xMakeLower(CheckCommand))
			return true;
		sToken.Empty();
		i++;
	}
	return false;
}

bool xRunCommand_GetValue(const std::map<CString, CString>& my_map, CString key, CString& out)
{
	/*typename*/ std::map<CString, CString>::const_iterator it = my_map.find(key);
	if (it != my_map.end())
	{
		out = it->second;
		return true;
	}
	return false;
}

CString xRunCommand_GetCommandsList(std::map<CString, CString>& my_map, CString key)
{
	CString _Command = xRunCommand_GetCommand(key, false);
	CString _CommandParams = xRunCommand_GetCommand(key, true);
	int countCommands = 0;
	CString CommandList = L"";
	CString OneCommand = L"";
	so.xRunCommand_FirstCommandText.Empty();

	for (std::map <CString, CString>::iterator it = my_map.begin(); it != my_map.end(); it++)
	{
		if (it->first.Left(_CommandParams.GetLength()) == _CommandParams)
		{
			countCommands++;
			OneCommand = it->first;
			CommandList = CommandList + _Command + L" " + it->first + L"\n\r";
			if (countCommands == 1)
				so.xRunCommand_FirstCommandText = _Command + L" " + OneCommand;
		}
	}

	return CommandList;
}

inline bool xRunCommand_FileExists(CString name1)
{
	boost::tregex regpos(L"^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\\?([^#]*))?(#(.*))?$");
	boost::tmatch matches;
	bool isMatchFound = boost::regex_match(name1, matches, regpos);

	if (isMatchFound)
		return true;
	else
	{
		std::string name = CW2A(name1.GetString()).m_psz;
		struct stat buffer;
		return (stat(name.c_str(), &buffer) == 0);
	}
}

CString xRunCommand_GetCommand(CString Command, bool returnParams)
{
	boost::regex regpos("(.[^ ]*)([ ]{1})(.*)");
	boost::smatch matches;

	std::string opStr = CW2A(Command.Trim().GetString()).m_psz;

	bool isMatchFound = boost::regex_match(opStr, matches, regpos);
	CString _Command;

	if (!returnParams)
		_Command = xMakeLower(CString(static_cast<std::string>(matches[1]).c_str()).Trim());
	else
		_Command = CString(static_cast<std::string>(matches[3]).c_str()).Trim();

	return _Command;
}

bool xRunCommand_DateTimeMatch(CString Command, CString &Response)
{
	boost::tmatch matches;

	if (boost::regex_match(Command, matches,
			boost::tregex(__string_xRunCommand_TimeCommand1)))
	{
		Response = xGetTime();
		return true;
	}
	else if (boost::regex_match(Command, matches,
		boost::tregex(__string_xRunCommand_TimeCommand2)))
	{
		Response = xGetTime(1);
		return true;
	}
	else if (boost::regex_match(Command, matches,
		boost::tregex(__string_xRunCommand_TimeCommand3)))
	{
		Response = xGetTime(2);
		return true;
	}
	else if (boost::regex_match(Command, matches,
		boost::tregex(__string_xRunCommand_TimeCommand4)))
	{
		Response = xGetTime(3);
		return true;
	}
	else if (boost::regex_match(Command, matches,
		boost::tregex(__string_xRunCommand_TimeCommand5)))
	{
		Response = xGetTime(4);
		return true;
	}
	else if (boost::regex_match(Command, matches,
		boost::tregex(__string_xRunCommand_TimeCommand6)))
	{
		Response = xGetTime(5);
		return true;
	}
	else
		return false;
}

bool xRunCommand_xRefreshCFG(CString Command, CString &Response)
{
	boost::tmatch matches;

	if (boost::regex_match(Command, matches,
		boost::tregex(so.OPT_APP_CONFIG_UPDATECFG_ALIASES)))
	{
		std::wifstream optFile(so.OPT_APP_CONFIG_FOLDER + so.OPT_APP_CONFIG_FILE);

		theApp.pMainFrame->l_TheseusResponseLine.SetWindowText(__string_xRunCommand_ParsingCFG);
		theApp.pMainFrame->RedrawWindow();

		if (optFile.is_open() && !optFile.fail())
		{
			const std::locale empty_locale = std::locale::empty();
			typedef std::codecvt_utf8<wchar_t> converter_type;
			const converter_type* converter = new converter_type;
			const std::locale utf8_locale = std::locale(empty_locale, converter);
			optFile.imbue(utf8_locale);
			so.F_OPT_LINES = _xInitalizeFile(optFile);
			_xParseCfg(so.F_OPT_LINES);

			optFile.close();
			Response = __string_xRunCommand_ParsingComplite;
			Response.Replace(L"%01", xGetTime(1));
		}
		else
			Response = __string_CSplashScreen_CfgNotFound;

		theApp.pMainFrame->RedrawWindow();
		return true;
	}
	else
		return false;
}

void xRunCommand(CString commandStr)
{
	CString cLine = commandStr;
	CString RESPONSE, _Command, _CommandOptions, _CommandOptionsParam = L"";
	int sLength = 0;

	cLine = xMakeLower(cLine.Trim());

	sLength = cLine.GetLength();

	if (sLength != 0 || !cLine.IsEmpty())
	{
		CString _AlCommandsBase = so.OPT_COMMANDALS_RUN + L"|" + so.OPT_COMMANDALS_INTSEARCH + L"|" + so.OPT_COMMANDALS_STOP;
		std::string __AlCommands = CW2A(_AlCommandsBase.GetString()).m_psz;
		boost::regex regpos("(" + __AlCommands + ")([ ]{1})(.*)"); //(.[^ ]*)([ ]{1})(.*)
		boost::smatch matches;

		std::string opStr = CW2A(cLine.GetString()).m_psz;

		bool isMatchFound =	boost::regex_match(opStr, matches, regpos);

		CString ClockResponse, RefreshResponse;

		if (cLine == L"cmd")
		{
			ShellExecute(NULL, L"open", L"c:\\Windows\\System32\\cmd.exe", NULL, NULL, SW_SHOWDEFAULT);
			RESPONSE = __string_xRunCommand_RunCMD;
		}
		else if (xRunCommand_DateTimeMatch(cLine, ClockResponse))
		{
			RESPONSE = ClockResponse;
		}
		else if (xRunCommand_xRefreshCFG(cLine, RefreshResponse))
		{
			RESPONSE = RefreshResponse;
		}
		else if (isMatchFound)
		{
			_Command = xMakeLower(CString(static_cast<std::string>(matches[1]).c_str()).Trim());
			_CommandOptions = CString(static_cast<std::string>(matches[3]).c_str()).Trim();

			if (xRunCommand_CheckAlses(so.OPT_COMMANDALS_RUN, _Command) && xRunCommand_GetValue(so.xOptions_Command_Run, _CommandOptions, _CommandOptionsParam))
			{

				int tpopEn = _CommandOptionsParam.Find(_T(".exe")) + 4;
				CString tpope = _CommandOptionsParam.Left(tpopEn); //COMMAND FILE
				CString tpdec = _CommandOptionsParam.Right(_CommandOptionsParam.GetLength() - tpopEn - 1); // COMMAND PARAMS

				HINSTANCE hOpenResult = ShellExecute(NULL, L"open", ( tpopEn <= 4 ? _CommandOptionsParam : tpope ), tpdec, NULL, SW_SHOWDEFAULT);
				INT_PTR OpenResult = (INT_PTR) hOpenResult;

				if (OpenResult <= HINSTANCE_ERROR) {
					switch (OpenResult) {
					case 0:
						RESPONSE = __string_xRunCommand_OpenResult0;
						break;
					case ERROR_FILE_NOT_FOUND:
						RESPONSE = __string_xRunCommand_OpenResult1;
						break;
					case ERROR_PATH_NOT_FOUND:
						RESPONSE = __string_xRunCommand_OpenResult2;
						break;
					case ERROR_BAD_FORMAT:
						RESPONSE = __string_xRunCommand_OpenResult3;
						break;
					case SE_ERR_ACCESSDENIED:
						RESPONSE = __string_xRunCommand_OpenResult4;
						break;
					case SE_ERR_ASSOCINCOMPLETE:
						RESPONSE = __string_xRunCommand_OpenResult5;
						break;
					case SE_ERR_DDEBUSY:
						RESPONSE = __string_xRunCommand_OpenResult6;
						break;
					case SE_ERR_DDEFAIL:
						RESPONSE = __string_xRunCommand_OpenResult7;
						break;
					case SE_ERR_DDETIMEOUT:
						RESPONSE = __string_xRunCommand_OpenResult8;
						break;
					case SE_ERR_DLLNOTFOUND:
						RESPONSE = __string_xRunCommand_OpenResult9;
						break;
					case SE_ERR_NOASSOC:
						RESPONSE = __string_xRunCommand_OpenResult10;
						break;
					case SE_ERR_OOM:
						RESPONSE = __string_xRunCommand_OpenResult11;
						break;
					case SE_ERR_SHARE:
						RESPONSE = __string_xRunCommand_OpenResult12;
						break;
					default:
						RESPONSE = __string_xRunCommand_OpenResult13;
					}
				}
				else {
					RESPONSE = __string_xRunCommand_OpenResultGood;
					RESPONSE.Replace(L"%01", _CommandOptions);

					int ii = 0;
					CString sToken, sToken2 = L"";
					while (AfxExtractSubString(sToken, __string_xRunCommand_ResultGoodReplace, ii, '|'))
					{
						if (sToken != L"")
						{
							int opEn = sToken.FindOneOf(_T(">"));
							sToken2 = sToken.Left(opEn);
							sToken = sToken.Right(sToken.GetLength() - opEn - 1);
							RESPONSE.Replace(sToken2, sToken);
						}
						else
							continue;
						ii++;
					}
				}
			} // END Command: RUN
			else if (xRunCommand_CheckAlses(so.OPT_COMMANDALS_STOP, _Command) && xRunCommand_GetValue(so.xOptions_Command_Stop, _CommandOptions, _CommandOptionsParam))
			{

				int tpopEn = _CommandOptionsParam.Find(_T(".exe")) + 4;
				CString tpope = _CommandOptionsParam.Left(tpopEn); //COMMAND FILE
				CString tpdec = _CommandOptionsParam.Right(_CommandOptionsParam.GetLength() - tpopEn - 1); // COMMAND PARAMS

				HINSTANCE hOpenResult = ShellExecute(NULL, L"open", (tpopEn <= 4 ? _CommandOptionsParam : tpope), tpdec, NULL, SW_SHOWDEFAULT);
				INT_PTR OpenResult = (INT_PTR)hOpenResult;

				if (OpenResult <= HINSTANCE_ERROR) {
					switch (OpenResult) {
					case 0:
						RESPONSE = __string_xRunCommand_OpenResult0;
						break;
					case ERROR_FILE_NOT_FOUND:
						RESPONSE = __string_xRunCommand_OpenResult1;
						break;
					case ERROR_PATH_NOT_FOUND:
						RESPONSE = __string_xRunCommand_OpenResult2;
						break;
					case ERROR_BAD_FORMAT:
						RESPONSE = __string_xRunCommand_OpenResult3;
						break;
					case SE_ERR_ACCESSDENIED:
						RESPONSE = __string_xRunCommand_OpenResult4;
						break;
					case SE_ERR_ASSOCINCOMPLETE:
						RESPONSE = __string_xRunCommand_OpenResult5;
						break;
					case SE_ERR_DDEBUSY:
						RESPONSE = __string_xRunCommand_OpenResult6;
						break;
					case SE_ERR_DDEFAIL:
						RESPONSE = __string_xRunCommand_OpenResult7;
						break;
					case SE_ERR_DDETIMEOUT:
						RESPONSE = __string_xRunCommand_OpenResult8;
						break;
					case SE_ERR_DLLNOTFOUND:
						RESPONSE = __string_xRunCommand_OpenResult9;
						break;
					case SE_ERR_NOASSOC:
						RESPONSE = __string_xRunCommand_OpenResult10;
						break;
					case SE_ERR_OOM:
						RESPONSE = __string_xRunCommand_OpenResult11;
						break;
					case SE_ERR_SHARE:
						RESPONSE = __string_xRunCommand_OpenResult12;
						break;
					default:
						RESPONSE = __string_xRunCommand_OpenResult13;
					}
				}
				else {
					RESPONSE = __string_xRunCommand_StopResultGood;
					RESPONSE.Replace(L"%01", _CommandOptions);

					int ii = 0;
					CString sToken, sToken2 = L"";
					while (AfxExtractSubString(sToken, __string_xRunCommand_ResultGoodReplace, ii, '|'))
					{
						if (sToken != L"")
						{
							int opEn = sToken.FindOneOf(_T(">"));
							sToken2 = sToken.Left(opEn);
							sToken = sToken.Right(sToken.GetLength() - opEn - 1);
							RESPONSE.Replace(sToken2, sToken);
						}
						else
							continue;
						ii++;
					}
				}
			} // END Command: STOP
			else if (xRunCommand_CheckAlses(so.OPT_COMMANDALS_INTSEARCH, _Command))
			{
				CString REST = L"";
				CString SearchUrl = so.xOptions_Command_InternetSearch[L"main_search"];
				CString SearchText = _CommandOptions;
				AfxExtractSubString(REST, _CommandOptions, 0, ' ');

				for (std::map <CString, CString>::iterator it = so.xOptions_Command_InternetSearch.begin(); it != so.xOptions_Command_InternetSearch.end(); it++)
				{
					if (it->first == REST || it->first + L" " == _CommandOptions.Left(it->first.GetLength() + 1))
					{
						SearchText = _CommandOptions.Right(_CommandOptions.GetLength() - it->first.GetLength() - 1);
						SearchUrl = it->second;
						break;
					}
				}

				SearchUrl.Replace(L"%search_word%", SearchText);
				ShellExecute(NULL, L"open", SearchUrl, NULL, NULL, SW_SHOWDEFAULT);
				RESPONSE = __string_xRunCommand_RunSearchMsg;
				RESPONSE.Replace(L"%01", (!SearchText.IsEmpty() ? L" " + __string_xRunCommand_RunSearchMsgAdd + L" \"" + SearchText + L"\"" : L""));
			} // END Command: SEARCH
			else
			{
				RESPONSE = __string_xRunCommand_CommandNotFound;
				RESPONSE.Replace(L"%01", cLine);
				theApp.pMainFrame->LearnCommandWindow(_CommandOptions);
			}
		}
		else
		{
			RESPONSE = __string_xRunCommand_CommandNotFound;
			RESPONSE.Replace(L"%01", cLine);
		}
		so.xRunCommand_LastCommandText = cLine;
		theApp.pMainFrame->l_TheseusResponseLine.SetWindowText(RESPONSE);
		theApp.pMainFrame->RedrawWindow();
	}
}

void xRunCommand_ShowCommandList(CString commandStr)
{
	if (xRunCommand_CheckAlses(so.OPT_COMMANDALS_RUN, xRunCommand_GetCommand(commandStr.Trim())))
	{
		CString CommandsList = xRunCommand_GetCommandsList(so.xOptions_Command_Run, commandStr);

		theApp.pMainFrame->l_TheseusResponseLine.SetWindowText(CommandsList);
		theApp.pMainFrame->RedrawWindow();
	}
	else if (xRunCommand_CheckAlses(so.OPT_COMMANDALS_STOP, xRunCommand_GetCommand(commandStr.Trim())))
	{
		CString CommandsList = xRunCommand_GetCommandsList(so.xOptions_Command_Stop, commandStr);

		theApp.pMainFrame->l_TheseusResponseLine.SetWindowText(CommandsList);
		theApp.pMainFrame->RedrawWindow();
	}
	else
	{
		theApp.pMainFrame->l_TheseusResponseLine.SetWindowText( L"" );
		theApp.pMainFrame->RedrawWindow();
	}
}
