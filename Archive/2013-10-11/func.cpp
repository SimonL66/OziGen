#include "stdafx.h"

#include <math.h>

#include "..\MapLib\mmmath.h"
#include "..\MapLib\Lambert93.h"
#include "func.h"

//#include "UTM.h"
#include "LatLong-UTM.h"

#include "CustomMap.h"

#include "proj_api.h"

#include "EPSG.h"

extern CCustomMap MyMap;

int abortProgram = FALSE;

BOOL CtrlCHandler(DWORD dwCtrlType)
{
	abortProgram = TRUE;
	return TRUE;
}

CString GetFilename(CString strPathName)
{
	int i = strPathName.ReverseFind('\\');
	if (i == 0)
		i = strPathName.ReverseFind(':');
	if (i)
		strPathName = strPathName.Mid(i+1);

	return strPathName;
}

CString GetPath(CString strPathName)
{
	int i = strPathName.ReverseFind('\\');
	if (i == 0)
		i = strPathName.ReverseFind(':');
	if (i)
		strPathName = strPathName.Left(i);

	return strPathName;
}

CString RemoveSuffix(CString strFileName)
{
	int nSuffix = strFileName.ReverseFind('.');
	
	if (nSuffix == -1) {
		return strFileName;
	} else {
		return strFileName.Left(nSuffix);
	}
}

void BackupFile(CString strFileName)
{
	CStdioFile fileInput;
	if (!fileInput.Open(strFileName, CFile::modeRead)) {
		return;
	}
	fileInput.Close();

	CString strBackup;
	strBackup = strFileName + ".backup";
	rename(strFileName,strBackup);
}

double ConvertMetresToFeet(double metres)
{
	return metres * 100.0 / 2.54 / 12.0;
}

/*
	1 mile = 5280 feet
	1 foot = 12 inches
	1 inch = .0254 m

	metres => miles
	meters / 5280 / 12 / .0254
	metres / 1609.344
*/
double ConvertMetresToMiles(double metres)
{
	return (metres / 1609.344);
}

CString MakeTitle(CString strTitle)
{
	bool bMakeLower = true;

	int nLength = strTitle.GetLength();

	for (int i=0; i < nLength; i++) {
		char ch = strTitle[i];
		if (bMakeLower) {
			strTitle.SetAt(i,tolower(ch));
			bMakeLower = false;
		}

		if (ch == ' ') {
			strTitle.SetAt(i, '_');
			bMakeLower = true;
		}
	}
	return strTitle;
}

CString RemoveSpaces(CString strTitle)
{
	int nLength = strTitle.GetLength();

	for (int i=0; i < nLength; i++) {
		
		char ch = strTitle[i];

		if (ch == ' ') {
			strTitle.SetAt(i, '_');
		}
	}
	return strTitle;
}

bool IsFilePresent(CString strFile)
{
	CStdioFile fileInput;
	if (!fileInput.Open(strFile, CFile::modeRead)) {
		return false;
	}
	fileInput.Close();
	return true;
}

/*
CString GetPoint (double dLat, double dLon)
{
	double easting, northing;
	OZIGEN_LatLon_to_NorthingEasting(dLat, dLon, &northing, &easting);
	
	char OSreference[64];
	easting_northing_to_OSref3((long)easting, (long)northing, OSreference);

	return OSreference;
}
*/

void DoFileCopy(CString inputFilename, CString outputFilename)
{
	long filelength;
	char * filedata;
	FILE * fpInput = fopen(inputFilename, "rb");

	if (fpInput == NULL)
		return;
	fseek(fpInput, 0, SEEK_END);
	filelength = ftell(fpInput);
	filedata = (char *) malloc(filelength);
	if (filedata == NULL) {
//		printf("Couldn't allocate %d bytes to read %s\n", filelength, inputFilename);
		fclose(fpInput);
		return;
	}
	fseek(fpInput, 0, SEEK_SET);
	fread(filedata, 1, filelength, fpInput);
	fclose(fpInput);

	FILE * fpOutput = fopen(outputFilename, "wb+");
	if (fpOutput != NULL) {
		fwrite(filedata, 1, filelength, fpOutput);
		fclose(fpOutput);
	}
	fclose(fpOutput);
	free(filedata);
}

/*
CString getString (unsigned char* data, long offset)
{
	CString str;
	BYTE sizeString= *(data+offset+1);
	char* strBuffer = (char*)malloc(sizeString+1);
	memset(strBuffer,0x00,sizeString+1);
	memcpy(strBuffer,data+offset+2,sizeString);
	str.Format("%s",strBuffer);
	free(strBuffer);
	return str;
}
*/

/*
long SearchBuffer (unsigned char* data, long data_length, long offset, char* string)
{
	int len = strlen(string);

	long i;
	for (i=offset; i < data_length; i++) {
		if (memcmp(data+i, string, len) == 0) {
			return i;
		}
	}
	return -1;
}
*/

void DoEvents(void)
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void ValidateMapExtents(long& minx, long& miny, long& maxx, long& maxy)
{
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
}


long CalculateCollar(long minx, long maxx, long miny, long maxy)
{
//	minx *= 1000;
//	maxx *= 1000;
//	miny *= 1000;
//	maxy *= 1000;
	double lat,lon;
	OZIGEN_NorthingEasting_to_LatLon(maxy, minx, &lat, &lon);
	double minlat = lat;
	double maxlat = lat;
	double minlon = lon;
	double maxlon = lon;
	OZIGEN_NorthingEasting_to_LatLon(maxy, maxx, &lat, &lon);
	if (lat < minlat) minlat = lat;
	if (lat > maxlat) maxlat = lat;
	if (lon < minlon) minlon = lon;
	if (lon > maxlon) maxlon = lon;
	OZIGEN_NorthingEasting_to_LatLon(miny, minx, &lat, &lon);
	if (lat < minlat) minlat = lat;
	if (lat > maxlat) maxlat = lat;
	if (lon < minlon) minlon = lon;
	if (lon > maxlon) maxlon = lon;
	OZIGEN_NorthingEasting_to_LatLon(miny, maxx, &lat, &lon);
	if (lat < minlat) minlat = lat;
	if (lat > maxlat) maxlat = lat;
	if (lon < minlon) minlon = lon;
	if (lon > maxlon) maxlon = lon;

	double tminx, tminy, tmaxx, tmaxy;
	OZIGEN_LatLon_to_NorthingEasting(maxlat, maxlon, &lat, &lon);
	tminy = lat;
	tmaxy = lat;
	tminx = lon;
	tmaxx = lon;
	OZIGEN_LatLon_to_NorthingEasting(minlat, minlon, &lat, &lon);
	if (lat > tmaxy) tmaxy = lat;
	if (lat < tminy) tminy = lat;
	if (lon > tmaxx) tmaxx = lon;
	if (lon < tminx) tminx = lon;
	OZIGEN_LatLon_to_NorthingEasting(minlat, maxlon, &lat, &lon);
	if (lat > tmaxy) tmaxy = lat;
	if (lat < tminy) tminy = lat;
	if (lon > tmaxx) tmaxx = lon;
	if (lon < tminx) tminx = lon;
	OZIGEN_LatLon_to_NorthingEasting(maxlat, minlon, &lat, &lon);
	if (lat > tmaxy) tmaxy = lat;
	if (lat < tminy) tminy = lat;
	if (lon > tmaxx) tmaxx = lon;
	if (lon < tminx) tminx = lon;

	double collar = tmaxx - maxx;
	if (minx-tminx > collar) collar = minx-tminx;
	if (tmaxy-maxy > collar) collar = tmaxy-maxy;
	if (miny-tminy > collar) collar = miny-tminy;
	return (long)((collar + 1000)/1000)*1000;
}



void OZIGEN_EastingNorthing_to_LatLon(long easting, long northing, double * latitude, double * longitude)
{
	OZIGEN_NorthingEasting_to_LatLon(northing, easting, latitude, longitude);
}

void OZIGEN_LatLon_to_EastingNorthing(double latitude, double longitude, long * easting, long * northing)
{
	double east, north;
    OZIGEN_LatLon_to_NorthingEasting(latitude, longitude, &north, &east);
	*easting = (long)(east + 0.5);
	*northing = (long)(north + 0.5);
}

void read_txf(const char * fname, long * nPoints, void ** PointsMemory);
long PointNearTrack(double lat, double lon, long nPoints, void * Track);
long TileNearTrack(double minlat, double maxlat, double minlon, double maxlon, long nPoints, void * Track, int trackSize);

extern long nPoints;
extern void * points;

int GetCustomMapTileCount(long minx, long maxx, long miny, long maxy, LPCSTR mapname, long tileWidth, long tileHeight, long trackSize)
{
	
	CString strMap =  RemoveSuffix(mapname);

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

	read_txf(mapname, &nPoints, &points);

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

	long collar = CalculateCollar(minx, maxx, miny, maxy);
	minx -= collar;
	maxx += collar;
	miny -= collar;
	maxy += collar;


	double dTilesAcross=(labs(maxx-minx))/(tileWidth*2.5);
	double dTilesDown=(labs(maxy-miny))/(tileHeight*2.5);

	long nTilesAcross = (long) ceil(dTilesAcross);
	long nTilesDown = (long) ceil(dTilesDown);


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

	OZIGEN_LatLon_to_NorthingEasting(minlat, minlon, &lat, &lon);
	OZIGEN_LatLon_to_NorthingEasting(minlat, maxlon, &lat, &lon);
	OZIGEN_LatLon_to_NorthingEasting(maxlat, maxlon, &lat, &lon);
	OZIGEN_LatLon_to_NorthingEasting(maxlat, minlon, &lat, &lon);

	long across, down;
	int nTileCount=0;
	for (across=0; across<nTilesAcross; across++) {
		for (down=0; down<nTilesDown; down++) {
			if (nPoints) {
				if (!TileNearTrack( minlat+(maxlat-minlat)*(nTilesDown-down)/nTilesDown,
									minlat+(maxlat-minlat)*(nTilesDown-down-1)/nTilesDown,
									minlon+(maxlon-minlon)*across/nTilesAcross,
									minlon+(maxlon-minlon)*(across+1)/nTilesAcross, nPoints, points, trackSize) )
					continue;
			}
			nTileCount++;
		}
	}
	return nTilesAcross*nTilesDown-nTileCount;
}

//	x = lon × 6371000.0 × pi ÷ 180
//	y = ln(tan((pi ÷ 4) + (lat × pi ÷ 360))) × 6371000.0
 
/*
void WGS84toUTM(double lat, double lon, double& y, double& x, int& UTMZone)
{
	LLtoUTM(22, lat, lon, y, x, UTMZone);
}

void UTMtoWGS84(double northing, double easting, double* latitude, double* longitude, int UTMZone)
{
	UTMtoLL(22, northing, easting, UTMZone, latitude,  longitude);
}
*/

// Conversion Functions

void _WGS84toLocalGrid(CString projWGS84, CString projLocalGrid, double lat, double lon, double* y, double* x)
{
	projPJ projFrom	= pj_init_plus(projWGS84);
	projPJ projTo	= pj_init_plus(projLocalGrid);

	double xx = lon * DEG_TO_RAD; 
	double yy = lat  * DEG_TO_RAD; 

	pj_transform(projFrom, projTo, 1, 0, &xx, &yy, 0); 

	*x = xx;
	*y = yy;
}

void _LocalGridtoWGS84(CString projLocalGrid, CString projWGS84, double northing, double easting, double* latitude, double* longitude)
{
	projPJ projFrom	= pj_init_plus(projLocalGrid);
	projPJ projTo	= pj_init_plus(projWGS84);

	double xx = easting; 
	double yy = northing; 

	pj_transform(projFrom, projTo, 1, 0, &xx, &yy, 0);

	*latitude = yy * RAD_TO_DEG;	
	*longitude = xx * RAD_TO_DEG;
}

struct ConversionTableEPSG {

	int		nEPSG;
	CString	strEPSG;

} ConversionTableEPSG[] = {

	{23030,	EPSG_23030},
	{23031,	EPSG_23031},
	{31467,	EPSG_31467},
	{32633,	EPSG_32633},
	{3912,	EPSG_3912},
	{31287,	EPSG_31287},
	{3763,	EPSG_3763},
	{3857,	EPSG_3857},
	{3812,	EPSG_3812},
	{27700,	EPSG_27700},
	{2180,	EPSG_2180},
	{31468,	EPSG_31468},
	{3067,	EPSG_3067},
	{3067,	EPSG_3067},
	{25832,	EPSG_25832},
	
	{-1, ""}
};

CString GetConversionEPSG(int nEPSG)
{
	for (int i=0; true; i++) {
		if (ConversionTableEPSG[i].nEPSG == -1) break;
		if (ConversionTableEPSG[i].nEPSG == nEPSG) 
			return ConversionTableEPSG[i].strEPSG;
	}
	return EPSG_WGS84;
}

void WGS84_EastingNorthing(int nEPSG, double dLat, double dLon, double* x, double* y)
{
	CString strEPSG = GetConversionEPSG(nEPSG);

	_WGS84toLocalGrid(EPSG_WGS84, strEPSG, dLat, dLon, y, x);
}

void EastingNorthing_WGS84(int nEPSG, double x, double y, double* dLat, double* dLon)
{
	CString strEPSG = GetConversionEPSG(nEPSG);

	_LocalGridtoWGS84(strEPSG, EPSG_WGS84, y, x, dLat, dLon);
}

const double M_PI = 4*atan(1.0);	// Gantz used: PI=3.14159265;

int long2tilex(double lon, int z) 
{ 
	return (int)(floor((lon + 180.0) / 360.0 * pow(2.0, z))); 
}
 
int lat2tiley(double lat, int z)
{ 
	return (int)(floor((1.0 - log( tan(lat * M_PI/180.0) + 1.0 / cos(lat * M_PI/180.0)) / M_PI) / 2.0 * pow(2.0, z))); 
}
 
double tilex2long(double x, double z) 
{
	return x / pow(2.0, z) * 360.0 - 180;
}
 
double tiley2lat(double y, double z) 
{
	double n = M_PI - 2.0 * M_PI * y / pow(2.0, z);
	return 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
}

void LatLon_to_OSM(double dLat, double dLon, double* dNorthing, double* dEasting)
{
	int z = MyMap.GetLevelOfDetail();

	*dNorthing = lat2tiley(dLat, z);
	*dEasting = long2tilex(dLon, z);

/*
	projPJ projFrom	= pj_init_plus(EPSG_WGS84);
	projPJ projTo	= pj_init_plus(EPSG_3857);

	double xx = dLon * DEG_TO_RAD; 
	double yy = dLat  * DEG_TO_RAD; 

	double z = 15.0;

	pj_transform(projFrom, projTo, 1, 0, &xx, &yy, &z); 

	*dEasting = xx;
	*dNorthing = yy;
*/
}


void OSM_to_LatLon(double dNorthing, double dEasting, double* dLat, double* dLon)
{
	int z = MyMap.GetLevelOfDetail();

	*dLat = tiley2lat(dNorthing, z);
	*dLon = tilex2long(dEasting, z);
}



/*

function tile2long(x,z) { return (x/Math.pow(2,z)*360-180); }
function tile2lat(y,z) { var n=Math.PI-2*Math.PI*y/Math.pow(2,z);
return (180/Math.PI*Math.atan(0.5*(Math.exp(n)-Math.exp(-n)))); }
*/





/*

1. LatLongToPixelXY()
 2. PixelXYToTileXY()
 3. TileXYToQuadKey() 

//------------------------------------------------------------------------------
// <copyright company="Microsoft">
//     Copyright (c) 2006-2009 Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

using System;
using System.Text;

//namespace Microsoft.MapPoint
*/

//namespace BingMaps
//{
//    static class TileSystem
//    {
        /*private*/ const double EarthRadius = 6378137.0;
        /*private*/ const double MinLatitude = -85.05112878;
        /*private*/ const double MaxLatitude = 85.05112878;
        /*private*/ const double MinLongitude = -180.0;
        /*private*/ const double MaxLongitude = 180.0;


        /// <summary>
        /// Clips a number to the specified minimum and maximum values.
        /// </summary>
        /// <param name="n">The number to clip.</param>
        /// <param name="minValue">Minimum allowable value.</param>
        /// <param name="maxValue">Maximum allowable value.</param>
        /// <returns>The clipped value.</returns>
        /*private*/ static double Clip(double n, double minValue, double maxValue)
        {
//            return Math.Min(Math.Max(n, minValue), maxValue);
            return min(max(n, minValue), maxValue);
        }
        
        

        /// <summary>
        /// Determines the map width and height (in pixels) at a specified level
        /// of detail.
        /// </summary>
        /// <param name="levelOfDetail">Level of detail, from 1 (lowest detail)
        /// to 23 (highest detail).</param>
        /// <returns>The map width and height in pixels.</returns>
 //       /*private*/ static uint MapSize(int levelOfDetail)
        /*private*/ static UINT MapSize(int levelOfDetail)
        {
//            return (uint) 256 << levelOfDetail;
            return (UINT) 256 << levelOfDetail;
        }



        /// <summary>
        /// Determines the ground resolution (in meters per pixel) at a specified
        /// latitude and level of detail.
        /// </summary>
        /// <param name="latitude">Latitude (in degrees) at which to measure the
        /// ground resolution.</param>
        /// <param name="levelOfDetail">Level of detail, from 1 (lowest detail)
        /// to 23 (highest detail).</param>
        /// <returns>The ground resolution, in meters per pixel.</returns>
        /*private*/ static double GroundResolution(double latitude, int levelOfDetail)
        {
            latitude = Clip(latitude, MinLatitude, MaxLatitude);
//            return Math.Cos(latitude * Math.PI / 180) * 2 * Math.PI * EarthRadius / MapSize(levelOfDetail);
            return cos(latitude * M_PI / 180.0) * 2.0 * M_PI * EarthRadius / MapSize(levelOfDetail);
        }



        /// <summary>
        /// Determines the map scale at a specified latitude, level of detail,
        /// and screen resolution.
        /// </summary>
        /// <param name="latitude">Latitude (in degrees) at which to measure the
        /// map scale.</param>
        /// <param name="levelOfDetail">Level of detail, from 1 (lowest detail)
        /// to 23 (highest detail).</param>
        /// <param name="screenDpi">Resolution of the screen, in dots per inch.</param>
        /// <returns>The map scale, expressed as the denominator N of the ratio 1 : N.</returns>
        /*private*/ static double MapScale(double latitude, int levelOfDetail, int screenDpi)
        {
            return GroundResolution(latitude, levelOfDetail) * screenDpi / 0.0254;
        }



        /// <summary>
        /// Converts a point from latitude/longitude WGS-84 coordinates (in degrees)
        /// into pixel XY coordinates at a specified level of detail.
        /// </summary>
        /// <param name="latitude">Latitude of the point, in degrees.</param>
        /// <param name="longitude">Longitude of the point, in degrees.</param>
        /// <param name="levelOfDetail">Level of detail, from 1 (lowest detail)
        /// to 23 (highest detail).</param>
        /// <param name="pixelX">Output parameter receiving the X coordinate in pixels.</param>
        /// <param name="pixelY">Output parameter receiving the Y coordinate in pixels.</param>
        /*private*/ static void LatLongToPixelXY(double latitude, double longitude, int levelOfDetail, /*out*/ double* pixelX, /*out*/ double* pixelY)
        {
            latitude = Clip(latitude, MinLatitude, MaxLatitude);
            longitude = Clip(longitude, MinLongitude, MaxLongitude);

            double x = (longitude + 180.0) / 360.0; 
            double sinLatitude = sin(latitude * M_PI / 180.0);
            double y = 0.5 - log((1.0 + sinLatitude) / (1.0 - sinLatitude)) / (4.0 * M_PI);

            UINT mapSize = MapSize(levelOfDetail);
            *pixelX = Clip(x * mapSize + 0.5, 0, mapSize - 1.0);
            *pixelY = Clip(y * mapSize + 0.5, 0, mapSize - 1.0);
        }



        /// <summary>
        /// Converts a pixel from pixel XY coordinates at a specified level of detail
        /// into latitude/longitude WGS-84 coordinates (in degrees).
        /// </summary>
        /// <param name="pixelX">X coordinate of the point, in pixels.</param>
        /// <param name="pixelY">Y coordinates of the point, in pixels.</param>
        /// <param name="levelOfDetail">Level of detail, from 1 (lowest detail)
        /// to 23 (highest detail).</param>
        /// <param name="latitude">Output parameter receiving the latitude in degrees.</param>
        /// <param name="longitude">Output parameter receiving the longitude in degrees.</param>
        /*private*/ static void PixelXYToLatLong(double pixelX, double pixelY, int levelOfDetail, double* latitude, double* longitude)
        {
            double mapSize = MapSize(levelOfDetail);
            double x = (Clip(pixelX, 0, mapSize - 1) / mapSize) - 0.5;
            double y = 0.5 - (Clip(pixelY, 0.0, mapSize - 1.0) / mapSize);

            *latitude = 90.0 - 360.0 * atan(exp(-y * 2.0 * M_PI)) / M_PI;
            *longitude = 360.0 * x;
        }



        /// <summary>
        /// Converts pixel XY coordinates into tile XY coordinates of the tile containing
        /// the specified pixel.
        /// </summary>
        /// <param name="pixelX">Pixel X coordinate.</param>
        /// <param name="pixelY">Pixel Y coordinate.</param>
        /// <param name="tileX">Output parameter receiving the tile X coordinate.</param>
        /// <param name="tileY">Output parameter receiving the tile Y coordinate.</param>
        /*private*/ static void PixelXYToTileXY(double pixelX, double pixelY, double* tileX, double* tileY)
        {
            *tileX = pixelX / 256.0;
            *tileY = pixelY / 256.0;
        }



        /// <summary>
        /// Converts tile XY coordinates into pixel XY coordinates of the upper-left pixel
        /// of the specified tile.
        /// </summary>
        /// <param name="tileX">Tile X coordinate.</param>
        /// <param name="tileY">Tile Y coordinate.</param>
        /// <param name="pixelX">Output parameter receiving the pixel X coordinate.</param>
        /// <param name="pixelY">Output parameter receiving the pixel Y coordinate.</param>
        /*private*/ static void TileXYToPixelXY(double tileX, double tileY, double* pixelX, double* pixelY)
        {
            *pixelX = tileX * 256;
            *pixelY = tileY * 256;
        }



        /// <summary>
        /// Converts tile XY coordinates into a QuadKey at a specified level of detail.
        /// </summary>
        /// <param name="tileX">Tile X coordinate.</param>
        /// <param name="tileY">Tile Y coordinate.</param>
        /// <param name="levelOfDetail">Level of detail, from 1 (lowest detail)
        /// to 23 (highest detail).</param>
        /// <returns>A string containing the QuadKey.</returns>
        /*private*/ CString TileXYToQuadKey(int tileX, int tileY, int levelOfDetail)
        {
            CString quadKey;

			quadKey.Empty();

            for (int i = levelOfDetail; i > 0; i--)
            {
                char digit = '0';
                int mask = 1 << (i - 1);
                if ((tileX & mask) != 0)
                {
                    digit++;
                }
                if ((tileY & mask) != 0)
                {
                    digit++;
                    digit++;
                }
				quadKey = quadKey + digit;
            }
            return quadKey;
        }



        /// <summary>
        /// Converts a QuadKey into tile XY coordinates.
        /// </summary>
        /// <param name="quadKey">QuadKey of the tile.</param>
        /// <param name="tileX">Output parameter receiving the tile X coordinate.</param>
        /// <param name="tileY">Output parameter receiving the tile Y coordinate.</param>
        /// <param name="levelOfDetail">Output parameter receiving the level of detail.</param>
        void QuadKeyToTileXY(CString quadKey, int& tileX, int& tileY, int& levelOfDetail)
        {
            tileX = tileY = 0;
            levelOfDetail = quadKey.GetLength();
            for (int i = levelOfDetail; i > 0; i--)
            {
                int mask = 1 << (i - 1);
                switch (quadKey[levelOfDetail - i])
                {
                    case '0':
                        break;

                    case '1':
                        tileX |= mask;
                        break;

                    case '2':
                        tileY |= mask;
                        break;

                    case '3':
                        tileX |= mask;
                        tileY |= mask;
                        break;
                }
            }
        }

		void LatLon_BingMapsXY(double dLat, double dLon, double* tileY, double* tileX)
		{
			double pixelX;
			double pixelY;
//			int tileX;
//			int tileY;
			int levelOfDetail = MyMap.GetLevelOfDetail();

			LatLongToPixelXY(dLat, dLon, levelOfDetail, &pixelX, &pixelY);
			PixelXYToTileXY(pixelX, pixelY, tileX, tileY);
//			return TileXYToQuadKey(tileX, tileY, levelOfDetail);
		}


		void BingMapsXY_LatLon(double tileY, double tileX, double* dLat, double* dLon)
		{
			double pixelX;
			double pixelY;
//			int tileX;
//			int tileY;
			int levelOfDetail = MyMap.GetLevelOfDetail();

TileXYToPixelXY(tileX, tileY, &pixelX, &pixelY);
PixelXYToLatLong(pixelX, pixelY, levelOfDetail, dLat, dLon);
		}

//    }
//}



// Coordinate functions

void OZIGEN_NorthingEasting_to_LatLon(double dNorthing, double dEasting, double* dLat, double* dLon)
{

// Projection Definitions
//	Austria;			EPSG:31287
//	Belgium;			EPSG:3812	
//	Britain;			EPSG:27700
//	Eire;				EPSG:
//	France;				EPSG:2154 - Not Sure if this is correct?
//	Germany;			EPSG:31467
//	Italy;				EPSG:
//	Luxembourg;			EPSG:
//	Netherlands;		EPSG:
//	Northern Ireland;	EPSG:
//	Portugal;			EPSG:
//	Spain;				EPSG:23030
//		Catalonia;		EPSG:23031
//	Switzerland;		EPSG:
//	Poland;				EPSG:
//	Czech Republic;		EPSG:32633
//	Slovenia;			EPSG:3912
//	Finland;			EPSG:3067

// OSM;					EPSG:3857 - Not Sure if this is correct?
// WGS84;				EPSG:4326  - This is actually the WGS84 projection!
// Bing;				EPSG:900913  - Not Sure if this is correct?

	int nProjection = MyMap.GetProjection();

	switch (nProjection) {
	case 3812: // Belgium
		LambertBelgium2008ToWGS84(dNorthing, dEasting, dLat, dLon);
		break;
	case 2154: // France
		Lambert93ToWGS84(dNorthing, dEasting, dLat, dLon);
		break;
	case 3857:	// OSM
		OSM_to_LatLon(dNorthing, dEasting, dLat, dLon);
		break;
	case 27700:	// Britian
		NorthingEasting_to_LatLon(dNorthing, dEasting, dLat, dLon);
		break;
	case 900913:	// Bing
		BingMapsXY_LatLon(dNorthing, dEasting, dLat, dLon);
		break;
//	case COUNTRY_SAT:
//		BingMaps_LatLon(dLat, dLon, dNorthing, dEasting);
//		break;	
	default:
		EastingNorthing_WGS84(nProjection, dEasting, dNorthing, dLat, dLon);
		break;
	}
/*
	switch (MyMap.GetCountry()) {
	case COUNTRY_BE:
		LambertBelgium2008ToWGS84(dNorthing, dEasting, dLat, dLon);
		break;
	case COUNTRY_FR:
		Lambert93ToWGS84(dNorthing, dEasting, dLat, dLon);
		break;
	case COUNTRY_OSM:
		OSM_to_LatLon(dNorthing, dEasting, dLat, dLon);
		break;
	case COUNTRY_UK:
		NorthingEasting_to_LatLon(dNorthing, dEasting, dLat, dLon);
		break;
//	case COUNTRY_SAT:
//		BingMaps_LatLon(dLat, dLon, dNorthing, dEasting);
//		break;	
	default:
		EastingNorthing_WGS84(nProjection, dEasting, dNorthing, dLat, dLon);
		break;
	}
*/

}

int OZIGEN_LatLon_to_NorthingEasting(double dLat, double dLon, double * dNorthing, double * dEasting)
{
	int nProjection = MyMap.GetProjection();

	switch (nProjection) {
	case 3812: // Belgium
		WGS84ToLambertBelgium2008(dLat, dLon, dNorthing, dEasting);
		break;
	case 2154: // France
		WGS84ToLambert93(dLat, dLon, dNorthing, dEasting);
		break;
	case 3857:	// OSM
		LatLon_to_OSM(dLat, dLon, dNorthing, dEasting);
		break;
	case 27700:	// Britian
		return LatLon_to_NorthingEasting(dLat, dLon, dNorthing, dEasting);
		break;
	case 900913: // Bing
		LatLon_BingMapsXY(dLat, dLon, dNorthing, dEasting);
//		LatLon_to_OSM(dLat, dLon, dNorthing, dEasting);
		break;
//	case COUNTRY_SAT:
//		LatLon_BingMaps(dLat, dLon, dNorthing, dEasting);
//		break;
	default:
		WGS84_EastingNorthing(nProjection, dLat, dLon, dEasting, dNorthing);
		break;
	}

/*
	CString strQuadKey;
	switch (MyMap.GetCountry()) {
	case COUNTRY_BE:
		WGS84ToLambertBelgium2008(dLat, dLon, dNorthing, dEasting);
		break;
	case COUNTRY_FR:
		WGS84ToLambert93(dLat, dLon, dNorthing, dEasting);
		break;
	case COUNTRY_OSM:
		LatLon_to_OSM(dLat, dLon, dNorthing, dEasting);
		break;
	case COUNTRY_UK:
		return LatLon_to_NorthingEasting(dLat, dLon, dNorthing, dEasting);
		break;
//	case COUNTRY_SAT:
//		LatLon_BingMaps(dLat, dLon, dNorthing, dEasting);
//		break;
	default:
		int nProjection = MyMap.GetProjection();
		WGS84_EastingNorthing(nProjection, dLat, dLon, dEasting, dNorthing);
		break;
	}
*/
	return 0;
}
