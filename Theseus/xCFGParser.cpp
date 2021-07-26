#include "stdafx.h"
#include "TheseusCore.h"
#include "xCFGParser.h"

bool LearnBaseAlrInc = false;

std::map <int, CString> _xInitalizeFile(std::wifstream &optFile)
{
	std::wstring _s_ReadLine;
	std::map <int, CString> _s_ReadText, _w_ReadText;
	CString sOptLine = _T("");
	int iOpt = 0;

	while (getline(optFile, _s_ReadLine))
	{
		sOptLine = _s_ReadLine.c_str();
		sOptLine.Trim();
		if (!sOptLine.IsEmpty())
		{
			_s_ReadText.insert(std::pair<int, CString>(iOpt, sOptLine));
			iOpt++;
		}
	}

	if (!LearnBaseAlrInc)
	{
		_s_ReadText.insert(std::pair<int, CString>(iOpt, L"@include( " + so.OPT_APP_CONFIG_LEARNBASE + L" );"));
		LearnBaseAlrInc = true;
	}

	_w_ReadText = _xParseCfgText(_s_ReadText);

	return _w_ReadText;
}

std::map <int, CString> _xParseCfgText(std::map <int, CString> _s_CfgText)
{

	CString sLine, str = L"";
	int sLength, _x_CommMidLeft, _x_CommMidRight = 0;
	std::map <int, CString> _sOF_CfgText;
	static bool _b_CommBlock, _b_CommLeftBl, _b_CommRightBl = false;

	for (std::map<int, CString>::iterator it = _s_CfgText.begin(); it != _s_CfgText.end(); ++it)
	{
		sLine = it->second;
		sLine.Trim();
		sLength = sLine.GetLength();

		if (sLength == 0 || sLine.IsEmpty() || sLine[0] == '#')
		{
			it->second = "";
		}
		else
		{

			const CString _x_RepComm = __T(";");
			sLine = boost::regex_replace(sLine, boost::tregex(L"\\;([ ]*)\\#(.*)"), _x_RepComm);

			str = sLine;
			const CString _x_RepEmpty = __T("");

			str = boost::regex_replace(str, boost::tregex(L"\"(.*)\""), _x_RepEmpty);
			str = boost::regex_replace(str, boost::tregex(L"\'(.*)\'"), _x_RepEmpty);

			str.Trim();

			const CString _x_RepCommSp = __T("");
			sLine = boost::regex_replace(sLine, boost::tregex(L"\\/\\*([^\\*\\/]*)\\*\\/"), _x_RepCommSp);

			for (int i = 0; i < str.GetLength(); i++)
			{
				if (str[i] == '/' && str[i + 1] == '*' && !_b_CommBlock)
				{
					for (int iR = i; iR < sLine.GetLength(); iR++)
					{
						if (sLine[i] == '/' && sLine[i + 1] == '*')
						{
							_b_CommBlock = true;
							_b_CommLeftBl = true;
							_x_CommMidLeft = i;
						}
					}
				}
				else if (str[i] == '*' && str[i + 1] == '/' && _b_CommBlock && _b_CommLeftBl)
				{
					for (int iR = i; iR < sLine.GetLength(); iR++)
					{
						if (sLine[i] == '*' && sLine[i + 1] == '/')
						{
							_b_CommBlock = true;
							_b_CommRightBl = true;
							_x_CommMidRight = i;
						}
					}
				}

			}

			if (_b_CommBlock && _b_CommLeftBl && !_b_CommRightBl)
			{
				sLine = sLine.Left(_x_CommMidLeft);
				_b_CommBlock = true;
				_b_CommLeftBl = true;
				_b_CommRightBl = false;
			}
			else if (_b_CommBlock && _b_CommLeftBl && _b_CommRightBl)
			{
				sLine = sLine.Right(_x_CommMidRight);
				_b_CommBlock = false;
				_b_CommLeftBl = false;
				_b_CommRightBl = false;
			}
			else if (_b_CommBlock && !_b_CommLeftBl && !_b_CommRightBl)
			{
				sLine.Empty();
				_b_CommBlock = true;
				_b_CommLeftBl = true;
				_b_CommRightBl = false;
			}

			it->second = sLine;
		}
	}

	int iOpt = 0;

	for (std::map<int, CString>::iterator it = _s_CfgText.begin(); it != _s_CfgText.end(); ++it)
	{
		if (it->second != "")
		{
			sLine = it->second;
			sLine.Trim();
			int strLen = sLine.GetLength();

			boost::tmatch xRexpIncludeMatch;

			// regexp Include File
			if (boost::regex_match(sLine, xRexpIncludeMatch, boost::tregex(so.OPT_REGEX_INC, boost::regex_constants::icase)))
			{
				const CString xStrIncFilePath = __T("$3");

				CString _x_str = boost::regex_replace(sLine, boost::tregex(so.OPT_REGEX_INC), xStrIncFilePath);
				_x_str = so.OPT_APP_CONFIG_FOLDER + _x_str.Trim();

				std::wifstream _x_optFile(_x_str);
				
				if (GetFileAttributes(_x_str) != INVALID_FILE_ATTRIBUTES)
				{
					const std::locale empty_locale = std::locale::empty();
					typedef std::codecvt_utf8<wchar_t> converter_type;
					const converter_type* converter = new converter_type;
					const std::locale utf8_locale = std::locale(empty_locale, converter);
					_x_optFile.imbue(utf8_locale);
					std::map<int, CString> _x_IncFile = _xInitalizeFile(_x_optFile);
					_x_optFile.close();
					int _x_i = 0;
					for (std::map<int, CString>::iterator itSub = _x_IncFile.begin(); itSub != _x_IncFile.end(); ++itSub)
					{
						_sOF_CfgText.insert(std::pair<int, CString>(iOpt++, itSub->second));
					}
				}
			}
			else if (sLine[0] == '('  && sLine[strLen - 1] == '{')
			{
				CString varBlockName = sLine;

				varBlockName.Remove('(');
				varBlockName.Remove('{');
				varBlockName.Remove(' ');
				varBlockName.Remove('"');

				so.x_bParBlock.insert(std::pair<int, CString>((int)(so.x_bParBlock.size()), varBlockName));
			}
			else if (sLine[0] == '}'  && sLine[strLen - 1] == ')')
			{
				int x_varBlId = (int)(so.x_bParBlock.size()) - 1;
				so.x_bParBlock.erase(x_varBlId);
			}
			else
			{
				int x_varBlCount = (int)(so.x_bParBlock.size());
				if (x_varBlCount > 0 && sLine[0] == '[')
				{
					CString x_spVarBlock = L"";
					CString x_spasBlock = L"";

					for (std::map<int, CString>::iterator itVarBl = so.x_bParBlock.begin(); itVarBl != so.x_bParBlock.end(); ++itVarBl)
					{
						x_spVarBlock = x_spVarBlock + x_spasBlock + itVarBl->second;
						x_spasBlock = "_";
					}

					CString x_Var = sLine.Right(strLen - 1);

					_sOF_CfgText.insert(std::pair<int, CString>(iOpt++, L"[" + x_spVarBlock + x_spasBlock + x_Var));
				}
				else
				{
					_sOF_CfgText.insert(std::pair<int, CString>(iOpt++, it->second));
				}
			}
		}

		continue;

	}

	return _sOF_CfgText;

}

void _xParseCfg(std::map <int, CString> _s_CfgText)
{
	LearnBaseAlrInc = false;

	so.x_bParBlock.clear();
	so.xOptions_Command_Run.clear();
	so.xOptions_Command_InternetSearch.clear();
	so.xOptions_Command_Stop.clear();

	CString str, ope, dec = _T("");

	bool commBlock = false;
	bool varBlock = false;

	boost::tmatch xRexpMatch;

	for (std::map <int, CString>::iterator it = _s_CfgText.begin(); it != _s_CfgText.end(); ++it)
	{
		str = it->second;
		str.Trim();

		if (str[0] == '['  && !str.IsEmpty())
		{
			int strLen = str.GetLength();
			int opEn, i = 0;

			bool elFndRst = false;
			bool elFrstLt = false;

			int opEnRSt = 0;

			for (int iR = 0; iR < str.GetLength() - 1; iR++)
			{
				dec = str[iR];

				if (dec == ';')
					elFrstLt = true;
				else if (elFrstLt && dec == '#')
				{
					str = str.Left(iR - 1);
					str.Trim();
					strLen = str.GetLength();
					elFndRst = true;
					break;
				}
			}

			if (str[strLen - 1] == ';')
			{
				opEn = str.FindOneOf(_T("]"));
				ope = str.Left(opEn);

				int oLPos = 0;
				dec = str[ope.GetLength()];
				bool elFnd = false;
				do
				{
					oLPos = ope.GetLength() + i;
					dec = str[oLPos];

					if (dec == ']' || dec == ' ' || dec == '=' || dec == ':' || dec == '-')
					{
						i++;
						elFnd = false;
					}
					else
						elFnd = true;
				} while (elFnd != true);

				dec = str.Right(str.GetLength() - oLPos);

				ope = ope.Right(ope.GetLength() - 1);
				dec = dec.Left(dec.GetLength() - 1);

				ope.Trim();  dec.Trim();

				ope.MakeUpper();
				ope.Replace('.', '_');
				ope.Replace('/', '_');

				if (boost::regex_match(ope, xRexpMatch, boost::tregex(L"(RUN|SEARCH|STOP)_(.*)", boost::regex_constants::icase)))
				{

					CString sInputAls = dec;
					CString sTokenAls = L"";
					CString sResultAls = L"";
					int i = 0;
					while (AfxExtractSubString(sTokenAls, sInputAls, i, '|'))
					{
						sTokenAls = xMakeLower(sTokenAls);

						boost::smatch xSRexpMatchAls;
						std::string sSTokenAls = CW2A(sTokenAls.Trim().GetString());

						CString sTokenResultCommand, sTokenResultParams = L"";

						if (boost::regex_match(sSTokenAls, xSRexpMatchAls, boost::regex("(.[^(]*)(.*)(.[)]*)", boost::regex_constants::icase)))
						{
							sTokenResultCommand = xMakeLower(CString(static_cast<std::string>(xSRexpMatchAls[1]).c_str()).Trim());
							sTokenResultParams = xMakeLower(CString(static_cast<std::string>(xSRexpMatchAls[2]).c_str()).Trim());

							sTokenResultParams = sTokenResultParams.Right(sTokenResultParams.GetLength() - 1);

							if (!sTokenResultCommand.IsEmpty() && !sTokenResultParams.IsEmpty())
							{
								CString sRSubParams = L"";
								int R = 0;
								while (AfxExtractSubString(sRSubParams, sTokenResultParams, R, '!'))
								{
									sResultAls += (R >= 1 || i >= 1 ? L"|" : L"") + sTokenResultCommand + L" " + sRSubParams;
									sRSubParams.Empty();
									R++;
								}
							}
							else
								sResultAls += (i >= 1 ? L"|" : L"") + sTokenAls;
						}
						else
							sResultAls += (i >= 1 ? L"|" : L"") + sInputAls;

						sTokenAls.Empty();
						i++;
					}

					if (xMakeLower(ope) == L"run_aliases")
						so.OPT_COMMANDALS_RUN = sResultAls;
					else if (xMakeLower(ope) == L"search_aliases")
						so.OPT_COMMANDALS_INTSEARCH = sResultAls;
					else if (xMakeLower(ope) == L"stop_aliases")
						so.OPT_COMMANDALS_STOP = sResultAls;
					else
					{
						CString sInput = xMakeLower(ope);
						CString sCommandBase = L"";
						if (sInput.Left(3) == L"run")
						{
							sCommandBase = L"run";
							sInput = sInput.Right(ope.GetLength() - 4);
						}
						else if (sInput.Left(6) == L"search")
						{
							sCommandBase = L"search";
							sInput = sInput.Right(ope.GetLength() - 7);
						}
						else if (sInput.Left(4) == L"stop")
						{
							sCommandBase = L"stop";
							sInput = sInput.Right(ope.GetLength() - 5);
						}

						CString sToken = L"";
						int i = 0;
						while (AfxExtractSubString(sToken, sInput, i, '|'))
						{
							sToken = xMakeLower(sToken);
							
							boost::smatch xSRexpMatch;
							std::string sSToken = CW2A(sToken.Trim().GetString());

							CString sTokenResultCommand, sTokenResultParams = L"";

							if (boost::regex_match(sSToken, xSRexpMatch, boost::regex("(.[^(]*)(.*)(.[)]*)", boost::regex_constants::icase)))
							{
								sTokenResultCommand = xMakeLower(CString(static_cast<std::string>(xSRexpMatch[1]).c_str()).Trim());
								sTokenResultParams = xMakeLower(CString(static_cast<std::string>(xSRexpMatch[2]).c_str()).Trim());

								sTokenResultParams = sTokenResultParams.Right(sTokenResultParams.GetLength() - 1);

								if (!sTokenResultCommand.IsEmpty() && !sTokenResultParams.IsEmpty())
								{
									CString sRSubParams = L"";
									int R = 0;
									while (AfxExtractSubString(sRSubParams, sTokenResultParams, R, '!'))
									{
										if (sCommandBase == L"run")
											so.xOptions_Command_Run.insert(std::make_pair(sTokenResultCommand + L" " + sRSubParams, dec));
										else if (sCommandBase == L"search")
											so.xOptions_Command_InternetSearch.insert(std::make_pair(sTokenResultCommand + L" " + sRSubParams, dec));
										else if (sCommandBase == L"stop")
											so.xOptions_Command_Stop.insert(std::make_pair(sTokenResultCommand + L" " + sRSubParams, dec));
										sRSubParams.Empty();
										R++;
									}
								}
								else
									if (sCommandBase == L"run")
										so.xOptions_Command_Run.insert(std::make_pair(sToken, dec));
									else if (sCommandBase == L"search")
										so.xOptions_Command_InternetSearch.insert(std::make_pair(sToken, dec));
									else if (sCommandBase == L"stop")
										so.xOptions_Command_Stop.insert(std::make_pair(sToken, dec));
							}
							else
								if (sCommandBase == L"run")
									so.xOptions_Command_Run.insert(std::make_pair(sToken, dec));
								else if (sCommandBase == L"search")
									so.xOptions_Command_InternetSearch.insert(std::make_pair(sToken, dec));
								else if (sCommandBase == L"stop")
									so.xOptions_Command_Stop.insert(std::make_pair(sToken, dec));

							sToken.Empty();
							i++;
						}
					}
				}
				else
				{
					if (ope == "SKIN_COLOR_APP_BG" && xRexpRGB(dec))
						xParseColor(so.SKIN_COLOR_APP_BG, dec);
					else if (ope == "SKIN_COLOR_APP_BORDER" && xRexpRGB(dec))
						xParseColor(so.SKIN_COLOR_APP_BORDER, dec);
					else if (ope == "SKIN_COLOR_SPLASH_BG" && xRexpRGB(dec))
						xParseColor(so.SKIN_COLOR_SPLASH_BG, dec);
					else if (ope == "SKIN_COLOR_SPLASH_TEXT" && xRexpRGB(dec))
						xParseColor(so.SKIN_COLOR_SPLASH_TEXT, dec);
					else if (ope == "SKIN_COLOR_COMMANDLINE_BG" && xRexpRGB(dec))
						xParseColor(so.SKIN_COLOR_COMMANDLINE_BG, dec);
					else if (ope == "SKIN_COLOR_COMMANDLINE_TEXT" && xRexpRGB(dec))
						xParseColor(so.SKIN_COLOR_COMMANDLINE_TEXT, dec);
					else if (ope == "SKIN_COLOR_CMDRESPONSELINE_BG" && xRexpRGB(dec))
						xParseColor(so.SKIN_COLOR_CMDRESPONSELINE_BG, dec);
					else if (ope == "SKIN_COLOR_CMDRESPONSELINE_TEXT" && xRexpRGB(dec))
						xParseColor(so.SKIN_COLOR_CMDRESPONSELINE_TEXT, dec);

					else if (ope == "SKIN_COLOR_FBTN_BORDER_NORMAL" && xRexpRGB(dec))
						xParseColor(so.SKIN_COLOR_FBTN_BORDER_NORMAL, dec);
					else if (ope == "SKIN_COLOR_FBTN_FILL_NORMAL" && xRexpRGB(dec))
						xParseColor(so.SKIN_COLOR_FBTN_FILL_NORMAL, dec);
					else if (ope == "SKIN_COLOR_FBTN_TEXT_NORMAL" && xRexpRGB(dec))
						xParseColor(so.SKIN_COLOR_FBTN_TEXT_NORMAL, dec);
					else if (ope == "SKIN_COLOR_FBTN_BORDER_HOVER" && xRexpRGB(dec))
						xParseColor(so.SKIN_COLOR_FBTN_BORDER_HOVER, dec);
					else if (ope == "SKIN_COLOR_FBTN_FILL_HOVER" && xRexpRGB(dec))
						xParseColor(so.SKIN_COLOR_FBTN_FILL_HOVER, dec);
					else if (ope == "SKIN_COLOR_FBTN_TEXT_HOVER" && xRexpRGB(dec))
						xParseColor(so.SKIN_COLOR_FBTN_TEXT_HOVER, dec);
					else if (ope == "SKIN_COLOR_FBTN_BORDER_CDOWN" && xRexpRGB(dec))
						xParseColor(so.SKIN_COLOR_FBTN_BORDER_CDOWN, dec);
					else if (ope == "SKIN_COLOR_FBTN_FILL_CDOWN" && xRexpRGB(dec))
						xParseColor(so.SKIN_COLOR_FBTN_FILL_CDOWN, dec);
					else if (ope == "SKIN_COLOR_FBTN_TEXT_CDOWN" && xRexpRGB(dec))
						xParseColor(so.SKIN_COLOR_FBTN_TEXT_CDOWN, dec);

					else if (ope == "SKIN_COLOR_FBTN_BORDER_CHCKD_NORMAL" && xRexpRGB(dec))
						xParseColor(so.SKIN_COLOR_FBTN_BORDER_CHCKD_NORMAL, dec);
					else if (ope == "SKIN_COLOR_FBTN_FILL_CHCKD_NORMAL" && xRexpRGB(dec))
						xParseColor(so.SKIN_COLOR_FBTN_FILL_CHCKD_NORMAL, dec);
					else if (ope == "SKIN_COLOR_FBTN_TEXT_CHCKD_NORMAL" && xRexpRGB(dec))
						xParseColor(so.SKIN_COLOR_FBTN_TEXT_CHCKD_NORMAL, dec);
					else if (ope == "SKIN_COLOR_FBTN_BORDER_CHCKD_HOVER" && xRexpRGB(dec))
						xParseColor(so.SKIN_COLOR_FBTN_BORDER_CHCKD_HOVER, dec);
					else if (ope == "SKIN_COLOR_FBTN_FILL_CHCKD_HOVER" && xRexpRGB(dec))
						xParseColor(so.SKIN_COLOR_FBTN_FILL_CHCKD_HOVER, dec);
					else if (ope == "SKIN_COLOR_FBTN_TEXT_CHCKD_HOVER" && xRexpRGB(dec))
						xParseColor(so.SKIN_COLOR_FBTN_TEXT_CHCKD_HOVER, dec);
					else if (ope == "SKIN_COLOR_FBTN_BORDER_CHCKD_CDOWN" && xRexpRGB(dec))
						xParseColor(so.SKIN_COLOR_FBTN_BORDER_CHCKD_CDOWN, dec);
					else if (ope == "SKIN_COLOR_FBTN_FILL_CHCKD_CDOWN" && xRexpRGB(dec))
						xParseColor(so.SKIN_COLOR_FBTN_FILL_CHCKD_CDOWN, dec);
					else if (ope == "SKIN_COLOR_FBTN_TEXT_CHCKD_CDOWN" && xRexpRGB(dec))
						xParseColor(so.SKIN_COLOR_FBTN_TEXT_CHCKD_CDOWN, dec);

					else if (ope == "OPT_APP_SHOWPOS" && !dec.IsEmpty())
						so.OPT_APP_SHOWPOS = dec;
					else if (ope == "OPT_APP_CONFIG_LEARNBASE" && !dec.IsEmpty())
						so.OPT_APP_CONFIG_LEARNBASE = dec;
					else if (ope == "OPT_APP_CONFIG_RECORD_STAYONTOP" && !dec.IsEmpty())
						so.OPT_APP_CONFIG_RECORD_STAYONTOP = (dec == L"1" ? 1 : 0);
					else if (ope == "OPT_APP_CONFIG_UPDATECFG_ALIASES" && !dec.IsEmpty())
						so.OPT_APP_CONFIG_UPDATECFG_ALIASES = dec;
					else if (ope == "OPT_APP_CONFIG_MAGICKEY" && !dec.IsEmpty())
						so.OPT_APP_CONFIG_MAGICKEY = dec;
				}
			}
			continue;
		}
		else
			continue;
	}
}

int optColorIPosR, optColorIPosG, optColorIPosB;

bool xParseEntColor(CString opStr)
{
	int opEn1, oLPos, oLCols, i;
	CString op1, pos1, pos2, pos3;

	opStr.Trim();

	if (opStr[0] == '#')
	{
		CString xHexColor = opStr;
		CString xRGBColor = L"";

		int xHEXr, xHEXg, xHEXb = 0;

		xHexColor.Trim();

		std::string sRGBColor;

		xRGBColor = xHexColor;
		xRGBColor.Remove('#');
		xRGBColor.Remove(' ');

		sRGBColor = CW2A(xRGBColor.GetString());

		std::istringstream(sRGBColor.substr(0, 2)) >> std::hex >> xHEXr;
		std::istringstream(sRGBColor.substr(2, 2)) >> std::hex >> xHEXg;
		std::istringstream(sRGBColor.substr(4, 2)) >> std::hex >> xHEXb;

		xRGBColor.Format(_T("%d, %d, %d"), xHEXr, xHEXg, xHEXb);

		optColorIPosR = xHEXr;
		optColorIPosG = xHEXg;
		optColorIPosB = xHEXb;
	}
	else
	{
		opEn1 = opStr.FindOneOf(L"(");
		op1 = opStr.Right(opStr.GetLength() - opEn1 - 1);
		op1 = op1.Left(op1.GetLength() - 1);
		op1.Trim();

		oLPos = op1.GetLength() - 1;

		oLCols = 1;

		for (i = 0; i <= oLPos; i++)
		{
			if (op1[i] != ' ' && op1[i] != ',')
			{
				if (oLCols == 1)
					pos1 = pos1 + op1[i];
				else if (oLCols == 2)
					pos2 = pos2 + op1[i];
				else if (oLCols == 3)
					pos3 = pos3 + op1[i];
				continue;
			}
			else if (op1[i] == ',')
			{
				oLCols++;
				continue;
			}
		}

		optColorIPosR = _ttoi(pos1);
		optColorIPosG = _ttoi(pos2);
		optColorIPosB = _ttoi(pos3);
	}

	return true;
}

void xParseColor(COLORREF &optSo, CString opStr)
{
	if (xParseEntColor(opStr))
	{
		if (optColorIPosR >= 0 && optColorIPosG >= 0 && optColorIPosB >= 0)
			optSo = RGB(optColorIPosR, optColorIPosG, optColorIPosB);
	}
}

void xParseColor(CBrush &optSo, CString opStr)
{
	if (xParseEntColor(opStr))
	{
		optSo.Detach();
		optSo.CreateSolidBrush(RGB((COLORREF)optColorIPosR, (COLORREF)optColorIPosG, (COLORREF)optColorIPosB));
	}
}

bool xRexpRGB(CString optSo)
{
	boost::tmatch xRexpRGBMatch;
	if (boost::regex_match(optSo, xRexpRGBMatch, boost::tregex(so.OPT_REGEX_RGB, boost::regex_constants::icase)))
		return true;
	else if (boost::regex_match(optSo, xRexpRGBMatch, boost::tregex(so.OPT_REGEX_HEX, boost::regex_constants::icase)))
		return true;
	else
		return false;
}
