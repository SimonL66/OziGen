#include "stdafx.h"

//#include <direct.h>

#include <Wininet.h>

#include "OziGen.h"
//#include "OziGenDlg.h"
#include "ProgressWindow.h"
#include "func.h"

//#include "..\MapLib\map_conv.h"
#include "..\MapLib\png.h"
#include "..\MapLib\palette.h"
//#include "..\MapLib\CoordConv.h"

//#include "Database.h"
//#include "TileDatabase.h"
#include "WebMapServer.h"

#include "Spain.h"


//#define printf visualprintf

extern CString g_strMapDatabaseFolder;
extern CString g_strMapScale;
extern CString g_strMapFlavour;

extern int abortProgram;


extern CCustomMap MyMap;

#define	Spain_TILE_WIDTH_10K	200


int Spain_read_png_10K(long northing, long easting, CTileDatabase * db, unsigned char * bm_ptr, long bitmap_width)
{
	// load file
	long tileSize;
	LPBYTE data = db->ReadTile(northing, easting, &tileSize);
	if (data == NULL) {
		// white tile = palette entry 255
		for (long ty=0; ty<Spain_TILE_WIDTH_10K; ty++)
			memset(bm_ptr+ty*bitmap_width, 255, Spain_TILE_WIDTH_10K);
		return 1;		// couldn't load file
	}
	char strTilename[256];
//	easting_northing_to_OSref3(easting, northing, strTilename);
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
			if (width != Spain_TILE_WIDTH_10K || height != Spain_TILE_WIDTH_10K) {
				printf("Bad size (%s) (%d, %d)\n", strTilename, width, height);
				db->DeleteTile(northing, easting);
				return 1;
			}
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

void Spain_GetTile(HINTERNET hConnect, char * text, long northing, long easting, CTileDatabase * db, FILE * fp_log, BOOL bForce, int nMapScale)
{
char strRequest[1024];
	if (text == NULL)
		return;

	long nPixels = GetPixelsPerTile(nMapScale);

	if (!bForce && db->TileLoaded(northing, easting))
		return;

	CString strEPSG;
/*
	if (MyMap.m_nUTMZone == 28) strEPSG = "EPSG:32628";
	if (MyMap.m_nUTMZone == 29) strEPSG = "EPSG:32629";
	if (MyMap.m_nUTMZone == 30) strEPSG = "EPSG:32630";
	if (MyMap.m_nUTMZone == 31) strEPSG = "EPSG:32631";
*/
strEPSG = "EPSG:23030";
//strEPSG = "EPSG:32630";

	if (nMapScale == MAP_SCALE_50K) {

		// http://www.idee.es/wms/MTN-Raster/MTN-Raster?LAYERS=mtn_rasterizado&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&EXCEPTIONS=application/vnd.ogc.se_inimage&FORMAT=image/jpeg&SRS=EPSG:23030&BBOX=168000,4736000,169000,4737000&WIDTH=400&HEIGHT=400
//	EPSG:23030 ED50 / UTM zone 30N;	sprintf(strRequest,"wms/MTN-Raster/MTN-Raster?LAYERS=mtn_rasterizado&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&EXCEPTIONS=application/vnd.ogc.se_inimage&FORMAT=image/png&SRS=EPSG:23030&BBOX=%d,%d,%d,%d&WIDTH=400&HEIGHT=400",easting, northing, easting+1000, northing+1000);

		sprintf(strRequest,"wms/MTN-Raster/MTN-Raster?LAYERS=mtn_rasterizado&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&EXCEPTIONS=application/vnd.ogc.se_inimage&FORMAT=image/png&SRS=%s&BBOX=%d,%d,%d,%d&WIDTH=400&HEIGHT=400",
			strEPSG,
			easting, northing, easting+1000, northing+1000);
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


int Spain_LoadTiles(CTileDatabase* g_db, long minx, long miny, long maxx, long maxy, BOOL bForce, int nMapScale)
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
	for (long y=miny; y<maxy; y+=nMeters/*500*/) {
		for (long x=minx; x<maxx; x+=nMeters/*500*/) {
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
g_db->InitDatabase(y,x,MAP_DATUM_ES);
				if (hOpen == NULL) {
					if ( !(hOpen = InternetOpen ( "Sample",  LOCAL_INTERNET_ACCESS , NULL, 0, 0) ) ) {
						ErrorOut ( GetLastError(), "InternetOpen");
						return 0;
					}

					if ( !(hConnect = InternetConnect ( hOpen, "www.idee.es", INTERNET_INVALID_PORT_NUMBER, "",	"", INTERNET_SERVICE_HTTP, 0  , 0) ) ) {
						ErrorOut (GetLastError(), "InternetConnect");
						return 0;
					}
				}
				Spain_GetTile(hConnect, strTile, y, x, g_db, fp_log, bForce, nMapScale);

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


#define N_Spain_ZONES		55

bool Spain_InDatabase(long northing, long easting)
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

	for (int i=0; i < N_Spain_ZONES; i++) {
		if (strZone == zones[i].strOSZone) {
			return true;
		}
	}
*/
	return false;
}
/*
bool Spain_ReadTiles(long minx, long maxx, long miny, long maxy, CProgressWindow& wndProgress, 
						unsigned char * bitmap_palette, long & bitmap_palette_length, 
						unsigned char * & bitmap_memory , long & bitmap_memory_size, 
						long & bitmap_width, long & dest_bitmap_width,
						long & bitmap_height, long & dest_bitmap_height,
						int nMapScale)
{
AlignMapExtents(nMapScale, minx, miny, maxx, maxy);

long scale = GetTileDatabaseScale(nMapScale);
long height = GetTileWidth(nMapScale);
long width = GetTileWidth(nMapScale);

int iterator = GetTileIterator(nMapScale);

	CTileDatabase db(MAP_DATUM_DE,scale);

	long x,y;
	for (y=maxy-iterator; y>=miny; y -= iterator) {
		for (x=minx; x<maxx; x += iterator) {
			if (!wndProgress.ProgressBar()) return false;

			if (bitmap_memory == NULL) {
				// First square found so allocate memory for bitmap
				bitmap_width = width*(maxx-minx);
				bitmap_height = height*(maxy-miny);
				dest_bitmap_width = width*(maxx-minx-2);
				dest_bitmap_height = height*(maxy-miny-2);

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
			BYTE * bitmap_offset = bitmap_memory + (maxy-1-y)*height*bitmap_width+(x-minx)*width;

			if (nMapScale == MAP_SCALE_10K) {
				for (int Yoffset=0; Yoffset < 4; Yoffset ++) {
					for (int Xoffset=0; Xoffset < 4; Xoffset ++) {
						Spain_read_png_10K(y*1000+250*Yoffset, x*1000+250*Xoffset, &db, bitmap_offset+(3-Yoffset)*250*bitmap_width+250*Xoffset, bitmap_width);
					}
				}
			} else if (nMapScale == MAP_SCALE_50K) {
				Spain_read_png_10K(y*1000, x*1000, &db, bitmap_memory + (maxy-1-y)*height*bitmap_width+(x-minx)*width, bitmap_width);
			}
		}
	}
	return true;
}
*/