#include "stdafx.h"

#include <direct.h>

#include <Wininet.h>

#include "OziGen.h"
#include "OziGenDlg.h"
#include "ProgressWindow.h"
#include "func.h"

#include "..\MapLib\map_conv.h"
#include "..\MapLib\png.h"
#include "..\MapLib\palette.h"
#include "..\MapLib\CoordConv.h"

/*
#include "Database.h"
#include "TileDatabase.h"
#include "WebMapServer.h"

#define printf visualprintf

extern CCustomMap MyMap;

extern CString g_strMapDatabaseFolder;
extern CString g_strMapScale;
extern CString g_strMapFlavour;


extern int abortProgram;

#define	NGI_TILE_WIDTH_10K	250


#define N_NGI_ZONES		55
*/

bool NGI_InDatabase(long northing, long easting)
{
/*
	struct map {
		char *	strZone_NGI;
	} zones[] = {

		"HP", "HT", "HU", "HW", "HX", "HY", "HZ", "NA",
		"NB", "NC", "ND", "NF", "NG", "NH", "NJ", "NK", "NL", "NM", "NN", "NO", "NR", "NS", "NT", "NU", "NW", "NX", "NY", "NZ",
		"OV", 
		"SD", "SE", "SH", "SJ", "SK", "SM", "SN", "SO", "SP", "SR", "SS", "ST", "SU", "SV", "SW", "SX", "SY", "SZ",
		"TA", "TF", "TG", "TL", "TM", "TQ", "TR", "TV"
	};

	char str[256];
	easting_northing_to_OS_Zone(easting, northing, str);

	CString strZone_NGI = str;

	for (int i=0; i < N_NGI_ZONES; i++) {
		if (strZone_NGI == zones[i].strZone_NGI) {
			return true;
		}
	}
*/
	return false;
}
