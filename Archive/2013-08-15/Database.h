#pragma once

#include "ProgressWindow.h"

/*
#define	NUMBER_MAP_TYPES	3
#define	NUMBER_COUNTRY_MAPS	13
#define	NUMBER_MAPS_SCALES	10
*/

enum DatabaseType {
	DB_Unknown,
	DB_OSGB_Tracklogs2,
	DB_OSGB_Tracklogs3,
	DB_OSGB_Local,
	DB_IGN_Local,
	DB_NGI_Local,
	DB_Kompass_Local,
	DB_Spain_Local,
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
