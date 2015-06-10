#include "stdafx.h"

#include <direct.h>

#include <Wininet.h>

#include "WebMapServer.h"

#include "OziGen.h"
#include "OziGenDlg.h"
#include "ProgressWindow.h"
#include "func.h"

#include "..\MapLib\map_conv.h"
#include "..\MapLib\png.h"
#include "..\MapLib\palette.h"
#include "..\MapLib\CoordConv.h"

#include "Database.h"
#include "TileDatabase.h"

#include "OSPro.h"

#define printf visualprintf

unsigned char uncompressed[1500*1500];
long	uncompressed_length;


int abortProgram = FALSE;

BOOL CtrlCHandler(DWORD dwCtrlType)
{
	abortProgram = TRUE;
	return TRUE;
}

// ToDo: To Make a database variable!
#define	TILE_WIDTH	200


int read_png(long northing, long easting, CTileDatabase * db, unsigned char * bm_ptr, long bitmap_width)
{
	// load file
	long tileSize;
	LPBYTE data = db->ReadTile(northing, easting, &tileSize);
	if (data == NULL) {
		// white tile = palette entry 255
		for (long ty=0; ty<TILE_WIDTH; ty++)
			memset(bm_ptr+ty*bitmap_width, 255, TILE_WIDTH);
		return 1;		// couldn't load file
	}
	char strTilename[256];
	easting_northing_to_OSref3(easting, northing, strTilename);
//	printf("%s\r", strTilename);

	FILE * fpin = fopen("tile.tmp", "wb+");
	if (fpin == NULL) {
		printf("Couldn't create tile.tmp\n");
		return 1;
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

			// ToAdd: Error checking for different Tile Sizes
/*			
			if (width != TILE_WIDTH || height != TILE_WIDTH) {
				printf("Bad size (%s) (%d, %d)\n", strTilename, width, height);
				db->DeleteTile(northing, easting);
				return 1;
			}
*/
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

#define	TILE_WIDTH_10K	250


int read_png_10K(long northing, long easting, CTileDatabase * db, unsigned char * bm_ptr, long bitmap_width)
{
	// load file
	long tileSize;
	LPBYTE data = db->ReadTile(northing, easting, &tileSize);
	if (data == NULL) {
		// white tile = palette entry 255
		for (long ty=0; ty<TILE_WIDTH_10K; ty++)
			memset(bm_ptr+ty*bitmap_width, 255, TILE_WIDTH_10K);
		return 1;		// couldn't load file
	}
	char strTilename[256];
	easting_northing_to_OSref3(easting, northing, strTilename);
//	printf("%s\r", strTilename);

	FILE * fpin = fopen("tile.tmp", "wb+");
	if (fpin == NULL) {
		printf("Couldn't create tile.tmp\n");
		return 1;
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
/*
			if (width != TILE_WIDTH_10K || height != TILE_WIDTH_10K) {
				printf("Bad size (%s) (%d, %d)\n", strTilename, width, height);
				db->DeleteTile(northing, easting);
				return 1;

			}
*/
		}	
	}

	long source_width = png_get_rowbytes(read_ptr, read_info_ptr);
	png_bytep row_buf0 = (png_bytep)png_malloc(read_ptr, source_width);

	if (read_info_ptr->pixel_depth == 24) {
		// Slow Method: Convert 24 bit png to 8 bit png. 
		for (long y = 0; y < (long)height; y++) {
			png_read_rows(read_ptr, (png_bytepp)&row_buf0, (png_bytepp)NULL, 1);
			for (long x=0; x<(long)width; x++) {
				bm_ptr[y*bitmap_width + x] = RGB_to_tracklog_palette(row_buf0[x*3+2], row_buf0[x*3+1], row_buf0[x*3]);
			}
		}
	} else if (read_info_ptr->pixel_depth == 32) {
		// Slow Method: Convert 32 bit png to 8 bit png. 
		for (long y = 0; y < (long)height; y++) {
			png_read_rows(read_ptr, (png_bytepp)&row_buf0, (png_bytepp)NULL, 1);
			for (long x=0; x<(long)width; x++) {
				bm_ptr[y*bitmap_width + x] = RGB_to_tracklog_palette(row_buf0[x*4+2], row_buf0[x*4+1], row_buf0[x*4]);
			}
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
/*
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
*/

void OSPro_GetTile(HINTERNET hConnect, char * text, long northing, long easting, CTileDatabase * db, FILE * fp_log, BOOL bForce, int nMapFlavour, int nMapScale)
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

// ToDo: Need to get the BBOX extents calculated correctly using m_nPixelsPerTile & m_nMetresPerTile variables!

// 1:50K = 200 * 5.0 -> 1000 meteres  
// 1:25K = 200 * 2.5 ->  500 meters
// 1:10K = 250 * 1.0 ->  250 meters

//if (nMapScale == MAP_SCALE_10K) {

	if (nMapFlavour == MAP_RASTER) {

		if (db->m_mapscale == 10000) {

			sprintf(strRequest, "osmapapi/ts?FORMAT=image%%2Fpng&KEY=DEMOAPI&URL=http%%3A%%2F%%2Fosopenspacewiki.ordnancesurvey.co.uk%%2Fwiki%%2Fextensions%%2Fhtmlets%%2FOSProCS.html&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&EXCEPTIONS=application%%2Fvnd.ogc.se_inimage&LAYERS=1&PRODUCT=10K&SRS=EPSG%%3A27700&"
								"BBOX=%d,%d,%d,%d&WIDTH=%d&HEIGHT=%d", easting, northing, easting+nMetres, northing-nMetres, nPixels, nPixels);

		} else if (db->m_mapscale == 25000) {

			sprintf(strRequest, "osmapapi/ts?FORMAT=image%%2Fpng&KEY=DEMOAPI&URL=http%%3A%%2F%%2Fosopenspacewiki.ordnancesurvey.co.uk%%2Fwiki%%2Fextensions%%2Fhtmlets%%2FOSProCS.html&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&EXCEPTIONS=application%%2Fvnd.ogc.se_inimage&LAYERS=2.5&PRODUCT=25K&SRS=EPSG%%3A27700&"
								"BBOX=%d,%d,%d,%d&WIDTH=%d&HEIGHT=%d", easting, northing, easting+nMetres, northing-nMetres, nPixels, nPixels);

		} else if (db->m_mapscale == 50000) {

			sprintf(strRequest, "osmapapi/ts?FORMAT=image%%2Fpng&KEY=DEMOAPI&URL=http%%3A%%2F%%2Fosopenspacewiki.ordnancesurvey.co.uk%%2Fwiki%%2Fextensions%%2Fhtmlets%%2FOSProCS.html&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&EXCEPTIONS=application%%2Fvnd.ogc.se_inimage&LAYERS=5&PRODUCT=50K&SRS=EPSG%%3A27700&"
								"BBOX=%d,%d,%d,%d&WIDTH=%d&HEIGHT=%d", easting, northing, easting+nMetres, northing-nMetres, nPixels, nPixels);

		} else if (db->m_mapscale == 250000) {

			sprintf(strRequest, "osmapapi/ts?FORMAT=image%%2Fpng&KEY=DEMOAPI&URL=http%%3A%%2F%%2Fosopenspacewiki.ordnancesurvey.co.uk%%2Fwiki%%2Fextensions%%2Fhtmlets%%2FOSProCS.html&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&EXCEPTIONS=application%%2Fvnd.ogc.se_inimage&LAYERS=25&PRODUCT=250K&SRS=EPSG%%3A27700&"
								"BBOX=%d,%d,%d,%d&WIDTH=%d&HEIGHT=%d", easting, northing, easting+nMetres, northing-nMetres, nPixels, nPixels);
		} else if (nMapScale == MAP_SCALE_MS) {

			sprintf(strRequest, "osmapapi/ts?FORMAT=image%%2Fpng&KEY=DEMOAPI&URL=http%%3A%%2F%%2Fosopenspacewiki.ordnancesurvey.co.uk%%2Fwiki%%2Fextensions%%2Fhtmlets%%2FOSProCS.html&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&EXCEPTIONS=application%%2Fvnd.ogc.se_inimage&LAYERS=100&PRODUCT=MS&SRS=EPSG%%3A27700&"
								"BBOX=%d,%d,%d,%d&WIDTH=200&HEIGHT=200", easting, northing, easting+20000, northing-20000);
		}

	} else if (nMapFlavour == MAP_VECTOR) {

		if (db->m_mapscale == 10000) {
				//http://osopenspacepro.ordnancesurvey.co.uk/osmapapi/ts?FORMAT=image%2Fpng&KEY=A493C3EB96133019E0405F0ACA6056E3&URL=http%3A%2F%2Fwww.geograph.org.uk%2Fshowmap.php%3Fgridref%3DSE65239512&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&EXCEPTIONS=application%2Fvnd.ogc.se_inimage&LAYERS=1&PRODUCT=VML&SRS=EPSG%3A27700&BBOX=465250,495250,465500,495500&WIDTH=250&HEIGHT=250
				sprintf(strRequest, "osmapapi/ts?FORMAT=image%%2Fpng&KEY=DEMOAPI&URL=http%%3A%%2F%%2Fosopenspacewiki.ordnancesurvey.co.uk%%2Fwiki%%2Fextensions%%2Fhtmlets%%2FOSProCS.html&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&EXCEPTIONS=application%%2Fvnd.ogc.se_inimage&LAYERS=1&PRODUCT=VML&SRS=EPSG%%3A27700&"
									"BBOX=%d,%d,%d,%d&WIDTH=250&HEIGHT=250", easting, northing, easting+250, northing-250);

		} else if (db->m_mapscale == 25000) {
			// http://osopenspacepro.ordnancesurvey.co.uk/osmapapi/ts?FORMAT=image%2Fpng&KEY=A493C3EB96133019E0405F0ACA6056E3&URL=http%3A%2F%2Fwww.geograph.org.uk%2Fshowmap.php%3Fgridref%3DSE65239512&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&EXCEPTIONS=application%2Fvnd.ogc.se_inimage&LAYERS=2.5&PRODUCT=VMD&SRS=EPSG%3A27700&BBOX=465000,495500,465500,496000&WIDTH=200&HEIGHT=200
			sprintf(strRequest, "osmapapi/ts?FORMAT=image%%2Fpng&KEY=DEMOAPI&URL=http%%3A%%2F%%2Fosopenspacewiki.ordnancesurvey.co.uk%%2Fwiki%%2Fextensions%%2Fhtmlets%%2FOSProCS.html&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&EXCEPTIONS=application%%2Fvnd.ogc.se_inimage&LAYERS=2.5&PRODUCT=VMD&SRS=EPSG%%3A27700&"
								"BBOX=%d,%d,%d,%d&WIDTH=200&HEIGHT=200", easting, northing, easting+500, northing-500);
		}

	} else if (nMapFlavour == MAP_STREET) {

		if (db->m_mapscale == 10000) {
						sprintf(strRequest, "osmapapi/ts?FORMAT=image%%2Fpng&KEY=DEMOAPI&URL=http%%3A%%2F%%2Fosopenspacewiki.ordnancesurvey.co.uk%%2Fwiki%%2Fextensions%%2Fhtmlets%%2FOSProCS.html&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&EXCEPTIONS=application%%2Fvnd.ogc.se_inimage&LAYERS=1&PRODUCT=SV&SRS=EPSG%%3A27700&"
											"BBOX=%d,%d,%d,%d&WIDTH=250&HEIGHT=250", easting, northing, easting+250, northing-250);
		}
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


// http://osopenspacepro.ordnancesurvey.co.uk/osmapapi/ts?FORMAT=image%2Fpng&KEY=DEMOAPI&URL=http%3A%2F%2Fosopenspacewiki.ordnancesurvey.co.uk%2Fwiki%2Fextensions%2Fhtmlets%2FOSProCS.html&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&EXCEPTIONS=application%2Fvnd.ogc.se_inimage&LAYERS=2.5&PRODUCT=25K&SRS=EPSG%3A27700&BBOX=258000,667500,258500,668000&WIDTH=200&HEIGHT=200

int OSPro_LoadTiles(CTileDatabase* g_db, long minx, long miny, long maxx, long maxy, BOOL bForce, int nMapFlavour, int nMapScale)
{
HINTERNET hOpen, hConnect;
char strTile[256];

	hOpen = NULL;
	FILE * fp_log = fopen("log.txt", "a+");

CProgressWindow wndProgress;
wndProgress.Initialize();

//long nTileSize = GetTileDatabaseScale(nMapScale);

int nMeters = g_db->m_nMetresPerTile;

wndProgress.ResetProgressBar("Downloading:", (maxy-miny)/nMeters*(maxx-minx)/nMeters);

	for (long y = miny; y < maxy; y += nMeters) {
		for (long x = minx; x < maxx; x += nMeters) {

			if (!wndProgress.ProgressBar()) return false;

			easting_northing_to_OSref5(x, y, strTile);
			if (!bForce && g_db->TileLoaded(y, x)) {
				fprintf(fp_log, "Tile: %s skipped - tile exists\n", strTile);
				continue;
			} else {

				// Only connect to OSPro if required
// SNL 11/06/2013 - hmmmm!
g_db->InitDatabase(y,x,MAP_DATUM_OSGB);
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
				OSPro_GetTile(hConnect, strTile, y, x, g_db, fp_log, bForce, nMapFlavour, nMapScale);
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

bool OSPro_ReadTiles(long minx, long maxx, long miny, long maxy, CProgressWindow& wndProgress, 
						unsigned char * bitmap_palette, long & bitmap_palette_length, 
						unsigned char * & bitmap_memory , long & bitmap_memory_size, 
						long & bitmap_width, long & dest_bitmap_width,
						long & bitmap_height, long & dest_bitmap_height,
						int nFlavour, int nCountry, int nScale)
{
	// PNG writing variables


/* 250K
	minx = minx/5*5;
	miny = miny/5*5;
	maxx = (maxx+4)/5*5;
	maxy = (maxy+4)/5*5;
*/

//	minx = minx/20*20;
//	miny = miny/20*20;
//	maxx = (maxx+19)/20*20;
//	maxy = (maxy+19)/20*20;
	AlignMapExtents(nScale, minx, miny, maxx, maxy);

int nDatum = GetDatum(nCountry);
int nMapScale = GetMapScale(nScale);
int nPixelsPerTile = GetTilePixelWidth(nFlavour, nCountry, nScale);
int nMetersPerTile = GetTileMeterWidth(nFlavour, nCountry, nScale);
int nTileBitDepth = GetTileBitDepth(nFlavour, nCountry, nScale);

//	CTileDatabase db(MAP_DATUM_OSGB);
	CTileDatabase	db(nDatum, nMetersPerTile, nScale, nPixelsPerTile);

	long scale = db.m_nMetresPerTile;

	
long height = GetTileWidth(nScale);
long width = GetTileWidth(nScale);

int iterator = GetTileIterator(nScale)*1000;

//	long square_width;
//	long square_height;
	long x,y;
	for (y=maxy-iterator; y>=miny; y -= iterator) {
		for (x=minx; x<maxx; x += iterator) {
/* 250K etc.
	for (y=maxy-20; y>=miny; y -= 20) {
		for (x=minx; x<maxx; x+= 20) {
*/
			if (!wndProgress.ProgressBar()) return false;

// COMMON start
			if (bitmap_memory == NULL) {
				// First square found so allocate memory for bitmap
/*
				bitmap_width = width*(maxx-minx);
				bitmap_height = height*(maxy-miny);
				dest_bitmap_width = width*(maxx-minx-2);
				dest_bitmap_height = height*(maxy-miny-2);
*/
				bitmap_width = width*((maxx-minx)/1000);
				bitmap_height = height*((maxy-miny)/1000);
				dest_bitmap_width = width*((maxx-minx-2000)/1000);
				dest_bitmap_height = height*((maxy-miny-2000)/1000);

				bitmap_memory_size = bitmap_width * bitmap_height;
				bitmap_memory = (BYTE *)malloc(bitmap_memory_size);

				if (bitmap_memory == NULL) {
					printf("Couldn't allocate %d bytes for bitmap\n", bitmap_memory_size);
					//exit(0);
					return false;
				}
				memset(bitmap_memory, 0xff, bitmap_memory_size);		// assume 0xff is white

				if (bitmap_width > 32000)
					printf("Warning: width is %d pixels\n", bitmap_width);
				if (bitmap_height > 32000)
					printf("Warning: height is %d pixels\n", bitmap_height);

				// get palette from database
				bitmap_palette_length = 4*256;
				memcpy(bitmap_palette, get_tracklog_palette(), bitmap_palette_length);
			}
// COMMON end
			// copy tile into bitmap area
//			BYTE * bitmap_offset = bitmap_memory + (maxy-1-y)*height*bitmap_width+(x-minx)*width;
			BYTE * bitmap_offset = bitmap_memory + ((maxy-1000-y)/1000)*height*bitmap_width+((x-minx)/1000)*width;

			if (nMapScale == 10000) {
				for (int Yoffset=0; Yoffset < 4; Yoffset ++) {
					for (int Xoffset=0; Xoffset < 4; Xoffset ++) {
//						read_png_10K(y/**1000*/+250*Yoffset, x/**1000*/+250*Xoffset, &db, bitmap_offset+(3-Yoffset)*250*bitmap_width+250*Xoffset, bitmap_width);
						if (nTileBitDepth == 8) {

//							read_png(y/*1000*/+250*Yoffset, x/*1000*/+250*Xoffset, &db, bitmap_offset+(3-Yoffset)*250*bitmap_width+250*Xoffset, bitmap_width);
							read_png(y+nMetersPerTile*Yoffset, x+nMetersPerTile*Xoffset, &db, bitmap_offset+(3-Yoffset)*nPixelsPerTile*bitmap_width+nPixelsPerTile*Xoffset, bitmap_width);
						} else {
							read_png_10K(y+nMetersPerTile*Yoffset, x+nMetersPerTile*Xoffset, &db, bitmap_offset+(3-Yoffset)*nPixelsPerTile*bitmap_width+nPixelsPerTile*Xoffset, bitmap_width);
						}
					}
				}
			} else if (nMapScale == 25000) {
/*
				read_png(y*1000, x*1000, &db, bitmap_offset+200*bitmap_width, bitmap_width);
				read_png(y*1000, x*1000+500, &db, bitmap_offset+200*bitmap_width+200, bitmap_width);
				read_png(y*1000+500, x*1000, &db, bitmap_offset, bitmap_width);
				read_png(y*1000+500, x*1000+500, &db, bitmap_offset+200, bitmap_width);
*/
/*
				read_png(y, x, &db, bitmap_offset+200*bitmap_width, bitmap_width);
				read_png(y, x+500, &db, bitmap_offset+200*bitmap_width+200, bitmap_width);
				read_png(y+500, x, &db, bitmap_offset, bitmap_width);
				read_png(y+500, x+500, &db, bitmap_offset+200, bitmap_width);
*/
				for (int Yoffset=0; Yoffset < 2; Yoffset ++) {
					for (int Xoffset=0; Xoffset < 2; Xoffset ++) {
						if (nTileBitDepth == 8) {
							read_png(y+nMetersPerTile*Yoffset, x+nMetersPerTile*Xoffset, &db, bitmap_offset+(1-Yoffset)*nPixelsPerTile*bitmap_width+nPixelsPerTile*Xoffset, bitmap_width);
						} else {
//							read_png_10K(y+nMetersPerTile*Yoffset, x+nMetersPerTile*Xoffset, &db, bitmap_offset+(3-Yoffset)*nPixelsPerTile*bitmap_width+nPixelsPerTile*Xoffset, bitmap_width);
				read_png_10K(y, x, &db, bitmap_offset+200*bitmap_width, bitmap_width);
				read_png_10K(y, x+500, &db, bitmap_offset+200*bitmap_width+200, bitmap_width);
				read_png_10K(y+500, x, &db, bitmap_offset, bitmap_width);
				read_png_10K(y+500, x+500, &db, bitmap_offset+200, bitmap_width);
						}
					}
				}

			} else if (nMapScale == 50000) {
//				read_png(y*1000, x*1000, &db, bitmap_memory + (maxy-1-y)*height*bitmap_width+(x-minx)*width, bitmap_width);
				read_png(y/*1000*/, x/*1000*/, &db, bitmap_memory + ((maxy-1000-y)/1000)*height*bitmap_width+((x-minx)/1000)*width, bitmap_width);

			} else if (nMapScale == MAP_SCALE_VML) {
				for (int Yoffset=0; Yoffset < 4; Yoffset ++) {
					for (int Xoffset=0; Xoffset < 4; Xoffset ++) {
						read_png_10K(y*1000+250*Yoffset, x*1000+250*Xoffset, &db, bitmap_offset+(3-Yoffset)*250*bitmap_width+250*Xoffset, bitmap_width);
					}
				}
			} else if (nMapScale == MAP_SCALE_SV) {
/*
				for (int Yoffset=0; Yoffset < 4; Yoffset ++) {
					for (int Xoffset=0; Xoffset < 4; Xoffset ++) {
						read_png(y*1000+250*Yoffset, x*1000+250*Xoffset, &db, bitmap_offset+(3-Yoffset)*250*bitmap_width+250*Xoffset, bitmap_width);
					}
				}
*/
			} else if (nMapScale == MAP_SCALE_VMD) {

				read_png(y*1000, x*1000, &db, bitmap_offset+200*bitmap_width, bitmap_width);
				read_png(y*1000, x*1000+500, &db, bitmap_offset+200*bitmap_width+200, bitmap_width);
				read_png(y*1000+500, x*1000, &db, bitmap_offset, bitmap_width);
				read_png(y*1000+500, x*1000+500, &db, bitmap_offset+200, bitmap_width);

			} else if (nMapScale == MAP_SCALE_250K) {
				read_png(y*1000, x*1000, &db, bitmap_memory + (maxy-iterator-y)*height*bitmap_width+(x-minx)*width, bitmap_width);
			} else if (nMapScale == MAP_SCALE_MS) {
				read_png(y*1000, x*1000, &db, bitmap_memory + (maxy-iterator-y)*height*bitmap_width+(x-minx)*width, bitmap_width);
			}
		}
	}
	return true;
}
