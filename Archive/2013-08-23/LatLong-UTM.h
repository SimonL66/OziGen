#pragma once

void LLtoUTM(int eId, double Lat, double Long,  double& Northing, double& Easting, int& Zone);
void UTMtoLL(int eId, double Northing, double Easting, int Zone,  double* Lat, double* Long);
