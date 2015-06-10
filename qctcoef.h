#ifndef _QCTCOEF_H_
#define	_QCTCOEF_H_	1


typedef void (*cvtCoord)(double northing, double easting, double * lat, double * lon);


void CalculateCoefficients(cvtCoord ENtoLatLon, double * coefs, long northing_min, long northing_rng, long easting_min, long easting_rng, long minx, long width, long miny, long height);
void CalibrationDiffs(cvtCoord ENtoLatLon, const double * coefs, long northing_min, long northing_rng, long easting_min, long easting_rng, long minx, long width, long miny, long height);


#endif	// _QCTCOEF_H_
