// Don't worry about unsafe standard C library functions.
#define _CRT_SECURE_NO_WARNINGS
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit



//#include "stdafx.h"
#include "targetver.h"

#include <afx.h>
#include <afxtempl.h>

#include <direct.h>

#include <Wininet.h>
/*
#include "OziGen.h"
#include "OziGenDlg.h"
#include "ProgressWindow.h"
#include "func.h"

#include "..\MapLib\map_conv.h"
#include "..\MapLib\png.h"
#include "..\MapLib\palette.h"
*/

#include "TileDatabase.h"

#include "..\MapLib\CoordConv.h"

/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

//#define printf visualprintf

extern CString g_strMapDatabaseFolder;
extern CString g_strMapCountry;
extern CString g_strMapScale;
extern CString g_strMapFlavour;

//unsigned char uncompressed[1500*1500];
//long	uncompressed_length;

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

//#define	TILE_DB_SCALE	250

CTileDatabase::CTileDatabase(long nDatum, int nMetresPerTile, int nMapScale,  int nPixelsPerTile)
{
	m_hFile = INVALID_HANDLE_VALUE;
	m_readonly = true;
	m_scale = nMetresPerTile;
	m_header_size = sizeof(struct tile_db_header) + sizeof(DWORD)*100000/nMetresPerTile*100000/nMetresPerTile;
	m_fileheader = (struct tile_db_header *)malloc(m_header_size);
	m_fileheader->m_easting = -1;
	m_fileheader->m_northing = -1;

	m_datum = nDatum;
	m_mapscale = nMapScale;
	m_nPixelsPerTile = nPixelsPerTile;
	m_nMetresPerTile = nMetresPerTile;
}

CTileDatabase::~CTileDatabase()
{
	if (m_hFile != INVALID_HANDLE_VALUE)
		CloseHandle(m_hFile);
	m_hFile = INVALID_HANDLE_VALUE;
	free(m_fileheader);
}

void CTileDatabase::InitDatabase(long northing, long easting, DWORD datum)
{
	if (northing/100000 == m_fileheader->m_northing && easting/100000 == m_fileheader->m_easting)
		return; // Database already open

	if (m_hFile != INVALID_HANDLE_VALUE) {
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}

if (g_strMapCountry == "dbOSGB") {
	sprintf(m_filename, "%s\\%s\\%s_%s\\", g_strMapDatabaseFolder, g_strMapCountry, g_strMapScale, g_strMapFlavour);
	easting_northing_to_OSref3(easting, northing, m_filename+strlen(m_filename));
	strcpy(m_filename+strlen(m_filename)-8, ".db");
} else {
	sprintf(m_filename, "%s\\%s\\%s\\%d_%d.db", g_strMapDatabaseFolder, g_strMapCountry, g_strMapScale, easting/100000, northing/100000);
}
	m_hFile = CreateFile(m_filename, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hFile == INVALID_HANDLE_VALUE) {
		memset((void *)m_fileheader, 0, m_header_size);
		m_fileheader->m_magic='P' + ('N'<<8)+ ('G'<<16)+ ('d'<<24);
		m_fileheader->m_padding[0]= datum;
		m_fileheader->m_northing = northing/100000;
		m_fileheader->m_easting = easting/100000;
		m_fileheader->m_scale = m_scale;
		if (m_readonly)
			return;			// Do not create file
		m_hFile = CreateFile(m_filename, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (m_hFile == INVALID_HANDLE_VALUE) {
			printf("Couldn't open %s\n", m_filename);
			return;
		}
		DWORD nWritten;
		WriteFile(m_hFile, (void *)m_fileheader, m_header_size, &nWritten, NULL);
		if (nWritten != m_header_size) {
			printf("Error writing header information\n");
		}
	}

	SetFilePointer(m_hFile, 0, NULL, FILE_BEGIN);

	DWORD nRead;
	ReadFile(m_hFile, (void *)m_fileheader, m_header_size, &nRead, NULL);
	if (nRead != m_header_size) {
		printf("Error reading %s\n", m_filename);
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
		return;
	}
	if (m_fileheader->m_magic != ('P' + ('N'<<8)+ ('G'<<16)+ ('d'<<24)) ||
		m_fileheader->m_northing != northing/100000 ||
		m_fileheader->m_easting != easting/100000 ||
		m_fileheader->m_scale != m_scale
		)
	{
		printf("%s has a bad header\n");
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
		return;
	}
}


BOOL CTileDatabase::TileLoaded(long northing, long easting)
{
	InitDatabase(northing, easting, 0);
	if (m_hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	DWORD offset = m_fileheader->m_offsets[northing%100000/m_fileheader->m_scale*(100000/m_fileheader->m_scale)+easting%100000/m_fileheader->m_scale];
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
	DWORD offset = m_fileheader->m_offsets[northing%100000/m_fileheader->m_scale*(100000/m_fileheader->m_scale)+easting%100000/m_fileheader->m_scale];
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
	DWORD offset = northing%100000/m_fileheader->m_scale*(100000/m_fileheader->m_scale)+easting%100000/m_fileheader->m_scale;
	if (m_fileheader->m_offsets[offset]) {
		printf("Clearing item of easting=%d, Northing=%d, %s\n", easting, northing, m_filename);
	}
	SetFilePointer(m_hFile, (char*)&m_fileheader->m_offsets[offset] - (char*)m_fileheader, NULL, FILE_BEGIN);
	DWORD nWritten;
	long	fileOffset=0;
	WriteFile(m_hFile, &fileOffset, sizeof(long), &nWritten, NULL);
	m_fileheader->m_offsets[offset] = fileOffset;
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
	DWORD offset = northing%100000/m_fileheader->m_scale*(100000/m_fileheader->m_scale)+easting%100000/m_fileheader->m_scale;
	if (m_fileheader->m_offsets[offset]) {
		printf("Replacing data of easting=%d, Northing=%d, %s\n", easting, northing, m_filename);
	}
	SetFilePointer(m_hFile, (char*)&m_fileheader->m_offsets[offset] - (char*)m_fileheader, NULL, FILE_BEGIN);
	DWORD nWritten;
	long	fileOffset;
	if (size == 0)
		fileOffset = 1;
	else
		fileOffset = GetFileSize(m_hFile, NULL);
	WriteFile(m_hFile, &fileOffset, sizeof(long), &nWritten, NULL);
	m_fileheader->m_offsets[offset] = fileOffset;
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
	/*
	 * Compute the number of bytes in the array of color
	 * indices and store the result in biSizeImage.
	 */
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
/*
	COUNTRY_OSGB,
	COUNTRY_IGN,
	COUNTRY_OSNI,
	COUNTRY_OSI,
	COUNTRY_CH,
	COUNTRY_NGI, 
	COUNTRY_AT,
	COUNTRY_DE,
	COUNTRY_NL
*/
void CTileDatabase::SetDatum(int nCountry)
{
	switch (nCountry) {
		case 0:
		m_fileheader->m_padding[0] ='O' + ('S'<<8)+ ('G'<<16)+ ('B'<<24);
		break;
		case 1:
		m_fileheader->m_padding[0] ='I' + ('G'<<8)+ ('N'<<16)+ (' '<<24);
		break;
		case 2:
		m_fileheader->m_padding[0] ='O' + ('S'<<8)+ ('N'<<16)+ ('i'<<24);
		break;
		case 3:
		m_fileheader->m_padding[0] ='O' + ('S'<<8)+ ('i'<<16)+ (' '<<24);
		break;
		case 4:
		m_fileheader->m_padding[0] ='C' + ('H'<<8)+ (' '<<16)+ (' '<<24);
		break;
		case 5:
		m_fileheader->m_padding[0] ='N' + ('G'<<8)+ ('I'<<16)+ (' '<<24);
		break;
		case 6:
		m_fileheader->m_padding[0] ='A' + ('T'<<8)+ (' '<<16)+ (' '<<24);
		break;
		case 7:
		m_fileheader->m_padding[0] ='D' + ('E'<<8)+ (' '<<16)+ (' '<<24);
		break;
		case 8:
		m_fileheader->m_padding[0] ='N' + ('L'<<8)+ (' '<<16)+ (' '<<24);
		break;
	}
}