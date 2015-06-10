#pragma once

#include "..\MapLib\TileDatabase.h"

/*
class CTileDatabase
{
private:
	HANDLE	m_hFile;
	CHAR	m_filename[MAX_PATH];
	bool	m_readonly;

	struct {
		DWORD	m_magic;
		long	m_easting;
		long	m_northing;
		long	m_scale;
		DWORD	m_padding[4];
		DWORD	m_offsets[200*200];		// 0=unused; 1=doesn't exist; else tile offset => DWORD length, data
	} m_fileheader;

public:
	CTileDatabase();
	~CTileDatabase();

	void InitDatabase(long northing, long easting);

	LPBYTE ReadTile(long northing, long easting, long * size);
	void WriteTile(long northing, long easting, LPBYTE data, long size);
	BOOL TileLoaded(long northing, long easting);
	void DumpDatabaseBMP(void);
	void DeleteTile(long northing, long easting);

	void SetReadonly(bool readonly) { m_readonly = readonly; };
};
*/

int read_png(long northing, long easting, CTileDatabase * db, unsigned char * bm_ptr, long bitmap_width);

void Create_PNG_JPR(LPCSTR mapname, long minx, long maxx, long miny, long maxy);

int LoadTiles(CTileDatabase* g_db, long minx, long miny, long maxx, long maxy, BOOL bForce);

#define OS_PRO_DATABASE_PATH	"db"

bool IsInOsDatabase(char* strZone);