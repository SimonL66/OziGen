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
#include "Germany.h"

extern CCustomMap MyMap;

#define printf visualprintf


extern int read_jpg(long northing, long easting, CTileDatabase * db, char * szFile, unsigned char * bm_ptr, long bitmap_width);

// ToDo: To Make a database variable!
//#define	TILE_WIDTH	200

int read_png(long northing, long easting, CTileDatabase * db, unsigned char * bm_ptr, long bitmap_width)
{
	// load file
	long tileSize;
	LPBYTE data = db->ReadTile(northing, easting, &tileSize);
	if (data == NULL) {
		// white tile = palette entry 255
		for (long ty=0; ty< /*TILE_WIDTH*/ MyMap.GetPixelsPerTile(); ty++)
			memset(bm_ptr+ty*bitmap_width, 255, MyMap.GetPixelsPerTile());
		return 1;		// couldn't load file
	}
//	char strTilename[256];
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
	if (ncolours == 2) {
		int fred=1;
	}
	for (int i = 0; i<ncolours; i++)
		palette_convert[i] = RGB_to_tracklog_palette(png_palette[i].blue, png_palette[i].green, png_palette[i].red);

	long source_width = png_get_rowbytes(read_ptr, read_info_ptr);
	png_bytep row_buf0 = (png_bytep)png_malloc(read_ptr, source_width);

	if (read_info_ptr->pixel_depth == 8) {
		for (long y = 0; y < (long)height; y++) {
			png_read_rows(read_ptr, (png_bytepp)&row_buf0, (png_bytepp)NULL, 1);
			for (long x=0; x<(long)width; x++) {
				bm_ptr[y*bitmap_width + x] = palette_convert[row_buf0[x]];
			}
		}
	} else if (read_info_ptr->pixel_depth == 4) {
		// TODO: Are single bit PNGs little endian or big endian
		for (long y = 0; y < (long)height; y++) {
			png_read_rows(read_ptr, (png_bytepp)&row_buf0, (png_bytepp)NULL, 1);
			for (long x=0; x<(long)width; x+=2) {
				unsigned char ch = row_buf0[x/2];
				bm_ptr[y*bitmap_width + x+1] = palette_convert[ch&0x0f]; ch>>=4;
				bm_ptr[y*bitmap_width + x+0] = palette_convert[ch];
			}
		}
	} else if (read_info_ptr->pixel_depth == 1) {
		// TODO: Are single bit PNGs little endian or big endian
		for (long y = 0; y < (long)height; y++) {
			png_read_rows(read_ptr, (png_bytepp)&row_buf0, (png_bytepp)NULL, 1);
			for (long x=0; x<(long)width; x+=8) {
				unsigned char ch = row_buf0[x/8];
				bm_ptr[y*bitmap_width + x+7] = palette_convert[ch&1]; ch>>=1;
				bm_ptr[y*bitmap_width + x+6] = palette_convert[ch&1]; ch>>=1;
				bm_ptr[y*bitmap_width + x+5] = palette_convert[ch&1]; ch>>=1;
				bm_ptr[y*bitmap_width + x+4] = palette_convert[ch&1]; ch>>=1;
				bm_ptr[y*bitmap_width + x+3] = palette_convert[ch&1]; ch>>=1;
				bm_ptr[y*bitmap_width + x+2] = palette_convert[ch&1]; ch>>=1;
				bm_ptr[y*bitmap_width + x+1] = palette_convert[ch&1]; ch>>=1;
				bm_ptr[y*bitmap_width + x+0] = palette_convert[ch];
			}
		}
	} else {
		CString str;
		str.Format("Cannot handle a %d pixel depth", read_info_ptr->pixel_depth);
		AfxMessageBox(str);
	}
	png_free(read_ptr, row_buf0);

	png_destroy_read_struct(&read_ptr, &read_info_ptr, &end_info_ptr);
	fclose(fpin);

	return 0;
}

void OSPro_GetTile(HINTERNET hConnect, char * text, long northing, long easting, CTileDatabase * db, FILE * fp_log, BOOL bForce, int nMapFlavour, int nMapScale)
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

		} else if (db->m_mapscale == 1000000) {

			sprintf(strRequest, "osmapapi/ts?FORMAT=image%%2Fpng&KEY=DEMOAPI&URL=http%%3A%%2F%%2Fosopenspacewiki.ordnancesurvey.co.uk%%2Fwiki%%2Fextensions%%2Fhtmlets%%2FOSProCS.html&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&EXCEPTIONS=application%%2Fvnd.ogc.se_inimage&LAYERS=100&PRODUCT=MS&SRS=EPSG%%3A27700&"
								"BBOX=%d,%d,%d,%d&WIDTH=200&HEIGHT=200", easting, northing, easting+nMetres, northing-nMetres, nPixels, nPixels);

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
g_db->InitDatabase(y,x,MAP_DATUM_UK);
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
/*
			if (abortProgram) {
				y=miny;
				x=maxx;
			}
*/
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
						long & bitmap_height, long & dest_bitmap_height)
{
	int nDatum = MyMap.GetDatum();
	int nMeters = MyMap.GetMetresPerTile();
	int nMapScale = MyMap.GetMapScale();
	int nPixles = MyMap.GetPixelsPerTile();

	CTileDatabase	db(nDatum, nMeters, nMapScale, nPixles);

	wndProgress.ResetProgressBar("Tile:", (maxy-miny)/nMeters*(maxx-minx)/nMeters);

	for (long y = maxy-nMeters; y >= miny; y -= nMeters) {

		for (long x=minx; x<maxx; x += nMeters) {

			if (!wndProgress.ProgressBar()) return false;

			if (bitmap_memory == NULL) { // COMMON start

				// First square found so allocate memory for bitmap
				bitmap_width = nPixles*((maxx-minx)/nMeters);
				bitmap_height = nPixles*((maxy-miny)/nMeters);
				dest_bitmap_width = nPixles*((maxx-minx-2*nMeters)/nMeters);
				dest_bitmap_height = nPixles*((maxy-miny-2*nMeters)/nMeters);

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
			BYTE * bitmap_offset = bitmap_memory + ((maxy-nMeters-y)/nMeters)*nPixles*bitmap_width+((x-minx)/nMeters)*nPixles;

			if (MyMap.GetTileGfxType() == PNG) {
				if (MyMap.GetTileBitDepth() == 8) {
					read_png(y, x, &db, bitmap_offset, bitmap_width);
				} else {
					Kompass_read_png_10K(y, x, &db,bitmap_offset, bitmap_width);
				}
			} else {
				char tstring[256];
				read_jpg(y, x, &db, tstring, bitmap_offset, bitmap_width);
			}
		}
	}
	return bitmap_memory != NULL;
}

bool OSM_ReadTiles(long minx, long maxx, long miny, long maxy, CProgressWindow& wndProgress, 
						unsigned char * bitmap_palette, long & bitmap_palette_length, 
						unsigned char * & bitmap_memory , long & bitmap_memory_size, 
						long & bitmap_width, long & dest_bitmap_width,
						long & bitmap_height, long & dest_bitmap_height)
{
	int nDatum = MyMap.GetDatum();
	int nMeters = MyMap.GetMetresPerTile();
	int nMapScale = MyMap.GetMapScale();
	int nPixles = MyMap.GetPixelsPerTile();

	CTileDatabase	db(nDatum, nMeters, nMapScale, nPixles);

nMeters = 1;

	wndProgress.ResetProgressBar("Tile:", (maxy-miny)/nMeters*(maxx-minx)/nMeters);



	for (long y = maxy-nMeters; y >= miny; y -= nMeters) {

		for (long x=minx; x< maxx; x += nMeters) {

			if (!wndProgress.ProgressBar()) return false;

// COMMON start
			if (bitmap_memory == NULL) {

				// First square found so allocate memory for bitmap
				bitmap_width = nPixles*((maxx-minx)/nMeters);
				bitmap_height = nPixles*((maxy-miny)/nMeters);
				dest_bitmap_width = nPixles*((maxx-minx-2*nMeters)/nMeters);
				dest_bitmap_height = nPixles*((maxy-miny-2*nMeters)/nMeters);

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
			}
// COMMON end

			// copy tile into bitmap area
			BYTE * bitmap_offset = bitmap_memory + ((y-miny)/nMeters)*nPixles*bitmap_width+((x-minx)/nMeters)*nPixles;
/*
			if (MyMap.GetTileBitDepth() == 8) {
				read_png(y, x, &db, bitmap_offset, bitmap_width);
			} else {
				Kompass_read_png_10K(y, x, &db,bitmap_offset, bitmap_width);
			}
*/
			if (MyMap.GetTileGfxType() == PNG) {
				if (MyMap.GetTileBitDepth() == 8) {
					read_png(y, x, &db, bitmap_offset, bitmap_width);
				} else {
					Kompass_read_png_10K(y, x, &db,bitmap_offset, bitmap_width);
				}
			} else {
				char tstring[256];
				read_jpg(y, x, &db, tstring, bitmap_offset, bitmap_width);
			}

		}
	}
	return bitmap_memory != NULL;
}

extern unsigned char mm_osgb_palette[1024];

bool OSPro_ReadTiles_QCT(long y, long x, CTileDatabase * db, unsigned char * bitmap_offset, long bitmap_width)
{
/*
	if (MyMap.GetTileBitDepth() == 8) {
		read_png(y, x, db, bitmap_offset, bitmap_width);
	} else {
		Kompass_read_png_10K(y, x, db, bitmap_offset, bitmap_width);
	}
*/
	if (MyMap.GetTileGfxType() == PNG) {
		if (MyMap.GetTileBitDepth() == 8) {
			read_png(y, x, db, bitmap_offset, bitmap_width);
		} else {
			Kompass_read_png_10K(y, x, db,bitmap_offset, bitmap_width);
		}
	} else {
		char tstring[256];
		read_jpg(y, x, db, tstring, bitmap_offset, bitmap_width);
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