#pragma once

#include "stdafx.h"

#define	MAX_COUNTRY		19
#define	MAX_PRODUCTS	5
#define	MAX_DETAILS		12

#define MAP_DATUM_AT		('A' + ('T'<<8)+ (' '<<16)+ (' '<<24))
#define MAP_DATUM_BE		('N' + ('G'<<8)+ ('I'<<16)+ (' '<<24))
#define MAP_DATUM_CH		('C' + ('H'<<8)+ (' '<<16)+ (' '<<24))
#define MAP_DATUM_CZ		('C' + ('Z'<<8)+ (' '<<16)+ (' '<<24))
#define MAP_DATUM_DE		('D' + ('E'<<8)+ (' '<<16)+ (' '<<24))
#define MAP_DATUM_ES		('E' + ('S'<<8)+ (' '<<16)+ (' '<<24))
#define MAP_DATUM_FI		('F' + ('I'<<8)+ ('N'<<16)+ (' '<<24))
#define MAP_DATUM_FR		('I' + ('G'<<8)+ ('N'<<16)+ (' '<<24))
#define MAP_DATUM_IE		('O' + ('S'<<8)+ ('i'<<16)+ (' '<<24))
#define MAP_DATUM_IT		('I' + ('T'<<8)+ (' '<<16)+ (' '<<24))
#define MAP_DATUM_LU		('L' + ('U'<<8)+ ('X'<<16)+ (' '<<24))
#define MAP_DATUM_NI		('O' + ('S'<<8)+ ('N'<<16)+ ('i'<<24))
#define MAP_DATUM_NL		('N' + ('L'<<8)+ (' '<<16)+ (' '<<24))
#define MAP_DATUM_PT		('P' + ('T'<<8)+ (' '<<16)+ (' '<<24))
#define MAP_DATUM_PL		('P' + ('L'<<8)+ (' '<<16)+ (' '<<24))
#define MAP_DATUM_SI		('S' + ('l'<<8)+ ('o'<<16)+ (' '<<24))
#define MAP_DATUM_UK		('O' + ('S'<<8)+ ('G'<<16)+ ('B'<<24))

#define MAP_DATUM_DK		('D' + ('K'<<8)+ (' '<<16)+ (' '<<24))


#define MAP_DATUM_CAT		('C' + ('a'<<8)+ ('t'<<16)+ (' '<<24))
#define MAP_DATUM_OSM		('O' + ('S'<<8)+ ('M'<<16)+ (' '<<24))
#define MAP_DATUM_BING		('B' + ('I'<<8)+ ('N'<<16)+ ('G'<<24))
#define MAP_DATUM_SAT		('S' + ('A'<<8)+ ('T'<<16)+ (' '<<24))

#define MAP_GFX_FORMAT_PNG	('P' + ('N'<<8)+ ('G'<<16)+ ('d'<<24))
#define MAP_GFX_FORMAT_BMP	('B' + ('M'<<8)+ ('P'<<16)+ ('z'<<24))
#define MAP_GFX_FORMAT_JPEG	('J' + ('P'<<8)+ ('E'<<16)+ ('G'<<24))
#define MAP_GFX_FORMAT_GIF	('G' + ('I'<<8)+ ('F'<<16)+ ('F'<<24))

#define MAP_GFX_TYPE_RASTER	('R' + ('A'<<8)+ ('S'<<16)+ ('T'<<24))
#define MAP_GFX_TYPE_VECTOR	('V' + ('E'<<8)+ ('C'<<16)+ ('T'<<24))
#define MAP_GFX_TYPE_STREET	('S' + ('T'<<8)+ ('R'<<16)+ ('E'<<24))
#define MAP_GFX_TYPE_AERIAL	('S' + ('A'<<8)+ ('T'<<16)+ (' '<<24))
#define MAP_GFX_TYPE_HIGHLAND	('H' + ('I'<<8)+ ('G'<<16)+ ('H'<<24))

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

enum ComboMapTypeUK {
	MAP_RASTER,
	MAP_VECTOR,
	MAP_STREET,
	MAP_AERIAL,
	MAP_HIGHLAND,
};

enum ComboMapTypeDE {
	MAP_DE_NRW,
	MAP_DE_MV,
	MAP_DE_BKG,
	MAP_DE_KOMPASS,
};

enum ComboMapTypeES {
	MAP_ES_IGN,
	MAP_ES_MTC,
};

enum RadioCountry {

	COUNTRY_BE, 
	COUNTRY_UK,
	COUNTRY_CAT,
	COUNTRY_CZ,
	COUNTRY_DE,
	COUNTRY_ES,
	COUNTRY_FI,

	COUNTRY_PT,
	COUNTRY_SI,

	COUNTRY_OSM,

	COUNTRY_SAT,

	COUNTRY_DK,


//	COUNTRY_AT,
//	COUNTRY_IE,
//	COUNTRY_IT,
//	COUNTRY_LU,
//	COUNTRY_NL,
//	COUNTRY_PL,
//	COUNTRY_CH,
	COUNTRY_FR,

	NUM_COUNTRY
};

enum MapScale {

	MAP_SCALE_10K,
	MAP_SCALE_25K,
	MAP_SCALE_50K,
	MAP_SCALE_100K,
	MAP_SCALE_200K,
	MAP_SCALE_250K,
	MAP_SCALE_500K,
	MAP_SCALE_750K,
	MAP_SCALE_1000K,
	MAP_SCALE_2000K,
	MAP_SCALE_2500K,
	MAP_SCALE_5000K,

	NUM_SCALE
};

enum DetailLevel {
	DETAIL_01,
	DETAIL_02,
	DETAIL_03,
	DETAIL_04,
	DETAIL_05,
	DETAIL_06,
	DETAIL_07,
	DETAIL_08,
	DETAIL_09,

	NUM_DETAILS
};

enum Products {
	PRODUCT_01,
	PRODUCT_02,
	PRODUCT_03,
	PRODUCT_04,
	PRODUCT_05,
	PRODUCT_06,
	PRODUCT_07,
	PRODUCT_08,
	PRODUCT_09,
	PRODUCT_10,

	NUM_PRODUCTS
};

enum MapServerTypes {
	WMS,
	GetMapping,
	OpenStreetMap,
	Kompass,
	Bing,
	SatMap,
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

enum GraphicsType {
	JPG,
	PNG,
	BMP,
	GIF
};

class CCustomMap
{
public:
	CCustomMap(void);
	~CCustomMap(void);

	CCustomMap(int nCountry, int nProduct, int nDetail);

//CTileDatabase	dbTiles;

	void Update(int nCountry, int nProduct, int nDetail);

	void UpdateCountry(int nCountry);
	void UpdateProduct(int nProduct);
	void UpdateDetail(int nDetail);

//	void UpdateWorkingData();

	int GetCountry();
	int GetProduct();
	int GetDetail();

	int GetDatum();
	int GetMetresPerTile();
	int GetPixelsPerTile();
	int GetMapScale();
	int GetTileBitDepth();
	int GetTileGfxType();

	int GetCoordinateSystem();

	int GetProjection();

	CString GetMapName(CRect rectBBox);

	int wmsGetPort();
	CString wmsGetAttribution();
	CString wmsGetRequest(int easting, int northing);

CString GetCountryName(int nCountry = -1);
CString GetRegistryValueName(int nCountry = -1, int nProduct = -1);
CString GetInternetCode(int nCountry = -1);
CString GetExampleLatLon(int nCountry = -1);
CString GetExampleGrid(int nCountry = -1);
CString GetCoordSystemName(int nCountry = -1);

bool IsValidScale(int nDetail = -1);

CString GetFlavourName(int nCountry = -1, int nProduct = -1);
CString GetFlavourCode(int nCountry = -1, int nProduct = -1);
int GetDefaultScale(int nCountry = -1, int nProduct = -1);

void MakeDatabaseFolders();

void UpdateDatabaseRoot(CString strFolder) { m_strDatabaseRoot = strFolder; };

int GetCountCountry() {	return m_nCountryCount; };

int GetServerType();

CString GetMapFlavourText(int nCountry = -1, int nProduct = -1);
CString GetMapScaleText(int nDetail = -1);

int GetMapScaleIndex(CString strScaleText);

CString GetScaleCode(int nDetail = -1);

CString GetDatabasePath();

void WriteProfile(/*CRect rectBBox*/CString strGridRef);
void ReadProfile(/*int x, int y*/CString& strGridRef);

CString getmappingGetRequest(int easting, int northing);
CString osmGetRequest(int easting, int northing);
CString kompassGetRequest(int easting, int northing);
CString bingGetRequest(int easting, int northing);
CString satmapGetRequest(int easting, int northing);

int CCustomMap::GetLevelOfDetail();

private:

	void ParseServerRequestString_WMS(int nCountry, int nProduct, int nDetail, int l);
	void ParseServerRequestString_GetMapping(int nCountry, int nProduct, int nDetail, int l);

void ParseServerRequestString_OSM(int nCountry, int nProduct, int nDetail, int l);
void ParseServerRequestString_Kompass(int nCountry, int nProduct, int nDetail, int l);
void ParseServerRequestString_Bing(int nCountry, int nProduct, int nDetail, int l);
void ParseServerRequestString_SatMap(int nCountry, int nProduct, int nDetail, int l);

	void InitialiseParameterArrayCountry();
	void InitialiseParameterArrayFlavour();
	void InitialiseParameterArrayTile();

private:

//	int m_nDatum;

	int m_nCountry;
	int m_nProduct;
	int m_nDetail;

//	int m_nMapScale;
//	int m_nPixelsPerTile;
//	int m_nMetresPerTile;

//	int m_nTileBitDepth;

	int m_nCoordSystem;

	int m_nCountryCount;

	CString m_strDatabaseRoot;

	CRect m_rectBBox;

public:
//	int m_nWMS;
//	int m_nWMSLayer;


};
