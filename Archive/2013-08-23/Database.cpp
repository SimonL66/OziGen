#include "stdafx.h"

#include "OziGen.h"
#include "OziGenDlg.h"
#include "func.h"
#include "jpeg.h"

#include "OSPro.h"
#include "IGN.h"
#include "NGI.h"

#include "GFX.h"

#include "stdint.h"

#include "Database.h"

#include "CustomMap.h"

#include "jnx.h"

#include <math.h>
#include <list>
#include <gdiplus.h>
#include <direct.h>

#include "..\MapLib\CoordConv.h"
#include "..\MapLib\mmmath.h"
#include "..\MapLib\png.h"
#include "..\MapLib\Palette.h"
#include "..\MapLib\map_conv.h"
#include "..\MapLib\License.h"
#include "..\MapLib\db_TL3.h"
#include "..\MapLib\zip.h"

#include "TracklogsDatabase.h"

#define printf visualprintf

CString g_strMapDatabaseFolder="C:\\OziGenDatabase";
CString g_strMapFlavour="Raster";
CString g_strMapCountry="dbOSGB";
CString g_strMapScale="25K";

int g_nMapDatabase;

CString g_strDatabasePath;

/*
char* MapScales[] = {
	"10K",
	"25K",
	"50K",
	"100K",
	"200K",

	"250K",

	"500K",
	"1000K",
	"2000K",

	"5000K",
	"OV"
};

char* MapFlavours[] = {
	"Raster",
	"Vector",
	"Street",
};

int nMapScales[] = {

	 10000,		// 1:10K
	 25000,		// 1:25K
	 50000,		// 1:50K
	100000,		// 1:100K
	250000,		// 1:250K
};

int nMapDatum[NUMBER_COUNTRY_MAPS] = {

	MAP_DATUM_AT,
	MAP_DATUM_NGI,
	MAP_DATUM_OSGB,
	MAP_DATUM_OSi,
	MAP_DATUM_IGN,
	MAP_DATUM_DE,
	MAP_DATUM_IT,
	MAP_DATUM_LUX,
	MAP_DATUM_NL,
	MAP_DATUM_OSNi,
	MAP_DATUM_PT,
	MAP_DATUM_ES,
	MAP_DATUM_CH,
};

char* Countries[NUMBER_COUNTRY_MAPS] = {
	"dbAT",
	"dbNGI", 
	"dbOSGB",
	"dbOSi",
	"dbIGN",
	"dbDE",
	"dbIT",
	"dbLux",
	"dbNL"
	"dbOSNi",
	"dbPT",
	"dbES",
	"dbCH",
};

char* GridReferenceRegistryValues[NUMBER_COUNTRY_MAPS] = {
	"GridReferenceAT",
	"GridReferenceNGI",
	"GridReferenceOSGB",
	"GridReferenceOSi",
	"GridReferenceIGN",
	"GridReferenceDE",
	"GridReferenceLux",
	"GridReferenceNL",
	"GridReferenceOSNi",
	"GridReferencePT",
	"GridReferenceES",
	"GridReferenceCH",
};

CString GetGridReferenceRegistryValues(long nCountry)
{
	CString str = GridReferenceRegistryValues[nCountry];
	return str;
}

int number_of_pixels_per_tile[NUMBER_MAP_TYPES][NUMBER_COUNTRY_MAPS][NUMBER_MAPS_SCALES] = {

//	Raster Map:
//	10K		25K		50K		100K	200K	250K	500K	1000K	2000K	5000k			Country
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Austria
	250,	-1,		200,	-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Belgium
	250,	200,	200,	-1,		-1,		200,	-1,		-1,		-1,		-1,			//	Britain
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Eire
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	France
	250,	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Germany
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Italy
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Luxembourg
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Netherlands
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Northern Ireland
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Portugal
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Spain
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Switzerland

//	Vector Map:
//	10K		25K		50K		100K	200K	250K	500K	1000K	2000K	5000k			Country
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Austria
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Belgium
	250,	200,	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Britain
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Eire
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	France
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Germany
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Italy
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Luxembourg
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Netherlands
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Northern Ireland
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Portugal
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Spain
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Switzerland

//	Street Map:
//	10K		25K		50K		100K	200K	250K	500K	1000K	2000K	5000k			Country
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Austria
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Belgium
	250,	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Britain
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Eire
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	France
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Germany
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Italy
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Luxembourg
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Netherlands
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Northern Ireland
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Portugal
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Spain
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Switzerland
};

long GetTilePixelWidth(long nFlavour, long nCountry, long nScale)
{
	return number_of_pixels_per_tile[nFlavour][nCountry][nScale];
}

int number_of_meters_per_tile[NUMBER_MAP_TYPES][NUMBER_COUNTRY_MAPS][NUMBER_MAPS_SCALES] = {

//	Raster Map:
//	10K		25K		50K		100K	200K	250K	500K	1000K	2000K	5000k			Country
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Austria
	250,	-1,		1000,	-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Belgium
	250,	500,	1000,	-1,		-1,		5000,	-1,		-1,		-1,		-1,			//	Britain
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Eire
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	France
	250,	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Germany
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Italy
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Luxembourg
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Netherlands
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Northern Ireland
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Portugal
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Spain
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Switzerland

//	Vector Map:
//	10K		25K		50K		100K	200K	250K	500K	1000K	2000K	5000k			Country
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Austria
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Belgium
	250,	500,	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Britain
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Eire
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	France
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Germany
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Italy
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Luxembourg
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Netherlands
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Northern Ireland
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Portugal
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Spain
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Switzerland

//	Street Map:
//	10K		25K		50K		100K	200K	250K	500K	1000K	2000K	5000k			Country
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Austria
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Belgium
	250,	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Britain
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Eire
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	France
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Germany
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Italy
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Luxembourg
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Netherlands
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Northern Ireland
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Portugal
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Spain
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Switzerland
};

long GetTileMeterWidth(long nFlavour, long nCountry, long nScale)
{
	return number_of_meters_per_tile[nFlavour][nCountry][nScale];
}

int bit_depth_per_tile[NUMBER_MAP_TYPES][NUMBER_COUNTRY_MAPS][NUMBER_MAPS_SCALES] = {

//	Raster Map:
//	10K		25K		50K		100K	200K	250K	500K	1000K	2000K	5000k			Country
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Austria
	24,		-1,		24,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Belgium
	24,		 8,		 8,		-1,		-1,		 8,		-1,		-1,		-1,		-1,			//	Britain
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Eire
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	France
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Germany
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Italy
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Luxembourg
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Netherlands
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Northern Ireland
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Portugal
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Spain
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Switzerland

//	Vector Map:
//	10K		25K		50K		100K	200K	250K	500K	1000K	2000K	5000k			Country
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Austria
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Belgium
	32,		 8,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Britain
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Eire
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	France
	32,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Germany
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Italy
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Luxembourg
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Netherlands
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Northern Ireland
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Portugal
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Spain
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Switzerland

//	Street Map:
//	10K		25K		50K		100K	200K	250K	500K	1000K	2000K	5000k			Country
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Austria
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Belgium
	 8,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Britain
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Eire
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	France
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Germany
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Italy
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Luxembourg
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Netherlands
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Northern Ireland
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Portugal
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Spain
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Switzerland
};
long GetTileBitDepth(long nFlavour, long nCountry, long nMapScale)
{
	return bit_depth_per_tile[nFlavour][nCountry][nMapScale];
}

#define PNG 1
#define JPG 2
#define BMP 3

long gfx_type_per_tile[NUMBER_COUNTRY_MAPS][NUMBER_MAPS_SCALES] = {

//	10K		25K		50K		100K	200K	250K	500K	1000K	2000K	5000k			Country
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Austria
	PNG,	-1,		PNG,	-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Belgium
	PNG,	PNG,	PNG,	-1,		-1,		PNG,	-1,		-1,		-1,		-1,			//	Britain
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Eire
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	France
	PNG,	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Germany
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Italy
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Luxembourg
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Netherlands
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Northern Ireland
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Portugal
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Spain
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Switzerland
};

long GetTileGFXType(long nCountry, long nMapScale)
{
	return gfx_type_per_tile[nCountry][nMapScale];
}
*/

/*

	"Austria",			"dbAT",		MAP_DATUM_AT,	"?",
	"Belgium",			"dbNGI",	MAP_DATUM_NGI,	"NGI",		//	NGI: Lambert08
	"Britain",			"dbOSGB",	MAP_DATUM_OSGB,	"OS",		//	BNG: OSGB36
	"Eire",				"dbOSi",	MAP_DATUM_OSi,	"OSi",
	"France",			"dbIGN",	MAP_DATUM_IGN,	"IGN",		//	IGN: Lambert93
	"Germany",			"dbDE",		MAP_DATUM_DE,	"?",
	"Italy",			"dbIT",		MAP_DATUM_IT,	"?",
	"Luxembourg",		"dbLux",	MAP_DATUM_LUX,	"?",
	"Netherlands",		"dbNL",		MAP_DATUM_NL,	"?",
	"Northern Ireland",	"dbOSNi",	MAP_DATUM_OSNi,	"OSNi",
	"Portugal",			"dbPT",		MAP_DATUM_PT,	"?"
	"Spain",			"dbES",		MAP_DATUM_ES,	"?",
	"Switzerland",		"dbCH",		MAP_DATUM_CH,	"?",

	"10K",	 10000,
	"25K",	 25000,
	"50K",	 50000,
	"100K",	100000,
	"250K",	250000,

//	10K		25K		50K		100K	250K			Country

	-1,		-1,		-1,		-1,		-1,			//	Austria
	-1,		-1,		-1,		-1,		-1,			//	Belgium
	-1,		-1,		-1,		-1,		-1,			//	Britain
	-1,		-1,		-1,		-1,		-1,			//	Eire
	-1,		-1,		-1,		-1,		-1,			//	France
	-1,		-1,		-1,		-1,		-1,			//	Germany
	-1,		-1,		-1,		-1,		-1,			//	Italy
	-1,		-1,		-1,		-1,		-1,			//	Luxembourg
	-1,		-1,		-1,		-1,		-1,			//	Netherlands
	-1,		-1,		-1,		-1,		-1,			//	Northern Ireland
	-1,		-1,		-1,		-1,		-1,			//	Portugal
	-1,		-1,		-1,		-1,		-1,			//	Spain
	-1,		-1,		-1,		-1,		-1,			//	Switzerland


struct NationalParkMaps {
	char *	dbCountry;
	char *	dbMapScale;
	int		nNationalPark;
	RECT	rRectangle;
	bool	bNationalPark;
	char *	strLocation;
} NationalParkMaps[N_NATIONAL_PARK_MAPS] = {

	{"NY0342", "SD5777",  1, {0,0,0,0}, false, "Lake District"},
	{"SD9212", "SK3344",  2, {0,0,0,0}, false, "Peak District"},
	{"NY6110", "SE1850",  3, {0,0,0,0}, false, "Yorkshire Dales"},
	{"SH4680", "SO0394",  4, {0,0,0,0}, false, "Snowdonia"},
	{"SS4652", "SY1055",  5, {0,0,0,0}, false, "Dartmoor and Exmoor"},
	{"SN6146", "SO3700",  6, {0,0,0,0}, false, "Brecon Beacons"},
	{"NH4937", "NO5266",  7, {0,0,0,0}, false, "Cairngorms"},
	{"NN0635", "NS7280",  8, {0,0,0,0}, false, "Loch Lomond and the Trossachs"},
	{"SM5865", "SS4789",  9, {0,0,0,0}, false, "Pembrokeshire Coast"},
	{"NT6135", "NZ0864", 10, {0,0,0,0}, false, "Northumberland"},
	{"NZ4226", "TA1675", 11, {0,0,0,0}, false, "North York Moors"},
	{"SU0036", "SZ5189", 12, {0,0,0,0}, false, "New Forest"}
};
*/


extern unsigned char uncompressed[1500*1500];
extern long	uncompressed_length;


bool ReadTracklogsTile(long minx, long maxx, long miny, long maxy, CProgressWindow& wndProgress, 
						unsigned char * bitmap_palette, long & bitmap_palette_length, 
						unsigned char * & bitmap_memory , long & bitmap_memory_size, 
						long & bitmap_width, long & dest_bitmap_width,
						long & bitmap_height, long & dest_bitmap_height,
						bool bUseTL3)
{
	CTracklogsDatabase23 dbaseTL23(bUseTL3);

	long width, height;
	long square_width;
	long square_height;
	long x,y;
	for (y=maxy-1; y>=miny; y--) {
		for (x=minx; x<maxx; x++) {

			if (!wndProgress.ProgressBar()) return false;

// IF Tracklogs

			long id = x*10000+y;

			long palette_length;
			BYTE * palette;
			long raster_length;
			BYTE * raster;
			long revision;

			if (!dbaseTL23.GetTile(	id,									// retrieves tile id, or returns false
									&width, &height,
									&palette_length, &palette,
									&raster_length, &raster,
									&revision))
				continue;
// COMMON start
			if (bitmap_memory == NULL) {
				// First square found so allocate memory for bitmap
				bitmap_width = width*(maxx-minx);
				bitmap_height = height*(maxy-miny);
				dest_bitmap_width = width*(maxx-minx-2);
				dest_bitmap_height = height*(maxy-miny-2);

				bitmap_memory_size = bitmap_width * bitmap_height;
				bitmap_memory = (BYTE *)malloc(bitmap_memory_size);
				if (bitmap_memory == NULL) {
					printf("Couldn't allocate %d bytes for bitmap\n", bitmap_memory_size);
					//exit(0);
					return false;
				}
				memset(bitmap_memory, 0xff, bitmap_memory_size);		// assume 0xff is white

				square_width = width;
				square_height = height;

/*
				if (bitmap_width > 32000)
					printf("Warning: width is %d pixels\n", bitmap_width);
				if (bitmap_height > 32000)
					printf("Warning: height is %d pixels\n", bitmap_height);
*/
				// get palette from database
				memcpy(bitmap_palette, palette, palette_length);
				bitmap_palette_length = palette_length;
			}
// COMMON end

			if (square_width != width) {
				printf("Tile %d mismatch on width. %d instead of %d\n", id, width, square_width);
				continue;
			}
			if (square_height != height) {
				printf("Tile %d mismatch on height. %d instead of %d\n", id, height, square_height);
				continue;
			}
			if (compare_palettes(bitmap_palette, palette, palette_length) != 0) {
				printf("Tile %d has different palette to first tile - ignoring\n", id);
				continue;
			}
			if (bitmap_palette_length != palette_length) {
				printf("Tile %d mismatch on palette size. %d instead of %d\n", id, palette_length, bitmap_palette_length);
				continue;
			}

			// expand compressed data using zlib library
			int err;
			z_stream d_stream; /* decompression stream */

			d_stream.zalloc = (alloc_func)0;
			d_stream.zfree = (free_func)0;
			d_stream.opaque = (voidpf)0;

			d_stream.next_in  = raster;
			d_stream.avail_in = (uInt)raster_length;

			err = inflateInit(&d_stream);
			CHECK_ERR(err, "inflateInit");

			for (;;) {
				d_stream.next_out = uncompressed;            /* discard the output */
				d_stream.avail_out = (uInt)sizeof(uncompressed);
				err = inflate(&d_stream, Z_NO_FLUSH);
				if (err == Z_STREAM_END)
					break;
				CHECK_ERR(err, "large inflate");
			}

			err = inflateEnd(&d_stream);
			CHECK_ERR(err, "inflateEnd");

			uncompressed_length = d_stream.total_out;

			// copy tile into bitmap area
			BYTE * bitmap_offset = bitmap_memory + (maxy-1-y)*height*bitmap_width+(x-minx)*width;
			int tx, ty;
			for (ty=0; ty<height; ty++) {
				for (tx=0; tx<width; tx++) {
					bitmap_offset[ty*bitmap_width + tx] = uncompressed[(height-1-ty)*width + tx];
				}
// ELSE OS Pro
			}
		}
	}
	return true;
}

DatabaseType DetermineDatabaseType(int nDatabase, int nCountry)
{
	if (nCountry == COUNTRY_OSGB) {
		if (nDatabase == DBASE_TRACKLOGS_2) return DB_OSGB_Tracklogs2;
		if (nDatabase == DBASE_TRACKLOGS_3) return DB_OSGB_Tracklogs3;
		if (nDatabase == DBASE_LOCAL) return DB_OSGB_Local;
	} else if (nCountry == COUNTRY_IGN) {
		return DB_IGN_Local;
	} else if (nCountry == COUNTRY_NGI) {
		return DB_NGI_Local;
	} else if (nCountry == COUNTRY_DE) {
		return DB_Kompass_Local;
	} else if (nCountry == COUNTRY_ES) {
		return DB_Spain_Local;
	}
	return DB_Unknown;
}
