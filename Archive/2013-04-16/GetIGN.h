#pragma once

class CIGNTileDatabase
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
		DWORD	m_offsets[100*100];		// 0=unused; 1=doesn't exist; else tile offset => DWORD length, data
	} m_fileheader;

public:
	CIGNTileDatabase();
	~CIGNTileDatabase();

	void InitDatabase(long northing, long easting);

	LPBYTE ReadTile(long northing, long easting, long * size);
	void WriteTile(long northing, long easting, LPBYTE data, long size);
	BOOL TileLoaded(long northing, long easting);
	void DumpDatabaseBMP(void);
	void DeleteTile(long northing, long easting);
};


int IGN_read_bmp(long northing, long easting, CIGNTileDatabase * db, unsigned char * bm_ptr, long bitmap_width);

void IGN_Create_PNG_JPR(LPCSTR mapname, long minx, long miny, long maxx, long maxy);

LPCSTR get_ign_palette(void);

void IGN_dumpjprfile(const char * fname, long minx, long miny, long maxx, long maxy, long width, long height, const char * missing_squares);

#define	IGN_TILE_WIDTH	400

void IGN_Create_OZI_MAP(const char * fname, long minx, long miny, long maxx, long maxy);
