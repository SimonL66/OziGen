#pragma once

#include "ProgressWindow.h"

enum DatabaseType {

	DB_Unknown,

	DB_OSGB_Tracklogs2,
	DB_OSGB_Tracklogs3,
	DB_OSGB_Local,

	DB_AT_Local,
	DB_BE_Local,

	DB_OSI_Local,
	DB_FR_Local,
	DB_DE_Local,
	DB_IT_Local,
	DB_LU_Local,
	DB_NL_Local,
	DB_OSNI_Local,
	DB_PT_Local,
	DB_ES_Local,
	DB_CH_Local,

	DB_PL_Local,
	DB_CZ_Local,
	DB_SI_Local,

	DB_CAT_Local,
	
	DB_SCO_Local,
};

void MakeDatabaseFolders(int nFlavour, int nCountry, int nScale);

bool ReadTracklogsTile(long minx, long maxx, long miny, long maxy, CProgressWindow& wndProgress, 
						unsigned char * bitmap_palette, long & bitmap_palette_length, 
						unsigned char * & bitmap_memory , long & bitmap_memory_size, 
						long & bitmap_width, long & dest_bitmap_width,
						long & bitmap_height, long & dest_bitmap_height,
						bool bUseTL3);

DatabaseType DetermineDatabaseType(int nDatabase, int nCountry);
