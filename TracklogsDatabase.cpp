#include "stdafx.h"
#include "..\MapLib\map_conv.h"
#include "..\MapLib\db_TL3.h"
#include "TracklogsDatabase.h"

#define printf visualprintf

long	g_use_TL3 = false;

//
// constructor
//
CTracklogsDatabase23::CTracklogsDatabase23(long useTL3)
{
	m_bUseTL3 = useTL3 != 0;

	m_pTL3IndexFile = NULL;
	m_pTL3DatFile = NULL;
	m_tile_buffer = NULL;
	m_records = NULL;
	m_pTL2dbase = NULL;

	if (m_bUseTL3) {
		CString db3path = GetTracklogsDatabase3Path();
		m_pTL3IndexFile = new CIndexFile(db3path + "TrackLogsData.idx");
		if (!m_pTL3IndexFile->OpenedOK()) {
			printf("Couldn't open Tracklogs 3 index\n");
		}
		m_pTL3DatFile = new CDatFile(db3path + "TrackLogsData");
	} else {
		AfxDaoInit();
		m_pTL2dbase = new CTracklogDatabase();
	}
}

//
// destructor
//
CTracklogsDatabase23::~CTracklogsDatabase23()
{
	if (m_pTL3IndexFile) {
		delete m_pTL3IndexFile;
		m_pTL3IndexFile = NULL;
	}
	if (m_pTL3DatFile) {
		delete m_pTL3DatFile;
		m_pTL3DatFile = NULL;
	}
	if (m_tile_buffer != NULL) {
		free(m_tile_buffer);
		m_tile_buffer = NULL;
	}
	if (m_records) {
		delete m_records;
		m_records = NULL;
	}
	if (m_pTL2dbase) {
		delete m_pTL2dbase;
		m_pTL2dbase = NULL;
		AfxDaoTerm();
	}
}

//
// OpenedOK
//
// returns true if database has been opened correctly
//
bool CTracklogsDatabase23::OpenedOK()
{
	if (m_bUseTL3) {
		return m_pTL3IndexFile != NULL && m_pTL3DatFile != NULL;
	} else {
		return m_pTL2dbase != NULL;
	}
}

bool CTracklogsDatabase23::GetTile(	long id,									// retrieves tile id, or returns false
									long * width, long * height,
									long * palette_length, BYTE ** palette,
									long * raster_length, BYTE ** raster,
									long * revision)
{
	if (m_bUseTL3) {
		// Tracklogs 3
		if (m_pTL3IndexFile == NULL || m_pTL3DatFile == NULL)
			return false;

		if (m_tile_buffer != NULL) {
			// free old tile contents
			free(m_tile_buffer);
			m_tile_buffer = NULL;
		}

		struct _block_hdr hdr;
		memset(&hdr, 0, sizeof(hdr));
		hdr.type = 0x0a;				// OS 1:25,000 Explorer
		hdr.id = id;
		long block = m_pTL3IndexFile->FindValue((unsigned char*)&hdr);
		if (block < 0)
			return false;

		m_tile_buffer = m_pTL3DatFile->ReadObject(block);
		struct smsg_hdr * tile = (struct smsg_hdr *)(m_tile_buffer+4);

		*width = tile->width;
		*height = tile->height;

		// extract palette
		*palette_length = tile->palette_size;
		*palette = (BYTE *)tile->palette;

		// extract raster data
		*raster_length = tile->raster_size;
		*raster = tile->raster;

		*revision = tile->revision;
	} else {
		// Tracklogs 2
		if (m_pTL2dbase == NULL)
			return false;

		if (m_records != NULL)
			delete m_records;

		m_records = NULL;

		m_records = m_pTL2dbase->GetRecords(id/10000, id%10000);
		if (m_records == NULL)
			return false;

		*width = m_records->GetFieldValue("width").lVal;
		*height = m_records->GetFieldValue("height").lVal;

		// extract palette
		m_varPalette = m_records->GetFieldValue("palette");
		SAFEARRAY * array = m_varPalette.parray;
		*palette_length = array->rgsabound[0].cElements;
		*palette = (BYTE *)array->pvData;

		// extract raster data
		m_varRaster = m_records->GetFieldValue("raster");
		array = m_varRaster.parray;
		*raster_length = array->rgsabound[0].cElements;
		*raster = (BYTE *)array->pvData;

		*revision = m_records->GetFieldValue("revision").iVal;
	}
	return true;
}
