#include "stdafx.h"

#include <direct.h>

#include <Wininet.h>

#include "OziGen.h"
#include "OziGenDlg.h"
#include "ProgressWindow.h"
#include "func.h"

#include "..\MapLib\map_conv.h"
#include "..\MapLib\png.h"
#include "..\MapLib\palette.h"
#include "..\MapLib\CoordConv.h"

#include "..\MapLib\TileDatabase.h"

#include "GetOSPro.h"


#define printf visualprintf

extern CString g_strMapDatabaseFolder;
extern CString g_strMapScale;

unsigned char uncompressed[1500*1500];
long	uncompressed_length;


int abortProgram = FALSE;

BOOL CtrlCHandler(DWORD dwCtrlType)
{
	abortProgram = TRUE;
	return TRUE;
}

#define	TILE_WIDTH	200


int read_png(long northing, long easting, CTileDatabase * db, unsigned char * bm_ptr, long bitmap_width)
{
	// load file
	long tileSize;
	LPBYTE data = db->ReadTile(northing, easting, &tileSize);
	if (data == NULL) {
		return 1;		// couldn't load file
	}
	char strTilename[256];
	easting_northing_to_OSref3(easting, northing, strTilename);
//	printf("%s\r", strTilename);

	FILE * fpin = fopen("tile.tmp", "wb+");
	if (fpin == NULL) {
		printf("Couldn't create tile.tmp\n");
		return 0;
	}
	fwrite(data, 1, tileSize, fpin);
	free(data);
	fseek(fpin, 0, SEEK_SET);


	png_structp read_ptr;
	png_infop read_info_ptr, end_info_ptr;
	png_uint_32 width, height;
	int bit_depth, color_type;
	
	png_debug(0, "Allocating read and write structures\n");
	read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, (png_voidp)NULL, (png_error_ptr)NULL, (png_error_ptr)NULL);
	png_debug(0, "Allocating read_info, write_info and end_info structures\n");
	read_info_ptr = png_create_info_struct(read_ptr);
	end_info_ptr = png_create_info_struct(read_ptr);

	png_debug(0, "Initializing input and output streams\n");
	png_init_io(read_ptr, fpin);

	  png_set_read_status_fn(read_ptr, NULL);

	png_debug(0, "Reading info struct\n");
	png_read_info(read_ptr, read_info_ptr);


	BYTE	palette_convert[256];
	memset(palette_convert, 0xfe, sizeof(palette_convert));
	png_debug(0, "Transferring info struct\n");
	{
		int interlace_type, compression_type, filter_type;

		if (png_get_IHDR(read_ptr, read_info_ptr, &width, &height, &bit_depth,
			&color_type, &interlace_type, &compression_type, &filter_type))
		{
			if (width != TILE_WIDTH || height != TILE_WIDTH) {
				printf("Bad size (%s) (%d, %d)\n", strTilename, width, height);
				db->DeleteTile(northing, easting);
				return 1;
			}
//			png_set_IHDR(write_ptr, write_info_ptr, width, height, bit_depth,
		}
		png_colorp png_palette= NULL;
		int ncolours = 0;
		png_get_PLTE(read_ptr, read_info_ptr, &png_palette, &ncolours);
		for (int i = 0; i<ncolours; i++)
			palette_convert[i] = RGB_to_tracklog_palette(png_palette[i].blue, png_palette[i].green, png_palette[i].red);
	}

	long source_width = png_get_rowbytes(read_ptr, read_info_ptr);
	png_bytep row_buf0 = (png_bytep)png_malloc(read_ptr, source_width);

	for (long y = 0; y < (long)height; y++) {
		png_read_rows(read_ptr, (png_bytepp)&row_buf0, (png_bytepp)NULL, 1);
		for (long x=0; x<(long)width; x++) {
			bm_ptr[y*bitmap_width + x] = palette_convert[row_buf0[x]];
		}
	}
	png_free(read_ptr, row_buf0);

	png_destroy_read_struct(&read_ptr, &read_info_ptr, &end_info_ptr);
	fclose(fpin);

	return 0;
}

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
int get_file_1(HINTERNET hConnect, LPCSTR pathname, LPCSTR tx_saveto)
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
	if (get_file_1(hConnect, pathname, tx_saveto))
		return 1;
	return get_file_1(hConnect, pathname, tx_saveto);
}


void GetTile(HINTERNET hConnect, char * text, long northing, long easting, CTileDatabase * db, FILE * fp_log, BOOL bForce)
{
char strRequest[1024];
	if (text == NULL)
		return;

	northing = northing/500*500;
	easting = easting/500*500;

	sprintf(strRequest, "osmapapi/ts?FORMAT=image%%2Fpng&KEY=DEMOAPI&URL=http%%3A%%2F%%2Fosopenspacewiki.ordnancesurvey.co.uk%%2Fwiki%%2Fextensions%%2Fhtmlets%%2FOSProCS.html&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&EXCEPTIONS=application%%2Fvnd.ogc.se_inimage&LAYERS=2.5&PRODUCT=25K&SRS=EPSG%%3A27700&"
						"BBOX=%d,%d,%d,%d&WIDTH=200&HEIGHT=200", easting, northing, easting+500, northing-500);

	if (!bForce && db->TileLoaded(northing, easting))
		return;

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


// http://osopenspacepro.ordnancesurvey.co.uk/osmapapi/ts?FORMAT=image%2Fpng&KEY=DEMOAPI&URL=http%3A%2F%2Fosopenspacewiki.ordnancesurvey.co.uk%2Fwiki%2Fextensions%2Fhtmlets%2FOSProCS.html&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&EXCEPTIONS=application%2Fvnd.ogc.se_inimage&LAYERS=2.5&PRODUCT=25K&SRS=EPSG%3A27700&BBOX=258000,667500,258500,668000&WIDTH=200&HEIGHT=200

int LoadTiles(CTileDatabase* g_db, long minx, long miny, long maxx, long maxy, BOOL bForce)
{
HINTERNET hOpen, hConnect;
char strTile[256];

	hOpen = NULL;
	FILE * fp_log = fopen("log.txt", "a+");
//	fprintf(fp_log, "\n\nGetOSPro Eastings: %04d %04d Northings: %04d %04d\n", minx, maxx, miny, maxy);

CProgressWindow wndProgress;
wndProgress.Initialize();
DoEvents();

wndProgress.ResetProgressBar("Downloading:", (maxy-miny)/500*(maxx-minx)/500);
DoEvents();

	// grab in 8 x 8 tiles
	for (long y=miny; y<maxy; y+=500) {
		for (long x=minx; x<maxx; x+=500) {
			easting_northing_to_OSref3(x, y, strTile);

//printf("Tile: %s %04d %04d - (%d of %d)\n", strTile, x/100, y/100, (y-miny)/500*(maxx-minx)/500+(x-minx)/500+1, (maxy-miny)/500*(maxx-minx)/500);
wndProgress.ProgressBar();
DoEvents();
if (wndProgress.m_Cancelled) return false;

			easting_northing_to_OSref3(x, y, strTile);
			if (!bForce && g_db->TileLoaded(y, x)) {
				fprintf(fp_log, "Tile: %s skipped - tile exists\n", strTile);
				continue;
			} else {
				// Only connect to OSPro if required
				if (hOpen == NULL) {
					if ( !(hOpen = InternetOpen ( "Sample",  LOCAL_INTERNET_ACCESS , NULL, 0, 0) ) ) {
						ErrorOut ( GetLastError(), "InternetOpen");
						return 0;
					}

					if ( !(hConnect = InternetConnect ( hOpen, "osopenspacepro.ordnancesurvey.co.uk", INTERNET_INVALID_PORT_NUMBER, "",	"", INTERNET_SERVICE_HTTP, 0  , 0) ) ) {
						ErrorOut (GetLastError(), "InternetConnect");
						return 0;
					}
				}
				GetTile(hConnect, strTile, y, x, /*&*/g_db, fp_log, bForce);

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


#define N_OS_ZONES	55

bool IsInOSGBDatabase(long northing, long easting)
{

	struct map {
		char *	strOSZone;
	} zones[] = {

		"HP", "HT", "HU", "HW", "HX", "HY", "HZ", "NA",
		"NB", "NC", "ND", "NF", "NG", "NH", "NJ", "NK", "NL", "NM", "NN", "NO", "NR", "NS", "NT", "NU", "NW", "NX", "NY", "NZ",
		"OV", 
		"SD", "SE", "SH", "SJ", "SK", "SM", "SN", "SO", "SP", "SR", "SS", "ST", "SU", "SV", "SW", "SX", "SY", "SZ",
		"TA", "TF", "TG", "TL", "TM", "TQ", "TR", "TV"
	};

	char str[256];
	easting_northing_to_OS_Zone(easting, northing, str);

	CString strZone = str;

	for (int i=0; i < N_OS_ZONES; i++) {
		if (strZone == zones[i].strOSZone) {
			return true;
		}
	}

	return false;
}