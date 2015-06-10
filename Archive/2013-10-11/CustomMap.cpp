#include "CustomMap.h"

#include <direct.h>

#include "func.h"
#include "ServerRequests.h"

// 1. Data.
// 2. Parsing Functions.
// 3. Array Initilastion.
// 4. Server Requests
// 5. Country Functions.
// 6. Flavour Functions.
// 7. Scale Functions.

struct CountryData {

	int		nCountry;
	int		nDatum;
	char*	strInternetCode;
	char*	strRegistryValue;
	char*	strCountry;

	CString	strExampleGrid;
	CString strExampleLatLon;

	CString strCoordSystem;

} CountryData[] = {

	{COUNTRY_BE, MAP_DATUM_BE, "be",	"GridReferenceNGI",  "Belgium",			"650000, 671000",	"50.8411° N, 4.3564° E",	"Lambert08"},
	{COUNTRY_UK, MAP_DATUM_UK, "uk",	"GridReferenceOSGB", "Britain",			"TQ 315 815",		"51.5171° N, 0.1062° W",	"OSGB"},
	{COUNTRY_CZ, MAP_DATUM_CZ, "cz",	"GridReferenceCZ",   "Czech Republic",	"459000 5549000",	"50.0833° N, 14.4167° E",	"UTM33"},
	{COUNTRY_DE, MAP_DATUM_DE, "de",	"GridReferenceDE",   "Germany",			"3798000 5831000",	"50.9500° N, 6.9667° E",	"Gauss Krueger"},
	{COUNTRY_FI, MAP_DATUM_FI, "fi",	"GridReferenceFI",   "Finland",			"",					"60.1708° N, 24.9375° E",	""},
	{COUNTRY_SI, MAP_DATUM_SI, "si",	"GridReferenceSI",   "Slovenia",		"",					"46.0556° N, 14.5083° E",	""},
	{COUNTRY_PT, MAP_DATUM_PT, "pt",	"GridReferencePT",   "Portugal",		"",					"38.7138° N, 9.1394° W",	""},
	{COUNTRY_ES, MAP_DATUM_ES, "es",	"GridReferenceES",   "Spain",			"443000 4473000",	"40.4000° N, 3.6833° W",	"ED50"},


	{COUNTRY_CAT, MAP_DATUM_CAT, "cat",	"GridReferenceCat",  "Catalonia",		"",					"41.3833° N, 2.1833° E",	""},
	{COUNTRY_OSM, MAP_DATUM_OSM, "osm",	"GridReferenceOSM",  "Open Street Map",	"",					"54.2439° N, 0.7760° W",	""},
	{COUNTRY_SAT, MAP_DATUM_SAT, "sat",	"GridReferenceSAT",  "SatMap",			"",					"54.2439° N, 0.7760° W",	""},

	{COUNTRY_DK, MAP_DATUM_DK,	"dk",	"GridReferenceDK",  "Denmark",			"",					"54.2439° N, 0.7760° W",	""},


//	{COUNTRY_FR,  MAP_DATUM_FR,		"fr",	"GridReferenceIGN",  "France",			"0652000, 6864000",	"48.8742° N, 2.3470° E",	"Lambert93"},
//	{COUNTRY_PL,  MAP_DATUM_PL,		"pl",	"GridReferencePL",   "Poland",			"",					"",							""},

//	{COUNTRY_AT,  MAP_DATUM_AT,		"at",	"GridReferenceAT",   "Austria",			"",					"",							""},
//	{COUNTRY_IE,  MAP_DATUM_IE,		"ie",	"GridReferenceOSi",  "Eire",			"",					"",							"OSi"},
//	{COUNTRY_IT,  MAP_DATUM_IT,		"it",	"GridReferenceIT",   "Italy",			"",					"",							""},
//	{COUNTRY_LU,  MAP_DATUM_LU,		"lu",	"GridReferenceLux",  "Luxembourg",		"",					"",							""},
//	{COUNTRY_NL,  MAP_DATUM_NL,		"nl",	"GridReferenceNL",   "Netherlands",		"",					"",							""},
//	{COUNTRY_NI,  MAP_DATUM_NI,		"ni",	"GridReferenceOSNi", "Northern Ireland","",					"",							"OSNi"},
//	{COUNTRY_CH,  MAP_DATUM_CH,		"ch",	"GridReferenceCH",   "Switzerland",		"",					"",							"CH1903"},


	{-1, -1, "", "", "", "", "", ""}
};

struct FlavourData {

	int		nCountry;
	int		nProduct;

	char*	strFlavourCode;
	char*	strFlavour;

} FlavourData[] = {

	{COUNTRY_BE, PRODUCT_01, "ngi", "NGI"},

	{COUNTRY_CZ, PRODUCT_01, "cz", "cz"},

	{COUNTRY_UK, MAP_RASTER,   "rm", "Raster"},
	{COUNTRY_UK, MAP_VECTOR,   "vm", "Vector"},
	{COUNTRY_UK, MAP_STREET,   "sv", "Street View"},
	{COUNTRY_UK, MAP_AERIAL,   "ap", "Aerial"},
	{COUNTRY_UK, MAP_HIGHLAND, "hm", "Highland"},

	{COUNTRY_DE, MAP_DE_NRW, "nrw", "North Rhine-Westphalia"},
	{COUNTRY_DE, MAP_DE_BKG, "bkg", "Bundesamt für Kartographie und Geodäsie"},
	{COUNTRY_DE, MAP_DE_MV,  "mv",  "Mecklenburg-Vorpommern"},
	{COUNTRY_DE, MAP_DE_KOMPASS, "km", "Kompass"},

	{COUNTRY_ES, PRODUCT_01, "ngi", "NGI(E)"},

	{COUNTRY_CAT, PRODUCT_01, "mtc", "MTC"},

	{COUNTRY_FI, PRODUCT_01, "fi", "fi"},

	{COUNTRY_SI, PRODUCT_01, "dtk", "DTK"},

	{COUNTRY_PT, PRODUCT_01, "sc", "SC"},

	{COUNTRY_OSM, PRODUCT_01, "vm", "Open Street Map"},
	{COUNTRY_OSM, PRODUCT_02, "km", "Kompass"},
	{COUNTRY_OSM, PRODUCT_03, "bn", "Bing"},
	{COUNTRY_OSM, PRODUCT_04, "sm", "SatMap"},
	{COUNTRY_OSM, PRODUCT_05, "vr", "ViewRanger"},

	{COUNTRY_SAT, PRODUCT_01, "at", "Austria"},
	{COUNTRY_SAT, PRODUCT_02, "be", "Belgium"},
	{COUNTRY_SAT, PRODUCT_03, "de", "Germany"},
	{COUNTRY_SAT, PRODUCT_04, "it", "Italy"},
	{COUNTRY_SAT, PRODUCT_05, "nl", "Netherlands"},
	{COUNTRY_SAT, PRODUCT_06, "ni", "Northern Ireland"},
	{COUNTRY_SAT, PRODUCT_07, "ie", "Eire"},
	{COUNTRY_SAT, PRODUCT_08, "es", "Spain"},
	{COUNTRY_SAT, PRODUCT_09, "ch", "Switzerland"},

	{COUNTRY_DK, PRODUCT_01, "kr", "Krak"},
	{COUNTRY_DK, PRODUCT_02, "ko", "Kort"},


	{-1, -1, "", ""}
};

struct TileData {

	int		nCountry;
	int		nProduct;
	int		nDetail;

	int		nPixels;
	int		nMeters;

	int		nGfxType;
	int		nBitDepth;

	CString	strWMSRequest;

	int nProjection;

	int nServerType;

	CString strProduct;
	CString strDetail;

	int nZoom;

} TileData[] = {

	{COUNTRY_FI, MAP_RASTER, MAP_SCALE_50K, 400, 1000, PNG, 32, WMS_FI_50, 3067, WMS, "", "50", 50},

	{COUNTRY_BE, MAP_RASTER, MAP_SCALE_10K,  250,  250, PNG, 24, WMS_NGI_BE_10,  -1, WMS, "NGI", "10", 10},
	{COUNTRY_BE, MAP_RASTER, MAP_SCALE_50K,  400, 1000, PNG, 24, WMS_NGI_BE_50,  -1, WMS, "NGI", "50", 25},
	{COUNTRY_BE, MAP_RASTER, MAP_SCALE_100K, 400, 2000, PNG, 24, WMS_NGI_BE_100, -1, WMS, "NGI", "100", 100},

	{COUNTRY_CZ, MAP_RASTER, MAP_SCALE_50K,   400,  1000, PNG, 8, WMS_CUZK_50,   -1, WMS, "", "50", 50},
	{COUNTRY_CZ, MAP_RASTER, MAP_SCALE_500K,  400, 10000, PNG, 8, WMS_CUZK_500,  -1, WMS, "", "500", 500},
	{COUNTRY_CZ, MAP_RASTER, MAP_SCALE_2500K, 400, 50000, PNG, 8, WMS_CUZK_2500, -1, WMS, "", "2500", 2500},

	{COUNTRY_DE, MAP_DE_NRW, MAP_SCALE_10K, 250,  250, PNG, 32, WMS_NRW_DTK10, -1, WMS, "NRW", "10", 10},
	{COUNTRY_DE, MAP_DE_NRW, MAP_SCALE_25K, 200,  500, PNG, 32, WMS_NRW_DTK25, -1, WMS, "NRW", "25", 25},
	{COUNTRY_DE, MAP_DE_NRW, MAP_SCALE_50K, 400, 1000, PNG, 32, WMS_NRW_DLM50, -1, WMS, "NRW", "50", 50},
	
	{COUNTRY_DE, MAP_DE_BKG, MAP_SCALE_200K,  500,  4000, PNG, 24, WMS_BKG_200K,  -1, WMS, "BKG", "200", 200},
	{COUNTRY_DE, MAP_DE_BKG, MAP_SCALE_500K,  500, 10000, PNG, 24, WMS_BKG_500K,  -1, WMS, "BKG", "500", 500},
	{COUNTRY_DE, MAP_DE_BKG, MAP_SCALE_1000K, 500, 20000, PNG, 24, WMS_BKG_1000K, -1, WMS, "BKG", "1000", 1000},
	{COUNTRY_DE, MAP_DE_BKG, MAP_SCALE_2500K, 500, 50000, PNG, 24, WMS_BKG_2500K, -1, WMS, "BKG", "2500", 2500},

	{COUNTRY_DE, MAP_DE_MV, MAP_SCALE_10K,   250,   250, PNG, 8, WMS_MV_DTK10,   -1, WMS, "MV", "10", 10},
	{COUNTRY_DE, MAP_DE_MV, MAP_SCALE_25K,   200,   500, PNG, 8, WMS_MV_DTK25,   -1, WMS, "MV", "25", 25},
	{COUNTRY_DE, MAP_DE_MV, MAP_SCALE_50K,   400,  1000, PNG, 8, WMS_MV_DTK50,   -1, WMS, "MV", "50", 50},
	{COUNTRY_DE, MAP_DE_MV, MAP_SCALE_100K,  250,  2000, PNG, 8, WMS_MV_DTK100,  -1, WMS, "MV", "100", 100},
	{COUNTRY_DE, MAP_DE_MV, MAP_SCALE_250K,  250,  5000, PNG, 8, WMS_MV_DTK250,  -1, WMS, "MV", "250", 250},
	{COUNTRY_DE, MAP_DE_MV, MAP_SCALE_750K,  250,  7500, PNG, 8, WMS_MV_DTK750,  -1, WMS, "MV", "750", 750},
	{COUNTRY_DE, MAP_DE_MV, MAP_SCALE_1000K, 250, 10000, PNG, 8, WMS_MV_DTK1000, -1, WMS, "MV", "1000", 1000},

	{COUNTRY_ES, MAP_RASTER, MAP_SCALE_50K,   400, 1000,   PNG, 24, WMS_IGN_ES, -1, WMS, "IGN", "50", 50},
	{COUNTRY_ES, MAP_RASTER, MAP_SCALE_200K,  400, 4000,   PNG, 24, WMS_IGN_ES, -1, WMS, "IGN", "200", 200},
	{COUNTRY_ES, MAP_RASTER, MAP_SCALE_500K,  400, 10000,  PNG, 24, WMS_IGN_ES, -1, WMS, "IGN", "500", 500},
	{COUNTRY_ES, MAP_RASTER, MAP_SCALE_1000K, 400, 20000,  PNG, 24, WMS_IGN_ES, -1, WMS, "IGN", "1000", 1000},
	{COUNTRY_ES, MAP_RASTER, MAP_SCALE_5000K, 400, 100000, PNG, 24, WMS_IGN_ES, -1, WMS, "IGN", "5000", 5000},

	{COUNTRY_CAT, MAP_RASTER, MAP_SCALE_10K,   250,   250, PNG, 24, WMS_MTC_10,   -1, WMS, "MTC", "10", 10},
	{COUNTRY_CAT, MAP_RASTER, MAP_SCALE_25K,   200,   500, PNG, 24, WMS_MTC_25,   -1, WMS, "MTC", "25", 25},
	{COUNTRY_CAT, MAP_RASTER, MAP_SCALE_50K,   400,  1000, PNG, 24, WMS_MTC_50,   -1, WMS, "MTC", "50", 50},
	{COUNTRY_CAT, MAP_RASTER, MAP_SCALE_250K,  400,  5000, PNG, 24, WMS_MTC_250,  -1, WMS, "MTC", "250", 250},
	{COUNTRY_CAT, MAP_RASTER, MAP_SCALE_500K,  400, 10000, PNG, 24, WMS_MTC_500,  -1, WMS, "MTC", "500", 500},
	{COUNTRY_CAT, MAP_RASTER, MAP_SCALE_1000K, 400, 20000, PNG, 24, WMS_MTC_1000, -1, WMS, "MTC", "1000", 1000},	

	{COUNTRY_SI, MAP_RASTER, MAP_SCALE_50K,   200,  1000, PNG, 24, WMS_DTK_50,   -1, WMS, "DTK", "50", 50},
	{COUNTRY_SI, MAP_RASTER, MAP_SCALE_250K,  200,  5000, PNG, 24, WMS_DPK_250,  -1, WMS, "DTK", "250", 250},
	{COUNTRY_SI, MAP_RASTER, MAP_SCALE_500K,  200, 10000, PNG, 24, WMS_DPK_500,  -1, WMS, "DTK", "500", 500},
	{COUNTRY_SI, MAP_RASTER, MAP_SCALE_1000K, 200, 20000, PNG, 24, WMS_DPK_1000, -1, WMS, "", "1000", 1000},

	{COUNTRY_PT, MAP_RASTER, MAP_SCALE_100K, 500,  2000, PNG, 24, WMS_SC_100, -1, WMS, "", "100", 100},
	{COUNTRY_PT, MAP_RASTER, MAP_SCALE_500K, 500, 10000, PNG, 24, WMS_SC_500, -1, WMS, "", "500", 500},


	{COUNTRY_UK, MAP_RASTER, MAP_SCALE_10K,	  250,   250, PNG, 24, WMS_OS_10K,  -1, WMS, "Raster", "10", 10},
	{COUNTRY_UK, MAP_RASTER, MAP_SCALE_25K,	  200,   500, PNG,  8, WMS_OS_25K,  -1, WMS, "Raster", "25", 25},
	{COUNTRY_UK, MAP_RASTER, MAP_SCALE_50K,	  200,  1000, PNG,  8, WMS_OS_50K,  -1, WMS, "Raster", "50", 50},
	{COUNTRY_UK, MAP_RASTER, MAP_SCALE_250K,  200,  5000, PNG,  8, WMS_OS_250K, -1, WMS, "Raster", "250", 250},
	{COUNTRY_UK, MAP_RASTER, MAP_SCALE_1000K, 200, 20000, PNG,  8, WMS_OS_MS,   -1, WMS, "Raster", "1000", 1000},

	{COUNTRY_UK, MAP_VECTOR, MAP_SCALE_10K, 250, 250, PNG, 32, WMS_OS_VML, -1, WMS, "Vector", "10", 10},
	{COUNTRY_UK, MAP_VECTOR, MAP_SCALE_25K, 200, 500, PNG,  8, WMS_OS_VMD, -1, WMS, "Vector", "25", 25},

	{COUNTRY_UK, MAP_STREET, MAP_SCALE_10K, 250, 500, PNG, 8, WMS_OS_SV, -1, WMS, "Street", "10", 10},

	{COUNTRY_UK, MAP_AERIAL, MAP_SCALE_25K, 250, 500, JPG, 32, WMS_GETMAPPING, 27700, GetMapping, "Aerial", "25", 25},

	{COUNTRY_UK, MAP_HIGHLAND, MAP_SCALE_25K, 200,  500, PNG, 24, WMS_HIGHLAND_25, -1, WMS, "Highland", "25", 25},
	{COUNTRY_UK, MAP_HIGHLAND, MAP_SCALE_50K, 200, 1000, PNG, 24, WMS_HIGHLAND_50, -1, WMS, "Highland", "50", 50},

	{COUNTRY_OSM, PRODUCT_01, MAP_SCALE_50K, 256, 1000, PNG,  8, WMS_OSM, 3857, OpenStreetMap, "OSM", "18", 18},
	{COUNTRY_OSM, PRODUCT_02, MAP_SCALE_50K, 256, 1000, JPG, 32, WMS_Kompass, 3857, Kompass, "Kompass", "15", 15},
	{COUNTRY_OSM, PRODUCT_03, MAP_SCALE_50K, 256, 1000, JPG, 32, WMS_BING, 900913, Bing, "Bing", "15", 15},
	{COUNTRY_OSM, PRODUCT_04, MAP_SCALE_50K, 256, 1000, JPG, 32, WMS_SATMAP, 900913, SatMap, "AT", "13", 13},

	{COUNTRY_OSM, PRODUCT_05, MAP_SCALE_50K, 256, 1000, JPG, 32, WMS_OSM, 3857, OpenStreetMap, "View", "10", 10},

/*
#define SATMAP_AT	"http://tileserver.satmap.com/AT/13/1202300303311.ipic"
#define SATMAP_BE	"http://tileserver.satmap.com/BE/14/12020231021132.ipic"
#define SATMAP_DE	"http://tileserver.satmap.com/DE/13/1202031303130.ipic"
#define SATMAP_IT	"http://tileserver.satmap.com/IT/13/1202302022002.ipic"
#define SATMAP_NL	"http://tileserver.satmap.com/NL/13/1202021112121.ipic"
#define SATMAP_NI	"http://tileserver.satmap.com/NI/13/0313101300333.ipic"
#define SATMAP_IE	"http://tileserver.satmap.com/IE/14/03131032003120.ipic"
#define SATMAP_ES	"http://tileserver.satmap.com/ES/10/0331121110.ipic"
#define SATMAP_CH	"http://tileserver.satmap.com/CH/12/120221300023.ipic"
#define SATMAP_FR	"http://s3-eu-west-1.amazonaws.com/france-100k/1202202132320.ipic"
*/

	{COUNTRY_SAT, PRODUCT_01, DETAIL_01, 256, 1000, JPG, 32, SATMAP_AT, 900913, SatMap, "AT", "13", 13},
	{COUNTRY_SAT, PRODUCT_02, DETAIL_01, 256, 1000, PNG,  8, SATMAP_BE, 900913, SatMap, "BE", "14", 14},
	{COUNTRY_SAT, PRODUCT_03, DETAIL_01, 256, 1000, JPG, 32, SATMAP_DE, 900913, SatMap, "DE", "14", 14},
	{COUNTRY_SAT, PRODUCT_04, DETAIL_01, 256, 1000, PNG,  8, SATMAP_IT, 900913, SatMap, "IT", "13", 13},
	{COUNTRY_SAT, PRODUCT_05, DETAIL_01, 256, 1000, PNG, 32, SATMAP_NL, 900913, SatMap, "NL", "13", 13},
	{COUNTRY_SAT, PRODUCT_06, DETAIL_01, 256, 1000, PNG, 32, SATMAP_NI, 900913, SatMap, "NI", "13", 13},
	{COUNTRY_SAT, PRODUCT_07, DETAIL_01, 256, 1000, PNG,  8, SATMAP_IE, 900913, SatMap, "IE", "14", 14},
	{COUNTRY_SAT, PRODUCT_08, DETAIL_01, 256, 1000, PNG, 32, SATMAP_ES, 900913, SatMap, "ES", "10", 10},
	{COUNTRY_SAT, PRODUCT_09, DETAIL_01, 256, 1000, JPG, 32, SATMAP_CH, 900913, SatMap, "CH", "12", 12 },
//	{COUNTRY_SAT, PRODUCT_10, DETAIL_01, 256, 1000, JPG, 32, SATMAP_FR, 900913, SatMap, "FR", "france-100k", 10},

	{COUNTRY_DK, PRODUCT_01, DETAIL_01, 256, 1000, PNG, 32, WMS_KRAK, 25832, WMS, "Krak", "50", 50},
	{COUNTRY_DK, PRODUCT_02, DETAIL_01, 256, 1000, PNG, 32, WMS_KORT, -1, WMS, "Kort", "50", 50},


	{-1, -1, -1, -1, -1, -1, -1, "", -1, -1, "", ""},
};

struct TileParameters {

	int	nPixels;
	int	nMeters;

	int	nGfxType;
	int	nBitDepth;

	CString strWebsite;
	CString strServer;
	CString strAttribution;
	CString strWMSLayer;
	CString strService;
	CString strVersion;
	CString strFormat;
	CString strBBox;
	CString strWidth;
	CString strHeight;
	CString strExceptions;
	CString strKey;
	CString strURL;
	CString strRefSystem;
	CString strEPGS;
	CString strImage;

	int nProjection;
	int nPort;
	int nServerType;

	CString strProduct;
	CString strDetail;

} TileParameters[MAX_COUNTRY][MAX_PRODUCTS][MAX_DETAILS];

struct CountryParameters {

	int		nCountry;
	int		nDatum;
	CString	strInternetCode;
	CString	strRegistryValue;
	CString	strCountry;
	int		nCoordSystem;
	CString strExampleLatLon;
	CString strExampleGrid;
	CString strCoordSystem;

} CountryParameters[MAX_COUNTRY];

struct FlavourParameters {

	int		nCountry;
	int		nProduct;

	CString strFlavourCode;
	CString strFlavour;

	int		nDefaultScale;

} FlavourParameters[MAX_COUNTRY][MAX_PRODUCTS];

struct MapScales {

// nDetail;
	int		nDetail;
	CString	strScale;
	CString	strFolder;

} MapScales[] = {

	{  10000,   "10K",   "10"},
	{  25000,   "25K",   "25"},
	{  50000,   "50K",   "50"},
	{ 100000,  "100K",  "100"},
	{ 200000,  "200K",  "200"},
	{ 250000,  "250K",  "250"},
	{ 500000,  "500K",  "500"},
	{ 750000,  "750K",  "750"},
	{1000000, "1000K", "1000"},
	{2000000, "2000K", "2000"},
	{2500000, "2500K", "2500"},
	{5000000, "5000K", "5000"},

	{-1, "", ""}
};

CString CCustomMap::GetScaleCode(int nDetail)
{
	if (nDetail < 0) {
		nDetail = m_nDetail;
	}

	return MapScales[nDetail].strFolder;
}

// Parsing Routines.

CString WMS_ExtractWebsiteServer(CString str)
{
	CString strWebsite;
	int nPos;
	
	strWebsite.Empty();
	nPos = str.Find("http://");
	if (nPos == -1) {
		strWebsite.Empty();
		return strWebsite;
	}
	strWebsite = str.Mid(nPos+7);
	nPos = strWebsite.Find("/");
	if (nPos == -1) {
		strWebsite.Empty();
		return strWebsite;
	}
	strWebsite = strWebsite.Mid(0,nPos);
	return strWebsite;
}

CString WMS_ExtractDomain(CString str)
{
	int nPos = str.Find(":");
	if (nPos == -1) {
		return str;
	}
	str = str.Left(nPos);
	return str;
}

int WMS_ExtractPort(CString str)
{
	int nPort=80;

	int nPos = str.Find(":");
	if (nPos != -1) {
		str = str.Mid(nPos+1);
		nPort = atoi(str);
	}
	return nPort;
}

CString WMS_ExtractLayer(CString str)
{
	CString strLayer;
	int nPos;

	strLayer.Empty();
	nPos = str.Find("?");
	if (nPos == -1) {
		strLayer.Empty();
		return strLayer;
	}
	strLayer = str.Left(nPos);
	nPos = strLayer.Find("http://");
	if (nPos == -1) {
		strLayer.Empty();
		return strLayer;
	}
	strLayer = strLayer.Mid(nPos+7);
	nPos = strLayer.Find("/");
	if (nPos == -1) {
		strLayer.Empty();
		return strLayer;
	}
	strLayer = strLayer.Mid(nPos+1);
	return strLayer;
}

int WMS_ExtractEPGS(CString str)
{
	int nPos;
	int nEPGS = 0;

	nPos = str.Find("EPSG%3A");
	if (nPos != -1) {
		str = str.Mid(nPos+7);
		nEPGS = atol(str);
	} else {
		nPos = str.Find("EPSG:");
		if (nPos != -1) {
			str = str.Mid(nPos+5);
			nEPGS = atol(str);
		}
	}
	return nEPGS;
}

int WMS_ExtractImage(CString str)
{
	CString strImage;
	int nPos;

	strImage.Empty();
	nPos = str.Find("image%2F");
	if (nPos != -1) {
		strImage = str.Mid(nPos+8);
	} else {
		nPos = str.Find("image/");
		if (nPos != -1) {
			strImage = str.Mid(nPos+6);
		}
	}

	if (strImage == "png") return PNG;
	if (strImage == "jpeg") return JPG;
	if (strImage == "bmp") return BMP;
	if (strImage == "gif") return GIF;

	return -1;
}

CString WMS_ExtractParameter(CString str, CString strParameter)
{
	CString strValue;
	int nPos;

	strValue.Empty();
	nPos = str.Find(strParameter);
	if (nPos == -1) {
		strValue.Empty();
		return strValue;
	}
	int nLength = strParameter.GetLength();
	strValue = str.Mid(nPos+nLength);
	nPos = strValue.Find("&");
	if (nPos == -1) {
		strValue.Empty();
		return strValue;
	}
	strValue = strValue.Mid(0,nPos);
	return strValue;
}

// http://tileserver.satmap.com/AT/14/12023002320313.ipic
void CCustomMap::ParseServerRequestString_SatMap(int nCountry, int nProduct, int nDetail, int l)
{
	TileParameters[nCountry][nProduct][nDetail].strWebsite = WMS_ExtractWebsiteServer(TileData[l].strWMSRequest);
	TileParameters[nCountry][nProduct][nDetail].strServer = WMS_ExtractLayer(TileData[l].strWMSRequest);
	TileParameters[nCountry][nProduct][nDetail].nPort = WMS_ExtractPort(TileParameters[nCountry][nProduct][nDetail].strWebsite);
	TileParameters[nCountry][nProduct][nDetail].strAttribution = WMS_ExtractDomain(TileParameters[nCountry][nProduct][nDetail].strWebsite);
	TileParameters[nCountry][nProduct][nDetail].nGfxType = TileData[l].nGfxType;
	TileParameters[nCountry][nProduct][nDetail].nProjection = TileData[l].nProjection;
}

// #define WMS_BING	"http://a1.ortho.tiles.virtualearth.net/tiles/a031311321031011.jpeg?g=52"
void CCustomMap::ParseServerRequestString_Bing(int nCountry, int nProduct, int nDetail, int l)
{
	TileParameters[nCountry][nProduct][nDetail].strWebsite = WMS_ExtractWebsiteServer(TileData[l].strWMSRequest);
	TileParameters[nCountry][nProduct][nDetail].strServer = WMS_ExtractLayer(TileData[l].strWMSRequest);
	TileParameters[nCountry][nProduct][nDetail].nPort = WMS_ExtractPort(TileParameters[nCountry][nProduct][nDetail].strWebsite);
	TileParameters[nCountry][nProduct][nDetail].strAttribution = WMS_ExtractDomain(TileParameters[nCountry][nProduct][nDetail].strWebsite);
	TileParameters[nCountry][nProduct][nDetail].nGfxType = TileData[l].nGfxType;
	TileParameters[nCountry][nProduct][nDetail].nProjection = TileData[l].nProjection;
}

// #define WMS_Kompass		"http://ec2.cdn.ecmaps.de/WmsGateway.ashx.jpg?Experience=kompass&MapStyle=KOMPASS%20Touristik&TileX=17018&TileY=10975&ZoomLevel=15"

void CCustomMap::ParseServerRequestString_Kompass(int nCountry, int nProduct, int nDetail, int l)
{
	TileParameters[nCountry][nProduct][nDetail].strWebsite = WMS_ExtractWebsiteServer(TileData[l].strWMSRequest);
	TileParameters[nCountry][nProduct][nDetail].strServer = WMS_ExtractLayer(TileData[l].strWMSRequest);
	TileParameters[nCountry][nProduct][nDetail].nPort = WMS_ExtractPort(TileParameters[nCountry][nProduct][nDetail].strWebsite);
	TileParameters[nCountry][nProduct][nDetail].strAttribution = WMS_ExtractDomain(TileParameters[nCountry][nProduct][nDetail].strWebsite);
	TileParameters[nCountry][nProduct][nDetail].strWMSLayer = WMS_ExtractParameter(TileData[l].strWMSRequest, "size=");
	TileParameters[nCountry][nProduct][nDetail].nGfxType = TileData[l].nGfxType;
	TileParameters[nCountry][nProduct][nDetail].nProjection = TileData[l].nProjection;
}

// http://tile.openstreetmap.org/18/130584/83719.png
void CCustomMap::ParseServerRequestString_OSM(int nCountry, int nProduct, int nDetail, int l)
{
	TileParameters[nCountry][nProduct][nDetail].strWebsite = WMS_ExtractWebsiteServer(TileData[l].strWMSRequest);
	TileParameters[nCountry][nProduct][nDetail].strServer = WMS_ExtractLayer(TileData[l].strWMSRequest);
	TileParameters[nCountry][nProduct][nDetail].nPort = WMS_ExtractPort(TileParameters[nCountry][nProduct][nDetail].strWebsite);
	TileParameters[nCountry][nProduct][nDetail].strAttribution = WMS_ExtractDomain(TileParameters[nCountry][nProduct][nDetail].strWebsite);
	TileParameters[nCountry][nProduct][nDetail].nGfxType = TileData[l].nGfxType;
	TileParameters[nCountry][nProduct][nDetail].nProjection = TileData[l].nProjection;
}

void CCustomMap::ParseServerRequestString_GetMapping(int nCountry, int nProduct, int nDetail, int l)
{
	TileParameters[nCountry][nProduct][nDetail].strWebsite = WMS_ExtractWebsiteServer(TileData[l].strWMSRequest);
	TileParameters[nCountry][nProduct][nDetail].strServer = WMS_ExtractLayer(TileData[l].strWMSRequest);
	TileParameters[nCountry][nProduct][nDetail].nPort = WMS_ExtractPort(TileParameters[nCountry][nProduct][nDetail].strWebsite);
	TileParameters[nCountry][nProduct][nDetail].strAttribution = WMS_ExtractDomain(TileParameters[nCountry][nProduct][nDetail].strWebsite);
	TileParameters[nCountry][nProduct][nDetail].strWMSLayer = WMS_ExtractParameter(TileData[l].strWMSRequest, "size=");
	TileParameters[nCountry][nProduct][nDetail].nGfxType = TileData[l].nGfxType;
	TileParameters[nCountry][nProduct][nDetail].nProjection = TileData[l].nProjection;
}

void CCustomMap::ParseServerRequestString_WMS(int nCountry, int nProduct, int nDetail, int l)
{
	TileParameters[nCountry][nProduct][nDetail].strWebsite = WMS_ExtractWebsiteServer(TileData[l].strWMSRequest);
	TileParameters[nCountry][nProduct][nDetail].strServer = WMS_ExtractLayer(TileData[l].strWMSRequest);
	TileParameters[nCountry][nProduct][nDetail].strWMSLayer = WMS_ExtractParameter(TileData[l].strWMSRequest, "LAYERS=");
	TileParameters[nCountry][nProduct][nDetail].strService = WMS_ExtractParameter(TileData[l].strWMSRequest, "SERVICE=");
	TileParameters[nCountry][nProduct][nDetail].strVersion = WMS_ExtractParameter(TileData[l].strWMSRequest, "VERSION=");
	TileParameters[nCountry][nProduct][nDetail].strFormat = WMS_ExtractParameter(TileData[l].strWMSRequest, "FORMAT=");
	TileParameters[nCountry][nProduct][nDetail].strBBox = WMS_ExtractParameter(TileData[l].strWMSRequest, "BBOX=");
	TileParameters[nCountry][nProduct][nDetail].strWidth = WMS_ExtractParameter(TileData[l].strWMSRequest, "WIDTH=");
	TileParameters[nCountry][nProduct][nDetail].strHeight = WMS_ExtractParameter(TileData[l].strWMSRequest, "HEIGHT=");
	TileParameters[nCountry][nProduct][nDetail].strExceptions = WMS_ExtractParameter(TileData[l].strWMSRequest, "Exceptions=");
	TileParameters[nCountry][nProduct][nDetail].strKey = WMS_ExtractParameter(TileData[l].strWMSRequest, "KEY=");
	TileParameters[nCountry][nProduct][nDetail].strURL = WMS_ExtractParameter(TileData[l].strWMSRequest, "URL=");

	if ((TileParameters[nCountry][nProduct][nDetail].strVersion == "1.3.0") || (TileParameters[nCountry][nProduct][nDetail].strVersion == "1.3")) {
		TileParameters[nCountry][nProduct][nDetail].strEPGS = WMS_ExtractParameter(TileData[l].strWMSRequest, "CRS=");
		TileParameters[nCountry][nProduct][nDetail].strRefSystem = "CRS";
	} else {
		TileParameters[nCountry][nProduct][nDetail].strEPGS = WMS_ExtractParameter(TileData[l].strWMSRequest, "SRS=");
		TileParameters[nCountry][nProduct][nDetail].strRefSystem = "SRS";
	}

	if (TileData[l].nProjection == -1) {
		TileParameters[nCountry][nProduct][nDetail].nProjection = WMS_ExtractEPGS(TileParameters[nCountry][nProduct][nDetail].strEPGS);
	} else {
		TileParameters[nCountry][nProduct][nDetail].nProjection = TileData[l].nProjection;
	}

	TileParameters[nCountry][nProduct][nDetail].nPort = WMS_ExtractPort(TileParameters[nCountry][nProduct][nDetail].strWebsite);
	TileParameters[nCountry][nProduct][nDetail].strAttribution = WMS_ExtractDomain(TileParameters[nCountry][nProduct][nDetail].strWebsite);

	if (TileData[l].nGfxType == -1) {
		TileParameters[nCountry][nProduct][nDetail].nGfxType = WMS_ExtractImage(TileParameters[nCountry][nProduct][nDetail].strFormat);
	} else {
		TileParameters[nCountry][nProduct][nDetail].nGfxType = TileData[l].nGfxType;
	}
}

// Array Initilisation.

void CCustomMap::InitialiseParameterArrayTile()
{
	for (int i=0; i < MAX_COUNTRY; i++) {
		for (int j=0; j < MAX_PRODUCTS; j++) {
			for (int k=0; k < MAX_DETAILS; k++) {

				TileParameters[i][j][k].nPixels = -1;
				TileParameters[i][j][k].nMeters = -1;
				TileParameters[i][j][k].nGfxType = -1;
				TileParameters[i][j][k].nBitDepth = -1;

				TileParameters[i][j][k].strWebsite.Empty();
				TileParameters[i][j][k].strServer.Empty();
				TileParameters[i][j][k].strAttribution.Empty();
				TileParameters[i][j][k].strWMSLayer.Empty();
				TileParameters[i][j][k].strService.Empty();
				TileParameters[i][j][k].strVersion.Empty();
				TileParameters[i][j][k].strFormat.Empty();
				TileParameters[i][j][k].strBBox.Empty();
				TileParameters[i][j][k].strWidth.Empty();
				TileParameters[i][j][k].strHeight.Empty();
				TileParameters[i][j][k].strExceptions.Empty();
				TileParameters[i][j][k].strKey.Empty();
				TileParameters[i][j][k].strURL.Empty();
				TileParameters[i][j][k].strRefSystem.Empty(); 
				TileParameters[i][j][k].strImage.Empty();

				TileParameters[i][j][k].nProjection = -1;
				TileParameters[i][j][k].nPort = -1;
			
				TileParameters[i][j][k].strProduct.Empty();
				TileParameters[i][j][k].strDetail.Empty();
			}
		}
	}

	for (int l=0; true; l++) {

		int nCountry = TileData[l].nCountry;
		int nProduct = TileData[l].nProduct;
		int nDetail   = TileData[l].nDetail;

		if (nCountry == -1) break;

		TileParameters[nCountry][nProduct][nDetail].nPixels = TileData[l].nPixels;
		TileParameters[nCountry][nProduct][nDetail].nMeters = TileData[l].nMeters;
		TileParameters[nCountry][nProduct][nDetail].nBitDepth = TileData[l].nBitDepth;
		TileParameters[nCountry][nProduct][nDetail].nServerType = TileData[l].nServerType;

		if      (TileData[l].nServerType == WMS)			ParseServerRequestString_WMS(nCountry, nProduct, nDetail, l);
		else if (TileData[l].nServerType == GetMapping)		ParseServerRequestString_GetMapping(nCountry, nProduct, nDetail, l);
		else if (TileData[l].nServerType == OpenStreetMap)	ParseServerRequestString_OSM(nCountry, nProduct, nDetail, l);
		else if (TileData[l].nServerType == Kompass)		ParseServerRequestString_Kompass(nCountry, nProduct, nDetail, l);
		else if (TileData[l].nServerType == Bing)			ParseServerRequestString_Bing(nCountry, nProduct, nDetail, l);
		else if (TileData[l].nServerType == SatMap)			ParseServerRequestString_SatMap(nCountry, nProduct, nDetail, l);

		if (FlavourParameters[nCountry][nProduct].nDefaultScale == -1) FlavourParameters[nCountry][nProduct].nDefaultScale = nDetail;

		TileParameters[nCountry][nProduct][nDetail].strProduct = TileData[l].strProduct;
		TileParameters[nCountry][nProduct][nDetail].strDetail  = TileData[l].strDetail;
	}
}

void CCustomMap::InitialiseParameterArrayCountry()
{
	for (int i=0; i < MAX_COUNTRY; i++) {

		CountryParameters[i].nDatum = -1;
		CountryParameters[i].strInternetCode.Empty();
		CountryParameters[i].strRegistryValue.Empty();
		CountryParameters[i].strCountry.Empty();
		CountryParameters[i].nCoordSystem = -1;
	}

	int l;
	for (l=0; true; l++) {

		int nCountry = CountryData[l].nCountry;

		if (nCountry == -1) break;

		CountryParameters[nCountry].nDatum = CountryData[l].nDatum;
		CountryParameters[nCountry].strInternetCode = CountryData[l].strInternetCode;
		CountryParameters[nCountry].strRegistryValue = CountryData[l].strRegistryValue;
		CountryParameters[nCountry].strCountry = CountryData[l].strCountry;
		CountryParameters[nCountry].strExampleGrid = CountryData[l].strExampleGrid;
		CountryParameters[nCountry].strExampleLatLon = CountryData[l].strExampleLatLon;
		CountryParameters[nCountry].strCoordSystem = CountryData[l].strCoordSystem;
	}
	m_nCountryCount = l;
}

void CCustomMap::InitialiseParameterArrayFlavour()
{
	for (int i=0; i < MAX_COUNTRY; i++) {
		for (int j=0; j < MAX_PRODUCTS; j++) {

			FlavourParameters[i][j].strFlavourCode.Empty();
			FlavourParameters[i][j].strFlavour.Empty();
			FlavourParameters[i][j].nDefaultScale = -1;
		}
	}

	for (int l=0; true; l++) {

		int nCountry = FlavourData[l].nCountry;
		int nProduct = FlavourData[l].nProduct;

		if (nCountry == -1) break;

		FlavourParameters[nCountry][nProduct].strFlavourCode = FlavourData[l].strFlavourCode;
		FlavourParameters[nCountry][nProduct].strFlavour = FlavourData[l].strFlavour;
	}
	//nCountProducts = l;

}

CString CCustomMap::GetFlavourName(int nCountry, int nProduct)
{
	if (nCountry < 0) {
		nCountry = m_nCountry;
	}
	if (nProduct < 0) {
		nProduct = m_nProduct;
	}
	return FlavourParameters[nCountry][nProduct].strFlavour;
}

CString CCustomMap::GetFlavourCode(int nCountry, int nProduct)
{
	if (nCountry < 0) {
		nCountry = m_nCountry;
	}
	if (nProduct < 0) {
		nProduct = m_nProduct;
	}
	return FlavourParameters[nCountry][nProduct].strFlavourCode;
}

int CCustomMap::GetDefaultScale(int nCountry, int nProduct)
{
	if (nCountry < 0) {
		nCountry = m_nCountry;
	}
	if (nProduct < 0) {
		nProduct = m_nProduct;
	}
	return FlavourParameters[nCountry][nProduct].nDefaultScale;
}

CString CCustomMap::GetMapScaleText(int nDetail)
{
	return MapScales[nDetail].strScale;
}

int CCustomMap::GetMapScaleIndex(CString strScaleText)
{
	for (int i=0; true; i++) {
		if (MapScales[i].nDetail == -1) break;
		if (MapScales[i].strScale == strScaleText) return i;
	}
	return -1;
}


CString CCustomMap::GetCountryName(int nCountry)
{
	if (nCountry < 0) {
		nCountry = m_nCountry;
	}
	return CountryParameters[nCountry].strCountry;
}

CString CCustomMap::GetRegistryValueName(int nCountry, int nProduct)
{
	if (nCountry < 0) {
		nCountry = m_nCountry;
	}
	if (nProduct < 0) {
		nProduct = m_nProduct;
	}

	CString str;

	str.Format("GridReference.%s$%s",
				CountryParameters[nCountry].strInternetCode,
				FlavourParameters[nCountry][nProduct].strFlavourCode);
	
//	return CountryParameters[nCountry].strRegistryValue;
	return str;
}

CString CCustomMap::GetInternetCode(int nCountry)
{
	if (nCountry < 0) {
		nCountry = m_nCountry;
	}
	return CountryParameters[nCountry].strInternetCode;
}

CString CCustomMap::GetExampleLatLon(int nCountry)
{
	if (nCountry < 0) {
		nCountry = m_nCountry;
	}
	return CountryParameters[nCountry].strExampleLatLon;
}

CString CCustomMap::GetExampleGrid(int nCountry)
{
	if (nCountry < 0) {
		nCountry = m_nCountry;
	}
	return CountryParameters[nCountry].strExampleGrid;
}

CString CCustomMap::GetCoordSystemName(int nCountry)
{
	if (nCountry < 0) {
		nCountry = m_nCountry;
	}
	return CountryParameters[nCountry].strCoordSystem;
}

bool CCustomMap::IsValidScale(int nDetail)
{
	if (nDetail < 0) {
		nDetail = m_nDetail;
	}

	bool bReturn = true;

	if (TileParameters[m_nCountry][m_nProduct][nDetail].nMeters == -1) {
		bReturn = false;
	}
	return bReturn;
}

char* UK_MapFlavours[] = {
	"Raster",
	"Vector",
	"Street",
	"Aerial",
	"Higland",
};

char* DE_MapFlavours[] = {
	"NRW",
	"MV",
	"BKG",
};

char* ES_MapFlavours[] = {
	"NGI ES",
	"Catalonia",
};

CString CCustomMap::GetMapFlavourText(int nCountry, int nProduct)
{
	if (nCountry == COUNTRY_DE) {
		return DE_MapFlavours[nProduct];
	} else {
		return UK_MapFlavours[nProduct];
	}
}

void CCustomMap::MakeDatabaseFolders()
{
	CString strFolder;

	strFolder.Format("%s\\%s", m_strDatabaseRoot, GetInternetCode());
	_mkdir(strFolder);

	strFolder.Format("%s\\%s\\%s", m_strDatabaseRoot, GetInternetCode(), GetFlavourCode());
	_mkdir(strFolder);

	strFolder.Format("%s\\%s\\%s\\%s", m_strDatabaseRoot, GetInternetCode(), GetFlavourCode(), GetScaleCode());
	_mkdir(strFolder);
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

 
Those first examples used the default projection, 4326 (which OL uses as shorthand for the Plate Carrée projection, i.e. geographical coordinates used as x/y coordinates, i.e. pretending the earth is a flat rectangle). Now let's look at an example using a 'proper' projection. For this, I'll use the Spanish WMS in one of the UTM projections, EPSG:23030.
 
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

// http://ec2.cdn.ecmaps.de/WmsGateway.ashx.jpg?Experience=kompass&MapStyle=KOMPASS%20Touristik&TileX=17018&TileY=10975&ZoomLevel=15
// http://sg.geodatenzentrum.de/wms_dtk500?LAYERS=DTK500&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image%2Fjpeg&SRS=EPSG%3A31467&BBOX=3779760,5802440,3797680,5820360&WIDTH=256&HEIGHT=256

CCustomMap::CCustomMap(void)
{
	InitialiseParameterArrayCountry();

	InitialiseParameterArrayFlavour();

	InitialiseParameterArrayTile();
}

CCustomMap::~CCustomMap(void)
{
}

CCustomMap::CCustomMap(int nCountry, int nProduct, int nDetail)
{
	m_nCountry = nCountry;
	m_nProduct = nProduct;
	m_nDetail  = nDetail;
//	UpdateWorkingData();
}

void CCustomMap::Update(int nCountry, int nProduct, int nDetail)
{
	m_nCountry = nCountry;
	m_nProduct = nProduct;
	m_nDetail  = nDetail;
//	UpdateWorkingData();
}

void CCustomMap::UpdateCountry(int nCountry)
{
	m_nCountry = nCountry;
	AfxGetApp()->WriteProfileInt("Main", "RadioCountry", nCountry);
//	UpdateWorkingData();
}

void CCustomMap::UpdateProduct(int nProduct)
{
	m_nProduct = nProduct;
	AfxGetApp()->WriteProfileInt("Main", "RadioFlavour", nProduct);
//	UpdateWorkingData();
}

void CCustomMap::UpdateDetail(int nDetail)
{
	m_nDetail = nDetail;
	AfxGetApp()->WriteProfileInt("Main", "RadioScale", nDetail);
//	UpdateWorkingData();
}

/*
void CCustomMap::UpdateWorkingData()
{
//	m_nDatum = CountryData[m_nCountry].nDatum;
//	m_nMapScale = MapScales[m_nDetail].nDetail;

//	m_nPixelsPerTile = TileParameters[m_nCountry][m_nProduct][m_nDetail].nPixels;
//	m_nMetresPerTile = TileParameters[m_nCountry][m_nProduct][m_nDetail].nMeters;
//	m_nTileBitDepth  = TileParameters[m_nCountry][m_nProduct][m_nDetail].nBitDepth;

//m_nCoordSystem = CountryParameters[m_nCountry].nCoordSystem;
}
*/

CString CCustomMap::GetMapName(CRect rectBBox)
{
	CString strMapName;

	strMapName.Format("%s_%s%s_%d_%d",
					GetInternetCode(),
					GetFlavourCode(),
					GetScaleCode(),
					rectBBox.CenterPoint().x,
					rectBBox.CenterPoint().y);

	return strMapName;
}

int CCustomMap::GetProjection() 
{
	return TileParameters[m_nCountry][m_nProduct][m_nDetail].nProjection;
}

int CCustomMap::GetServerType() 
{	
	return TileParameters[m_nCountry][m_nProduct][m_nDetail].nServerType;
}

CString CCustomMap::wmsGetAttribution() { return TileParameters[m_nCountry][m_nProduct][m_nDetail].strAttribution; }

int CCustomMap::wmsGetPort() { return TileParameters[m_nCountry][m_nProduct][m_nDetail].nPort; }

CString GfxTypeMapping[] = {"jpeg", "png", "bmp", "gif"};

// #define WMS_KRAK	"http://webmapservice.krak.dk/wms/krakbykort?kwuid=demo@krakwms.dk&kwpwd=uyu8tSdD&nbr&request=GetMap&version=1.1.1&service=WMS&bbox=705576,6203416,705832,6203672&format=image/png&width=256&height=256&LAYERS=krakbykort&SRS=EPSG:25832"

CString CCustomMap::wmsGetRequest(int easting, int northing)
{
	int nPixels = TileParameters[m_nCountry][m_nProduct][m_nDetail].nPixels;
	int nMetres = TileParameters[m_nCountry][m_nProduct][m_nDetail].nMeters;

	CString strRequest;

	if (m_nCountry == COUNTRY_DK) {
		strRequest.Format(
				"%s"
				"?LAYERS=%s"
				"&SERVICE=WMS&VERSION=%s"
				"&REQUEST=GetMap"
				"&STYLES="
				"&FORMAT=image%%2F%s"
				"&%s=EPSG%%3A%d"
				"&kwuid=demo@krakwms.dk&kwpwd=uyu8tSdD&nbr"
				"kwpwd=uyu8tSdD&nbr"
				"&BBOX=%d,%d,%d,%d"
				"&WIDTH=%d&HEIGHT=%d",
				TileParameters[m_nCountry][m_nProduct][m_nDetail].strServer,
				TileParameters[m_nCountry][m_nProduct][m_nDetail].strWMSLayer,
				TileParameters[m_nCountry][m_nProduct][m_nDetail].strVersion,
				GfxTypeMapping[TileParameters[m_nCountry][m_nProduct][m_nDetail].nGfxType],
				TileParameters[m_nCountry][m_nProduct][m_nDetail].strRefSystem, TileParameters[m_nCountry][m_nProduct][m_nDetail].nProjection,
	//			TileParameters[m_nCountry][m_nProduct][m_nDetail].strURL,
	//			TileParameters[m_nCountry][m_nProduct][m_nDetail].strKey,
				easting, northing, easting + nMetres, northing + nMetres,
				nPixels, nPixels);
	} else {
		strRequest.Format(
				"%s"
				"?LAYERS=%s"
				"&SERVICE=WMS&VERSION=%s"
				"&REQUEST=GetMap"
				"&STYLES="
				"&FORMAT=image%%2F%s"
				"&%s=EPSG%%3A%d"
				"&URL=%s"
				"&KEY=%s"
				"&BBOX=%d,%d,%d,%d"
				"&WIDTH=%d&HEIGHT=%d",
				TileParameters[m_nCountry][m_nProduct][m_nDetail].strServer,
				TileParameters[m_nCountry][m_nProduct][m_nDetail].strWMSLayer,
				TileParameters[m_nCountry][m_nProduct][m_nDetail].strVersion,
				GfxTypeMapping[TileParameters[m_nCountry][m_nProduct][m_nDetail].nGfxType],
				TileParameters[m_nCountry][m_nProduct][m_nDetail].strRefSystem, TileParameters[m_nCountry][m_nProduct][m_nDetail].nProjection,
				TileParameters[m_nCountry][m_nProduct][m_nDetail].strURL,
				TileParameters[m_nCountry][m_nProduct][m_nDetail].strKey,
				easting, northing, easting + nMetres, northing + nMetres,
				nPixels, nPixels);
	}

	return strRequest;
}

// #define WMS_GETMAPPING	"http://www.getmapping.com/preview/preview.ashx?X=486463.0&Y=496681.5&productID=9&dataSet=MM&size=500.0"

CString CCustomMap::getmappingGetRequest(int easting, int northing)
{
	double dMetres = TileParameters[m_nCountry][m_nProduct][m_nDetail].nMeters / 2.0;

	CString strRequest;

	strRequest.Format(
			"%s"
			"?size=%s"
			"&X=%.1f&Y=%.1f",
			TileParameters[m_nCountry][m_nProduct][m_nDetail].strServer,
			TileParameters[m_nCountry][m_nProduct][m_nDetail].strWMSLayer,
			easting + dMetres, northing + dMetres);

	return strRequest;

}

// http://tile.openstreetmap.org/18/130584/83719.png
CString CCustomMap::osmGetRequest(int easting, int northing)
{
	CString strRequest;

	strRequest.Format(
			"%s"
			"15/%d/%d.png",
			TileParameters[m_nCountry][m_nProduct][m_nDetail].strServer,
			easting, northing);

	return strRequest;

}

//#define WMS_Kompass		"http://ec2.cdn.ecmaps.de/WmsGateway.ashx.jpg?Experience=kompass&MapStyle=KOMPASS%20Touristik&TileX=17018&TileY=10975&ZoomLevel=15"
CString CCustomMap::kompassGetRequest(int easting, int northing)
{
	CString strRequest;

	strRequest.Format(
			"%s"
			"?Experience=kompass&MapStyle=KOMPASS%%20Touristik&TileX=%d&TileY=%d&ZoomLevel=15",
			TileParameters[m_nCountry][m_nProduct][m_nDetail].strServer,
			easting, northing);

	return strRequest;

}

// #define WMS_BING	"http://a1.ortho.tiles.virtualearth.net/tiles/a031311321031011.jpeg?g=52"
CString CCustomMap::bingGetRequest(int easting, int northing)
{
	CString strRequest;
	CString strQuadKey;

	int levelOfDetail = atoi(TileParameters[m_nCountry][m_nProduct][m_nDetail].strDetail);

	strQuadKey = TileXYToQuadKey(easting, northing, levelOfDetail);

	strRequest.Format(
			"tiles/a%s.jpeg?g=52",
			strQuadKey);

	return strRequest;
}

// #define WMS_SATMAP	"http://tileserver.satmap.com/AT/14/12023002320313.ipic"
CString CCustomMap::satmapGetRequest(int easting, int northing)
{
	CString strRequest;
	CString strQuadKey;

	int levelOfDetail = atoi(TileParameters[m_nCountry][m_nProduct][m_nDetail].strDetail);


	strQuadKey = TileXYToQuadKey(easting, northing, levelOfDetail);

	strRequest.Format(
			"%s/%s/%s.ipic",
			TileParameters[m_nCountry][m_nProduct][m_nDetail].strProduct,
			TileParameters[m_nCountry][m_nProduct][m_nDetail].strDetail,
			strQuadKey);

	return strRequest;
}

void CCustomMap::WriteProfile(CString strGridRef)
{
//	CString str;
	
//	str.Format("%s,%d,%d", strGridRef, m_nProduct, m_nDetail);
	AfxGetApp()->WriteProfileString("Main", GetRegistryValueName(), /*str*/strGridRef);
}

void CCustomMap::ReadProfile(CString& strGridRef)
{
	CString str;

//CString strReg = GetRegistryValueName();
	str = AfxGetApp()->GetProfileString("Main", GetRegistryValueName(), "");
//	sscanf(str,"%s,%d,%d", strGridRef, m_nProduct, m_nDetail);

	strGridRef = str;
}

CString CCustomMap::GetDatabasePath()
{
	CString str;
	str.Format("%s\\%s\\%s\\%s", m_strDatabaseRoot, GetInternetCode(), GetFlavourCode(), GetScaleCode());
	return str;
}

int CCustomMap::GetCountry()
{ 
	return m_nCountry; 
}

int CCustomMap::GetProduct()
{ 
	return m_nProduct;
}

int CCustomMap::GetDetail()
{
	return m_nDetail;
}






int CCustomMap::GetDatum()
{ 
	return CountryData[m_nCountry].nDatum;
}

int CCustomMap::GetPixelsPerTile()
{
	return TileParameters[m_nCountry][m_nProduct][m_nDetail].nPixels;
}

int CCustomMap::GetMetresPerTile()
{
	return TileParameters[m_nCountry][m_nProduct][m_nDetail].nMeters;
}

int CCustomMap::GetMapScale()
{
	return MapScales[m_nDetail].nDetail;
}

int CCustomMap::GetTileBitDepth()
{
	return TileParameters[m_nCountry][m_nProduct][m_nDetail].nBitDepth;
}

int CCustomMap::GetTileGfxType()
{
	return TileParameters[m_nCountry][m_nProduct][m_nDetail].nGfxType;
}






int CCustomMap::GetCoordinateSystem()
{
	return m_nCoordSystem;
}

int CCustomMap::GetLevelOfDetail()
{
	return atoi(TileParameters[m_nCountry][m_nProduct][m_nDetail].strDetail);
}