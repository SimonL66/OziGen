#include "CustomMap.h"

#include <direct.h>

#include "func.h"
#include "ServerRequests.h"

#define _OziGenPro	1

// 1. Data.
// 2. Parsing Functions.
// 3. Array Initilastion.
// 4. Server Requests
// 5. Country Functions.
// 6. Flavour Functions.
// 7. Scale Functions.

// Database

struct CountryData {

	int		nCountry;
	int		nDatum;

	CString strCode;
//	char*	strRegistryValue;
	CString strName;


	CString	strExampleGrid;
	CString strExampleLatLon;

	CString strCoordSystem;

} CountryData[] = {

	{COUNTRY_BE, MAP_DATUM_BE, "be", "Belgium",			"650000, 671000",	"50.8411° N, 4.3564° E",	"Lambert08"},
	{COUNTRY_UK, MAP_DATUM_UK, "uk", "Britain",			"TQ 315 815",		"51.5171° N, 0.1062° W",	"OSGB"},
	{COUNTRY_CZ, MAP_DATUM_CZ, "cz", "Czech Republic",	"459000 5549000",	"50.0833° N, 14.4167° E",	"UTM33"},
	{COUNTRY_DE, MAP_DATUM_DE, "de", "Germany",			"3798000 5831000",	"50.9500° N, 6.9667° E",	"Gauss Krueger"},
	{COUNTRY_FI, MAP_DATUM_FI, "fi", "Finland",			"",					"60.1708° N, 24.9375° E",	""},
	{COUNTRY_SI, MAP_DATUM_SI, "si", "Slovenia",		"",					"46.0556° N, 14.5083° E",	""},
	{COUNTRY_PT, MAP_DATUM_PT, "pt", "Portugal",		"",					"38.7138° N, 9.1394° W",	""},
	{COUNTRY_ES, MAP_DATUM_ES, "es", "Spain",			"443000 4473000",	"40.4000° N, 3.6833° W",	"ED50"},


//	{COUNTRY_CAT, MAP_DATUM_CAT, "cat", "Catalonia",		"",					"41.3833° N, 2.1833° E",	""},

	{COUNTRY_OSM, MAP_DATUM_OSM, "osm", "Open Street Map",	"",					"54.2439° N, 0.7760° W",	""},

	{COUNTRY_DK, MAP_DATUM_DK,	"dk", "Denmark",			"",					"54.2439° N, 0.7760° W",	""},

	{COUNTRY_FR,  MAP_DATUM_FR,		"fr",	 "France",			"0652000, 6864000",	"48.8742° N, 2.3470° E",	"Lambert93"},

	{COUNTRY_LU,  MAP_DATUM_LU,		"lu",	 "Luxembourg",		"",					"",							""},

	{COUNTRY_CH,  MAP_DATUM_CH,		"ch",	 "Switzerland",		"",					"",							"CH1903"},

	{COUNTRY_SE,  MAP_DATUM_SE,		"se",	 "Sweden",		"",					"",							"Sweref"},

	{COUNTRY_HR,  MAP_DATUM_HR,		"hr",	 "Croatia",		"",					"",							"HTRS96"},

	{COUNTRY_IT,  MAP_DATUM_IT,		"it",	 "Italy",		"",					"",							"UTM33"},


	{COUNTRY_ARC, MAP_DATUM_ARC, "arc", "ArcGIS",			"",					"54.2439° N, 0.7760° W",	""},

#if _OziGenPro
	{COUNTRY_SAT, MAP_DATUM_SAT, "sat", "SatMap",			"",					"54.2439° N, 0.7760° W",	""},
#endif

//	{COUNTRY_PL,  MAP_DATUM_PL,		"pl",	 "Poland",			"",					"",							""},

//	{COUNTRY_AT,  MAP_DATUM_AT,		"at",	 "Austria",			"",					"",							""},
//	{COUNTRY_IE,  MAP_DATUM_IE,		"ie",	 "Eire",			"",					"",							"OSi"},
//	{COUNTRY_IT,  MAP_DATUM_IT,		"it",	 "Italy",			"",					"",							""},
//	{COUNTRY_NL,  MAP_DATUM_NL,		"nl",	 "Netherlands",		"",					"",							""},
//	{COUNTRY_NI,  MAP_DATUM_NI,		"ni",	 "Northern Ireland","",					"",							"OSNi"},


	{-1, -1, "", "", "", "", ""}
};

struct ProductData {

	int		nCountry;
	int		nProduct;

	CString strCode;
	CString strName;

} ProductData[] = {

	{COUNTRY_BE, PRODUCT_01, "wms", "WMS"},
	{COUNTRY_BE, PRODUCT_02, "ngi", "NGI"},

	{COUNTRY_CZ, PRODUCT_01, "cuzk", "CUZK"},

	{COUNTRY_UK, PRODUCT_01, "ap", "Aerial"},
	{COUNTRY_UK, PRODUCT_02, "hi", "Highland"},
	{COUNTRY_UK, PRODUCT_03, "pro", "OSPro"},
	{COUNTRY_UK, PRODUCT_04, "vm", "Vector"},
	{COUNTRY_UK, PRODUCT_05, "sv", "Street View"},

	{COUNTRY_UK, PRODUCT_06, "npe", "New Popular Edition"},
	{COUNTRY_UK, PRODUCT_07, "os1", "1st Edition"},
	{COUNTRY_UK, PRODUCT_08, "os7", "7th Edition"},

	{COUNTRY_UK, PRODUCT_09, "old", "Old 25K"},
	{COUNTRY_UK, PRODUCT_10, "his", "Historical"},
//	{COUNTRY_UK, PRODUCT_11, "1930", "OS 1930"},

	{COUNTRY_DE, PRODUCT_01, "nrw", "NRW"},		// North Rhine-Westphalia.
	{COUNTRY_DE, PRODUCT_02, "bkg", "BKG"},		// Bundesamt für Kartographie und Geodäsie.
	{COUNTRY_DE, PRODUCT_03, "mv",  "MV"},		// Mecklenburg-Vorpommern.
	{COUNTRY_DE, PRODUCT_04, "km", "Kompass"},	// Kompass.

	{COUNTRY_ES, PRODUCT_01, "ngi", "NGI(E)"},
	{COUNTRY_ES, PRODUCT_02, "mtc", "Catalonia (MTC)"},

//	{COUNTRY_CAT, PRODUCT_01, "mtc", "MTC"},

	{COUNTRY_FI, PRODUCT_01, "fi", "fi"},

	{COUNTRY_SI, PRODUCT_01, "dtk", "DTK"},

	{COUNTRY_PT, PRODUCT_01, "sc", "SC"},

	{COUNTRY_OSM, PRODUCT_01, "vm", "OSM"},
	{COUNTRY_OSM, PRODUCT_02, "cy", "Cycle"},
	{COUNTRY_OSM, PRODUCT_03, "tr", "Transport"},
	{COUNTRY_OSM, PRODUCT_04, "ln", "Landscape"},
	{COUNTRY_OSM, PRODUCT_05, "od", "Outdoor"},

	{COUNTRY_OSM, PRODUCT_06, "km", "Kompass"},
	{COUNTRY_OSM, PRODUCT_07, "bn", "Bing"},
	{COUNTRY_OSM, PRODUCT_08, "vr", "ViewRanger"},

//#if _OziGenPro
	{COUNTRY_SAT, PRODUCT_01, "at", "Austria"},
	{COUNTRY_SAT, PRODUCT_02, "be", "Belgium"},
	{COUNTRY_SAT, PRODUCT_03, "de", "Germany"},
	{COUNTRY_SAT, PRODUCT_04, "it", "Italy"},
	{COUNTRY_SAT, PRODUCT_05, "nl", "Netherlands"},
	{COUNTRY_SAT, PRODUCT_06, "ni", "Northern Ireland"},
	{COUNTRY_SAT, PRODUCT_07, "ie", "Eire"},
	{COUNTRY_SAT, PRODUCT_08, "es", "Spain"},
	{COUNTRY_SAT, PRODUCT_09, "ch", "Switzerland"},
	{COUNTRY_SAT, PRODUCT_10, "fr", "France"},
	{COUNTRY_SAT, PRODUCT_11, "rsa", "South Africa"},
	{COUNTRY_SAT, PRODUCT_12, "bev", "AT/DE (BEV)"},
//#endif

	{COUNTRY_DK, PRODUCT_01, "kr", "Krak"},
	{COUNTRY_DK, PRODUCT_02, "ko", "Kort"},

	{COUNTRY_FR, PRODUCT_01, "ign", "IGN"},
//	{COUNTRY_FR, PRODUCT_02, "aq", "Anquet"},

	{COUNTRY_LU, PRODUCT_01, "ign", "IGN"},

	{COUNTRY_CH, PRODUCT_01, "1903", "CH 1903"},

	{COUNTRY_SE, PRODUCT_01, "topo", "Topographic"},

	{COUNTRY_HR, PRODUCT_01, "tk25", "TK25"},

	{COUNTRY_IT, PRODUCT_01, "25", "IGM25"},

	{COUNTRY_ARC, PRODUCT_01, "topo", "Topographic"},

	{-1, -1, "", ""}
};

struct LayerData {

	int		nCountry;
	int		nProduct;
	int		nLayer;

	int		nPixels;
	int		nMetres;

	int		nGfxType;
	int		nBitDepth;

	CString	strWMSRequest;

	int nProjection;

	int nServerType;

	CString strCode;
	CString strName;

	int nZoom;

	CString strReferer;

} LayerData[] = {

	{COUNTRY_BE, PRODUCT_01, LAYER_01, 250,  250, PNG, 24, WMS_NGI_BE_10,  -1, WMS,  "10",  "10,000",  10, REFER_NULL},
	{COUNTRY_BE, PRODUCT_01, LAYER_02, 400, 1000, PNG, 24, WMS_NGI_BE_50,  -1, WMS,  "50",  "50,000",  50, REFER_NULL},
	{COUNTRY_BE, PRODUCT_01, LAYER_03, 400, 2000, PNG, 24, WMS_NGI_BE_100, -1, WMS, "100", "100,000", 100, REFER_NULL},

	{COUNTRY_BE, PRODUCT_02, LAYER_01, 256,  677.3346880027092, PNG, 24, WMTS_NGI, 3812, NGI, "08", "08", 8, REFER_NGI},
	{COUNTRY_BE, PRODUCT_02, LAYER_02, 256,  22222, PNG, 24, WMTS_NGI, 3812, NGI, "05", "05", 5, REFER_NGI},

	{COUNTRY_CZ, PRODUCT_01, LAYER_01, 400,  1000, PNG, 8, WMS_CUZK_50,   -1, WMS,   "50",   "50,000",   50, REFER_NULL},
	{COUNTRY_CZ, PRODUCT_01, LAYER_02, 400, 10000, PNG, 8, WMS_CUZK_500,  -1, WMS,  "500",  "500,000",  500, REFER_NULL},
	{COUNTRY_CZ, PRODUCT_01, LAYER_03, 400, 50000, PNG, 8, WMS_CUZK_2500, -1, WMS, "2500", "2500,000", 2500, REFER_NULL},

	{COUNTRY_DE, PRODUCT_01, LAYER_01, 250,  250, PNG, 32, WMS_NRW_DTK10, -1, WMS, "10", "10,000", 10, REFER_NULL},
	{COUNTRY_DE, PRODUCT_01, LAYER_02, 200,  500, PNG, 32, WMS_NRW_DTK25, -1, WMS, "25", "25,000", 25, REFER_NULL},
	{COUNTRY_DE, PRODUCT_01, LAYER_03, 400, 1000, PNG, 32, WMS_NRW_DLM50, -1, WMS, "50", "50,000", 50, REFER_NULL},
	
	{COUNTRY_DE, PRODUCT_02, LAYER_01, 500,  4000, PNG, 24, WMS_BKG_200K,  -1, WMS,  "200",  "200,000",  200, REFER_NULL},
	{COUNTRY_DE, PRODUCT_02, LAYER_02, 500, 10000, PNG, 24, WMS_BKG_500K,  -1, WMS,  "500",  "500,000",  500, REFER_NULL},
	{COUNTRY_DE, PRODUCT_02, LAYER_03, 500, 20000, PNG, 24, WMS_BKG_1000K, -1, WMS, "1000", "1000,000", 1000, REFER_NULL},
	{COUNTRY_DE, PRODUCT_02, LAYER_04, 500, 50000, PNG, 24, WMS_BKG_2500K, -1, WMS, "2500", "2500,000", 2500, REFER_NULL},

	{COUNTRY_DE, PRODUCT_03, LAYER_01, 250,   250, PNG, 8, WMS_MV_DTK10,   -1, WMS,   "10",   "10,000",   10, REFER_NULL},
	{COUNTRY_DE, PRODUCT_03, LAYER_02, 200,   500, PNG, 8, WMS_MV_DTK25,   -1, WMS,   "25",   "25,000",   25, REFER_NULL},
	{COUNTRY_DE, PRODUCT_03, LAYER_03, 400,  1000, PNG, 8, WMS_MV_DTK50,   -1, WMS,   "50",   "50,000",   50, REFER_NULL},
	{COUNTRY_DE, PRODUCT_03, LAYER_04, 250,  2000, PNG, 8, WMS_MV_DTK100,  -1, WMS,  "100",  "100,000",  100, REFER_NULL},
	{COUNTRY_DE, PRODUCT_03, LAYER_05, 250,  5000, PNG, 8, WMS_MV_DTK250,  -1, WMS,  "250",  "250,000",  250, REFER_NULL},
	{COUNTRY_DE, PRODUCT_03, LAYER_06, 250,  7500, PNG, 8, WMS_MV_DTK750,  -1, WMS,  "750",  "750,000",  750, REFER_NULL},
	{COUNTRY_DE, PRODUCT_03, LAYER_07, 250, 10000, PNG, 8, WMS_MV_DTK1000, -1, WMS, "1000", "1000,000", 1000, REFER_NULL},

	{COUNTRY_DE, PRODUCT_04, LAYER_01, 256, 1, JPEG, 32, WMS_Kompass, 3857, Kompass, "15",    "15,000", 15, REFER_NULL},
	{COUNTRY_DE, PRODUCT_04, LAYER_02, 256, 1, JPEG, 32, WMS_Kompass, 3857, Kompass, "14",    "35,000", 14, REFER_NULL},
	{COUNTRY_DE, PRODUCT_04, LAYER_03, 256, 1, JPEG, 32, WMS_Kompass, 3857, Kompass, "13",    "70,000", 13, REFER_NULL},
	{COUNTRY_DE, PRODUCT_04, LAYER_04, 256, 1, JPEG, 32, WMS_Kompass, 3857, Kompass, "12",   "150,000", 12, REFER_NULL},
	{COUNTRY_DE, PRODUCT_04, LAYER_05, 256, 1, JPEG, 32, WMS_Kompass, 3857, Kompass, "11",   "250,000", 11, REFER_NULL},
	{COUNTRY_DE, PRODUCT_04, LAYER_06, 256, 1, JPEG, 32, WMS_Kompass, 3857, Kompass, "10",   "500,000", 10, REFER_NULL},
	{COUNTRY_DE, PRODUCT_04, LAYER_07, 256, 1, JPEG, 32, WMS_Kompass, 3857, Kompass, "09", "1,000,000",  9, REFER_NULL},

	{COUNTRY_ES, PRODUCT_01, LAYER_01, 400, 1000,   PNG, 24, WMS_IGN_ES, -1, WMS,   "50",   "50,000",   50, REFER_NULL},
	{COUNTRY_ES, PRODUCT_01, LAYER_02, 400, 4000,   PNG, 24, WMS_IGN_ES, -1, WMS,  "200",  "200,000",  200, REFER_NULL},
	{COUNTRY_ES, PRODUCT_01, LAYER_03, 400, 10000,  PNG, 24, WMS_IGN_ES, -1, WMS,  "500",  "500,000",  500, REFER_NULL},
	{COUNTRY_ES, PRODUCT_01, LAYER_04, 400, 20000,  PNG, 24, WMS_IGN_ES, -1, WMS, "1000", "1000,000", 1000, REFER_NULL},
	{COUNTRY_ES, PRODUCT_01, LAYER_05, 400, 100000, PNG, 24, WMS_IGN_ES, -1, WMS, "5000", "5000,000", 5000, REFER_NULL},

	{COUNTRY_ES, PRODUCT_02, LAYER_01, 250,   250, PNG, 24, WMS_MTC_10,   -1, WMS,   "10",   "10,000",   10, REFER_NULL},
	{COUNTRY_ES, PRODUCT_02, LAYER_01, 250,   250, PNG, 24, WMS_MTC_10,   -1, WMS,   "10",   "10,000",   10, REFER_NULL},
	{COUNTRY_ES, PRODUCT_02, LAYER_02, 200,   500, PNG, 24, WMS_MTC_25,   -1, WMS,   "25",   "25,000",   25, REFER_NULL},
	{COUNTRY_ES, PRODUCT_02, LAYER_03, 400,  1000, PNG, 24, WMS_MTC_50,   -1, WMS,   "50",   "50,000",   50, REFER_NULL},
	{COUNTRY_ES, PRODUCT_02, LAYER_04, 400,  5000, PNG, 24, WMS_MTC_250,  -1, WMS,  "250",  "250,000",  250, REFER_NULL},
	{COUNTRY_ES, PRODUCT_02, LAYER_05, 400, 10000, PNG, 24, WMS_MTC_500,  -1, WMS,  "500",  "500,000",  500, REFER_NULL},
	{COUNTRY_ES, PRODUCT_02, LAYER_06, 400, 20000, PNG, 24, WMS_MTC_1000, -1, WMS, "1000", "1000,000", 1000, REFER_NULL},	
/*
	{COUNTRY_CAT, PRODUCT_01, LAYER_01, 250,   250, PNG, 24, WMS_MTC_10,   -1, WMS,   "10",   "10K",   10, REFER_NULL},
	{COUNTRY_CAT, PRODUCT_01, LAYER_02, 200,   500, PNG, 24, WMS_MTC_25,   -1, WMS,   "25",   "25K",   25, REFER_NULL},
	{COUNTRY_CAT, PRODUCT_01, LAYER_03, 400,  1000, PNG, 24, WMS_MTC_50,   -1, WMS,   "50",   "50K",   50, REFER_NULL},
	{COUNTRY_CAT, PRODUCT_01, LAYER_04, 400,  5000, PNG, 24, WMS_MTC_250,  -1, WMS,  "250",  "250K",  250, REFER_NULL},
	{COUNTRY_CAT, PRODUCT_01, LAYER_05, 400, 10000, PNG, 24, WMS_MTC_500,  -1, WMS,  "500",  "500K",  500, REFER_NULL},
	{COUNTRY_CAT, PRODUCT_01, LAYER_06, 400, 20000, PNG, 24, WMS_MTC_1000, -1, WMS, "1000", "1000K", 1000, REFER_NULL},	
*/

	{COUNTRY_FI, PRODUCT_01, LAYER_01, 400, 1000, PNG, 32, WMS_FI_50, 3067, WMS, "50", "50,000", 50, REFER_NULL},

	{COUNTRY_SI, PRODUCT_01, LAYER_01, 200,  1000, PNG, 24, WMS_DTK_50,   -1, WMS,   "50",   "50,000",   50, REFER_NULL},
	{COUNTRY_SI, PRODUCT_01, LAYER_02, 200,  5000, PNG, 24, WMS_DPK_250,  -1, WMS,  "250",  "250,000",  250, REFER_NULL},
	{COUNTRY_SI, PRODUCT_01, LAYER_03, 200, 10000, PNG, 24, WMS_DPK_500,  -1, WMS,  "500",  "500,000",  500, REFER_NULL},
	{COUNTRY_SI, PRODUCT_01, LAYER_04, 200, 20000, PNG, 24, WMS_DPK_1000, -1, WMS, "1000", "1000,000", 1000, REFER_NULL},

	{COUNTRY_PT, PRODUCT_01, LAYER_01, 500,  2000, PNG, 24, WMS_SC_100, -1, WMS, "100", "100,000", 100, REFER_NULL},
	{COUNTRY_PT, PRODUCT_01, LAYER_02, 500, 10000, PNG, 24, WMS_SC_500, -1, WMS, "500", "500,000", 500, REFER_NULL},

	{COUNTRY_UK, PRODUCT_01, LAYER_01, 250,  125, JPEG, 32, WMS_GETMAPPING_125,  27700, GetMapping,  "125",  "125",  125, REFER_NULL},
	{COUNTRY_UK, PRODUCT_01, LAYER_02, 250,  250, JPEG, 32, WMS_GETMAPPING_250,  27700, GetMapping,  "250",  "250",  250, REFER_NULL},
	{COUNTRY_UK, PRODUCT_01, LAYER_03, 250,  500, JPEG, 32, WMS_GETMAPPING_500,  27700, GetMapping,  "500",  "500",  500, REFER_NULL},
	{COUNTRY_UK, PRODUCT_01, LAYER_04, 250, 1000, JPEG, 32, WMS_GETMAPPING_1000, 27700, GetMapping, "1000", "1000", 1000, REFER_NULL},
	{COUNTRY_UK, PRODUCT_01, LAYER_05, 250, 2000, JPEG, 32, WMS_GETMAPPING_2000, 27700, GetMapping, "2000", "2000", 2000, REFER_NULL},

	{COUNTRY_UK, PRODUCT_02, LAYER_01, 200,  500, PNG, 24, WMS_HIGHLAND_25, -1, WMS, "25", "25,000", 25, REFER_HIGHLAND},
	{COUNTRY_UK, PRODUCT_02, LAYER_02, 200, 1000, PNG, 24, WMS_HIGHLAND_50, -1, WMS, "50", "50,000", 50, REFER_HIGHLAND},

	{COUNTRY_UK, PRODUCT_03, LAYER_01, 250,   250, PNG, 24, WMS_OS_10K,  -1, WMS,   "10",   "10,000",   10, REFER_NULL},
	{COUNTRY_UK, PRODUCT_03, LAYER_02, 200,   500, PNG,  8, WMS_OS_25K,  -1, WMS,   "25",   "25,000",   25, REFER_NULL},
	{COUNTRY_UK, PRODUCT_03, LAYER_03, 200,  1000, PNG,  8, WMS_OS_50K,  -1, WMS,   "50",   "50,000",   50, REFER_NULL},
	{COUNTRY_UK, PRODUCT_03, LAYER_04, 200,  5000, PNG,  8, WMS_OS_250K, -1, WMS,  "250",  "250,000",  250, REFER_NULL},
	{COUNTRY_UK, PRODUCT_03, LAYER_05, 200, 20000, PNG,  8, WMS_OS_MS,   -1, WMS, "1000", "1000,000", 1000, REFER_NULL},

	{COUNTRY_UK, PRODUCT_04, LAYER_01, 250, 250, PNG, 32, WMS_OS_VML, -1, WMS, "10", "10,000", 10, REFER_NULL},
	{COUNTRY_UK, PRODUCT_04, LAYER_02, 200, 500, PNG,  8, WMS_OS_VMD, -1, WMS, "25", "25,000", 25, REFER_NULL},

	{COUNTRY_UK, PRODUCT_05, LAYER_01, 250, 250, PNG, 8, WMS_OS_SV, -1, WMS, "10", "10,000", 10, REFER_NULL},

//#define WMS_OS_NPE		"http://a.ooc.openstreetmap.org/npe/13/4067/2625.png
//#define WMS_OS_1st		"http://a.ooc.openstreetmap.org/os1/13/4066/2624.jpg"
//#define WMS_OS_7th		"http://a.ooc.openstreetmap.org/os7/13/4069/2624.jpg"
//#define WMS_OS_OLD_25K	"http://geo.nls.uk/mapdata2/os/25000/15/16259/21897.png"
//#define WMS_OS_HISTORICAL "http://t4.uk.tileserver.com/_os1/r0/14/8130/5434.jpg"
//#define WMS_OS_1930		"http://tiles.snaffle.me.uk/03/tqs/rsr/srr/ttt/qrq.jpg"
	{COUNTRY_UK, PRODUCT_06, LAYER_01, 256, 1, PNG, 8, WMTS_OS_NPE, 3857, OSM, "13", "13", 13, REFER_NULL},

	{COUNTRY_UK, PRODUCT_07, LAYER_01, 256, 1, JPG, 8, WMTS_OS_1st, 3857, OSM, "13", "13", 13, REFER_NULL},
	{COUNTRY_UK, PRODUCT_08, LAYER_01, 256, 1, JPG, 8, WMTS_OS_7th, 3857, OSM, "13", "13", 13, REFER_NULL},

	{COUNTRY_UK, PRODUCT_09, LAYER_01, 256, 1, PNG, 8, WMTS_OS_OLD_25K, 3857, OSM, "15", "15", 15, REFER_NULL},
	{COUNTRY_UK, PRODUCT_10, LAYER_01, 256, 1, JPG, 24, WMTS_OS_HISTORICAL, 3857, OSM, "14", "14", 14, REFER_NULL},


	// Open Street Map

	{COUNTRY_OSM, PRODUCT_01, LAYER_01, 256, 1, PNG,  8, WMS_OSM, 3857, OSM, "18", "18", 18, REFER_NULL},
	{COUNTRY_OSM, PRODUCT_01, LAYER_02, 256, 1, PNG,  8, WMS_OSM, 3857, OSM, "15", "15", 15, REFER_NULL},
	{COUNTRY_OSM, PRODUCT_01, LAYER_03, 256, 1, PNG,  8, WMS_OSM, 3857, OSM, "12", "12", 12, REFER_NULL},

	{COUNTRY_OSM, PRODUCT_02, LAYER_01, 256, 1, PNG,  8, WMS_OSM_CYCLE, 3857, OSM, "18", "18", 18, REFER_NULL},
	{COUNTRY_OSM, PRODUCT_02, LAYER_02, 256, 1, PNG,  8, WMS_OSM_CYCLE, 3857, OSM, "15", "15", 15, REFER_NULL},
	{COUNTRY_OSM, PRODUCT_02, LAYER_03, 256, 1, PNG,  8, WMS_OSM_CYCLE, 3857, OSM, "12", "12", 12, REFER_NULL},

	{COUNTRY_OSM, PRODUCT_03, LAYER_01, 256, 1, PNG,  8, WMS_OSM_TRANSPORT, 3857, OSM, "18", "18", 18, REFER_NULL},
	{COUNTRY_OSM, PRODUCT_03, LAYER_02, 256, 1, PNG,  8, WMS_OSM_TRANSPORT, 3857, OSM, "15", "15", 15, REFER_NULL},
	{COUNTRY_OSM, PRODUCT_03, LAYER_03, 256, 1, PNG,  8, WMS_OSM_TRANSPORT, 3857, OSM, "12", "12", 12, REFER_NULL},

	{COUNTRY_OSM, PRODUCT_04, LAYER_01, 256, 1, PNG,  8, WMS_OSM_LANDSCAPE, 3857, OSM, "18", "18", 18, REFER_NULL},
	{COUNTRY_OSM, PRODUCT_04, LAYER_02, 256, 1, PNG,  8, WMS_OSM_LANDSCAPE, 3857, OSM, "15", "15", 15, REFER_NULL},
	{COUNTRY_OSM, PRODUCT_04, LAYER_03, 256, 1, PNG,  8, WMS_OSM_LANDSCAPE, 3857, OSM, "12", "12", 12, REFER_NULL},

	{COUNTRY_OSM, PRODUCT_05, LAYER_01, 256, 1, PNG,  8, WMS_OSM_OUTDOOR, 3857, OSM, "18", "18", 18, REFER_NULL},
	{COUNTRY_OSM, PRODUCT_05, LAYER_02, 256, 1, PNG,  8, WMS_OSM_OUTDOOR, 3857, OSM, "15", "15", 15, REFER_NULL},
	{COUNTRY_OSM, PRODUCT_05, LAYER_03, 256, 1, PNG,  8, WMS_OSM_OUTDOOR, 3857, OSM, "12", "12", 12, REFER_NULL},

	{COUNTRY_OSM, PRODUCT_06, LAYER_01, 256, 1, JPEG, 32, WMS_Kompass, 3857, Kompass, "15",    "15,000", 15, REFER_NULL},
	{COUNTRY_OSM, PRODUCT_06, LAYER_02, 256, 1, JPEG, 32, WMS_Kompass, 3857, Kompass, "14",    "35,000", 14, REFER_NULL},
	{COUNTRY_OSM, PRODUCT_06, LAYER_03, 256, 1, JPEG, 32, WMS_Kompass, 3857, Kompass, "13",    "70,000", 13, REFER_NULL},
	{COUNTRY_OSM, PRODUCT_06, LAYER_04, 256, 1, JPEG, 32, WMS_Kompass, 3857, Kompass, "12",   "150,000", 12, REFER_NULL},
	{COUNTRY_OSM, PRODUCT_06, LAYER_05, 256, 1, JPEG, 32, WMS_Kompass, 3857, Kompass, "11",   "250,000", 11, REFER_NULL},
	{COUNTRY_OSM, PRODUCT_06, LAYER_06, 256, 1, JPEG, 32, WMS_Kompass, 3857, Kompass, "10",   "500,000", 10, REFER_NULL},
	{COUNTRY_OSM, PRODUCT_06, LAYER_07, 256, 1, JPEG, 32, WMS_Kompass, 3857, Kompass, "09", "1,000,000",  9, REFER_NULL},

	{COUNTRY_OSM, PRODUCT_07, LAYER_01, 256, 1, JPEG, 32, WMS_BING, 900913, Bing, "15", "15", 15, REFER_NULL},

	{COUNTRY_OSM, PRODUCT_08, LAYER_01, 256, 1, JPEG, 32, WMS_OSM, 3857, OSM, "10", "10", 10, REFER_NULL},
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
#if _OziGenPro
	{COUNTRY_SAT, PRODUCT_01, LAYER_01, 256, 1, JPEG, 32, SATMAP_AT, 900913, SatMap, "13",   "13", 13, REFER_NULL},
	{COUNTRY_SAT, PRODUCT_01, LAYER_02, 256, 1, JPEG, 32, SATMAP_AT, 900913, SatMap, "14",   "14", 14, REFER_NULL},
	{COUNTRY_SAT, PRODUCT_01, LAYER_03, 256, 1, JPEG, 32, SATMAP_AT, 900913, SatMap, "15",   "15", 15, REFER_NULL}, // Should work?

	{COUNTRY_SAT, PRODUCT_02, LAYER_01, 256, 1, PNG,  8, SATMAP_BE, 900913, SatMap, "14",   "14", 14, REFER_NULL},
	{COUNTRY_SAT, PRODUCT_02, LAYER_02, 256, 1, PNG,  8, SATMAP_BE, 900913, SatMap, "15",   "15", 15, REFER_NULL},

	{COUNTRY_SAT, PRODUCT_03, LAYER_01, 256, 1, JPEG, 32, SATMAP_DE, 900913, SatMap, "13",   "13", 13, REFER_NULL},
	{COUNTRY_SAT, PRODUCT_03, LAYER_02, 256, 1, JPEG, 32, SATMAP_DE, 900913, SatMap, "14",   "14", 14, REFER_NULL},

	{COUNTRY_SAT, PRODUCT_04, LAYER_01, 256, 1, PNG,  8, SATMAP_IT, 900913, SatMap, "13",   "13", 13, REFER_NULL},
	{COUNTRY_SAT, PRODUCT_04, LAYER_02, 256, 1, PNG,  8, SATMAP_IT, 900913, SatMap, "14",   "14", 14, REFER_NULL},
	{COUNTRY_SAT, PRODUCT_04, LAYER_03, 256, 1, PNG,  8, SATMAP_IT, 900913, SatMap, "15",   "15", 15, REFER_NULL},

	{COUNTRY_SAT, PRODUCT_05, LAYER_01, 256, 1, PNG,  8, SATMAP_NL, 900913, SatMap, "13",   "13", 13, REFER_NULL},
	{COUNTRY_SAT, PRODUCT_05, LAYER_02, 256, 1, PNG,  8, SATMAP_NL, 900913, SatMap, "14",   "14", 14, REFER_NULL},

	{COUNTRY_SAT, PRODUCT_06, LAYER_01, 256, 1, PNG, 32, SATMAP_NI, 900913, SatMap, "13",   "13", 13, REFER_NULL},
	{COUNTRY_SAT, PRODUCT_06, LAYER_02, 256, 1, PNG, 32, SATMAP_NI, 900913, SatMap, "14",   "14", 14, REFER_NULL},
	{COUNTRY_SAT, PRODUCT_06, LAYER_03, 256, 1, PNG, 32, SATMAP_NI, 900913, SatMap, "15",   "15", 15, REFER_NULL},

	{COUNTRY_SAT, PRODUCT_07, LAYER_01, 256, 1, PNG,  8, SATMAP_IE, 900913, SatMap, "11",   "11", 11, REFER_NULL},
	{COUNTRY_SAT, PRODUCT_07, LAYER_02, 256, 1, PNG,  8, SATMAP_IE, 900913, SatMap, "12",   "12", 12, REFER_NULL},
	{COUNTRY_SAT, PRODUCT_07, LAYER_03, 256, 1, PNG,  8, SATMAP_IE, 900913, SatMap, "13",   "13", 13, REFER_NULL},
	{COUNTRY_SAT, PRODUCT_07, LAYER_04, 256, 1, PNG,  8, SATMAP_IE, 900913, SatMap, "14",   "14", 14, REFER_NULL},
	{COUNTRY_SAT, PRODUCT_07, LAYER_05, 256, 1, PNG,  8, SATMAP_IE, 900913, SatMap, "15",   "15", 15, REFER_NULL},

	{COUNTRY_SAT, PRODUCT_08, LAYER_01, 256, 1, PNG, 32, SATMAP_ES, 900913, SatMap, "10",   "10", 10, REFER_NULL},
	{COUNTRY_SAT, PRODUCT_08, LAYER_02, 256, 1, PNG, 32, SATMAP_ES, 900913, SatMap, "11",   "11", 11, REFER_NULL}, // Why does this crash?
	{COUNTRY_SAT, PRODUCT_08, LAYER_03, 256, 1, PNG, 32, SATMAP_ES, 900913, SatMap, "12",   "12", 12, REFER_NULL},

	{COUNTRY_SAT, PRODUCT_09, LAYER_01, 256, 1, JPEG, 32, SATMAP_CH, 900913, SatMap, "12",   "12", 12, REFER_NULL},
	{COUNTRY_SAT, PRODUCT_09, LAYER_02, 256, 1, JPEG, 32, SATMAP_CH, 900913, SatMap, "13",   "13", 13, REFER_NULL},
	{COUNTRY_SAT, PRODUCT_09, LAYER_03, 256, 1, JPEG, 32, SATMAP_CH, 900913, SatMap, "14",   "14", 14, REFER_NULL},
	{COUNTRY_SAT, PRODUCT_09, LAYER_04, 256, 1, JPEG, 32, SATMAP_CH, 900913, SatMap, "15",   "15", 15, REFER_NULL}, // Why does this download blanks?
	{COUNTRY_SAT, PRODUCT_09, LAYER_05, 256, 1, JPEG, 32, SATMAP_CH, 900913, SatMap, "16",   "16", 16, REFER_NULL},

	{COUNTRY_SAT, PRODUCT_10, LAYER_01, 256, 1, PNG, 32, SATMAP_FR, 900913, SatMap, "13", "100K", 13, REFER_NULL},

	{COUNTRY_SAT, PRODUCT_11, LAYER_01, 256, 1, PNG, 32, SATMAP_RSA, 900913, SatMap, "15", "50K", 15, REFER_NULL},

	{COUNTRY_SAT, PRODUCT_12, LAYER_01, 256, 1, PNG, 32, SATMAP_BEV, 900913, SatMap, "15", "15", 15, REFER_NULL},
#endif

	{COUNTRY_DK, PRODUCT_01, LAYER_01, 256,   250, PNG, 32, WMS_KRAK_10,   25832, WMS,   "10",   "10,000",   10, REFER_NULL},
	{COUNTRY_DK, PRODUCT_01, LAYER_02, 256,  2000, PNG, 32, WMS_KRAK_100,  25832, WMS,  "100",  "100,000",  100, REFER_NULL},
	{COUNTRY_DK, PRODUCT_01, LAYER_03, 256, 20000, PNG, 32, WMS_KRAK_1000, 25832, WMS, "1000", "1000,000", 1000, REFER_NULL},

	{COUNTRY_DK, PRODUCT_02, LAYER_01, 256, 1000, PNG, 32, WMS_KORT, 25832, WMS, "50", "50", 50, REFER_NULL},

	{COUNTRY_SE, PRODUCT_01, LAYER_01, 250, 500, JPEG, 32, WMS_SE, 3006, WMS, "50", "50", 50, REFER_NULL},

	{COUNTRY_HR, PRODUCT_01, LAYER_01, 200, 500, PNG, 24, WMS_HR, 3765, WMS, "25", "25", 25, REFER_NULL},

	{COUNTRY_IT, PRODUCT_01, LAYER_01, 400, 500, PNG, 24, WMS_IT, 3004, WMS, "25", "25", 25, REFER_NULL},

	{COUNTRY_FR, PRODUCT_01, LAYER_01, 256, 1, JPEG, 24, WMS_IGN, 3857, IGN, "18",     "2,000", 18, REFER_IGN},
	{COUNTRY_FR, PRODUCT_01, LAYER_02, 256, 1, JPEG, 24, WMS_IGN, 3857, IGN, "17",     "4,000", 17, REFER_IGN},
	{COUNTRY_FR, PRODUCT_01, LAYER_03, 256, 1, JPEG, 24, WMS_IGN, 3857, IGN, "16",     "8,000", 16, REFER_IGN},
	{COUNTRY_FR, PRODUCT_01, LAYER_04, 256, 1, JPEG, 24, WMS_IGN, 3857, IGN, "15",    "15,000", 15, REFER_IGN},
	{COUNTRY_FR, PRODUCT_01, LAYER_05, 256, 1, JPEG, 24, WMS_IGN, 3857, IGN, "14",    "35,000", 14, REFER_IGN},
	{COUNTRY_FR, PRODUCT_01, LAYER_06, 256, 1, JPEG, 24, WMS_IGN, 3857, IGN, "13",    "70,000", 13, REFER_IGN},
	{COUNTRY_FR, PRODUCT_01, LAYER_07, 256, 1, JPEG, 24, WMS_IGN, 3857, IGN, "12",   "150,000", 12, REFER_IGN},
	{COUNTRY_FR, PRODUCT_01, LAYER_08, 256, 1, JPEG, 24, WMS_IGN, 3857, IGN, "11",   "250,000", 11, REFER_IGN},
	{COUNTRY_FR, PRODUCT_01, LAYER_09, 256, 1, JPEG, 24, WMS_IGN, 3857, IGN, "10",   "500,000", 10, REFER_IGN},
	{COUNTRY_FR, PRODUCT_01, LAYER_10, 256, 1, JPEG, 24, WMS_IGN, 3857, IGN, "09", "1,000,000",  9, REFER_IGN},
	{COUNTRY_FR, PRODUCT_01, LAYER_11, 256, 1, JPEG, 24, WMS_IGN, 3857, IGN, "08", "2,000,000",  8, REFER_IGN},
	{COUNTRY_FR, PRODUCT_01, LAYER_12, 256, 1, JPEG, 24, WMS_IGN, 3857, IGN, "07", "4,000,000",  7, REFER_IGN},

	//	{COUNTRY_FR, PRODUCT_02, LAYER_01, 250, 250, PNG, 32, "", 2154, ANQUET, "25", "25", 25, REFER_NULL},

//geoadmin.resolutions = "500.0,250.0,150.0,100.0,50.0,20.0,10.0,5.0,2.0,1.0,0.5,0.25";
//geoadmin.fakeScales = "750,1500,3000,6000,15000,30000,60000,150000,300000,450000,700000,1500000";

	{COUNTRY_LU, PRODUCT_01, LAYER_01, 256,   128, PNG, 24, WMTS_LUX, 2169, LUX, "10",   "1,500", 10, REFER_LUX},
	{COUNTRY_LU, PRODUCT_01, LAYER_02, 256,   256, PNG, 24, WMTS_LUX, 2169, LUX, "09",   "3,000",  9, REFER_LUX},
	{COUNTRY_LU, PRODUCT_01, LAYER_03, 256,   512, PNG, 24, WMTS_LUX, 2169, LUX, "08",   "6,000",  8, REFER_LUX},
	{COUNTRY_LU, PRODUCT_01, LAYER_04, 256,  1280, PNG, 24, WMTS_LUX, 2169, LUX, "07",  "15,000",  7, REFER_LUX},
	{COUNTRY_LU, PRODUCT_01, LAYER_05, 256,  2560, PNG, 24, WMTS_LUX, 2169, LUX, "06",  "30,000",  6, REFER_LUX},
	{COUNTRY_LU, PRODUCT_01, LAYER_06, 256,  5120, PNG, 24, WMTS_LUX, 2169, LUX, "05",  "60,000",  5, REFER_LUX},
	{COUNTRY_LU, PRODUCT_01, LAYER_07, 256, 12800, PNG, 24, WMTS_LUX, 2169, LUX, "04", "150,000",  4, REFER_LUX},
	{COUNTRY_LU, PRODUCT_01, LAYER_08, 256, 25600, PNG, 24, WMTS_LUX, 2169, LUX, "03", "300,000",  3, REFER_LUX},
	{COUNTRY_LU, PRODUCT_01, LAYER_09, 256, 38400, PNG, 24, WMTS_LUX, 2169, LUX, "02", "450,000",  2, REFER_LUX},

	{COUNTRY_CH, PRODUCT_01, LAYER_01, 256,  128, JPEG, 24, WMTS_CH, 21781, CH1903, "26", "26", 26, REFER_CH1903},

	{COUNTRY_CH, PRODUCT_01, LAYER_02, 256,  256, JPEG, 24, WMTS_CH, 21781, CH1903, "25", "25", 25, REFER_CH1903},
	{COUNTRY_CH, PRODUCT_01, LAYER_03, 256,  512, JPEG, 24, WMTS_CH, 21781, CH1903, "23", "23", 23, REFER_CH1903},
	{COUNTRY_CH, PRODUCT_01, LAYER_04, 256, 1280, JPEG, 24, WMTS_CH, 21781, CH1903, "21", "21", 21, REFER_CH1903},
	{COUNTRY_CH, PRODUCT_01, LAYER_05, 256, 2560, JPEG, 24, WMTS_CH, 21781, CH1903, "19", "19", 19, REFER_CH1903},

	{COUNTRY_ARC, PRODUCT_01, LAYER_01, 256, 1, JPEG, 32, WMTS_ARCCIS, 3857, ArcGIS, "13",   "13", 13, REFER_NULL},
	{COUNTRY_ARC, PRODUCT_01, LAYER_02, 256, 1, JPEG, 32, WMTS_ARCCIS, 3857, ArcGIS, "14",   "14", 14, REFER_NULL},
	{COUNTRY_ARC, PRODUCT_01, LAYER_03, 256, 1, JPEG, 32, WMTS_ARCCIS, 3857, ArcGIS, "15",   "15", 15, REFER_NULL},
	{COUNTRY_ARC, PRODUCT_01, LAYER_04, 256, 1, JPEG, 32, WMTS_ARCCIS, 3857, ArcGIS, "16",   "16", 16, REFER_NULL},
	{COUNTRY_ARC, PRODUCT_01, LAYER_05, 256, 1, JPEG, 32, WMTS_ARCCIS, 3857, ArcGIS, "17",   "17", 17, REFER_NULL},
	{COUNTRY_ARC, PRODUCT_01, LAYER_06, 256, 1, JPEG, 32, WMTS_ARCCIS, 3857, ArcGIS, "18",   "18", 18, REFER_NULL},

	{-1, -1, -1, -1, -1, -1, -1, "", -1, -1, "", "", -1, ""},
};

struct CountryArray {

	int		nCountry;
	int		nDatum;

	CString	strCode;
	CString	strName;

	CString	strRegistryValue;

	int		nCoordSystem;
	CString strExampleLatLon;
	CString strExampleGrid;
	CString strCoordSystem;

	int		nNumProducts;

} CountryArray[NUM_COUNTRY];

struct ProductParameters {

	int		nCountry;
	int		nProduct;

	CString strCode;
	CString strName;

	int		nDefaultScale;

	int		nNumLayers;

} ProductArray[NUM_COUNTRY][NUM_PRODUCTS];

struct LayerParameters {

	int	nPixels;
	int	nMetres;

	int	nGfxType;
	int	nBitDepth;

	CString strWebsite;
	CString strServer;
	CString strAttribution;
	CString strMap;
	CString strWMSLayer;
	CString strProduct;
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
	int nZoom;

	CString strReferer;

	CString strCode;
	CString strName;

} LayerArray[NUM_COUNTRY][NUM_PRODUCTS][NUM_LAYERS];

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
	if (strImage == "jpg") return JPG;
	if (strImage == "bmp") return BMP;
	if (strImage == "gif") return GIF;
	if (strImage == "jpeg") return JPEG;

	return -1;
}

CString WMS_ExtractParameter(CString str, CString strParameter)
{
	CString strValue;
	int nPos;

//	str = str.MakeLower();
//	strParameter = strParameter.MakeLower();

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
CString SatMap_ExtractParamter(CString str, int nParamter)
{
	CString strValue;
	int nPos;

	nPos = str.Find("http://");
	if (nPos == -1) {
		strValue.Empty();
		return strValue;
	}

	str = str.Mid(nPos+7);

	for (int i=0; i < nParamter; i++) {

		nPos = str.Find("/");
		if (nPos == -1) {
			strValue.Empty();
			return strValue;
		}

		str = str.Mid(nPos+1);
	}

	nPos = str.Find("/");
	if (nPos == -1) {
		strValue.Empty();
		return strValue;
	}

	strValue = str.Left(nPos);

	return strValue;
}

// http://services.arcgisonline.com/ArcGIS/rest/services/World_Topo_Map/MapServer/tile/18/84212/130283
void CCustomMap::ParseServerRequestString_ArcGIS(int nCountry, int nProduct, int nLayer, int l)
{
	LayerArray[nCountry][nProduct][nLayer].strWebsite = WMS_ExtractWebsiteServer(LayerData[l].strWMSRequest);
	LayerArray[nCountry][nProduct][nLayer].nPort = WMS_ExtractPort(LayerArray[nCountry][nProduct][nLayer].strWebsite);
	LayerArray[nCountry][nProduct][nLayer].strAttribution = WMS_ExtractDomain(LayerArray[nCountry][nProduct][nLayer].strWebsite);
	LayerArray[nCountry][nProduct][nLayer].nGfxType = LayerData[l].nGfxType;
	LayerArray[nCountry][nProduct][nLayer].nProjection = LayerData[l].nProjection;
	
	LayerArray[nCountry][nProduct][nLayer].strProduct = SatMap_ExtractParamter(LayerData[l].strWMSRequest, 1);
	LayerArray[nCountry][nProduct][nLayer].strWMSLayer = SatMap_ExtractParamter(LayerData[l].strWMSRequest, 2);
}

void CCustomMap::ParseServerRequestString_SatMap(int nCountry, int nProduct, int nLayer, int l)
{
	LayerArray[nCountry][nProduct][nLayer].strWebsite = WMS_ExtractWebsiteServer(LayerData[l].strWMSRequest);
	LayerArray[nCountry][nProduct][nLayer].nPort = WMS_ExtractPort(LayerArray[nCountry][nProduct][nLayer].strWebsite);
	LayerArray[nCountry][nProduct][nLayer].strAttribution = WMS_ExtractDomain(LayerArray[nCountry][nProduct][nLayer].strWebsite);
	LayerArray[nCountry][nProduct][nLayer].nGfxType = LayerData[l].nGfxType;
	LayerArray[nCountry][nProduct][nLayer].nProjection = LayerData[l].nProjection;
	
	LayerArray[nCountry][nProduct][nLayer].strProduct = SatMap_ExtractParamter(LayerData[l].strWMSRequest, 1);
	LayerArray[nCountry][nProduct][nLayer].strWMSLayer = SatMap_ExtractParamter(LayerData[l].strWMSRequest, 2);
}

// #define WMS_BING	"http://a1.ortho.tiles.virtualearth.net/tiles/a031311321031011.jpeg?g=52"
void CCustomMap::ParseServerRequestString_Bing(int nCountry, int nProduct, int nLayer, int l)
{
	LayerArray[nCountry][nProduct][nLayer].strWebsite = WMS_ExtractWebsiteServer(LayerData[l].strWMSRequest);
	LayerArray[nCountry][nProduct][nLayer].strServer = WMS_ExtractLayer(LayerData[l].strWMSRequest);
	LayerArray[nCountry][nProduct][nLayer].nPort = WMS_ExtractPort(LayerArray[nCountry][nProduct][nLayer].strWebsite);
	LayerArray[nCountry][nProduct][nLayer].strAttribution = WMS_ExtractDomain(LayerArray[nCountry][nProduct][nLayer].strWebsite);
	LayerArray[nCountry][nProduct][nLayer].nGfxType = LayerData[l].nGfxType;
	LayerArray[nCountry][nProduct][nLayer].nProjection = LayerData[l].nProjection;
}

// #define WMS_Kompass		"http://ec2.cdn.ecmaps.de/WmsGateway.ashx.jpg?Experience=kompass&MapStyle=KOMPASS%20Touristik&TileX=17018&TileY=10975&ZoomLevel=15"

void CCustomMap::ParseServerRequestString_Kompass(int nCountry, int nProduct, int nLayer, int l)
{
	LayerArray[nCountry][nProduct][nLayer].strWebsite = WMS_ExtractWebsiteServer(LayerData[l].strWMSRequest);
	LayerArray[nCountry][nProduct][nLayer].strServer = WMS_ExtractLayer(LayerData[l].strWMSRequest);
	LayerArray[nCountry][nProduct][nLayer].nPort = WMS_ExtractPort(LayerArray[nCountry][nProduct][nLayer].strWebsite);
	LayerArray[nCountry][nProduct][nLayer].strAttribution = WMS_ExtractDomain(LayerArray[nCountry][nProduct][nLayer].strWebsite);
	LayerArray[nCountry][nProduct][nLayer].strWMSLayer = WMS_ExtractParameter(LayerData[l].strWMSRequest, "size=");
	LayerArray[nCountry][nProduct][nLayer].nGfxType = LayerData[l].nGfxType;
	LayerArray[nCountry][nProduct][nLayer].nProjection = LayerData[l].nProjection;
}

// http://tile.openstreetmap.org/18/130584/83719.png
void CCustomMap::ParseServerRequestString_OSM(int nCountry, int nProduct, int nLayer, int l)
{
	LayerArray[nCountry][nProduct][nLayer].strWebsite = WMS_ExtractWebsiteServer(LayerData[l].strWMSRequest);
	LayerArray[nCountry][nProduct][nLayer].strServer = WMS_ExtractLayer(LayerData[l].strWMSRequest);
	LayerArray[nCountry][nProduct][nLayer].nPort = WMS_ExtractPort(LayerArray[nCountry][nProduct][nLayer].strWebsite);
	LayerArray[nCountry][nProduct][nLayer].strAttribution = WMS_ExtractDomain(LayerArray[nCountry][nProduct][nLayer].strWebsite);
	LayerArray[nCountry][nProduct][nLayer].nGfxType = LayerData[l].nGfxType;
	LayerArray[nCountry][nProduct][nLayer].nProjection = LayerData[l].nProjection;

CString str = LayerData[l].strWMSRequest;
	LayerArray[nCountry][nProduct][nLayer].strProduct.Empty();
	if (LayerData[l].strWMSRequest.Find("cycle") != -1) LayerArray[nCountry][nProduct][nLayer].strProduct = "cycle";
	if (LayerData[l].strWMSRequest.Find("transport") != -1) LayerArray[nCountry][nProduct][nLayer].strProduct = "transport";
	if (LayerData[l].strWMSRequest.Find("landscape") != -1) LayerArray[nCountry][nProduct][nLayer].strProduct = "landscape";
	if (LayerData[l].strWMSRequest.Find("outdoors") != -1) LayerArray[nCountry][nProduct][nLayer].strProduct = "outdoors";

	if (LayerData[l].strWMSRequest.Find("25000") != -1) LayerArray[nCountry][nProduct][nLayer].strProduct = "mapdata2/os/25000";
	if (LayerData[l].strWMSRequest.Find("_os1/r0") != -1) LayerArray[nCountry][nProduct][nLayer].strProduct = "_os1/r0";


	if (LayerData[l].strWMSRequest.Find("npe") != -1) LayerArray[nCountry][nProduct][nLayer].strProduct = "npe";
	if (LayerData[l].strWMSRequest.Find("os1") != -1) LayerArray[nCountry][nProduct][nLayer].strProduct = "os1";
	if (LayerData[l].strWMSRequest.Find("os7") != -1) LayerArray[nCountry][nProduct][nLayer].strProduct = "os7";
}

void CCustomMap::ParseServerRequestString_GetMapping(int nCountry, int nProduct, int nLayer, int l)
{
	LayerArray[nCountry][nProduct][nLayer].strWebsite = WMS_ExtractWebsiteServer(LayerData[l].strWMSRequest);
	LayerArray[nCountry][nProduct][nLayer].strServer = WMS_ExtractLayer(LayerData[l].strWMSRequest);
	LayerArray[nCountry][nProduct][nLayer].nPort = WMS_ExtractPort(LayerArray[nCountry][nProduct][nLayer].strWebsite);
	LayerArray[nCountry][nProduct][nLayer].strAttribution = WMS_ExtractDomain(LayerArray[nCountry][nProduct][nLayer].strWebsite);
	LayerArray[nCountry][nProduct][nLayer].strWMSLayer = WMS_ExtractParameter(LayerData[l].strWMSRequest, "size=");
	LayerArray[nCountry][nProduct][nLayer].nGfxType = LayerData[l].nGfxType;
	LayerArray[nCountry][nProduct][nLayer].nProjection = LayerData[l].nProjection;
}

//http://webmapservice.krak.dk/wms/krakbykort?kwuid=demo@krakwms.dk&kwpwd=uyu8tSdD&nbr&request=GetMap&version=1.1.1&service=WMS&bbox=705832,6203672,706088,6203928&format=image/png&width=256&height=256&LAYERS=krakbykort&STYLES=&BGCOLOR=0xFFFFFF&TRANSPARENT=TRUE&SRS=EPSG:25832
void CCustomMap::ParseServerRequestString_WMS(int nCountry, int nProduct, int nLayer, int l)
{
	LayerArray[nCountry][nProduct][nLayer].strWebsite = WMS_ExtractWebsiteServer(LayerData[l].strWMSRequest);
	LayerArray[nCountry][nProduct][nLayer].strServer = WMS_ExtractLayer(LayerData[l].strWMSRequest);
	LayerArray[nCountry][nProduct][nLayer].strMap = WMS_ExtractParameter(LayerData[l].strWMSRequest, "MAP=");
	LayerArray[nCountry][nProduct][nLayer].strWMSLayer = WMS_ExtractParameter(LayerData[l].strWMSRequest, "LAYERS=");
	LayerArray[nCountry][nProduct][nLayer].strProduct = WMS_ExtractParameter(LayerData[l].strWMSRequest, "PRODUCT=");
	LayerArray[nCountry][nProduct][nLayer].strService = WMS_ExtractParameter(LayerData[l].strWMSRequest, "SERVICE=");
	LayerArray[nCountry][nProduct][nLayer].strVersion = WMS_ExtractParameter(LayerData[l].strWMSRequest, "VERSION=");
	LayerArray[nCountry][nProduct][nLayer].strFormat = WMS_ExtractParameter(LayerData[l].strWMSRequest, "FORMAT=");
	LayerArray[nCountry][nProduct][nLayer].strBBox = WMS_ExtractParameter(LayerData[l].strWMSRequest, "BBOX=");
	LayerArray[nCountry][nProduct][nLayer].strWidth = WMS_ExtractParameter(LayerData[l].strWMSRequest, "WIDTH=");
	LayerArray[nCountry][nProduct][nLayer].strHeight = WMS_ExtractParameter(LayerData[l].strWMSRequest, "HEIGHT=");
	LayerArray[nCountry][nProduct][nLayer].strExceptions = WMS_ExtractParameter(LayerData[l].strWMSRequest, "Exceptions=");

	if (nCountry == COUNTRY_DK) {
		LayerArray[nCountry][nProduct][nLayer].strURL = WMS_ExtractParameter(LayerData[l].strWMSRequest, "kwuid=");
		LayerArray[nCountry][nProduct][nLayer].strKey = WMS_ExtractParameter(LayerData[l].strWMSRequest, "kwpwd=");
	} else {
		LayerArray[nCountry][nProduct][nLayer].strURL = WMS_ExtractParameter(LayerData[l].strWMSRequest, "URL=");
		LayerArray[nCountry][nProduct][nLayer].strKey = WMS_ExtractParameter(LayerData[l].strWMSRequest, "KEY=");
	}

	if ((LayerArray[nCountry][nProduct][nLayer].strVersion == "1.3.0") || (LayerArray[nCountry][nProduct][nLayer].strVersion == "1.3")) {
		LayerArray[nCountry][nProduct][nLayer].strEPGS = WMS_ExtractParameter(LayerData[l].strWMSRequest, "CRS=");
		LayerArray[nCountry][nProduct][nLayer].strRefSystem = "CRS";
	} else {
		LayerArray[nCountry][nProduct][nLayer].strEPGS = WMS_ExtractParameter(LayerData[l].strWMSRequest, "SRS=");
		LayerArray[nCountry][nProduct][nLayer].strRefSystem = "SRS";
	}

	if (LayerData[l].nProjection == -1) {
		LayerArray[nCountry][nProduct][nLayer].nProjection = WMS_ExtractEPGS(LayerArray[nCountry][nProduct][nLayer].strEPGS);
	} else {
		LayerArray[nCountry][nProduct][nLayer].nProjection = LayerData[l].nProjection;
	}

	LayerArray[nCountry][nProduct][nLayer].nPort = WMS_ExtractPort(LayerArray[nCountry][nProduct][nLayer].strWebsite);
	LayerArray[nCountry][nProduct][nLayer].strAttribution = WMS_ExtractDomain(LayerArray[nCountry][nProduct][nLayer].strWebsite);

	if (LayerData[l].nGfxType == -1) {
		LayerArray[nCountry][nProduct][nLayer].nGfxType = WMS_ExtractImage(LayerArray[nCountry][nProduct][nLayer].strFormat);
	} else {
		LayerArray[nCountry][nProduct][nLayer].nGfxType = LayerData[l].nGfxType;
	}
}

void CCustomMap::ParseServerRequestString_IGN(int nCountry, int nProduct, int nLayer, int l)
{
	LayerArray[nCountry][nProduct][nLayer].strWebsite = WMS_ExtractWebsiteServer(LayerData[l].strWMSRequest);
	LayerArray[nCountry][nProduct][nLayer].strServer = WMS_ExtractLayer(LayerData[l].strWMSRequest);
	LayerArray[nCountry][nProduct][nLayer].nPort = WMS_ExtractPort(LayerArray[nCountry][nProduct][nLayer].strWebsite);
	LayerArray[nCountry][nProduct][nLayer].strAttribution = WMS_ExtractDomain(LayerArray[nCountry][nProduct][nLayer].strWebsite);
	LayerArray[nCountry][nProduct][nLayer].nGfxType = LayerData[l].nGfxType;
	LayerArray[nCountry][nProduct][nLayer].nProjection = LayerData[l].nProjection;
}

void CCustomMap::ParseServerRequestString_Lux(int nCountry, int nProduct, int nLayer, int l)
{
	LayerArray[nCountry][nProduct][nLayer].strWebsite = WMS_ExtractWebsiteServer(LayerData[l].strWMSRequest);
	LayerArray[nCountry][nProduct][nLayer].strServer = WMS_ExtractLayer(LayerData[l].strWMSRequest);
	LayerArray[nCountry][nProduct][nLayer].nPort = WMS_ExtractPort(LayerArray[nCountry][nProduct][nLayer].strWebsite);
	LayerArray[nCountry][nProduct][nLayer].strAttribution = WMS_ExtractDomain(LayerArray[nCountry][nProduct][nLayer].strWebsite);
	LayerArray[nCountry][nProduct][nLayer].nGfxType = LayerData[l].nGfxType;
	LayerArray[nCountry][nProduct][nLayer].nProjection = LayerData[l].nProjection;
}

void CCustomMap::ParseServerRequestString_NGI(int nCountry, int nProduct, int nLayer, int l)
{
	LayerArray[nCountry][nProduct][nLayer].strWebsite = WMS_ExtractWebsiteServer(LayerData[l].strWMSRequest);
	LayerArray[nCountry][nProduct][nLayer].strServer = WMS_ExtractLayer(LayerData[l].strWMSRequest);
	LayerArray[nCountry][nProduct][nLayer].nPort = WMS_ExtractPort(LayerArray[nCountry][nProduct][nLayer].strWebsite);
	LayerArray[nCountry][nProduct][nLayer].strAttribution = WMS_ExtractDomain(LayerArray[nCountry][nProduct][nLayer].strWebsite);
	LayerArray[nCountry][nProduct][nLayer].nGfxType = LayerData[l].nGfxType;
	LayerArray[nCountry][nProduct][nLayer].nProjection = LayerData[l].nProjection;
}

void CCustomMap::ParseServerRequestString_CH1903(int nCountry, int nProduct, int nLayer, int l)
{
	LayerArray[nCountry][nProduct][nLayer].strWebsite = WMS_ExtractWebsiteServer(LayerData[l].strWMSRequest);
	LayerArray[nCountry][nProduct][nLayer].strServer = WMS_ExtractLayer(LayerData[l].strWMSRequest);
	LayerArray[nCountry][nProduct][nLayer].nPort = WMS_ExtractPort(LayerArray[nCountry][nProduct][nLayer].strWebsite);
	LayerArray[nCountry][nProduct][nLayer].strAttribution = WMS_ExtractDomain(LayerArray[nCountry][nProduct][nLayer].strWebsite);
	LayerArray[nCountry][nProduct][nLayer].nGfxType = LayerData[l].nGfxType;
	LayerArray[nCountry][nProduct][nLayer].nProjection = LayerData[l].nProjection;
}

// Array Initilisation.

void CCustomMap::InitialiseArrayLayer()
{
	for (int i=0; i < NUM_COUNTRY; i++) {
		for (int j=0; j < NUM_PRODUCTS; j++) {
			for (int k=0; k < NUM_LAYERS; k++) {

				LayerArray[i][j][k].nPixels = -1;
				LayerArray[i][j][k].nMetres = -1;
				LayerArray[i][j][k].nGfxType = -1;
				LayerArray[i][j][k].nBitDepth = -1;

				LayerArray[i][j][k].strWebsite.Empty();
				LayerArray[i][j][k].strServer.Empty();
				LayerArray[i][j][k].strAttribution.Empty();
				LayerArray[i][j][k].strMap.Empty();
				LayerArray[i][j][k].strWMSLayer.Empty();
				LayerArray[i][j][k].strService.Empty();
				LayerArray[i][j][k].strVersion.Empty();
				LayerArray[i][j][k].strFormat.Empty();
				LayerArray[i][j][k].strBBox.Empty();
				LayerArray[i][j][k].strWidth.Empty();
				LayerArray[i][j][k].strHeight.Empty();
				LayerArray[i][j][k].strExceptions.Empty();
				LayerArray[i][j][k].strKey.Empty();
				LayerArray[i][j][k].strURL.Empty();
				LayerArray[i][j][k].strRefSystem.Empty(); 
				LayerArray[i][j][k].strImage.Empty();

				LayerArray[i][j][k].nProjection = -1;
				LayerArray[i][j][k].nPort = -1;
			
				LayerArray[i][j][k].strCode.Empty();
				LayerArray[i][j][k].strName.Empty();

				LayerArray[i][j][k].nZoom = -1;
			}
		}
	}

	for (int l=0; true; l++) {

		int nCountry = LayerData[l].nCountry;
		int nProduct = LayerData[l].nProduct;
		int nLayer  = LayerData[l].nLayer;

		if (nCountry == -1) break;

		LayerArray[nCountry][nProduct][nLayer].nPixels = LayerData[l].nPixels;
		LayerArray[nCountry][nProduct][nLayer].nMetres = LayerData[l].nMetres;
		LayerArray[nCountry][nProduct][nLayer].nBitDepth = LayerData[l].nBitDepth;
		LayerArray[nCountry][nProduct][nLayer].nServerType = LayerData[l].nServerType;

		LayerArray[nCountry][nProduct][nLayer].nZoom = LayerData[l].nZoom;

		if      (LayerData[l].nServerType == WMS)			ParseServerRequestString_WMS(nCountry, nProduct, nLayer, l);
		else if (LayerData[l].nServerType == GetMapping)	ParseServerRequestString_GetMapping(nCountry, nProduct, nLayer, l);
		else if (LayerData[l].nServerType == OSM)			ParseServerRequestString_OSM(nCountry, nProduct, nLayer, l);
		else if (LayerData[l].nServerType == Kompass)		ParseServerRequestString_Kompass(nCountry, nProduct, nLayer, l);
		else if (LayerData[l].nServerType == Bing)			ParseServerRequestString_Bing(nCountry, nProduct, nLayer, l);
		else if (LayerData[l].nServerType == SatMap)		ParseServerRequestString_SatMap(nCountry, nProduct, nLayer, l);
		else if (LayerData[l].nServerType == IGN)			ParseServerRequestString_IGN(nCountry, nProduct, nLayer, l);
		else if (LayerData[l].nServerType == LUX)			ParseServerRequestString_Lux(nCountry, nProduct, nLayer, l);
		else if (LayerData[l].nServerType == NGI)			ParseServerRequestString_NGI(nCountry, nProduct, nLayer, l);
		else if (LayerData[l].nServerType == CH1903)		ParseServerRequestString_CH1903(nCountry, nProduct, nLayer, l);
		else if (LayerData[l].nServerType == ArcGIS)		ParseServerRequestString_ArcGIS(nCountry, nProduct, nLayer, l);

		if (ProductArray[nCountry][nProduct].nDefaultScale == -1) ProductArray[nCountry][nProduct].nDefaultScale = nLayer;

		LayerArray[nCountry][nProduct][nLayer].strCode = LayerData[l].strCode;
		LayerArray[nCountry][nProduct][nLayer].strName = LayerData[l].strName;

		if (ProductArray[nCountry][nProduct].nNumLayers < nLayer) ProductArray[nCountry][nProduct].nNumLayers = nLayer;

		LayerArray[nCountry][nProduct][nLayer].strReferer = LayerData[l].strReferer;
	}
}

void CCustomMap::InitialiseArrayCountry()
{
	for (int i=0; i < NUM_COUNTRY; i++) {

		CountryArray[i].nDatum = -1;
		
		CountryArray[i].strRegistryValue.Empty();

		CountryArray[i].strCode.Empty();
		CountryArray[i].strName.Empty();

		CountryArray[i].nCoordSystem = -1;
		
		CountryArray[i].nNumProducts = -1;
	}

	int l;
	for (l=0; true; l++) {

		int nCountry = CountryData[l].nCountry;

		if (nCountry == -1) break;

		CountryArray[nCountry].nDatum = CountryData[l].nDatum;

		CountryArray[nCountry].strCode = CountryData[l].strCode;
		CountryArray[nCountry].strName = CountryData[l].strName;

//		CountryArray[nCountry].strRegistryValue = CountryData[l].strRegistryValue;
		CountryArray[nCountry].strExampleGrid = CountryData[l].strExampleGrid;
		CountryArray[nCountry].strExampleLatLon = CountryData[l].strExampleLatLon;
		CountryArray[nCountry].strCoordSystem = CountryData[l].strCoordSystem;
	}
	m_nCountryCount = l;
}

void CCustomMap::InitialiseArrayProduct()
{
	for (int i=0; i < NUM_COUNTRY; i++) {
		for (int j=0; j < NUM_PRODUCTS; j++) {

			ProductArray[i][j].strCode.Empty();
			ProductArray[i][j].strName.Empty();
			ProductArray[i][j].nDefaultScale = -1;

			ProductArray[i][j].nNumLayers = -1;
		}
	}

	for (int l=0; true; l++) {

		int nCountry = ProductData[l].nCountry;
		int nProduct = ProductData[l].nProduct;

		if (nCountry == -1) break;

		ProductArray[nCountry][nProduct].strCode = ProductData[l].strCode;
		ProductArray[nCountry][nProduct].strName = ProductData[l].strName;

		if (CountryArray[nCountry].nNumProducts < nProduct) CountryArray[nCountry].nNumProducts = nProduct;
	}
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
				CountryArray[nCountry].strCode,
				ProductArray[nCountry][nProduct].strCode);
	
//	return CountryArray[nCountry].strRegistryValue;
	return str;
}

CString CCustomMap::GetExampleLatLon(int nCountry)
{
	if (nCountry < 0) {
		nCountry = m_nCountry;
	}
	return CountryArray[nCountry].strExampleLatLon;
}

CString CCustomMap::GetExampleGrid(int nCountry)
{
	if (nCountry < 0) {
		nCountry = m_nCountry;
	}
	return CountryArray[nCountry].strExampleGrid;
}

CString CCustomMap::GetCoordSystemName(int nCountry)
{
	if (nCountry < 0) {
		nCountry = m_nCountry;
	}
	return CountryArray[nCountry].strCoordSystem;
}

void CCustomMap::MakeDatabaseFolders()
{
	CString strFolder;

	strFolder.Format("%s\\%s", m_strDatabaseRoot, CountryCode());
	_mkdir(strFolder);

	strFolder.Format("%s\\%s\\%s", m_strDatabaseRoot, CountryCode(), ProductCode());
	_mkdir(strFolder);

	strFolder.Format("%s\\%s\\%s\\%s", m_strDatabaseRoot, CountryCode(), ProductCode(), LayerCode());
	_mkdir(strFolder);
}

CCustomMap::CCustomMap(void)
{
	InitialiseArrayCountry();
	InitialiseArrayProduct();
	InitialiseArrayLayer();
}

CCustomMap::~CCustomMap(void)
{
}

CCustomMap::CCustomMap(int nCountry, int nProduct, int nLayer)
{
	m_nCountry = nCountry;
	m_nProduct = nProduct;
	m_nLayer  = nLayer;
//	UpdateWorkingData();
}

void CCustomMap::Update(int nCountry, int nProduct, int nLayer)
{
	m_nCountry = nCountry;
	m_nProduct = nProduct;
	m_nLayer  = nLayer;
//	UpdateWorkingData();
}

void CCustomMap::UpdateCountry(int nCountry)
{
	m_nCountry = nCountry;
	AfxGetApp()->WriteProfileInt("Main", "Country", nCountry);
//	UpdateWorkingData();
}

void CCustomMap::UpdateProduct(int nProduct)
{
	m_nProduct = nProduct;
	AfxGetApp()->WriteProfileInt("Main", "Product", nProduct);
//	UpdateWorkingData();
}

void CCustomMap::UpdateLayer(int nLayer)
{
	m_nLayer = nLayer;
	AfxGetApp()->WriteProfileInt("Main", "Layer", nLayer);
//	UpdateWorkingData();
}

/*
void CCustomMap::UpdateWorkingData()
{
//	m_nDatum = CountryData[m_nCountry].nDatum;
//	m_nMapScale = MapScales[m_nLayer].nLayer;

//	m_nPixelsPerTile = LayerArray[m_nCountry][m_nProduct][m_nLayer].nPixels;
//	m_nMetresPerTile = LayerArray[m_nCountry][m_nProduct][m_nLayer].nMeters;
//	m_nTileBitDepth  = LayerArray[m_nCountry][m_nProduct][m_nLayer].nBitDepth;

//m_nCoordSystem = CountryArray[m_nCountry].nCoordSystem;
}
*/

CString CCustomMap::GetMapName(CRect rectBBox)
{
	CString strMapName;

	strMapName.Format("%s_%s_%s_%d_%d",
					CountryCode(),
					ProductCode(),
					LayerCode(),
					rectBBox.CenterPoint().x,
					rectBBox.CenterPoint().y);

	return strMapName;
}

int CCustomMap::GetProjection() 
{
	return LayerArray[m_nCountry][m_nProduct][m_nLayer].nProjection;
}

int CCustomMap::GetServerType() 
{	
	return LayerArray[m_nCountry][m_nProduct][m_nLayer].nServerType;
}

CString CCustomMap::wmsGetAttribution() { return LayerArray[m_nCountry][m_nProduct][m_nLayer].strAttribution; }

int CCustomMap::wmsGetPort() { return LayerArray[m_nCountry][m_nProduct][m_nLayer].nPort; }

CString GfxTypeMapping[] = {"jpeg", "png", "bmp", "gif", "jpg"};

// #define WMS_KRAK	"http://webmapservice.krak.dk/wms/krakbykort?kwuid=demo@krakwms.dk&kwpwd=uyu8tSdD&nbr&request=GetMap&version=1.1.1&service=WMS&bbox=705576,6203416,705832,6203672&format=image/png&width=256&height=256&LAYERS=krakbykort&SRS=EPSG:25832"

CString CCustomMap::GetRequest_WMS(int easting, int northing)
{
	int nPixels = LayerArray[m_nCountry][m_nProduct][m_nLayer].nPixels;
	int nMetres = LayerArray[m_nCountry][m_nProduct][m_nLayer].nMetres;

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
				LayerArray[m_nCountry][m_nProduct][m_nLayer].strServer,
				LayerArray[m_nCountry][m_nProduct][m_nLayer].strWMSLayer,
				LayerArray[m_nCountry][m_nProduct][m_nLayer].strVersion,
				GfxTypeMapping[LayerArray[m_nCountry][m_nProduct][m_nLayer].nGfxType],
				LayerArray[m_nCountry][m_nProduct][m_nLayer].strRefSystem, LayerArray[m_nCountry][m_nProduct][m_nLayer].nProjection,
	//			LayerArray[m_nCountry][m_nProduct][m_nLayer].strURL,
	//			LayerArray[m_nCountry][m_nProduct][m_nLayer].strKey,
				easting, northing, easting + nMetres, northing + nMetres,
				nPixels, nPixels);
	} else if (m_nCountry == COUNTRY_IT) {
		strRequest.Format(
				"%s"
				"?LAYERS=%s"
				"&MAP=%s"
				"&PRODUCT=%s"
				"&SERVICE=WMS&VERSION=%s"
				"&REQUEST=GetMap"
				"&STYLES="
				"&FORMAT=image%%2F%s"
				"&%s=EPSG%%3A%d"
				"&URL=%s"
				"&KEY=%s"
				"&BBOX=%d,%d,%d,%d"
				"&WIDTH=%d&HEIGHT=%d",
				LayerArray[m_nCountry][m_nProduct][m_nLayer].strServer,
				LayerArray[m_nCountry][m_nProduct][m_nLayer].strWMSLayer,
				LayerArray[m_nCountry][m_nProduct][m_nLayer].strMap,
				LayerArray[m_nCountry][m_nProduct][m_nLayer].strProduct,
				LayerArray[m_nCountry][m_nProduct][m_nLayer].strVersion,
				GfxTypeMapping[LayerArray[m_nCountry][m_nProduct][m_nLayer].nGfxType],
				LayerArray[m_nCountry][m_nProduct][m_nLayer].strRefSystem, LayerArray[m_nCountry][m_nProduct][m_nLayer].nProjection,
				LayerArray[m_nCountry][m_nProduct][m_nLayer].strURL,
				LayerArray[m_nCountry][m_nProduct][m_nLayer].strKey,
				easting, northing, easting + nMetres, northing + nMetres,
				nPixels, nPixels);
	} else {
	
		strRequest.Format(
				"%s"
				"?LAYERS=%s"
				"&PRODUCT=%s"
				"&SERVICE=WMS&VERSION=%s"
				"&REQUEST=GetMap"
				"&STYLES="
				"&FORMAT=image%%2F%s"
				"&%s=EPSG%%3A%d"
				"&URL=%s"
				"&KEY=%s"
				"&BBOX=%d,%d,%d,%d"
				"&WIDTH=%d&HEIGHT=%d",
				LayerArray[m_nCountry][m_nProduct][m_nLayer].strServer,
				LayerArray[m_nCountry][m_nProduct][m_nLayer].strWMSLayer,
				LayerArray[m_nCountry][m_nProduct][m_nLayer].strProduct,
				LayerArray[m_nCountry][m_nProduct][m_nLayer].strVersion,
				GfxTypeMapping[LayerArray[m_nCountry][m_nProduct][m_nLayer].nGfxType],
				LayerArray[m_nCountry][m_nProduct][m_nLayer].strRefSystem, LayerArray[m_nCountry][m_nProduct][m_nLayer].nProjection,
				LayerArray[m_nCountry][m_nProduct][m_nLayer].strURL,
				LayerArray[m_nCountry][m_nProduct][m_nLayer].strKey,
				easting, northing, easting + nMetres, northing + nMetres,
				nPixels, nPixels);
	}

	return strRequest;
}

// #define WMS_GETMAPPING	"http://www.getmapping.com/preview/preview.ashx?X=486463.0&Y=496681.5&productID=9&dataSet=MM&size=500.0"

CString CCustomMap::GetRequest_GetMapping(int easting, int northing)
{
	double dMetres = LayerArray[m_nCountry][m_nProduct][m_nLayer].nMetres / 2.0;

	CString strRequest;

	strRequest.Format(
			"%s"
			"?size=%s"
			"&X=%.1f&Y=%.1f",
			LayerArray[m_nCountry][m_nProduct][m_nLayer].strServer,
			LayerArray[m_nCountry][m_nProduct][m_nLayer].strWMSLayer,
			easting + dMetres, northing + dMetres);

	return strRequest;
}

// http://tile.openstreetmap.org/18/130584/83719.png
CString CCustomMap::GetRequest_OSM(int easting, int northing)
{
	CString strRequest;

	int levelOfDetail = LayerArray[m_nCountry][m_nProduct][m_nLayer].nZoom;

	if (LayerArray[m_nCountry][m_nProduct][m_nLayer].strProduct.IsEmpty()) {
		strRequest.Format(
				"%d/%d/%d.png",
				levelOfDetail,
				easting, northing);
	} else {

		strRequest.Format(
				"%s/%d/%d/%d.%s",
				LayerArray[m_nCountry][m_nProduct][m_nLayer].strProduct,
				levelOfDetail,
				easting, northing,
				GfxTypeMapping[LayerArray[m_nCountry][m_nProduct][m_nLayer].nGfxType]);
	}

	return strRequest;
}

//#define WMS_Kompass		"http://ec2.cdn.ecmaps.de/WmsGateway.ashx.jpg?Experience=kompass&MapStyle=KOMPASS%20Touristik&TileX=17018&TileY=10975&ZoomLevel=15"
CString CCustomMap::GetRequest_Kompass(int easting, int northing)
{
	CString strRequest;

	int levelOfDetail = LayerArray[m_nCountry][m_nProduct][m_nLayer].nZoom;

	strRequest.Format(
			"%s"
			"?Experience=kompass&MapStyle=KOMPASS%%20Touristik&TileX=%d&TileY=%d&ZoomLevel=%d",
			LayerArray[m_nCountry][m_nProduct][m_nLayer].strServer,
			easting, northing, levelOfDetail);

	return strRequest;
}

// #define WMS_BING	"http://a1.ortho.tiles.virtualearth.net/tiles/a031311321031011.jpeg?g=52"
CString CCustomMap::GetRequest_Bing(int easting, int northing)
{
	CString strRequest;
	CString strQuadKey;

	int levelOfDetail = LayerArray[m_nCountry][m_nProduct][m_nLayer].nZoom;

	strQuadKey = TileXYToQuadKey(easting, northing, levelOfDetail);

	strRequest.Format(
			"tiles/a%s.jpeg?g=52",
			strQuadKey);

	return strRequest;
}

// #define WMS_SATMAP	"http://tileserver.satmap.com/AT/14/12023002320313.ipic"
CString CCustomMap::GetRequest_SatMap(int easting, int northing)
{
	CString strRequest;
	CString strQuadKey;

	int levelOfDetail = LayerArray[m_nCountry][m_nProduct][m_nLayer].nZoom;

	strQuadKey = TileXYToQuadKey(easting, northing, levelOfDetail);

	if (LayerArray[m_nCountry][m_nProduct][m_nLayer].strWMSLayer.IsEmpty()) {
		strRequest.Format(
				"%s/%s.ipic",
				LayerArray[m_nCountry][m_nProduct][m_nLayer].strProduct,
				strQuadKey);
	} else {
		strRequest.Format(
				"%s/%s/%s.ipic",
				LayerArray[m_nCountry][m_nProduct][m_nLayer].strProduct,
				LayerArray[m_nCountry][m_nProduct][m_nLayer].strWMSLayer,
				strQuadKey);
	}

	return strRequest;
}

// #define WMS_IGN		"http://gpp3-wxs.ign.fr/sg68l6zf6zemkg3cdr0bknay/wmts?SERVICE=WMTS&REQUEST=GetTile&VERSION=1.0.0&LAYER=GEOGRAPHICALGRIDSYSTEMS.MAPS&STYLE=normal&TILEMATRIXSET=PM&TILEMATRIX=16&TILEROW=23149&TILECOL=33916&FORMAT=image/jpeg"
CString CCustomMap::GetRequest_IGN(int easting, int northing)
{
	CString strRequest;

	int levelOfDetail = LayerArray[m_nCountry][m_nProduct][m_nLayer].nZoom;

// #define WMS_IGN		"http://gpp3-wxs.ign.fr/sg68l6zf6zemkg3cdr0bknay/wmts?SERVICE=WMTS&REQUEST=GetTile&VERSION=1.0.0&LAYER=GEOGRAPHICALGRIDSYSTEMS.MAPS&STYLE=normal&TILEMATRIXSET=PM&TILEMATRIX=16&TILEROW=23149&TILECOL=33916&FORMAT=image/jpeg"
	strRequest.Format(
			"%s"
			"?SERVICE=WMTS&REQUEST=GetTile&VERSION=1.0.0&LAYER=GEOGRAPHICALGRIDSYSTEMS.MAPS&STYLE=normal&TILEMATRIXSET=PM"
			"&TILEMATRIX=%d&TILEROW=%d&TILECOL=%d&FORMAT=image/jpeg",
			LayerArray[m_nCountry][m_nProduct][m_nLayer].strServer,
			levelOfDetail, northing, easting);

	return strRequest;
}

/*
// the map resolutions for the given theme
geoadmin.resolutions = "500.0,250.0,150.0,100.0,50.0,20.0,10.0,5.0,2.0,1.0,0.5,0.25";
geoadmin.fakeScales = "750,1500,3000,6000,15000,30000,60000,150000,300000,450000,700000,1500000";
geoadmin.printScales = "1500,2500,5000,10000,15000,20000,25000,50000,100000,200000,25000,400000";


geoadmin.startZoom = "2";
geoadmin.startX = "100000";
geoadmin.startY = "75000";
geoadmin.startLayers = "";

[0,0,150000,200000]?
*/

// #define WMS_LUX		"http://tile2.geoportail.lu/topo_gr/08/000/000/154/000/000/195.png"
// http://tile2.geoportail.lu/topo_gr/07/000/000/065/000/000/075.png
CString CCustomMap::GetRequest_Lux(int easting_tile, int northing_tile)
{
	CString strRequest;

	int levelOfDetail = LayerArray[m_nCountry][m_nProduct][m_nLayer].nZoom;
/*
//int z = 8;

//int pixels_per_tile = 256;

	int pixels_per_tile = LayerArray[m_nCountry][m_nProduct][m_nLayer].nPixels;
	int metres_per_tile = LayerArray[m_nCountry][m_nProduct][m_nLayer].nMetres;

//	double metres_per_pixel = 2.0;

	double metres_per_pixel = metres_per_tile / (double) pixels_per_tile;

double northing_pixel = northing / metres_per_pixel;
long northing_tile = (long)(northing_pixel/pixels_per_tile);

double easting_pixel = easting / metres_per_pixel;
long easting_tile = (long)(easting_pixel/pixels_per_tile);
*/
	strRequest.Format(
			"topo_gr/%02d/000/000/%03d/000/000/%03d.png",
			levelOfDetail,
			easting_tile, northing_tile);

	return strRequest;
}

/*
"xmin": 484647.1136336568,
"ymin": 502357.87922870886,
"xmax": 822367.8262509668,
"ymax": 780354.4553065482,
*/

//#define WMS_NGI		"http://www.ngi.be/cartoweb/1.0.0/topo/default/3812/7/116/162.png"
//http://www.ngi.be/cartoweb/1.0.0/topo/default/3812/9/596/795.png
CString CCustomMap::GetRequest_NGI(int easting, int northing)
{
	CString strRequest;

	int levelOfDetail = LayerArray[m_nCountry][m_nProduct][m_nLayer].nZoom;

	/*
double xOrigin = 450000.0;
double yOrigin = 800000.0;


	int levelOfDetail = LayerArray[m_nCountry][m_nProduct][m_nLayer].nZoom;


//int pixels_per_tile = 256;

	int pixels_per_tile = LayerArray[m_nCountry][m_nProduct][m_nLayer].nPixels;
	int metres_per_tile = LayerArray[m_nCountry][m_nProduct][m_nLayer].nMetres;


//	double metres_per_pixel = metres_per_tile / pixels_per_tile;
	double metres_per_pixel = 2.6458386250105836;

double northing_pixel = (yOrigin - northing) / metres_per_pixel;
//double northing_pixel = (ymax - northing) / metres_per_pixel;
long northing_tile = (long)(northing_pixel/pixels_per_tile);

double easting_pixel = (easting - xOrigin) / metres_per_pixel;
//double easting_pixel = (easting - xmin) / metres_per_pixel;
long easting_tile = (long)(easting_pixel/pixels_per_tile);
*/

	strRequest.Format(
			"cartoweb/1.0.0/topo/default/3812/%d/%d/%d.png",
			levelOfDetail,
			northing, easting);

	return strRequest;
}

// http://wmts1.geo.admin.ch/1.0.0/ch.swisstopo.pixelkarte-farbe/default/20130213/21781/23/359/494.jpeg
CString CCustomMap::GetRequest_CH1903(int easting, int northing)
{
	CString strRequest;

	int levelOfDetail = LayerArray[m_nCountry][m_nProduct][m_nLayer].nZoom;

	strRequest.Format(
			"1.0.0/ch.swisstopo.pixelkarte-farbe/default/20130213/21781/%d/%d/%d.jpeg",
			levelOfDetail,
			northing, easting);

	return strRequest;
}

// http://services.arcgisonline.com/ArcGIS/rest/services/World_Topo_Map/MapServer/tile/18/84212/130283"
CString CCustomMap::GetRequest_ArcGIS(int easting, int northing)
{
	CString strRequest;

	int levelOfDetail = LayerArray[m_nCountry][m_nProduct][m_nLayer].nZoom;

	strRequest.Format(
			"ArcGIS/rest/services/World_Topo_Map/MapServer/tile/%d/%d/%d",
			levelOfDetail, northing, easting);

	return strRequest;
}



void CCustomMap::WriteProfile(CString strGridRef)
{
//	CString str;
	
//	str.Format("%s,%d,%d", strGridRef, m_nProduct, m_nLayer);
	AfxGetApp()->WriteProfileString("Main", GetRegistryValueName(), /*str*/strGridRef);
}

void CCustomMap::ReadProfile(CString& strGridRef)
{
	CString str;

//CString strReg = GetRegistryValueName();
	str = AfxGetApp()->GetProfileString("Main", GetRegistryValueName(), "");
//	sscanf(str,"%s,%d,%d", strGridRef, m_nProduct, m_nLayer);

	strGridRef = str;
}

CString CCustomMap::GetDatabasePath()
{
	CString str;
	str.Format("%s\\%s\\%s\\%s", m_strDatabaseRoot, CountryCode(), ProductCode(), LayerCode());
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

int CCustomMap::GetLayer()
{
	return m_nLayer;
}






int CCustomMap::GetDatum()
{ 
	return CountryData[m_nCountry].nDatum;
}

int CCustomMap::GetPixelsPerTile()
{
	return LayerArray[m_nCountry][m_nProduct][m_nLayer].nPixels;
}

int CCustomMap::GetMetresPerTile()
{
	return LayerArray[m_nCountry][m_nProduct][m_nLayer].nMetres;
}

int CCustomMap::GetTileBitDepth()
{
	return LayerArray[m_nCountry][m_nProduct][m_nLayer].nBitDepth;
}

int CCustomMap::GetTileGfxType()
{
	return LayerArray[m_nCountry][m_nProduct][m_nLayer].nGfxType;
}

int CCustomMap::GetCoordinateSystem()
{
	return m_nCoordSystem;
}

int CCustomMap::GetLevelOfDetail(int nCountry, int nProduct, int nLayer)
{
	if (nCountry < 0) {
		nCountry = m_nCountry;
	}
	if (nProduct < 0) {
		nProduct = m_nProduct;
	}
	if (nLayer < 0) {
		nLayer = m_nLayer;
	}
	return LayerArray[m_nCountry][m_nProduct][m_nLayer].nZoom;
}


int CCustomMap::NumberProducts(int nCountry)
{
	if (nCountry < 0) {
		nCountry = m_nCountry;
	}
	return CountryArray[nCountry].nNumProducts+1;
}

int CCustomMap::NumberLayers(int nCountry, int nProduct)
{
	if (nCountry < 0) {
		nCountry = m_nCountry;
	}
	if (nProduct < 0) {
		nProduct = m_nProduct;
	}
	return ProductArray[nCountry][nProduct].nNumLayers+1;
}

CString CCustomMap::CountryCode(int nCountry)
{
	if (nCountry < 0) {
		nCountry = m_nCountry;
	}
	return CountryArray[nCountry].strCode;
}

CString CCustomMap::CountryName(int nCountry)
{
	if (nCountry < 0) {
		nCountry = m_nCountry;
	}
	return CountryArray[nCountry].strName;
}

CString CCustomMap::ProductCode(int nCountry, int nProduct)
{
	if (nCountry < 0) {
		nCountry = m_nCountry;
	}
	if (nProduct < 0) {
		nProduct = m_nProduct;
	}
	return ProductArray[nCountry][nProduct].strCode;
}

CString CCustomMap::ProductName(int nCountry, int nProduct)
{
	if (nCountry < 0) {
		nCountry = m_nCountry;
	}
	if (nProduct < 0) {
		nProduct = m_nProduct;
	}
	return ProductArray[nCountry][nProduct].strName;
}

CString CCustomMap::LayerCode(int nCountry, int nProduct, int nLayer)
{
	if (nCountry < 0) {
		nCountry = m_nCountry;
	}
	if (nProduct < 0) {
		nProduct = m_nProduct;
	}
	if (nLayer < 0) {
		nLayer = m_nLayer;
	}

	return LayerArray[nCountry][nProduct][nLayer].strCode;
}

CString CCustomMap::LayerName(int nCountry, int nProduct, int nLayer)
{
	if (nCountry < 0) {
		nCountry = m_nCountry;
	}
	if (nProduct < 0) {
		nProduct = m_nProduct;
	}
	if (nLayer < 0) {
		nLayer = m_nLayer;
	}

	return LayerArray[nCountry][nProduct][nLayer].strName;
}

CString CCustomMap::Referer(int nCountry, int nProduct, int nLayer)
{
	if (nCountry < 0) {
		nCountry = m_nCountry;
	}
	if (nProduct < 0) {
		nProduct = m_nProduct;
	}
	if (nLayer < 0) {
		nLayer = m_nLayer;
	}

	return LayerArray[nCountry][nProduct][nLayer].strReferer;
}
