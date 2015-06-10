#pragma once

int read_jpg(long northing, long easting, CTileDatabase * db, char * szFile, unsigned char * bm_ptr, long bitmap_width);
int read_png(long northing, long easting, CTileDatabase * db, unsigned char * bm_ptr, long bitmap_width);
