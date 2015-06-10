#pragma once

// Projection Definitions
//	Austria;			EPSG:31287
//	Belgium;			EPSG:3812	
//	Britain;			EPSG:27700
//	Eire;				EPSG:
//	France;				EPSG:
//	Germany;			EPSG:31467
//	Italy;				EPSG:
//	Luxembourg;			EPSG:
//	Netherlands;		EPSG:
//	Northern Ireland;	EPSG:
//	Portugal;			EPSG:
//	Spain;				EPSG:23030
//		Catalonia;		EPSG:23031
//	Switzerland;		EPSG:
//	Poland;				EPSG:
//	Czech Republic;		EPSG:32633
//	Slovenia;			EPSG:3912
//	Finland;			EPSG:3067
//	Denmark;			EPSG:25832

// OSM;					EPSG:3857 - Not Sure if this is correct?

// WGS84;				EPSG:4326  - This is actually the WGS84 projection!

// Bing;				EPSG:900913  - Not Sure if this is correct?

#define EPSG_WGS84		"+proj=latlong +ellps=WGS84 +towgs84=0,0,0 +no_defs"

#define EPSG_23030		"+proj=utm +zone=30 +ellps=intl +towgs84=-131,-100.3,-163.4,-1.244,-0.020,-1.144,9.39 +units=m +no_defs"
#define EPSG_23031		"+proj=utm +zone=31 +ellps=intl +towgs84=-136.65549,-141.4658,-167.29848,-2.093111,-0.0014055,-0.1077098,11.54611 +units=m +no_defs"
#define EPSG_31467		"+proj=tmerc +lat_0=0 +lon_0=9 +k=1 +x_0=3500000 +y_0=0 +ellps=bessel +datum=potsdam +units=m +no_defs"
#define EPSG_32633		"+proj=utm +zone=33 +ellps=WGS84 +datum=WGS84 +units=m +no_defs"
#define EPSG_3912		"+proj=tmerc +lat_0=0 +lon_0=15 +k=0.9999 +x_0=500000 +y_0=-5000000 +ellps=bessel +towgs84=682,-203,480,0,0,0,0 +units=m +no_defs"
#define EPSG_31287		"+proj=lcc +lat_1=49 +lat_2=46 +lat_0=47.5 +lon_0=13.33333333333333 +x_0=400000 +y_0=400000 +ellps=bessel +towgs84=577.326,90.129,463.919,5.137,1.474,5.297,2.4232 +units=m +no_defs"
#define EPSG_3763		"+proj=tmerc +lat_0=39.66825833333333 +lon_0=-8.133108333333334 +k=1 +x_0=0 +y_0=0 +ellps=GRS80 +units=m +no_defs"


#define EPSG_3812		"+proj=lcc +lat_1=49.83333333333334 +lat_2=51.16666666666666 +lat_0=50.797815 +lon_0=4.359215833333333 +x_0=649328 +y_0=665262 +ellps=GRS80 +units=m +no_defs"
#define EPSG_27700		"+proj=tmerc +lat_0=49 +lon_0=-2 +k=0.9996012717 +x_0=400000 +y_0=-100000 +ellps=airy +datum=OSGB36 +units=m +no_defs"
#define EPSG_2180		"+proj=tmerc +lat_0=0 +lon_0=19 +k=0.9993 +x_0=500000 +y_0=-5300000 +ellps=GRS80 +units=m +no_defs"

#define EPSG_31468		"+proj=tmerc +lat_0=0 +lon_0=12 +k=1 +x_0=4500000 +y_0=0 +datum=potsdam +units=m +no_defs"

#define EPSG_3067		"+proj=utm +zone=35 +ellps=GRS80 +units=m +no_defs"

//#define EPSG_3857		"+proj=merc +lon_0=0 +k=1 +x_0=0 +y_0=0 +a=6378137 +b=6378137 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs"



#define EPSG_3857		"+title=Web Mercator +proj=merc +a=6378137 +b=6378137 +lat_ts=0.0 +lon_0=0.0 +x_0=0.0 +y_0=0 +k=1.0 +units=m +nadgrids=@null +no_defs"

#define EPSG_25832		"+proj=utm +zone=32 +ellps=GRS80 +units=m +no_defs"
