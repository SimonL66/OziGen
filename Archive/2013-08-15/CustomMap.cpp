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
	250,	-1,		400,	-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Germany
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
	250,	-1,		1000,	-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Germany
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
	 4,		-1,		 1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Germany
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
	32,		-1,		32,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Germany
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
	PNG,	-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,		-1,			//	Germany
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
