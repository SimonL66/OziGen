#include "stdafx.h"

#include "WebMapServer.h"

#include "ProgressWindow.h"

#include "CustomMap.h"

#include "func.h"

#include "..\MapLib\palette.h"

#include "Graphics.h"

extern unsigned char mm_osgb_palette[1024];

extern CCustomMap MyMap;

extern int abortProgram;		// This needs to go... maybe?

void Tile2LatLon_Luref(double dNorthingTile, double dEastingTile, double* dLat, double* dLon)
{
	double dPixels_per_tile = MyMap.GetPixelsPerTile();
	double dMetres_per_tile = MyMap.GetMetresPerTile();

	double dMetres_per_pixel = dMetres_per_tile / dPixels_per_tile;

	double dEasting_pixel = (dEastingTile * dPixels_per_tile);
	double dEasting = (dEasting_pixel * dMetres_per_pixel);

	double dNorthing_pixel = (dNorthingTile * dPixels_per_tile);
	double dNorthing = (dNorthing_pixel*dMetres_per_pixel);

	int nProjection = MyMap.GetProjection();
	EastingNorthing_WGS84(nProjection, dEasting, dNorthing, dLat, dLon);
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
/*
CString CCustomMap::GetRequest_Lux(int easting, int northing)
{
	CString strRequest;

	int levelOfDetail = LayerArray[m_nCountry][m_nProduct][m_nLayer].nZoom;

	int pixels_per_tile = LayerArray[m_nCountry][m_nProduct][m_nLayer].nPixels;
	int metres_per_tile = LayerArray[m_nCountry][m_nProduct][m_nLayer].nMetres;

	double metres_per_pixel = metres_per_tile / (double) pixels_per_tile;

double northing_pixel = northing / metres_per_pixel;
long northing_tile = (long)(northing_pixel/pixels_per_tile);

double easting_pixel = easting / metres_per_pixel;
long easting_tile = (long)(easting_pixel/pixels_per_tile);

	strRequest.Format(
			"topo_gr/%02d/000/000/%03d/000/000/%03d.png",
			levelOfDetail,
			easting_tile, northing_tile);

	return strRequest;
}
*/

void LatLon2Tile_Luref(double dLat, double dLon, double* dNorthingTile, double* dEastingTile)
{
	double dNorthing, dEasting;

	int nProjection = MyMap.GetProjection();
	WGS84_EastingNorthing(nProjection, dLat, dLon, &dEasting, &dNorthing);

	double dPixels_per_tile = MyMap.GetPixelsPerTile();
	double dMetres_per_tile = MyMap.GetMetresPerTile();

	double dMetres_per_pixel = dMetres_per_tile / dPixels_per_tile;

	double dEasting_pixel = dEasting / dMetres_per_pixel;
	*dEastingTile = (dEasting_pixel/dPixels_per_tile);

	double dNorthing_pixel = dNorthing / dMetres_per_pixel;
	*dNorthingTile = (dNorthing_pixel/dPixels_per_tile);
}


bool CH1903_ReadTiles_QCT(long y, long x, CTileDatabase * db, unsigned char * bitmap_offset, long bitmap_width)
{
	if (MyMap.GetTileGfxType() == PNG) {
		if (!read_png(y, x, db, bitmap_offset, bitmap_width) == 1)
		{
			char tstring[256];
			sprintf(tstring,"Y=%d, X=%d", y,x);
			read_jpg(y, x, db, tstring, bitmap_offset, bitmap_width);
		}
	} else {
		char tstring[256];
		sprintf(tstring,"Y=%d, X=%d", y,x);
		if (read_jpg(y, x, db, tstring, bitmap_offset, bitmap_width) == 1)
		{
			read_png(y, x, db, bitmap_offset, bitmap_width);
		}
	}

	unsigned char	palette_convert[256];
	unsigned char * tracklog_palette = get_tracklog_palette();

	for (int i=0; i < 256; i++)
		palette_convert[i] = (unsigned char)RGBtoPalette(mm_osgb_palette, 128, tracklog_palette[i*4+0], tracklog_palette[i*4+1], tracklog_palette[i*4+2]);

	for (y=0; y < MyMap.GetPixelsPerTile()/2; y++) {
		for (x=0; x < MyMap.GetPixelsPerTile(); x++) {
			// Flip image in the Y axis and convert to the 128 colour memory map palette
			unsigned char t = palette_convert[bitmap_offset[x+y*bitmap_width]];
			bitmap_offset[x+y*bitmap_width] = palette_convert[bitmap_offset[x+(MyMap.GetPixelsPerTile()-1-y)*bitmap_width]];
			bitmap_offset[x+(MyMap.GetPixelsPerTile()-1-y)*bitmap_width] = t;
		}
	}
	return true;
}

bool ReadTiles_Luref(	CRect BBox, 
						CProgressWindow& wndProgress, 
						unsigned char * bitmap_palette, long & bitmap_palette_length, 
						unsigned char * & bitmap_memory , long & bitmap_memory_size, 
						long & bitmap_width, long & dest_bitmap_width,
						long & bitmap_height, long & dest_bitmap_height)
{
	int nDatum = MyMap.GetDatum();
	int nMeters = MyMap.GetMetresPerTile();

	int nMapScale = MyMap.GetLevelOfDetail();;
	int nPixles = MyMap.GetPixelsPerTile();

	CTileDatabase	db(nDatum, nMeters, nMapScale, nPixles);

	wndProgress.ResetProgressBar("Tile:", (BBox.right-BBox.left)*(BBox.top-BBox.bottom));

	for (long y = BBox.bottom; y < BBox.top; y++) {
		for (long x = BBox.left; x < BBox.right; x++) {

//	for (long y = maxy-nMeters; y >= miny; y -= nMeters) {
//		for (long x=minx; x<maxx; x += nMeters) {

			if (!wndProgress.ProgressBar()) return false;

			if (bitmap_memory == NULL) { // COMMON start

				// First square found so allocate memory for bitmap
				// Needs to be double as now we're dividing by Meter per pixel which may not be rounded to the nearest factor of 10!

				bitmap_width = nPixles*(BBox.right-BBox.left);
				bitmap_height = nPixles*(BBox.top-BBox.bottom);

				dest_bitmap_width = nPixles*(BBox.right-BBox.left-2);
				dest_bitmap_height = nPixles*(BBox.top-BBox.bottom-2);

				bitmap_memory_size = bitmap_width * bitmap_height;
				bitmap_memory = (BYTE *)malloc(bitmap_memory_size);

				if (bitmap_memory == NULL) {
					printf("Couldn't allocate %d bytes for bitmap\n", bitmap_memory_size);
					return false;
				}
				memset(bitmap_memory, 0xff, bitmap_memory_size);		// assume 0xff is white

				// get palette from database
				bitmap_palette_length = 4*256;
				memcpy(bitmap_palette, get_tracklog_palette(), bitmap_palette_length);
			
			} // COMMON end


			// copy tile into bitmap area
			BYTE * bitmap_offset;
			if ((MyMap.GetProjection() == 3812) || (MyMap.GetProjection() == 21781)) {
				bitmap_offset = bitmap_memory + (y-BBox.bottom)*nPixles*bitmap_width+(x-BBox.left)*nPixles;
			} else {
				bitmap_offset = bitmap_memory + (BBox.top-y)*nPixles*bitmap_width+(x-BBox.left)*nPixles;
			}

			if (MyMap.GetTileGfxType() == PNG) {
				read_png(y, x, &db, bitmap_offset, bitmap_width);
			} else {
				char tstring[256];
				sprintf(tstring,"Y=%d, X=%d", y,x);
				if (read_jpg(y, x, &db, tstring, bitmap_offset, bitmap_width) == 1) {
					read_png(y, x, &db, bitmap_offset, bitmap_width);
				}
			}
		}
	}
	return bitmap_memory != NULL;
}

bool ReadTiles_CH1903(	CRect BBox, 
						CProgressWindow& wndProgress, 
						unsigned char * bitmap_palette, long & bitmap_palette_length, 
						unsigned char * & bitmap_memory , long & bitmap_memory_size, 
						long & bitmap_width, long & dest_bitmap_width,
						long & bitmap_height, long & dest_bitmap_height)
{
	int nDatum = MyMap.GetDatum();
	int nMeters = MyMap.GetMetresPerTile();

	int nMapScale = MyMap.GetLevelOfDetail();;
	int nPixles = MyMap.GetPixelsPerTile();

	CTileDatabase	db(nDatum, nMeters, nMapScale, nPixles);

	wndProgress.ResetProgressBar("Tile:", (BBox.right-BBox.left)*(BBox.top-BBox.bottom));

	for (long y = BBox.bottom; y < BBox.top; y++) {
		for (long x = BBox.left; x < BBox.right; x++) {

//	for (long y = maxy-nMeters; y >= miny; y -= nMeters) {
//		for (long x=minx; x<maxx; x += nMeters) {

			if (!wndProgress.ProgressBar()) return false;

			if (bitmap_memory == NULL) { // COMMON start

				// First square found so allocate memory for bitmap
				// Needs to be double as now we're dividing by Meter per pixel which may not be rounded to the nearest factor of 10!

				bitmap_width = nPixles*(BBox.right-BBox.left);
				bitmap_height = nPixles*(BBox.top-BBox.bottom);

				dest_bitmap_width = nPixles*(BBox.right-BBox.left-2);
				dest_bitmap_height = nPixles*(BBox.top-BBox.bottom-2);

				bitmap_memory_size = bitmap_width * bitmap_height;
				bitmap_memory = (BYTE *)malloc(bitmap_memory_size);

				if (bitmap_memory == NULL) {
					printf("Couldn't allocate %d bytes for bitmap\n", bitmap_memory_size);
					return false;
				}
				memset(bitmap_memory, 0xff, bitmap_memory_size);		// assume 0xff is white

				// get palette from database
				bitmap_palette_length = 4*256;
				memcpy(bitmap_palette, get_tracklog_palette(), bitmap_palette_length);
			
			} // COMMON end


			// copy tile into bitmap area
			BYTE * bitmap_offset;
			if ((MyMap.GetProjection() == 3812) || (MyMap.GetProjection() == 21781)) {
				bitmap_offset = bitmap_memory + (y-BBox.bottom)*nPixles*bitmap_width+(x-BBox.left)*nPixles;
			} else {
				bitmap_offset = bitmap_memory + (BBox.top-1-y)*nPixles*bitmap_width+(x-BBox.left)*nPixles;
			}

			if (MyMap.GetTileGfxType() == PNG) {
				read_png(y, x, &db, bitmap_offset, bitmap_width);
			} else {
				char tstring[256];
				sprintf(tstring,"Y=%d, X=%d", y,x);
				if (read_jpg(y, x, &db, tstring, bitmap_offset, bitmap_width) == 1) {
					read_png(y, x, &db, bitmap_offset, bitmap_width);
				}
			}
		}
	}
	return bitmap_memory != NULL;
}

void Tile2LatLon_NGI(double dNorthingTile, double dEastingTile, double* dLat, double* dLon)
{
double xOrigin = 450000.0;
double yOrigin = 800000.0;

	double dPixels_per_tile = MyMap.GetPixelsPerTile();

//	double dMetres_per_tile = MyMap.GetMetresPerTile();
//	double dMetres_per_pixel = dMetres_per_tile / dPixels_per_tile;

	double dMetres_per_pixel = 2.6458386250105836;

	double dEasting_pixel = (dEastingTile * dPixels_per_tile);
	double dEasting = xOrigin + (dEasting_pixel * dMetres_per_pixel);

	double dNorthing_pixel = (dNorthingTile * dPixels_per_tile);
	double dNorthing = yOrigin - (dNorthing_pixel*dMetres_per_pixel);

	int nProjection = MyMap.GetProjection();
	EastingNorthing_WGS84(nProjection, dEasting, dNorthing, dLat, dLon);
}

void LatLon2Tile_NGI(double dLat, double dLon, double* dNorthingTile, double* dEastingTile)
{
double xOrigin = 450000.0;
double yOrigin = 800000.0;

	double dNorthing, dEasting;

	int nProjection = MyMap.GetProjection();
	WGS84_EastingNorthing(nProjection, dLat, dLon, &dEasting, &dNorthing);

	double dPixels_per_tile = MyMap.GetPixelsPerTile();
//	double dMetres_per_tile = MyMap.GetMetresPerTile();
//	double dMetres_per_pixel = dMetres_per_tile / dPixels_per_tile;

	double dMetres_per_pixel = 2.6458386250105836;

	double dEasting_pixel = (dEasting - xOrigin) / dMetres_per_pixel;
	*dEastingTile = (dEasting_pixel/dPixels_per_tile);

	double dNorthing_pixel = (yOrigin - dNorthing) / dMetres_per_pixel;
	*dNorthingTile = (dNorthing_pixel/dPixels_per_tile);
}


void Tile2LatLon_CH1903(double dNorthingTile, double dEastingTile, double* dLat, double* dLon)
{
	double dPixels_per_tile = MyMap.GetPixelsPerTile();
	double dMetres_per_tile = MyMap.GetMetresPerTile();

	double dMetres_per_pixel = dMetres_per_tile / dPixels_per_tile;

	double dEasting_pixel = (dEastingTile * dPixels_per_tile);
	double dEasting = 420000 + (dEasting_pixel * dMetres_per_pixel);

	double dNorthing_pixel = (dNorthingTile * dPixels_per_tile);
	double dNorthing = 350000 - (dNorthing_pixel*dMetres_per_pixel);

	int nProjection = MyMap.GetProjection();
	EastingNorthing_WGS84(nProjection, dEasting, dNorthing, dLat, dLon);
}

void LatLon2Tile_CH1903(double dLat, double dLon, double* dNorthingTile, double* dEastingTile)
{
	double dNorthing, dEasting;

	int nProjection = MyMap.GetProjection();
	WGS84_EastingNorthing(nProjection, dLat, dLon, &dEasting, &dNorthing);

	double dPixels_per_tile = MyMap.GetPixelsPerTile();
	double dMetres_per_tile = MyMap.GetMetresPerTile();

	double dMetres_per_pixel = dMetres_per_tile / dPixels_per_tile;

	double dEasting_pixel = (dEasting - 420000) / dMetres_per_pixel;
	*dEastingTile = (dEasting_pixel/dPixels_per_tile);

	double dNorthing_pixel = (350000 - dNorthing) / dMetres_per_pixel;
	*dNorthingTile = (dNorthing_pixel/dPixels_per_tile);
}

#define	HTTP_GET_SIZE	65536
bool ch1903_get_file_1(HINTERNET hConnect, LPCSTR pathname, LPCSTR strReferer, LPCSTR tx_saveto)
{
HINTERNET hReq;
DWORD  dwSize, dwCode;
CHAR szData[HTTP_GET_SIZE+1];

	if ( !(hReq = HttpOpenRequest (hConnect, "GET", pathname, HTTP_VERSION, strReferer, NULL, 0 ,0 ))) {
		ErrorOut (GetLastError(), "HttpOpenRequest");
		return false;
	}

	if (!HttpSendRequest (hReq, NULL, 0, NULL, 0) ) {
		ErrorOut (GetLastError(), "HttpSend");
		return false;
	}

	dwSize = sizeof (DWORD) ;
	if ( !HttpQueryInfo (hReq, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwCode, &dwSize, NULL)) {
		ErrorOut (GetLastError(), "HttpQueryInfo");
		return false;
	}

	if ( dwCode == HTTP_STATUS_DENIED || dwCode == HTTP_STATUS_PROXY_AUTH_REQ) {
		// This is a secure page.
		fprintf(stderr, "This page is password protected.\n");
		return false;
	}
	if ( dwCode == 404) {
		fprintf(stderr, "Page not found.\n");
		return false;
	}

	FILE * fp = fopen(tx_saveto, "wb+");
	if (fp == NULL) {
		printf("Couldn't create %s\n", tx_saveto);
		return false;
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
		return false;

	// Validate PNG
	LPBYTE buffer = (LPBYTE)malloc(file_len+1);
	if (buffer == 0) {
		fprintf(stderr, "Couldn't allocate %d bytes to verify %s\n", file_len, tx_saveto);
		return false;
	}
	memset(buffer, 0, file_len+1);
	fp = fopen(tx_saveto, "rb");
	if (fp == NULL) {
		fprintf(stderr, "Failed to reopen %s\n", tx_saveto);
		free(buffer);
		return false;
	}
	if (fread(buffer, 1, file_len, fp) != file_len) {
		fprintf(stderr, "Error reading %s\n", tx_saveto);
		free(buffer);
		return false;
	}
	fclose(fp);

	unsigned char pnghdr[] = {0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a};
	unsigned char jpghdr[] = {0xFF, 0xD8, 0xFF, 0xE0, 0x00, 0x10, 0x4A, 0x46, 0x49, 0x46};
	unsigned char gifhdr[] = {0x47, 0x49, 0x46, 0x38, 0x39, 0x61};

	if ((memcmp(buffer, pnghdr, sizeof(pnghdr)) == 0)
	||  (memcmp(buffer, jpghdr, sizeof(jpghdr)) == 0)
	||  (memcmp(buffer, gifhdr, sizeof(gifhdr)) == 0)) {
		free(buffer);
		return true;
	} else {
		fprintf(stderr, "Error retrieving %s\n", tx_saveto);
		free(buffer);
		return false;
	}
}

int GetFile_CH1903(HINTERNET hConnect, LPCSTR pathname, LPCSTR strReferer, LPCSTR tx_saveto)
{
	// Attempt to get file and retry once if failed
	if (ch1903_get_file_1(hConnect, pathname, strReferer, tx_saveto))
		return 1;
	return ch1903_get_file_1(hConnect, pathname, strReferer, tx_saveto);
}

void RequestTile_CH1903(HINTERNET hConnect, char * text, long northing, long easting, CTileDatabase * db, FILE * fp_log, bool bForce)
{
	if (text == NULL) return;

//	int nPixels = db->m_nPixelsPerTile;
//	int nLayer = db->m_mapscale;
//	int nMetres = db->m_nMetresPerTile;

//int nMetres = MyMap.GetMetresPerTile();

//	northing = northing/nMetres*nMetres;
//	easting = easting/nMetres*nMetres;

	if (!bForce && db->TileLoaded(northing, easting)) return;

	CString strRequest;

	strRequest.Empty();

	if (MyMap.GetServerType() == WMS)				strRequest = MyMap.GetRequest_WMS(easting, northing);

	else if (MyMap.GetServerType() == GetMapping)	strRequest = MyMap.GetRequest_GetMapping(easting, northing);
	else if (MyMap.GetServerType() == OSM)			strRequest = MyMap.GetRequest_OSM(easting, northing);

	else if (MyMap.GetServerType() == Kompass)		strRequest = MyMap.GetRequest_Kompass(easting, northing);
	else if (MyMap.GetServerType() == Bing)			strRequest = MyMap.GetRequest_Bing(easting, northing);
	else if (MyMap.GetServerType() == SatMap)		strRequest = MyMap.GetRequest_SatMap(easting, northing);

	else if (MyMap.GetServerType() == IGN)			strRequest = MyMap.GetRequest_IGN(easting, northing);
	else if (MyMap.GetServerType() == LUX)			strRequest = MyMap.GetRequest_Lux(easting, northing);
	else if (MyMap.GetServerType() == NGI)			strRequest = MyMap.GetRequest_NGI(easting, northing);
	else if (MyMap.GetServerType() == CH1903)		strRequest = MyMap.GetRequest_CH1903(easting, northing);
	
	else if (MyMap.GetServerType() == ArcGIS)		strRequest = MyMap.GetRequest_ArcGIS(easting, northing);

	CString strReferer = MyMap.Referer();

	if (GetFile_CH1903(hConnect, strRequest, strReferer, "tile.tmp")) {
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

bool DownloadTiles_CH1903(CTileDatabase* g_db, CRect BBox, long nDelta, bool bForce)
{
HINTERNET hOpen, hConnect;
char strTile[256];

	hOpen = NULL;
	FILE * fp_log = fopen("log.txt", "a+");

CProgressWindow wndProgress;
wndProgress.Initialize();

wndProgress.ResetProgressBar("Downloading:", (BBox.right-BBox.left)*(BBox.top-BBox.bottom));

	for (long y = BBox.bottom; y < BBox.top; y++) {
		for (long x = BBox.left; x < BBox.right; x++) {

			if (!wndProgress.ProgressBar()) return false;

			if (!bForce && g_db->TileLoaded(y, x)) {
		
				fprintf(fp_log, "Tile: CH1903 [%05d,%05d] skipped - tile exists\n", x,y);
				continue;
			} else {
				
				// Only connect to WMS if required.
				g_db->InitDatabase(y,x,MyMap.GetDatum());
				if (hOpen == NULL) {
					if ( !(hOpen = InternetOpen ( "Sample",  LOCAL_INTERNET_ACCESS , NULL, 0, 0) ) ) {
						ErrorOut ( GetLastError(), "InternetOpen");
						return false;
					}

					if ( !(hConnect = InternetConnect ( hOpen, MyMap.wmsGetAttribution(), MyMap.wmsGetPort(), "",	"", INTERNET_SERVICE_HTTP, 0  , 0) ) ) {
						ErrorOut (GetLastError(), "InternetConnect");
						return false;
					}
				}
				fprintf(fp_log, "Tile: CH1903 [%05d,%05d] loading...\n", x,y);
				RequestTile_CH1903(hConnect, strTile, y, x, g_db, fp_log, bForce);
			}
		}
	}
	fclose(fp_log);

	if (hOpen != NULL) {
		if (!InternetCloseHandle (hConnect) ) {
			ErrorOut (GetLastError (), "CloseHandle on hConnect");
			return false;
		}
		if (!InternetCloseHandle (hOpen) ) {
			ErrorOut (GetLastError (), "CloseHandle on hOpen");
			return false;
		}
	}
	return true;
}
