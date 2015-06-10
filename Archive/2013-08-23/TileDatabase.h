#pragma once

#include "CustomMap.h"

#pragma warning(disable:4200)
struct tile_db_header {
	DWORD	m_magic;
	long	m_easting;
	long	m_northing;
	long	m_scale;
	DWORD	m_padding[4];
	DWORD	m_offsets[0];			// 0=unused; 1=doesn't exist; else tile offset => DWORD length, data
};
#pragma warning(default:4200)


class CTileDatabase
{
private:
	HANDLE	m_hFile;
	CHAR	m_filename[MAX_PATH];
	bool	m_readonly;
	long	m_scale;
	struct tile_db_header * m_fileheader;
	long	m_header_size;

	long	m_datum;

public:

	long	m_mapscale;
	long	m_nPixelsPerTile;
	long	m_nMetresPerTile;

public:
	// e.g. for a 1-25K Tile; 4x4 tiles = 1km, each tile is 500x500 meters.
	CTileDatabase(long nDatum=MAP_DATUM_OSGB, int nMetersPerTile=500, int nMapScale=25000, int nPixelsPerTile=200); 
	~CTileDatabase();

	void InitDatabase(long northing, long easting, DWORD dataum=MAP_DATUM_OSGB);

	LPBYTE ReadTile(long northing, long easting, long * size);
	void WriteTile(long northing, long easting, LPBYTE data, long size);
	BOOL TileLoaded(long northing, long easting);
	void DumpDatabaseBMP(void);
	void DeleteTile(long northing, long easting);

	void SetReadonly(bool readonly) { m_readonly = readonly; };

	void SetDatum(int nCountry);
};
