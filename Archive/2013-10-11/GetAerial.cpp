#include "stdafx.h"

#include <direct.h>

#include "TileDatabase.h"
#include "WebMapServer.h"

#include "..\MapLib\CoordConv.h"
#include "..\MapLib\map_conv.h"
#include "..\MapLib\png.h"
#include "..\MapLib\palette.h"

#include "WebMapServer.h"

#include "ProgressWindow.h"

#include "CustomMap.h"
extern CCustomMap MyMap;

extern int abortProgram;

#define printf visualprintf

unsigned char ap25_nz_qct_palette[1024] = {
	0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x01, 0xFF, 0xFF, 0x00, 0x77, 0x9D, 0x59, 0x00, 
	0x28, 0x27, 0xE0, 0x00, 0x94, 0xAE, 0xD5, 0x00, 0x3A, 0x36, 0x67, 0x00, 0xD0, 0xDF, 0xA9, 0x00, 
	0x4A, 0x76, 0xA3, 0x00, 0x2F, 0x6A, 0x17, 0x00, 0x5A, 0xC9, 0xB3, 0x00, 0xB7, 0xD7, 0x5D, 0x00, 
	0x36, 0x89, 0x59, 0x00, 0xA8, 0x94, 0x92, 0x00, 0xC8, 0xD3, 0xE6, 0x00, 0x1E, 0x27, 0x2A, 0x00, 
	0x68, 0x6A, 0x71, 0x00, 0x7D, 0x6C, 0xC8, 0x00, 0x8D, 0xCA, 0x9C, 0x00, 0x45, 0x3F, 0x9D, 0x00, 
	0x5A, 0xAC, 0x8C, 0x00, 0x56, 0x80, 0x3B, 0x00, 0x45, 0x3E, 0x31, 0x00, 0xD6, 0xAD, 0xC7, 0x00, 
	0x73, 0x89, 0x98, 0x00, 0xE3, 0xEA, 0xD4, 0x00, 0x2E, 0x5A, 0x47, 0x00, 0x27, 0x5A, 0x82, 0x00, 
	0xAC, 0xB0, 0xB2, 0x00, 0x86, 0xCE, 0xCA, 0x00, 0x67, 0x95, 0xB5, 0x00, 0x15, 0x3B, 0x62, 0x00, 
	0x8A, 0x7D, 0x77, 0x00, 0xBB, 0xAA, 0xE7, 0x00, 0xA9, 0xCE, 0xD3, 0x00, 0xD6, 0xF4, 0xF4, 0x00, 
	0x48, 0x71, 0x7B, 0x00, 0x6B, 0x5D, 0xA5, 0x00, 0x4B, 0x67, 0x55, 0x00, 0x16, 0x4B, 0x16, 0x00, 
	0x84, 0xB9, 0x7F, 0x00, 0x30, 0x85, 0x37, 0x00, 0x64, 0x8B, 0x70, 0x00, 0x86, 0xA4, 0xAD, 0x00, 
	0x25, 0x15, 0x17, 0x00, 0x30, 0x28, 0x3B, 0x00, 0x60, 0x94, 0x43, 0x00, 0x63, 0xE0, 0xD3, 0x00, 
	0x65, 0x57, 0x56, 0x00, 0xC8, 0xE8, 0xD6, 0x00, 0xEA, 0xD1, 0xD1, 0x00, 0xA9, 0xB1, 0x95, 0x00, 
	0x46, 0x8D, 0x8F, 0x00, 0x4E, 0x62, 0x94, 0x00, 0x57, 0x6C, 0xC0, 0x00, 0x41, 0x5C, 0x1A, 0x00, 
	0x1C, 0x26, 0x4C, 0x00, 0xB7, 0xE0, 0xE9, 0x00, 0x68, 0x73, 0x56, 0x00, 0x3E, 0x55, 0x64, 0x00, 
	0x7D, 0xB9, 0xB2, 0x00, 0x43, 0x5A, 0x3C, 0x00, 0x0E, 0x13, 0x14, 0x00, 0x18, 0x49, 0x33, 0x00, 
	0xC5, 0xCF, 0xCB, 0x00, 0xB9, 0xCC, 0xBA, 0x00, 0x64, 0x7B, 0xD0, 0x00, 0xAA, 0x96, 0xAF, 0x00, 
	0x81, 0x9B, 0xCA, 0x00, 0x61, 0x76, 0x8B, 0x00, 0x5E, 0xA9, 0x63, 0x00, 0x4F, 0x94, 0x6E, 0x00, 
	0x61, 0xAD, 0xAC, 0x00, 0x40, 0x41, 0x7F, 0x00, 0x76, 0xA2, 0x96, 0x00, 0x63, 0x7B, 0xAE, 0x00, 
	0x81, 0xBC, 0x65, 0x00, 0x6A, 0x55, 0x8E, 0x00, 0x8A, 0x85, 0xAA, 0x00, 0xA6, 0xB4, 0xCC, 0x00, 
	0x9A, 0xC3, 0xB6, 0x00, 0x47, 0x7D, 0x48, 0x00, 0x21, 0x28, 0x73, 0x00, 0x26, 0x70, 0x6F, 0x00, 
	0x31, 0x4A, 0x1C, 0x00, 0x42, 0x48, 0x52, 0x00, 0x38, 0x2E, 0x26, 0x00, 0xF8, 0xE7, 0xF3, 0x00, 
	0x8A, 0x7C, 0x90, 0x00, 0x89, 0x98, 0x89, 0x00, 0x25, 0x69, 0x34, 0x00, 0xDB, 0xE9, 0xEB, 0x00, 
	0xDE, 0xD5, 0xBF, 0x00, 0x54, 0x75, 0x68, 0x00, 0x3A, 0x69, 0x32, 0x00, 0x71, 0xA6, 0x72, 0x00, 
	0x34, 0x44, 0x3D, 0x00, 0xF7, 0xF4, 0xE7, 0x00, 0x2B, 0x39, 0x51, 0x00, 0xCA, 0xB9, 0xB6, 0x00, 
	0x6B, 0x94, 0x8A, 0x00, 0x8A, 0xAE, 0x95, 0x00, 0x20, 0x16, 0x2B, 0x00, 0x58, 0x4E, 0x5B, 0x00, 
	0x2B, 0x58, 0x2B, 0x00, 0x16, 0x0C, 0x0D, 0x00, 0x90, 0xB7, 0xCB, 0x00, 0x28, 0x3C, 0x2D, 0x00, 
	0x4C, 0x5B, 0xAC, 0x00, 0x59, 0x62, 0x7A, 0x00, 0x26, 0x60, 0x57, 0x00, 0xB8, 0xC0, 0xDA, 0x00, 
	0x3F, 0x68, 0x69, 0x00, 0x34, 0x46, 0x98, 0x00, 0x63, 0xCA, 0xC4, 0x00, 0x2A, 0x3E, 0x7B, 0x00, 
	0x7A, 0x81, 0x84, 0x00, 0x61, 0x8A, 0x56, 0x00, 0x58, 0x65, 0x46, 0x00, 0x75, 0x93, 0xAB, 0x00, 
	0x57, 0x8B, 0x8C, 0x00, 0x1E, 0x2C, 0x17, 0x00, 0x76, 0xBD, 0xA2, 0x00, 0x37, 0x68, 0x4D, 0x00, 
	0x58, 0x46, 0x45, 0x00, 0x47, 0x36, 0x48, 0x00, 0x40, 0x58, 0x84, 0x00, 0x26, 0x47, 0x48, 0x00, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


//#define	TILE_WIDTH	250

unsigned char * colour_convert;
#define	COLOUR_UNKNOWN	0xFE

unsigned char get_colour(unsigned char * bitmap)
{
long x_value;

	long r = bitmap[0];
	long g = bitmap[1];
	long b = bitmap[2];
	x_value = r + (g<<8) + (b<<16);
	if (colour_convert[x_value] == COLOUR_UNKNOWN)
		colour_convert[x_value] = RGBtoPalette(get_tracklog_palette(), 256, r, g, b); // Palette was AP25!
	return colour_convert[x_value];
}

int read_jpg(long northing, long easting, CTileDatabase * db, char * szFile, unsigned char * bm_ptr, long bitmap_width)
{
int TileWidth = MyMap.GetPixelsPerTile();

	// load file
	long tileSize;
	LPBYTE data = db->ReadTile(northing, easting, &tileSize);
	if (data == NULL) {
		return 1;		// couldn't load file
	}

	// alloc memory based on file size
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, tileSize);
	LPVOID pvData = GlobalLock(hGlobal);
	memcpy(pvData, data, tileSize);
	free(data);
	// read file and store in global memory
	GlobalUnlock(hGlobal);
	
	LPSTREAM pstm = NULL;

	// create IStream* from global memory
	HRESULT hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pstm);
	if (hr != S_OK || pstm == NULL) {
		printf("Error creating IStream for (%d %d)\n", szFile);
		return 1;
	}

	// Create IPicture from image file
	LPPICTURE pPicture;
	hr = ::OleLoadPicture(pstm, tileSize, FALSE, IID_IPicture, (LPVOID*)&pPicture);
	pstm->Release();	// free memory allocated in hGlobal

	if (hr != S_OK || pPicture == NULL) {
		printf("Couldn't load picture %s\n", szFile);
		return 1;
	}


	// get the bitmap handle
	HBITMAP hBitmap;
	if (S_OK != pPicture->get_Handle((OLE_HANDLE FAR *)&hBitmap)) {
		printf("Couldn't get bitmap handle from picture (%s)\n", szFile);
		return 1;
	}

	BITMAP	bmp;
	// Retrieve bitmap's color format, width, and height
	if (!GetObject(hBitmap, sizeof(BITMAP), (LPVOID)&bmp)) {
		printf("Couldn't retrieve format of bitmap (%s)\n", szFile);
		pPicture->Release();
		return 1;
	}
	if (bmp.bmBitsPixel != 24) {
		printf("Not 24 bit bitmap (%s)\n", szFile);
		pPicture->Release();
		return 1;
	}

	if (bmp.bmWidth != TileWidth || bmp.bmHeight != TileWidth) {
		printf("Bad size (%s) (%d, %d)\n", szFile, bmp.bmWidth, bmp.bmHeight);
		pPicture->Release();
		return 1;
	}

	int y,x;
	for (y=0; y < TileWidth; y++) {
		for (x=0; x<TileWidth; x++) {
			bm_ptr[x+(TileWidth-y-1)*bitmap_width] = get_colour((unsigned char*)bmp.bmBits+y*bmp.bmWidthBytes+x*3);
		}
	}

	pPicture->Release();
	return 0;
}


// CTileDatabase	g_db;



void Create_PNG_JPR_from_JPEG(CTileDatabase* g_db, LPCSTR mapname, long minx, long maxx, long miny, long maxy)
{
int nMeters = MyMap.GetMetresPerTile();
int TileWidth = MyMap.GetPixelsPerTile();

char tstring[256];
long bitmap_width = TileWidth*(maxx-minx)/500;
long bitmap_size = bitmap_width*TileWidth;
unsigned char * bitmap_memory = (unsigned char *)malloc(bitmap_size);

	if (bitmap_memory == NULL) {
		printf("Couldn't allocate %d bytes of memory\n");
		return;
	}


	// Create PNG header and write out palette
	sprintf(tstring, "%s.png", mapname);
	FILE * fp_png = fopen(tstring, "wb");
	if (fp_png == NULL) {
		printf("Couldn't open %s\n", tstring);
		return;
	}
	
	// Create and initialize the png_struct with the desired error handler functions.
	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		fclose(fp_png);
		return;
	}
	
	// Allocate/initialize the image information data.  REQUIRED.
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		fclose(fp_png);
		exit(0);
	}
	
	// Set error handling.  REQUIRED.
	if (setjmp(png_jmpbuf(png_ptr))) {
		// If we get here, we had a problem reading the file.
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(fp_png);
		exit(0);
	}
	// set up the output control if you are using standard C streams.
	png_init_io(png_ptr, fp_png);
	
	// Set the image information here.  Width and height are up to 2^31,
	// bit_depth is one of 1, 2, 4, 8, or 16, but valid values also depend on
	// the color_type selected. color_type is one of PNG_COLOR_TYPE_GRAY,
	// PNG_COLOR_TYPE_GRAY_ALPHA, PNG_COLOR_TYPE_PALETTE, PNG_COLOR_TYPE_RGB,
	// or PNG_COLOR_TYPE_RGB_ALPHA.  interlace is either PNG_INTERLACE_NONE or
	// PNG_INTERLACE_ADAM7, and the compression_type and filter_type MUST
	// currently be PNG_COMPRESSION_TYPE_BASE and PNG_FILTER_TYPE_BASE. REQUIRED.
	png_set_IHDR(png_ptr, info_ptr, TileWidth*(maxx-minx)/500, TileWidth*((maxy-miny)/500), 8, PNG_COLOR_TYPE_PALETTE,
				PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	// set the palette if there is one.  REQUIRED for indexed-color images.
	png_colorp png_palette = (png_colorp)png_malloc(png_ptr, PNG_MAX_PALETTE_LENGTH * sizeof (png_color));

	int i;
	BYTE * pal_ptr = (BYTE *)png_palette;
	BYTE * palette = ap25_nz_qct_palette;
	for (i=0; i<256; i++) {
		*pal_ptr++ = palette[i*4 + 2];
		*pal_ptr++ = palette[i*4 + 1];
		*pal_ptr++ = palette[i*4 + 0];
	}

	// ... set palette colors ...
	png_set_PLTE(png_ptr, info_ptr, png_palette, PNG_MAX_PALETTE_LENGTH);
	// You must not free palette here, because png_set_PLTE only makes a link to
	// the palette that you malloced.  Wait until you are about to destroy the png structure.
	
	// Write the file header information.  REQUIRED.
	png_write_info(png_ptr, info_ptr);

CProgressWindow wndProgress;
wndProgress.Initialize();

wndProgress.ResetProgressBar("Jpeg:", (maxy-miny)/nMeters*(maxx-minx)/nMeters);

	for (int y=maxy; y>miny; y-=500) {
		memset(bitmap_memory, 0x01, bitmap_size);
		for (int x=minx; x<maxx; x+=500) {

			if (!wndProgress.ProgressBar()) return /*false*/;

			easting_northing_to_OSref3(x, y, tstring);
//			printf("%s\r", tstring);
			read_jpg(y, x, g_db, tstring, bitmap_memory+(x-minx)/500*TileWidth, bitmap_width);
		}


		// write row of PNG to file
		for (int x=0; x<TileWidth; x++)
			png_write_row(png_ptr, bitmap_memory + x*bitmap_width);
	}
	// It is REQUIRED to call this to finish writing the rest of the file.
	png_write_end(png_ptr, info_ptr);

	png_free(png_ptr, png_palette);
	
	// clean up after the write, and free any memory allocated.
	png_destroy_write_struct(&png_ptr, &info_ptr);

	// close the file.
	fclose(fp_png);

	dumpjpr_file(mapname, minx, miny, maxx, maxy, (maxx-minx)/500 * TileWidth, (maxy-miny)/500*TileWidth, NULL);
	free(bitmap_memory);
}

#define	HTTP_GET_SIZE	4096
int GetAerial_get_file(HINTERNET hConnect, LPCSTR pathname, long northing, long easting, CTileDatabase * db)
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

	LPCSTR	tx_saveto = "temp.jpg";
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
			printf("Error writing %d bytes to %s (%d %d)\n", tx_saveto, easting, northing);
			file_len = -1;
			break;
		}
		file_len += dwSize;
	//	printf("%d\r", file_len);
	}
	fclose(fp);

	if (!InternetCloseHandle (hReq) ) {
		ErrorOut (GetLastError (), "CloseHandle on hReq");
		file_len = -1;
	}
	if (file_len <= 0)
		return FALSE;

	LPBYTE buffer = (LPBYTE)malloc(file_len+1);
	if (buffer == 0) {
		fprintf(stderr, "Couldn't allocate %d bytes to verify file (%d %d)\n", file_len, easting, northing);
		return FALSE;
	}
	fp = fopen(tx_saveto, "rb");
	if (fp == NULL) {
		fprintf(stderr, "Failed to reopen %s\n", tx_saveto);
		free(buffer);
		return FALSE;
	}
	long file_len2 = fread(buffer, 1, file_len, fp);
	fclose(fp);
	buffer[file_len] = 0;
	unsigned char jpghdr[] = {0xFF, 0xD8, 0xFF, 0xE0, 0x00, 0x10, 0x4A, 0x46, 0x49, 0x46};
	unsigned char gifhdr[] = {0x47, 0x49, 0x46, 0x38, 0x39, 0x61};
	if (memcmp(buffer, jpghdr, sizeof(jpghdr)) == 0) {
		db->WriteTile(northing, easting, buffer, file_len);
	} else if (memcmp(buffer, gifhdr, sizeof(gifhdr)) == 0) {
		// file is really a GIF
		db->WriteTile(northing, easting, buffer, 0);
		remove(tx_saveto);
	} else {
		fprintf(stderr, "Error reading %s. Retrying\n", tx_saveto);
		free(buffer);
//		return GetAerial_get_file(hConnect, pathname, northing, easting, db);
		return false;
	}
	free(buffer);
	return TRUE;
}




int GetAerial_LoadTiles(CTileDatabase* g_db, long minx, long miny, long maxx, long maxy)
{
HINTERNET hOpen, hConnect;
char tstring[1024];

	if ( !(hOpen = InternetOpen ( "HeadDump",  LOCAL_INTERNET_ACCESS , NULL, 0, 0) ) ) {
		ErrorOut ( GetLastError(), "InternetOpen");
		return 0;
	}

	if ( !(hConnect = InternetConnect ( hOpen, "www.getmapping.com", INTERNET_INVALID_PORT_NUMBER, "",	"", INTERNET_SERVICE_HTTP, 0  , 0) ) ) {
		ErrorOut (GetLastError(), "InternetConnect");
		return 0;
	}
	FILE * fp_log = fopen("log.txt", "a+");
	fprintf(fp_log, "\n\nGetAerial Eastings: %04d %04d Northings: %04d %04d\n", minx, maxx, miny, maxy);

CProgressWindow wndProgress;
wndProgress.Initialize();

int nMeters = MyMap.GetMetresPerTile();

wndProgress.ResetProgressBar("Downloading:", (maxy-miny)/nMeters*(maxx-minx)/nMeters);


//	for (int x=minx; x<maxx; x+=500) {
//		for (int y=maxy; y>miny; y-=500) {

	for (long y=miny; y<maxy; y+=nMeters) {
		for (long x=minx; x<maxx; x+=nMeters) {

			if (!wndProgress.ProgressBar()) return false;

			easting_northing_to_OSref3(x, y, tstring);
	//		printf("Tile: %s %04d %04d - (%d of %d)\n", tstring, x/100, y/100, (x-minx)/500*(maxy-miny)/500+(maxy-y)/500+1, (maxx-minx)/500*(maxy-miny)/500);
			if (g_db->TileLoaded(y, x)) {
				fprintf(fp_log, "Tile: %s skipped - file exists\n", tstring);
				continue;
			}

			int revision = 0;

			easting_northing_to_OSref3(x, y, tstring);

// http://www.getmapping.com/preview/preview.ashx?X=486463.0&Y=496681.5&productID=9&dataSet=MM&size=125.0

			sprintf(tstring, "/preview/preview.ashx?x=%d&y=%d&productID=9&dataSet=mm&size=500", 250+x, y+250);

//			sprintf(tstring, "/preview/preview.ashx?x=%d&y=%d&productID=9&dataSet=mm&size=500", 250+x, y-252+500);
//			sprintf(tstring, "/preview/preview.ashx?x=%d&y=%d&productID=9&dataSet=mm&size=250", 250+x, y-252);

			if (GetAerial_get_file(hConnect, tstring, y, x, g_db)) {
				fprintf(fp_log, "Tile: %s Retrieved\n", tstring);
			} else {
				fprintf(fp_log, "Tile: %s Retrieval error\n", tstring);
			}

			if (abortProgram) {
				y=miny;
				x=maxx;
			}
		}
	}
	fclose(fp_log);

	if (!InternetCloseHandle (hConnect) ) {
		ErrorOut (GetLastError (), "CloseHandle on hConnect");
		return FALSE;
	}
	if (!InternetCloseHandle (hOpen) ) {
		ErrorOut (GetLastError (), "CloseHandle on hOpen");
		return FALSE;
	}
	return 0;
}


// http://www.getmapping.com/webpreview/preview.aspx?x=490000&y=510500&productID=9&dataSet=mm&size=500

// http://www.getmapping.com/preview/preview.ashx?X=486463.0&Y=496681.5&productID=9&dataSet=MM&size=125.0

void GetAerial(CTileDatabase* g_db, long minx, long miny, long maxx, long maxy, BOOL bLoadTiles)
{
/*
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

	minx = minx/500*500;
	miny = miny/500*500;
	maxx = (maxx+499)/500*500;
	maxy = (maxy+499)/500*500;
*/

//	_mkdir("jpg");

//	if (bLoadTiles)
		GetAerial_LoadTiles(g_db, minx, miny, maxx, maxy);

//	if (abortProgram)
//		return;

	/*
	colour_convert = (unsigned char*)malloc(256*256*256);
	memset(colour_convert, COLOUR_UNKNOWN, 256*256*256);
	Create_PNG_JPR_from_JPEG(g_db, "map", minx, maxx, miny, maxy);
	free(colour_convert);
*/
	return;
}

void DumpAerial(CTileDatabase* g_db, LPCSTR mapname, long minx, long miny, long maxx, long maxy)
{
	colour_convert = (unsigned char*)malloc(256*256*256);
	memset(colour_convert, COLOUR_UNKNOWN, 256*256*256);
	Create_PNG_JPR_from_JPEG(g_db, mapname, minx, maxx, miny, maxy);
	free(colour_convert);
}
