#pragma once

#include "Database.h"

bool CreateKMZ(long minx, long maxx, long miny, long maxy, LPCSTR mapname, long tileWidth, long tileHeight, long trackSize, DatabaseType nDatabase, int nDrawOrder, int nFlavour, int nCountry, int nScale);
bool DecMapKMZ(CString strPath, CString strFile, CRect MyRect, long tileWidth, long tileHeight, DatabaseType nDatabase, int trackSize, int nDrawOrder, int nFlavour, int nCountry, int nScale);
