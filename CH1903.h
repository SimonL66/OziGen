#pragma once

void Tile2LatLon_NGI(double dNorthingTile, double dEastingTile, double* dLat, double* dLon);
void LatLon2Tile_NGI(double dLat, double dLon, double* dNorthingTile, double* dEastingTile);

void Tile2LatLon_CH1903(double dNorthingTile, double dEastingTile, double* dLat, double* dLon);
void LatLon2Tile_CH1903(double dLat, double dLon, double* dNorthingTile, double* dEastingTile);

bool DownloadTiles_CH1903(CTileDatabase* g_db, CRect BBox, long nDelta, bool bForce);

bool ReadTiles_CH1903(	CRect BBox, 
						CProgressWindow& wndProgress, 
						unsigned char * bitmap_palette, long & bitmap_palette_length, 
						unsigned char * & bitmap_memory , long & bitmap_memory_size, 
						long & bitmap_width, long & dest_bitmap_width,
						long & bitmap_height, long & dest_bitmap_height);

bool CH1903_ReadTiles_QCT(long y, long x, CTileDatabase * db, unsigned char * bitmap_offset, long bitmap_width);
