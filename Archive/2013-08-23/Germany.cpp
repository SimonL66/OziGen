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

#include "Database.h"
#include "TileDatabase.h"
#include "WebMapServer.h"

#include "OSPro.h"
//#include "NGI.h"

#include "Germany.h"


#define printf visualprintf

extern CString g_strMapDatabaseFolder;
extern CString g_strMapScale;
extern CString g_strMapFlavour;


extern CCustomMap MyMap;


extern int abortProgram;

#define	Kompass_TILE_WIDTH_10K	250


int Kompass_read_png_10K(long northing, long easting, CTileDatabase * db, unsigned char * bm_ptr, long bitmap_width)
{
	// load file
	long tileSize;
	LPBYTE data = db->ReadTile(northing, easting, &tileSize);
	if (data == NULL) {
		// white tile = palette entry 255
		for (long ty=0; ty<Kompass_TILE_WIDTH_10K; ty++)
			memset(bm_ptr+ty*bitmap_width, 255, Kompass_TILE_WIDTH_10K);
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

bool Kompass_ReadTiles(long minx, long maxx, long miny, long maxy, CProgressWindow& wndProgress, 
						unsigned char * bitmap_palette, long & bitmap_palette_length, 
						unsigned char * & bitmap_memory , long & bitmap_memory_size, 
						long & bitmap_width, long & dest_bitmap_width,
						long & bitmap_height, long & dest_bitmap_height,
						int nFlavour, int nCountry, int nScale)
{
	CTileDatabase	db(MyMap.m_nDatum, MyMap.m_nMetresPerTile, MyMap.m_nMapScale, MyMap.m_nPixelsPerTile);

int iterator = GetTileIterator(nScale)*1000;

wndProgress.ResetProgressBar("Tile:", (maxy-miny)/iterator*(maxx-minx)/iterator);

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
				bitmap_width = MyMap.m_nTileWidth*((maxx-minx)/1000);
				bitmap_height = MyMap.m_nTileWidth*((maxy-miny)/1000);
				dest_bitmap_width = MyMap.m_nTileWidth*((maxx-minx-2000)/1000);
				dest_bitmap_height = MyMap.m_nTileWidth*((maxy-miny-2000)/1000);

				bitmap_memory_size = bitmap_width * bitmap_height;
				bitmap_memory = (BYTE *)malloc(bitmap_memory_size);

				if (bitmap_memory == NULL) {
					printf("Couldn't allocate %d bytes for bitmap\n", bitmap_memory_size);
					return false;
				}
				memset(bitmap_memory, 0xff, bitmap_memory_size);		// assume 0xff is white

/*
				if (bitmap_width > 32000)
					printf("Warning: width is %d pixels\n", bitmap_width);
				if (bitmap_height > 32000)
					printf("Warning: height is %d pixels\n", bitmap_height);
*/

				// get palette from database
				bitmap_palette_length = 4*256;
				memcpy(bitmap_palette, get_tracklog_palette(), bitmap_palette_length);
			}
// COMMON end
			// copy tile into bitmap area

			BYTE * bitmap_offset = bitmap_memory + ((maxy-1000-y)/1000)*MyMap.m_nTileWidth*bitmap_width+((x-minx)/1000)*MyMap.m_nTileWidth;

			for (int Yoffset=0; Yoffset < MyMap.m_nTilesPerKilometre; Yoffset ++) {
				for (int Xoffset=0; Xoffset < MyMap.m_nTilesPerKilometre; Xoffset ++) {
					if (MyMap.m_nTileBitDepth == 8) {
						read_png(y+MyMap.m_nMetresPerTile*Yoffset, 
								x+MyMap.m_nMetresPerTile*Xoffset, 
								&db, 
								bitmap_offset+(MyMap.m_nTilesPerKilometre-1-Yoffset)*MyMap.m_nPixelsPerTile*bitmap_width+MyMap.m_nPixelsPerTile*Xoffset,
								bitmap_width);
					} else {
						Kompass_read_png_10K(y+MyMap.m_nMetresPerTile*Yoffset, 
									 x+MyMap.m_nMetresPerTile*Xoffset, 
									 &db,
									 bitmap_offset+(MyMap.m_nTilesPerKilometre-1-Yoffset)*MyMap.m_nPixelsPerTile*bitmap_width+MyMap.m_nPixelsPerTile*Xoffset,
									 bitmap_width);
					}
				}
			}
		}
	}
	return true;
}