#pragma once

void        xRunCommand                   (CString commandStr);
bool        xRunCommand_GetValue          (const std::map<CString, CString>& my_map, CString key, CString& out);
CString     xRunCommand_GetCommandsList   (const std::map<CString, CString>& my_map, CString key);
inline bool xRunCommand_FileExists        (CString name1);
CString     xRunCommand_GetCommand        (CString Command, bool returnParams = false);
void        xRunCommand_ShowCommandList   (CString commandStr);
bool        xRunCommand_DateTimeMatch     (CString Command, CString &Response);
bool        xRunCommand_xRefreshCFG       (CString Command, CString &Response);
