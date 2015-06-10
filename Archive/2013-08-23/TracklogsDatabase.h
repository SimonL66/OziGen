#ifndef TRACKLOGSDATABASE23
#define	TRACKLOGSDATABASE23

// Tracklogs package format (simplified -- hopefully doesn't change too much)

#pragma pack(push,1)
	struct smsg_hdr {
		unsigned long length;		// 0
		unsigned long unknown1;		// 4
		unsigned short type;		// 8
		unsigned long id;			// 0x0a
		unsigned long unknown2[2];	// 0x0e
		unsigned short revision;	// 0x16
		unsigned short unknown3[3];	// 0x18
//		unsigned short unknown3[3+14];	// 0x18
		unsigned long width;		// 0x1e
		unsigned short unknown4[3];	// 0x22
		unsigned long height;		// 0x28
		unsigned short unknown5[3];	// 0x2c
		unsigned long format;		// 0x32
		unsigned short unknown6;	// 0x36
		unsigned long palette_size;	// 0x38
		unsigned char palette[1024];// 0x3c
		unsigned short unknown7;	// 0x43c
		unsigned long raster_size;	// 0x43e
		unsigned char raster[1];	// 0x442
	};
#pragma pack(pop)

//
// CTracklogsDatabase23
//
// Wrapper class for handling either Tracklogs 2 or 3 databases
//
class CTracklogsDatabase23
{
private:
	bool	m_bUseTL3;

	// TL3 variables
	CIndexFile *	m_pTL3IndexFile;
	CDatFile *		m_pTL3DatFile;
	BYTE *			m_tile_buffer;

	// TL2 variables
	CTracklogDatabase * m_pTL2dbase;
	CDaoRecordset *	m_pRecords;
	COleVariant		m_varPalette;
	COleVariant		m_varRaster;
	CDaoRecordset * m_records;

public:
	CTracklogsDatabase23(long useTL3);
	~CTracklogsDatabase23();

	bool OpenedOK(void);										// returns true if database has opened
	bool GetTile(	long id,									// retrieves tile id, or returns false
					long * width, long * height,
					long * palette_length, BYTE ** palette,
					long * raster_length, BYTE ** raster,
					long * revision);
};
#endif
