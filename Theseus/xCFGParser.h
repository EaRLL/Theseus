#pragma once

std::map <int, CString>     _xInitalizeFile(std::wifstream &optFile);
std::map <int, CString>     _xParseCfgText(std::map <int, CString> _s_CfgText);
void                        _xParseCfg(std::map <int, CString> _s_CfgText);

bool      xParseEntColor(CString opStr);
void      xParseColor(COLORREF &optSo, CString opStr);
void      xParseColor(CBrush &optSo, CString opStr);
bool      xRexpRGB(CString optSo);
