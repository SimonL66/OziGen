#pragma once

#include <direct.h>
#include <Wininet.h>

#include "TileDatabase.h"

BOOL ErrorOut(DWORD dError, TCHAR * szCallFunc);

int get_file_1(HINTERNET hConnect, LPCSTR pathname, LPCSTR tx_saveto);

int wmsGetFile(HINTERNET hConnect, LPCSTR pathname, LPCSTR tx_saveto);

int wmsLoadTiles(CTileDatabase* g_db, long minx, long miny, long maxx, long maxy, BOOL bForce, int nMapScale);

void wmsGetTile(HINTERNET hConnect, char * text, long northing, long easting, CTileDatabase * db, FILE * fp_log, BOOL bForce, int nMapScale);

