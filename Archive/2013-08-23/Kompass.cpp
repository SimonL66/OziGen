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


static import java.lang.Math.*;

name = "Kompass Verlag";

tileType = "png";

tileSize = 256; // optional
minZoom = 6;   // optional
maxZoom = 17;   // optional

/**
This method is called for each tile: 
input parameters for this script: "zoom", "x" and "y"
**/
String getTileUrl( int zoom, int x, int y ) {
return ("http://www.kompass.de/map/koogle.php?LAYERS=koogle&FORMAT=image%2Fpng&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&EXCEPTIONS=application%2Fvnd.ogc.se_inimage&SRS=EPSG:900913&BBOX=" +  mercatorTileEdges(x,y,zoom) +
"&WIDTH=256&HEIGHT=256") ;
} 

numTiles(z){
    return abs(pow(2,z));
}

mercatorToLat(mercatorY){
  return(toDegrees(atan(sinh(mercatorY))));
}

latEdges1(y,z){
  n = numTiles(z);
  unit = 1 / n;
  relY1 = y * unit;
  lat1 = mercatorToLat(PI * (1 - 2 * relY1));
  return lat1;
}

latEdges2(y,z){
  n = numTiles(z);
  unit = 1 / n;
  relY1 = y * unit;
  relY2 = relY1 + unit;
  lat2 = mercatorToLat(PI * (1 - 2 * relY2));
  return lat2;
}

lonEdges1(x,z){
  n = numTiles(z);
  unit = 360 / n;
  lon1 = -180 + x * unit;
  return lon1;
}

lonEdges2(x,z){
  n = numTiles(z);
  unit = 360 / n;
  lon1 = -180 + x * unit;
  lon2 = lon1 + unit;
  return lon2;
}

tileEdges(x,y,z){
  return (latEdges2(y,z) + "," +
   	  lonEdges1(x,z) + "," +
	  latEdges1(y,z) + "," +
	  lonEdges2(x,z));
}

lon2mercator(l){
  return (l * 20037508.34 / 180);
}

lat2mercator(l){
 r = toRadians(l);
 lat = log((1+sin(r)) / (1-sin(r)));
 return (lat * 20037508.34 / 2 / PI);
}

mercatorTileEdges(x,y,z){
  return (lon2mercator(lonEdges1(x,z)) + "," +
          lat2mercator(latEdges2(y,z)) + "," +
	  lon2mercator(lonEdges2(x,z)) + "," +
	  lat2mercator(latEdges1(y,z)));
}
