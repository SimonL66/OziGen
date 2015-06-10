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

#include "Germany.h"

#include "CustomMap.h"

extern CCustomMap MyMap;

#define printf visualprintf

extern int abortProgram;

//#define	Kompass_TILE_WIDTH_10K	250


int Kompass_read_png_10K(long northing, long easting, CTileDatabase * db, unsigned char * bm_ptr, long bitmap_width)
{
	// load file
	long tileSize;
	LPBYTE data = db->ReadTile(northing, easting, &tileSize);
	if (data == NULL) {
		// white tile = palette entry 255
		for (long ty = 0; ty < MyMap.GetPixelsPerTile(); ty++)
			memset(bm_ptr+ty*bitmap_width, 255, MyMap.GetPixelsPerTile());
		return 1;		// couldn't load file
	}

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
			if (width != Kompass_TILE_WIDTH_10K || height != Kompass_TILE_WIDTH_10K) {
				printf("Bad size (%s) (%d, %d)\n", strTilename, width, height);
				db->DeleteTile(northing, easting);
				return 1;

			}
*/
		}
	}
	unsigned long passes = png_set_interlace_handling(read_ptr);
	png_read_update_info(read_ptr, read_info_ptr);

	long source_width = png_get_rowbytes(read_ptr, read_info_ptr);
	png_bytepp row_bufs = (png_bytepp)png_malloc(read_ptr, sizeof(png_bytep)*height);
	DWORD y;
	for (y=0; y<height; y++)
		row_bufs[y] = (png_bytep)png_malloc(read_ptr, source_width);

	if (read_info_ptr->pixel_depth == 24) {
		// Slow Method: Convert 24 bit png to 8 bit png. 
		for (y=0; y<passes; y++)
			png_read_rows(read_ptr, row_bufs, (png_bytepp)NULL, height);
		for (y = 0; y < height; y++) {
			for (DWORD x=0; x<width; x++) {
				bm_ptr[y*bitmap_width + x] = RGB_to_tracklog_palette(row_bufs[y][x*3+2], row_bufs[y][x*3+1], row_bufs[y][x*3]);
			}
		}
	} else if (read_info_ptr->pixel_depth == 32) {
		// Slow Method: Convert 32 bit png to 8 bit png. 
		for (y=0; y<passes; y++)
			png_read_rows(read_ptr, row_bufs, (png_bytepp)NULL, height);
		for (y = 0; y < height; y++) {
			for (DWORD x=0; x<width; x++) {
				bm_ptr[y*bitmap_width + x] = RGB_to_tracklog_palette(row_bufs[y][x*4+2], row_bufs[y][x*4+1], row_bufs[y][x*4]);
			}
		}
	}
	for (y=0; y<height; y++)
		png_free(read_ptr, row_bufs[y]);
	png_free(read_ptr, row_bufs);

	png_destroy_read_struct(&read_ptr, &read_info_ptr, &end_info_ptr);
	fclose(fpin);

	return 0;
}

#define N_KOMPASS_ZONES		55

bool Kompass_InDatabase(long northing, long easting)
{
/*
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

	for (int i=0; i < N_KOMPASS_ZONES; i++) {
		if (strZone == zones[i].strOSZone) {
			return true;
		}
	}
*/
	return false;
}

int Kompass_LoadTiles(CTileDatabase* g_db, long minx, long miny, long maxx, long maxy, BOOL bForce, int nMapScale)
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
				g_db->InitDatabase(y,x,MyMap.GetDatum());
				if (hOpen == NULL) {
					if ( !(hOpen = InternetOpen ( "Sample",  LOCAL_INTERNET_ACCESS , NULL, 0, 0) ) ) {
						ErrorOut ( GetLastError(), "InternetOpen");
						return 0;
					}

					if ( !(hConnect = InternetConnect ( hOpen, "ec2.cdn.ecmaps.de", 80, "",	"", INTERNET_SERVICE_HTTP, 0  , 0) ) ) {
						ErrorOut (GetLastError(), "InternetConnect");
						return 0;
					}
				}
				fprintf(fp_log, "Tile: [%05d,%05d] loading...\n", x,y);
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

void Kompass_GetTile(HINTERNET hConnect, char * text, long northing, long easting, CTileDatabase * db, FILE * fp_log, BOOL bForce, int nMapScale)
{
	char strRequest[1024];
	if (text == NULL)
		return;

	int nPixels = db->m_nPixelsPerTile;
	int nDetail = db->m_mapscale;
	int nMetres = db->m_nMetresPerTile;

	northing = northing/nMetres*nMetres;
	easting = easting/nMetres*nMetres;

	if (!bForce && db->TileLoaded(northing, easting))
		return;


//	http://ec2.cdn.ecmaps.de/WmsGateway.ashx.jpg?Experience=kompass&MapStyle=KOMPASS%20Touristik&TileX=17018&TileY=10975&ZoomLevel=15

	sprintf (strRequest,"%s","WmsGateway.ashx.png?Experience=kompass&MapStyle=KOMPASS%20Touristik&TileX=17018&TileY=10975&ZoomLevel=15");

//	fprintf(fp_log, "Tile: [] loading %s\n", text);
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
