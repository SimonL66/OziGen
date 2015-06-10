#include "stdafx.h"
#include "jnx.h"

#include "OziGen.h"
#include "OziGenDlg.h"
#include "ProgressWindow.h"
#include "func.h"
#include "jpeg.h"

#include "OSPro.h"
#include "IGN.h"

#include "stdint.h"

#include <math.h>
#include <list>
#include <gdiplus.h>

#include "..\MapLib\CoordConv.h"
#include "..\MapLib\mmmath.h"
#include "..\MapLib\png.h"
#include "..\MapLib\Palette.h"
#include "..\MapLib\map_conv.h"
#include "..\MapLib\License.h"
#include "..\MapLib\db_TL3.h"
#include "..\MapLib\zip.h"

#include "TracklogsDatabase.h"

#include "Database.h"

#define printf visualprintf


#define JNX_MAX_TILES       50000 //6250

#define HEADER_BLOCK_SIZE   1024

#include "CustomMap.h"

extern CCustomMap MyMap;

struct jnx_hdr_t
{
    jnx_hdr_t(): version(0x00000004), devid(0), expire(0), productId(0), crc(0), signature(0), signature_offset(0), zorder(50){}
    uint32_t version;           // byte 00000000..00000003
    uint32_t devid;             // byte 00000004..00000007
    int32_t  top;               // byte 00000014..00000017
    int32_t  right;             // byte 00000010..00000013
    int32_t  bottom;            // byte 0000000C..0000000F
    int32_t  left;              // byte 00000008..0000000B
    uint32_t details;           // byte 00000018..0000001B
    uint32_t expire;            // byte 0000001C..0000001F
    uint32_t productId;         // byte 00000020..00000023
    uint32_t crc;               // byte 00000024..00000027
    uint32_t signature;         // byte 00000028..0000002B
    uint32_t signature_offset;  // byte 0000002C..0000002F
    uint32_t zorder;            // byte 00000030..00000033
};


struct jnx_level_t
{
    jnx_level_t(): nTiles(0), offset(0), scale(0), dummy(2){}

    uint32_t nTiles;
    uint32_t offset;
    uint32_t scale;
    uint32_t dummy;
};

struct jnx_tile_t
{
    jnx_tile_t() : top(0), right(0), bottom(0), left(0), width(0), height(0), size(0), offset(0){}
    int32_t  top;
    int32_t  right;
    int32_t  bottom;
    int32_t  left;
    uint16_t width;
    uint16_t height;
    uint32_t size;
    uint32_t offset;
};

struct file_t
{
//    file_t(): dataset(0), pj(0){memset(colortable,0, sizeof(colortable));}
    ~file_t()
    {
        //if(dataset) delete dataset;
//        if(pj) pj_free(pj);
    }

    bool operator<(const file_t& other)
    {
        return (xscale > other.xscale);
    }

//    std::string     filename;
//    std::string     projection;

	uint32_t        width;
    uint32_t        height;
    double          xscale;
    double          yscale;
    double          scale;
    double          xref1;
    double          yref1;
    double          xref2;
    double          yref2;

    double          lon1;
    double          lat1;
    double          lon2;
    double          lat2;
};

struct level_t : public jnx_level_t
{
    std::list<file_t *> files;
};

struct scale_t
{
    double scale;
    uint32_t jnxScale;
};

static uint32_t scale2jnx(double scale)
{
    /*
		Ok, I've made some calculations, and got the following formula to
		calculate the JNX scale (S) depending on the map's meters/pixel
		ratio (R):

		  S(R) =
			round(
			  76437 *
			  exp(
				ln(2.000032708011) *
				round(
				  ln(R * 130.2084 / 76437) /
				  ln(2.000032708011)
				)
			  )
			)


		where
		  round - is a function which returns the closest integer from
			floating point value, [unfortunately its defined in C99 but not standard C++]
		  exp - exponent,
		  ln - natural logarithm.

		Magic number 130.2084 - is an average value for
		  (JNX scale) / (maps meters per pixel)
		ratio among all zoom levels in metric system.

		Magic number 2.000032708011 is a ratio on which our standard scale
		table is built. It is (76437 / 4777) ^ (1/4).
    */

	return (uint32_t)floor(0.5 + 76437 * exp(log(2.000032708011) * floor(0.5 + log(scale * 10 * 130.2084 / 76437) / log(2.000032708011) ) ) );
}

static char randChar()
{
    char buf[2];
    int r = (int)((rand() * 16.0) / RAND_MAX);
    sprintf(buf,"%X", r & 0x0F);
    return buf[0];
}

static void createGUID(char * guid)
{
    srand((unsigned int)time(0));

    guid[0]     = randChar();
    guid[1]     = randChar();
    guid[2]     = randChar();
    guid[3]     = randChar();
    guid[4]     = randChar();
    guid[5]     = randChar();
    guid[6]     = randChar();
    guid[7]     = randChar();
    guid[8]     = '-';
    guid[9]     = randChar();
    guid[10]    = randChar();
    guid[11]    = randChar();
    guid[12]    = randChar();
    guid[13]    = '-';
    guid[14]    = randChar();
    guid[15]    = randChar();
    guid[16]    = randChar();
    guid[17]    = randChar();
    guid[18]    = '-';
    guid[19]    = randChar();
    guid[20]    = randChar();
    guid[21]    = randChar();
    guid[22]    = randChar();
    guid[23]    = '-';
    guid[24]    = randChar();
    guid[25]    = randChar();
    guid[26]    = randChar();
    guid[27]    = randChar();
    guid[28]    = randChar();
    guid[29]    = randChar();
    guid[30]    = randChar();
    guid[31]    = randChar();
    guid[32]    = randChar();
    guid[33]    = randChar();
    guid[34]    = randChar();
    guid[35]    = randChar();
    guid[36]    = 0;

}

bool CreateJNXJpeg(JPEG_tile_list& tiles, long minx, long maxx, long miny, long maxy, CString mapname, long tileWidth, long tileHeight, int nDatabase, int nDrawOrder)
{

	CProgressWindow wndProgress;
	wndProgress.Initialize();

double dMetersPerPixel;
int nTileWidth;

switch (MyMap.GetDetail()) {
case MAP_SCALE_10K:
	dMetersPerPixel = 1.0;
	nTileWidth = 1000;
	break;
case MAP_SCALE_25K:
	dMetersPerPixel = 2.5;
	nTileWidth = 400;
	break;
case MAP_SCALE_50K:
	dMetersPerPixel = 5.0;
	nTileWidth = 200;
	nTileWidth = 400;
	break;
default:
	dMetersPerPixel = 2.5;
	break;
}

// JKL's code to create the JPEGs

//	long width, height;
//	long x,y;

	unsigned char * bitmap_memory = NULL;

	long bitmap_width, dest_bitmap_width;
	long bitmap_height, dest_bitmap_height;

	long bitmap_memory_size;
//	long square_width;
//	long square_height;
	unsigned char bitmap_palette[1024];
	long bitmap_palette_length;

	if (minx > maxx) {
		long t = minx;
		minx = maxx;
		maxx = t;
	}
	if (miny > maxy) {
		long t = miny;
		miny = maxy;
		maxy = t;
	}

//	long collar = CalculateCollar(minx*1000, maxx*1000, miny*1000, maxy*1000);
	long collar = CalculateCollar(minx, maxx, miny, maxy);
	minx -= (collar/1000)*1000;
	maxx += (collar/1000)*1000;
	miny -= (collar/1000)*1000;
	maxy += (collar/1000)*1000;

	wndProgress.ResetProgressBar("Tile:", ((maxy-miny)*(maxx-minx))/1000*1000);

	bool successful = false;

	if (nDatabase == DBASE_LOCAL) {

		if  (MyMap.GetCountry() == COUNTRY_FR) {

			successful = IGN_ReadTiles(minx, maxx, miny, maxy, wndProgress, 
										bitmap_palette, bitmap_palette_length, 
										bitmap_memory , bitmap_memory_size, 
										bitmap_width, dest_bitmap_width,
										bitmap_height, dest_bitmap_height);
		} else {
			successful = OSPro_ReadTiles(minx, maxx, miny, maxy, wndProgress, 
										bitmap_palette, bitmap_palette_length, 
										bitmap_memory , bitmap_memory_size, 
										bitmap_width, dest_bitmap_width,
										bitmap_height, dest_bitmap_height);
		}

	} else {

		bool b_use_TL3 = nDatabase == DBASE_TRACKLOGS_3;

		successful = ReadTracklogsTile(minx, maxx, miny, maxy, wndProgress, 
										bitmap_palette, bitmap_palette_length, 
										bitmap_memory , bitmap_memory_size, 
										bitmap_width, dest_bitmap_width,
										bitmap_height, dest_bitmap_height,
										b_use_TL3);

	}

//	dest_bitmap_width = 400*(maxx-minx-2*collar/1000);
//	dest_bitmap_height = 400*(maxy-miny-2*collar/1000);

	dest_bitmap_width = nTileWidth*((maxx-minx-(2*collar))/1000);
	dest_bitmap_height = nTileWidth*((maxy-miny-(2*collar))/1000);

	if (!successful) {
		if (bitmap_memory)
			free(bitmap_memory);
		return false;
	}

	if (bitmap_memory == NULL) {
		printf("No images to process\n");
		return false;
	}

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	CLSID	encoderClsid;
	// Get the CLSID of the JPEG encoder.
	GetEncoderClsid(L"image/jpeg", &encoderClsid);

	Gdiplus::EncoderParameters encoderParameters;
	encoderParameters.Count = 1;
	ULONG quality = 80;
	encoderParameters.Parameter[0].Guid = Gdiplus::EncoderQuality;
	encoderParameters.Parameter[0].Type = Gdiplus::EncoderParameterValueTypeLong;
	encoderParameters.Parameter[0].NumberOfValues = 1;
	encoderParameters.Parameter[0].Value = &quality;

	// xy
	// 00 10 20 30 40
	// 01 11 21 31 41
	// 02 12 22 32 42
	// 03 13 23 33 43
/*
	minx *= 1000;
	maxx *= 1000;
	miny *= 1000;
	maxy *= 1000;
*/
	double lat,lon;
	OZIGEN_NorthingEasting_to_LatLon(maxy-collar, minx+collar, &lat, &lon);
	double minlat = lat;
	double maxlat = lat;
	double minlon = lon;
	double maxlon = lon;
	OZIGEN_NorthingEasting_to_LatLon(maxy-collar, maxx-collar, &lat, &lon);
	if (lat < minlat) minlat = lat;
	if (lat > maxlat) maxlat = lat;
	if (lon < minlon) minlon = lon;
	if (lon > maxlon) maxlon = lon;
	OZIGEN_NorthingEasting_to_LatLon(miny+collar, minx+collar, &lat, &lon);
	if (lat < minlat) minlat = lat;
	if (lat > maxlat) maxlat = lat;
	if (lon < minlon) minlon = lon;
	if (lon > maxlon) maxlon = lon;
	OZIGEN_NorthingEasting_to_LatLon(miny+collar, maxx-collar, &lat, &lon);
	if (lat < minlat) minlat = lat;
	if (lat > maxlat) maxlat = lat;
	if (lon < minlon) minlon = lon;
	if (lon > maxlon) maxlon = lon;

	OZIGEN_LatLon_to_NorthingEasting(minlat, minlon, &lat, &lon);
	OZIGEN_LatLon_to_NorthingEasting(minlat, maxlon, &lat, &lon);
	OZIGEN_LatLon_to_NorthingEasting(maxlat, maxlon, &lat, &lon);
	OZIGEN_LatLon_to_NorthingEasting(maxlat, minlon, &lat, &lon);


	char jnxName[512];
	GetCurrentDirectory(sizeof(jnxName), jnxName);
	CString Currdir = jnxName;						// Save current directory

	// change directory to %TEMP% and make folder 'files'
	GetTempPath(sizeof(jnxName), jnxName);
	SetCurrentDirectory(jnxName);
	CreateDirectory("files", NULL);

//	sprintf(jnxName, "%s\\%s.kmz", Currdir, mapname);

	long nTilesAcross=(dest_bitmap_width+(tileWidth-1))/tileWidth;
	long nTilesDown=(dest_bitmap_height+(tileHeight-1))/tileHeight;

//	KMZ_tiles tiles; - gone global
	
	long across, down;
	for (across=0; across<nTilesAcross; across++) {
		for (down=0; down<nTilesDown; down++) {

			JPEG_tile * pTile = new JPEG_tile;
			pTile->fname.Format("files/c%02d%02d.jpg", down, across);
			pTile->lat_north = minlat+(maxlat-minlat)*(nTilesDown-down)/nTilesDown;
			pTile->lon_east  = minlon+(maxlon-minlon)*(across+1)/nTilesAcross;
			pTile->lat_south = minlat+(maxlat-minlat)*(nTilesDown-down-1)/nTilesDown;
			pTile->lon_west = minlon+(maxlon-minlon)*across/nTilesAcross;
			pTile->offset_x0 = dest_bitmap_width*across/nTilesAcross;
			pTile->offset_x1 = dest_bitmap_width*(across+1)/nTilesAcross;
			pTile->offset_y0 = dest_bitmap_height*down/nTilesDown;
			pTile->offset_y1 = dest_bitmap_height*(down+1)/nTilesDown;
			tiles.AddTail(pTile);
		}
	}

	Global_AddToResultsWindow("Number of tiles = %d",tiles.GetCount());


// SNL
	wndProgress.ResetProgressBar("JPEG:",tiles.GetCount());

	long tileCount=0;
	long index = 1;
	POSITION pos;
	for (pos=tiles.GetHeadPosition(); pos != NULL; tiles.GetNext(pos)) {
		
		wndProgress.ProgressBar();

		if (wndProgress.m_Cancelled) return false;

		JPEG_tile * pTile = (JPEG_tile *)tiles.GetAt(pos);

		Gdiplus::Rect r(0, 0, pTile->offset_x1-pTile->offset_x0, pTile->offset_y1-pTile->offset_y0);
		Gdiplus::Bitmap bmp(r.GetRight(), r.GetBottom(), PixelFormat24bppRGB );
		Gdiplus::BitmapData bmpData;
		bmp.LockBits(&r, Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeWrite, PixelFormat24bppRGB, &bmpData);

		long x,y;
		for (y=pTile->offset_y0; y<pTile->offset_y1; y++) {
			lat = maxlat-(maxlat-minlat)/dest_bitmap_height*y;
			unsigned char * dest_bitmap_offset = ((unsigned char *)bmpData.Scan0) + bmpData.Stride*(y-pTile->offset_y0);
			for (x=pTile->offset_x0; x<pTile->offset_x1; x++) {
				lon = minlon+(maxlon-minlon)/dest_bitmap_width*x;
				double northing, easting;
				OZIGEN_LatLon_to_NorthingEasting(lat, lon, &northing, &easting);
				northing += 0.5;
				easting += 0.5;
				if (northing < miny || easting < minx || northing > maxy || easting > maxx) {
					// No bitmap data present -- white
					*dest_bitmap_offset++ = 255;
					*dest_bitmap_offset++ = 255;
					*dest_bitmap_offset++ = 255;
				} else {
					// Look up colour of bitmap pixel
					unsigned char * pal = bitmap_palette + 
												4*bitmap_memory[(long)((maxy-northing)/dMetersPerPixel)*bitmap_width +
																(long)((easting-minx)/dMetersPerPixel)];
					*dest_bitmap_offset++ = pal[0];
					*dest_bitmap_offset++ = pal[1];
					*dest_bitmap_offset++ = pal[2];
				}
			}
		}
		bmp.UnlockBits(&bmpData);
		wchar_t wbuffer[64];
		int i;
		for (i=0; i<pTile->fname.GetLength(); i++)
			wbuffer[i] = pTile->fname.GetAt(i);
		wbuffer[i] = 0;
		bmp.Save(wbuffer, &encoderClsid, &encoderParameters);
		tileCount++;
	}


	SetCurrentDirectory(Currdir);
//	tiles.RemoveAll();
/*
	if (nPoints) {
		free(points);
		nPoints = 0;
		points = NULL;
	}
*/

	Gdiplus::GdiplusShutdown(gdiplusToken);

	free(bitmap_memory);


// Next bit from Map2JNX

	// up to five levels. nLevels gives the actual count
	static level_t levels[5];

	// information about all files
	static std::list<file_t> files;

	// the JNX file header to be copied to the outfile
	static jnx_hdr_t jnx_hdr;

	// the tile information table for all 5 levels
	static jnx_tile_t tileTable[JNX_MAX_TILES * 5];

	const uint8_t dummy = 0;
	uint32_t tileTableStart = 0;
	uint32_t tileCnt    = 0;

	const char *jnx_copyright = "Unknown";
	const char *jnx_subscname = "BirdsEye";
	const char *jnx_mapname   = "Unknown";

	char *copyright_buf = NULL;
	char *subscname_buf = NULL;
	char *mapname_buf   = NULL;

	double right    = -180.0;
	double top      =  -90.0;
	double left     =  180.0;
	double bottom   =   90.0;

	double scale = 0.0;

	// Number of used levels
	static int32_t nLevels = 1;

    FILE * fid = fopen(mapname+".jnx","wb");
	if(fid == 0)
    {
		// Failed to create file.
        return false;
    }

// New code writer start
//	POSITION pos;
	for (pos=tiles.GetHeadPosition(); pos != NULL; tiles.GetNext(pos)) {
		JPEG_tile * pTile = (JPEG_tile *)tiles.GetAt(pos);
		if (left > pTile->lon_west)
			left = pTile->lon_west;
		if (right < pTile->lon_east)
			right = pTile->lon_east;
		if (top < pTile->lat_north)
			top = pTile->lat_north;
		if (bottom > pTile->lat_south)
			bottom = pTile->lat_south;
	}
	nLevels = 1;
// New code writer end

	jnx_hdr.zorder  = nDrawOrder;
	jnx_hdr.left    = (int32_t)((left   * 0x7FFFFFFF) / 180);
	jnx_hdr.top     = (int32_t)((top    * 0x7FFFFFFF) / 180);
	jnx_hdr.right   = (int32_t)((right  * 0x7FFFFFFF) / 180);
	jnx_hdr.bottom  = (int32_t)((bottom * 0x7FFFFFFF) / 180);

	jnx_hdr.details = nLevels;

	for(int i=0; i<HEADER_BLOCK_SIZE; i++)
	{
		fwrite(&dummy, sizeof(dummy), 1, fid);
	}
	_fseeki64(fid,0,SEEK_SET);
	fwrite(&jnx_hdr, sizeof(jnx_hdr), 1, fid);

	// get all information to write the table of detail levels and the dummy tile table

	int i=0;

	level_t& level  = levels[i];

	level.nTiles   = tiles.GetCount();
	level.offset   = HEADER_BLOCK_SIZE;		// still has to be offset by complete header
	level.scale    = 1509;

	fwrite(&level.nTiles, sizeof(level.nTiles), 1, fid);
	fwrite(&level.offset, sizeof(level.offset), 1, fid);
	fwrite(&level.scale, sizeof(level.scale), 1, fid);
	fwrite(&level.dummy, sizeof(level.dummy), 1, fid);
	fwrite(jnx_copyright, strlen(jnx_copyright) + 1, 1, fid);

	// printf("\n    Level %i: % 5i tiles, offset %08X, scale: %i, %ix%i", i, level.nTiles, level.offset, level.scale, level.tileSize, level.tileSize);

	// write map loader info block
	uint32_t blockVersion = 0x00000009;
	char GUID[40];
	createGUID(GUID);

	fwrite(&blockVersion, sizeof(blockVersion), 1, fid);
	fwrite(GUID, 37, 1, fid);
	fwrite(jnx_subscname, strlen(jnx_subscname) + 1, 1, fid);
	fwrite(&dummy, sizeof(dummy), 1, fid);
	fwrite(&dummy, sizeof(dummy), 1, fid);
	fwrite(&dummy, sizeof(dummy), 1, fid);
	fwrite(jnx_mapname, strlen(jnx_mapname) + 1, 1, fid);
	fwrite(&nLevels , sizeof(nLevels), 1, fid);

	for(int i = 1; i <= nLevels; i++)
	{
		char str[40];
		sprintf(str,"Level %i", i);
		fwrite(str, strlen(str) + 1, 1, fid);
		fwrite(str, strlen(str) + 1, 1, fid);
		fwrite(jnx_copyright, strlen(jnx_copyright) + 1, 1, fid);
		fwrite(&i,sizeof(i), 1, fid);
	}

    // write dummy tile table
    tileTableStart = HEADER_BLOCK_SIZE;
    _fseeki64(fid, tileTableStart, SEEK_SET);
    fwrite(tileTable, sizeof(jnx_tile_t), tileCount, fid);

// New code writer start
	char TempPath[512];
	GetTempPath(sizeof(TempPath), TempPath);
	CString tPath = TempPath;
	tPath = tPath + "\\";

	wndProgress.ResetProgressBar("JNX:",tiles.GetCount());

	for (pos=tiles.GetHeadPosition(); pos != NULL; tiles.GetNext(pos)) {

		JPEG_tile * pTile = (JPEG_tile *)tiles.GetAt(pos);
//		printf("JPEG %d of %d\r", tileCnt, tiles.GetCount());

       jnx_tile_t& tile = tileTable[tileCnt++];

		tile.left    = (int32_t)(pTile->lon_west * 0x7FFFFFFF / 180);
		tile.top     = (int32_t)(pTile->lat_north * 0x7FFFFFFF / 180);
		tile.right   = (int32_t)(pTile->lon_east * 0x7FFFFFFF / 180);
		tile.bottom  = (int32_t)(pTile->lat_south * 0x7FFFFFFF / 180);

		tile.width  = (uint16_t)(pTile->offset_x1-pTile->offset_x0);
		tile.height = (uint16_t)(pTile->offset_y1-pTile->offset_y0);
		tile.offset = (uint32_t)(_ftelli64(fid) & 0x0FFFFFFFF);
		tile.size   = appendJPG(tPath + pTile->fname, fid);
	}

// New code writer end

    // terminate output file
    fwrite("BirdsEye", 8, 1, fid);

    // write final tile table
    _fseeki64(fid, tileTableStart, SEEK_SET);
    fwrite(tileTable, sizeof(jnx_tile_t), tileCount, fid);
 
	// done
    fclose(fid);

exit_CreateJNXJpeg:

    // Clean up
	if (copyright_buf) free(copyright_buf);
	if (subscname_buf) free(subscname_buf);
	if (mapname_buf) free(mapname_buf);

	//POSITION pos;
	for (pos=tiles.GetHeadPosition(); pos != NULL; tiles.GetNext(pos)) {
		JPEG_tile * pTile = (JPEG_tile *)tiles.GetAt(pos);
		DeleteFile(pTile->fname);
	}

	tiles.RemoveAll();

	return true;
}

bool DecMapJNX(CString strPath, CString strFile, CRect MyRect, long tileWidth, long tileHeight, int nDatabase, int nDrawOrder)
{
	SetCurrentDirectory(strPath);

	CString strMap =  RemoveSuffix(strFile);

	JPEG_tile_list tiles;

	bool bResult = CreateJNXJpeg(tiles,
//								MyRect.left/1000, MyRect.right/1000, MyRect.bottom/1000, MyRect.top/1000,
								MyRect.left, MyRect.right, MyRect.bottom, MyRect.top,
								strFile,
								tileWidth, tileHeight,
								nDatabase,
								nDrawOrder);
/*
	POSITION pos;
	for (pos=tiles.GetHeadPosition(); pos != NULL; tiles.GetNext(pos)) {
		JPEG_tile * pTile = (JPEG_tile *)tiles.GetAt(pos);
		DeleteFile(pTile->fname);
	}

	tiles.RemoveAll();
*/
	return bResult;
}
