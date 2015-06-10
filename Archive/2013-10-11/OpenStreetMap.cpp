#include "stdafx.h"

#include <direct.h>

#include "TileDatabase.h"
#include "WebMapServer.h"

#include "..\MapLib\CoordConv.h"
#include "..\MapLib\map_conv.h"
#include "..\MapLib\png.h"
#include "..\MapLib\palette.h"

#include "WebMapServer.h"

#include "ProgressWindow.h"

#include "CustomMap.h"
extern CCustomMap MyMap;

extern int abortProgram;

#define printf visualprintf




#define	HTTP_GET_SIZE	4096
int OSM_get_file(HINTERNET hConnect, LPCSTR pathname, long northing, long easting, CTileDatabase * db)
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

	LPCSTR	tx_saveto = "tile.tmp";
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
			printf("Error writing %d bytes to %s (%d %d)\n", tx_saveto, easting, northing);
			file_len = -1;
			break;
		}
		file_len += dwSize;
	//	printf("%d\r", file_len);
	}
	fclose(fp);

	if (!InternetCloseHandle (hReq) ) {
		ErrorOut (GetLastError (), "CloseHandle on hReq");
		file_len = -1;
	}
	if (file_len <= 0)
		return FALSE;

	LPBYTE buffer = (LPBYTE)malloc(file_len+1);
	if (buffer == 0) {
		fprintf(stderr, "Couldn't allocate %d bytes to verify file (%d %d)\n", file_len, easting, northing);
		return FALSE;
	}
	fp = fopen(tx_saveto, "rb");
	if (fp == NULL) {
		fprintf(stderr, "Failed to reopen %s\n", tx_saveto);
		free(buffer);
		return FALSE;
	}
	long file_len2 = fread(buffer, 1, file_len, fp);
	fclose(fp);
	buffer[file_len] = 0;
	unsigned char jpghdr[] = {0xFF, 0xD8, 0xFF, 0xE0, 0x00, 0x10, 0x4A, 0x46, 0x49, 0x46};
	unsigned char gifhdr[] = {0x47, 0x49, 0x46, 0x38, 0x39, 0x61};
	unsigned char pnghdr[] = {0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a}; // Note: only check first four bytes for png!
	if (memcmp(buffer, jpghdr, sizeof(jpghdr)) == 0) {
		// file is a JPEG
		db->WriteTile(northing, easting, buffer, file_len);
	} else if (memcmp(buffer, gifhdr, sizeof(gifhdr)) == 0) {
		// file is really a GIF
		db->WriteTile(northing, easting, buffer, file_len);
		remove(tx_saveto);
	} else if (memcmp(buffer, pnghdr, sizeof(pnghdr)) == 0) {
		// file is a PNG
		db->WriteTile(northing, easting, buffer, file_len);
	} else {
		fprintf(stderr, "Error reading %s. Retrying\n", tx_saveto);
		free(buffer);
//		return GetAerial_get_file(hConnect, pathname, northing, easting, db);
		return false;
	}
	free(buffer);
	return TRUE;
}




int OSM_LoadTiles(CTileDatabase* g_db, long minx, long miny, long maxx, long maxy)
{
HINTERNET hOpen, hConnect;

	if ( !(hOpen = InternetOpen ( "HeadDump",  LOCAL_INTERNET_ACCESS , NULL, 0, 0) ) ) {
		ErrorOut ( GetLastError(), "InternetOpen");
		return 0;
	}

//	if ( !(hConnect = InternetConnect ( hOpen, "ec2.cdn.ecmaps.de", INTERNET_INVALID_PORT_NUMBER, "",	"", INTERNET_SERVICE_HTTP, 0  , 0) ) ) {
//	if ( !(hConnect = InternetConnect ( hOpen, "tile.openstreetmap.org", INTERNET_INVALID_PORT_NUMBER, "",	"", INTERNET_SERVICE_HTTP, 0  , 0) ) ) {
	if ( !(hConnect = InternetConnect ( hOpen, MyMap.wmsGetAttribution(), MyMap.wmsGetPort(), "",	"", INTERNET_SERVICE_HTTP, 0  , 0) ) ) {
		ErrorOut (GetLastError(), "InternetConnect");
		return 0;
	}
	FILE * fp_log = fopen("log.txt", "a+");
	fprintf(fp_log, "\n\nGetOSM Eastings: %04d %04d Northings: %04d %04d\n", minx, maxx, miny, maxy);

CProgressWindow wndProgress;
wndProgress.Initialize();

//int nMeters = MyMap.GetMetresPerTile();

wndProgress.ResetProgressBar("Downloading:", (maxy-miny)*(maxx-minx));


	for (int x=minx; x<maxx; x++) {
		for (int y=maxy-1; y>=miny; y--) {


			if (!wndProgress.ProgressBar()) return false;

			if (g_db->TileLoaded(y, x)) {
				fprintf(fp_log, "Tile: %d,%d skipped - file exists\n", x, y);
				continue;
			}

//			int revision = 0;

// http://tile.openstreetmap.org/18/130584/83719.png
 //http://tile.openstreetmap.org/15/16311/10481.png"
// http://ec2.cdn.ecmaps.de/WmsGateway.ashx.jpg?Experience=kompass&MapStyle=KOMPASS%20Touristik&TileX=17018&TileY=10975&ZoomLevel=15
//			sprintf(tstring, "/15/%d/%d.png", x, y);
//			sprintf(tstring, "WmsGateway.ashx.jpg?Experience=kompass&MapStyle=KOMPASS%%20Touristik&TileX=%d&TileY=%d&ZoomLevel=15", x, y);
		CString strRequest;
		
		if		(MyMap.GetServerType() == OpenStreetMap)	strRequest = MyMap.osmGetRequest(x, y);
		else if (MyMap.GetServerType() == Kompass)			strRequest = MyMap.kompassGetRequest(x, y);
		else if (MyMap.GetServerType() == Bing)				strRequest = MyMap.bingGetRequest(x, y);
		else if (MyMap.GetServerType() == SatMap)			strRequest = MyMap.satmapGetRequest(x, y);
	
		if (OSM_get_file(hConnect, strRequest, y, x, g_db)) {
				fprintf(fp_log, "Tile: %s Retrieved\n", strRequest);
			} else {
				fprintf(fp_log, "Tile: %s Retrieval error\n", strRequest);
			}

		}
	}
	fclose(fp_log);

	if (!InternetCloseHandle (hConnect) ) {
		ErrorOut (GetLastError (), "CloseHandle on hConnect");
		return FALSE;
	}
	if (!InternetCloseHandle (hOpen) ) {
		ErrorOut (GetLastError (), "CloseHandle on hOpen");
		return FALSE;
	}
	return 0;
}



/*
int OSM_LoadTiles(CTileDatabase* g_db, long minx, long miny, long maxx, long maxy)
{
HINTERNET hOpen, hConnect;
char strTile[256];

	hOpen = NULL;
	FILE * fp_log = fopen("log.txt", "a+");

CProgressWindow wndProgress;
wndProgress.Initialize();

wndProgress.ResetProgressBar("Downloading:", (maxy-miny)*(maxx-minx));

	// Grab in (8 x 8????) tiles.
	for (long y=miny; y<maxy; y++) {
		for (long x=minx; x<maxx; x++) {

			if (!wndProgress.ProgressBar()) return false;

			if (!bForce && g_db->TileLoaded(y, x)) {
		
				fprintf(fp_log, "Tile: [%05d,%05d] skipped - tile exists\n", x,y);
				continue;
			} else {
				
				// Only connect to WMS if required.
// SNL 11/06/2013 - hmmmm!
CString str1 = MyMap.wmsGetAttribution();
int port = MyMap.wmsGetPort();
				g_db->InitDatabase(y,x,MyMap.GetDatum());
				if (hOpen == NULL) {
					if ( !(hOpen = InternetOpen ( "Sample",  LOCAL_INTERNET_ACCESS , NULL, 0, 0) ) ) {
						ErrorOut ( GetLastError(), "InternetOpen");
						return 0;
					}

					if ( !(hConnect = InternetConnect ( hOpen, MyMap.wmsGetAttribution(), MyMap.wmsGetPort(), "",	"", INTERNET_SERVICE_HTTP, 0  , 0) ) ) {
						ErrorOut (GetLastError(), "InternetConnect");
						return 0;
					}
				}
				fprintf(fp_log, "Tile: [%05d,%05d] loading...\n", x,y);
				wmsGetTile(hConnect, strTile, y, x, g_db, fp_log, false, 1);
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


void Get_OpenStreetMap(CTileDatabase* g_db, long minx, long miny, long maxx, long maxy, BOOL bLoadTiles)
{
	if (minx > maxx) {
		long t = minx;
		minx = maxx;
		maxx = t;
	}
	if (miny > maxy) {
		long t = miny;
		miny = maxy;
		maxy = t;
	}

/*
	minx = minx/500*500;
	miny = miny/500*500;
	maxx = (maxx+499)/500*500;
	maxy = (maxy+499)/500*500;
*/

//	_mkdir("jpg");

//	if (bLoadTiles)
		OSM_LoadTiles(g_db, minx, miny, maxx, maxy);

//	if (abortProgram) return;

/*
	colour_convert = (unsigned char*)malloc(256*256*256);
	memset(colour_convert, COLOUR_UNKNOWN, 256*256*256);
	Create_PNG_JPR_from_JPEG(g_db, "map", minx, maxx, miny, maxy);
	free(colour_convert);
*/
	return;
}
/*
void Dump_OpenStreetMap(CTileDatabase* g_db, LPCSTR mapname, long minx, long miny, long maxx, long maxy)
{
	colour_convert = (unsigned char*)malloc(256*256*256);
	memset(colour_convert, COLOUR_UNKNOWN, 256*256*256);
	Create_PNG_JPR_from_JPEG(g_db, mapname, minx, maxx, miny, maxy);
	free(colour_convert);
}
*/
