// Don't worry about unsafe standard C library functions.
#define _CRT_SECURE_NO_WARNINGS
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit



#include "stdafx.h"
#include "targetver.h"

#include <afx.h>
#include <afxtempl.h>

#include <direct.h>

#include <Wininet.h>


#include "TileDatabase.h"

#include "..\MapLib\CoordConv.h"

#include "OziGen.h"

#include "CustomMap.h"


#define printf visualprintf

extern CCustomMap MyMap;

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
if (MyMap.GetCountry() == COUNTRY_UK) {
	sprintf(m_filename, "%s\\",MyMap.GetDatabasePath());
	easting_northing_to_OSref3(easting, northing, m_filename+strlen(m_filename));
	strcpy(m_filename+strlen(m_filename)-8, ".db");
} else if ((MyMap.GetProjection() == 3857) || (MyMap.GetProjection() == 900913)) {
	easting  *= m_scale;
	northing *= m_scale;
	sprintf(m_filename, "%s\\%d_%d.db", MyMap.GetDatabasePath(), easting/100000, northing/100000);
} else {
//	sprintf(m_filename, "%s\\%s\\%s_%s\\%d_%d.db", g_strMapDatabaseFolder, g_strMapCountry, g_strMapScale, g_strMapFlavour, easting/100000, northing/100000);
	sprintf(m_filename, "%s\\%d_%d.db", MyMap.GetDatabasePath(), easting/100000, northing/100000);
}

	if (northing/100000 == m_fileheader->m_northing && easting/100000 == m_fileheader->m_easting)
		return; // Database already open

	if (m_hFile != INVALID_HANDLE_VALUE) {
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
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
		printf("%s has a bad header\n", m_filename);
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
		return;
	}
}


BOOL CTileDatabase::TileLoaded(long northing, long easting)
{
	InitDatabase(northing, easting, 0);

if ((MyMap.GetProjection() == 3857) || (MyMap.GetProjection() == 900913)) {
	easting  *= m_scale;
	northing *= m_scale;
}
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

if ((MyMap.GetProjection() == 3857) || (MyMap.GetProjection() == 900913)) {
	easting  *= m_scale;
	northing *= m_scale;
}

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

if ((MyMap.GetProjection() == 3857) || (MyMap.GetProjection() == 900913)) {
	easting  *= m_scale;
	northing *= m_scale;
}

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
	
if ((MyMap.GetProjection() == 3857) || (MyMap.GetProjection() == 900913)) {
	easting  *= m_scale;
	northing *= m_scale;
}
	
	
	if (m_hFile == INVALID_HANDLE_VALUE)
		return;
	DWORD offset = northing%100000/m_fileheader->m_scale*(100000/m_fileheader->m_scale)+easting%100000/m_fileheader->m_scale;
	if (m_fileheader->m_offsets[offset]) {
//		printf("Replacing data of easting=%d, Northing=%d, %s\n", easting, northing, m_filename);
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
