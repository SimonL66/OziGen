#pragma once

CString GetFilename(CString strPathName);
CString GetPath(CString strPathName);

CString RemoveSuffix(CString strFileName);

void BackupFile(CString strFileName);

double ConvertMetresToFeet(double metres);
double ConvertMetresToMiles(double metres);

CString MakeTitle(CString strTitle);

CString RemoveSpaces(CString strTitle);

bool IsFilePresent(CString strFile);

CString GetPoint (double dLat, double dLon);

void DoFileCopy(CString inputFilename, CString outputFilename);

CString getString (unsigned char* data, long offset);

long SearchBuffer (unsigned char* data, long data_length, long offset, char* string);

void DoEvents(void);

void ValidateMapExtents(long& minx, long& miny, long& maxx, long& maxy);

long CalculateCollar(long minx, long maxx, long miny, long maxy);



enum CoordinateSystems {
	COORD_OSGB,
	COORD_FRANCE_LAMBERT_1993,
	COORD_BELGIUM_LAMBERT_2008,
	COORD_GERMANY_GAUSS_KRUEGER,
	COORD_SPAIN,
	COORD_AUSTRIA,
	COORD_LUXEMBOURG,
	COORD_NETHERLANDS,
	COORD_PORTUGAL,
	COORD_OSNi,
	COORD_OSi,
	COORD_SWISS_GRID,
	COORD_ITALY,
	COORD_POLAND,
	COORD_CZECH,
	COORD_SLOVENIA
};

void OZIGEN_NorthingEasting_to_LatLon(double northing, double easting, double * lat, double * lon);
int OZIGEN_LatLon_to_NorthingEasting(double lat, double lon, double * northing, double * easting);

void OZIGEN_EastingNorthing_to_LatLon(long easting, long northing, double * lat, double * lon);
void OZIGEN_LatLon_to_EastingNorthing(double lat, double lon, long * easting, long * northing);

extern CoordinateSystems g_CoordinateSystem;

int GetCustomMapTileCount(long minx, long maxx, long miny, long maxy, LPCSTR mapname, long tileWidth, long tileHeight, long trackSize);

void WGS84toUTM(double lat, double lon, double& y, double& x, int& UTMZone);
void UTMtoWGS84(double northing, double easting, double* latitude, double* longitude);

void WGS84toED50(double lat, double lon, double& y, double& x);
void ED50toWGS84(double northing, double easting, double* latitude, double* longitude);

void WGS84toGaussKrueger(double lat, double lon, double& y, double& x);
void GaussKruegertoWGS84(double northing, double easting, double* latitude, double* longitude);
