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

extern CCustomMap MyMap;

#define printf visualprintf

extern unsigned char mm_osgb_palette[1024];

extern int read_jpg(long northing, long easting, CTileDatabase * db, char * szFile, unsigned char * bm_ptr, long bitmap_width);
extern int read_png(long northing, long easting, CTileDatabase * db, unsigned char * bm_ptr, long bitmap_width);

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
						long & bitmap_height, long & dest_bitmap_height)
{
	int nDatum = MyMap.GetDatum();
	int nMeters = MyMap.GetMetresPerTile();

	int nMapScale = MyMap.GetLevelOfDetail();;
	int nPixles = MyMap.GetPixelsPerTile();

	CTileDatabase	db(nDatum, nMeters, nMapScale, nPixles);

	wndProgress.ResetProgressBar("Tile:", (maxy-miny)/nMeters*(maxx-minx)/nMeters);

	for (long y = maxy-nMeters; y >= miny; y -= nMeters) {

		for (long x=minx; x<maxx; x += nMeters) {

			if (!wndProgress.ProgressBar()) return false;

			if (bitmap_memory == NULL) { // COMMON start

				// First square found so allocate memory for bitmap
				// Needs to be double as now we're dividing by Meter per pixel which may not be rounded to the nearest factor of 10!
				bitmap_width = nPixles*((double)(maxx-minx)/nMeters);
				bitmap_height = nPixles*((double)(maxy-miny)/nMeters);
				dest_bitmap_width = nPixles*((maxx-minx-2/**nMeters*/)/nMeters);
				dest_bitmap_height = nPixles*((maxy-miny-2/**nMeters*/)/nMeters);

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
			if ((MyMap.GetProjection() == 3857) || (MyMap.GetProjection() == 900913)) {
				bitmap_offset = bitmap_memory + ((y-miny)/nMeters)*nPixles*bitmap_width+((x-minx)/nMeters)*nPixles;
			} else {
				bitmap_offset = bitmap_memory + ((maxy-nMeters-y)/nMeters)*nPixles*bitmap_width+((x-minx)/nMeters)*nPixles;
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

bool OSPro_ReadTiles_QCT(long y, long x, CTileDatabase * db, unsigned char * bitmap_offset, long bitmap_width)
{
	if (MyMap.GetTileGfxType() == PNG) {
		read_png(y, x, db, bitmap_offset, bitmap_width);
	} else {
		char tstring[256];
		sprintf(tstring,"Y=%d, X=%d", y,x);
//		read_jpg(y, x, db, tstring, bitmap_offset, bitmap_width);
		if (read_jpg(y, x, db, tstring, bitmap_offset, bitmap_width) == 1) {
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

/*
void ReadTiles()
{
	LPBYTE data = db->ReadTile(northing, easting, &tileSize);

	if (png)
	else if (jpeg)
	else if
		unknown

}
*/
