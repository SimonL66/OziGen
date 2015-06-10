#include "stdafx.h"

#include "OziGen.h"
#include "OziGenDlg.h"
#include "func.h"
#include "jpeg.h"
#include "GetOSPro.h"
#include "GetIGN.h"

#include "Resource.h"

#include "GFX.h"

#include "stdint.h"

#include "Database.h"

#include "jnx.h"

#include <math.h>
#include <list>
#include <gdiplus.h>

#include "..\MapLib\CoordConv.h"
#include "..\MapLib\mmmath.h"
#include "..\MapLib\png.h"
#include "..\MapLib\Palette.h"
#include "..\MapLib\map_conv.h"
#include "..\MapLib\License.h"
#include "..\MapLib\db_TL3.h"
#include "..\MapLib\zip.h"

#include "TracklogsDatabase.h"

#define printf visualprintf

CString g_strMapDatabaseFolder="C:\\OziGenDatabase";
CString g_strMapScale="25K";

extern unsigned char uncompressed[1500*1500];
extern long	uncompressed_length;


bool ReadTracklogsTile(long minx, long maxx, long miny, long maxy, CProgressWindow& wndProgress, 
						unsigned char * bitmap_palette, long & bitmap_palette_length, 
						unsigned char * & bitmap_memory , long & bitmap_memory_size, 
						long & bitmap_width, long & dest_bitmap_width,
						long & bitmap_height, long & dest_bitmap_height,
						bool bUseTL3)
{
	CTracklogsDatabase23 dbaseTL23(bUseTL3);

	long width, height;
	long square_width;
	long square_height;
	long x,y;
	for (y=maxy-1; y>=miny; y--) {
		for (x=minx; x<maxx; x++) {

			wndProgress.ProgressBar();
			DoEvents();

			if (wndProgress.m_Cancelled) return false;

// IF Tracklogs

			long id = x*10000+y;

			long palette_length;
			BYTE * palette;
			long raster_length;
			BYTE * raster;
			long revision;

			if (!dbaseTL23.GetTile(	id,									// retrieves tile id, or returns false
									&width, &height,
									&palette_length, &palette,
									&raster_length, &raster,
									&revision))
				continue;
// COMMON start
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

				square_width = width;
				square_height = height;

				if (bitmap_width > 32000)
					printf("Warning: width is %d pixels\n", bitmap_width);
				if (bitmap_height > 32000)
					printf("Warning: height is %d pixels\n", bitmap_height);

				// get palette from database
				memcpy(bitmap_palette, palette, palette_length);
				bitmap_palette_length = palette_length;
			}
// COMMON end

			if (square_width != width) {
				printf("Tile %d mismatch on width. %d instead of %d\n", id, width, square_width);
				continue;
			}
			if (square_height != height) {
				printf("Tile %d mismatch on height. %d instead of %d\n", id, height, square_height);
				continue;
			}
			if (compare_palettes(bitmap_palette, palette, palette_length) != 0) {
				printf("Tile %d has different palette to first tile - ignoring\n", id);
				continue;
			}
			if (bitmap_palette_length != palette_length) {
				printf("Tile %d mismatch on palette size. %d instead of %d\n", id, palette_length, bitmap_palette_length);
				continue;
			}

			// expand compressed data using zlib library
			int err;
			z_stream d_stream; /* decompression stream */

			d_stream.zalloc = (alloc_func)0;
			d_stream.zfree = (free_func)0;
			d_stream.opaque = (voidpf)0;

			d_stream.next_in  = raster;
			d_stream.avail_in = (uInt)raster_length;

			err = inflateInit(&d_stream);
			CHECK_ERR(err, "inflateInit");

			for (;;) {
				d_stream.next_out = uncompressed;            /* discard the output */
				d_stream.avail_out = (uInt)sizeof(uncompressed);
				err = inflate(&d_stream, Z_NO_FLUSH);
				if (err == Z_STREAM_END)
					break;
				CHECK_ERR(err, "large inflate");
			}

			err = inflateEnd(&d_stream);
			CHECK_ERR(err, "inflateEnd");

			uncompressed_length = d_stream.total_out;

			// copy tile into bitmap area
			BYTE * bitmap_offset = bitmap_memory + (maxy-1-y)*height*bitmap_width+(x-minx)*width;
			int tx, ty;
			for (ty=0; ty<height; ty++) {
				for (tx=0; tx<width; tx++) {
					bitmap_offset[ty*bitmap_width + tx] = uncompressed[(height-1-ty)*width + tx];
				}
// ELSE OS Pro
			}
		}
	}
	return true;
}

bool ReadOSProTile(long minx, long maxx, long miny, long maxy, CProgressWindow& wndProgress, 
						unsigned char * bitmap_palette, long & bitmap_palette_length, 
						unsigned char * & bitmap_memory , long & bitmap_memory_size, 
						long & bitmap_width, long & dest_bitmap_width,
						long & bitmap_height, long & dest_bitmap_height)
{
	// PNG writing variables
	CTileDatabase db;

	long width=400, height=400;
//	long square_width;
//	long square_height;
	long x,y;
	for (y=maxy-1; y>=miny; y--) {
		for (x=minx; x<maxx; x++) {

			wndProgress.ProgressBar();
			DoEvents();
			if (wndProgress.m_Cancelled) return false;

// COMMON start
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
			read_png(y*1000, x*1000, &db, bitmap_offset+200*bitmap_width, bitmap_width);
			read_png(y*1000, x*1000+500, &db, bitmap_offset+200*bitmap_width+200, bitmap_width);
			read_png(y*1000+500, x*1000, &db, bitmap_offset, bitmap_width);
			read_png(y*1000+500, x*1000+500, &db, bitmap_offset+200, bitmap_width);
		}
	}
	return true;
}

bool ReadIGNTile(long minx, long maxx, long miny, long maxy, CProgressWindow& wndProgress, 
						unsigned char * bitmap_palette, long & bitmap_palette_length, 
						unsigned char * & bitmap_memory , long & bitmap_memory_size, 
						long & bitmap_width, long & dest_bitmap_width,
						long & bitmap_height, long & dest_bitmap_height)
{
	// PNG writing variables
CIGNTileDatabase	db;

	long width=400, height=400;

	long x,y;
	for (y=maxy-1; y>=miny; y--) {
		for (x=minx; x<maxx; x++) {

			wndProgress.ProgressBar();
			DoEvents();
			if (wndProgress.m_Cancelled) return false;

// COMMON start
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
				memset(bitmap_memory, 0x00, bitmap_memory_size);		// assume 0x00 is white

				if (bitmap_width > 32000)
					printf("Warning: width is %d pixels\n", bitmap_width);
				if (bitmap_height > 32000)
					printf("Warning: height is %d pixels\n", bitmap_height);

				// get palette from database
				bitmap_palette_length = 4*256;
				memcpy(bitmap_palette, get_ign_palette(), bitmap_palette_length);
			}
// COMMON end
			// copy tile into bitmap area
			IGN_read_bmp(y*1000, x*1000, &db, bitmap_memory + (maxy-1-y)*height*bitmap_width+(x-minx)*width, bitmap_width);
		}
	}
	return true;
}

DatabaseType DetermineDatabaseType(int nRadioDatabase, int nRadioDatum)
{
	if (nRadioDatum == IDC_RADIO_OSGB) {
		if (nRadioDatabase == IDC_RADIO_DBASE_TRACKLOGS_2) return DB_OSGB_Tracklogs2;
		if (nRadioDatabase == IDC_RADIO_DBASE_TRACKLOGS_3) return DB_OSGB_Tracklogs3;
		if (nRadioDatabase == IDC_RADIO_DBASE_LOCAL) return DB_OSGB_Local;
	} else {
		if (nRadioDatum == IDC_RADIO_IGN) return DB_IGN_Local;
	}

	return DB_Unknown;
}