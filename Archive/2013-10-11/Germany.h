#pragma once

#include <direct.h>
#include <Wininet.h>

#include "TileDatabase.h"

int Kompass_read_png_10K(long northing, long easting, CTileDatabase * db, unsigned char * bm_ptr, long bitmap_width);

bool Kompass_InDatabase(long northing, long easting);

void Kompass_GetTile(HINTERNET hConnect, char * text, long northing, long easting, CTileDatabase * db, FILE * fp_log, BOOL bForce, int nMapScale);
int Kompass_LoadTiles(CTileDatabase* g_db, long minx, long miny, long maxx, long maxy, BOOL bForce, int nMapScale);


