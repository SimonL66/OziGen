#include "stdafx.h"

#include <direct.h>
#include <ctype.h>
#include <math.h>

//#include <windows.h>
//#include <stdio.h>

#include "..\MapLib\png.h"
#include "..\MapLib\mmmath.h"
#include "..\MapLib\Lambert93.h"

#include "IGN.h"

#include "ProgressWindow.h"
#include "func.h"

extern CString g_strMapDatabaseFolder;
extern CString g_strMapScale;
extern CString g_strMapFlavour;

CIGNTileDatabase::CIGNTileDatabase()
{
	m_hFile = INVALID_HANDLE_VALUE;
	m_readonly = true;
	m_fileheader.m_easting = -1;
	m_fileheader.m_northing = -1;
}

CIGNTileDatabase::~CIGNTileDatabase()
{
	if (m_hFile != INVALID_HANDLE_VALUE)
		CloseHandle(m_hFile);
	m_hFile = INVALID_HANDLE_VALUE;
}

void CIGNTileDatabase::InitDatabase(long northing, long easting)
{
	if (northing/100000 == m_fileheader.m_northing && easting/100000 == m_fileheader.m_easting)
		return; // Database already open

	if (m_hFile != INVALID_HANDLE_VALUE) {
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}

	sprintf(m_filename, "%s\\dbIGN\\%s\\%d_%d.db", g_strMapDatabaseFolder, g_strMapScale, easting/100000, northing/100000);

	m_hFile = CreateFile(m_filename, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hFile == INVALID_HANDLE_VALUE) {
		memset((void *)&m_fileheader, 0, sizeof(m_fileheader));
		m_fileheader.m_magic='B' + ('M'<<8)+ ('P'<<16)+ ('z'<<24);
		m_fileheader.m_northing = northing/100000;
		m_fileheader.m_easting = easting/100000;
		m_fileheader.m_scale = 1000;
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
	if (m_fileheader.m_magic != ('B' + ('M'<<8)+ ('P'<<16)+ ('z'<<24)) ||
		m_fileheader.m_northing != northing/100000 ||
		m_fileheader.m_easting != easting/100000 ||
		m_fileheader.m_scale != 1000)
	{
		printf("%s has a bad header\n");
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
		return;
	}
}

BOOL CIGNTileDatabase::TileLoaded(long northing, long easting)
{
	InitDatabase(northing, easting);
	if (m_hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	DWORD offset = m_fileheader.m_offsets[northing%100000/m_fileheader.m_scale*(100000/m_fileheader.m_scale)+easting%100000/m_fileheader.m_scale];
	if (offset == 0 || offset == 1)
		return FALSE;				// either not loaded, or no tile
	return TRUE;
}

LPBYTE CIGNTileDatabase::ReadTile(long northing, long easting, long * size)
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
		printf("Error reading length from %s. Easting=%d, northing=%d\n", m_filename, easting, northing);
		return NULL;
	}
	BYTE * buffer = (BYTE*)malloc(tile_length);
	if (buffer == NULL) {
		printf("Couldn't allocate %d bytes to read easting=%d, northing=%d of %s\n", tile_length, easting, northing, m_filename);
		return NULL;
	}
	ReadFile(m_hFile, buffer, tile_length, &nRead, NULL);
	if (nRead !=tile_length) {
		printf("Error reading data from %s. Easting=%d, northing=%d\n", m_filename, easting, northing);
		return NULL;
	}
	*size = tile_length;
	return buffer;
}

void CIGNTileDatabase::DeleteTile(long northing, long easting)
{
	InitDatabase(northing, easting);
	if (m_hFile == INVALID_HANDLE_VALUE)
		return;
	DWORD offset = northing%100000/m_fileheader.m_scale*(100000/m_fileheader.m_scale)+easting%100000/m_fileheader.m_scale;
	if (m_fileheader.m_offsets[offset]) {
		printf("Clearing item of easting=%d, northing=%d, %s\n", easting, northing, m_filename);
	}
	SetFilePointer(m_hFile, (char*)&m_fileheader.m_offsets[offset] - (char*)&m_fileheader, NULL, FILE_BEGIN);
	DWORD nWritten;
	long	fileOffset=0;
	WriteFile(m_hFile, &fileOffset, sizeof(long), &nWritten, NULL);
	m_fileheader.m_offsets[offset] = fileOffset;
	if (nWritten != sizeof(long)) {
		printf("Error writing offset to %s. Easting=%d, northing=%d\n", m_filename, easting, northing);
		return;
	}
}

void CIGNTileDatabase::WriteTile(long northing, long easting, LPBYTE data, long size)
{
	InitDatabase(northing, easting);
	if (m_hFile == INVALID_HANDLE_VALUE)
		return;
	DWORD offset = northing%100000/m_fileheader.m_scale*(100000/m_fileheader.m_scale)+easting%100000/m_fileheader.m_scale;
	if (m_fileheader.m_offsets[offset]) {
//		printf("Replacing data of easting=%d, northing=%d, %s\n", easting, northing, m_filename);
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
		printf("Error writing offset to %s. Easting=%d, northing=%d\n", m_filename, easting, northing);
		return;
	}
	if (size == 0)
		return;
	SetFilePointer(m_hFile, fileOffset, NULL, FILE_BEGIN);
	WriteFile(m_hFile, &size, sizeof(long), &nWritten, NULL);
	if (nWritten != sizeof(long)) {
		printf("Error writing size to %s. Easting=%d, northing=%d\n", m_filename, easting, northing);
		return;
	}
	WriteFile(m_hFile, data, size, &nWritten, NULL);
	if (nWritten != size) {
		printf("Error writing tile to %s. Easting=%d, northing=%d\n", m_filename, easting, northing);
		return;
	}
}

/*
easting     0,000 -> 1200,000	1200000
northing 6100,000 -> 7200,000	1100000
*/

/*
#define BMP_FRANCE_WIDTH	(1200)
#define BMP_FRANCE_HEIGHT	(1100)
*/

unsigned char ign_palette[1024] = {
	0xFE, 0xFE, 0xFE, 0xFF, 0xFE, 0x00, 0xFE, 0xFF, 0xFE, 0x32, 0x00, 0xFF, 0xFE, 0x7F, 0x00, 0xFF, 
	0xFE, 0xD8, 0xB2, 0xFF, 0xFE, 0xFE, 0x00, 0xFF, 0xFE, 0xFE, 0x65, 0xFF, 0x00, 0xFE, 0x00, 0xFF, 
	0x4B, 0xFE, 0x65, 0xFF, 0x00, 0xFE, 0x54, 0xFF, 0x3F, 0xFE, 0x7F, 0xFF, 0xB9, 0xFE, 0xD0, 0xFF, 
	0xD0, 0xFE, 0xDF, 0xFF, 0x00, 0xFE, 0xFE, 0xFF, 0x4B, 0x98, 0xFE, 0xFF, 0x7F, 0x98, 0xE5, 0xFF, 
	0x32, 0x00, 0xFE, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xF8, 0xF8, 0xF8, 0xFF, 0xF2, 0xF2, 0xF2, 0xFF, 
	0xEC, 0xEC, 0xEC, 0xFF, 0xE6, 0xE6, 0xE6, 0xFF, 0xE0, 0xE0, 0xE0, 0xFF, 0xDA, 0xDA, 0xDA, 0xFF, 
	0xD4, 0xD4, 0xD4, 0xFF, 0xCE, 0xCE, 0xCE, 0xFF, 0xC8, 0xC8, 0xC8, 0xFF, 0xC2, 0xC2, 0xC2, 0xFF, 
	0xBC, 0xBC, 0xBC, 0xFF, 0xB6, 0xB6, 0xB6, 0xFF, 0xB0, 0xB0, 0xB0, 0xFF, 0xAA, 0xAA, 0xAA, 0xFF, 
	0xA4, 0xA4, 0xA4, 0xFF, 0x62, 0xA7, 0x79, 0xFF, 0x67, 0xAC, 0x7E, 0xFF, 0x6C, 0xB1, 0x83, 0xFF, 
	0x74, 0xB9, 0x8B, 0xFF, 0x79, 0xBE, 0x90, 0xFF, 0x7E, 0xC3, 0x95, 0xFF, 0x83, 0xC8, 0x9A, 0xFF, 
	0x88, 0xCD, 0x9F, 0xFF, 0x90, 0xD5, 0xA7, 0xFF, 0x95, 0xDA, 0xAC, 0xFF, 0x9A, 0xDF, 0xB1, 0xFF, 
	0xA2, 0xE7, 0xB9, 0xFF, 0xA7, 0xEC, 0xBE, 0xFF, 0xAC, 0xF1, 0xC3, 0xFF, 0xB4, 0xF9, 0xCB, 0xFF, 
	0x77, 0xA5, 0x86, 0xFF, 0x7D, 0xAB, 0x8C, 0xFF, 0x83, 0xB1, 0x92, 0xFF, 0x89, 0xB7, 0x98, 0xFF, 
	0x8F, 0xBD, 0x9E, 0xFF, 0x95, 0xC3, 0xA4, 0xFF, 0x9B, 0xC9, 0xAA, 0xFF, 0xA1, 0xCF, 0xB0, 0xFF, 
	0xA7, 0xD5, 0xB6, 0xFF, 0xAD, 0xDB, 0xBC, 0xFF, 0xB3, 0xE1, 0xC2, 0xFF, 0xB9, 0xE7, 0xC8, 0xFF, 
	0xBF, 0xED, 0xCE, 0xFF, 0xC5, 0xF3, 0xD5, 0xFF, 0xCB, 0xF8, 0xDA, 0xFF, 0x7F, 0x7F, 0x7F, 0xFF, 
	0x07, 0x07, 0x07, 0xFF, 0x0F, 0x0F, 0x30, 0xFF, 0x11, 0x22, 0x11, 0xFF, 0x18, 0x28, 0x30, 0xFF, 
	0x28, 0x28, 0x28, 0xFF, 0x39, 0x0F, 0x07, 0xFF, 0x30, 0x07, 0x30, 0xFF, 0x07, 0x30, 0x51, 0xFF, 
	0x28, 0x41, 0x39, 0xFF, 0x30, 0x72, 0x6A, 0xFF, 0x7A, 0x49, 0x0F, 0xFF, 0x59, 0x59, 0x59, 0xFF, 
	0x49, 0x51, 0x62, 0xFF, 0x49, 0x6A, 0x51, 0xFF, 0x51, 0x72, 0x6A, 0xFF, 0x7A, 0x7A, 0x7A, 0xFF, 
	0x59, 0x30, 0x07, 0xFF, 0x49, 0x07, 0x59, 0xFF, 0x62, 0x7A, 0xC4, 0xFF, 0x51, 0xF6, 0x6A, 0xFF, 
	0x7A, 0xB4, 0x8B, 0xFF, 0xF6, 0x83, 0x0F, 0xFF, 0x92, 0x93, 0x93, 0xFF, 0x93, 0xA4, 0xD5, 0xFF, 
	0x8B, 0xC4, 0x93, 0xFF, 0x91, 0xD5, 0xA4, 0xFF, 0x8B, 0xF6, 0x93, 0xFF, 0x93, 0xE5, 0xB4, 0xFF, 
	0xA4, 0xF6, 0xB4, 0xFF, 0xB9, 0xCC, 0x83, 0xFF, 0x95, 0xD6, 0xC6, 0xFF, 0xAB, 0xB5, 0xE6, 0xFF, 
	0x93, 0xE5, 0xC4, 0xFF, 0xB4, 0xF6, 0xC4, 0xFF, 0xC1, 0xF6, 0xE5, 0xFF, 0xC3, 0xCD, 0xDF, 0xFF, 
	0xB4, 0xC4, 0xE5, 0xFF, 0xF6, 0xD5, 0xB4, 0xFF, 0xBC, 0x07, 0xCD, 0xFF, 0xD5, 0xD5, 0xE5, 0xFF, 
	0xD5, 0xF2, 0xD9, 0xFF, 0xD5, 0xF6, 0xE5, 0xFF, 0xF6, 0xE8, 0xE2, 0xFF, 0xE5, 0xE5, 0xF6, 0xFF, 
	0xEB, 0xD7, 0xCB, 0xFF, 0xDD, 0xBC, 0x9B, 0xFF, 0xF6, 0x9B, 0xF6, 0xFF, 0xE5, 0x62, 0x0F, 0xFF, 
	0xF6, 0x07, 0xF6, 0xFF, 0xB4, 0xF6, 0xD5, 0xFF, 0x0F, 0x9B, 0xCD, 0xFF, 0xE5, 0xF6, 0xF6, 0xFF, 
	0x30, 0x07, 0xEE, 0xFF, 0xD5, 0xDD, 0xF6, 0xFF, 0xE6, 0xF5, 0xE6, 0xFF, 0xB4, 0x83, 0x0F, 0xFF, 
	0x07, 0xB4, 0x0F, 0xFF, 0x09, 0x07, 0x16, 0xFF, 0x18, 0x07, 0x18, 0xFF, 0x07, 0x18, 0x18, 0xFF, 
	0x18, 0x18, 0x18, 0xFF, 0x07, 0x62, 0x18, 0xFF, 0xF6, 0xBC, 0xF6, 0xFF, 0xC4, 0xD2, 0xF6, 0xFF, 
	0x0F, 0xEE, 0x20, 0xFF, 0x29, 0x38, 0x28, 0xFF, 0x93, 0x83, 0x28, 0xFF, 0x0F, 0x8B, 0x28, 0xFF, 
	0x62, 0x41, 0x30, 0xFF, 0x39, 0x39, 0x39, 0xFF, 0x9B, 0x51, 0x39, 0xFF, 0x72, 0x62, 0x39, 0xFF, 
	0xBC, 0x9B, 0x39, 0xFF, 0x20, 0xBC, 0x39, 0xFF, 0x28, 0x28, 0x41, 0xFF, 0x41, 0x59, 0x41, 0xFF, 
	0xF6, 0x9B, 0x41, 0xFF, 0x49, 0x49, 0x49, 0xFF, 0x30, 0x51, 0x49, 0xFF, 0x30, 0x6A, 0x49, 0xFF, 
	0xCD, 0x7A, 0x49, 0xFF, 0x30, 0xEE, 0x49, 0xFF, 0x93, 0xAC, 0x51, 0xFF, 0x0F, 0xF6, 0x51, 0xFF, 
	0x18, 0x49, 0x59, 0xFF, 0x30, 0x93, 0x59, 0xFF, 0xB4, 0xB4, 0x59, 0xFF, 0x41, 0xBC, 0x59, 0xFF, 
	0x30, 0x41, 0x62, 0xFF, 0x9B, 0x72, 0x62, 0xFF, 0x6A, 0x83, 0x62, 0xFF, 0x62, 0x30, 0x6A, 0xFF, 
	0x0F, 0x51, 0x6A, 0xFF, 0x6A, 0x6A, 0x6A, 0xFF, 0xE5, 0x9B, 0x6A, 0xFF, 0x30, 0xF6, 0x6A, 0xFF, 
	0x51, 0x8B, 0x72, 0xFF, 0x51, 0xAC, 0x72, 0xFF, 0xF6, 0xB4, 0x72, 0xFF, 0x51, 0xCD, 0x72, 0xFF, 
	0x6A, 0x93, 0x7A, 0xFF, 0xBC, 0x93, 0x7A, 0xFF, 0x8B, 0x94, 0xD5, 0xFF, 0x6A, 0x07, 0x83, 0xFF, 
	0x59, 0x62, 0x83, 0xFF, 0x0F, 0x6A, 0x83, 0xFF, 0x6A, 0x7A, 0x83, 0xFF, 0x8B, 0x9B, 0x83, 0xFF, 
	0x18, 0x0F, 0x8B, 0xFF, 0x19, 0x32, 0x7A, 0xFF, 0x41, 0x59, 0x8B, 0xFF, 0x8B, 0x8B, 0x8B, 0xFF, 
	0x49, 0x93, 0x8B, 0xFF, 0x7A, 0x93, 0x8B, 0xFF, 0x6A, 0xA4, 0x8B, 0xFF, 0x6A, 0xB4, 0x8C, 0xFF, 
	0x72, 0xE5, 0x8B, 0xFF, 0x51, 0xF6, 0x8B, 0xFF, 0x83, 0x51, 0x93, 0xFF, 0x20, 0x7A, 0x93, 0xFF, 
	0x58, 0xA6, 0xF6, 0xFF, 0x51, 0xAC, 0x93, 0xFF, 0xAC, 0x49, 0x07, 0xFF, 0x8B, 0xB4, 0x93, 0xFF, 
	0x7A, 0xC4, 0x93, 0xFF, 0xF6, 0xC4, 0x93, 0xFF, 0x62, 0x49, 0xEE, 0xFF, 0xB4, 0xDD, 0x93, 0xFF, 
	0x6A, 0xF6, 0x93, 0xFF, 0x6A, 0x72, 0x9B, 0xFF, 0x8B, 0x83, 0x9B, 0xFF, 0x30, 0x8B, 0x9B, 0xFF, 
	0x6A, 0xA4, 0x9B, 0xFF, 0x7A, 0xA4, 0x9B, 0xFF, 0x7A, 0xB4, 0x9B, 0xFF, 0x62, 0xCD, 0x9B, 0xFF, 
	0xE5, 0xBC, 0xE5, 0xFF, 0x93, 0x07, 0xA4, 0xFF, 0x51, 0x6A, 0xA4, 0xFF, 0xBC, 0xA4, 0xA4, 0xFF, 
	0xA4, 0xB4, 0xA4, 0xFF, 0xBC, 0xB4, 0xA4, 0xFF, 0x8B, 0xC4, 0xA4, 0xFF, 0xF6, 0x59, 0xF6, 0xFF, 
	0x07, 0xF6, 0xF6, 0xFF, 0xF6, 0xD5, 0xA4, 0xFF, 0x8E, 0xE5, 0xA4, 0xFF, 0xE5, 0x20, 0x0F, 0xFF, 
	0xA4, 0xE5, 0xA4, 0xFF, 0xB4, 0xE5, 0xA3, 0xFF, 0x7A, 0xF6, 0xA4, 0xFF, 0x8C, 0xF6, 0xA4, 0xFF, 
	0x83, 0x18, 0x07, 0xFF, 0xB3, 0xE7, 0xD5, 0xFF, 0xF6, 0x6A, 0xAC, 0xFF, 0x49, 0x93, 0xAC, 0xFF, 
	0x93, 0x93, 0xAC, 0xFF, 0x51, 0xAC, 0xAC, 0xFF, 0x93, 0xAC, 0xAC, 0xFF, 0x6A, 0x0F, 0xB4, 0xFF, 
	0xB7, 0x39, 0xC7, 0xFF, 0x72, 0x8B, 0xC5, 0xFF, 0x62, 0xB4, 0xB4, 0xFF, 0xA4, 0xB4, 0xB4, 0xFF, 
	0x7A, 0xC4, 0xB4, 0xFF, 0xCD, 0xC5, 0xB4, 0xFF, 0xEE, 0xC4, 0xB4, 0xFF, 0x90, 0xD5, 0xB4, 0xFF, 
	0xF6, 0xF6, 0x07, 0xFF, 0xE5, 0xD5, 0xB4, 0xFF, 0x83, 0xE5, 0xB4, 0xFF, 0xB4, 0xE5, 0xB4, 0xFF, 
	0xCD, 0xE5, 0xB4, 0xFF, 0xF6, 0xE5, 0xB4, 0xFF, 0x92, 0xF6, 0xB4, 0xFF, 0x59, 0x93, 0x18, 0xFF, 
	0xB4, 0xF6, 0xB4, 0xFF, 0xB8, 0x6E, 0xC9, 0xFF, 0x3E, 0x7C, 0xCE, 0xFF, 0xA4, 0xA4, 0xBC, 0xFF, 
	0x8B, 0xC4, 0xBC, 0xFF, 0xBC, 0x9B, 0xC4, 0xFF, 0x78, 0xA9, 0xC8, 0xFF, 0xAA, 0xB6, 0xD3, 0xFF, 
	0xD5, 0xC4, 0xC4, 0xFF, 0xC4, 0xDF, 0xCB, 0xFF, 0xD5, 0xDB, 0xCA, 0xFF, 0xA4, 0xE5, 0xC4, 0xFF, 
	0x51, 0xEE, 0xEE, 0xFF, 0xC4, 0xE5, 0xEA, 0xFF, 0xA4, 0xF6, 0xC4, 0xFF, 0xC4, 0xF6, 0xC4, 0xFF
};

LPCSTR get_ign_palette(void)
{
	return (LPCSTR)&ign_palette;
}

/*
void CIGNTileDatabase::DumpDatabaseBMP(void)
{
BITMAPFILEHEADER	bmp_file_header;
BITMAPINFOHEADER	bmp_info_header;
FILE * fp;

	memset(ign_palette,0xff,sizeof(ign_palette));
	ign_palette[4]=0;
	ign_palette[5]=0;
	ign_palette[6]=0;
	ign_palette[7]=0;
	fp = fopen("overview.bmp", "wb+");
	if (fp == NULL) {
		printf("Couldn't overwrite overview.bmp\n");
		return;
	}

	memset(&bmp_info_header, 0, sizeof(bmp_info_header));
   	bmp_info_header.biSize = sizeof(BITMAPINFOHEADER);
	bmp_info_header.biWidth = BMP_FRANCE_WIDTH;
	bmp_info_header.biHeight = BMP_FRANCE_HEIGHT;
	bmp_info_header.biPlanes = 1;
	bmp_info_header.biBitCount = 8;
	bmp_info_header.biCompression = BI_RGB;		// uncompressed bitmap

	// Compute the number of bytes in the array of color
	//indices and store the result in biSizeImage.

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
	fwrite(ign_palette, 256*4, 1, fp);						// colour palette

	unsigned char * bitmap = (unsigned char *)malloc(BMP_FRANCE_WIDTH*BMP_FRANCE_HEIGHT);
	memset(bitmap, 0, BMP_FRANCE_WIDTH*BMP_FRANCE_HEIGHT);

	m_readonly = true;
	for (long y0=6100; y0<7200; y0+=100) {
		for (long x0=0; x0<1200; x0+=100) {
			for (long y=0; y<100; y++) {
				for (long x=0; x<100; x++) {
					bitmap[(y0-6100+y)*BMP_FRANCE_WIDTH+x0-0+x] = TileLoaded((y0+y)*1000, (x0+x)*1000) ? 1 : 0;
				}
			}
		}
	}
	m_readonly = false;
	fwrite(bitmap, 1, BMP_FRANCE_WIDTH*BMP_FRANCE_HEIGHT, fp);
	fclose(fp);
	free(bitmap);
}
*/

#define	TILE_WIDTH	400

int IGN_read_bmp(long northing, long easting, CIGNTileDatabase * db, unsigned char * bm_ptr, long bitmap_width)
{
	// load file
	long tileSize=0;
	LPBYTE data = db->ReadTile(northing, easting, &tileSize);
	if (data == NULL) {
		return 1;		// couldn't load file
	}
	char strTilename[256];
	sprintf(strTilename, "%d-%d\r", easting/1000, northing/1000);
//	printf("%s\r", strTilename);

	unsigned char * bmp = (unsigned char *)malloc(161078);
	if (bmp == NULL) {
		free(data);
		printf("Couldn't allocate 161078 bytes for bitmap\n");
		return 1;
	}


	// decode zlib encoded data

	int ret;
	z_stream strm;

	/* allocate inflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	ret = inflateInit(&strm);
	if (ret != Z_OK) {
		free(bmp);
		free(data);
		return ret;
	}

	strm.avail_in = tileSize;
	strm.next_in = data;
	strm.avail_out = 161078;
	strm.next_out = bmp;
	ret = inflate(&strm, Z_NO_FLUSH);
	(void)inflateEnd(&strm);

	if (memcmp(bmp+0x36, ign_palette, 0x400) != 0) {
		printf("tile %d %d has a different palette\n", easting/1000, northing/1000);
	}

/*
	FILE * fp = fopen("out.bmp", "wb+");
	fwrite(bmp, 1,161078, fp);
	fclose(fp);
*/
	long y,x;
	for (y=0; y < TILE_WIDTH; y++) {
		for (x=0; x<TILE_WIDTH; x++) {
			bm_ptr[x+y*bitmap_width] = bmp[(TILE_WIDTH-1-y)*TILE_WIDTH+x+0x436];
		}
	}

	free(bmp);
	free(data);
	return 0;
}

// ToDo: Make it work using Lambert93 instead of Equidistance Lat/Lon (probably)
void IGN_Create_OZI_MAP(const char * fname, long minx, long miny, long maxx, long maxy)
{
FILE * fp;
char OSvalue[128];

	sprintf(OSvalue, "%s.map", fname);
	fp = fopen(OSvalue, "w+");
	if (fp == NULL) {
		printf("Couldn't create %s\n", fname);
		return;
	}

	// Header
	fprintf (fp,"OziExplorer Map Data File Version 2.2\n");
	fprintf (fp,"IGN 1:25.000 - %s\n", fname);
	fprintf (fp,"%s.png\n", fname);
	fprintf (fp,"1 ,Map Code,\n");
	fprintf (fp,"WGS 84, WGS 84,   0.0000,   0.0000,WGS 84\n");
	fprintf (fp,"Reserved 1\n");
	fprintf (fp,"Reserved 2\n");
	fprintf (fp,"Magnetic Variation,,,E\n");
	fprintf (fp,"Map Projection,Latitude/Longitude,PolyCal,No,AutoCalOnly,No,BSBUseWPX,Yes\n");

	struct {
		long e;
		long n;
	} MapBounds[4] = {
		{minx, maxy},
		{maxx, maxy},
		{maxx, miny},
		{minx, miny}
	};

	//	Dump Points
	//	e.g.
	//	Point01,xy,     0,     0,in, deg,  45, 48.90592137,N,   6, 42.79800443,E, grid,   ,           ,           ,N
	//	Point02,xy,  4000,     0,in, deg,  45, 48.60112785,N,   6, 50.50502958,E, grid,   ,           ,           ,N
	//	Point03,xy,  4000,  4000,in, deg,  45, 43.21195185,N,   6, 50.06361276,E, grid,   ,           ,           ,N
	//	Point04,xy,     0,  4000,in, deg,  45, 43.51624444,N,   6, 42.36912227,E, grid,   ,           ,           ,N

	for (int i=0 ; i < 30; i++)
	{
		if (i < 4)
		{
			double dLat, dLon;
			Lambert93ToWGS84(MapBounds[i].n, MapBounds[i].e, &dLat, &dLon);
	

			fprintf (fp,"Point%02d,xy,%5d,%5d,in, deg,%4d,%8.5f,%c,%4d,%8.5f,%c, grid,   ,           ,           ,N\n",

				i+1,labs(MapBounds[i].e-MapBounds[0].e)*2/5,labs(MapBounds[i].n-MapBounds[0].n)*2/5,
						(long)fabs(dLat), 60*fmod(fabs(dLat), 1.0), dLat < 0 ? 'S' : 'N',
						(long)fabs(dLon), 60*fmod(fabs(dLon), 1.0), dLon < 0 ? 'W' : 'E'
						);
		} 
		else
		{
			fprintf (fp,"Point%02d,xy,     ,     ,in, deg,    ,        ,N,    ,        ,E, grid,   ,           ,           ,N\n",i+1);
		}
	}

	// Dump Projection details etc...
	fprintf (fp,"Projection Setup,    45.000000000,     5.000000000,     0.999600000,            0.00,            0.00,    49.000000000,    45.000000000,,,\n");
	fprintf (fp,"Map Feature = MF ; Map Comment = MC     These follow if they exist\n");
	fprintf (fp,"Track File = TF      These follow if they exist\n");
	fprintf (fp,"Moving Map Parameters = MM?    These follow if they exist\n");
	fprintf (fp,"MM0,Yes\n");

/*
MMPNUM,4
MMPXY,1,0,0
MMPXY,2, 4000,0
MMPXY,3, 4000, 4000
MMPXY,4,0, 4000
*/
	// Dump MMPXY
	fprintf (fp,"MMPNUM,%d\n", 4);
	for (int i=0 ; i < 4; i++)
	{
		fprintf (fp,"MMPXY,%d,%d,%d\n",i+1, labs(MapBounds[i].e-MapBounds[0].e)*2/5,labs(MapBounds[i].n-MapBounds[0].n)*2/5);
	}

/*
MMPLL,1,  6.71330007, 45.81509869
MMPLL,2,  6.84175049, 45.81001880
MMPLL,3,  6.83439355, 45.72019920
MMPLL,4,  6.70615204, 45.72527074
*/
	// Dump MMPLL
	for (int i=0 ; i < 4; i++)
	{
		double dLat, dLon;
		Lambert93ToWGS84(MapBounds[i].n, MapBounds[i].e, &dLat, &dLon);
		fprintf (fp,"MMPLL,%d,  %.8f, %.8f\n",i+1, dLon, dLat);
	}
/*
MM1B,2.471979
MOP,Map Open Position,1500,1700
IWH,Map Image Width/Height, 4000, 4000
*/
// Footer
	fprintf (fp,"MM1B,2.500000\n");
	fprintf (fp,"MOP,Map Open Position,0,0\n");
	
	// Image Size
	fprintf (fp,"IWH,Map Image Width/Height,%d,%d\n",labs(maxx-minx)*2/5,labs(maxy-miny)*2/5);

	fclose(fp);
}

void IGN_dumpjprfile(const char * fname, long minx, long miny, long maxx, long maxy, long width, long height, const char * missing_squares)
{
FILE * fp;
char OSvalue[128];
double lat, lon;

	sprintf(OSvalue, "%s.jpr", fname);
	fp = fopen(OSvalue, "w+");
	if (fp == NULL) {
		printf("Couldn't create %s\n", fname);
		return;
	}
	
	fprintf(fp, "// %d %d to %d %d\n", minx, maxy, maxx, miny);
	fprintf(fp, "vr=1.14\n");
	fprintf(fp, "nm=%s\n", fname);
	fprintf(fp, "cu=Metres\n"
				"ci=10\n"
				"du=Metres\n");
	fprintf(fp, "ed=1\n"
				"et=2004\n");
	fprintf(fp, "it=PNG\n");
	fprintf(fp, "pr=UTM\n");
	fprintf(fp, "dm=WGS84\n");
	fprintf(fp, "sc=25000\n"
				"st=0.000000\n"
				"sn=0.000000\n"
				"su=Land\n");


	Lambert93ToWGS84(maxy, minx, &lat, &lon);
	fprintf(fp, "rp1=%.8f,%.8f,%d,%d\n", lat,lon,0,0);
	Lambert93ToWGS84(maxy, maxx, &lat, &lon);
	fprintf(fp, "rp2=%.8f,%.8f,%d,%d\n", lat,lon,width,0);
	Lambert93ToWGS84(miny, maxx, &lat, &lon);
	fprintf(fp, "rp3=%.8f,%.8f,%d,%d\n", lat,lon,width,height);
	Lambert93ToWGS84(miny, minx, &lat, &lon);
	fprintf(fp, "rp4=%.8f,%.8f,%d,%d\n", lat,lon,0,height);

	// Assume rectangular region
	fprintf(fp, "vp1=0,0\n");
	fprintf(fp, "vp2=%d,0\n",width);
	fprintf(fp, "vp3=%d,%d\n",width,height);
	fprintf(fp, "vp4=0,%d\n",height);
	fclose(fp);
}

bool IGN_Create_PNG_JPR(LPCSTR mapname, long minx, long miny, long maxx, long maxy)
{
char tstring[256];
long bitmap_width = TILE_WIDTH*(maxx-minx)/1000;
long bitmap_size = bitmap_width*TILE_WIDTH;
unsigned char * bitmap_memory = (unsigned char *)malloc(bitmap_size);
CIGNTileDatabase	db;


	if (bitmap_memory == NULL) {
		printf("Couldn't allocate %d bytes of memory\n");
		return false;
	}


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
	png_set_IHDR(png_ptr, info_ptr, TILE_WIDTH*(maxx-minx)/1000, TILE_WIDTH*((maxy-miny)/1000), 8, PNG_COLOR_TYPE_PALETTE,
				PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	/* set the palette if there is one.  REQUIRED for indexed-color images */
	png_colorp png_palette = (png_colorp)png_malloc(png_ptr, PNG_MAX_PALETTE_LENGTH * sizeof (png_color));


	int i;
	BYTE * pal_ptr = (BYTE *)png_palette;
	BYTE * palette = ign_palette;
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

CProgressWindow wndProgress;
wndProgress.Initialize();

wndProgress.ResetProgressBar("Tile:", (maxy-miny)/500*(maxx-minx)/500);


	for (int y=maxy-1000; y>=miny; y-=1000) {
		memset(bitmap_memory, 0x00, bitmap_size);
		for (int x=minx; x<maxx; x+=1000) {
			IGN_read_bmp(y, x, &db, bitmap_memory+(x-minx)/1000*TILE_WIDTH, bitmap_width);
		}


		// write row of PNG to file
		for (int x=0; x<TILE_WIDTH; x++) {
			png_write_row(png_ptr, bitmap_memory + x*bitmap_width);

			if (!wndProgress.ProgressBar()) return false;
		}
	}
	/* It is REQUIRED to call this to finish writing the rest of the file */
	png_write_end(png_ptr, info_ptr);

	png_free(png_ptr, png_palette);
	
	/* clean up after the write, and free any memory allocated */
	png_destroy_write_struct(&png_ptr, &info_ptr);

	/* close the file */
	fclose(fp_png);

//	IGN_dumpjprfile(mapname, minx, miny, maxx, maxy, (maxx-minx)/1000 * TILE_WIDTH, (maxy-miny)/1000*TILE_WIDTH, NULL);
	free(bitmap_memory);
	return true;
}

/*
void IGN_read_pkg(char * filename)
{
FILE * fp;
long length;
unsigned char * filedata;
long offset,offset2;
long compressed_length, uncompressed_length;
long easting, northing;
CIGNTileDatabase	db;


	fp = fopen(filename, "rb");
	if (fp == NULL) {
		printf("Couldn't open %s\n", filename);
	}
	fseek(fp, 0, SEEK_END);
	length = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	if ((filedata = (unsigned char *)malloc(length)) == NULL) {
		printf("Couldn't allocate %d bytes of memory for %s\n", length, filename);
		fclose(fp);
	}
	if (fread(filedata, 1, length, fp) != length) {
		printf("Error reading %s\n", filename);
		free(filedata);
		fclose(fp);
	}

	fclose(fp);

	offset = 0;
	while (offset < length) {
		if (filedata[offset] == '_' && isdigit(filedata[offset-1]) && isdigit(filedata[offset+1])) {
			offset2 = offset-1;
			while (isdigit(filedata[offset2]))
				offset2--;
			offset2++;
			if (*((long*)(filedata+offset2-8)) == 0 && *((long*)(filedata+offset2-12)) == 0) {
				break;
			}
		}
		offset++;
	}
	offset = offset2-4;
	while (offset < length) {
//		printf("%x\n", offset);
		compressed_length = *((long*)(filedata+offset));
		offset += 4;
		sscanf((char*)(filedata+offset), "%d_%d", &easting, &northing);
		offset += 0x100;
		uncompressed_length = *((long*)(filedata+offset));
		if (db.TileLoaded(northing*1000, easting*1000)) {
			printf("%d-%d present\n", easting, northing);
		} else {
			printf("%d-%d adding\n", easting, northing);
			db.WriteTile(northing*1000, easting*1000, filedata+offset+4, compressed_length-4);
		}
		offset += compressed_length;
	}

	free(filedata);
}
*/

#define N_IGN_ZONES	84

bool IsInIGNDatabase(long northing, long easting)
{

	struct map {
		char *	strIGNZone;
	} zones[] = {

		 "0_68",
		 "1_67",  "1_68",
		 "2_66",  "2_67",  "2_68",
		 "3_62",  "3_63",  "3_64",  "3_65",  "3_66",  "3_67",  "3_68",  "3_69",
		 "4_61",  "4_62",  "4_63",  "4_64",  "4_65",  "4_66",  "4_67",  "4_68",  "4_69",
		 "5_61",  "5_62",  "5_63",  "5_64",  "5_65",  "5_66",  "5_67",  "5_68",  "5_69",  "5_70",
		 "6_61",  "6_62",  "6_63",  "6_64",  "6_65",  "6_66",  "6_67",  "6_68",  "6_69",  "6_70",  "6_71",
		 "7_61",  "7_62",  "7_63",  "7_64",  "7_65",  "7_66",  "7_67",  "7_68",  "7_69",  "7_70",
		 "8_62",  "8_63",  "8_64",  "8_65",  "8_66",  "8_67",  "8_68",  "8_69",  "8_70",
		 "9_62",  "9_63",  "9_64",  "9_65",  "9_66",  "9_67",  "9_68",  "9_69",
		"10_62", "10_63", "10_64", "10_65", "10_66", "10_67", "10_68", "10_69",
		"11_60", "11_61",
		"12_60", "12_61", "12_62",
	};

	CString strZone;
	strZone.Format("%d_%d", easting/100000, northing/100000);
	
	for (int i=0; i < N_IGN_ZONES; i++) {
		if (strZone == zones[i].strIGNZone) {
			return true;
		}
	}

	return false;
}

bool IGN_ReadTiles(long minx, long maxx, long miny, long maxy, CProgressWindow& wndProgress, 
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

			if (!wndProgress.ProgressBar()) return false;

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

/*
				if (bitmap_width > 32000)
					printf("Warning: width is %d pixels\n", bitmap_width);
				if (bitmap_height > 32000)
					printf("Warning: height is %d pixels\n", bitmap_height);
*/

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
