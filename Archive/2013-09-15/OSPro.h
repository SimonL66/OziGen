#pragma once

#include "TileDatabase.h"
#include "ProgressWindow.h"

int read_png(long northing, long easting, CTileDatabase * db, unsigned char * bm_ptr, long bitmap_width);

int read_png_10K(long northing, long easting, CTileDatabase * db, unsigned char * bm_ptr, long bitmap_width);

void Create_PNG_JPR(LPCSTR mapname, long minx, long maxx, long miny, long maxy);

int OSPro_LoadTiles(CTileDatabase* g_db, long minx, long miny, long maxx, long maxy, BOOL bForce, int nMapFlavour, int nMapScale);

bool IsInOSGBDatabase(long northing, long easting);

bool OSPro_ReadTiles(long minx, long maxx, long miny, long maxy, CProgressWindow& wndProgress, 
						unsigned char * bitmap_palette, long & bitmap_palette_length, 
						unsigned char * & bitmap_memory , long & bitmap_memory_size, 
						long & bitmap_width, long & dest_bitmap_width,
						long & bitmap_height, long & dest_bitmap_height,
						int nFlavour, int nCountry, int nScale);

bool OSPro_ReadTiles_QCT(long y, long x, CTileDatabase * db, unsigned char * bitmap_offset, long bitmap_width);

