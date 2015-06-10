#pragma once

#include "TileDatabase.h"

int Kompass_LoadTiles(CTileDatabase* g_db, long minx, long miny, long maxx, long maxy, BOOL bForce, int nMapScale);

int Kompass_read_png_10K(long northing, long easting, CTileDatabase * db, unsigned char * bm_ptr, long bitmap_width);

bool Kompass_InDatabase(long northing, long easting);

bool Kompass_ReadTiles(long minx, long maxx, long miny, long maxy, CProgressWindow& wndProgress, 
						unsigned char * bitmap_palette, long & bitmap_palette_length, 
						unsigned char * & bitmap_memory , long & bitmap_memory_size, 
						long & bitmap_width, long & dest_bitmap_width,
						long & bitmap_height, long & dest_bitmap_height,
						int nMapScale);

