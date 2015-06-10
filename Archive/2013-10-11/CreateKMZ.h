#pragma once

#include "Database.h"

//bool CreateKMZ(long minx, long maxx, long miny, long maxy, LPCSTR mapname, long tileWidth, long tileHeight, long trackSize, int nDatabase, int nDrawOrder);
bool DecMapKMZ(CString strPath, CString strFile, CRect MyRect, long tileWidth, long tileHeight, int nDatabase, int trackSize, int nDrawOrder);
