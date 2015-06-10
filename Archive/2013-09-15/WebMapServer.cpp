#include "stdafx.h"

#include "WebMapServer.h"

#include "ProgressWindow.h"

#include "CustomMap.h"
extern CCustomMap MyMap;

extern int abortProgram;


struct wmsMapRequests {

	int		nWMS;
	int		nPort;
	char*	strAttribution;
	char*	strWMSVersion;
	char*	strRefSystem;			//	SRS (Spatial Reference System) or CRS (Coordinate Reference System)
	char*	strProjection;

	char*	strUrl;

} wmsMapRequests[] = {

	{WMS_AT,		80,	"www.geoland.at",						"1.1.1", "SRS", "31287", ""},
	{WMS_NGI_BE,	80,	"ngi.be",								"1.3.0", "CRS", "3812",  ""},
	{WMS_OSGB,		80,	"osopenspacepro.ordnancesurvey.co.uk",	"1.1.1", "SRS", "27700", ""},
	{WMS_IE,		80,	"maps.osi.ie", "", "", "", ""},
	{WMS_FR,		80,	"gpp3-wxs.ign.fr",						"1.0.0", "", "", ""},
	{WMS_NRW_DE,	80,	"www.wms.nrw.de",						"1.1.1", "SRS", "31467", ""},
	{WMS_IT,		80,	"wms.pcn.minambiente.it", "", "", "", ""},
	{WMS_LU,		80,	"ecadastre.public.lu", "", "", "", ""},
	{WMS_NL,		80,	"", "", "", "", ""},
	{WMS_NI,		80,	"", "", "", "", ""},
	{WMS_PT,		80,	"mapas.igeo.pt", "", "", "", ""},
	{WMS_IGN_ES,	80,	"www.idee.es",							"1.1.1", "SRS", "23030", ""},
	{WMS_CH,		80,	"map.geodataviewer.admin.ch", "", "", "", ""},
	{WMS_PL,		80,	"ars.geoportal.gov.pl", "", "", "", ""},
	{WMS_CZ,		80,	"geoportal.cuzk.cz",					"1.1.1", "SRS", "32633", ""},
	{WMS_SI,		80,	"prostor2.gov.si",						"1.3",	 "CRS", "3912",  ""},
	{WMS_CAT,		80,	"shagrat.icc.es",						"1.1.1", "SRS", "23031", ""},
	{WMS_HIGHLAND, 8080, "www.walkhighlands.co.uk",				"1.1.1", "SRS", "27700", "&URL=http%3A%2F%2Fwww.walkhighlands.co.uk%2F"},

// http://www.geodaten-mv.de/dienste/adv_dtk50?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=mv_dtk50&BBOX=4461245,5943634,4463517,5945149&SRS=EPSG:31468&FORMAT=image/jpeg&WIDTH=300&HEIGHT=200&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage
	{WMS_MV_DE,	80,	"www.geodaten-mv.de",						"1.1.1", "SRS", "31467", ""},
};	

struct wmsMapLayers {

	int		nMapLayer;
	char*	strServer;
	char*	strLayer;
	char*	strImage;

} wmsMapLayers[] = {

	{WMS_NGI_BE_10K,	"testbed/wms/top10r_l08_fr", "0", "png"},
	{WMS_NGI_BE_50K,	"testbed/wms/top50r_l08_fr", "0", "png"},

	{WMS_NRW_DE_10K,	"geobasis/adv_dtk10", "DTK10", "png"},
	{WMS_NRW_DE_25K,	"geobasis/adv_dtk25", "DTK25vi", "png"},
	{WMS_NRW_DE_50K,	"wms/dlm50", "dlm_50", "png"},

	{WMS_IGN_ES_50K,	"wms/MTN-Raster/MTN-Raster", "mtn_rasterizado", "png"},

	{WMS_CZ_50K,		"WMS_ZM50_PUB/WMService.aspx", "GR_ZM50", "png"},
	{WMS_SI_50K,		"ows2-m-pub/ows", "raster_gurs_pub:SI.GURS.RASTRI.DTK50", "png"},

	{WMS_CAT_10K,		"lizardtech/iserv/ows", "mtc10m", "png"},
	{WMS_CAT_25K,		"lizardtech/iserv/ows", "mtc25m", "png"},
	{WMS_CAT_50K,		"lizardtech/iserv/ows", "mtc50m", "png"},

	{WMS_HIGHLAND_25K,	"geoserver/wms", "osgb:25k", "png"},
	{WMS_HIGHLAND_50K,	"geoserver/wms", "osgb:50k", "png"},


// http://www.geodaten-mv.de/dienste/adv_dtk50?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=mv_dtk50&BBOX=4461245,5943634,4463517,5945149&SRS=EPSG:31468&FORMAT=image/jpeg&WIDTH=300&HEIGHT=200&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage
	{WMS_MV_DE_50K,		"dienste/adv_dtk50", "mv_dtk50", "png"},
	{WMS_MV_DE_25K,		"dienste/adv_dtk25", "mv_dtk25", "png"},
	{WMS_MV_DE_10K,		"dienste/adv_dtk10", "mv_dtk10", "png"},
};

/*
struct wmsMapLayers10K {

	int		nCountry;
	char*	strServer;
	char*	strLayer;
	char*	strImage;

} wmsMapLayers[] = {

	{COUNTRY_AT,	"", "", ""},
	{COUNTRY_BE,	"testbed/wms/top10r_l08_fr", "0", "png"},
	{COUNTRY_UK,	"", "", ""},
	{COUNTRY_IE,	"", "", ""},
	{COUNTRY_FR,	"", "", ""},
	{COUNTRY_DE,	"geobasis/adv_dtk10", "DTK10", "png"},
	{COUNTRY_IT,	"", "", ""},
	{COUNTRY_LU,	"", "", ""},
	{COUNTRY_NL,	"", "", ""},
	{COUNTRY_NI,	"", "", ""},
	{COUNTRY_PT,	"", "", ""},
	{COUNTRY_ES,	"", "", ""},
	{COUNTRY_CH,	"", "", ""},
	{COUNTRY_PL,	"", "", ""},
	{COUNTRY_CZ,	"", "", ""},
	{COUNTRY_SI,	"", "", ""},
	{COUNTRY_CAT,	"lizardtech/iserv/ows", "mtc10m", "png"},
	{COUNTRY_SCO,	"", "", ""},
};

struct wmsMapLayers25K {

	int		nCountry;
	char*	strServer;
	char*	strLayer;
	char*	strImage;

} wmsMapLayers25K[] = {

	{COUNTRY_AT,	"", "", ""},
	{COUNTRY_BE,	"", "", ""},
	{COUNTRY_UK,	"", "", ""},
	{COUNTRY_IE,	"", "", ""},
	{COUNTRY_FR,	"", "", ""},
	{COUNTRY_DE,	"geobasis/adv_dtk25", "DTK25vi", "png"},
	{COUNTRY_IT,	"", "", ""},
	{COUNTRY_LU,	"", "", ""},
	{COUNTRY_NL,	"", "", ""},
	{COUNTRY_NI,	"", "", ""},
	{COUNTRY_PT,	"", "", ""},
	{COUNTRY_ES,	"", "", ""},
	{COUNTRY_CH,	"", "", ""},
	{COUNTRY_PL,	"", "", ""},
	{COUNTRY_CZ,	"", "", ""},
	{COUNTRY_SI,	"", "", ""},
	{COUNTRY_CAT,	"lizardtech/iserv/ows", "mtc25m", "png"},
	{COUNTRY_SCO,	"geoserver/wms", "osgb:25k", "png"},
};

struct wmsMapLayers50K {

	int		nCountry;
	char*	strServer;
	char*	strLayer;
	char*	strImage;

} wmsMapLayers50K[] = {

	{COUNTRY_AT,	 "geo_webgis3/ogc.aspx?map=geo_31287", "38%%2C39%%2C40", "png"},
	{COUNTRY_BE,	 "testbed/wms/top50r_l08_fr", "0", "png"},
	{COUNTRY_UK,	 "", "", ""},
	{COUNTRY_IE,	 "", "", ""},
	{COUNTRY_FR,	 "", "", ""},
	{COUNTRY_DE,	 "wms/dlm50", "dlm_50", "png"},
	{COUNTRY_IT,	 "", "", ""},
	{COUNTRY_LU,	 "", "", ""},
	{COUNTRY_NL,	 "", "", ""},
	{COUNTRY_NI,	 "", "", ""},
	{COUNTRY_PT,	 "", "", ""},
	{COUNTRY_ES,	 "wms/MTN-Raster/MTN-Raster", "mtn_rasterizado", "png"},
	{COUNTRY_CH,	 "", "", ""},
	{COUNTRY_PL,	 "", "", ""},
	{COUNTRY_CZ,	 "WMS_ZM50_PUB/WMService.aspx", "GR_ZM50", "png"},
	{COUNTRY_SI,	 "ows2-m-pub/ows", "raster_gurs_pub:SI.GURS.RASTRI.DTK50", "png"},
	{COUNTRY_CAT,	 "lizardtech/iserv/ows", "mtc50m", "png"},
	{COUNTRY_SCO,	 "geoserver/wms", "osgb:50k", "png"},
};
*/

/****************************************************************************
*
*	 FUNCTION: ErrorOut
*
*	 PURPOSE: This function is used to get extended Internet error.
*
*	 COMMENTS:	Function returns TRUE on success and FALSE on failure.
*
****************************************************************************/

BOOL ErrorOut(DWORD dError, TCHAR * szCallFunc)
{
	fprintf(stderr, "%s error %d\n", szCallFunc, dError);
	if (dError == ERROR_INTERNET_EXTENDED_ERROR) {
		DWORD  dwIntError , dwLength = 0;
		TCHAR *szBuffer=NULL;
		InternetGetLastResponseInfo (&dwIntError, NULL, &dwLength);
		if (dwLength) {
			if ( !(szBuffer = (TCHAR *) LocalAlloc ( LPTR,	dwLength) ) ) {
				fprintf(stderr, "Unable to allocate memory to display Internet error code. Error code: %d\n", GetLastError());
				return FALSE;
			}
			if (!InternetGetLastResponseInfo (&dwIntError, (LPTSTR) szBuffer, &dwLength)) {
				fprintf(stderr, "Unable to get Intrnet error. Error code: %d\n", GetLastError());
				return FALSE;
			}
			fprintf(stderr, "%s\n", szBuffer);
			LocalFree (szBuffer);
		}
	}
	return TRUE;
}


#define	HTTP_GET_SIZE	65536
int wms_get_file_1(HINTERNET hConnect, LPCSTR pathname, LPCSTR tx_saveto)
{
HINTERNET hReq;
DWORD  dwSize, dwCode;
CHAR szData[HTTP_GET_SIZE+1];

	if ( !(hReq = HttpOpenRequest (hConnect, "GET", pathname, HTTP_VERSION, "", NULL, 0 ,0 ))) {
		ErrorOut (GetLastError(), "HttpOpenRequest");
		return FALSE;
	}


	if (!HttpSendRequest (hReq, NULL, 0, NULL, 0) ) {
		ErrorOut (GetLastError(), "HttpSend");
		return FALSE;
	}

	dwSize = sizeof (DWORD) ;
	if ( !HttpQueryInfo (hReq, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwCode, &dwSize, NULL)) {
		ErrorOut (GetLastError(), "HttpQueryInfo");
		return FALSE;
	}

	if ( dwCode == HTTP_STATUS_DENIED || dwCode == HTTP_STATUS_PROXY_AUTH_REQ) {
		// This is a secure page.
		fprintf(stderr, "This page is password protected.\n");
		return FALSE;
	}
	if ( dwCode == 404) {
		fprintf(stderr, "Page not found.\n");
		return FALSE;
	}

	FILE * fp = fopen(tx_saveto, "wb+");
	if (fp == NULL) {
		printf("Couldn't create %s\n", tx_saveto);
		return FALSE;
	}
	long file_len=0;
	while (!abortProgram) {
		if (!InternetReadFile (hReq, (LPVOID)szData, HTTP_GET_SIZE, &dwSize) ) {
			ErrorOut (GetLastError (), "InternetReadFile");
			file_len = -1;
			break;
		}
		if (dwSize == 0)
			break;

		if (fwrite(szData, sizeof(char), dwSize, fp) != dwSize) {
			printf("Error writing %d bytes to %s\n", dwSize, tx_saveto);
			file_len = -1;
			break;
		}
		file_len += dwSize;
//		printf("%d \r", file_len);
	}
	fclose(fp);

	if (!InternetCloseHandle (hReq) ) {
		ErrorOut (GetLastError (), "CloseHandle on hReq");
		file_len = -1;
	}
	if (file_len <= 0)
		return FALSE;

	// Validate PNG
	LPBYTE buffer = (LPBYTE)malloc(file_len+1);
	if (buffer == 0) {
		fprintf(stderr, "Couldn't allocate %d bytes to verify %s\n", file_len, tx_saveto);
		return FALSE;
	}
	memset(buffer, 0, file_len+1);
	fp = fopen(tx_saveto, "rb");
	if (fp == NULL) {
		fprintf(stderr, "Failed to reopen %s\n", tx_saveto);
		free(buffer);
		return FALSE;
	}
	if (fread(buffer, 1, file_len, fp) != file_len) {
		fprintf(stderr, "Error reading %s\n", tx_saveto);
		free(buffer);
		return FALSE;
	}
	fclose(fp);

	unsigned char pnghdr[] = {0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a};
	if (memcmp(buffer, pnghdr, sizeof(pnghdr)) != 0) {
		fprintf(stderr, "Error retrieving %s\n", tx_saveto);
		free(buffer);
		return FALSE;
	}

	free(buffer);
	return TRUE;
}

int get_file(HINTERNET hConnect, LPCSTR pathname, LPCSTR tx_saveto)
{
	// Attempt to get file and retry once if failed
	if (wms_get_file_1(hConnect, pathname, tx_saveto))
		return 1;
	return wms_get_file_1(hConnect, pathname, tx_saveto);
}

void wmsGetTile(HINTERNET hConnect, char * text, long northing, long easting, CTileDatabase * db, FILE * fp_log, BOOL bForce, int nMapScale)
{
	char strRequest[1024];
	if (text == NULL)
		return;

	int nPixels = db->m_nPixelsPerTile;
	int nScale = db->m_mapscale;
	int nMetres = db->m_nMetresPerTile;

	northing = northing/nMetres*nMetres;
	easting = easting/nMetres*nMetres;

	if (!bForce && db->TileLoaded(northing, easting))
		return;

	CString strServer;
	CString strLayer;
	CString strImage;

/*
	if (db->m_mapscale == 10000) {

		strServer = wmsMapLayers10K[MyMap.m_nCountry].strServer;
		strLayer  = wmsMapLayers10K[MyMap.m_nCountry].strLayer;
		strImage  = wmsMapLayers10K[MyMap.m_nCountry].strImage;

	} else if (db->m_mapscale == 25000) {

		strServer = wmsMapLayers25K[MyMap.m_nCountry].strServer;
		strLayer  = wmsMapLayers25K[MyMap.m_nCountry].strLayer;
		strImage  = wmsMapLayers25K[MyMap.m_nCountry].strImage;

	} else if (db->m_mapscale == 50000) {

		strServer = wmsMapLayers50K[MyMap.m_nCountry].strServer;
		strLayer  = wmsMapLayers50K[MyMap.m_nCountry].strLayer;
		strImage  = wmsMapLayers50K[MyMap.m_nCountry].strImage;
	
	} else {

		fprintf(stderr, "Error: Unknown Map Scale (%d)\n", db->m_mapscale);
		return;
	}
*/
	strServer = wmsMapLayers[MyMap.m_nWMSLayer].strServer;
	strLayer  = wmsMapLayers[MyMap.m_nWMSLayer].strLayer;
	strImage  = wmsMapLayers[MyMap.m_nWMSLayer].strImage;


	sprintf(strRequest,
			"%s"
			"?LAYERS=%s"
			"&SERVICE=WMS&VERSION=%s"
			"&REQUEST=GetMap"
			"&STYLES="
			"&FORMAT=image/%s"
			"&%s=EPSG:%s"
			"%s"
			"&BBOX=%d,%d,%d,%d"
			"&WIDTH=%d&HEIGHT=%d",
			strServer,
			strLayer,
			wmsMapRequests[MyMap.m_nCountry].strWMSVersion,
			strImage,
			wmsMapRequests[MyMap.m_nCountry].strRefSystem,wmsMapRequests[MyMap.m_nCountry].strProjection,
wmsMapRequests[MyMap.m_nCountry].strUrl,
			easting, northing, easting+nMetres, northing+nMetres,
			nPixels, nPixels);

//	fprintf(fp_log, "Tile: [] loading %s\n", text);
	if (get_file(hConnect, strRequest, "tile.tmp")) {
		FILE * fp = fopen("tile.tmp", "rb");
		if (fp != NULL) {
			fseek(fp, 0, SEEK_END);
			long file_length = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			unsigned char * filedata = (unsigned char*)malloc(file_length);
			fread(filedata, 1, file_length, fp);
			fclose(fp);
			db->WriteTile(northing, easting, filedata, file_length);
			free(filedata);
		} else {
			fprintf(stderr, "Error reading tile.tmp (%s)\n", text);
			return;
		}
	} else {
		fprintf(stderr, "Error retrieving tile (%s)\n", text);
		return;
	}
}


int wmsLoadTiles(CTileDatabase* g_db, long minx, long miny, long maxx, long maxy, BOOL bForce, int nMapScale)
{
HINTERNET hOpen, hConnect;
char strTile[256];

	hOpen = NULL;
	FILE * fp_log = fopen("log.txt", "a+");

CProgressWindow wndProgress;
wndProgress.Initialize();

int nMeters = MyMap.m_nMetresPerTile;

wndProgress.ResetProgressBar("Downloading:", (maxy-miny)/nMeters*(maxx-minx)/nMeters);

	// Grab in (8 x 8????) tiles.
	for (long y=miny; y<maxy; y+=nMeters) {
		for (long x=minx; x<maxx; x+=nMeters) {

			if (!wndProgress.ProgressBar()) return false;

			if (!bForce && g_db->TileLoaded(y, x)) {
		
				fprintf(fp_log, "Tile: [%05d,%05d] skipped - tile exists\n", x,y);
				continue;
			} else {
				
				// Only connect to WMS if required.
// SNL 11/06/2013 - hmmmm!
				g_db->InitDatabase(y,x,MyMap.m_nDatum);
				if (hOpen == NULL) {
					if ( !(hOpen = InternetOpen ( "Sample",  LOCAL_INTERNET_ACCESS , NULL, 0, 0) ) ) {
						ErrorOut ( GetLastError(), "InternetOpen");
						return 0;
					}

					if ( !(hConnect = InternetConnect ( hOpen, wmsMapRequests[MyMap.m_nWMS].strAttribution, wmsMapRequests[MyMap.m_nWMS].nPort, "",	"", INTERNET_SERVICE_HTTP, 0  , 0) ) ) {
						ErrorOut (GetLastError(), "InternetConnect");
						return 0;
					}
				}
				fprintf(fp_log, "Tile: [%05d,%05d] loading...\n", x,y);
				wmsGetTile(hConnect, strTile, y, x, g_db, fp_log, bForce, nMapScale);
			}

			if (abortProgram) {
				y=miny;
				x=maxx;
			}
		}
	}
	fclose(fp_log);

	if (hOpen != NULL) {
		if (!InternetCloseHandle (hConnect) ) {
			ErrorOut (GetLastError (), "CloseHandle on hConnect");
			return FALSE;
		}
		if (!InternetCloseHandle (hOpen) ) {
			ErrorOut (GetLastError (), "CloseHandle on hOpen");
			return FALSE;
		}
	}
	return 0;
}
