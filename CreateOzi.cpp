#include "stdafx.h"

#include "OziGen.h"
#include "OziGenDlg.h"

#include "ProgressWindow.h"
#include "Database.h"

#include "OSPro.h"
#include "CH1903.h"

#include "CreateOzi.h"

#include "func.h"
#include "math.h"

#include "..\MapLib\CoordConv.h"

#include "..\MapLib\png.h"
#include "..\MapLib\palette.h"

#include "CustomMap.h"
extern CCustomMap MyMap;


CString CreateOziPathName(CString strPath, CString strFile)
{
	CString strOzi;

	if (!strPath.IsEmpty())
		strOzi = strPath + "\\";
	strOzi += RemoveSuffix(strFile);
	strOzi += ".map";

	return strOzi;
}

bool DumpOziFile(CString strPath, CRect MyRect)
{
	CString strTemp = RemoveSuffix(strPath);

	CString strFile;
	int i = strTemp.ReverseFind('\\');
	strFile = strTemp.Mid(i+1);

	CString strOutput = strTemp + ".map";
	CString strPng = strFile + ".png";

	FILE * fp = fopen(strOutput, "w+");
	if (fp == NULL) {
//		printf("Couldn't create file %s\n", strOutput);
		return false;
	}

	// Header
	fprintf (fp,"OziExplorer Map Data File Version 2.2\n");
	fprintf (fp,"OS Great Britain 1:25.000 - %s\n", strFile);
	fprintf (fp,"%s\n", strPng);
	fprintf (fp,"1 ,Map Code,\n");
	fprintf (fp,"WGS 84, WGS 84,   0.0000,   0.0000,WGS 84\n");
	fprintf (fp,"Reserved 1\n");
	fprintf (fp,"Reserved 2\n");
	fprintf (fp,"Magnetic Variation,,,E\n");
	fprintf (fp,"Map Projection,(BNG) British National Grid,PolyCal,No,AutoCalOnly,No,BSBUseWPX,No\n");

	// Dump Points
	/*
		e.g.
		Point01,xy,  200,  128,in, deg,    ,        ,N,    ,        ,W, grid, HV,      01000,      00000,N
		Point02,xy,32400,  128,in, deg,    ,        ,N,    ,        ,W, grid, HW,      62000,      00000,N
		Point03,xy,32400,32328,in, deg,    ,        ,N,    ,        ,W, grid, NG,      62000,      39000,N
		Point04,xy,  200,32328,in, deg,    ,        ,N,    ,        ,W, grid, NF,      01000,      39000,N
		Point05,xy,     ,     ,in, deg,    ,        ,N,    ,        ,W, grid,   ,           ,           ,N
	*/
	struct {
		long e;
		long n;
	} MapBounds[4] = {
		{MyRect.left, MyRect.top},
		{MyRect.right, MyRect.top},
		{MyRect.right, MyRect.bottom},
		{MyRect.left, MyRect.bottom}
	};

	for (int i=0 ; i < 30; i++)
	{
		if (i < 4)
		{
			double dLat, dLon;
			easting_northing_to_latitude_longitude(MapBounds[i].e, MapBounds[i].n, &dLat, &dLon);
	
			char strOS[256];
			easting_northing_to_OSref5(MapBounds[i].e, MapBounds[i].n, strOS);

			CString str, strGrid, strEasting, strNorthing;
			str = strOS;
			strGrid = str.Left(2);
			strEasting = str.Mid(3,5);
			strNorthing = str.Mid(9,5);

			fprintf (fp,"Point%02d,xy,%5d,%5d,in, deg,%4d,%8.5f,%c,%4d,%8.5f,%c, grid,%3s,%11s,%11s,N\n",
						i+1,labs(MapBounds[i].e-MapBounds[0].e)*2/5,labs(MapBounds[i].n-MapBounds[0].n)*2/5,
						(long)fabs(dLat), 60*fmod(fabs(dLat), 1.0), dLat < 0 ? 'S' : 'N',
						(long)fabs(dLon), 60*fmod(fabs(dLon), 1.0), dLon < 0 ? 'W' : 'E',
						strGrid,strEasting,strNorthing);
		} 
		else
		{
			fprintf (fp,"Point%02d,xy,     ,     ,in, deg,    ,        ,N,    ,        ,W, grid,   ,           ,           ,N\n",i+1);
		}
	}

	// Dump Projection details etc...
	fprintf (fp,"Projection Setup,,,,,,,,,,\n");
	fprintf (fp,"Map Feature = MF ; Map Comment = MC     These follow if they exist\n");
	fprintf (fp,"Track File = TF      These follow if they exist\n");
	fprintf (fp,"Moving Map Parameters = MM?    These follow if they exist\n");
	fprintf (fp,"MM0,Yes\n");

	// Dump MMPXY
	fprintf (fp,"MMPNUM,%d\n", 4);
	for (int i=0 ; i < 4; i++)
	{
		fprintf (fp,"MMPXY,%d,%d,%d\n",i+1, labs(MapBounds[i].e-MapBounds[0].e)*2/5,labs(MapBounds[i].n-MapBounds[0].n)*2/5);
	}

	// Dump MMPLL
	for (int i=0 ; i < 4; i++)
	{
		double dLat, dLon;
		easting_northing_to_latitude_longitude(MapBounds[i].e, MapBounds[i].n, &dLat, &dLon);
		fprintf (fp,"MMPLL,%d,  %.8f, %.8f\n",i+1, dLon, dLat);
	}

	// Footer
	fprintf (fp,"MM1B,2.500000\n");
	fprintf (fp,"MOP,Map Open Position,0,0\n");
	
	// Image Size
	fprintf (fp,"IWH,Map Image Width/Height,%d,%d\n",labs(MyRect.Width())*2/5,labs(MyRect.Height())*2/5);

	fclose(fp);
	return true;
}

unsigned char tracklog_palette[][4];

bool Create_OZI_PNG(LPCSTR mapname, long minx, long maxx, long miny, long maxy, int nDatabase)
{
	CProgressWindow wndProgress;
	wndProgress.Initialize();

	if (minx > maxx) {
		long t = minx;
		minx = maxx;
		maxx = t;
	}
	if (miny > maxy) {
		long t = miny;
		miny = maxy;
		maxy = t;
	}

	unsigned char * bitmap_memory = NULL;

	long bitmap_width, dest_bitmap_width;
	long bitmap_height, dest_bitmap_height;

	long bitmap_memory_size;

	unsigned char bitmap_palette[1024];
	long bitmap_palette_length;

	wndProgress.ResetProgressBar("Tile:", (maxy-miny)*(maxx-minx));

	bool successful=false;

	if (nDatabase == DBASE_LOCAL) {

			successful = OSPro_ReadTiles(minx, maxx, miny, maxy, wndProgress, 
										bitmap_palette, bitmap_palette_length, 
										bitmap_memory , bitmap_memory_size, 
										bitmap_width, dest_bitmap_width,
										bitmap_height, dest_bitmap_height);

	} else {

		bool b_use_TL3 = nDatabase == DBASE_TRACKLOGS_3;

		successful = ReadTracklogsTile(minx, maxx, miny, maxy, wndProgress, 
										bitmap_palette, bitmap_palette_length, 
										bitmap_memory , bitmap_memory_size, 
										bitmap_width, dest_bitmap_width,
										bitmap_height, dest_bitmap_height,
										b_use_TL3);
	}

	if (!successful) {
		if (bitmap_memory)
			free(bitmap_memory);
		return false;
	}
/*
	if (bitmap_memory == NULL) {
		printf("No images to process\n");
		return false;
	}
	
	
	if (bitmap_memory == NULL) {
		printf("Couldn't allocate %d bytes of memory\n");
		return false;
	}
*/
char tstring[256];

	// Create PNG header and write out palette
	sprintf(tstring, "%s.png", mapname);
	FILE * fp_png = fopen(tstring, "wb");
	if (fp_png == NULL) {
		printf("Couldn't open %s\n", tstring);
		return false;
	}
	
	// Create and initialize the png_struct with the desired error handler functions.
	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		fclose(fp_png);
		return false;
	}
	
	/* Allocate/initialize the image information data.  REQUIRED */
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		fclose(fp_png);
		return false;
	}
	
	/* Set error handling.  REQUIRED */
	if (setjmp(png_jmpbuf(png_ptr))) {
		/* If we get here, we had a problem reading the file */
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(fp_png);
		return false;
	}
	/* set up the output control if you are using standard C streams */
	png_init_io(png_ptr, fp_png);
	
	/* Set the image information here.  Width and height are up to 2^31,
	 * bit_depth is one of 1, 2, 4, 8, or 16, but valid values also depend on
	 * the color_type selected. color_type is one of PNG_COLOR_TYPE_GRAY,
	 * PNG_COLOR_TYPE_GRAY_ALPHA, PNG_COLOR_TYPE_PALETTE, PNG_COLOR_TYPE_RGB,
	 * or PNG_COLOR_TYPE_RGB_ALPHA.  interlace is either PNG_INTERLACE_NONE or
	 * PNG_INTERLACE_ADAM7, and the compression_type and filter_type MUST
	 * currently be PNG_COMPRESSION_TYPE_BASE and PNG_FILTER_TYPE_BASE. REQUIRED
	 */
	png_set_IHDR(png_ptr, info_ptr, bitmap_width, bitmap_height, 8, PNG_COLOR_TYPE_PALETTE,
				PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	/* set the palette if there is one.  REQUIRED for indexed-color images */
	png_colorp png_palette = (png_colorp)png_malloc(png_ptr, PNG_MAX_PALETTE_LENGTH * sizeof (png_color));

	int i;
	BYTE * pal_ptr = (BYTE *)png_palette;
	BYTE * palette = (BYTE*)tracklog_palette;
	for (i=0; i<256; i++) {
		*pal_ptr++ = palette[i*4 + 2];
		*pal_ptr++ = palette[i*4 + 1];
		*pal_ptr++ = palette[i*4 + 0];
	}

	/* ... set palette colors ... */
	png_set_PLTE(png_ptr, info_ptr, png_palette, PNG_MAX_PALETTE_LENGTH);
	/* You must not free palette here, because png_set_PLTE only makes a link to
	 * the palette that you malloced.  Wait until you are about to destroy
	 *the png structure. */
	
	/* Write the file header information.  REQUIRED */
	png_write_info(png_ptr, info_ptr);

	// write row of PNG to file
	for (int x=0; x<bitmap_height; x++)
		png_write_row(png_ptr, bitmap_memory + x*bitmap_width);
/*
	for (int y=maxy; y>miny; y-=500) {
		memset(bitmap_memory, 0xff, bitmap_size);
		for (int x=minx; x<maxx; x+=500) {
			read_png(y-500, x, &g_db, bitmap_memory+(x-minx)/500*TILE_WIDTH, bitmap_width);
		}


		// write row of PNG to file
		for (int x=0; x<TILE_WIDTH; x++)
			png_write_row(png_ptr, bitmap_memory + x*bitmap_width);
	}
*/
	/* It is REQUIRED to call this to finish writing the rest of the file */
	png_write_end(png_ptr, info_ptr);

	png_free(png_ptr, png_palette);
	
	/* clean up after the write, and free any memory allocated */
	png_destroy_write_struct(&png_ptr, &info_ptr);

	/* close the file */
	fclose(fp_png);

//	dumpjpr_file(mapname, minx, miny, maxx, maxy, (maxx-minx)/500 * TILE_WIDTH, (maxy-miny)/500*TILE_WIDTH, NULL);
	free(bitmap_memory);

	return true;
}

bool Create_OZI_PNG_CH1903(LPCSTR mapname, CRect BBox)
{
	CProgressWindow wndProgress;
	wndProgress.Initialize();

	unsigned char * bitmap_memory = NULL;

	long bitmap_width, dest_bitmap_width;
	long bitmap_height, dest_bitmap_height;

	long bitmap_memory_size;

	unsigned char bitmap_palette[1024];
	long bitmap_palette_length;

	wndProgress.ResetProgressBar("Tile:", (BBox.top-BBox.bottom)*(BBox.right-BBox.left));

	bool successful=false;

	successful = ReadTiles_CH1903(BBox,
								wndProgress, 
								bitmap_palette, bitmap_palette_length, 
								bitmap_memory , bitmap_memory_size, 
								bitmap_width, dest_bitmap_width,
								bitmap_height, dest_bitmap_height);

	if (!successful) {
		if (bitmap_memory)
			free(bitmap_memory);
		return false;
	}

char tstring[256];

	// Create PNG header and write out palette
	sprintf(tstring, "%s.png", mapname);
	FILE * fp_png = fopen(tstring, "wb");
	if (fp_png == NULL) {
		printf("Couldn't open %s\n", tstring);
		return false;
	}
	
	// Create and initialize the png_struct with the desired error handler functions.
	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		fclose(fp_png);
		return false;
	}
	
	/* Allocate/initialize the image information data.  REQUIRED */
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		fclose(fp_png);
		return false;
	}
	
	/* Set error handling.  REQUIRED */
	if (setjmp(png_jmpbuf(png_ptr))) {
		/* If we get here, we had a problem reading the file */
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(fp_png);
		return false;
	}
	/* set up the output control if you are using standard C streams */
	png_init_io(png_ptr, fp_png);
	
	/* Set the image information here.  Width and height are up to 2^31,
	 * bit_depth is one of 1, 2, 4, 8, or 16, but valid values also depend on
	 * the color_type selected. color_type is one of PNG_COLOR_TYPE_GRAY,
	 * PNG_COLOR_TYPE_GRAY_ALPHA, PNG_COLOR_TYPE_PALETTE, PNG_COLOR_TYPE_RGB,
	 * or PNG_COLOR_TYPE_RGB_ALPHA.  interlace is either PNG_INTERLACE_NONE or
	 * PNG_INTERLACE_ADAM7, and the compression_type and filter_type MUST
	 * currently be PNG_COMPRESSION_TYPE_BASE and PNG_FILTER_TYPE_BASE. REQUIRED
	 */
	png_set_IHDR(png_ptr, info_ptr, bitmap_width, bitmap_height, 8, PNG_COLOR_TYPE_PALETTE,
				PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	/* set the palette if there is one.  REQUIRED for indexed-color images */
	png_colorp png_palette = (png_colorp)png_malloc(png_ptr, PNG_MAX_PALETTE_LENGTH * sizeof (png_color));

	int i;
	BYTE * pal_ptr = (BYTE *)png_palette;
	BYTE * palette = (BYTE*)tracklog_palette;
	for (i=0; i<256; i++) {
		*pal_ptr++ = palette[i*4 + 2];
		*pal_ptr++ = palette[i*4 + 1];
		*pal_ptr++ = palette[i*4 + 0];
	}

	/* ... set palette colors ... */
	png_set_PLTE(png_ptr, info_ptr, png_palette, PNG_MAX_PALETTE_LENGTH);
	/* You must not free palette here, because png_set_PLTE only makes a link to
	 * the palette that you malloced.  Wait until you are about to destroy
	 *the png structure. */
	
	/* Write the file header information.  REQUIRED */
	png_write_info(png_ptr, info_ptr);

	// write row of PNG to file
	for (int x=0; x<bitmap_height; x++)
		png_write_row(png_ptr, bitmap_memory + x*bitmap_width);
/*
	for (int y=maxy; y>miny; y-=500) {
		memset(bitmap_memory, 0xff, bitmap_size);
		for (int x=minx; x<maxx; x+=500) {
			read_png(y-500, x, &g_db, bitmap_memory+(x-minx)/500*TILE_WIDTH, bitmap_width);
		}


		// write row of PNG to file
		for (int x=0; x<TILE_WIDTH; x++)
			png_write_row(png_ptr, bitmap_memory + x*bitmap_width);
	}
*/
	/* It is REQUIRED to call this to finish writing the rest of the file */
	png_write_end(png_ptr, info_ptr);

	png_free(png_ptr, png_palette);
	
	/* clean up after the write, and free any memory allocated */
	png_destroy_write_struct(&png_ptr, &info_ptr);

	/* close the file */
	fclose(fp_png);

//	dumpjpr_file(mapname, minx, miny, maxx, maxy, (maxx-minx)/500 * TILE_WIDTH, (maxy-miny)/500*TILE_WIDTH, NULL);
	free(bitmap_memory);

	return true;
}
