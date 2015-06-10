#pragma once

BOOL ErrorOut(DWORD dError, TCHAR * szCallFunc);

int get_file_1(HINTERNET hConnect, LPCSTR pathname, LPCSTR tx_saveto);
int get_file(HINTERNET hConnect, LPCSTR pathname, LPCSTR tx_saveto);
