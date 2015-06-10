#include "stdafx.h"
#include "..\MapLib\mmmath.h"
#include "..\MapLib\png.h"
#include "..\MapLib\Palette.h"
#include "..\MapLib\map_conv.h"
#include "..\MapLib\License.h"
#include "..\MapLib\db_TL3.h"
#include "..\MapLib\zip.h"

#include "TracklogsDatabase.h"
#include <math.h>

#include "OziGen.h"
#include "OziGenDlg.h"
#include "ProgressWindow.h"
#include "func.h"

#include "jpeg.h"

#include "Database.h"

#include <gdiplus.h>

//#include "Resource.h"

#include "jnx.h"

#define printf visualprintf

//int ZipFileAdd(const char * zipname, const char * filenameinzip);

extern long g_use_TL3;

extern unsigned char uncompressed;
extern long	uncompressed_length;

void read_txf(const char * fname, long * nPoints, void ** PointsMemory);
long PointNearTrack(double lat, double lon, long nPoints, void * Track);
long TileNearTrack(double minlat, double maxlat, double minlon, double maxlon, long nPoints, void * Track, int trackSize);
long nPoints;
void * points;

bool CreateKMZ(long minx, long maxx, long miny, long maxy, LPCSTR mapname, long tileWidth, long tileHeight, long trackSize, DatabaseType nDatabase, int nDrawOrder)
{
//long width, height;
//long x,y;
unsigned char * bitmap_memory = NULL;

long bitmap_width, dest_bitmap_width;
long bitmap_height, dest_bitmap_height;

long bitmap_memory_size;
//long square_width;
//long square_height;
unsigned char bitmap_palette[1024];
long bitmap_palette_length;

	CProgressWindow wndProgress;
	wndProgress.Initialize();
	DoEvents();

	CString str_fname;

	str_fname.Format("%s.txf", mapname);
	read_txf(str_fname, &nPoints, &points);

	str_fname.Format("%s.png", mapname);

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

	long collar = CalculateCollar(minx*1000, maxx*1000, miny*1000, maxy*1000);
	minx -= collar/1000;
	maxx += collar/1000;
	miny -= collar/1000;
	maxy += collar/1000;

	wndProgress.ResetProgressBar("Tile:", (maxy-miny)*(maxx-minx));
	DoEvents();

	bool successful = false;

	if (nDatabase == DB_OSGB_Local) {

		successful = ReadOSProTile(minx, maxx, miny, maxy, wndProgress, 
									bitmap_palette, bitmap_palette_length, 
									bitmap_memory , bitmap_memory_size, 
									bitmap_width, dest_bitmap_width,
									bitmap_height, dest_bitmap_height);

	} else if  (nDatabase == DB_OSGB_Tracklogs2 || nDatabase == DB_OSGB_Tracklogs3) {

		bool b_use_TL3 = nDatabase == DB_OSGB_Tracklogs3;

		successful = ReadTracklogsTile(minx, maxx, miny, maxy, wndProgress, 
										bitmap_palette, bitmap_palette_length, 
										bitmap_memory , bitmap_memory_size, 
										bitmap_width, dest_bitmap_width,
										bitmap_height, dest_bitmap_height,
										b_use_TL3);
	
	} else if  (nDatabase == DB_IGN_Local) {

		successful = ReadIGNTile(minx, maxx, miny, maxy, wndProgress, 
									bitmap_palette, bitmap_palette_length, 
									bitmap_memory , bitmap_memory_size, 
									bitmap_width, dest_bitmap_width,
									bitmap_height, dest_bitmap_height);
	}

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

	minx *= 1000;
	maxx *= 1000;
	miny *= 1000;
	maxy *= 1000;
	double lat,lon;
	OZIGEN_NorthingEasting_to_LatLon(maxy-1000, minx+1000, &lat, &lon);
	double minlat = lat;
	double maxlat = lat;
	double minlon = lon;
	double maxlon = lon;
	OZIGEN_NorthingEasting_to_LatLon(maxy-1000, maxx-1000, &lat, &lon);
	if (lat < minlat) minlat = lat;
	if (lat > maxlat) maxlat = lat;
	if (lon < minlon) minlon = lon;
	if (lon > maxlon) maxlon = lon;
	OZIGEN_NorthingEasting_to_LatLon(miny+1000, minx+1000, &lat, &lon);
	if (lat < minlat) minlat = lat;
	if (lat > maxlat) maxlat = lat;
	if (lon < minlon) minlon = lon;
	if (lon > maxlon) maxlon = lon;
	OZIGEN_NorthingEasting_to_LatLon(miny+1000, maxx-1000, &lat, &lon);
	if (lat < minlat) minlat = lat;
	if (lat > maxlat) maxlat = lat;
	if (lon < minlon) minlon = lon;
	if (lon > maxlon) maxlon = lon;

//	printf("%d %d %d %d\n\n", minx+1000, miny+1000, maxx-1000, maxy-1000);
	OZIGEN_LatLon_to_NorthingEasting(minlat, minlon, &lat, &lon);
//	printf("%f %f %f %f\n", minlat, minlon, lat, lon);
	OZIGEN_LatLon_to_NorthingEasting(minlat, maxlon, &lat, &lon);
//	printf("%f %f %f %f\n", minlat, maxlon, lat, lon);
	OZIGEN_LatLon_to_NorthingEasting(maxlat, maxlon, &lat, &lon);
//	printf("%f %f %f %f\n", maxlat, maxlon, lat, lon);
	OZIGEN_LatLon_to_NorthingEasting(maxlat, minlon, &lat, &lon);
//	printf("%f %f %f %f\n\n", maxlat, minlon, lat, lon);

	char kmzName[512];
	GetCurrentDirectory(sizeof(kmzName), kmzName);
	CString Currdir = kmzName;						// Save current directory

	// change directory to %TEMP% and make folder 'files'
	GetTempPath(sizeof(kmzName), kmzName);
	SetCurrentDirectory(kmzName);
	CreateDirectory("files", NULL);

	sprintf(kmzName, "%s\\%s.kmz", Currdir, mapname);

	FILE * fp = fopen("doc.kml", "w+");
	if (fp == NULL) {
		printf("Couldn't open doc.kml\n");
		return false;
	}



	fprintf(fp,	"<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\n"
				"<kml xmlns=\"http://earth.google.com/kml/2.1\">\n"
				"<Document>\n"
				"<name>Custom Map</name>\n"
				"<Region>\n"
				"<LatLonAltBox>\n"
				"<north>%.9f</north>\n"
				"<south>%.9f</south>\n"
				"<east>%.9f</east>\n"
				"<west>%.9f</west>\n"
				"</LatLonAltBox>\n"
				"</Region>\n",
				maxlat,
				minlat,
				maxlon,
				minlon);


	long nTilesAcross=(dest_bitmap_width+(tileWidth-1))/tileWidth;
	long nTilesDown=(dest_bitmap_height+(tileHeight-1))/tileHeight;

	JPEG_tile_list tiles;
	
	long across, down;
	for (across=0; across<nTilesAcross; across++) {
		for (down=0; down<nTilesDown; down++) {
			if (nPoints) {
				if (!TileNearTrack( minlat+(maxlat-minlat)*(nTilesDown-down)/nTilesDown,
									minlat+(maxlat-minlat)*(nTilesDown-down-1)/nTilesDown,
									minlon+(maxlon-minlon)*across/nTilesAcross,
									minlon+(maxlon-minlon)*(across+1)/nTilesAcross, nPoints, points, trackSize) )
					continue;
/*				if (!PointNearTrack(minlat+(maxlat-minlat)*(nTilesDown-down)/nTilesDown,					// ne
									minlon+(maxlon-minlon)*(across+1)/nTilesAcross, nPoints, points) &&
					!PointNearTrack(minlat+(maxlat-minlat)*(nTilesDown-down-1)/nTilesDown,					// se
									minlon+(maxlon-minlon)*(across+1)/nTilesAcross, nPoints, points) &&
					!PointNearTrack(minlat+(maxlat-minlat)*(nTilesDown-down)/nTilesDown,					// nw
									minlon+(maxlon-minlon)*across/nTilesAcross, nPoints, points) &&
					!PointNearTrack(minlat+(maxlat-minlat)*(nTilesDown-down-1)/nTilesDown,					// sw
									minlon+(maxlon-minlon)*across/nTilesAcross, nPoints, points) &&
					!PointNearTrack(minlat+(maxlat-minlat)*(2*nTilesDown-2*down-1)/2/nTilesDown,			// middle
									minlon+(maxlon-minlon)*(2*across+1)/2/nTilesAcross, nPoints, points))
						continue;
*/
			}
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

	if (tiles.GetCount() > 100) {

		if (AfxMessageBox("Warning Tile Count > 100!", MB_OKCANCEL) == IDCANCEL)
			return false;
	}

	wndProgress.m_progress_bar.SetRange(1, tiles.GetCount());

	long index = 1;
	POSITION pos;
	for (pos=tiles.GetHeadPosition(); pos != NULL; tiles.GetNext(pos)) {

		JPEG_tile * pTile = (JPEG_tile *)tiles.GetAt(pos);
//		printf("JPEG %d of %d\r", index++, tiles.GetCount());

		wndProgress.m_progress_bar.SetPos(index);
		wndProgress.m_edit_message.Format("JPEG: %d of %d", index++, tiles.GetCount());
		wndProgress.UpdateData(FALSE);

		DoEvents();

		if (wndProgress.m_Cancelled) return false;

		fprintf(fp, "<GroundOverlay>\n"
					"<drawOrder>%d</drawOrder>\n"
					"<Icon>\n"
					"<href>%s</href>\n"
					"</Icon>\n"
					"<LatLonBox>\n"
					"<north>%.9f</north>\n"
					"<east>%.9f</east>\n"
					"<south>%.9f</south>\n"
					"<west>%.9f</west>\n"
					"</LatLonBox>\n"
					"<Region>\n"
					"<LatLonAltBox>\n"
					"<north>%.9f</north>\n"
					"<east>%.9f</east>\n"
					"<south>%.9f</south>\n"
					"<west>%.9f</west>\n"
					"</LatLonAltBox>\n"
					"<Lod>\n"
					"<minLodPixels>64</minLodPixels>\n"
					"</Lod>\n"
					"</Region>\n"
					"<color>A1FFFFFF</color>\n"
					"</GroundOverlay>\n",
					nDrawOrder,
					pTile->fname,
					pTile->lat_north,
					pTile->lon_east,
					pTile->lat_south,
					pTile->lon_west,
					pTile->lat_north,
					pTile->lon_east,
					pTile->lat_south,
					pTile->lon_west);

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
												4*bitmap_memory[(long)((maxy-northing)/2.5)*bitmap_width +
																(long)((easting-minx)/2.5)];
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
	}

	fprintf(fp, "</Document>\n"
				"</kml>\n");
	fclose(fp);

	// Add jpegs / kml to kmz file
	DeleteFile(kmzName);
	ZipFileAdd(kmzName, "doc.kml");
	DeleteFile("doc.kml");


	long indexZip = 1;
	wndProgress.m_progress_bar.SetRange(1, tiles.GetCount());

	for (pos=tiles.GetHeadPosition(); pos != NULL; tiles.GetNext(pos)) {
		JPEG_tile * pTile = (JPEG_tile *)tiles.GetAt(pos);

		wndProgress.m_progress_bar.SetPos(indexZip);
		wndProgress.m_edit_message.Format("Zip: %d of %d", indexZip++, tiles.GetCount());
		wndProgress.UpdateData(FALSE);
		DoEvents();

		ZipFileAdd(kmzName, pTile->fname);

		DeleteFile(pTile->fname);
	}

	RemoveDirectory("files");
	SetCurrentDirectory(Currdir);
	tiles.RemoveAll();
	if (nPoints) {
		free(points);
		nPoints = 0;
		points = NULL;
	}

	Gdiplus::GdiplusShutdown(gdiplusToken);

	free(bitmap_memory);

	return true;
}


bool DecMapKMZ(CString strPath, CString strFile, CRect MyRect, long tileWidth, long tileHeight, DatabaseType nDatabase, int trackSize, int nDrawOrder)
{
	SetCurrentDirectory(strPath);

	CString strMap =  RemoveSuffix(strFile);

	g_use_TL3 = nDatabase == DB_OSGB_Tracklogs3;

	long fromTrack=100;
	if (trackSize == 0) {
		fromTrack = 125;
	} else if (trackSize == 1) {
		fromTrack = 250;
	} else if (trackSize == 2) {
		fromTrack = 500;
	} else if (trackSize == 3) {
		fromTrack = 1000;
	} else if (trackSize == 4) {
		fromTrack = 1500;
	} else if (trackSize == 5) {
		fromTrack = 2000;
	} else {
		fromTrack = 1000;
	}

	return CreateKMZ(MyRect.left/1000, MyRect.right/1000, MyRect.bottom/1000, MyRect.top/1000,
					 strFile,
					 tileWidth, tileHeight, fromTrack,
					 nDatabase,
					 nDrawOrder);
}
