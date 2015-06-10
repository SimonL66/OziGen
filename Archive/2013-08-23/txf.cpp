#include "stdafx.h"
#include "func.h"

#include <math.h>



struct latlon {
	long northing;
	long easting;
} * Points;



void DistanceFromLine(	double cx, double cy,
						double ax, double ay, double bx, double by,
						double * distanceSegment, double * distanceLine )
{
	//
	// find the distance from the point (cx,cy) to the line
	// determined by the points (ax,ay) and (bx,by)
	//
	// distanceSegment = distance from the point to the line segment
	// distanceLine = distance from the point to the line (assuming
	//					infinite extent in both directions
	//

/*

Subject 1.02: How do I find the distance from a point to a line?


    Let the point be C (Cx,Cy) and the line be AB (Ax,Ay) to (Bx,By).
    Let P be the point of perpendicular projection of C on AB.  The parameter
    r, which indicates P's position along AB, is computed by the dot product 
    of AC and AB divided by the square of the length of AB:
    
    (1)     AC dot AB
        r = ---------  
            ||AB||^2
    
    r has the following meaning:
    
        r=0      P = A
        r=1      P = B
        r<0      P is on the backward extension of AB
        r>1      P is on the forward extension of AB
        0<r<1    P is interior to AB
    
    The length of a line segment in d dimensions, AB is computed by:
    
        L = sqrt( (Bx-Ax)^2 + (By-Ay)^2 + ... + (Bd-Ad)^2)

    so in 2D:   
    
        L = sqrt( (Bx-Ax)^2 + (By-Ay)^2 )
    
    and the dot product of two vectors in d dimensions, U dot V is computed:
    
        D = (Ux * Vx) + (Uy * Vy) + ... + (Ud * Vd)
    
    so in 2D:   
    
        D = (Ux * Vx) + (Uy * Vy) 
    
    So (1) expands to:
    
            (Cx-Ax)(Bx-Ax) + (Cy-Ay)(By-Ay)
        r = -------------------------------
                          L^2

    The point P can then be found:

        Px = Ax + r(Bx-Ax)
        Py = Ay + r(By-Ay)

    And the distance from A to P = r*L.

    Use another parameter s to indicate the location along PC, with the 
    following meaning:
           s<0      C is left of AB
           s>0      C is right of AB
           s=0      C is on AB

    Compute s as follows:

            (Ay-Cy)(Bx-Ax)-(Ax-Cx)(By-Ay)
        s = -----------------------------
                        L^2


    Then the distance from C to P = |s|*L.

*/
	if (fabs(bx-ax)<1e-6 && fabs(by-ay)<1e-6) {
		*distanceLine = (cx-ax)*(cx-ax) + (cy-ay)*(cy-ay);
		*distanceSegment = *distanceLine;
		return;
	}

	double r_numerator = (cx-ax)*(bx-ax) + (cy-ay)*(by-ay);
	double r_denominator = (bx-ax)*(bx-ax) + (by-ay)*(by-ay);
	double r = r_numerator / r_denominator;

    double s = ( (ay-cy)*(bx-ax)-(ax-cx)*(by-ay) ) / r_denominator;

	*distanceLine = fabs(s)*sqrt(r_denominator);

	if ( (r >= 0) && (r <= 1) ) {
		*distanceSegment = *distanceLine;
	} else {
		double dist1 = (cx-ax)*(cx-ax) + (cy-ay)*(cy-ay);
		double dist2 = (cx-bx)*(cx-bx) + (cy-by)*(cy-by);
		if (dist1 < dist2) {
			*distanceSegment = sqrt(dist1);
		} else {
			*distanceSegment = sqrt(dist2);
		}
	}
}



void read_txf(const char * fname, long * nPoints, void ** PointsMemory)
{
char buffer[4096];

	*nPoints = 0;

	FILE * fp = fopen(fname, "r");
	if (fp == NULL) {
//		printf("Couldn't open %s\n", fname);
		return;
	}

	while (fgets(buffer, sizeof(buffer), fp) != NULL)
		(*nPoints)++;

	Points = (struct latlon *)(malloc(*nPoints * sizeof(struct latlon)));
	*PointsMemory = (void*)Points;
	if (Points == NULL) {
		printf("Couldn't allocate memory for points\n");
	}

	fseek(fp, 0, SEEK_SET);
	*nPoints = 0;
	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		double lat, lon, northing, easting;
		if (sscanf(buffer, "%lf, %lf,", &lat, &lon) != 2)
			break;
		OZIGEN_LatLon_to_NorthingEasting(lat, lon, &northing, &easting);
		Points[*nPoints].northing = (long)northing;
		Points[*nPoints].easting = (long)easting;
		(*nPoints)++;
	}
	fclose (fp);
}

long PointNearTrack(double lat, double lon, long nPoints, void * Track, int trackSize)
{
double distanceSegment, distanceLine;
long pt;
struct latlon * points;
double northing, easting;

	OZIGEN_LatLon_to_NorthingEasting(lat, lon, &northing, &easting);

	points = (struct latlon *)Track;
	for (pt = 0; pt < nPoints-1; pt++) {
		DistanceFromLine(easting, northing,
						points[pt].easting, points[pt].northing, points[pt+1].easting, points[pt+1].northing,
						&distanceSegment, &distanceLine);
		if (distanceSegment < trackSize)
			return 1;
	}
	return 0;
}


long TileNearTrack(double minlat, double maxlat, double minlon, double maxlon, long nPoints, void * Track, int trackSize)
{
int y,x;
	for (y=0; y<11; y++) {
		for (x=0; x<11; x++) {
			if (PointNearTrack(minlat+(maxlat-minlat)*y/10.0, minlon+(maxlon-minlon)*x/10.0, nPoints, Track, trackSize))
				return 1;
		}
	}
	return 0;
}

