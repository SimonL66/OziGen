#pragma once

#include "ProgressWindow.h"

#define	NUMBER_MAP_TYPES	3
#define	NUMBER_COUNTRY_MAPS	13
#define	NUMBER_MAPS_SCALES	6

enum DatabaseType {
	DB_Unknown,
	DB_OSGB_Tracklogs2,
	DB_OSGB_Tracklogs3,
	DB_OSGB_Local,
	DB_IGN_Local,
	DB_NGI_Local,
	DB_Kompass_Local,
};

/*
#define MAP_RASTER		0
#define MAP_VECTOR		1
#define MAP_STREET		2
*/

void MakeDatabaseFolders(int nFlavour, int nCountry, int nScale);

bool ReadTracklogsTile(long minx, long maxx, long miny, long maxy, CProgressWindow& wndProgress, 
						unsigned char * bitmap_palette, long & bitmap_palette_length, 
						unsigned char * & bitmap_memory , long & bitmap_memory_size, 
						long & bitmap_width, long & dest_bitmap_width,
						long & bitmap_height, long & dest_bitmap_height,
						bool bUseTL3);

DatabaseType DetermineDatabaseType(int nDatabase, int nCountry);

CString GetMapFlavourText(long nFlavour);
CString GetMapCountryText(long nCountry);
CString GetMapScaleText(long nScale);

long GetTilePixelWidth(long nFlavour, long nCountry, long nMapScale);
long GetTileMeterWidth(long nFlavour, long nCountry, long nMapScale);

long GetDatum(long nCountry);
long GetMapScale(long nMapScale);
long GetTileDatabaseScale(long nMapType);
double GetMetersPerPixel(long nMapType);
long GetTileWidth(long nMapType);
long GetPixelsPerTile(long nMapType);

long GetTileBitDepth(long nFlavour, long nCountry, long nMapScale);

long GetTileGFXType(long nCountry, long nMapScale);

int GetTileIterator(int nMapScale);

CString GetGridReferenceRegistryValues(long nCountry);

void AlignMapExtents(int nMapScale, long& minx, long& maxx, long& miny, long& maxy);

