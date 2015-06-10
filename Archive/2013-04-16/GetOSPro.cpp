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

#include "..\MapLib\TileDatabase.h"

#include "GetOSPro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define printf visualprintf

extern CString g_strMapDatabaseFolder;
extern CString g_strMapScale;

unsigned char uncompressed[1500*1500];
long	uncompressed_length;

// libpng.lib library seems to need this to compile
/*
extern "C" {
	BYTE _iob[4] = {0,0,0,0};
}
*/

// -d -s nz8301 se8990
// TQ100600 TQ155480
/////////////////////////////////////////////////////////////////////////////
// The one and only application object

/*
CWinApp theApp;

using namespace std;
*/

/*
CTileDatabase::CTileDatabase()
{
	m_hFile = INVALID_HANDLE_VALUE;
	m_readonly = true;
	m_fileheader.m_easting = -1;
	m_fileheader.m_northing = -1;
}

CTileDatabase::~CTileDatabase()
{
	if (m_hFile != INVALID_HANDLE_VALUE)
		CloseHandle(m_hFile);
	m_hFile = INVALID_HANDLE_VALUE;
}

void CTileDatabase::InitDatabase(long northing, long easting)
{
	if (northing/100000 == m_fileheader.m_northing && easting/100000 == m_fileheader.m_easting)
		return; // Database already open

	if (m_hFile != INVALID_HANDLE_VALUE) {
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}

	sprintf(m_filename, "%s\\dbOSGB\\%s\\", g_strMapDatabaseFolder, g_strMapScale);
	easting_northing_to_OSref3(easting, northing, m_filename+strlen(m_filename));
	strcpy(m_filename+strlen(m_filename)-8, ".db");

	m_hFile = CreateFile(m_filename, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hFile == INVALID_HANDLE_VALUE) {
		memset((void *)&m_fileheader, 0, sizeof(m_fileheader));
		m_fileheader.m_magic='P' + ('N'<<8)+ ('G'<<16)+ ('d'<<24);
		m_fileheader.m_northing = northing/100000;
		m_fileheader.m_easting = easting/100000;
		m_fileheader.m_scale = 500;
		if (m_readonly)
			return;			// Do not create file
		m_hFile = CreateFile(m_filename, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (m_hFile == INVALID_HANDLE_VALUE) {
			printf("Couldn't open %s\n", m_filename);
			return;
		}
		DWORD nWritten;
		WriteFile(m_hFile, (void *)&m_fileheader, sizeof(m_fileheader), &nWritten, NULL);
		if (nWritten != sizeof(m_fileheader)) {
			printf("Error writing header information\n");
		}
	}

	SetFilePointer(m_hFile, 0, NULL, FILE_BEGIN);

	DWORD nRead;
	ReadFile(m_hFile, (void *)&m_fileheader, sizeof(m_fileheader), &nRead, NULL);
	if (nRead != sizeof(m_fileheader)) {
		printf("Error reading %s\n", m_filename);
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
		return;
	}
	if (m_fileheader.m_magic != ('P' + ('N'<<8)+ ('G'<<16)+ ('d'<<24)) ||
		m_fileheader.m_northing != northing/100000 ||
		m_fileheader.m_easting != easting/100000 ||
		m_fileheader.m_scale != 500)
	{
		printf("%s has a bad header\n");
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
		return;
	}
}

BOOL CTileDatabase::TileLoaded(long northing, long easting)
{
	InitDatabase(northing, easting);
	if (m_hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	DWORD offset = m_fileheader.m_offsets[northing%100000/m_fileheader.m_scale*(100000/m_fileheader.m_scale)+easting%100000/m_fileheader.m_scale];
	if (offset == 0 || offset == 1)
		return FALSE;				// either not loaded, or no tile
	return TRUE;
}

LPBYTE CTileDatabase::ReadTile(long northing, long easting, long * size)
{
	*size = 0;
	InitDatabase(northing, easting);
	if (m_hFile == INVALID_HANDLE_VALUE)
		return NULL;
	DWORD offset = m_fileheader.m_offsets[northing%100000/m_fileheader.m_scale*(100000/m_fileheader.m_scale)+easting%100000/m_fileheader.m_scale];
	if (offset == 0 || offset == 1)
		return NULL;				// either not loaded, or no tile
	SetFilePointer(m_hFile, offset, NULL, FILE_BEGIN);
	DWORD tile_length, nRead;
	ReadFile(m_hFile, &tile_length, sizeof(DWORD), &nRead, NULL);
	if (nRead != sizeof(DWORD)) {
		printf("Error reading length from %s. Easting=%d, Northing=%d\n", m_filename, easting, northing);
		return NULL;
	}
	BYTE * buffer = (BYTE*)malloc(tile_length);
	if (buffer == NULL) {
		printf("Couldn't allocate %d bytes to read easting=%d, Northing=%d of %s\n", tile_length, easting, northing, m_filename);
		return NULL;
	}
	ReadFile(m_hFile, buffer, tile_length, &nRead, NULL);
	if (nRead !=tile_length) {
		printf("Error reading data from %s. Easting=%d, Northing=%d\n", m_filename, easting, northing);
		return NULL;
	}
	*size = tile_length;
	return buffer;
}

void CTileDatabase::DeleteTile(long northing, long easting)
{
	InitDatabase(northing, easting);
	if (m_hFile == INVALID_HANDLE_VALUE)
		return;
	DWORD offset = northing%100000/m_fileheader.m_scale*(100000/m_fileheader.m_scale)+easting%100000/m_fileheader.m_scale;
	if (m_fileheader.m_offsets[offset]) {
		printf("Clearing item of easting=%d, Northing=%d, %s\n", easting, northing, m_filename);
	}
	SetFilePointer(m_hFile, (char*)&m_fileheader.m_offsets[offset] - (char*)&m_fileheader, NULL, FILE_BEGIN);
	DWORD nWritten;
	long	fileOffset=0;
	WriteFile(m_hFile, &fileOffset, sizeof(long), &nWritten, NULL);
	m_fileheader.m_offsets[offset] = fileOffset;
	if (nWritten != sizeof(long)) {
		printf("Error writing offset to %s. Easting=%d, Northing=%d\n", m_filename, easting, northing);
		return;
	}
}

void CTileDatabase::WriteTile(long northing, long easting, LPBYTE data, long size)
{
	InitDatabase(northing, easting);
	if (m_hFile == INVALID_HANDLE_VALUE)
		return;
	DWORD offset = northing%100000/m_fileheader.m_scale*(100000/m_fileheader.m_scale)+easting%100000/m_fileheader.m_scale;
	if (m_fileheader.m_offsets[offset]) {
		printf("Replacing data of easting=%d, Northing=%d, %s\n", easting, northing, m_filename);
	}
	SetFilePointer(m_hFile, (char*)&m_fileheader.m_offsets[offset] - (char*)&m_fileheader, NULL, FILE_BEGIN);
	DWORD nWritten;
	long	fileOffset;
	if (size == 0)
		fileOffset = 1;
	else
		fileOffset = GetFileSize(m_hFile, NULL);
	WriteFile(m_hFile, &fileOffset, sizeof(long), &nWritten, NULL);
	m_fileheader.m_offsets[offset] = fileOffset;
	if (nWritten != sizeof(long)) {
		printf("Error writing offset to %s. Easting=%d, Northing=%d\n", m_filename, easting, northing);
		return;
	}
	if (size == 0)
		return;
	SetFilePointer(m_hFile, fileOffset, NULL, FILE_BEGIN);
	WriteFile(m_hFile, &size, sizeof(long), &nWritten, NULL);
	if (nWritten != sizeof(long)) {
		printf("Error writing size to %s. Easting=%d, Northing=%d\n", m_filename, easting, northing);
		return;
	}
	WriteFile(m_hFile, data, size, &nWritten, NULL);
	if (nWritten != size) {
		printf("Error writing tile to %s. Easting=%d, Northing=%d\n", m_filename, easting, northing);
		return;
	}
}


#define BMP_UK_WIDTH	(700*2)
#define BMP_UK_HEIGHT	(1300*2)
unsigned char tracklog_palette[][4];


void CTileDatabase::DumpDatabaseBMP(void)
{
BITMAPFILEHEADER	bmp_file_header;
BITMAPINFOHEADER	bmp_info_header;
FILE * fp;

	fp = fopen("overview.bmp", "wb+");
	if (fp == NULL) {
		printf("Couldn't overwrite overview.bmp\n");
		return;
	}

	memset(&bmp_info_header, 0, sizeof(bmp_info_header));
   	bmp_info_header.biSize = sizeof(BITMAPINFOHEADER);
	bmp_info_header.biWidth = BMP_UK_WIDTH;
	bmp_info_header.biHeight = BMP_UK_HEIGHT;
	bmp_info_header.biPlanes = 1;
	bmp_info_header.biBitCount = 8;
	bmp_info_header.biCompression = BI_RGB;		// uncompressed bitmap

	// Compute the number of bytes in the array of color
	// indices and store the result in biSizeImage.

	bmp_info_header.biSizeImage = 0;
	bmp_info_header.biClrUsed = 256;
	bmp_info_header.biClrImportant = 0;		// all device colors are important
	
	bmp_file_header.bfType = 'B' + ('M'<<8);		// BM
	bmp_file_header.bfReserved1 = 0;
	bmp_file_header.bfReserved2 = 0;

	// Offset to the array of color indices
	bmp_file_header.bfOffBits = sizeof(BITMAPFILEHEADER) + bmp_info_header.biSize + bmp_info_header.biClrUsed * sizeof (RGBQUAD);

	// Size of entire file
	bmp_file_header.bfSize = bmp_file_header.bfOffBits + bmp_info_header.biSizeImage;

	fwrite(&bmp_file_header, sizeof(bmp_file_header), 1, fp);	// header
	fwrite(&bmp_info_header, sizeof(bmp_info_header), 1, fp);	// header info
	fwrite(tracklog_palette, 256*4, 1, fp);						// colour palette

	unsigned char * bitmap = (unsigned char *)malloc(BMP_UK_WIDTH*BMP_UK_HEIGHT);
	memset(bitmap, 0xff, BMP_UK_WIDTH*BMP_UK_HEIGHT);

	m_readonly = true;
	for (long y0=0; y0<13; y0++) {
		for (long x0=0; x0<7; x0++) {
			for (long y=0; y<200; y++) {
				for (long x=0; x<200; x++) {
					bitmap[(y0*200+y)*BMP_UK_WIDTH+x0*200+x] = TileLoaded((y0*200+y)*500, (x0*200+x)*500) ? 254 : 255;
				}
			}
		}
	}
	m_readonly = false;
	fwrite(bitmap, 1, BMP_UK_WIDTH*BMP_UK_HEIGHT, fp);
	fclose(fp);
	free(bitmap);
}

*/

int abortProgram = FALSE;

BOOL CtrlCHandler(DWORD dwCtrlType)
{
	abortProgram = TRUE;
	return TRUE;
}

#define	TILE_WIDTH	200

int read_png(long northing, long easting, CTileDatabase * db, unsigned char * bm_ptr, long bitmap_width)
{
	// load file
	long tileSize;
	LPBYTE data = db->ReadTile(northing, easting, &tileSize);
	if (data == NULL) {
		return 1;		// couldn't load file
	}
	char strTilename[256];
	easting_northing_to_OSref3(easting, northing, strTilename);
//	printf("%s\r", strTilename);

	FILE * fpin = fopen("tile.tmp", "wb+");
	if (fpin == NULL) {
		printf("Couldn't create tile.tmp\n");
		return 0;
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
			if (width != TILE_WIDTH || height != TILE_WIDTH) {
				printf("Bad size (%s) (%d, %d)\n", strTilename, width, height);
				db->DeleteTile(northing, easting);
				return 1;
			}
//			png_set_IHDR(write_ptr, write_info_ptr, width, height, bit_depth,
		}
		png_colorp png_palette= NULL;
		int ncolours = 0;
		png_get_PLTE(read_ptr, read_info_ptr, &png_palette, &ncolours);
		for (int i = 0; i<ncolours; i++)
			palette_convert[i] = RGB_to_tracklog_palette(png_palette[i].blue, png_palette[i].green, png_palette[i].red);
	}

	long source_width = png_get_rowbytes(read_ptr, read_info_ptr);
	png_bytep row_buf0 = (png_bytep)png_malloc(read_ptr, source_width);

	for (long y = 0; y < (long)height; y++) {
		png_read_rows(read_ptr, (png_bytepp)&row_buf0, (png_bytepp)NULL, 1);
		for (long x=0; x<(long)width; x++) {
			bm_ptr[y*bitmap_width + x] = palette_convert[row_buf0[x]];
		}
	}
	png_free(read_ptr, row_buf0);

	png_destroy_read_struct(&read_ptr, &read_info_ptr, &end_info_ptr);
	fclose(fpin);

	return 0;
}


// CTileDatabase	g_db;

/*
void Create_PNG_JPR(LPCSTR mapname, long minx, long maxx, long miny, long maxy)
{
char tstring[256];
long bitmap_width = TILE_WIDTH*(maxx-minx)/500;
long bitmap_size = bitmap_width*TILE_WIDTH;
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
	
	// Allocate/initialize the image information data.
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		fclose(fp_png);
		exit(0);
	}
	
	// Set error handling.
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
	// currently be PNG_COMPRESSION_TYPE_BASE and PNG_FILTER_TYPE_BASE. REQUIRED

	png_set_IHDR(png_ptr, info_ptr, TILE_WIDTH*(maxx-minx)/500, TILE_WIDTH*((maxy-miny)/500), 8, PNG_COLOR_TYPE_PALETTE,
				PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	// set the palette if there is one.  REQUIRED for indexed-color images.
	png_colorp png_palette = (png_colorp)png_malloc(png_ptr, PNG_MAX_PALETTE_LENGTH * sizeof (png_color));

	int i;
	BYTE * pal_ptr = (BYTE *)png_palette;
	BYTE * palette = (BYTE*)tracklog_palette;
	for (i=0; i<256; i++) {
		*pal_ptr++ = palette[i*4 + 2];
		*pal_ptr++ = palette[i*4 + 1];
		*pal_ptr++ = palette[i*4 + 0];
	}

	// ... set palette colors ...
	png_set_PLTE(png_ptr, info_ptr, png_palette, PNG_MAX_PALETTE_LENGTH);
	// You must not free palette here, because png_set_PLTE only makes a link to
	// the palette that you malloced.  Wait until you are about to destroy
	//the png structure.
	
	// Write the file header information.
	png_write_info(png_ptr, info_ptr);
	for (int y=maxy; y>miny; y-=500) {
		memset(bitmap_memory, 0xff, bitmap_size);
		for (int x=minx; x<maxx; x+=500) {
			read_png(y-500, x, &g_db, bitmap_memory+(x-minx)/500*TILE_WIDTH, bitmap_width);
		}


		// write row of PNG to file
		for (int x=0; x<TILE_WIDTH; x++)
			png_write_row(png_ptr, bitmap_memory + x*bitmap_width);
	}
	// It is REQUIRED to call this to finish writing the rest of the file.
	png_write_end(png_ptr, info_ptr);

	png_free(png_ptr, png_palette);
	
	// clean up after the write, and free any memory allocated.
	png_destroy_write_struct(&png_ptr, &info_ptr);

	// close the file.
	fclose(fp_png);

	dumpjpr_file(mapname, minx, miny, maxx, maxy, (maxx-minx)/500 * TILE_WIDTH, (maxy-miny)/500*TILE_WIDTH, NULL);
	free(bitmap_memory);
}

void Create_PKG(char * mapname, long minx, long maxx, long miny, long maxy)
{
char tstring[256];
long bitmap_width = 2*TILE_WIDTH;
long bitmap_size = 2*bitmap_width*bitmap_width;
unsigned char * bitmap_memory = (unsigned char *)malloc(bitmap_size);
unsigned char tbuffer[2*TILE_WIDTH];

	if (bitmap_memory == NULL) {
		printf("Couldn't allocate %d bytes of memory\n");
		return;
	}

	CTracklogPackage	pkg;
	sprintf(tstring, "%s.pkg", mapname);
	pkg.Open(tstring, false);
	minx = minx/1000*1000;
	miny = miny/1000*1000;
	maxx = (maxx+999)/1000*1000;
	maxy = (maxy+999)/1000*1000;
	for (int y=miny; y<maxy; y+=1000) {
		for (int x=minx; x<maxx; x+=1000) {
			memset(bitmap_memory, 0xff, bitmap_size);
			read_png(y, x, &g_db, bitmap_memory+TILE_WIDTH*bitmap_width, bitmap_width);
			read_png(y, x+500, &g_db, bitmap_memory+TILE_WIDTH*bitmap_width+TILE_WIDTH, bitmap_width);
			read_png(y+500, x, &g_db, bitmap_memory, bitmap_width);
			read_png(y+500, x+500, &g_db, bitmap_memory+TILE_WIDTH, bitmap_width);
			// invert bitmap
			for (int y2=0; y2<TILE_WIDTH; y2++) {
				unsigned char * p1 = bitmap_memory+bitmap_width*y2;
				unsigned char * p2 = bitmap_memory+bitmap_width*(2*TILE_WIDTH-1-y2);
				memcpy(tbuffer, p1, bitmap_width);
				memcpy(p1, p2, bitmap_width);
				memcpy(p2, tbuffer, bitmap_width);
			}
			pkg.WriteTile(x/1000, y/1000, 0x101, bitmap_memory, bitmap_width, 9);
		}
	}
	free(bitmap_memory);
}
*/

/****************************************************************************
*
*	 FUNCTION: ErrorOut
*
*	 PURPOSE: This function is used to get extended Internet error.
*
*	 COMMENTS:	Function returns TRUE on success and FALSE on failure.
*
****************************************************************************/

BOOL ErrorOut(DWORD dError, TCHAR * szCallFunc)
{
	fprintf(stderr, "%s error %d\n", szCallFunc, dError);
	if (dError == ERROR_INTERNET_EXTENDED_ERROR) {
		DWORD  dwIntError , dwLength = 0;
		TCHAR *szBuffer=NULL;
		InternetGetLastResponseInfo (&dwIntError, NULL, &dwLength);
		if (dwLength) {
			if ( !(szBuffer = (TCHAR *) LocalAlloc ( LPTR,	dwLength) ) ) {
				fprintf(stderr, "Unable to allocate memory to display Internet error code. Error code: %d\n", GetLastError());
				return FALSE;
			}
			if (!InternetGetLastResponseInfo (&dwIntError, (LPTSTR) szBuffer, &dwLength)) {
				fprintf(stderr, "Unable to get Intrnet error. Error code: %d\n", GetLastError());
				return FALSE;
			}
			fprintf(stderr, "%s\n", szBuffer);
			LocalFree (szBuffer);
		}
	}
	return TRUE;
}


#define	HTTP_GET_SIZE	65536
int get_file_1(HINTERNET hConnect, LPCSTR pathname, LPCSTR tx_saveto)
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
		return FALSE;

	// Validate PNG
	LPBYTE buffer = (LPBYTE)malloc(file_len+1);
	if (buffer == 0) {
		fprintf(stderr, "Couldn't allocate %d bytes to verify %s\n", file_len, tx_saveto);
		return FALSE;
	}
	memset(buffer, 0, file_len+1);
	fp = fopen(tx_saveto, "rb");
	if (fp == NULL) {
		fprintf(stderr, "Failed to reopen %s\n", tx_saveto);
		free(buffer);
		return FALSE;
	}
	if (fread(buffer, 1, file_len, fp) != file_len) {
		fprintf(stderr, "Error reading %s\n", tx_saveto);
		free(buffer);
		return FALSE;
	}
	fclose(fp);

	unsigned char pnghdr[] = {0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a};
	if (memcmp(buffer, pnghdr, sizeof(pnghdr)) != 0) {
		fprintf(stderr, "Error retrieving %s\n", tx_saveto);
		free(buffer);
		return FALSE;
	}

	free(buffer);
	return TRUE;
}


int get_file(HINTERNET hConnect, LPCSTR pathname, LPCSTR tx_saveto)
{
// Attempt to get file and retry once if failed
	if (get_file_1(hConnect, pathname, tx_saveto))
		return 1;
	return get_file_1(hConnect, pathname, tx_saveto);
}


void GetTile(HINTERNET hConnect, char * text, long northing, long easting, CTileDatabase * db, FILE * fp_log, BOOL bForce)
{
char strRequest[1024];
	if (text == NULL)
		return;

	northing = northing/500*500;
	easting = easting/500*500;

	sprintf(strRequest, "osmapapi/ts?FORMAT=image%%2Fpng&KEY=DEMOAPI&URL=http%%3A%%2F%%2Fosopenspacewiki.ordnancesurvey.co.uk%%2Fwiki%%2Fextensions%%2Fhtmlets%%2FOSProCS.html&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&EXCEPTIONS=application%%2Fvnd.ogc.se_inimage&LAYERS=2.5&PRODUCT=25K&SRS=EPSG%%3A27700&"
						"BBOX=%d,%d,%d,%d&WIDTH=200&HEIGHT=200", easting, northing, easting+500, northing-500);

	if (!bForce && db->TileLoaded(northing, easting))
		return;

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


// http://osopenspacepro.ordnancesurvey.co.uk/osmapapi/ts?FORMAT=image%2Fpng&KEY=DEMOAPI&URL=http%3A%2F%2Fosopenspacewiki.ordnancesurvey.co.uk%2Fwiki%2Fextensions%2Fhtmlets%2FOSProCS.html&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&EXCEPTIONS=application%2Fvnd.ogc.se_inimage&LAYERS=2.5&PRODUCT=25K&SRS=EPSG%3A27700&BBOX=258000,667500,258500,668000&WIDTH=200&HEIGHT=200

int LoadTiles(CTileDatabase* g_db, long minx, long miny, long maxx, long maxy, BOOL bForce)
{
HINTERNET hOpen, hConnect;
char strTile[256];

	hOpen = NULL;
	FILE * fp_log = fopen("log.txt", "a+");
//	fprintf(fp_log, "\n\nGetOSPro Eastings: %04d %04d Northings: %04d %04d\n", minx, maxx, miny, maxy);

CProgressWindow wndProgress;
wndProgress.Initialize();
DoEvents();

wndProgress.ResetProgressBar("Downloading:", (maxy-miny)/500*(maxx-minx)/500);
DoEvents();

	// grab in 8 x 8 tiles
	for (long y=miny; y<maxy; y+=500) {
		for (long x=minx; x<maxx; x+=500) {
			easting_northing_to_OSref3(x, y, strTile);

//printf("Tile: %s %04d %04d - (%d of %d)\n", strTile, x/100, y/100, (y-miny)/500*(maxx-minx)/500+(x-minx)/500+1, (maxy-miny)/500*(maxx-minx)/500);
wndProgress.ProgressBar();
DoEvents();
if (wndProgress.m_Cancelled) return false;

			easting_northing_to_OSref3(x, y, strTile);
			if (!bForce && g_db->TileLoaded(y, x)) {
				fprintf(fp_log, "Tile: %s skipped - tile exists\n", strTile);
				continue;
			} else {
				// Only connect to OSPro if required
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
				GetTile(hConnect, strTile, y, x, /*&*/g_db, fp_log, bForce);

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


// GetOSPro [extent1] [extent2]
// GetOSPro [minx] [maxx] [miny] [maxy]

/*
int GetOSPro_tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
unsigned long tc = GetTickCount();

	// initialize MFC and print and error on failure
//
//	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0)) {
//		// TODO: change error code to suit your needs
//		cerr << _T("Fatal Error: MFC initialization failed") << endl;
//		return 1;
//	}


	long	minx, miny, maxx, maxy;
	BOOL	bForce = FALSE;
	BOOL	bLoadTiles = TRUE;
	BOOL	bPKGfile = FALSE;

	char strMapname[256];

	strcpy(strMapname, "map");

	SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlCHandler, TRUE);

	while (argc > 1) {
		if (stricmp(argv[1], "-d") ==0) {
			g_db.DumpDatabaseBMP();
			return 0;
		}
		if (stricmp(argv[1], "-f") ==0) {
			bForce = TRUE;
			argc--;
			argv++;
			continue;
		}
		if (stricmp(argv[1], "-s") ==0) {
			bLoadTiles = FALSE;
			argc--;
			argv++;
			continue;
		}
		if (stricmp(argv[1], "-p") ==0) {
			bPKGfile = TRUE;
			argc--;
			argv++;
			continue;
		}
		break;
	}

	if (argc == 2) {
		if (!OSref_to_easting_northing(argv[1], &minx, &miny))
			return 0;
		maxx = minx+10000;
		maxy = miny+10000;
		sprintf(strMapname, "%s", argv[1]);
	} else if (argc == 3) {
		if (!OSref_to_easting_northing(argv[1], &minx, &miny))
			return 0;
		if (!OSref_to_easting_northing(argv[2], &maxx, &maxy))
			return 0;
	} else if (argc == 5) {
		minx = atol(argv[1]);
		maxx = atol(argv[2]);
		miny = atol(argv[3]);
		maxy = atol(argv[4]);
		if (maxx<1000) {
			minx*=100;
			miny*=100;
			maxx*=100;
			maxy*=100;
		}
	} else {
		printf(	"GetOSPro [-f][-p][-s] extent1\n"
				"GetOSPro [-f][-p][-s] extent1 extent2\n"
				"GetOSPro [-f][-p][-s] easting1 easting2 northing1 northing2\n"
				"GetOSPro [-d]\n"
				"\t-f force reloading of tiles\n"
				"\t-p write PKG instead of PNG/JPR\n"
				"\t-s Skip downloading of tiles\n"
				"\t-d Dump overview.bmp of downloaded tiles\n");
		return 0;
	}
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

	_mkdir("db");

	if (bLoadTiles)
		LoadTiles(minx, miny, maxx, maxy, bForce);

	if (abortProgram)
		return 0;

	if (bPKGfile)
		Create_PKG(strMapname, minx, maxx, miny, maxy);
	else
		Create_PNG_JPR(strMapname, minx, maxx, miny, maxy);

//	printf("%d\n", GetTickCount() - tc);

	return 0;
}
*/
//-s 470000 505000 479000 521000

/*
void GetOSProOZI(CRect MyRect)
{
	char strRef1_2[256];
	char strRef2_2[256];

	easting_northing_to_OSref2(MyRect.left, MyRect.top, strRef1_2);
	easting_northing_to_OSref2(MyRect.right, MyRect.bottom, strRef2_2);

	char * GetOSProParams[16];
	GetOSProParams[0] = _strdup("GetOSPro");
	GetOSProParams[1] = _strdup(strRef1_2);
	GetOSProParams[2] = _strdup(strRef2_2);

	GetOSPro_tmain(3, GetOSProParams, NULL);

	free(GetOSProParams[0]);
	free(GetOSProParams[1]);
	free(GetOSProParams[2]);
}
*/

#define N_OS_ZONES	60

bool IsInOsDatabase(char* strZone)
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

	for (int i=0; i < N_OS_ZONES; i++) {
		if (strZone == zones[i].strOSZone) {
			return true;
		}
	}

	return false;
}