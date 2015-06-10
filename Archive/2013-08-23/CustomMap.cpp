#include "CustomMap.h"

#include <direct.h>

#include "ozigen.h"

extern CString g_strMapDatabaseFolder;
extern CString g_strMapFlavour;
extern CString g_strMapCountry;
extern CString g_strMapScale;

CCustomMap::CCustomMap(void)
{
}

CCustomMap::~CCustomMap(void)
{
}

CCustomMap::CCustomMap(int nFlavour, int nCountry, int nScale)
{
	m_nFlavour = nFlavour;
	m_nCountry = nCountry;
	m_nScale = nScale;
	UpdateWorkingData();
}

void CCustomMap::UpdateFlavour(int nFlavour)
{
	m_nFlavour = nFlavour;
	UpdateWorkingData();
}

void CCustomMap::UpdateCountry(int nCountry)
{
	m_nCountry = nCountry;
	UpdateWorkingData();
}

void CCustomMap::UpdateScale(int nScale)
{
	m_nScale = nScale;
	UpdateWorkingData();
}


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
	"Aerial",
};

int nMapScales[] = {

	 10000,		// 1:10K
	 25000,		// 1:25K
	 50000,		// 1:50K
	100000,		// 1:100K
	250000,		// 1:250K
	500000,		// 1:250K
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

char* DatabaseName[NUMBER_COUNTRY_MAPS] = {
	"dbAT",					//	Austria
	"dbNGI", 				//	Belgium
	"dbOSGB",				//	Britain
	"dbOSi",				//	Eire
	"dbIGN",				//	France
	"dbDE",					//	Germany
	"dbIT",					//	Italy
	"dbLux",				//	Luxembourg
	"dbNL",					//	Netherlands
	"dbOSNi",				//	Northern Ireland
	"dbPT",					//	Portugal
	"dbES",					//	Spain
	"dbCH",					//	Switzerland
};

char* CountryPrefix[NUMBER_COUNTRY_MAPS] = {
	"at",					//	Austria
	"ngi", 					//	Belgium
	"osgb",					//	Britain
	"oSi",					//	Eire
	"ign",					//	France
	"de",					//	Germany
	"it",					//	Italy
	"lunx",					//	Luxembourg
	"no",					//	Netherlands
	"osni",					//	Northern Ireland
	"pt",					//	Portugal
	"es",					//	Spain
	"ch",					//	Switzerland
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
	250,	200,	400,	-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Germany
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Italy
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Luxembourg
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Netherlands
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Northern Ireland
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Portugal
	-1,		-1,		400,	-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Spain
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

//	Aerial:
//	10K		25K		50K		100K	200K	250K	500K	1000K	2000K	5000k			Country
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Austria
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Belgium
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Britain
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
	250,	500,	1000,	-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Germany
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Italy
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Luxembourg
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Netherlands
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Northern Ireland
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Portugal
	-1,		-1,		1000,	-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Spain
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

//	Aerial:
//	10K		25K		50K		100K	200K	250K	500K	1000K	2000K	5000k			Country
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Austria
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Belgium
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Britain
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

int number_of_tiles_per_km[NUMBER_MAP_TYPES][NUMBER_COUNTRY_MAPS][NUMBER_MAPS_SCALES] = {

//	Raster Map:
//	10K		25K		50K		100K	200K	250K	500K	1000K	2000K	5000k			Country
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Austria
	 4,		-1,		 1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Belgium
	 4,		 2,		 1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Britain
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Eire
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	France
	 4,		 2,		 1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Germany
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Italy
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Luxembourg
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Netherlands
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Northern Ireland
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Portugal
	-1,		-1,		 1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Spain
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Switzerland

//	Vector Map:
//	10K		25K		50K		100K	200K	250K	500K	1000K	2000K	5000k			Country
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Austria
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Belgium
	 4,		 2,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Britain
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
	 4,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Britain
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

//	Aerial:
//	10K		25K		50K		100K	200K	250K	500K	1000K	2000K	5000k			Country
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Austria
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Belgium
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Britain
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

int bit_depth_per_tile[NUMBER_MAP_TYPES][NUMBER_COUNTRY_MAPS][NUMBER_MAPS_SCALES] = {

//	Raster Map:
//	10K		25K		50K		100K	200K	250K	500K	1000K	2000K	5000k			Country
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Austria
	24,		-1,		24,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Belgium
	24,		 8,		 8,		-1,		-1,		 8,		-1,		-1,		-1,		-1,			//	Britain
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Eire
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	France
	32,		32,		32,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Germany
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Italy
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Luxembourg
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Netherlands
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Northern Ireland
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Portugal
	-1,		-1,		24,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Spain
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Switzerland

//	Vector Map:
//	10K		25K		50K		100K	200K	250K	500K	1000K	2000K	5000k			Country
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Austria
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Belgium
	32,		 8,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Britain
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

//	Aerial:
//	10K		25K		50K		100K	200K	250K	500K	1000K	2000K	5000k			Country
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Austria
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Belgium
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Britain
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

long gfx_type_per_tile[NUMBER_MAP_TYPES][NUMBER_COUNTRY_MAPS][NUMBER_MAPS_SCALES] = {

//	Raster Map:
//	10K		25K		50K		100K	200K	250K	500K	1000K	2000K	5000k			Country
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Austria
	PNG,	-1,		PNG,	-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Belgium
	PNG,	PNG,	PNG,	-1,		-1,		PNG,	-1,		-1,		-1,		-1,			//	Britain
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Eire
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	France
	PNG,	PNG,	PNG,	-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Germany
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Italy
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Luxembourg
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Netherlands
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Northern Ireland
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Portugal
	-1,		-1,		PNG,	-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Spain
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Switzerland

//	Vector Map:
//	10K		25K		50K		100K	200K	250K	500K	1000K	2000K	5000k			Country
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Austria
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Belgium
	PNG,	PNG,	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Britain
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
	PNG,	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Britain
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

//	Aerial:
//	10K		25K		50K		100K	200K	250K	500K	1000K	2000K	5000k			Country
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Austria
	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Belgium
	-1,		-1,		-JPG,	-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Britain
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

long GetTileGFXType(long nFlavour, long nCountry, long nMapScale)
{
	return gfx_type_per_tile[nFlavour][nCountry][nMapScale];
}
/*
long GetTileDatabaseScale(long nMapType)
{
	long nScale;
	switch (nMapType) {
	case MAP_SCALE_10K:
		nScale = 250;
		break;
	case MAP_SCALE_25K:
		nScale = 500;
		break;
	case MAP_SCALE_50K:
		nScale = 1000;
		break;

	case MAP_SCALE_100K:
		nScale = 2000;
		break;

	case MAP_SCALE_250K:
		nScale = 5000;
		break;

	case MAP_SCALE_500K:
		nScale = 250;
		break;
	case MAP_SCALE_1000K:
		nScale = 250;
		break;
	case MAP_SCALE_2000K:
		nScale = 500;
		break;
	case MAP_SCALE_5000K:
		nScale = 20000;
		break;

	default:
		nScale = 500;
		break;
	}
	return nScale;
}
*/

double GetMetersPerPixel(long nMapType)
{
/*
	MAP_SCALE_10K,
	MAP_SCALE_25K,
	MAP_SCALE_50K,
	MAP_SCALE_100K,
	MAP_SCALE_250K,

	MAP_SCALE_VML,
	MAP_SCALE_SV,
	MAP_SCALE_VMD,
	MAP_SCALE_MS,
	MAP_SCALE_OV,
*/
	double dMetersPerPixel;

	switch (nMapType) {
	case MAP_SCALE_10K:
		dMetersPerPixel = 1.0;
		break;
	case MAP_SCALE_25K:
		dMetersPerPixel = 2.5;
		break;
	case MAP_SCALE_50K:
		dMetersPerPixel = 5.0;
		break;
	case MAP_SCALE_100K:
		dMetersPerPixel = 10.0;
		break;
	case MAP_SCALE_250K:
		dMetersPerPixel = 25.0;
		break;

	default:
		dMetersPerPixel = 2.5;
		break;
	}
	return dMetersPerPixel;
}

long GetTileWidth(long nMapType)
{
/*
	MAP_SCALE_10K,
	MAP_SCALE_25K,
	MAP_SCALE_50K,
	MAP_SCALE_100K,
	MAP_SCALE_250K,
*/
	int nTileWidth;

	switch (nMapType) {
	case MAP_SCALE_10K:
		nTileWidth = 1000;
		break;
	case MAP_SCALE_25K:
		nTileWidth = 400;
		break;
	case MAP_SCALE_50K:
		nTileWidth = 200;
		break;
	case MAP_SCALE_100K:
		nTileWidth = 100;
		break;
	case MAP_SCALE_250K:
		nTileWidth = 40;
		break;
/*
	case MAP_SCALE_MS:
		nTileWidth = 10;
		break;
	case MAP_SCALE_OV:
		nTileWidth = 2;
		break;
*/
	default:
		nTileWidth = 400;
		break;
	}
	return nTileWidth;
}

long GetPixelsPerTile(long nMapType)
{
/*
	MAP_SCALE_10K,
	MAP_SCALE_25K,
	MAP_SCALE_50K,
	MAP_SCALE_100K,
	MAP_SCALE_250K,

	MAP_SCALE_VML,
	MAP_SCALE_SV,
	MAP_SCALE_VMD,
	MAP_SCALE_MS,
	MAP_SCALE_OV,
*/
	int nPixelsPerTile;

	switch (nMapType) {
	case MAP_SCALE_10K:
		nPixelsPerTile = 250;
		break;
	case MAP_SCALE_25K:
		nPixelsPerTile = 200;
		break;
	case MAP_SCALE_50K:
		nPixelsPerTile = 200;
		break;
	case MAP_SCALE_100K:
//		nPixelsPerTile = 100;
		break;
	case MAP_SCALE_250K:
//		nPixelsPerTile = 40;
		break;
/*
	case MAP_SCALE_MS:
//		nPixelsPerTile = 10;
		break;
	case MAP_SCALE_OV:
//		nPixelsPerTile = 2;
		break;
*/
	default:
		nPixelsPerTile = 200;
		break;
	}
	return nPixelsPerTile;
}

int GetTileIterator(int nMapScale)
{
	int iterator = 1;
/*
	if (nMapScale == MAP_SCALE_250K) {
		iterator = 5;
	} else if (nMapScale == MAP_SCALE_MS) {
		iterator = 20;
	}
*/
	return iterator;
}

void AlignMapExtents(int nMapScale, long& minx, long& maxx, long& miny, long& maxy)
{
/*
	if (nMapScale == MAP_SCALE_250K) {
		minx = minx/5*5;
		miny = miny/5*5;
		maxx = (maxx+4)/5*5;
		maxy = (maxy+4)/5*5;
	} else if (nMapScale == MAP_SCALE_MS) {
		minx = minx/20*20;
		miny = miny/20*20;
		maxx = (maxx+19)/20*20;
		maxy = (maxy+19)/20*20;
	}
*/
}


long GetDatum(long nCountry)
{
	return nMapDatum[nCountry];
}

long GetMapScale(long nMapScale)
{
	return nMapScales[nMapScale];
}

CString GetMapFlavourText(long nFlavour)
{
	CString str = MapFlavours[nFlavour];
	return str;
}

CString GetDatabaseText(long nCountry)
{
	CString str = DatabaseName[nCountry];
	return str;
}

CString GetCountryPrefix(long nCountry)
{
	CString str = CountryPrefix[nCountry];
	return str;
}

CString GetMapScaleText(long nScale)
{
	CString str = MapScales[nScale];
	return str;
}

void MakeDatabaseFolders(int nFlavour, int nCountry, int nScale)
{
	CString strFolder;

	g_strMapFlavour = MapFlavours[nFlavour];
	g_strMapCountry = DatabaseName[nCountry];
	g_strMapScale = MapScales[nScale];

	strFolder.Format("%s\\%s", g_strMapDatabaseFolder, GetDatabaseText(nCountry));
	_mkdir(strFolder);

	strFolder.Format("%s\\%s\\%s_%s", g_strMapDatabaseFolder, GetDatabaseText(nCountry), GetMapScaleText(nScale), GetMapFlavourText(nFlavour));
	_mkdir(strFolder);
}

void CCustomMap::UpdateWorkingData()
{
	m_nDatum = GetDatum(m_nCountry);
	m_nMapScale = GetMapScale(m_nScale);
	m_nPixelsPerTile = GetTilePixelWidth(m_nFlavour, m_nCountry, m_nScale);
	m_nMetresPerTile = GetTileMeterWidth(m_nFlavour, m_nCountry, m_nScale);
	m_nTileBitDepth = GetTileBitDepth(m_nFlavour, m_nCountry, m_nScale);
	m_nTilesPerKilometre = number_of_tiles_per_km[m_nFlavour][m_nCountry][m_nScale];

	m_nTileWidth = m_nPixelsPerTile * m_nTilesPerKilometre;
}




/*
    map.projection = "EPSG:23030";
    map.displayProjection = new OpenLayers.Projection("EPSG:4326");
    map.maxExtent = new OpenLayers.Bounds(-100000, 3950000, 1150000, 4900000);
    map.resolutions = [1800,900,450,225,120,50,25,10,4.5,3,2,1,0.5];
    map.tileSize = new OpenLayers.Size(200, 200);
    var ign = new OpenLayers.Layer.WMS( "IGN topo",
        "http://www.idee.es/wms/MTN-Raster/MTN-Raster", {layers: 'mtn_rasterizado'} );
    map.addLayer(ign);
    ign.attribution = 'IGN topo maps from <a target="_blank" href="http://www.idee.es/">IDEE</a>';
    var pnoa = new OpenLayers.Layer.WMS( "PNOA aerial/topo",
        "http://www.idee.es/wms/PNOA/PNOA", {layers: 'pnoa'} );
    map.addLayer(pnoa);
    pnoa.attribution = 'PNOA photos from <a target="_blank" href="http://www.idee.es/">IDEE</a>';
*/

/*
Using OpenLayers: Changing the Projection

 
Those first examples used the default projection, 4326 (which OL uses as shorthand for the Plate Carr�e projection, i.e. geographical coordinates used as x/y coordinates, i.e. pretending the earth is a flat rectangle). Now let's look at an example using a 'proper' projection. For this, I'll use the Spanish WMS in one of the UTM projections, EPSG:23030.
 
    map.projection = "EPSG:23030";
    map.displayProjection = new OpenLayers.Projection("EPSG:4326");
    map.maxExtent = new OpenLayers.Bounds(-100000, 3950000, 1150000, 4900000);
    map.resolutions = [1800,900,450,225,120,50,25,10,4.5,3,2,1,0.5];
    map.tileSize = new OpenLayers.Size(200, 200);
    var ign = new OpenLayers.Layer.WMS( "IGN topo",
        "http://www.idee.es/wms/MTN-Raster/MTN-Raster", {layers: 'mtn_rasterizado'} );
    map.addLayer(ign);
    ign.attribution = 'IGN topo maps from <a target="_blank" href="http://www.idee.es/">IDEE</a>';
    var pnoa = new OpenLayers.Layer.WMS( "PNOA aerial/topo",
        "http://www.idee.es/wms/PNOA/PNOA", {layers: 'pnoa'} );
    map.addLayer(pnoa);
    pnoa.attribution = 'PNOA photos from <a target="_blank" href="http://www.idee.es/">IDEE</a>';
 
The first few lines show what has to be set up for this. First we define the projection (I'll come back to displayProjection in a moment); the WMS only covers Spain, so we set the maxExtent (based on that given in the WMS getCapabilities). This is a full WMS, so is capable of providing any resolution you like; however, these are rasterised images so you're really limited to whatever resolutions the images were rasterised at. Unfortunately, in this case I've found that trial and error is the only way of establishing which are the most sensible resolutions. This list of resolutions of course corresponds to the zoomlevels, so in this case, there will be 14 zoomlevels. I provide whole numbers, and also set the tilesize to 200x200px so that the tiles provided are easier to relate to m/km on the ground (in principle, it's rather odd to have a tileSize property on the map, as layers not maps have a tileSize, but it does mean that if all your layers are the same you only need to set it once). I add two layers, one for the rasterised topo-maps, the other for orthophotos (only provided at higher zoomlevels). These are thus both baselayers, and both in the same projection. As there are now 2 layers, I add a layerswitcher control so users can change from one to the other:
 
    new OpenLayers.Control.LayerSwitcher( {title: 'Switch/add/remove layers'} )
 
You can define the map options after creating the map, as I do above, or you can add these new options to the options object given to Map at instantiation; the latter is probably the better way of doing this.
 
The way projections are defined in OL is rather confusing, so here's some more details. map.projection is a string with a default value of EPSG:4326; it is thus different from layer.projection, which can be a string or a projection object. When a layer is added to a map, if layer.projection is not set, it's taken from map.projection; if the result is a string, it's converted to a projection object. So after being added, map.projection is always a string, layer.projection is always a projection object. Map.js has two methods, getProjection() and getProjectionObject(), but if you read the docs for these you'll find that the logic will change with version 3. getProjectionObject() returns map.baseLayer.projection, so you can simply use that instead.
 
In my pages, I have patched OL so layers can be in different projections, but to simplify things here I define map.projection, and then let all layers be set to that projection.

Adding coordinate conversion
 
Once you start using different projections, you have to start thinking about the possibility of converting coordinates from one projection to another, either when displaying those coordinates, or when reading in vector data in a different coordinate system from the map's. As stated before, this conversion logic is not provided by OL (apart from the special case of SphericalMercator, as described below), so you have to use Proj4js, which you will have to include in your page. The compressed library is now available on cdnjs:
 
<script src="http://cdnjs.cloudflare.com/ajax/libs/proj4js/1.1.0/proj4js-compressed.js"></script>
 
but you can also download the latest zip file from the Proj4js site and use the compressed version in that.
 
You then also need to provide the definition for the particular projection/coordinate system, so that Proj can use it to convert the coordinates. You can get these from a Proj installation or online from spatialreference.org. Here, we use EPSG:23030, for which the proj4js def is here. You then stick this at the top of your script so proj4js can use it.
 
However, there is a complication in the case of 23030 for Spain, as you have to allow for the datum shift, otherwise conversions in/out of 4326 will be about 100m out. You do this by providing the towgs84 parameter, so the full def at the top of the script looks like:
 
Proj4js.defs["EPSG:23030"] = "+proj=utm +zone=30 +ellps=intl +towgs84=-131,-100.3,-163.4,-1.244,-0.020,-1.144,9.39 +units=m +no_defs";
 
Once this is set up, you can then use it for any other coordinate transformation you like. LonLat, Bounds and Geometry objects all have a transform() method which can be used for this. For example, the initial setCenter() coordinates can either be given directly in the map projection, or they can be given as a LonLat and then transformed. For example, for roughly the centre of Spain:
 
    var lonlat = new OpenLayers.LonLat(-3.57138, 39.8384);
    lonlat.transform(map.displayProjection, map.baseLayer.projection);
    map.setCenter(lonlat, 5);
 
Note that transform() changes the actual object, so lonlat.lat and lonlat.long will be in the new coordinate system, and no longer the geographical coordinates entered.

SphericalMercator
 
There is one special case which OL does provide the transform() logic for, namely SphericalMercator, also known as Web Mercator. This is the name given to the projection used by the commercial API providers, such as Google, and also by OpenStreetMap. It has been given the code EPSG:3857, though OL still uses the older, unofficial EPSG:900913 (which looks a bit like 'google'!). For this, there is no need to use Proj4js. The way this is defined changes with 2.12. There is no longer any need to set any layer parameters for Google, Bing or OSM layers, as these are set by default. You can set them on the map as for other projections (as above). There's an example of this, plus the map options needed, in spherical-mercator.html in the OL examples. I also use OpenStreetMap as the baselayer in my edit example later. Again, once this is set up, you can then transform LonLat, Bounds or Geometry objects as above. Besides EPSG:3857 and the fictitious EPSG:900913, OL also supports the equally fictitious EPSG:102100 and EPSG:102113, used by ArcGIS.

displayProjection
 
There are currently 3 controls which use displayProjection: ArgParser, MousePosition, Permalink. This is to enable converting the coordinates into a projection other than the map's. So when, for example, MousePosition needs to display coordinates, it checks whether displayProjection is set, and if so converts them from the map projection to the displayProjection, as with transform() above. (displayProjection too is always a projection object.)
 
Look at the docs for Map.displayProjection, and you will see that if it's set it will be used on any control where it's not set. This means you don't have to define it on each control, but of course you have to make sure you set it on the map before adding the controls.
 
On my maps, I have two MousePositions, for both lat/longs and projected x/y coordinates:
 
    map.addControl(new OpenLayers.Control.MousePosition( {id: "ll_mouse", formatOutput: formatLonlats} ));
    map.addControl(new OpenLayers.Control.MousePosition( {id: "utm_mouse", prefix: "UTM ", displayProjection: map.baseLayer.projection, numDigits: 0} ));
 
The first one is set from the map (4326, as we defined at the top of the page); the second is in the UTM metres, using the numDigits option to chop off decimal places. Of course, we also have to define in CSS where we want these to appear:
 
.olControlMousePosition {
    position: absolute;
    right: 10px;
    top: 0px;
    height: 15px;
    font-size: 8pt;
    background-color: white
}
#utm_mouse {
    top: 15px;
}
 
I also use the formatOutput option to add a DMS version to the lonlats by adding the following function to our JS:

    function formatLonlats(lonLat) {
        var lat = lonLat.lat;
        var long = lonLat.lon;
        var ns = OpenLayers.Util.getFormattedLonLat(lat);
        var ew = OpenLayers.Util.getFormattedLonLat(long,'lon');
        return ns + ', ' + ew + ' (' + (Math.round(lat * 10000) / 10000) + ', ' + (Math.round(long * 10000) / 10000) + ')';
    }
 
View/download page with this logic
*/