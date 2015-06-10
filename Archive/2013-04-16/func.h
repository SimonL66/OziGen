#pragma once

CString GetFilename(CString strPathName);
CString GetPath(CString strPathName);

CString RemoveSuffix(CString strFileName);

void BackupFile(CString strFileName);

double ConvertMetresToFeet(double metres);
double ConvertMetresToMiles(double metres);

CString MakeTitle(CString strTitle);

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
	COORD_LAMBERT93
};

void OZIGEN_NorthingEasting_to_LatLon(double northing, double easting, double * lat, double * lon);
int OZIGEN_LatLon_to_NorthingEasting(double lat, double lon, double * northing, double * easting);

void OZIGEN_EastingNorthing_to_LatLon(long easting, long northing, double * lat, double * lon);
void OZIGEN_LatLon_to_EastingNorthing(double lat, double lon, long * easting, long * northing);

extern CoordinateSystems g_CoordinateSystem;

int GetCustomMapTileCount(long minx, long maxx, long miny, long maxy, LPCSTR mapname, long tileWidth, long tileHeight, long trackSize);
