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

// RGB to Palette Functions
#define	COLOUR_UNKNOWN	0xFE

class CRGBtoPalette {

private:
	unsigned char * m_colour_convert;

public:
	CRGBtoPalette();
	~CRGBtoPalette();

	void Init(void);
	void Clear(void);
	void Free(void);
	unsigned char CRGBtoPalette::ColourConvert(long r, long g, long b);
};

CRGBtoPalette::CRGBtoPalette()
{
	m_colour_convert = NULL;
}

CRGBtoPalette::~CRGBtoPalette()
{
	Free();
}

void CRGBtoPalette::Init()
{
	if (m_colour_convert) {

		return;
	}
	m_colour_convert = (unsigned char*)malloc(256*256*256);
	memset(m_colour_convert, COLOUR_UNKNOWN, 256*256*256);
}

void CRGBtoPalette::Clear()
{
	memset(m_colour_convert, COLOUR_UNKNOWN, 256*256*256);
}

void CRGBtoPalette::Free()
{
	if (m_colour_convert != NULL) {
		free(m_colour_convert);
		m_colour_convert = NULL;
	}
}

unsigned char CRGBtoPalette::ColourConvert(long r, long g, long b)
{
long x_value;

//	long r = bitmap[0];
//	long g = bitmap[1];
//	long b = bitmap[2];
	x_value = r + (g<<8) + (b<<16);
	if (m_colour_convert[x_value] == COLOUR_UNKNOWN)
		m_colour_convert[x_value] = RGBtoPalette(get_tracklog_palette(), 256, r, g, b); // Palette was AP25!
	return m_colour_convert[x_value];
}

CRGBtoPalette g_RGBtoPalette;


//unsigned char * colour_convert;

/*
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
*/

int read_jpg(long northing, long easting, CTileDatabase * db, char * szFile, unsigned char * bm_ptr, long bitmap_width)
{
g_RGBtoPalette.Init();

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
//		printf("Couldn't load picture %s\n", szFile);
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
//			bm_ptr[x+(TileWidth-y-1)*bitmap_width] = get_colour((unsigned char*)bmp.bmBits+y*bmp.bmWidthBytes+x*3);
			unsigned char * ptr = (unsigned char*)bmp.bmBits+y*bmp.bmWidthBytes+x*3;
			bm_ptr[x+(TileWidth-y-1)*bitmap_width] = g_RGBtoPalette.ColourConvert(ptr[0], ptr[1], ptr[2]);
		}
	}

	pPicture->Release();
	return 0;
}

// ToDo: To Make a database variable!
//#define	TILE_WIDTH	200

int read_png(long northing, long easting, CTileDatabase * db, unsigned char * bm_ptr, long bitmap_width)
{
g_RGBtoPalette.Init();

	 bool bReturnValue = 0;
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
	if (read_info_ptr->pixel_depth <= 8) {
		png_get_PLTE(read_ptr, read_info_ptr, &png_palette, &ncolours);

		for (int i = 0; i<ncolours; i++)
			palette_convert[i] = RGB_to_tracklog_palette(png_palette[i].blue, png_palette[i].green, png_palette[i].red);
	}

	unsigned long passes = png_set_interlace_handling(read_ptr);
	png_read_update_info(read_ptr, read_info_ptr);

	long source_width = png_get_rowbytes(read_ptr, read_info_ptr);
	png_bytepp row_bufs = (png_bytepp)png_malloc(read_ptr, sizeof(png_bytep)*height);
	DWORD y;
	for (y=0; y<height; y++)
		row_bufs[y] = (png_bytep)png_malloc(read_ptr, source_width);

	
	for (y=0; y<passes; y++)
		png_read_rows(read_ptr, row_bufs, (png_bytepp)NULL, height);
	if (read_info_ptr->pixel_depth == 24) {
		// Slow Method: Convert 24 bit png to 8 bit png. 
		for (y = 0; y < height; y++) {
			for (DWORD x=0; x<width; x++) {
				//bm_ptr[y*bitmap_width + x] = RGB_to_tracklog_palette(row_bufs[y][x*3+2], row_bufs[y][x*3+1], row_bufs[y][x*3]);
bm_ptr[y*bitmap_width + x] = g_RGBtoPalette.ColourConvert(row_bufs[y][x*3+2], row_bufs[y][x*3+1], row_bufs[y][x*3]);
			}
		}
	} else if (read_info_ptr->pixel_depth == 32) {
		// Slow Method: Convert 32 bit png to 8 bit png. 
		for (y = 0; y < height; y++) {
			for (DWORD x=0; x<width; x++) {
				//bm_ptr[y*bitmap_width + x] = RGB_to_tracklog_palette(row_bufs[y][x*4+2], row_bufs[y][x*4+1], row_bufs[y][x*4]);
bm_ptr[y*bitmap_width + x] = g_RGBtoPalette.ColourConvert(row_bufs[y][x*4+2], row_bufs[y][x*4+1], row_bufs[y][x*4]);
			}
		}
	} else if (read_info_ptr->pixel_depth == 8) {
		for (y = 0; y < height; y++) {
			for (long x=0; x<(long)width; x++) {
				bm_ptr[y*bitmap_width + x] = palette_convert[row_bufs[y][x]];
			}
		}
	} else if (read_info_ptr->pixel_depth == 4) {
		// TODO: Are single bit PNGs little endian or big endian
		for (y = 0; y < height; y++) {
			for (long x=0; x<(long)width; x+=2) {
				unsigned char ch = row_bufs[y][x/2];
				bm_ptr[y*bitmap_width + x+1] = palette_convert[ch&0x0f]; ch>>=4;
				bm_ptr[y*bitmap_width + x+0] = palette_convert[ch];
			}
		}
	} else if (read_info_ptr->pixel_depth == 1) {
		// TODO: Are single bit PNGs little endian or big endian
		for (y = 0; y < height; y++) {
			for (long x=0; x<(long)width; x+=8) {
				unsigned char ch = row_bufs[y][x/8];
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
		bReturnValue = 1;
	}

	for (y=0; y<height; y++)
		png_free(read_ptr, row_bufs[y]);
	png_free(read_ptr, row_bufs);

	png_destroy_read_struct(&read_ptr, &read_info_ptr, &end_info_ptr);
	fclose(fpin);

	return bReturnValue;
}

/*
void DumpAerial(CTileDatabase* g_db, LPCSTR mapname, long minx, long miny, long maxx, long maxy)
{
	colour_convert = (unsigned char*)malloc(256*256*256);
	memset(colour_convert, COLOUR_UNKNOWN, 256*256*256);
	Create_PNG_JPR_from_JPEG(g_db, mapname, minx, maxx, miny, maxy);
	free(colour_convert);
}
*/

/*
int compare_palettes(const void * palette1, const void * palette2, int palette_len)
{
const DWORD * pal1 = (DWORD*)palette1;
const DWORD * pal2 = (DWORD*)palette2;
	palette_len >>= 2;
	while (palette_len--) {
		if ((*pal1 & 0xffffff) != (*pal2++ & 0xffffff))
			return 0;
	}
	return 1;
}
*/