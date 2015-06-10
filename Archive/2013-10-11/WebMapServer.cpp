#include "stdafx.h"

#include "WebMapServer.h"

#include "ProgressWindow.h"

#include "CustomMap.h"

extern CCustomMap MyMap;

extern int abortProgram;

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
	unsigned char jpghdr[] = {0xFF, 0xD8, 0xFF, 0xE0, 0x00, 0x10, 0x4A, 0x46, 0x49, 0x46};
	unsigned char gifhdr[] = {0x47, 0x49, 0x46, 0x38, 0x39, 0x61};

	if ((memcmp(buffer, pnghdr, sizeof(pnghdr)) == 0)
	||  (memcmp(buffer, jpghdr, sizeof(jpghdr)) == 0)
	||  (memcmp(buffer, gifhdr, sizeof(gifhdr)) == 0)) {
		free(buffer);
		return TRUE;
	} else {
		fprintf(stderr, "Error retrieving %s\n", tx_saveto);
		free(buffer);
		return FALSE;
	}
}

int wmsGetFile(HINTERNET hConnect, LPCSTR pathname, LPCSTR tx_saveto)
{
	// Attempt to get file and retry once if failed
	if (wms_get_file_1(hConnect, pathname, tx_saveto))
		return 1;
	return wms_get_file_1(hConnect, pathname, tx_saveto);
}

void wmsGetTile(HINTERNET hConnect, char * text, long northing, long easting, CTileDatabase * db, FILE * fp_log, BOOL bForce, int nMapScale)
{
	if (text == NULL) return;

	int nPixels = db->m_nPixelsPerTile;
	int nDetail = db->m_mapscale;
	int nMetres = db->m_nMetresPerTile;

	northing = northing/nMetres*nMetres;
	easting = easting/nMetres*nMetres;

	if (!bForce && db->TileLoaded(northing, easting)) return;

	CString strRequest;

	strRequest.Empty();

	if (MyMap.GetServerType() == WMS)					strRequest = MyMap.wmsGetRequest(easting, northing);
	else if (MyMap.GetServerType() == GetMapping)		strRequest = MyMap.getmappingGetRequest(easting, northing);
	else if (MyMap.GetServerType() == OpenStreetMap)	strRequest = MyMap.osmGetRequest(easting, northing);

	if (wmsGetFile(hConnect, strRequest, "tile.tmp")) {
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

int nMeters = MyMap.GetMetresPerTile();

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
