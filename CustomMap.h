#pragma once

#include "stdafx.h"

#define	MAX_COUNTRY		18
#define	MAX_PRODUCTS	12
#define	MAX_LAYERS		12

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

#define MAP_DATUM_SE		('S' + ('E'<<8)+ (' '<<16)+ (' '<<24))

#define MAP_DATUM_HR		('H' + ('R'<<8)+ (' '<<16)+ (' '<<24))


#define MAP_DATUM_ARC		('A' + ('R'<<8)+ ('C'<<16)+ (' '<<24))


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

/*
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
*/

enum Country {

	COUNTRY_BE,		// Belgium
	COUNTRY_UK,		// Britain
	COUNTRY_HR,		// Croatia
	COUNTRY_CZ,		// Czech Republic
	COUNTRY_DK,		// Denmark
	COUNTRY_FI,		// Finland
	COUNTRY_FR,		// France
	COUNTRY_DE,		// Germany
	COUNTRY_IT,		// Italy
	COUNTRY_LU,		// Luxembourg
	COUNTRY_PT,		// Portugal
	COUNTRY_SI,		// Slovenia
	COUNTRY_ES,		// Spain
	COUNTRY_SE,		// Sweden
	COUNTRY_CH,		// Switzerland

	COUNTRY_OSM,
	COUNTRY_ARC,
	COUNTRY_SAT,

//	COUNTRY_AT,
//	COUNTRY_IE,
//	COUNTRY_NL,
//	COUNTRY_PL,

//	COUNTRY_CAT,

	NUM_COUNTRY
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
	PRODUCT_11,
	PRODUCT_12,

	NUM_PRODUCTS
};

enum Layers {
	
	LAYER_01,
	LAYER_02,
	LAYER_03,
	LAYER_04,
	LAYER_05,
	LAYER_06,
	LAYER_07,
	LAYER_08,
	LAYER_09,
	LAYER_10,
	LAYER_11,
	LAYER_12,

	NUM_LAYERS
};

enum MapServers {

	WMS,

	WMTS,

	TMS,

	GetMapping,
//	OpenStreetMap,
	Kompass,
	Bing,
	SatMap,

	OSM,
	KOMPASS,
	BING,
	SAT,

	ArcGIS,

	ANQUET,

	IGN,
	LUX,
	NGI,

	CH1903
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
	JPEG,
	PNG,
	BMP,
	GIF,
	JPG
};

class CCustomMap
{
public:
	CCustomMap(void);
	~CCustomMap(void);

	CCustomMap(int nCountry, int nProduct, int nLayer);

//CTileDatabase	dbTiles;

	void Update(int nCountry, int nProduct, int nLayer);

	void UpdateCountry(int nCountry);
	void UpdateProduct(int nProduct);
	void UpdateLayer(int nLayer);

//	void UpdateWorkingData();

	int GetCountry();
	int GetProduct();
	int GetLayer();

	int GetDatum();
	int GetMetresPerTile();
	int GetPixelsPerTile();
//	int GetMapScale();
	int GetTileBitDepth();
	int GetTileGfxType();

	int GetCoordinateSystem();

	int GetProjection();

	CString GetMapName(CRect rectBBox);

	int wmsGetPort();
	CString wmsGetAttribution();

CString GetRegistryValueName(int nCountry = -1, int nProduct = -1);
CString GetExampleLatLon(int nCountry = -1);
CString GetExampleGrid(int nCountry = -1);
CString GetCoordSystemName(int nCountry = -1);

bool IsValidScale(int nLayer = -1);

//int GetDefaultScale(int nCountry = -1, int nProduct = -1);

void MakeDatabaseFolders();

void UpdateDatabaseRoot(CString strFolder) { m_strDatabaseRoot = strFolder; };


int GetServerType();

CString GetMapFlavourText(int nCountry = -1, int nProduct = -1);
//CString GetMapScaleText(int nLayer = -1);

//int GetMapScaleIndex(CString strScaleText);

CString GetDatabasePath();

void WriteProfile(/*CRect rectBBox*/CString strGridRef);
void ReadProfile(/*int x, int y*/CString& strGridRef);

CString GetRequest_WMS(int easting, int northing);
CString GetRequest_GetMapping(int easting, int northing);
CString GetRequest_OSM(int easting, int northing);
CString GetRequest_Kompass(int easting, int northing);
CString GetRequest_Bing(int easting, int northing);
CString GetRequest_SatMap(int easting, int northing);
CString GetRequest_IGN(int easting, int northing);
CString GetRequest_Lux(int easting, int northing);
CString GetRequest_NGI(int easting, int northing);
CString GetRequest_CH1903(int easting, int northing);
CString GetRequest_ArcGIS(int easting, int northing);

int GetLevelOfDetail(int nCountry = -1, int nProduct = -1, int nLayer = -1);

CString CountryCode(int nCountry = -1);
CString CountryName(int nCountry = -1);

CString ProductCode(int nCountry = -1, int nProduct = -1);
CString ProductName(int nCountry = -1, int nProduct = -1);

CString LayerCode(int nCountry = -1, int nProduct = -1, int nLayer = -1);
CString LayerName(int nCountry = -1, int nProduct = -1, int nLayer = -1);

CString Referer(int nCountry = -1, int nProduct = -1, int nLayer = -1);

int NumberCountry() { return m_nCountryCount; };
int NumberProducts(int nCountry = -1);
int NumberLayers(int nCountry = -1, int nProduct = -1);

private:

void ParseServerRequestString_WMS(int nCountry, int nProduct, int nLayer, int l);
void ParseServerRequestString_GetMapping(int nCountry, int nProduct, int nLayer, int l);

void ParseServerRequestString_OSM(int nCountry, int nProduct, int nLayer, int l);
void ParseServerRequestString_Kompass(int nCountry, int nProduct, int nLayer, int l);
void ParseServerRequestString_Bing(int nCountry, int nProduct, int nLayer, int l);
void ParseServerRequestString_SatMap(int nCountry, int nProduct, int nLayer, int l);
void ParseServerRequestString_IGN(int nCountry, int nProduct, int nLayer, int l);

void ParseServerRequestString_Lux(int nCountry, int nProduct, int nLayer, int l);
void ParseServerRequestString_NGI(int nCountry, int nProduct, int nLayer, int l);
void ParseServerRequestString_CH1903(int nCountry, int nProduct, int nLayer, int l);

void ParseServerRequestString_ArcGIS(int nCountry, int nProduct, int nLayer, int l);

	void InitialiseArrayCountry();
	void InitialiseArrayProduct();
	void InitialiseArrayLayer();

private:

//	int m_nDatum;

	int m_nCountry;
	int m_nProduct;
	int m_nLayer;

//	int m_nMapScale;
//	int m_nPixelsPerTile;
//	int m_nMetresPerTile;

//	int m_nTileBitDepth;

	int m_nCoordSystem;

	int m_nCountryCount;

	CString m_strDatabaseRoot;

	CRect m_rectBBox;
};
