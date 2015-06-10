#include "stdafx.h"

#include <math.h>

/** 
beanshell code to use the online maps form kompass-verlag
(http://www.kompass.de/online-karte.html) in "Mobile Atlas Creator"
(http://mobac.dnsalias.org/)

put it into your "mapsources"-directory...

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>

**/


//static import java.lang.Math.*;

//name = "Kompass Verlag";
//char *tileType = "png";

const double Kompass_PI       =	4*atan(1.0);	//Gantz used: PI=3.14159265;
const double Kompass_deg2rad  =	Kompass_PI/180.0;
const double Kompass_rad2deg  =	180.0/Kompass_PI;

int tileSize = 256; // optional
int minZoom = 6;   // optional
int maxZoom = 17;   // optional

int numTiles(double z)
{
	return (int)fabs(pow(2.0,z));
}

double toDegrees(double r)
{
	return r*Kompass_rad2deg;
}

double toRadians(double d)
{
	return d*Kompass_deg2rad;
}

double mercatorToLat(double mercatorY)
{
	return(toDegrees(atan(sinh(mercatorY))));
}

double latEdges1(double y,int z)
{
	int n = numTiles(z);
	double unit = 1.0 / n;
	double relY1 = y * unit;
	double lat1 = mercatorToLat(Kompass_PI * (1 - 2 * relY1));

	return lat1;
}

double latEdges2(double y,int z)
{
	int n = numTiles(z);
	double unit = 1.0 / n;
	double relY1 = y * unit;
	double relY2 = relY1 + unit;
	double lat2 = mercatorToLat(Kompass_PI * (1 - 2 * relY2));

	return lat2;
}

double lonEdges1(double x,int z)
{
	int n = numTiles(z);
	double unit = 360.0 / n;
	double lon1 = -180.0 + x * unit;

	return lon1;
}

double lonEdges2(double x,int z)
{
	int n = numTiles(z);
	double unit = 360.0 / n;
	double lon1 = -180.0 + x * unit;
	double lon2 = lon1 + unit;
 
	return lon2;
}

CString tileEdges(double x,double y,int z)
{
	CString str;

	str.Format("%f,%f,%f,%f",
				latEdges2(y,z),
   				lonEdges1(x,z),
				latEdges1(y,z),
				lonEdges2(x,z));

	return str;
}

double lon2mercator(double l)
{
	return (l * 20037508.34 / 180.0);
}

double lat2mercator(double l)
{
	double r = toRadians(l);
	double lat = log((1.0+sin(r)) / (1.0-sin(r)));

	return (lat * 20037508.34 / 2.0 / Kompass_PI);
}

CString mercatorTileEdges(double x, double y, int z)
{
	CString str;

	str.Format ("%f,%f,%f,%f",
				lon2mercator(lonEdges1(x,z)),
				lat2mercator(latEdges2(y,z)),
				lon2mercator(lonEdges2(x,z)),
				lat2mercator(latEdges1(y,z)));

	return str;
}

//	This method is called for each tile: input parameters for this script: "zoom", "x" and "y"
CString getTileUrl( int zoom, double x, double y )
{
	CString str;

	str.Format( "http://www.kompass.de/map/koogle.php?LAYERS=koogle&FORMAT=image%%2Fpng&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&EXCEPTIONS=application%%2Fvnd.ogc.se_inimage&SRS=EPSG:900913&BBOX=%s"
				"&WIDTH=256&HEIGHT=256",
				mercatorTileEdges(x,y,zoom));

	return str;
} 
