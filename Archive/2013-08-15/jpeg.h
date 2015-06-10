#pragma once

class JPEG_tile : public CObject
{
public:
	JPEG_tile() { };
	~JPEG_tile() { };
	long offset_x0;
	long offset_x1;
	long offset_y0;
	long offset_y1;
	double lat_north;
	double lon_east;
	double lat_south;
	double lon_west;
	CString fname;
};

class JPEG_tile_list : public CObList
{
public:
	JPEG_tile_list() { };
	~JPEG_tile_list() { };
	void RemoveAll(void) {
		while (!IsEmpty()) {
			JPEG_tile * tile = (JPEG_tile *)RemoveTail();
			delete tile;
		}
		CObList::RemoveAll();
	}
};

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
DWORD appendJPG(CString fname, FILE * fid);
