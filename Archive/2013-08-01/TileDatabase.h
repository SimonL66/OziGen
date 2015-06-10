#pragma once

#define MAP_DATUM_OSGB		('O' + ('S'<<8)+ ('G'<<16)+ ('B'<<24))
#define MAP_DATUM_IGN		('I' + ('G'<<8)+ ('N'<<16)+ (' '<<24))
#define MAP_DATUM_OSNi		('O' + ('S'<<8)+ ('N'<<16)+ ('i'<<24))
#define MAP_DATUM_OSi		('O' + ('S'<<8)+ ('i'<<16)+ (' '<<24))
#define MAP_DATUM_CH		('C' + ('H'<<8)+ (' '<<16)+ (' '<<24))
#define MAP_DATUM_NGI		('N' + ('G'<<8)+ ('I'<<16)+ (' '<<24))
#define MAP_DATUM_AT		('A' + ('T'<<8)+ (' '<<16)+ (' '<<24))
#define MAP_DATUM_DE		('D' + ('E'<<8)+ (' '<<16)+ (' '<<24))
#define MAP_DATUM_NL		('N' + ('L'<<8)+ (' '<<16)+ (' '<<24))
#define MAP_DATUM_IT		('I' + ('T'<<8)+ (' '<<16)+ (' '<<24))
#define MAP_DATUM_ES		('E' + ('S'<<8)+ (' '<<16)+ (' '<<24))
#define MAP_DATUM_LUX		('L' + ('U'<<8)+ ('X'<<16)+ (' '<<24))
#define MAP_DATUM_PT		('P' + ('T'<<8)+ (' '<<16)+ (' '<<24))

#define MAP_GFX_FORMAT_PNG	('P' + ('N'<<8)+ ('G'<<16)+ ('d'<<24))
#define MAP_GFX_FORMAT_BMP	('B' + ('M'<<8)+ ('P'<<16)+ ('z'<<24))
#define MAP_GFX_FORMAT_JPEG	('J' + ('P'<<8)+ ('E'<<16)+ ('G'<<24))

#define MAP_GFX_TYPE_RASTER	('R' + ('A'<<8)+ ('S'<<16)+ ('T'<<24))
#define MAP_GFX_TYPE_VECTOR	('V' + ('E'<<8)+ ('C'<<16)+ ('T'<<24))
#define MAP_GFX_TYPE_STREET	('S' + ('T'<<8)+ ('R'<<16)+ ('E'<<24))

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
