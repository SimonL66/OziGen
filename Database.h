#pragma once

#include "ProgressWindow.h"

bool ReadTracklogsTile(long minx, long maxx, long miny, long maxy, CProgressWindow& wndProgress, 
						unsigned char * bitmap_palette, long & bitmap_palette_length, 
						unsigned char * & bitmap_memory , long & bitmap_memory_size, 
						long & bitmap_width, long & dest_bitmap_width,
						long & bitmap_height, long & dest_bitmap_height,
						bool bUseTL3);

//DatabaseType DetermineDatabaseType(int nDatabase, int nCountry);
