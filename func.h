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

void OZIGEN_NorthingEasting_to_LatLon(double northing, double easting, double * lat, double * lon);
int  OZIGEN_LatLon_to_NorthingEasting(double lat, double lon, double * northing, double * easting);

void OZIGEN_EastingNorthing_to_LatLon(long easting, long northing, double * lat, double * lon);
void OZIGEN_LatLon_to_EastingNorthing(double lat, double lon, long * easting, long * northing);

int GetCustomMapTileCount(long minx, long maxx, long miny, long maxy, LPCSTR mapname, long tileWidth, long tileHeight, long trackSize);

void WGS84_EastingNorthing(int nEPSG, double lat, double lon, double* x, double* y);
void EastingNorthing_WGS84(int nEPSG, double x, double y, double* lat, double* lon);

void LatLon_to_OSM(double dLat, double dLon, double* dNorthing, double* dEasting);
void OSM_to_LatLon(double dNorthing, double dEasting, double* dLat, double* dLon);

void LatLon_BingMapsXY(double dLat, double dLon, double* tileX, double* tileY);

CString TileXYToQuadKey(int tileX, int tileY, int levelOfDetail);

void BingMapsXY_LatLon(double tileY, double tileX, double* dLat, double* dLon);
