#include "stdafx.h"

#include <math.h>

#include "..\MapLib\mmmath.h"
#include "..\MapLib\Lambert93.h"
#include "func.h"

//#include "UTM.h"
#include "LatLong-UTM.h"

#include "CustomMap.h"

#include "proj_api.h"

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

//	x = lon × 6371000.0 × pi ÷ 180
//	y = ln(tan((pi ÷ 4) + (lat × pi ÷ 360))) × 6371000.0
 
void WGS84toUTM(double lat, double lon, double& y, double& x, int& UTMZone)
{
	LLtoUTM(22, lat, lon, y, x, UTMZone);
}

void UTMtoWGS84(double northing, double easting, double* latitude, double* longitude, int UTMZone)
{
	UTMtoLL(22, northing, easting, UTMZone, latitude,  longitude);
}

void WGS84toED50(double lat, double lon, double& y, double& x)
{

// c.defs["EPSG:23030"]="+proj=utm +zone=30 +ellps=intl +towgs84=-131,-100.3,-163.4,-1.244,-0.020,-1.144,9.39 +units=m +no_defs "});
//<23030> +proj=utm +zone=30 +ellps=intl +towgs84=-87,-98,-121,0,0,0,0 +units=m +no_defs  <>
//# ED50 - <4230> +proj=longlat +ellps=intl +towgs84=-87,-98,-121,0,0,0,0 +no_defs  <>

// 32630 - "+proj=utm +zone=30 +ellps=WGS84 +datum=WGS84 +units=m +no_defs";

        // From GPS WGS84 
projPJ        fromProj = pj_init_plus( "+proj=latlong " 
                                                "+ellps=WGS84 " 
                                                "+towgs84=0,0,0 "
// "+towgs84=-87,-98,-121,0,0,0,0 "	// From EPGS File.
//												"+towgs84=-131,-100.3,-163.4,-1.244,-0.020,-1.144,9.39 "
//"+towgs84=-125.098545,-76.000054,-156.198703,0.0,0.0,-1.129,8.30463103 "	// Roger Oberholtzer.
                                                "+no_defs"); 

        // To ED50 
projPJ        toProj   = pj_init_plus( "+proj=utm " 
                                                "+ellps=intl " 
                                                "+zone=30 " 
												"+towgs84=-131,-100.3,-163.4,-1.244,-0.020,-1.144,9.39 "
                                                "+units=m " 
                                                "+no_defs");



double xx = lon * DEG_TO_RAD; 
double yy = lat  * DEG_TO_RAD; 
double dAlt = 0; // We do not give this a height 

        pj_transform(fromProj, toProj, 1, 0, &xx, &yy, &dAlt); 

		x = xx;
		y = yy;

}

void ED50toWGS84(double northing, double easting, double* latitude, double* longitude)
{
        // From GPS WGS84 
projPJ        toProj = pj_init_plus( "+proj=latlong " 
                                                "+ellps=WGS84 " 
                                                "+towgs84=0,0,0 "	// Default.
//"+towgs84=-87,-98,-121,0,0,0,0 "	// From EPGS File.
//"+towgs84=-131,-100.3,-163.4,-1.244,-0.020,-1.144,9.39 " //	ED50 from Peter Robins.
//"+towgs84=-125.098545,-76.000054,-156.198703,0.0,0.0,-1.129,8.30463103 "	// Roger Oberholtzer.
"+no_defs"); 

toProj = pj_init_plus( "+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs" );
        // To ED50 

projPJ        fromProj   = pj_init_plus( "+proj=utm " 
                                                "+ellps=intl " 
                                                "+zone=30 " 
												"+towgs84=-131,-100.3,-163.4,-1.244,-0.020,-1.144,9.39 "
                                                "+units=m " 
                                                "+no_defs");


double xx = easting; 
double yy = northing; 
double dAlt = 0; // We do not give this a height 

        pj_transform(fromProj, toProj, 1, 0, &xx, &yy, &dAlt); 

		*latitude = yy * RAD_TO_DEG;
		*longitude = xx * RAD_TO_DEG;

}

//f.defs["EPSG:31467"]="+proj=tmerc +lat_0=0 +lon_0=9 +k=1 +x_0=3500000 +y_0=0 +ellps=bessel +datum=potsdam +units=m +no_defs"});

void WGS84toGaussKrueger(double lat, double lon, double& y, double& x)
{
	projPJ	fromProj = pj_init_plus( "+proj=latlong "
									 "+ellps=WGS84 "
									 "+towgs84=0,0,0 "
									 "+no_defs");

	projPJ	toProj   = pj_init_plus( "+proj=tmerc "
									 "+lat_0=0 "
									 "+lon_0=9 "
									 "+k=1 "
									 "+x_0=3500000 "
									 "+y_0=0"
									 "+ellps=bessel " 
									 "+datum=potsdam "
									 "+units=m "
									 "+no_defs");

	double xx = lon * DEG_TO_RAD; 
	double yy = lat  * DEG_TO_RAD; 
	double dAlt = 0; // We do not give this a height 

	pj_transform(fromProj, toProj, 1, 0, &xx, &yy, &dAlt); 

	x = xx;
	y = yy;
}

void GaussKruegertoWGS84(double northing, double easting, double* latitude, double* longitude)
{
	projPJ	toProj	= pj_init_plus( "+proj=latlong "
									 "+ellps=WGS84 "
									 "+towgs84=0,0,0 "
									 "+no_defs");

	projPJ	fromProj = pj_init_plus( "+proj=tmerc "
									 "+lat_0=0 "
									 "+lon_0=9 "
									 "+k=1 "
									 "+x_0=3500000 "
									 "+y_0=0"
									 "+ellps=bessel " 
									 "+datum=potsdam "
									 "+units=m "
									 "+no_defs");

	double xx = easting; 
	double yy = northing; 
	double dAlt = 0; // We do not give this a height.

	pj_transform(fromProj, toProj, 1, 0, &xx, &yy, &dAlt);

	*latitude = yy * RAD_TO_DEG;	
	*longitude = xx * RAD_TO_DEG;
}