#pragma once

CString CreateOziPathName(CString strPath, CString strFile);
bool DumpOziFile(CString strFileName, CRect MyRect);

bool Create_OZI_PNG(LPCSTR mapname, long minx, long maxx, long miny, long maxy, int nDatabase);
