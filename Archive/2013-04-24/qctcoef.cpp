#include <stdio.h>
#include <math.h>
#include <memory.h>
#include "xmath/xpre.h"
#include "qctcoef.h"


/************************************/
// QCT coefficient calculator
/************************************/
#define MAX_N_POINTS	128
#define N_POINTS		101
#define	N_COEFFICENTS	10
struct xpr f[N_COEFFICENTS+1][MAX_N_POINTS*MAX_N_POINTS];

void CalculateCoefficients(cvtCoord ENtoLatLon, double * coefs, long northing_min, long northing_rng, long easting_min, long easting_rng, long minx, long width, long miny, long height)
{
struct xpr a[N_COEFFICENTS][N_COEFFICENTS+1];
double coef[N_COEFFICENTS];
double x,y;
long i,j,k, parameter;
double northing, easting;
double lat,lon;
struct xpr t;

	northing_min += northing_rng;

	for (parameter=0; parameter<4; parameter++) {
		for (i=0; i<N_POINTS*N_POINTS; i++) {
			easting = easting_min + easting_rng*(i%N_POINTS)/(N_POINTS-1.0);
			northing = northing_min - northing_rng*(i/N_POINTS)/(N_POINTS-1.0);

			(*ENtoLatLon)(northing, easting, &lat, &lon);
			switch (parameter) {
			case 0:
				y=lon;
				x=lat;
				f[10][i] = dbltox(0.5+minx+width*(i%N_POINTS)/(N_POINTS-1.0));
				break;
			case 1:
				y=lon;
				x=lat;
				f[10][i] = dbltox(0.5+miny+height*(i/N_POINTS)/(N_POINTS-1.0));
				break;
			case 2:
				x = 0.5+minx+width*(i%N_POINTS)/(N_POINTS-1.0);
				y = 0.5+miny+height*(i/N_POINTS)/(N_POINTS-1.0);
				f[10][i] = dbltox(lat);
				break;
			case 3:
				x = 0.5+minx+width*(i%N_POINTS)/(N_POINTS-1.0);
				y = 0.5+miny+height*(i/N_POINTS)/(N_POINTS-1.0);
				f[10][i] = dbltox(lon);
				break;
			}
			f[0][i] = xOne;
			f[1][i] = dbltox(x);
			f[2][i] = dbltox(y);
			f[3][i] = dbltox(x*x);
			f[4][i] = dbltox(x*y);
			f[5][i] = dbltox(y*y);
			f[6][i] = dbltox(x*x*x);
			f[7][i] = dbltox(x*x*y);
			f[8][i] = dbltox(x*y*y);
			f[9][i] = dbltox(y*y*y);
		}

		for (i=0; i<N_COEFFICENTS; i++) {
			for (j=0; j<=N_COEFFICENTS; j++) {
				a[i][j] = xZero;
				for (k=0; k<N_POINTS*N_POINTS; k++)
					a[i][j] = xadd(a[i][j], xmul(f[i][k], f[j][k]), 0);
			}
		}

/*		for (i=0; i<N_COEFFICENTS; i++) {
			for (j=0; j<N_COEFFICENTS+1; j++)
				printf("%g\t", a[i][j]);
			printf("\n");
		}
*/		// Perform Gaussian Elimination
		// eliminate
		for (i=0; i<N_COEFFICENTS; i++) {
			long max = i;
			for (j=i+1; j<N_COEFFICENTS; j++) {
				if (xgt(xabs(a[j][i]), xabs(a[max][i])))
					max = j;
			}
			for (k=i; k<=N_COEFFICENTS; k++) {
				t=a[i][k];
				a[i][k] = a[max][k];
				a[max][k] = t;
			}
			for (j=i+1; j<N_COEFFICENTS; j++) {
				for (k=N_COEFFICENTS; k>=i; k--) {
					 a[j][k] = xadd(a[j][k], xdiv(xmul(a[i][k],a[j][i]),a[i][i]), 1);
				}
			}
/*			printf("\n");
			for (int i0=0; i0<N_COEFFICENTS; i0++) {
				for (int j0=0; j0<N_COEFFICENTS+1; j0++)
					printf("%g\t", a[i0][j0]);
				printf("\n");
			}
*/		}

		// substitute
		for (j=N_COEFFICENTS-1; j>=0; j--) {
			t = a[j][N_COEFFICENTS];
			for (k=j+1; k<N_COEFFICENTS; k++)
				t = xadd(t, xmul(a[j][k], dbltox(coef[k])), 1);
			coef[j] = xtodbl(xdiv(t,a[j][j]));
		}

		memcpy(&coefs[parameter*10], coef, sizeof(coef));
	}
}


void latlon_to_pixel(const double * coefs, double x, double y, long * lat, long * lon)
{
	*lat =	(long)
			(coefs[0] +
			coefs[1] * x +
			coefs[2] * y +
			coefs[3] * x * x +
			coefs[4] * x * y +
			coefs[5] * y * y +
			coefs[6] * x * x * x +
			coefs[7] * x * x * y +
			coefs[8] * x * y * y +
			coefs[9] * y * y * y);
	*lon =	(long)
			(coefs[10] +
			coefs[11] * x +
			coefs[12] * y +
			coefs[13] * x * x +
			coefs[14] * x * y +
			coefs[15] * y * y +
			coefs[16] * x * x * x +
			coefs[17] * x * x * y +
			coefs[18] * x * y * y +
			coefs[19] * y * y * y);
}

void pixel_to_latlon(const double * coefs, double x, double y, double * lat, double * lon)
{
	x += 0.5;
	y += 0.5;
	*lat =	coefs[20] +
			coefs[21] * x +
			coefs[22] * y +
			coefs[23] * x * x +
			coefs[24] * x * y +
			coefs[25] * y * y +
			coefs[26] * x * x * x +
			coefs[27] * x * x * y +
			coefs[28] * x * y * y +
			coefs[29] * y * y * y;
	*lon =	coefs[30] +
			coefs[31] * x +
			coefs[32] * y +
			coefs[33] * x * x +
			coefs[34] * x * y +
			coefs[35] * y * y +
			coefs[36] * x * x * x +
			coefs[37] * x * x * y +
			coefs[38] * x * y * y +
			coefs[39] * y * y * y;
}

void CalibrationDiffs(cvtCoord ENtoLatLon, const double * coefs, long northing_min, long northing_rng, long easting_min, long easting_rng, long minx, long width, long miny, long height)
{
long x,y, newx,newy;
double lat, lon;
double newlat, newlon;
double dx=0.0, dy=0.0, dlat=0.0, dlon=0.0;
	northing_min += northing_rng;
	for (y=miny; y<height+miny; y+=height/20) {
		for (x=minx; x<width+minx; x+=width/20) {

			(*ENtoLatLon)(
				northing_min-(long)(northing_rng*((double)(y-miny))/height),
				easting_min+(long)(easting_rng*((double)(x-minx))/width),
				&lat, &lon);
			pixel_to_latlon(coefs, x, y, &newlat, &newlon);
			dlat += fabs((double)(lat-newlat));
			dlon += fabs((double)(lon-newlon));

			latlon_to_pixel(coefs, lat,lon, &newx,&newy);
			dx += fabs((double)(x-newx));
			dy += fabs((double)(y-newy));
		}
	}
	printf("Calibration diffs over 400 points Lat:%g Lon:%g x:%g y:%g\n", dlat, dlon, dx, dy);
}
