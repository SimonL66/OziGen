#pragma once

#include "stdafx.h"

#define	NUMBER_MAP_TYPES	4
#define	NUMBER_COUNTRY_MAPS	13
#define	NUMBER_MAPS_SCALES	10

#define MAP_DATUM_AT		('A' + ('T'<<8)+ (' '<<16)+ (' '<<24))
#define MAP_DATUM_NGI		('N' + ('G'<<8)+ ('I'<<16)+ (' '<<24))
#define MAP_DATUM_OSGB		('O' + ('S'<<8)+ ('G'<<16)+ ('B'<<24))
#define MAP_DATUM_OSi		('O' + ('S'<<8)+ ('i'<<16)+ (' '<<24))
#define MAP_DATUM_IGN		('I' + ('G'<<8)+ ('N'<<16)+ (' '<<24))
#define MAP_DATUM_DE		('D' + ('E'<<8)+ (' '<<16)+ (' '<<24))
#define MAP_DATUM_IT		('I' + ('T'<<8)+ (' '<<16)+ (' '<<24))
#define MAP_DATUM_LUX		('L' + ('U'<<8)+ ('X'<<16)+ (' '<<24))
#define MAP_DATUM_NL		('N' + ('L'<<8)+ (' '<<16)+ (' '<<24))
#define MAP_DATUM_OSNi		('O' + ('S'<<8)+ ('N'<<16)+ ('i'<<24))
#define MAP_DATUM_PT		('P' + ('T'<<8)+ (' '<<16)+ (' '<<24))
#define MAP_DATUM_ES		('E' + ('S'<<8)+ (' '<<16)+ (' '<<24))
#define MAP_DATUM_CH		('C' + ('H'<<8)+ (' '<<16)+ (' '<<24))

#define MAP_GFX_FORMAT_PNG	('P' + ('N'<<8)+ ('G'<<16)+ ('d'<<24))
#define MAP_GFX_FORMAT_BMP	('B' + ('M'<<8)+ ('P'<<16)+ ('z'<<24))
#define MAP_GFX_FORMAT_JPEG	('J' + ('P'<<8)+ ('E'<<16)+ ('G'<<24))

#define MAP_GFX_TYPE_RASTER	('R' + ('A'<<8)+ ('S'<<16)+ ('T'<<24))
#define MAP_GFX_TYPE_VECTOR	('V' + ('E'<<8)+ ('C'<<16)+ ('T'<<24))
#define MAP_GFX_TYPE_STREET	('S' + ('T'<<8)+ ('R'<<16)+ ('E'<<24))

enum RadioDatabase {
	DBASE_TRACKLOGS_2,
	DBASE_TRACKLOGS_3,
	DBASE_LOCAL
};

enum RadioMapType {
	MAP_OZI,
	MAP_KMZ,
	MAP_JNX,
	MAP_JPR,
	MAP_QCT,
	MAP_PKG
};

enum ComboMapType {
	MAP_RASTER,
	MAP_VECTOR,
	MAP_STREET,
};

enum RadioCountry {
	COUNTRY_AT,
	COUNTRY_NGI, 
	COUNTRY_OSGB,
	COUNTRY_OSI,
	COUNTRY_IGN,
	COUNTRY_DE,
	COUNTRY_IT,
	COUNTRY_LUX,
	COUNTRY_NL,
	COUNTRY_OSNI,
	COUNTRY_PT,
	COUNTRY_ES,
	COUNTRY_CH,
};

enum MapScale {

	MAP_SCALE_10K,
	MAP_SCALE_25K,
	MAP_SCALE_50K,
	MAP_SCALE_100K,
	MAP_SCALE_250K,

	MAP_SCALE_500K,
	MAP_SCALE_1000K,
	MAP_SCALE_2000K,
	MAP_SCALE_5000K,
};

enum GPSModel {
	GPS_MODEL_MONTANA,
	GPS_MODEL_OREGON,
	GPS_MODEL_DAKOTA,
	GPS_MODEL_ETREX,
	GPS_MODEL_GPS_6X,
	GPS_MODEL_OREGON_6X
};

enum CoordSystem {
	COORD_SYSTEM_LAT_LON,
	COORD_SYSTEM_GRID,
	COORD_SYSTEM_OS
};

enum DatabaseGraphicsType {
	GFX_TYPE_BMP,
	GFX_TYPE_JPEG,
	GFX_TYPE_PNG_8,
	GFX_TYPE_PNG_24,
	GFX_TYPE_PNG_32
};

class CCustomMap
{
public:
	CCustomMap(void);
	~CCustomMap(void);

	CCustomMap(int nFlavour, int nCountry, int nScale);


	void UpdateFlavour(int nFlavour);
	void UpdateCountry(int nCountry);
	void UpdateScale(int nScale);

	void UpdateWorkingData();

	int m_nDatum;

	int m_nFlavour;
	int m_nCountry;
	int m_nScale;

	int m_nPixelsPerTile;
	int m_nMapScale;
	int m_nMetresPerTile;

	int m_nTilesPerKilometre;

	int m_nTileBitDepth;

	int m_nTileWidth;

	int m_nUTMZone;
};

CString GetMapFlavourText(long nFlavour);
CString GeDatabaseText(long nCountry);
CString GetCountryPrefix(long nCountry);
CString GetMapScaleText(long nScale);

long GetTilePixelWidth(long nFlavour, long nCountry, long nMapScale);
long GetTileMeterWidth(long nFlavour, long nCountry, long nMapScale);

long GetDatum(long nCountry);
long GetMapScale(long nMapScale);
double GetMetersPerPixel(long nMapType);
long GetTileWidth(long nMapType);
long GetPixelsPerTile(long nMapType);

long GetTileBitDepth(long nFlavour, long nCountry, long nMapScale);

long GetTileGFXType(long nFlavour, long nCountry, long nMapScale);

int GetTileIterator(int nMapScale);

CString GetGridReferenceRegistryValues(long nCountry);

void AlignMapExtents(int nMapScale, long& minx, long& maxx, long& miny, long& maxy);

//long GetTileDatabaseScale(long nMapType);
