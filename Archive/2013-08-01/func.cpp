#include "stdafx.h"

#include <math.h>

#include "..\MapLib\mmmath.h"
#include "..\MapLib\Lambert93.h"
#include "func.h"

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


// Coordinate functions

CoordinateSystems g_CoordinateSystem=COORD_OSGB;

void OZIGEN_NorthingEasting_to_LatLon(double northing, double easting, double * lat, double * lon)
{
	switch (g_CoordinateSystem) {
	case COORD_BELGIUM_LAMBERT_2008:
		LambertBelgium2008ToWGS84(northing, easting, lat, lon);
		break;
	case COORD_FRANCE_LAMBERT_1993:
		Lambert93ToWGS84(northing, easting, lat, lon);
		break;
	case COORD_OSGB:
	default:
		NorthingEasting_to_LatLon(northing, easting, lat, lon);
		break;
	}
}

int OZIGEN_LatLon_to_NorthingEasting(double lat, double lon, double * northing, double * easting)
{
	switch (g_CoordinateSystem) {
	case COORD_BELGIUM_LAMBERT_2008:
		WGS84ToLambertBelgium2008(lat, lon, northing, easting);
		return 0;
		break;
	case COORD_FRANCE_LAMBERT_1993:
		WGS84ToLambert93(lat, lon, northing, easting);
		return 0;
		break;
	case COORD_OSGB:
	default:
		return LatLon_to_NorthingEasting(lat, lon, northing, easting);
		break;
	}
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
