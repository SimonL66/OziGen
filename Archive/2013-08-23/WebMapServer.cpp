#include "stdafx.h"

#include "WebMapServer.h"

#include "ProgressWindow.h"

#include "CustomMap.h"
extern CCustomMap MyMap;

extern int abortProgram;


struct wmsMapRequests {

	int		nCountry;
	char*	strAttribution;

	char*	strWMSVersion;

	char*	strImage;
	char*	strProjection;

} wmsMapRequests[] = {

	{COUNTRY_AT,	"", "", "", ""},
	{COUNTRY_NGI,	"ngi.be",								"1.3.0", "png", "CRS=EPSG:3812"},
	{COUNTRY_OSGB,	"osopenspacepro.ordnancesurvey.co.uk",	"1.1.1", "png", "SRS=EPSG:27700"},
	{COUNTRY_OSI,	"", "", "", ""},
	{COUNTRY_IGN,	"", "", "", ""},
	{COUNTRY_DE,	"www.wms.nrw.de",						"1.1.1", "png", "SRS=EPSG:31467"},
	{COUNTRY_IT,	"", "", "", ""},
	{COUNTRY_LUX,	"", "", "", ""},
	{COUNTRY_NL,	"", "", "", ""},
	{COUNTRY_OSNI,	"", "", "", ""},
	{COUNTRY_PT,	"", "", "", ""},
	{COUNTRY_ES,	"www.idee.es",							"1.1.1", "png", "SRS=EPSG:23030"},
	{COUNTRY_CH,	"", "", "", ""},
	{COUNTRY_PL,	"", "", "", ""},
	{COUNTRY_CZ,	"", "", "", ""},
	{COUNTRY_SLO,	"", "", "", ""},
};

struct wmsMapLayers10K {

	int		nCountry;
	char*	strServer;
	char*	strLayer;

} wmsMapLayers10K[] = {

	{COUNTRY_AT,	"", ""},
	{COUNTRY_NGI,	"testbed/wms/top10r_l08_fr", "0"},
	{COUNTRY_OSGB,	"", ""},
	{COUNTRY_OSI,	"", ""},
	{COUNTRY_IGN,	"", ""},
	{COUNTRY_DE,	"geobasis/adv_dtk10", "DTK10"},
	{COUNTRY_IT,	"", ""},
	{COUNTRY_LUX,	"", ""},
	{COUNTRY_NL,	"", ""},
	{COUNTRY_OSNI,	"", ""},
	{COUNTRY_PT,	"", ""},
	{COUNTRY_ES,	"", ""},
	{COUNTRY_CH,	"", ""},
	{COUNTRY_PL,	"", ""},
	{COUNTRY_CZ,	"", ""},
	{COUNTRY_SLO,	"", ""},
};

struct wmsMapLayers25K {

	int		nCountry;
	char*	strServer;
	char*	strLayer;

} wmsMapLayers25K[] = {

	{COUNTRY_AT,	"", ""},
	{COUNTRY_NGI,	"", ""},
	{COUNTRY_OSGB,	"", ""},
	{COUNTRY_OSI,	"", ""},
	{COUNTRY_IGN,	"", ""},
	{COUNTRY_DE,	"geobasis/adv_dtk25", "DTK25vi"},
	{COUNTRY_IT,	"", ""},
	{COUNTRY_LUX,	"", ""},
	{COUNTRY_NL,	"", ""},
	{COUNTRY_OSNI,	"", ""},
	{COUNTRY_PT,	"", ""},
	{COUNTRY_ES,	"", ""},
	{COUNTRY_CH,	"", ""},
	{COUNTRY_PL,	"", ""},
	{COUNTRY_CZ,	"", ""},
	{COUNTRY_SLO,	"", ""},
};

struct wmsMapLayers50K {

	int		nCountry;
	char*	strServer;
	char*	strLayer;

} wmsMapLayers50K[] = {

	{COUNTRY_AT,	"", ""},
	{COUNTRY_NGI,	"testbed/wms/top50r_l08_fr", "0"},
	{COUNTRY_OSGB,	"", ""},
	{COUNTRY_OSI,	"", ""},
	{COUNTRY_IGN,	"", ""},
	{COUNTRY_DE,	"wms/dlm50", "dlm_50"},
	{COUNTRY_IT,	"", ""},
	{COUNTRY_LUX,	"", ""},
	{COUNTRY_NL,	"", ""},
	{COUNTRY_OSNI,	"", ""},
	{COUNTRY_PT,	"", ""},
	{COUNTRY_ES,	"wms/MTN-Raster/MTN-Raster", "mtn_rasterizado"},
	{COUNTRY_CH,	"", ""},
	{COUNTRY_PL,	"", ""},
	{COUNTRY_CZ,	"", ""},
	{COUNTRY_SLO,	"", ""},
};
//		sprintf(strRequest, "testbed/wms/top50r_l08_fr?LAYERS=0&CRS=EPSG%%3A3812&VERSION=1.3.0&FORMAT=image%%2Fpng&EXCEPTIONS=INIMAGE&SERVICE=WMS&REQUEST=GetMap&STYLES=&BBOX=%d,%d,%d,%d&WIDTH=%d&HEIGHT=%d", easting, northing, easting + nMeters, northing + nMeters, nPixels, nPixels);

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

//	long nPixels = GetPixelsPerTile(nMapScale);

	int nPixels = db->m_nPixelsPerTile;
	int nScale = db->m_mapscale;
	int nMetres = db->m_nMetresPerTile;

	northing = northing/nMetres*nMetres;
	easting = easting/nMetres*nMetres;

	if (!bForce && db->TileLoaded(northing, easting))
		return;

	/*
	if (db->m_mapscale == 50000) {

		// http://www.idee.es/wms/MTN-Raster/MTN-Raster?LAYERS=mtn_rasterizado&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&EXCEPTIONS=application/vnd.ogc.se_inimage&FORMAT=image/jpeg&SRS=EPSG:23030&BBOX=168000,4736000,169000,4737000&WIDTH=400&HEIGHT=400
		//	EPSG:23030 ED50 / UTM zone 30N;	sprintf(strRequest,"wms/MTN-Raster/MTN-Raster?LAYERS=mtn_rasterizado&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&EXCEPTIONS=application/vnd.ogc.se_inimage&FORMAT=image/png&SRS=EPSG:23030&BBOX=%d,%d,%d,%d&WIDTH=400&HEIGHT=400",easting, northing, easting+1000, northing+1000);

		sprintf(strRequest,
				"%s"
				"?LAYERS=%s"
				"&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap"
				"&FORMAT=image/%s"
				"&SRS=EPSG:%s"
				"&BBOX=%d,%d,%d,%d"
				"&WIDTH=400&HEIGHT=400",
				wmsMapLayers50K[MyMap.m_nCountry].strServer,
				wmsMapLayers50K[MyMap.m_nCountry].strLayer,
				wmsMapRequests[MyMap.m_nCountry].strImage,
				wmsMapRequests[MyMap.m_nCountry].strProjection,
				easting, northing, easting+1000, northing+1000);
	}
*/
	CString strServer;
	CString strLayer;

	if (db->m_mapscale == 10000) {

		strServer = wmsMapLayers10K[MyMap.m_nCountry].strServer;
		strLayer  = wmsMapLayers10K[MyMap.m_nCountry].strLayer;

	} else if (db->m_mapscale == 25000) {

		strServer = wmsMapLayers25K[MyMap.m_nCountry].strServer;
		strLayer  = wmsMapLayers25K[MyMap.m_nCountry].strLayer;

	} else if (db->m_mapscale == 50000) {

		strServer = wmsMapLayers50K[MyMap.m_nCountry].strServer;
		strLayer  = wmsMapLayers50K[MyMap.m_nCountry].strLayer;
	
	} else {

		fprintf(stderr, "Error: Unknown Map Scale (%d)\n", db->m_mapscale);
		return;
	}

	sprintf(strRequest,
			"%s"
			"?LAYERS=%s"
			"&SERVICE=WMS&VERSION=%s"
			"&REQUEST=GetMap"
			"&FORMAT=image/%s"
			"&%s"
			"&BBOX=%d,%d,%d,%d"
			"&WIDTH=%d&HEIGHT=%d",
			strServer,
			strLayer,
			wmsMapRequests[MyMap.m_nCountry].strWMSVersion,
			wmsMapRequests[MyMap.m_nCountry].strImage,
			wmsMapRequests[MyMap.m_nCountry].strProjection,
			easting, northing, easting+nMetres, northing+nMetres,
			nPixels, nPixels);

	fprintf(fp_log, "Tile: loading %s\n", text);
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

					if ( !(hConnect = InternetConnect ( hOpen, wmsMapRequests[MyMap.m_nCountry].strAttribution, INTERNET_INVALID_PORT_NUMBER, "",	"", INTERNET_SERVICE_HTTP, 0  , 0) ) ) {
						ErrorOut (GetLastError(), "InternetConnect");
						return 0;
					}
				}
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

/*
void Kompass_GetTile(HINTERNET hConnect, char * text, long northing, long easting, CTileDatabase * db, FILE * fp_log, BOOL bForce, int nMapScale)
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

	if (db->m_mapscale == 10000) {

			//http://www.wms.nrw.de/geobasis/adv_dtk10?
			//REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS
			//&FORMAT=image%2Fpng
			//&SRS=EPSG%3A31466
			//&LAYERS=DTK10&
			//STYLES=&BBOX=2516500,5746200,2516700,5746400
			//&WIDTH=200&HEIGHT=200
		
		sprintf(strRequest,
				"geobasis/adv_dtk10?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&FORMAT=image%%2Fpng&SRS=EPSG%%3A31467&LAYERS=DTK10&STYLES=&BBOX=%d,%d,%d,%d&WIDTH=%d&HEIGHT=%d",
				easting, northing, easting+nMetres, northing+nMetres, 
				nPixels, nPixels);

	} else if (db->m_mapscale == 25000)

		sprintf(strRequest,
				"geobasis/adv_dtk25?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&FORMAT=image%%2Fpng&SRS=EPSG%%3A31467&LAYERS=DTK25vi&STYLES=&BBOX=%d,%d,%d,%d&WIDTH=%d&HEIGHT=%d",
				easting, northing, easting+nMetres, northing+nMetres,
				nPixels, nPixels);

	else if (db->m_mapscale == 50000) {
	
			//http://www.wms.nrw.de/wms/dlm50?
			//LAYERS=dlm_50
			//&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=
			//&FORMAT=image%2Fpng
			//&SRS=EPSG%3A31467
			//&BBOX=3648944,5480136,3649712,5480904
			//&WIDTH=400&HEIGHT=400

		sprintf(strRequest,
				"%s"
				"?LAYERS=%s"
				"&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap"
				"&FORMAT=image/%s"
				"&SRS=EPSG:%s"
				"&BBOX=%d,%d,%d,%d"
				"&WIDTH=%d&HEIGHT=%d",
				wmsMapLayers50K[MyMap.m_nCountry].strServer,
				wmsMapLayers50K[MyMap.m_nCountry].strLayer,
				wmsMapRequests[MyMap.m_nCountry].strImage,
				wmsMapRequests[MyMap.m_nCountry].strProjection,
				easting, northing, easting+nMetres, northing+nMetres,
				nPixels, nPixels);

	} else {
		fprintf(stderr, "Invalid German Scale Specified\n");
		return;
	}

	fprintf(fp_log, "Tile: loading %s\n", text);
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
	}
}
*/

/*
int Kompass_LoadTiles(CTileDatabase* g_db, long minx, long miny, long maxx, long maxy, BOOL bForce, int nMapScale)
{
HINTERNET hOpen, hConnect;
char strTile[256];

	hOpen = NULL;
	FILE * fp_log = fopen("log.txt", "a+");
//	fprintf(fp_log, "\n\nGetOSPro Eastings: %04d %04d Northings: %04d %04d\n", minx, maxx, miny, maxy);

CProgressWindow wndProgress;
wndProgress.Initialize();

int nMeters = g_db->m_nMetresPerTile;

wndProgress.ResetProgressBar("Downloading:", (maxy-miny)/nMeters*(maxx-minx)/nMeters);
	// grab in (8 x 8????) tiles
	for (long y=miny; y<maxy; y+=nMeters) {
		for (long x=minx; x<maxx; x+=nMeters) {
//			easting_northing_to_OSref3(x, y, strTile);

//printf("Tile: %s %04d %04d - (%d of %d)\n", strTile, x/100, y/100, (y-miny)/500*(maxx-minx)/500+(x-minx)/500+1, (maxy-miny)/500*(maxx-minx)/500);
			if (!wndProgress.ProgressBar()) return false;

//			easting_northing_to_OSref5(x, y, strTile);
			if (!bForce && g_db->TileLoaded(y, x)) {
				fprintf(fp_log, "Tile: %s skipped - tile exists\n", strTile);
				continue;
			} else {
				// Only connect to OSPro if required
// SNL 11/06/2013 - hmmmm!
g_db->InitDatabase(y,x,MAP_DATUM_DE);
				if (hOpen == NULL) {
					if ( !(hOpen = InternetOpen ( "Sample",  LOCAL_INTERNET_ACCESS , NULL, 0, 0) ) ) {
						ErrorOut ( GetLastError(), "InternetOpen");
						return 0;
					}

					if ( !(hConnect = InternetConnect ( hOpen, "www.wms.nrw.de", INTERNET_INVALID_PORT_NUMBER, "",	"", INTERNET_SERVICE_HTTP, 0  , 0) ) ) {
						ErrorOut (GetLastError(), "InternetConnect");
						return 0;
					}
				}
				Kompass_GetTile(hConnect, strTile, y, x, g_db, fp_log, bForce, nMapScale);

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
*/