#include "stdafx.h"
#include "MapExtents.h"
#include "func.h"

#include "..\MapLib\mmmath.h"
#include "..\MapLib\map_conv.h"

#define	N_NATIONAL_PARK_MAPS	12

struct NationalParkMaps {
	char *	strCoord1;
	char *	strCoord2;
	int		nNationalPark;
	RECT	rRectangle;
	bool	bNationalPark;
	char *	strLocation;
} NationalParkMaps[N_NATIONAL_PARK_MAPS] = {
	{"NY0342", "SD5777",  1, {0,0,0,0}, false, "Lake District"},
	{"SD9212", "SK3344",  2, {0,0,0,0}, false, "Peak District"},
	{"NY6110", "SE1850",  3, {0,0,0,0}, false, "Yorkshire Dales"},
	{"SH4680", "SO0394",  4, {0,0,0,0}, false, "Snowdonia"},
	{"SS4652", "SY1055",  5, {0,0,0,0}, false, "Dartmoor and Exmoor"},
	{"SN6146", "SO3700",  6, {0,0,0,0}, false, "Brecon Beacons"},
	{"NH4937", "NO5266",  7, {0,0,0,0}, false, "Cairngorms"},
	{"NN0635", "NS7280",  8, {0,0,0,0}, false, "Loch Lomond and the Trossachs"},
	{"SM5865", "SS4789",  9, {0,0,0,0}, false, "Pembrokeshire Coast"},
	{"NT6135", "NZ0864", 10, {0,0,0,0}, false, "Northumberland"},
	{"NZ4226", "TA1675", 11, {0,0,0,0}, false, "North York Moors"},
	{"SU0036", "SZ5189", 12, {0,0,0,0}, false, "New Forest"}
};

void GetNationalParkExtents (int nMapNumber, CString& strWestNorth, CString& strEastSouth, CString& strMap)
{
	for (int i=0; i < N_NATIONAL_PARK_MAPS; i++) {
		if (nMapNumber == NationalParkMaps[i].nNationalPark) {
			strWestNorth = NationalParkMaps[i].strCoord1;
			strEastSouth = NationalParkMaps[i].strCoord2;
			strMap.Format("NationalPark%03d",NationalParkMaps[i].nNationalPark);
			return;
		}
	}
}

int GetNationalParkMapNumber(int nMapNumber)
{
	if (nMapNumber <= 1) return 1;
	if (nMapNumber >= N_NATIONAL_PARK_MAPS) return N_NATIONAL_PARK_MAPS;
	return nMapNumber;
}

CString GetNationalParkMapName (int nMapNumber)
{
	for (int i=0; i < N_NATIONAL_PARK_MAPS; i++) {
		if (nMapNumber == NationalParkMaps[i].nNationalPark) {
			return NationalParkMaps[i].strLocation;
		}
	}
	return ("N/A");
}

#define	N_REGIONAL_AREA_MAPS	117

struct RegionalAreaMaps {
	char *	strCoord1;
	char *	strCoord2;
	int		nRegionalArea;
	RECT	rRectangle;
	bool	bRegionalAreaMap;
	char *	strLocation;
} RegionalAreaMaps[N_REGIONAL_AREA_MAPS] = {

	{"SQ 74 04", "SX 15 01",   1, {0,0,0,0}, false, "Cornwall West"},
	{"SS 10 33", "SX 46 34",   2, {0,0,0,0}, false, "Cornwall East"},
	{"SX 46 96", "SY 00 34",   3, {0,0,0,0}, false, "Dartmoor and South Devon"},
	{"SS 46 22", "SY 19 78",   4, {0,0,0,0}, false, "Mid Devon"},
	{"ST 19 29", "SY 72 67",   5, {0,0,0,0}, false, "Dorset West"},
	{"ST 67 29", "SZ 17 67",   6, {0,0,0,0}, false, "Cranborne Chase and Isle of Purbeck"},
	{"SU 09 29", "SZ 69 73",   7, {0,0,0,0}, false, "New Forest and Isle of Wight"},
	{"SU 69 29", "TV 48 91",   8, {0,0,0,0}, false, "South Downs West"},
	{"TQ 30 37", "TW 03 91",   9, {0,0,0,0}, false, "South Downs East"},
	{"SS 11 53", "ST 32 22",  10, {0,0,0,0}, false, "Exmoor and Quantock Hills"},
	{"ST 20 78", "ST 83 29",  11, {0,0,0,0}, false, "Mendip Hills and Somerset Levels"},
	{"ST 83 90", "SU 24 29",  12, {0,0,0,0}, false, "Marlborough Downs and Salisbury Plain"},
	{"SU 24 66", "SU 79 25",  13, {0,0,0,0}, false, "Winchester and Basingstoke"},
	{"SU 69 72", "TQ 22 29",  14, {0,0,0,0}, false, "Surrey South and West"},
	{"TQ 22 81", "TQ 75 25",  15, {0,0,0,0}, false, "Surrey Hills and The Weald"},
	{"TQ 75 81", "TR 44 13",  16, {0,0,0,0}, false, "Kent East"},
	{"SO 50 06", "SU 24 56",  17, {0,0,0,0}, false, "Cotswolds South and Bristol"},
	{"SP 24 06", "SU 69 57",  18, {0,0,0,0}, false, "Berkshire Downs"},
	{"SP 58 32", "TQ 14 72",  19, {0,0,0,0}, false, "The Chilterns"},
	{"TL 04 01", "TQ 58 54",  20, {0,0,0,0}, false, "London"},
	{"SM 65 57", "SS 35 91",  21, {0,0,0,0}, false, "Pembrokeshire"},
	{"SN 14 36", "SS 75 73",  22, {0,0,0,0}, false, "The Gower and Carmarthen"},
	{"SN 61 45", "ST 36 99",  23, {0,0,0,0}, false, "Brecon Beacons"},
	{"SN 75 12", "ST 31 64",  24, {0,0,0,0}, false, "South Wales Valleys"},
	{"SO 31 33", "ST 83 78",  25, {0,0,0,0}, false, "Forest of Dean"},
	{"SO 83 51", "SP 46 06",  26, {0,0,0,0}, false, "Cotswolds North"},
	{"SP 32 45", "SU 77 93",  27, {0,0,0,0}, false, "Oxford"},
	{"SP 69 45", "TQ 14 93",  28, {0,0,0,0}, false, "Milton Keynes and Luton"},
	{"TL 14 45", "TQ 56 97",  29, {0,0,0,0}, false, "Mid Hertfordshire"},
	{"TL 52 45", "TQ 88 81",  30, {0,0,0,0}, false, "Chelmsford and River Blackwater"},
	{"TL 83 53", "TR 49 81",  31, {0,0,0,0}, false, "Colchester, Ipswich and Dedham Vale"},
	{"SN 14 95", "SN 75 36",  32, {0,0,0,0}, false, "Cardigan Coast and Afon Teifi"},
	{"SN 75 70", "SO 29 28",  33, {0,0,0,0}, false, "Radnor Forest and Mynydd Epynt"},
	{"SO 22 70", "SO 69 22",  34, {0,0,0,0}, false, "Hereford and Wye Valley"},
	{"SO 58 75", "SO 99 22",  35, {0,0,0,0}, false, "Worcester and Malvern Hills"},
	{"SO 88 85", "SP 46 45",  36, {0,0,0,0}, false, "Shakespeare Country"},
	{"SP 46 81", "TL 10 45",  37, {0,0,0,0}, false, "Northampton"},
	{"TL 10 81", "TL 72 45",  38, {0,0,0,0}, false, "Cambridge"},
	{"TL 72 76", "TM 55 45",  39, {0,0,0,0}, false, "Mid Suffolk"},
	{"SH 75 11", "SO 25 66",  40, {0,0,0,0}, false, "Rhayader and Welshpool"},
	{"SJ 15 11", "SO 66 70",  41, {0,0,0,0}, false, "Shropshire Hills"},
	{"SJ 47 22", "SO 88 70",  42, {0,0,0,0}, false, "Wyre Forest and Severn Valley"},
	{"SJ 82 22", "SP 26 75",  43, {0,0,0,0}, false, "Birmingham and Cannock Chase"},
	{"SK 26 22", "SP 81 81",  44, {0,0,0,0}, false, "Leicester and Market Harborough"},
	{"SK 81 17", "TL 42 81",  45, {0,0,0,0}, false, "Rutland and Peterborough"},
	{"TF 42 08", "TM 15 76",  46, {0,0,0,0}, false, "Norfolk South West"},
	{"TG 15 48", "TM 59 75",  47, {0,0,0,0}, false, "Norfolk East"},
	{"SH 53 48", "SO 23 95",  48, {0,0,0,0}, false, "Snowdonia South and Berwyns"},
	{"SJ 03 48", "SJ 62 11",  49, {0,0,0,0}, false, "Shropshire North and West"},
	{"SJ 62 64", "SK 11 17",  50, {0,0,0,0}, false, "The Potteries"},
	{"SK 07 57", "SK 71 22",  51, {0,0,0,0}, false, "Derby and Nottingham"},
	{"SK 71 76", "TF 10 17",  52, {0,0,0,0}, false, "Grantham and Sleaford"},
	{"TF 10 66", "TF 73 17",  53, {0,0,0,0}, false, "Spalding and Boston"},
	{"TF 42 48", "TG 15 08",  54, {0,0,0,0}, false, "Norfolk North West"},
	{"SC 06 01", "SH 89 19",  55, {0,0,0,0}, false, "Snowdonia North, Lleyn Peninsula and Anglesea"},
	{"SH 74 88", "SJ 28 40",  56, {0,0,0,0}, false, "Clwydian Hills"},
	{"SD 17 24", "SJ 62 48",  57, {0,0,0,0}, false, "Chester and Merseyside"},
	{"SD 62 16", "SK 07 64",  58, {0,0,0,0}, false, "Greater Manchester"},
	{"SD 89 26", "SK 41 83",  59, {0,0,0,0}, false, "North Peak"},
	{"SJ 89 88", "SK 41 43",  60, {0,0,0,0}, false, "South Peak"},
	{"SE 41 21", "SK 76 57",  61, {0,0,0,0}, false, "Doncaster, Worksop and Mansfield"},
	{"SE 76 04", "TF 17 57",  62, {0,0,0,0}, false, "Lincoln and Gainsborough"},
	{"TA 10 10", "TF 63 66",  63, {0,0,0,0}, false, "Lincolnshire Wolds"},
	{"SD 25 54", "SD 89 05",  64, {0,0,0,0}, false, "Preston"},
	{"SD 44 72", "SE 01 32",  65, {0,0,0,0}, false, "Forest of Bowland"},
	{"SD 65 80", "SE 35 45",  66, {0,0,0,0}, false, "Yorkshire Dales South"},
	{"SD 89 61", "SE 41 26",  67, {0,0,0,0}, false, "Skipton, Harrogate and Leeds"},
	{"SE 35 75", "SE 76 21",  68, {0,0,0,0}, false, "Vale of York"},
	{"SE 76 94", "TA 31 37",  69, {0,0,0,0}, false, "Yorkshire Wolds and Flamborough Head"},
	{"SE 76 45", "TA 50 04",  70, {0,0,0,0}, false, "Kingston upon Hull"},
	{"NX 97 10", "SD 65 54",  71, {0,0,0,0}, false, "Lake District South"},
	{"NY 61 14", "SE 16 72",  72, {0,0,0,0}, false, "Yorkshire Dales North"},
	{"NZ 16 35", "SE 67 75",  73, {0,0,0,0}, false, "Teesside, Northallerton and Thirsk"},
	{"NZ 43 23", "TA 05 75",  74, {0,0,0,0}, false, "North York Moors"},
	{"NX 92 43", "NY 61 05",  75, {0,0,0,0}, false, "Lake District North"},
	{"NY 61 60", "NZ 16 14",  76, {0,0,0,0}, false, "Teesdale and Weardale"},
	{"NU 04 04", "NZ 67 35",  77, {0,0,0,0}, false, "Newcastle upon Tyne and Durham"},
	{"NY 02 92", "NY 61 43",  78, {0,0,0,0}, false, "Solway Firth"},
	{"NT 53 04", "NZ 08 60",  79, {0,0,0,0}, false, "Northumberland National Park, South"},
	{"NT 67 75", "NU 51 04",  80, {0,0,0,0}, false, "Northumberland National Park, North"},
	{"NW 88 90", "NX 56 28",  81, {0,0,0,0}, false, "Stranraer"},
	{"NS 56 01", "NY 02 42",  82, {0,0,0,0}, false, "Kirkcudbright and New Galloway"},
	{"NR 84 54", "NX 56 90",  83, {0,0,0,0}, false, "Ayr Coast and Arran"},
	{"NS 56 51", "NT 02 01",  84, {0,0,0,0}, false, "Lanarkshire South"},
	{"NT 02 51", "NY 41 92",  85, {0,0,0,0}, false, "Moffat, Biggar and Upper Tweeddale"},
	{"NT 41 63", "NY 78 92",  86, {0,0,0,0}, false, "Galashiels and Jedburgh"},
	{"NM 11 02", "NR 95 02",  87, {0,0,0,0}, false, "Jura, Islay and Mull of Kintyre"},
	{"NM 90 17", "NS 46 51",  88, {0,0,0,0}, false, "Argyll and Bute"},
	{"NS 12 87", "NS 84 50",  89, {0,0,0,0}, false, "Glasgow and Firth of Clyde"},
	{"NS 84 91", "NT 78 51",  90, {0,0,0,0}, false, "Edinburgh and Lothian"},
	{"NN 46 32", "NS 95 82",  91, {0,0,0,0}, false, "Stirling and The Trossachs"},
	{"NN 95 32", "NT 65 82",  92, {0,0,0,0}, false, "Fife"},
	{"NL 85 68", "NR 91 84",  93, {0,0,0,0}, false, "Mull, Coll, Tiree and Colonsay"},
	{"NM 91 61", "NN 46 17",  94, {0,0,0,0}, false, "Glen Coe, Rannoch Moor and Crianlarich"},
	{"NN 46 71", "NO 05 32",  95, {0,0,0,0}, false, "Loch Rannoch and Loch Tay"},
	{"NO 05 68", "NO 81 32",  96, {0,0,0,0}, false, "Dundee and Montrose"},
	{"HV 01 35", "NM 84 77",  97, {0,0,0,0}, false, "Outer Hebrides"},
	{"NG 04 79", "NM 83 77",  98, {0,0,0,0}, false, "Skye and the Small Isles"},
	{"NG 27 18", "NN 07 61",  99, {0,0,0,0}, false, "Knoydart, Morar and Moidart"},
	{"NN 07 98", "NN 78 61", 100, {0,0,0,0}, false, "Ben Nevis and Glen Spean"},
	{"NH 78 18", "NO 24 68", 101, {0,0,0,0}, false, "Cairngorm Mountains"},
	{"NJ 11 08", "NO 69 68", 102, {0,0,0,0}, false, "Royal Deeside and Lochnagar"},
	{"NJ 62 37", "NP 14 68", 103, {0,0,0,0}, false, "Aberdeen"},
	{"NG 73 41", "NN 33 98", 104, {0,0,0,0}, false, "Kintail and Glen Affric"},
	{"NH 33 54", "NN 78 98", 105, {0,0,0,0}, false, "Loch Ness"},
	{"NH 71 75", "NJ 24 18", 106, {0,0,0,0}, false, "Nairn and Elgin"},
	{"NJ 24 75", "NJ 62 08", 107, {0,0,0,0}, false, "Huntly and Keith"},
	{"NJ 53 80", "NK 21 26", 108, {0,0,0,0}, false, "Fraserburgh and Peterhead"},
	{"NB 66 01", "NH 13 33", 109, {0,0,0,0}, false, "Wester Ross"},
	{"NC 09 01", "NH 44 37", 110, {0,0,0,0}, false, "Ullapool and Loch Fannich"},
	{"NH 44 97", "NH 96 40", 111, {0,0,0,0}, false, "Dornoch Firth and Cromarty Firth"},
	{"NB 91 81", "NC 47 01", 112, {0,0,0,0}, false, "Assynt and Cape Wrath"},
	{"NC 44 31", "NJ 25 97", 113, {0,0,0,0}, false, "Lairg and Helmsdale"},
	{"NC 36 81", "NC 92 31", 114, {0,0,0,0}, false, "Tongue and Loch Naver"},
	{"NC 92 82", "ND 49 31", 115, {0,0,0,0}, false, "Thurso and John o' Groats"},
	{"HY 04 80", "NE 29 82", 116, {0,0,0,0}, false, "Orkney Islands and Fair Isle"},
	{"HO 67 41", "HZ 92 98", 117, {0,0,0,0}, false, "Shetland Islands"}
};

void GetRegionalAreaExtents (int nMapNumber, CString& strWestNorth, CString& strEastSouth, CString& strMap)
{
	for (int i=0; i < N_REGIONAL_AREA_MAPS; i++) {
		if (nMapNumber == RegionalAreaMaps[i].nRegionalArea) {
			strWestNorth = RegionalAreaMaps[i].strCoord1;
			strEastSouth = RegionalAreaMaps[i].strCoord2;
			strMap.Format("RegionalArea%03d",RegionalAreaMaps[i].nRegionalArea);
			return;
		}
	}
}

int GetRegionalAreaMapNumber(int nMapNumber)
{
	if (nMapNumber <= 1) return 1;
	if (nMapNumber >= N_REGIONAL_AREA_MAPS) return N_REGIONAL_AREA_MAPS;
	return nMapNumber;
}

CString GetRegionalAreaMapName (int nMapNumber)
{
	for (int i=0; i < N_REGIONAL_AREA_MAPS; i++) {
		if (nMapNumber == RegionalAreaMaps[i].nRegionalArea) {
			return RegionalAreaMaps[i].strLocation;
		}
	}
	return ("N/A");
}

#define	N_LARGE_REGIONAL_AREA_MAPS	10

struct LargeRegionalAreaMaps {
	char *	strCoord1;
	char *	strCoord2;
	int		nLargeRegionalArea;
	RECT	rRectangle;
	bool	bLargeRegionalAreaMap;
	char *	strLocation;
} LargeRegionalAreaMaps[N_LARGE_REGIONAL_AREA_MAPS] = {

	{"SL 70 30", "SZ 30 00",  1, {0,0,0,0}, false, "South West England"},
	{"SP 20 35", "TW 50 70",  2, {0,0,0,0}, false, "South East England"},
	{"SD 10 20", "SP 95 25",  3, {0,0,0,0}, false, "Central England"},
	{"SE 65 30", "TN 00 10",  4, {0,0,0,0}, false, "Eastern England"},
	{"NW 80 80", "TA 60 05",  5, {0,0,0,0}, false, "Northern England"},
	{"SB 40 20", "ST 60 60",  6, {0,0,0,0}, false, "Wales"},
	{"NR 80 90", "NZ 40 30",  7, {0,0,0,0}, false, "Scottish Lowlands and Borders"},
	{"NF 40 05", "NU 40 00",  8, {0,0,0,0}, false, "Central Scotland"},
	{"HV 00 40", "NN 55 10",  9, {0,0,0,0}, false, "North West Scotland"},
	{"HN 30 40", "NP 80 90", 10, {0,0,0,0}, false, "North East Scotland"}
};

void GetLargeRegionalAreaExtents (int nMapNumber, CString& strWestNorth, CString& strEastSouth, CString& strMap)
{
	for (int i=0; i < N_LARGE_REGIONAL_AREA_MAPS; i++) {
		if (nMapNumber == LargeRegionalAreaMaps[i].nLargeRegionalArea) {
			strWestNorth = LargeRegionalAreaMaps[i].strCoord1;
			strEastSouth = LargeRegionalAreaMaps[i].strCoord2;
			strMap.Format("LargeRegionalArea%03d",LargeRegionalAreaMaps[i].nLargeRegionalArea);
			return;
		}
	}
}

int GetLargeRegionalAreaMapNumber(int nMapNumber)
{
	if (nMapNumber <= 1) return 1;
	if (nMapNumber >= N_LARGE_REGIONAL_AREA_MAPS) return N_LARGE_REGIONAL_AREA_MAPS;
	return nMapNumber;
}

CString GetLargeRegionalAreaMapName (int nMapNumber)
{
	for (int i=0; i < N_LARGE_REGIONAL_AREA_MAPS; i++) {
		if (nMapNumber == LargeRegionalAreaMaps[i].nLargeRegionalArea) {
			return LargeRegionalAreaMaps[i].strLocation;
		}
	}
	return ("N/A");
}

/*

{"SD 93 10", "SK 33 80", 000, {0,0,0,0}, false, "[Explorers\OL1 Peak District - Dark Peak Area]"}
{"SD 58 90", "SE 10 50", 000, {0,0,0,0}, false, "[Explorers\OL2 Yorkshire Dales - Southern & Western areas]"}
{"NY 01 50", "NY 31 09", 000, {0,0,0,0}, false, "[Explorers\OL4 English Lakes - North-western area]"}
{"NY 30 50", "NY 60 09", 000, {0,0,0,0}, false, "[Explorers\OL5 English Lakes -  North-eastern area]"}
{"NY 01 11", "SD 31 61", 000, {0,0,0,0}, false, "[Explorers\OL6 English Lakes - South-western area]"}
{"NY 29 11", "SD 60 70", 000, {0,0,0,0}, false, "[Explorers\OL7 English Lakes - South-eastern area]"}
{"SS 50 52", "ST 10 26", 000, {0,0,0,0}, false, "[Explorers\OL9 Exmoor]"}
{"SN 67 36", "SO 11 04", 000, {0,0,0,0}, false, "[Explorers\OL12 Brecon Beacons National Park West & Central]"}
{"SO 10 45", "SO 40 05", 000, {0,0,0,0}, false, "[Explorers\OL13 Brecon Beacons National Park East]"}
{"SO 40 30", "ST 78 90", 000, {0,0,0,0}, false, "[Explorers\OL14 Wye Valley & Forest of Dean]"}
{"SY 45 96", "SZ 07 67", 000, {0,0,0,0}, false, "[Explorers\OL15 Purbeck & South Dorset]"}
{"NT 60 30", "NU 02 00", 000, {0,0,0,0}, false, "[Explorers\OL16 The Cheviot Hills]"}
{"SH 47 85", "SH 90 45", 000, {0,0,0,0}, false, "[Explorers\OL17 Snowdon ]"}
{"SH 54 55", "SH 99 15", 000, {0,0,0,0}, false, "[Explorers\OL18 Harlech, Porthmadog & Bala]"}
{"NY 60 30", "SD 90 90", 000, {0,0,0,0}, false, "[Explorers\OL19 Howgill Fells & Upper Eden Valley]"}
{"SX 48 66", "SX 95 34", 000, {0,0,0,0}, false, "[Explorers\OL20 South Devon - Brixham to Newton Ferrers]"}
{"SD 78 52", "SE 10 10", 000, {0,0,0,0}, false, "[Explorers\OL21 South Pennines]"}
{"SU 06 20", "SZ 50 89", 000, {0,0,0,0}, false, "[Explorers\OL22 New Forest]"}
{"SH 56 36", "SN 98 90", 000, {0,0,0,0}, false, "[Explorers\OL23 Cadair Idris & Bala Lake ]"}
{"SJ 92 80", "SK 36 50", 000, {0,0,0,0}, false, "[Explorers\OL24 Peak District - White Peak Area]"}
{"NZ 42 22", "SE 74 78", 000, {0,0,0,0}, false, "[Explorers\OL26 North York Moors - Western area]"}
{"NZ 70 21", "TA 05 79", 000, {0,0,0,0}, false, "[Explorers\OL27 North York Moors - Eastern Area]"}
{"SX 48 96", "SX 82 55", 000, {0,0,0,0}, false, "[Explorers\OL28 Dartmoor]"}
{"SU 29 05", "SZ 67 75", 000, {0,0,0,0}, false, "[Explorers\OL29 Isle of Wight]"}
{"NY 84 14", "SE 14 70", 000, {0,0,0,0}, false, "[Explorers\OL30 Yorkshire Dales - Northern & Central Areas]"}
{"NY 60 50", "NZ 10 10", 000, {0,0,0,0}, false, "[Explorers\OL31 North Pennines - Teesdale & Weardale]"}
{"SM 65 53", "SN 20 20", 000, {0,0,0,0}, false, "[Explorers\OL35 North Pembrokeshire ]"}
{"SM 45 24", "SS 20 92", 000, {0,0,0,0}, false, "[Explorers\OL36 South Pembrokeshire ]"}
{"TG 22 30", "TM 56 88", 000, {0,0,0,0}, false, "[Explorers\OL40 The Broads]"}
{"SD 47 70", "SD 89 38", 000, {0,0,0,0}, false, "[Explorers\OL41 Forest of Bowland & Ribblesdale]"}
{"NT 50 02", "NZ 10 80", 000, {0,0,0,0}, false, "[Explorers\OL42 Kielder Water]"}
{"NY 58 80", "NZ 00 50", 000, {0,0,0,0}, false, "[Explorers\OL43 Hadrian's Wall]"}
{"SP 00 42", "SP 30 00", 000, {0,0,0,0}, false, "[Explorers\OL45 The Cotswolds]"}
{"SV 80 19", "SV 96 03", 000, {0,0,0,0}, false, "[Explorers\101 Isles of Scilly]"}
{"SW 31 44", "SW 61 20", 000, {0,0,0,0}, false, "[Explorers\102 Land's End]"}
{"SW 61 37", "SW 83 10", 000, {0,0,0,0}, false, "[Explorers\103 The Lizard]"}
{"SW 61 64", "SW 83 34", 000, {0,0,0,0}, false, "[Explorers\104 Redruth & St Agnes]"}
{"SW 75 52", "SX 05 30", 000, {0,0,0,0}, false, "[Explorers\105 Falmouth & Mevagissey]"}
{"SW 79 82", "SX 03 52", 000, {0,0,0,0}, false, "[Explorers\106 Newquay  & Padstow]"}
{"SX 00 68", "SX 30 48", 000, {0,0,0,0}, false, "[Explorers\107 St Austell & Liskeard]"}
{"SX 30 78", "SX 53 48", 000, {0,0,0,0}, false, "[Explorers\108 Lower Tamar Valley & Plymouth]"}
{"SX 02 86", "SX 30 62", 000, {0,0,0,0}, false, "[Explorers\109 Bodmin Moor]"}
{"SX 78 90", "SY 00 60", 000, {0,0,0,0}, false, "[Explorers\110 Torquay & Dawlish]"}
{"SS 01 08", "SX 32 81", 000, {0,0,0,0}, false, "[Explorers\111 Bude, Boscastle & Tintagel]"}
{"SS 28 08", "SX 51 78", 000, {0,0,0,0}, false, "[Explorers\112 Launceston & Holsworthy]"}
{"SS 50 10", "SX 80 90", 000, {0,0,0,0}, false, "[Explorers\113 Okehampton]"}
{"SS 80 30", "SY 00 87", 000, {0,0,0,0}, false, "[Explorers\114 Exeter & the Exe Valley]"}
{"SS 99 10", "SY 21 80", 000, {0,0,0,0}, false, "[Explorers\115 Exmouth & Sidmouth]"}
{"ST 21 10", "SY 49 88", 000, {0,0,0,0}, false, "[Explorers\116 Lyme Regis & Bridport]"}
{"ST 47 10", "SY 90 90", 000, {0,0,0,0}, false, "[Explorers\117 Cerne Abbas & Bere Regis]"}
{"ST 83 30", "SZ 06 89", 000, {0,0,0,0}, false, "[Explorers\118 Shaftesbury & Cranborne Chase]"}
{"SU 48 25", "SZ 70 97", 000, {0,0,0,0}, false, "[Explorers\119 Meon Valley, Portsmouth, Gosport & Fareham]"}
{"SU 68 22", "SZ 90 92", 000, {0,0,0,0}, false, "[Explorers\120 Chichester]"}
{"SU 90 20", "TV 17 97", 000, {0,0,0,0}, false, "[Explorers\121 Arundel & Pulborough]"}
{"TQ 17 21", "TV 47 99", 000, {0,0,0,0}, false, "[Explorers\122 Brighton & Hove]"}
{"TQ 43 22", "TV 65 95", 000, {0,0,0,0}, false, "[Explorers\123 Eastbourne & Beachy Head]"}
{"TQ 61 24", "TQ 91 02", 000, {0,0,0,0}, false, "[Explorers\124 Hastings & Bexhill]"}
{"TQ 80 35", "TR 11 15", 000, {0,0,0,0}, false, "[Explorers\125 Romney Marsh, Rye & Winchelsea]"}
{"SS 20 28", "SS 50 08", 000, {0,0,0,0}, false, "[Explorers\126 Clovelly & Hartland]"}
{"SS 50 30", "SS 80 10", 000, {0,0,0,0}, false, "[Explorers\127 South Molton & Chulmleigh]"}
{"ST 00 30", "ST 40 07", 000, {0,0,0,0}, false, "[Explorers\128 Taunton & Blackdown Hills]"}
{"ST 40 30", "ST 83 10", 000, {0,0,0,0}, false, "[Explorers\129 Yeovil & Sherborne]"}
{"ST 97 60", "SU 20 20", 000, {0,0,0,0}, false, "[Explorers\130 Salisbury & Stonehenge]"}
{"SU 20 60", "SU 43 20", 000, {0,0,0,0}, false, "[Explorers\131 Romsey, Andover & Test Valley]"}
{"SU 40 40", "SU 70 20", 000, {0,0,0,0}, false, "[Explorers\132 Winchester, New Alresford & East Meon]"}
{"SU 70 40", "TQ 00 20", 000, {0,0,0,0}, false, "[Explorers\133 Haslemere & Petersfield]"}
{"TQ 00 40", "TQ 30 20", 000, {0,0,0,0}, false, "[Explorers\134 Crawley & Horsham]"}
{"TQ 30 40", "TQ 60 20", 000, {0,0,0,0}, false, "[Explorers\135 Ashdown Forest]"}
{"TQ 58 51", "TQ 80 22", 000, {0,0,0,0}, false, "[Explorers\136 High Weald]"}
{"TQ 80 55", "TR 10 35", 000, {0,0,0,0}, false, "[Explorers\137 Ashford]"}
{"TR 09 51", "TR 39 29", 000, {0,0,0,0}, false, "[Explorers\138 Dover, Folkestone & Hythe]"}
{"SS 12 50", "SS 60 20", 000, {0,0,0,0}, false, "[Explorers\139 Bideford, Ilfracombe & Barnstaple]"}
{"ST 10 50", "ST 40 28", 000, {0,0,0,0}, false, "[Explorers\140 Quantock Hills & Bridgwater]"}
{"ST 40 60", "ST 60 30", 000, {0,0,0,0}, false, "[Explorers\141 Cheddar Gorge & Mendip Hills West]"}
{"ST 60 60", "ST 80 30", 000, {0,0,0,0}, false, "[Explorers\142 Shepton Mallet & Mendip Hills East]"}
{"ST 80 60", "SU 00 30", 000, {0,0,0,0}, false, "[Explorers\143 Warminster & Trowbridge]"}
{"SU 40 60", "SU 80 37", 000, {0,0,0,0}, false, "[Explorers\144 Basingstoke, Alton & Whitchurch]"}
{"SU 80 60", "TQ 10 40", 000, {0,0,0,0}, false, "[Explorers\145 Guildford & Farnham]"}
{"TQ 10 60", "TQ 40 40", 000, {0,0,0,0}, false, "[Explorers\146 Dorking, Box Hill & Reigate]"}
{"TQ 40 65", "TQ 62 38", 000, {0,0,0,0}, false, "[Explorers\147 Sevenoaks & Tonbridge]"}
{"TQ 60 70", "TQ 90 50", 000, {0,0,0,0}, false, "[Explorers\148 Maidstone & the Medway Towns]"}
{"TQ 88 77", "TR 10 50", 000, {0,0,0,0}, false, "[Explorers\149 Sittingbourne & Faversham]"}
{"TR 10 73", "TR 42 51", 000, {0,0,0,0}, false, "[Explorers\150 Canterbury & the Isle of Thanet]"}
{"SS 78 86", "ST 24 62", 000, {0,0,0,0}, false, "[Explorers\151 Cardiff & Bridgend]"}
{"SO 20 06", "ST 42 76", 000, {0,0,0,0}, false, "[Explorers\152 Newport & Pontypool]"}
{"ST 20 73", "ST 42 43", 000, {0,0,0,0}, false, "[Explorers\153 Weston-Super-Mare & Bleadon Hill]"}
{"ST 40 90", "ST 60 60", 000, {0,0,0,0}, false, "[Explorers\154 Bristol West & Portishead]"}
{"ST 53 82", "ST 80 60", 000, {0,0,0,0}, false, "[Explorers\155 Bristol & Bath]"}
{"ST 80 83", "SU 00 53", 000, {0,0,0,0}, false, "[Explorers\156 Chippenham & Bradford-on-Avon]"}
{"ST 99 80", "SU 30 60", 000, {0,0,0,0}, false, "[Explorers\157 Marlborough & Savernake Forest]"}
{"SU 28 80", "SU 58 60", 000, {0,0,0,0}, false, "[Explorers\158 Newbury & Hungerford]"}
{"SU 55 80", "SU 85 60", 000, {0,0,0,0}, false, "[Explorers\159 Reading, Wokingham & Pangbourne]"}
{"SU 80 80", "TQ 10 60", 000, {0,0,0,0}, false, "[Explorers\160 Windsor, Weybridge & Bracknell]"}
{"TQ 10 80", "TQ 40 60", 000, {0,0,0,0}, false, "[Explorers\161 London South]"}
{"TQ 36 85", "TQ 66 65", 000, {0,0,0,0}, false, "[Explorers\162 Greenwich & Gravesend]"}
{"TQ 63 84", "TQ 90 62", 000, {0,0,0,0}, false, "[Explorers\163 Gravesend & Rochester]"}
{"SN 35 04", "SS 65 84", 000, {0,0,0,0}, false, "[Explorers\164 Gower]"}
{"SN 60 11", "SS 82 81", 000, {0,0,0,0}, false, "[Explorers\165 Swansea ]"}
{"SN 81 09", "ST 21 86", 000, {0,0,0,0}, false, "[Explorers\166 Rhondda & Merthyr Tydfil ]"}
{"SO 53 03", "ST 80 81", 000, {0,0,0,0}, false, "[Explorers\167 Thornbury, Dursley & Yate]"}
{"SO 78 07", "SU 00 80", 000, {0,0,0,0}, false, "[Explorers\168 Stroud, Tetbury & Malmesbury]"}
{"SP 00 05", "SU 21 75", 000, {0,0,0,0}, false, "[Explorers\169 Cirencester & Swindon]"}
{"SP 19 00", "SU 62 80", 000, {0,0,0,0}, false, "[Explorers\170 Abingdon, Wantage & Vale of White Horse]"}
{"SP 59 01", "SU 81 74", 000, {0,0,0,0}, false, "[Explorers\171 Chiltern Hills West]"}
{"SP 80 00", "TQ 10 80", 000, {0,0,0,0}, false, "[Explorers\172 Chiltern Hills East]"}
{"TL 10 00", "TQ 38 78", 000, {0,0,0,0}, false, "[Explorers\173 London North]"}
{"TL 30 15", "TQ 50 85", 000, {0,0,0,0}, false, "[Explorers\174 Epping Forest & Lee Valley]"}
{"TL 50 00", "TQ 93 80", 000, {0,0,0,0}, false, "[Explorers\175 Southend-on-Sea & Basildon]"}
{"TL 84 11", "TR 06 84", 000, {0,0,0,0}, false, "[Explorers\176 Blackwater Estuary]"}
{"SN 16 24", "SN 46 04", 000, {0,0,0,0}, false, "[Explorers\177 Carmarthen & Kidwelly]"}
{"SN 37 18", "SS 67 98", 000, {0,0,0,0}, false, "[Explorers\178 Llanelli & Ammanford]"}
{"SO 78 30", "SP 00 03", 000, {0,0,0,0}, false, "[Explorers\179 Gloucester, Cheltenham & Stroud]"}
{"SP 29 20", "SP 72 00", 000, {0,0,0,0}, false, "[Explorers\180 Oxford]"}
{"SP 72 20", "TL 02 00", 000, {0,0,0,0}, false, "[Explorers\181 Chiltern Hills North]"}
{"TL 00 20", "TL 30 00", 000, {0,0,0,0}, false, "[Explorers\182 St Albans & Hatfield]"}
{"TL 50 20", "TL 90 00", 000, {0,0,0,0}, false, "[Explorers\183 Chelmsford & The Rodings]"}
{"TL 90 34", "TM 27 10", 000, {0,0,0,0}, false, "[Explorers\184 Colchester]"}
{"SN 20 45", "SN 47 23", 000, {0,0,0,0}, false, "[Explorers\185 Newcastle Emlyn ]"}
{"SN 46 45", "SN 68 18", 000, {0,0,0,0}, false, "[Explorers\186 Llandeilo & Brechfa Forest ]"}
{"SN 67 68", "SN 90 32", 000, {0,0,0,0}, false, "[Explorers\187 Llandovery]"}
{"SN 90 53", "SO 20 33", 000, {0,0,0,0}, false, "[Explorers\188 Builth Wells ]"}
{"SO 40 42", "SO 70 20", 000, {0,0,0,0}, false, "[Explorers\189 Hereford & Ross-on-Wye]"}
{"SO 70 50", "SP 00 30", 000, {0,0,0,0}, false, "[Explorers\190 Malvern Hills & Bredon Hill]"}
{"SP 28 43", "SP 64 20", 000, {0,0,0,0}, false, "[Explorers\191 Banbury, Bicester & Chipping Norton]"}
{"SP 60 43", "TL 00 20", 000, {0,0,0,0}, false, "[Explorers\192 Buckingham & Milton Keynes]"}
{"TL 00 40", "TL 30 20", 000, {0,0,0,0}, false, "[Explorers\193 Luton & Stevenage]"}
{"TL 30 40", "TL 50 10", 000, {0,0,0,0}, false, "[Explorers\194 Hertford & Bishop's Stortford]"}
{"TL 47 40", "TL 90 20", 000, {0,0,0,0}, false, "[Explorers\195 Braintree & Saffron Walden]"}
{"TL 80 50", "TM 10 30", 000, {0,0,0,0}, false, "[Explorers\196 Sudbury, Hadleigh & Dedham Vale]"}
{"TM 10 51", "TM 40 29", 000, {0,0,0,0}, false, "[Explorers\197 Ipswich, Felixstowe & Harwich]"}
{"SN 13 69", "SN 52 40", 000, {0,0,0,0}, false, "[Explorers\198 Cardigan & New Quay ]"}
{"SN 48 70", "SN 70 43", 000, {0,0,0,0}, false, "[Explorers\199 Lampeter]"}
{"SN 80 73", "SO 20 50", 000, {0,0,0,0}, false, "[Explorers\200 Llandrindod Wells & Elan Valley ]"}
{"SO 20 83", "SO 40 40", 000, {0,0,0,0}, false, "[Explorers\201 Knighton & Presteigne ]"}
{"SO 40 60", "SO 70 40", 000, {0,0,0,0}, false, "[Explorers\202 Leominter & Bromyard]"}
{"SO 40 80", "SO 70 60", 000, {0,0,0,0}, false, "[Explorers\203 Ludlow]"}
{"SO 70 70", "SP 00 50", 000, {0,0,0,0}, false, "[Explorers\204 Worcester & Droitwich Spa]"}
{"SP 00 60", "SP 30 40", 000, {0,0,0,0}, false, "[Explorers\205 Stratford-upon-Avon & Evesham]"}
{"SP 30 60", "SP 60 40", 000, {0,0,0,0}, false, "[Explorers\206 Edge Hill & Fenny Compton]"}
{"SP 55 63", "SP 93 40", 000, {0,0,0,0}, false, "[Explorers\207 Newport Pagnell & Northampton South]"}
{"SP 93 63", "TL 33 40", 000, {0,0,0,0}, false, "[Explorers\208 Bedford & St Neots]"}
{"TL 33 63", "TL 60 40", 000, {0,0,0,0}, false, "[Explorers\209 Cambridge]"}
{"TL 60 70", "TL 80 40", 000, {0,0,0,0}, false, "[Explorers\210 Newmarket & Haverhill]"}
{"TL 80 70", "TM 23 50", 000, {0,0,0,0}, false, "[Explorers\211 Bury St Edmunds & Stowmarket]"}
{"TM 21 68", "TM 48 46", 000, {0,0,0,0}, false, "[Explorers\212 Woodbridge & Saxmundham]"}
{"SN 50 90", "SN 81 67", 000, {0,0,0,0}, false, "[Explorers\213 Aberystwyth & Cwm Rheidol]"}
{"SN 80 93", "SO 20 70", 000, {0,0,0,0}, false, "[Explorers\214 Llanidloes & Newtown ]"}
{"SH 75 10", "SO 18 90", 000, {0,0,0,0}, false, "[Explorers\215 Newtown & Machynlleth]"}
{"SJ 18 10", "SO 40 83", 000, {0,0,0,0}, false, "[Explorers\216 Welshpool & Montgomery ]"}
{"SJ 35 00", "SO 65 80", 000, {0,0,0,0}, false, "[Explorers\217 The Long Mynd & Wenlock Edge]"}
{"SJ 65 00", "SO 85 70", 000, {0,0,0,0}, false, "[Explorers\218 Kidderminster & Wyre Forest]"}
{"SJ 80 07", "SP 00 70", 000, {0,0,0,0}, false, "[Explorers\219 Wolverhampton & Dudley]"}
{"SK 00 03", "SP 20 60", 000, {0,0,0,0}, false, "[Explorers\220 Birmingham]"}
{"SP 20 90", "SP 40 60", 000, {0,0,0,0}, false, "[Explorers\221 Coventry & Warwick]"}
{"SP 40 90", "SP 60 60", 000, {0,0,0,0}, false, "[Explorers\222 Rugby & Daventry]"}
{"SP 60 94", "SP 83 56", 000, {0,0,0,0}, false, "[Explorers\223 Northampton & Market Harborough]"}
{"SK 80 00", "TL 03 60", 000, {0,0,0,0}, false, "[Explorers\224 Corby, Kettering & Wellingborough]"}
{"TL 03 80", "TL 46 60", 000, {0,0,0,0}, false, "[Explorers\225 Huntingdon & St Ives]"}
{"TL 46 82", "TL 76 62", 000, {0,0,0,0}, false, "[Explorers\226 Ely & Newmarket]"}
{"TF 03 03", "TL 43 80", 000, {0,0,0,0}, false, "[Explorers\227 Peterborough]"}
{"TF 39 03", "TL 75 80", 000, {0,0,0,0}, false, "[Explorers\228 March & Ely]"}
{"TF 75 00", "TL 95 70", 000, {0,0,0,0}, false, "[Explorers\229 Thetford Forest in The Brecks]"}
{"TL 90 90", "TM 30 67", 000, {0,0,0,0}, false, "[Explorers\230 Diss & Harleston]"}
{"TM 27 90", "TM 54 68", 000, {0,0,0,0}, false, "[Explorers\231 Southwold & Bungay]"}
{"SK 11 11", "SP 42 89", 000, {0,0,0,0}, false, "[Explorers\232 Nuneaton & Tamworth]"}
{"SK 41 10", "SP 82 87", 000, {0,0,0,0}, false, "[Explorers\233 Leicester & Hinckley]"}
{"SK 80 15", "TL 10 95", 000, {0,0,0,0}, false, "[Explorers\234 Rutland Water]"}
{"TF 10 20", "TL 50 97", 000, {0,0,0,0}, false, "[Explorers\235 Wisbech & Peterborough North]"}
{"TF 50 23", "TF 90 00", 000, {0,0,0,0}, false, "[Explorers\236 King's Lynn , Downham Market & Swaffham]"}
{"TF 89 13", "TM 29 90", 000, {0,0,0,0}, false, "[Explorers\237 Norwich]"}
{"TF 80 30", "TG 23 10", 000, {0,0,0,0}, false, "[Explorers\238 East Dereham & Aylsham]"}
{"SH 94 28", "SJ 21 06", 000, {0,0,0,0}, false, "[Explorers\239 Lake Vyrnwy & Llanfyllin]"}
{"SJ 20 40", "SJ 40 10", 000, {0,0,0,0}, false, "[Explorers\240 Oswestry]"}
{"SJ 40 40", "SJ 60 00", 000, {0,0,0,0}, false, "[Explorers\241 Shrewsbury]"}
{"SJ 60 20", "SJ 90 00", 000, {0,0,0,0}, false, "[Explorers\242 Telford, Ironbridge & The Wrekin]"}
{"SJ 60 40", "SJ 90 20", 000, {0,0,0,0}, false, "[Explorers\243 Market Drayton]"}
{"SJ 90 30", "SK 12 03", 000, {0,0,0,0}, false, "[Explorers\244 Cannock Chase & Chasewater]"}
{"SK 11 30", "SK 51 07", 000, {0,0,0,0}, false, "[Explorers\245 The National Forest]"}
{"SK 50 30", "SK 80 10", 000, {0,0,0,0}, false, "[Explorers\246 Loughborough]"}
{"SK 80 45", "TF 00 15", 000, {0,0,0,0}, false, "[Explorers\247 Grantham]"}
{"TF 00 45", "TF 20 15", 000, {0,0,0,0}, false, "[Explorers\248 Bourne & Heckington]"}
{"TF 20 40", "TF 63 20", 000, {0,0,0,0}, false, "[Explorers\249 Spalding & Holbeach]"}
{"TF 60 47", "TF 82 17", 000, {0,0,0,0}, false, "[Explorers\250 Norfolk Coast West]"}
{"TF 81 48", "TG 11 28", 000, {0,0,0,0}, false, "[Explorers\251 Norfolk Coast Central]"}
{"TG 10 45", "TG 40 23", 000, {0,0,0,0}, false, "[Explorers\252 Norfolk Coast East]"}
{"SH 10 45", "SH 40 20", 000, {0,0,0,0}, false, "[Explorers\253 Lleyn Peninsula West]"}
{"SH 34 61", "SH 57 34", 000, {0,0,0,0}, false, "[Explorers\254 Lleyn Peninsula East]"}
{"SH 98 47", "SJ 29 25", 000, {0,0,0,0}, false, "[Explorers\255 Llangollen & Berwyn]"}
{"SJ 07 59", "SJ 37 37", 000, {0,0,0,0}, false, "[Explorers\256 Wrexham & Llangollen]"}
{"SJ 37 60", "SJ 80 40", 000, {0,0,0,0}, false, "[Explorers\257 Crewe & Nantwich]"}
{"SJ 80 60", "SK 00 30", 000, {0,0,0,0}, false, "[Explorers\258 Stoke-on-Trent & Newcastle-under-Lyme]"}
{"SK 00 50", "SK 43 30", 000, {0,0,0,0}, false, "[Explorers\259 Derby]"}
{"SK 40 50", "SK 83 30", 000, {0,0,0,0}, false, "[Explorers\260 Nottingham]"}
{"TF 10 60", "TF 53 40", 000, {0,0,0,0}, false, "[Explorers\261 Boston]"}
{"SH 20 96", "SH 42 66", 000, {0,0,0,0}, false, "[Explorers\262 Anglesey West]"}
{"SH 35 95", "SH 66 60", 000, {0,0,0,0}, false, "[Explorers\263 Anglesey East ]"}
{"SH 90 85", "SJ 13 45", 000, {0,0,0,0}, false, "[Explorers\264 Vale of Clwyd ]"}
{"SJ 05 86", "SJ 25 55", 000, {0,0,0,0}, false, "[Explorers\265 Clwydian Hills ]"}
{"SJ 18 96", "SJ 48 59", 000, {0,0,0,0}, false, "[Explorers\266 Wirral & Chester ]"}
{"SJ 48 82", "SJ 75 60", 000, {0,0,0,0}, false, "[Explorers\267 Northwich & Delamere Forest]"}
{"SJ 74 90", "SJ 97 54", 000, {0,0,0,0}, false, "[Explorers\268 Wilmslow, Macclesfield & Congleton]"}
{"SK 32 81", "SK 52 50", 000, {0,0,0,0}, false, "[Explorers\269 Chesterfield & Alfreton]"}
{"SK 50 80", "SK 70 50", 000, {0,0,0,0}, false, "[Explorers\270 Sherwood Forest]"}
{"SK 70 88", "SK 90 45", 000, {0,0,0,0}, false, "[Explorers\271 Newark-on-Trent]"}
{"SK 90 88", "TF 10 45", 000, {0,0,0,0}, false, "[Explorers\272 Lincoln]"}
{"TF 10 80", "TF 40 60", 000, {0,0,0,0}, false, "[Explorers\273 Lincolnshire Wolds South]"}
{"TF 38 83", "TF 58 53", 000, {0,0,0,0}, false, "[Explorers\274 Skegness, Alford & Spilsby]"}
{"SD 29 01", "SJ 59 79", 000, {0,0,0,0}, false, "[Explorers\275 Liverpool]"}
{"SD 55 12", "SJ 75 82", 000, {0,0,0,0}, false, "[Explorers\276 Bolton, Wigan & Warrington]"}
{"SD 74 15", "SJ 97 88", 000, {0,0,0,0}, false, "[Explorers\277 Manchester & Salford]"}
{"SE 27 20", "SK 50 80", 000, {0,0,0,0}, false, "[Explorers\278 Sheffield & Barnsley]"}
{"SE 47 20", "SK 70 80", 000, {0,0,0,0}, false, "[Explorers\279 Doncaster]"}
{"SE 70 15", "SK 92 88", 000, {0,0,0,0}, false, "[Explorers\280 Isle of Axholme]"}
{"SE 87 26", "TF 10 88", 000, {0,0,0,0}, false, "[Explorers\281 Ancholme Valley]"}
{"TA 10 02", "TF 37 80", 000, {0,0,0,0}, false, "[Explorers\282 Lincolnshire Wolds North]"}
{"TA 32 08", "TF 54 78", 000, {0,0,0,0}, false, "[Explorers\283 Louth & Mablethorpe]"}
{"TA 10 26", "TF 33 96", 000, {0,0,0,0}, false, "[Explorers\284 Grimsby, Cleethorpes & Immingham]"}
{"SD 24 23", "SD 64 00", 000, {0,0,0,0}, false, "[Explorers\285 Southport & Chorley]"}
{"SD 30 41", "SD 60 21", 000, {0,0,0,0}, false, "[Explorers\286 Blackpool & Preston]"}
{"SD 57 40", "SD 80 10", 000, {0,0,0,0}, false, "[Explorers\287 West Pennine Moors]"}
{"SE 08 42", "SE 28 04", 000, {0,0,0,0}, false, "[Explorers\288 Bradford & Huddersfield]"}
{"SE 25 60", "SE 48 20", 000, {0,0,0,0}, false, "[Explorers\289 Leeds]"}
{"SE 48 60", "SE 71 20", 000, {0,0,0,0}, false, "[Explorers\290 York]"}
{"SE 67 42", "SE 89 15", 000, {0,0,0,0}, false, "[Explorers\291 Goole & Gilberdyke]"}
{"TA 19 45", "TA 43 09", 000, {0,0,0,0}, false, "[Explorers\292 Withernsea & Spurn Head]"}
{"SE 89 44", "TA 19 24", 000, {0,0,0,0}, false, "[Explorers\293 Kingston upon Hull & Beverley]"}
{"SE 70 60", "TA 00 40", 000, {0,0,0,0}, false, "[Explorers\294 Market Weighton & Yorkshire Wolds Central]"}
{"TA 00 71", "TA 23 44", 000, {0,0,0,0}, false, "[Explorers\295 Bridlington, Driffield & Hornsea]"}
{"SD 30 70", "SD 50 40", 000, {0,0,0,0}, false, "[Explorers\296 Lancaster, Morecambe & Fleetwood]"}
{"SE 05 60", "SE 35 40", 000, {0,0,0,0}, false, "[Explorers\297 Lower Wharfedale & Washburn Valley]"}
{"SE 05 80", "SE 32 58", 000, {0,0,0,0}, false, "[Explorers\298 Nidderdale]"}
{"SE 27 80", "SE 57 60", 000, {0,0,0,0}, false, "[Explorers\299 Ripon & Boroughbridge]"}
{"SE 57 80", "TA 00 60", 000, {0,0,0,0}, false, "[Explorers\300 Howardian Hills & Malton]"}
{"TA 00 95", "TA 26 65", 000, {0,0,0,0}, false, "[Explorers\301 Scarborough, Bridlington & Flamborough Head]"}
{"NZ 14 00", "SE 45 80", 000, {0,0,0,0}, false, "[Explorers\302 Northallerton & Thirsk]"}
{"NX 92 34", "NY 14 03", 000, {0,0,0,0}, false, "[Explorers\303 Whitehaven & Workington]"}
{"NZ 08 21", "SE 44 98", 000, {0,0,0,0}, false, "[Explorers\304 Darlington & Richmond]"}
{"NZ 10 40", "NZ 40 20", 000, {0,0,0,0}, false, "[Explorers\305 Bishop Auckland]"}
{"NZ 40 38", "NZ 70 16", 000, {0,0,0,0}, false, "[Explorers\306 Middlesbrough & Hartlepool]"}
{"NY 90 60", "NZ 20 38", 000, {0,0,0,0}, false, "[Explorers\307 Consett & Derwent Reservoir]"}
{"NZ 18 60", "NZ 48 38", 000, {0,0,0,0}, false, "[Explorers\308 Durham & Sunderland]"}
{"NW 95 75", "NX 28 30", 000, {0,0,0,0}, false, "[Explorers\309 Stranraer & The Rhins]"}
{"NX 15 78", "NX 36 48", 000, {0,0,0,0}, false, "[Explorers\310 Glenluce & Kirkcowan]"}
{"NX 28 63", "NX 50 33", 000, {0,0,0,0}, false, "[Explorers\311 Wigtown, Whithorn & The Machars]"}
{"NX 50 63", "NX 80 42", 000, {0,0,0,0}, false, "[Explorers\312 Kirkcudbright & Castle Douglas]"}
{"NX 80 78", "NY 00 48", 000, {0,0,0,0}, false, "[Explorers\313 Dumfries & Dalbeattie Forest]"}
{"NY 00 67", "NY 30 47", 000, {0,0,0,0}, false, "[Explorers\314 Solway Firth]"}
{"NY 29 70", "NY 60 50", 000, {0,0,0,0}, false, "[Explorers\315 Carlisle]"}
{"NZ 00 80", "NZ 42 60", 000, {0,0,0,0}, false, "[Explorers\316 Newcastle upon Tyne]"}
{"NS 01 01", "NX 34 75", 000, {0,0,0,0}, false, "[Explorers\317 Ballantrae, Barr & Barrhill]"}
{"NS 32 00", "NX 63 78", 000, {0,0,0,0}, false, "[Explorers\318 Galloway Forest Park North]"}
{"NX 35 82", "NX 66 62", 000, {0,0,0,0}, false, "[Explorers\319 Galloway Forest Park South]"}
{"NX 61 92", "NX 81 61", 000, {0,0,0,0}, false, "[Explorers\320 Castle Douglas, Loch Ken & New Galloway]"}
{"NS 78 00", "NY 00 73", 000, {0,0,0,0}, false, "[Explorers\321 Nithsdale & Dumfries]"}
{"NT 00 04", "NY 23 64", 000, {0,0,0,0}, false, "[Explorers\322 Annandale]"}
{"NT 18 04", "NY 41 64", 000, {0,0,0,0}, false, "[Explorers\323 Eskdale & Castle O'er Forest]"}
{"NT 38 00", "NY 58 70", 000, {0,0,0,0}, false, "[Explorers\324 Liddesdale & Kershope Forest]"}
{"NU 06 02", "NZ 33 80", 000, {0,0,0,0}, false, "[Explorers\325 Morpeth & Blyth]"}
{"NS 17 34", "NX 48 96", 000, {0,0,0,0}, false, "[Explorers\326 Ayr & Troon]"}
{"NS 40 30", "NS 60 00", 000, {0,0,0,0}, false, "[Explorers\327 Cumnock & Dalmellington]"}
{"NS 56 30", "NX 79 90", 000, {0,0,0,0}, false, "[Explorers\328 Sanquhar & New Cumnock]"}
{"NS 77 24", "NY 00 97", 000, {0,0,0,0}, false, "[Explorers\329 Lowther Hills, Sanquhar & Leadhills]"}
{"NT 00 24", "NT 30 04", 000, {0,0,0,0}, false, "[Explorers\330 Moffat & St Mary's Loch]"}
{"NT 30 20", "NT 60 00", 000, {0,0,0,0}, false, "[Explorers\331 Teviotdale South]"}
{"NU 02 24", "NU 32 01", 000, {0,0,0,0}, false, "[Explorers\332 Alnwick & Amble]"}
{"NS 26 59", "NS 48 29", 000, {0,0,0,0}, false, "[Explorers\333 Kilmarnock & Irvine]"}
{"NS 48 56", "NS 70 29", 000, {0,0,0,0}, false, "[Explorers\334 East Kilbride, Galston & Darvel]"}
{"NS 69 46", "NS 99 24", 000, {0,0,0,0}, false, "[Explorers\335 Lanark & Tinto Hills]"}
{"NS 98 49", "NT 20 22", 000, {0,0,0,0}, false, "[Explorers\336 Biggar & Broughton]"}
{"NT 20 50", "NT 40 20", 000, {0,0,0,0}, false, "[Explorers\337 Peebles & Innerleithen]"}
{"NT 40 50", "NT 62 20", 000, {0,0,0,0}, false, "[Explorers\338 Galashiels, Selkirk & Melrose]"}
{"NT 57 50", "NU 00 30", 000, {0,0,0,0}, false, "[Explorers\339 Kelso, Coldstream & Lower Tweed Valley]"}
{"NT 99 50", "NU 27 23", 000, {0,0,0,0}, false, "[Explorers\340 Holy Island & Bamburgh]"}
{"NS 14 78", "NS 41 40", 000, {0,0,0,0}, false, "[Explorers\341 Greenock, Largs & Millport]"}
{"NS 41 75", "NS 71 55", 000, {0,0,0,0}, false, "[Explorers\342 Glasgow]"}
{"NS 70 66", "NT 00 46", 000, {0,0,0,0}, false, "[Explorers\343 Motherwell & Coatbridge]"}
{"NT 00 68", "NT 30 48", 000, {0,0,0,0}, false, "[Explorers\344 Pentland Hills]"}
{"NT 29 70", "NT 72 50", 000, {0,0,0,0}, false, "[Explorers\345 Lammermuir Hills]"}
{"NT 72 72", "NU 02 50", 000, {0,0,0,0}, false, "[Explorers\346 Berwick-upon-Tweed]"}
{"NS 20 95", "NS 52 73", 000, {0,0,0,0}, false, "[Explorers\347 Loch Lomond South]"}
{"NS 50 95", "NS 80 74", 000, {0,0,0,0}, false, "[Explorers\348 Campsie Fells]"}
{"NS 70 88", "NT 08 65", 000, {0,0,0,0}, false, "[Explorers\349 Falkirk, Cumbernauld & Livingston]"}
{"NT 07 86", "NT 37 66", 000, {0,0,0,0}, false, "[Explorers\350 Edinburgh]"}
{"NT 32 88", "NT 79 68", 000, {0,0,0,0}, false, "[Explorers\351 Dunbar &  North Berwick]"}
{"NR 26 67", "NR 48 40", 000, {0,0,0,0}, false, "[Explorers\352 Islay South]"}
{"NR 15 81", "NR 45 51", 000, {0,0,0,0}, false, "[Explorers\353 Islay North]"}
{"NM 30 02", "NR 52 75", 000, {0,0,0,0}, false, "[Explorers\354 Colonsay & Oronsay]"}
{"NM 42 07", "NR 74 61", 000, {0,0,0,0}, false, "[Explorers\355 Jura & Scarba]"}
{"NR 58 45", "NR 83 03", 000, {0,0,0,0}, false, "[Explorers\356 Kintyre South]"}
{"NR 62 73", "NR 93 43", 000, {0,0,0,0}, false, "[Explorers\357 Kintyre North]"}
{"NM 66 07", "NR 98 71", 000, {0,0,0,0}, false, "[Explorers\358 Lochgilphead & Knapdale North]"}
{"NM 63 36", "NM 93 05", 000, {0,0,0,0}, false, "[Explorers\359 Oban, Kerrera & Loch Melfort]"}
{"NM 85 30", "NS 19 96", 000, {0,0,0,0}, false, "[Explorers\360 Loch Awe]"}
{"NR 86 53", "NS 09 18", 000, {0,0,0,0}, false, "[Explorers\361 Isle of  Arran]"}
{"NR 90 99", "NS 12 51", 000, {0,0,0,0}, false, "[Explorers\362 Cowal West & Isle of Bute]"}
{"NN 04 13", "NS 25 67", 000, {0,0,0,0}, false, "[Explorers\363 Cowal East]"}
{"NN 18 32", "NS 42 93", 000, {0,0,0,0}, false, "[Explorers\364 Loch Lomond North]"}
{"NN 41 31", "NS 65 93", 000, {0,0,0,0}, false, "[Explorers\365 The Trossachs]"}
{"NN 63 10", "NT 00 87", 000, {0,0,0,0}, false, "[Explorers\366 Stirling & Ochil Hills West]"}
{"NN 93 04", "NT 43 80", 000, {0,0,0,0}, false, "[Explorers\367 Dunfermline, Kirkcaldy & Glenrothes South]"}
{"NN 61 31", "NN 98 08", 000, {0,0,0,0}, false, "[Explorers\368 Crieff, Comrie & Glen Artney]"}
{"NN 98 28", "NT 18 98", 000, {0,0,0,0}, false, "[Explorers\369 Perth & Kinross]"}
{"NO 15 22", "NO 45 00", 000, {0,0,0,0}, false, "[Explorers\370 Glenrothes North, Falkland & Lomond Hills]"}
{"NO 30 30", "NT 65 99", 000, {0,0,0,0}, false, "[Explorers\371 St Andrews & East Fife]"}
{"NL 90 67", "NM 31 35", 000, {0,0,0,0}, false, "[Explorers\372 Coll & Tiree]"}
{"NM 23 37", "NM 55 11", 000, {0,0,0,0}, false, "[Explorers\373 Ross of Mull]"}
{"NM 23 60", "NM 60 34", 000, {0,0,0,0}, false, "[Explorers\374 Isle of Mull North, Tobermory & Staffa]"}
{"NM 40 44", "NM 76 19", 000, {0,0,0,0}, false, "[Explorers\375 Isle of Mull South East]"}
{"NM 77 58", "NN 01 28", 000, {0,0,0,0}, false, "[Explorers\376 Oban & North Lorn]"}
{"NM 99 48", "NN 39 25", 000, {0,0,0,0}, false, "[Explorers\377 Loch Etive & Glen Orchy]"}
{"NN 37 53", "NN 78 30", 000, {0,0,0,0}, false, "[Explorers\378 Ben Lawers & Glen Lyon]"}
{"NN 76 50", "NO 13 27", 000, {0,0,0,0}, false, "[Explorers\379 Dunkeld, Aberfeldy & Glen Almond]"}
{"NO 13 47", "NO 51 19", 000, {0,0,0,0}, false, "[Explorers\380 Dundee & Sidlaw Hills]"}
{"NO 13 61", "NO 43 39", 000, {0,0,0,0}, false, "[Explorers\381 Blairgowrie, Kirriemuir & Glamis]"}
{"NO 50 73", "NO 82 30", 000, {0,0,0,0}, false, "[Explorers\382 Arbroath, Montrose & Carnoustie]"}
{"NM 54 63", "NM 95 39", 000, {0,0,0,0}, false, "[Explorers\383 Morvern, Kingairloch & Lochaline]"}
{"NM 99 68", "NN 29 45", 000, {0,0,0,0}, false, "[Explorers\384 Glen Coe & Glen Etive]"}
{"NN 28 73", "NN 63 48", 000, {0,0,0,0}, false, "[Explorers\385 Rannoch Moor & Ben Alder]"}
{"NN 60 71", "NO 00 48", 000, {0,0,0,0}, false, "[Explorers\386 Pitlochry & Loch Tummel]"}
{"NO 00 92", "NO 20 50", 000, {0,0,0,0}, false, "[Explorers\387 Glen Shee & Braemar]"}
{"NO 17 97", "NO 40 61", 000, {0,0,0,0}, false, "[Explorers\388 Lochnagar, Glen Muick & Glen Clova]"}
{"NO 40 77", "NO 62 47", 000, {0,0,0,0}, false, "[Explorers\389 Strathmore, Blairgowrie & Forfar]"}
{"NM 38 83", "NM 80 59", 000, {0,0,0,0}, false, "[Explorers\390 Ardnamurchan]"}
{"NM 80 81", "NN 10 59", 000, {0,0,0,0}, false, "[Explorers\391 Ardgour]"}
{"NN 05 83", "NN 35 61", 000, {0,0,0,0}, false, "[Explorers\392 Ben Nevis & Fort William]"}
{"NN 34 90", "NN 65 67", 000, {0,0,0,0}, false, "[Explorers\393 Ben Alder, Loch Ericht & Loch Laggan]"}
{"NN 62 90", "NO 00 67", 000, {0,0,0,0}, false, "[Explorers\394 Atholl]"}
{"NJ 40 02", "NO 62 75", 000, {0,0,0,0}, false, "[Explorers\395 Glen Esk & Glen Tanar]"}
{"NO 62 92", "NO 92 70", 000, {0,0,0,0}, false, "[Explorers\396 Stonehaven & Inverbervie]"}
{"NG 14 07", "NM 50 77", 000, {0,0,0,0}, false, "[Explorers\397 Rum, Eigg, Muck, Canna & Sanday]"}
{"NG 60 02", "NN 00 79", 000, {0,0,0,0}, false, "[Explorers\398 Loch Morar, Mallaig, Arisaig & Glenfinnan]"}
{"NG 98 04", "NN 20 73", 000, {0,0,0,0}, false, "[Explorers\399 Loch Arkaig & Fort William]"}
{"NH 17 11", "NN 39 81", 000, {0,0,0,0}, false, "[Explorers\400 Loch Lochy & Glen Roy, Fort Augustus & Spean Bridg]"}
{"NH 37 10", "NN 59 80", 000, {0,0,0,0}, false, "[Explorers\401 Loch Laggan & Corrieyairack Forest]"}
{"NH 58 10", "NN 85 88", 000, {0,0,0,0}, false, "[Explorers\402 Strathspey - Kingussie & Newtonmore]"}
{"NH 84 23", "NO 07 86", 000, {0,0,0,0}, false, "[Explorers\403 Cairn Gorm & Aviemore]"}
{"NJ 03 22", "NO 26 88", 000, {0,0,0,0}, false, "[Explorers\404 Braemar, Tomintoul & Glen Avon]"}
{"NJ 26 19", "NO 66 96", 000, {0,0,0,0}, false, "[Explorers\405 Aboyne, Alford & Strathdon]"}
{"NJ 61 14", "NO 98 91", 000, {0,0,0,0}, false, "[Explorers\406 Aberdeen & Banchory]"}
{"NG 12 68", "NG 34 36", 000, {0,0,0,0}, false, "[Explorers\407 Skye - Dunvegan]"}
{"NG 31 82", "NG 53 50", 000, {0,0,0,0}, false, "[Explorers\408 Skye - Trotternish & The Storr]"}
{"NG 47 64", "NG 69 27", 000, {0,0,0,0}, false, "[Explorers\409 Raasay, Rona & Scalpay]"}
{"NG 27 50", "NG 57 30", 000, {0,0,0,0}, false, "[Explorers\410 Skye - Portree & Bracadale]"}
{"NG 30 33", "NG 62 11", 000, {0,0,0,0}, false, "[Explorers\411 Skye - Cuillin Hills]"}
{"NG 55 29", "NM 77 99", 000, {0,0,0,0}, false, "[Explorers\412 Skye - Sleat]"}
{"NG 69 31", "NM 97 96", 000, {0,0,0,0}, false, "[Explorers\413 Knoydart, Loch Hourn & Loch Duich]"}
{"NG 93 33", "NH 16 00", 000, {0,0,0,0}, false, "[Explorers\414 Glen Shiel & Kintail Forest]"}
{"NH 15 34", "NH 37 03", 000, {0,0,0,0}, false, "[Explorers\415 Glen Affric & Glen Morriston, Cannich]"}
{"NH 37 50", "NH 77 08", 000, {0,0,0,0}, false, "[Explorers\416 Inverness, Culloden Moor & Loch Ness]"}
{"NH 57 32", "NH 84 10", 000, {0,0,0,0}, false, "[Explorers\417 Monadhliath Mountains North & Strathdearn]"}
{"NH 74 42", "NJ 04 22", 000, {0,0,0,0}, false, "[Explorers\418 Lochindorb]"}
{"NH 99 52", "NJ 23 19", 000, {0,0,0,0}, false, "[Explorers\419 Grantown-on-Spey & Hills of Cromdale]"}
{"NJ 21 33", "NJ 69 13", 000, {0,0,0,0}, false, "[Explorers\420 Correen Hills & Glenlivet]"}
{"NJ 66 34", "NK 06 11", 000, {0,0,0,0}, false, "[Explorers\421 Formantine, Inverurie, Ellon & Oldmeldrum]"}
{"NH 73 62", "NJ 00 40", 000, {0,0,0,0}, false, "[Explorers\422 Nairn & Cawdor]"}
{"NH 96 73", "NJ 26 51", 000, {0,0,0,0}, false, "[Explorers\423 Elgin & Forres]"}
{"NJ 22 70", "NJ 45 33", 000, {0,0,0,0}, false, "[Explorers\424 Keith, Buckie & Charlestown of Aberlour]"}
{"NJ 45 70", "NJ 68 33", 000, {0,0,0,0}, false, "[Explorers\425 Huntly & Cullen, Portknockie & Portsoy]"}
{"NJ 68 68", "NJ 91 31", 000, {0,0,0,0}, false, "[Explorers\426 Banff, Macduff & Turriff]"}
{"NJ 91 68", "NK 15 31", 000, {0,0,0,0}, false, "[Explorers\427 Peterhead & Fraserburgh]"}
{"NG 67 66", "NG 91 27", 000, {0,0,0,0}, false, "[Explorers\428 Kyle of Lochalsh, Plockton & Applecross]"}
{"NG 90 57", "NH 13 30", 000, {0,0,0,0}, false, "[Explorers\429 Glen Carron & West Monar]"}
{"NH 10 57", "NH 32 30", 000, {0,0,0,0}, false, "[Explorers\430 Loch Monar, Glen Cannich & Glen Strathfarrar]"}
{"NH 32 56", "NH 54 28", 000, {0,0,0,0}, false, "[Explorers\431 Glen Urquhart & Strathglass]"}
{"NH 52 68", "NH 82 46", 000, {0,0,0,0}, false, "[Explorers\432 Black Isle]"}
{"NG 71 77", "NH 03 54", 000, {0,0,0,0}, false, "[Explorers\433 Torridon - Beinn Eighe & Liathach]"}
{"NB 72 00", "NG 94 70", 000, {0,0,0,0}, false, "[Explorers\434 Gairloch & Loch Ewe]"}
{"NB 93 00", "NH 18 57", 000, {0,0,0,0}, false, "[Explorers\435 An Teallach & Slioch, Kinlochewe]"}
{"NH 12 96", "NH 35 57", 000, {0,0,0,0}, false, "[Explorers\436 Beinn Dearg & Loch Fannich]"}
{"NH 32 93", "NH 56 55", 000, {0,0,0,0}, false, "[Explorers\437 Ben Wyvis & Strathpeffer]"}
{"NH 55 92", "NH 96 67", 000, {0,0,0,0}, false, "[Explorers\438 Dornoch & Tain]"}
{"NB 90 23", "NH 30 94", 000, {0,0,0,0}, false, "[Explorers\439 Coigach & Summer Isles]"}
{"NC 30 35", "NH 50 90", 000, {0,0,0,0}, false, "[Explorers\440 Glen Cassley & Glen Oykel]"}
{"NC 49 10", "NH 95 89", 000, {0,0,0,0}, false, "[Explorers\441 Lairg, Bonar Bridge & Golspie]"}
{"NC 00 36", "NC 37 13", 000, {0,0,0,0}, false, "[Explorers\442 Assynt & Lochinver]"}
{"NC 50 35", "NC 72 08", 000, {0,0,0,0}, false, "[Explorers\443 Ben Klibreck & Ben Armine Forest]"}
{"NC 71 30", "ND 18 09", 000, {0,0,0,0}, false, "[Explorers\444 Helmsdale & Strath of Kildonan]"}
{"NC 11 53", "NC 39 30", 000, {0,0,0,0}, false, "[Explorers\445 Foinaven, Arkle, Kylesku & Scourie]"}
{"NC 16 76", "NC 46 52", 000, {0,0,0,0}, false, "[Explorers\446 Durness & Cape Wrath]"}
{"NC 39 70", "NC 62 32", 000, {0,0,0,0}, false, "[Explorers\447 Ben Hope, Ben Loyal & Kyle of Tongue]"}
{"NC 58 70", "NC 81 30", 000, {0,0,0,0}, false, "[Explorers\448 Strath Naver/Srath Nabhair & Loch Loyal]"}
{"NC 80 70", "ND 03 30", 000, {0,0,0,0}, false, "[Explorers\449 Strath Halladale & Strathy Point]"}
{"ND 00 56", "ND 39 29", 000, {0,0,0,0}, false, "[Explorers\450 Wick & the Flow Country]"}
{"ND 00 80", "ND 41 52", 000, {0,0,0,0}, false, "[Explorers\451 Thurso & John o' Groats]"}
{"NF 53 16", "NL 82 79", 000, {0,0,0,0}, false, "[Explorers\452 Barra & Vatersay / Barraigh agus Bhatarsaigh]"}
{"NF 70 59", "NF 93 07", 000, {0,0,0,0}, false, "[Explorers\453 Benbecula & South Uist / Beinn na Faoghla agus Uib]"}
{"NF 57 85", "NG 03 57", 000, {0,0,0,0}, false, "[Explorers\454 North Uist & Berneray /  Uibhist a Tuath agus Bear]"}
{"NA 86 04", "NG 25 81", 000, {0,0,0,0}, false, "[Explorers\455 South Harris / Ceann a Deas na Hearadh]"}
{"NA 87 20", "NB 25 00", 000, {0,0,0,0}, false, "[Explorers\456 North Harris & Loch Shiphoirt / Ceann a Tuath na Hearadh]"}
{"NB 22 26", "NG 45 96", 000, {0,0,0,0}, false, "[Explorers\457 South East Lewis / Taobh an Eardheas Leodhais]"}
{"NA 97 45", "NB 24 19", 000, {0,0,0,0}, false, "[Explorers\458 West Lewis / Taobh Siar Leodhais]"}
{"NB 16 44", "NB 58 23", 000, {0,0,0,0}, false, "[Explorers\459 Central Lewis & Stornoway / Meadhan Leodhais agus Steornabhagh]"}
{"HV 05 34", "NG 83 96", 000, {0,0,0,0}, false, "[Explorers\460 North Lewis / Ceann a Tuath Leodhais]"}
{"HY 33 23", "ND 62 76", 000, {0,0,0,0}, false, "[Explorers\461 Orkney - East Mainland]"}
{"HY 17 10", "ND 44 87", 000, {0,0,0,0}, false, "[Explorers\462 Orkney - Hoy, Flotta & South Walls]"}
{"HY 21 32", "HY 44 02", 000, {0,0,0,0}, false, "[Explorers\463 Orkney - West Mainland - Stromness & Tingwall]"}
{"HY 35 56", "HY 58 25", 000, {0,0,0,0}, false, "[Explorers\464 Orkney - Westray. Rousay & Eday]"}
{"HY 50 58", "HY 80 15", 000, {0,0,0,0}, false, "[Explorers\465 Orkney - Sanday, Stronsay, Eday & North Ronaldsay]"}
{"HU 19 46", "HZ 56 68", 000, {0,0,0,0}, false, "[Explorers\466 Shetland - Mainland South]"}
{"HT 93 63", "HU 53 36", 000, {0,0,0,0}, false, "[Explorers\467 Central Mainland - Lerwick, Papa Stour & Foula]"}
{"HU 40 80", "HU 71 58", 000, {0,0,0,0}, false, "[Explorers\468 Shetland - Mainland North East & Whalsay]"}
{"HU 09 99", "HU 43 60", 000, {0,0,0,0}, false, "[Explorers\469 Shetland - Mainland North]"}
*/

#define	N_EXPLORER_MAPS	404

struct map {
	char *	strCoord1;
	char *	strCoord2;
	long	lngExplorer;
	RECT	rRectangle;
	bool	bExplorer;
	char *	strLocation;
} maps[N_EXPLORER_MAPS] = {
	{"SD9310" , "SK3380",   1, {0,0,0,0}, false, "Peak District"},
	{"SD5890" , "SE1050",   2, {0,0,0,0}, false, "Yorkshire Dales"},
	{"NY0150" , "NY3109",   4, {0,0,0,0}, false, "Lake District"},
	{"NY3050" , "NY6009",   5, {0,0,0,0}, false, "Lake District"},
	{"NY0111" , "SD3161",   6, {0,0,0,0}, false, "Lake District"},
	{"NY3011" , "SD6070",   7, {0,0,0,0}, false, "Lake District"},
	{"SS5052" , "ST1027",   9, {0,0,0,0}, false, "Exmoor"},
	{"SN6736" , "SO1105",  12, {0,0,0,0}, false, "Brecon Beacons"},
	{"SO1045" , "SO4005",  13, {0,0,0,0}, false, "Brecon Beacons"},
	{"SO4030" , "ST7890",  14, {0,0,0,0}, false, "Forest Of Dean"},
	{"SY4596" , "SZ0868",  15, {0,0,0,0}, false, "South Dorset"},
	{"NT6030" , "NU0200",  16, {0,0,0,0}, false, "Cheviot Hills"},
	{"SH4785" , "SH9045",  17, {0,0,0,0}, false, "Snowdon"},
	{"SH5455" , "SH9915",  18, {0,0,0,0}, false, "Harlech Porthmadog & Bala"},
	{"NY6030" , "SD9090",  19, {0,0,0,0}, false, "Howgill Fells & Upper Eden Valley"},
	{"SX4866" , "SX9734",  20, {0,0,0,0}, false, "South Devon"},
	{"SD7852" , "SE1010",  21, {0,0,0,0}, false, "South Pennines"},
	{"SU0620" , "SZ5089",  22, {0,0,0,0}, false, "New Forest"},
	{"SH5536" , "SN9890",  23, {0,0,0,0}, false, "Cadair Idris"},
	{"SJ9280" , "SK3250",  24, {0,0,0,0}, false, "Peak District"},
	{"NZ4222" , "SE7478",  26, {0,0,0,0}, false, "North York Moors"},
	{"NZ7021" , "TA0579",  27, {0,0,0,0}, false, "North York Moors"},
	{"SX4896" , "SX8155",  28, {0,0,0,0}, false, "Dartmoor"},
	{"SU2805" , "SZ6775",  29, {0,0,0,0}, false, "Isle Of Wight"},
	{"NY8414" , "SE1470",  30, {0,0,0,0}, false, "Yorkshire Dales"},
	{"NY6050" , "NZ1010",  31, {0,0,0,0}, false, "North Pennines"},
	{"SM6552" , "SN2020",  35, {0,0,0,0}, false, "Pembrokeshire"},
	{"SM4623" , "SS2092",  36, {0,0,0,0}, false, "Pembrokeshire"},
	{"TG2330" , "TM5688",  40, {0,0,0,0}, false, "The Broads"},
	{"SD4770" , "SD8938",  41, {0,0,0,0}, false, "Forest Of Bowland"},
	{"NT5002" , "NZ1080",  42, {0,0,0,0}, false, "Kielder"},
	{"NY5880" , "NZ0250",  43, {0,0,0,0}, false, "Hadrians Wall"},
	{"SP0042" , "SP3000",  45, {0,0,0,0}, false, "The Cotswolds"},
	{"SV8020" , "SW0000", 101, {0,0,0,0}, false, "Isles Of Scilly"},
	{"SW3144" , "SW6120", 102, {0,0,0,0}, false, "Land's End"},
	{"SW6135" , "SW8310", 103, {0,0,0,0}, false, "The Lizard"},
	{"SW6164" , "SW8334", 104, {0,0,0,0}, false, "Redruth & St Agnes"},
	{"SW7552" , "SX0530", 105, {0,0,0,0}, false, "Falmouth & Mevagissey"},
	{"SW8082" , "SX0352", 106, {0,0,0,0}, false, "Newquay & Padstow"},
	{"SX0068" , "SX3048", 107, {0,0,0,0}, false, "St Austell & Liskeard"},
	{"SX3078" , "SX5248", 108, {0,0,0,0}, false, "Lower Tamar Valley & Plymouth"},
	{"SX0386" , "SX3063", 109, {0,0,0,0}, false, "Bodmin Moor"},
	{"SX7890" , "SY0060", 110, {0,0,0,0}, false, "Torquay & Dawlish"},
	{"SS0108" , "SX3280", 111, {0,0,0,0}, false, "Bude Boscastle & Tintagel"},
	{"SS2908" , "SX5178", 112, {0,0,0,0}, false, "Launceston & Holsworthy"},
	{"SS5010" , "SX8090", 113, {0,0,0,0}, false, "Okehampton"},
	{"SS8031" , "SY0090", 114, {0,0,0,0}, false, "Exeter & The Exe Valley"},
	{"SS9910" , "SY2179", 115, {0,0,0,0}, false, "Exmouth & Sidmouth"},
	{"ST2110" , "SY4987", 116, {0,0,0,0}, false, "Lyme Regis & Bridport"},
	{"ST4710" , "SY9090", 117, {0,0,0,0}, false, "Cerne Abbas & Bere Regis"},
	{"ST8331" , "SZ0690", 118, {0,0,0,0}, false, "Shaftesbury & Cranborne Chase"},
	{"SU4826" , "SZ7097", 119, {0,0,0,0}, false, "Meon Valley Portsmouth Gosport & Fareham"},
	{"SU6823" , "SZ9092", 120, {0,0,0,0}, false, "Chichester"},
	{"SU9020" , "TV1797", 121, {0,0,0,0}, false, "Arundel & Pulborough"},
	{"TQ1721" , "TV4799", 122, {0,0,0,0}, false, "South Downs Way  Steyning To Newhaven"},
	{"TQ4322" , "TV6595", 123, {0,0,0,0}, false, "South Downs Way  Newhaven To Eastbourne"},
	{"TQ6124" , "TQ9102", 124, {0,0,0,0}, false, "Hastings & Bexhill"},
	{"TQ8035" , "TR1012", 125, {0,0,0,0}, false, "Romney Marsh Rye & Winchelsea"},
	{"SS1929" , "SS5008", 126, {0,0,0,0}, false, "Clovelly & Hartland"},
	{"SS5030" , "SS8010", 127, {0,0,0,0}, false, "South Molton & Chulmleigh"},
	{"ST0031" , "ST4005", 128, {0,0,0,0}, false, "Taunton & Blackdown Hills"},
	{"ST4031" , "ST8310", 129, {0,0,0,0}, false, "Yeovil & Sherborne"},
	{"ST9760" , "SU2020", 130, {0,0,0,0}, false, "Salisbury & Stonehenge"},
	{"SU2060" , "SU4020", 131, {0,0,0,0}, false, "Romsey Andover & Test Valley"},
	{"SU4040" , "SU7020", 132, {0,0,0,0}, false, "Winchester"},
	{"SU7040" , "TQ0020", 133, {0,0,0,0}, false, "Haslemere & Petersfield"},
	{"TQ0040" , "TQ3020", 134, {0,0,0,0}, false, "Crawley & Horsham"},
	{"TQ3041" , "TQ6020", 135, {0,0,0,0}, false, "Ashdown Forest"},
	{"TQ5650" , "TQ8022", 136, {0,0,0,0}, false, "The Weald Royal Tunbridge Wells"},
	{"TQ8055" , "TR1035", 137, {0,0,0,0}, false, "Ashford"},
	{"TR0951" , "TR3928", 138, {0,0,0,0}, false, "Dover Folkestone & Hythe"},
	{"SS1150" , "SS6020", 139, {0,0,0,0}, false, "Bideford Ilfracombe & Barnstaple"},
	{"ST1050" , "ST4030", 140, {0,0,0,0}, false, "Quantock Hills & Bridgwater"},
	{"ST4060" , "ST6030", 141, {0,0,0,0}, false, "Cheddar Gorge & Mendip Hills West"},
	{"ST6060" , "ST8030", 142, {0,0,0,0}, false, "Shepton Mallet & Mendip Hills East"},
	{"ST8060" , "SU0030", 143, {0,0,0,0}, false, "Warminster & Trowbridge"},
	{"SU4060" , "SU8037", 144, {0,0,0,0}, false, "Basingstoke Alton & Whitchurch"},
	{"SU8060" , "TQ1040", 145, {0,0,0,0}, false, "Guildford & Farnham"},
	{"TQ1060" , "TQ4040", 146, {0,0,0,0}, false, "Dorking Box Hill & Reigate"},
	{"TQ4065" , "TQ6238", 147, {0,0,0,0}, false, "Sevenoaks & Tonbridge"},
	{"TQ6070" , "TQ9050", 148, {0,0,0,0}, false, "Maidstone & Medway Towns"},
	{"TQ8877" , "TR1050", 149, {0,0,0,0}, false, "Sittingbourne & Faversham"},
	{"TR1073" , "TR4151", 150, {0,0,0,0}, false, "Canterbury & The Isle Of Thanet"},
	{"SS7886" , "ST2464", 151, {0,0,0,0}, false, "Cardiff & Bridgend"},
	{"SO2006" , "ST4273", 152, {0,0,0,0}, false, "Newport & Pontypool"},
	{"ST2073" , "ST4243", 153, {0,0,0,0}, false, "Westonsupermare & Bleadon Hill"},
	{"ST4090" , "ST6060", 154, {0,0,0,0}, false, "Bristol West & Portishead"},
	{"ST5381" , "ST8060", 155, {0,0,0,0}, false, "Bristol & Bath"},
	{"ST8083" , "SU0053", 156, {0,0,0,0}, false, "Chippenham & Bradford-on-avon"},
	{"SU0080" , "SU3060", 157, {0,0,0,0}, false, "Marlborough & Savernake Forest"},
	{"SU2880" , "SU5860", 158, {0,0,0,0}, false, "Newbury & Hungerford"},
	{"SU5580" , "SU8560", 159, {0,0,0,0}, false, "Reading"},
	{"SU8080" , "TQ1060", 160, {0,0,0,0}, false, "Windsor Weybridge & Bracknell"},
	{"TQ1080" , "TQ4060", 161, {0,0,0,0}, false, "London South"},
	{"TQ3685" , "TQ6665", 162, {0,0,0,0}, false, "Greenwich & Gravesend"},
	{"TQ6384" , "TQ9162", 163, {0,0,0,0}, false, "Gravesend & Rochester"},
	{"SN3504" , "SS6584", 164, {0,0,0,0}, false, "Gower"},
	{"SN6011" , "SS8281", 165, {0,0,0,0}, false, "Swansea"},
	{"SN8109" , "ST2186", 166, {0,0,0,0}, false, "Rhondda & Merthyr Tydfil"},
	{"SO5303" , "ST8081", 167, {0,0,0,0}, false, "Thornbury Dursley & Yate"},
	{"SO7807" , "SU0080", 168, {0,0,0,0}, false, "Stroud Tetbury & Malmesbury"},
	{"SP0005" , "SU2075", 169, {0,0,0,0}, false, "Cirencester & Swindon"},
	{"SP1900" , "SU6280", 170, {0,0,0,0}, false, "Abingdon Wantage & Vale Of White Horse"},
	{"SP5901" , "SU8174", 171, {0,0,0,0}, false, "Chiltern Hills West"},
	{"SP8000" , "TQ1080", 172, {0,0,0,0}, false, "Chiltern Hills East"},
	{"TL1000" , "TQ3778", 173, {0,0,0,0}, false, "London North"},
	{"TL3015" , "TQ5585", 174, {0,0,0,0}, false, "Epping Forest & Lee Valley"},
	{"TL5500" , "TQ9380", 175, {0,0,0,0}, false, "Southend-on-sea & Basildon"},
	{"TL8411" , "TR1880", 176, {0,0,0,0}, false, "Blackwater Estuary"},
	{"SN1624" , "SN4604", 177, {0,0,0,0}, false, "Carmarthen & Kidwelly"},
	{"SN3718" , "SS6798", 178, {0,0,0,0}, false, "Llanelli & Ammanford"},
	{"SO7830" , "SP0003", 179, {0,0,0,0}, false, "Gloucester Cheltenham & Stroud"},
	{"SP3020" , "SP7200", 180, {0,0,0,0}, false, "Oxford"},
	{"SP7220" , "TL0200", 181, {0,0,0,0}, false, "Chiltern Hills North"},
	{"TL0220" , "TL3000", 182, {0,0,0,0}, false, "St Albans & Hatfield"},
	{"TL5020" , "TL9000", 183, {0,0,0,0}, false, "Chelmsford & The Rodings"},
	{"TL9034" , "TM3210", 184, {0,0,0,0}, false, "Colchester"},
	{"SN2045" , "SN4723", 185, {0,0,0,0}, false, "Newcastle Emlyn"},
	{"SN4645" , "SN6818", 186, {0,0,0,0}, false, "Llandeilo & Brechfa Forest"},
	{"SN6768" , "SN9032", 187, {0,0,0,0}, false, "Llandovery"},
	{"SN9053" , "SO2033", 188, {0,0,0,0}, false, "Builth Wells"},
	{"SO4042" , "SO7020", 189, {0,0,0,0}, false, "Hereford & Ross On Wye"},
	{"SO7050" , "SP0030", 190, {0,0,0,0}, false, "Malvern Hills & Bredon Hill"},
	{"SP2843" , "SP6420", 191, {0,0,0,0}, false, "Banbury Bicester & Chipping Norton"},
	{"SP6043" , "TL0020", 192, {0,0,0,0}, false, "Buckingham & Milton Keynes"},
	{"TL0040" , "TL3020", 193, {0,0,0,0}, false, "Luton & Stevenage"},
	{"TL3040" , "TL5010", 194, {0,0,0,0}, false, "Hertford & Bishop's Stortford"},
	{"TL4740" , "TL9020", 195, {0,0,0,0}, false, "Braintree & Saffron Walden"},
	{"TL8050" , "TM1030", 196, {0,0,0,0}, false, "Sudbury Hadleigh & Dedham Vale"},
	{"TM1051" , "TM4026", 197, {0,0,0,0}, false, "Ipswich Felixstowe & Harwich"},
	{"SN1368" , "SN5240", 198, {0,0,0,0}, false, "Cardigan & New Quay"},
	{"SN4870" , "SN7043", 199, {0,0,0,0}, false, "Lampeter"},
	{"SN8073" , "SO2053", 200, {0,0,0,0}, false, "Llandrindod Wells & Elan Valley"},
	{"SO2083" , "SO4040", 201, {0,0,0,0}, false, "Knighton & Presteigne"},
	{"SO4060" , "SO7040", 202, {0,0,0,0}, false, "Leominster & Bromyard"},
	{"SO4080" , "SO7060", 203, {0,0,0,0}, false, "Ludlow"},
	{"SO7070" , "SP0050", 204, {0,0,0,0}, false, "Worcester & Droitwich Spa"},
	{"SP0060" , "SP3040", 205, {0,0,0,0}, false, "Stratford Upon Avon & Evesham"},
	{"SP3060" , "SP6040", 206, {0,0,0,0}, false, "Edge Hill & Fenny Compton"},
	{"SP5563" , "SP9340", 207, {0,0,0,0}, false, "Newport Pagnell & Northampton South"},
	{"SP9360" , "TL3340", 208, {0,0,0,0}, false, "Bedford & St Neots"},
	{"TL3362" , "TL6040", 209, {0,0,0,0}, false, "Cambridge"},
	{"TL6070" , "TL8040", 210, {0,0,0,0}, false, "Newmarket & Haverhill"},
	{"TL8070" , "TM2350", 211, {0,0,0,0}, false, "Bury St Edmunds & Stowmarket"},
	{"TM2168" , "TM4846", 212, {0,0,0,0}, false, "Woodbridge & Saxmundham"},
	{"SN5190" , "SN8168", 213, {0,0,0,0}, false, "Aberystwyth & Cwm Rheidol"},
	{"SN8093" , "SO2073", 214, {0,0,0,0}, false, "Llanidloes & Newtown"},
	{"SH7510" , "SO1890", 215, {0,0,0,0}, false, "Newtown & Machynlleth"},
	{"SJ1810" , "SO4083", 216, {0,0,0,0}, false, "Welshpool & Montgomery"},
	{"SJ3500" , "SO6580", 217, {0,0,0,0}, false, "The Long Mynd & Wenlock Edge"},
	{"SJ6500" , "SO8570", 218, {0,0,0,0}, false, "Kidderminster & Wyre Forest"},
	{"SJ8007" , "SP0070", 219, {0,0,0,0}, false, "Wolverhampton & Dudley"},
	{"SK0003" , "SP2060", 220, {0,0,0,0}, false, "Birmingham"},
	{"SP2090" , "SP4060", 221, {0,0,0,0}, false, "Coventry & Warwick"},
	{"SP4090" , "SP6060", 222, {0,0,0,0}, false, "Rugby & Daventry"},
	{"SP6094" , "SP8356", 223, {0,0,0,0}, false, "Northampton & Market Harborough"},
	{"SP8095" , "TL0360", 224, {0,0,0,0}, false, "Corby Kettering & Wellingborough"},
	{"TL0380" , "TL4660", 225, {0,0,0,0}, false, "Huntingdon & St Ives"},
	{"TL4682" , "TL7662", 226, {0,0,0,0}, false, "Ely & Newmarket"},
	{"TF0303" , "TL4380", 227, {0,0,0,0}, false, "Peterborough"},
	{"TF3903" , "TL7580", 228, {0,0,0,0}, false, "March & Ely"},
	{"TF7500" , "TL9570", 229, {0,0,0,0}, false, "Thetford Forest In The Brecks"},
	{"TL9090" , "TM3067", 230, {0,0,0,0}, false, "Diss & Harleston"},
	{"TM2790" , "TM5468", 231, {0,0,0,0}, false, "Southwold & Bungay"},
	{"SK1211" , "SP4189", 232, {0,0,0,0}, false, "Nuneaton & Tamworth"},
	{"SK4110" , "SP8287", 233, {0,0,0,0}, false, "Leicester & Hinckley"},
	{"SK8015" , "TL1095", 234, {0,0,0,0}, false, "Rutland Water"},
	{"TF1020" , "TL5097", 235, {0,0,0,0}, false, "Wisbech & Peterborough North"},
	{"TF5023" , "TF9000", 236, {0,0,0,0}, false, "King's Lynn Downham Market & Swaffham"},
	{"TF8913" , "TM2990", 237, {0,0,0,0}, false, "Norwich"},
	{"TF8030" , "TG2310", 238, {0,0,0,0}, false, "East Dereham & Aylsham"},
	{"SH9428" , "SJ2106", 239, {0,0,0,0}, false, "Lake Vyrnwy & Llanfyllin"},
	{"SJ2040" , "SJ4010", 240, {0,0,0,0}, false, "Oswestry"},
	{"SJ4040" , "SJ6000", 241, {0,0,0,0}, false, "Shrewsbury"},
	{"SJ6020" , "SJ9000", 242, {0,0,0,0}, false, "Telford Ironbridge & The Wrekin"},
	{"SJ6040" , "SJ9020", 243, {0,0,0,0}, false, "Market Drayton"},
	{"SJ9030" , "SK1203", 244, {0,0,0,0}, false, "Cannock Chase & Chasewater"},
	{"SK1230" , "SK5207", 245, {0,0,0,0}, false, "The National Forest Burton Upon Trent"},
	{"SK5030" , "SK8010", 246, {0,0,0,0}, false, "Loughborough"},
	{"SK8045" , "TF0015", 247, {0,0,0,0}, false, "Grantham"},
	{"TF0045" , "TF2015", 248, {0,0,0,0}, false, "Bourne & Heckington"},
	{"TF2040" , "TF6320", 249, {0,0,0,0}, false, "Spalding & Holbeach"},
	{"TF6047" , "TF8217", 250, {0,0,0,0}, false, "Norfolk Coast West"},
	{"TF8148" , "TG1128", 251, {0,0,0,0}, false, "Norfolk Coast Central"},
	{"TG1045" , "TG4023", 252, {0,0,0,0}, false, "Norfolk Coast East"},
	{"SH1046" , "SH4020", 253, {0,0,0,0}, false, "Lleyn Peninsula West"},
	{"SH3561" , "SH5733", 254, {0,0,0,0}, false, "Lleyn Peninsula East"},
	{"SH9847" , "SJ2825", 255, {0,0,0,0}, false, "Llangollen & Berwyn"},
	{"SJ0759" , "SJ3737", 256, {0,0,0,0}, false, "Wrexham & Llangollen"},
	{"SJ3760" , "SJ8040", 257, {0,0,0,0}, false, "Crewe & Nantwich"},
	{"SJ8060" , "SK0030", 258, {0,0,0,0}, false, "Stoke-on-trent & Newcastle-under-lyme"},
	{"SK0050" , "SK4330", 259, {0,0,0,0}, false, "Derby"},
	{"SK4050" , "SK8330", 260, {0,0,0,0}, false, "Nottingham"},
	{"TF1060" , "TF6140", 261, {0,0,0,0}, false, "Boston"},
	{"SH2096" , "SH4266", 262, {0,0,0,0}, false, "Anglesey West"},
	{"SH3595" , "SH6860", 263, {0,0,0,0}, false, "Anglesey East"},
	{"SH9085" , "SJ1345", 264, {0,0,0,0}, false, "Vale Of Clwyd"},
	{"SJ0590" , "SJ2555", 265, {0,0,0,0}, false, "Clwydian Range"},
	{"SJ1998" , "SJ4859", 266, {0,0,0,0}, false, "Wirral & Chester"},
	{"SJ4882" , "SJ7560", 267, {0,0,0,0}, false, "Northwich & Delamere Forest"},
	{"SJ7490" , "SJ9754", 268, {0,0,0,0}, false, "Wilmslow Macclesfield & Congleton"},
	{"SK3280" , "SK5250", 269, {0,0,0,0}, false, "Chesterfield & Alfreton"},
	{"SK5080" , "SK7050", 270, {0,0,0,0}, false, "Sherwood Forest"},
	{"SK7088" , "SK9045", 271, {0,0,0,0}, false, "Newark-on-trent"},
	{"SK9088" , "TF1045", 272, {0,0,0,0}, false, "Lincoln"},
	{"TF1080" , "TF4060", 273, {0,0,0,0}, false, "Lincolnshire Wolds"},
	{"TF3883" , "TF6053", 274, {0,0,0,0}, false, "Skegness Alford & Spilsby"},
	{"SD2901" , "SJ5979", 275, {0,0,0,0}, false, "Liverpool"},
	{"SD5512" , "SJ7582", 276, {0,0,0,0}, false, "Bolton Wigan & Warrington"},
	{"SD7515" , "SJ9788", 277, {0,0,0,0}, false, "Manchester & Salford"},
	{"SE2720" , "SK5080", 278, {0,0,0,0}, false, "Sheffield & Barnsley"},
	{"SE4720" , "SK7080", 279, {0,0,0,0}, false, "Doncaster"},
	{"SE7015" , "SK9088", 280, {0,0,0,0}, false, "Isle Of Axholme"},
	{"SE8726" , "TF1088", 281, {0,0,0,0}, false, "Ancholme Valley"},
	{"TA1002" , "TF3780", 282, {0,0,0,0}, false, "Lincolnshire Wolds"},
	{"TA3209" , "TF5478", 283, {0,0,0,0}, false, "Louth & Mablethorpe"},
	{"TA1026" , "TF3296", 284, {0,0,0,0}, false, "Grimsby Cleethorpes & Immingham"},
	{"SD2123" , "SJ6497", 285, {0,0,0,0}, false, "Southport & Chorley"},
	{"SD2741" , "SD6021", 286, {0,0,0,0}, false, "Blackpool & Preston"},
	{"SD6040" , "SD8010", 287, {0,0,0,0}, false, "West Pennine Moors"},
	{"SE0842" , "SE2804", 288, {0,0,0,0}, false, "Bradford & Huddersfield"},
	{"SE2560" , "SE4820", 289, {0,0,0,0}, false, "Leeds"},
	{"SE4860" , "SE7020", 290, {0,0,0,0}, false, "York"},
	{"SE6742" , "SE8915", 291, {0,0,0,0}, false, "Goole & Gilberdyke"},
	{"TA1945" , "TA4309", 292, {0,0,0,0}, false, "Withernsea & Spurn Head"},
	{"SE8944" , "TA1924", 293, {0,0,0,0}, false, "Kingston Upon Hull & Beverley"},
	{"SE7060" , "TA0040", 294, {0,0,0,0}, false, "Market Weighton & Yorkshire Wolds Central"},
	{"TA0065" , "TA2344", 295, {0,0,0,0}, false, "Bridlington Driffield & Hornsea"},
	{"SD3070" , "SD5040", 296, {0,0,0,0}, false, "Lancaster Morecambe & Fleetwood"},
	{"SE0560" , "SE3540", 297, {0,0,0,0}, false, "Lower Wharfedale & Washburn Valley"},
	{"SE0580" , "SE3258", 298, {0,0,0,0}, false, "Nidderdale"},
	{"SE2780" , "SE5760", 299, {0,0,0,0}, false, "Ripon & Boroughbridge"},
	{"SE5780" , "TA0060", 300, {0,0,0,0}, false, "Howardian Hills & Malton"},
	{"TA0095" , "TA2665", 301, {0,0,0,0}, false, "Scarborough Bridlington & Flamborough Head"},
	{"NZ1400" , "SE4480", 302, {0,0,0,0}, false, "Northallerton & Thirsk"},
	{"NX9234" , "NY1403", 303, {0,0,0,0}, false, "Whitehaven & Workington"},
	{"NZ0821" , "NZ4400", 304, {0,0,0,0}, false, "Darlington & Richmond"},
	{"NZ1038" , "NZ4020", 305, {0,0,0,0}, false, "Bishop Auckland"},
	{"NZ4038" , "NZ7016", 306, {0,0,0,0}, false, "Middlesbrough & Hartlepool"},
	{"NY9060" , "NZ2038", 307, {0,0,0,0}, false, "Consett & Derwent Reservoir"},
	{"NZ1860" , "NZ4838", 308, {0,0,0,0}, false, "Durham & Sunderland"},
	{"NW9575" , "NX2830", 309, {0,0,0,0}, false, "Stranraer & The Rhins"},
	{"NX1678" , "NX3548", 310, {0,0,0,0}, false, "Glenluce & Kirkcowan"},
	{"NX2863" , "NX5033", 311, {0,0,0,0}, false, "Wigtown Whithorn & Machars"},
	{"NX5063" , "NX8142", 312, {0,0,0,0}, false, "Kirkcudbright & Castle Douglas"},
	{"NX8078" , "NY0048", 313, {0,0,0,0}, false, "Dumfries & Dalbeattie"},
	{"NY0067" , "NY3047", 314, {0,0,0,0}, false, "Solway Firth"},
	{"NY3070" , "NY6050", 315, {0,0,0,0}, false, "Carlisle"},
	{"NZ0080" , "NZ4260", 316, {0,0,0,0}, false, "Newcastle Upon Tyne"},
	{"NS0102" , "NX3475", 317, {0,0,0,0}, false, "Ballantrae Bar & Barrhill"},
	{"NS3200" , "NX6278", 318, {0,0,0,0}, false, "Galloway Forest Park North"},
	{"NX3582" , "NX6562", 319, {0,0,0,0}, false, "Galloway Forest Park South"},
	{"NX6192" , "NX8162", 320, {0,0,0,0}, false, "Castle Douglas Loch Ken & New Galloway"},
	{"NS7800" , "NY0073", 321, {0,0,0,0}, false, "Nithsdale & Dumfries"},
	{"NT0004" , "NY2364", 322, {0,0,0,0}, false, "Annandale"},
	{"NT1804" , "NY4164", 323, {0,0,0,0}, false, "Eskdale & Castle O'er Forest"},
	{"NT3800" , "NY5870", 324, {0,0,0,0}, false, "Liddesdale & Kershope Forest"},
	{"NU0602" , "NZ3480", 325, {0,0,0,0}, false, "Morpeth & Blyth"},
	{"NS1734" , "NX4896", 326, {0,0,0,0}, false, "Ayr & Troon"},
	{"NS4030" , "NS6000", 327, {0,0,0,0}, false, "Cumnock & Dalmellington"},
	{"NS5630" , "NX7990", 328, {0,0,0,0}, false, "Sanquhar & New Cumnock"},
	{"NS7824" , "NY0097", 329, {0,0,0,0}, false, "Lowther Hills Sanquhar & Leadhills"},
	{"NT0024" , "NT3004", 330, {0,0,0,0}, false, "Moffat & St Mary's Loch"},
	{"NT3020" , "NT6000", 331, {0,0,0,0}, false, "Teviotdale South"},
	{"NU0224" , "NU3202", 332, {0,0,0,0}, false, "Alnwick & Amble"},
	{"NS2659" , "NS4829", 333, {0,0,0,0}, false, "Kilmarnock & Irvine"},
	{"NS4856" , "NS7029", 334, {0,0,0,0}, false, "East Kilbride Galston & Darvel"},
	{"NS6946" , "NS9924", 335, {0,0,0,0}, false, "Lanark & Tinto Hills"},
	{"NS9849" , "NT2022", 336, {0,0,0,0}, false, "Biggar & Broughton"},
	{"NT2050" , "NT4020", 337, {0,0,0,0}, false, "Peebles & Innerleithen"},
	{"NT4050" , "NT6220", 338, {0,0,0,0}, false, "Galashiels Selkirk & Melrose"},
	{"NT5750" , "NU0030", 339, {0,0,0,0}, false, "Kelso Coldstream & Lower Tweed Valley"},
	{"NT9950" , "NU2624", 340, {0,0,0,0}, false, "Holy Island & Bamburgh"},
	{"NS1379" , "NS4140", 341, {0,0,0,0}, false, "Greenock Largs & Millport"},
	{"NS4175" , "NS7155", 342, {0,0,0,0}, false, "Glasgow"},
	{"NS7066" , "NT0046", 343, {0,0,0,0}, false, "Motherwell & Coatbridge"},
	{"NT0068" , "NT3048", 344, {0,0,0,0}, false, "Pentland Hills"},
	{"NT2970" , "NT7250", 345, {0,0,0,0}, false, "Lammermuir Hills"},
	{"NT7272" , "NU0250", 346, {0,0,0,0}, false, "Berwick upon Tweed"},
	{"NS2095" , "NS5173", 347, {0,0,0,0}, false, "Loch Lomond South"},
	{"NS5095" , "NS8075", 348, {0,0,0,0}, false, "Campsie Fells"},
	{"NS7088" , "NT0865", 349, {0,0,0,0}, false, "Falkirk Cumbernauld & Livingston"},
	{"NT0786" , "NT3766", 350, {0,0,0,0}, false, "Edinburgh"},
	{"NT3288" , "NT7968", 351, {0,0,0,0}, false, "Dunbar & North Berwick"},
	{"NR2667" , "NR4839", 352, {0,0,0,0}, false, "Islay South"},
	{"NR1581" , "NR4550", 353, {0,0,0,0}, false, "Islay North"},
	{"NM3002" , "NR5275", 354, {0,0,0,0}, false, "Colonsay & Oransay"},
	{"NM4207" , "NR7461", 355, {0,0,0,0}, false, "Jura & Scarba"},
	{"NR5845" , "NR8303", 356, {0,0,0,0}, false, "Kintyre South"},
	{"NR6273" , "NR9343", 357, {0,0,0,0}, false, "Kintyre North"},
	{"NM6607" , "NR9871", 358, {0,0,0,0}, false, "Lochgilphead & Knapdale North"},
	{"NM6338" , "NM9205", 359, {0,0,0,0}, false, "Oban Kerrera & Loch Melfort"},
	{"NM8529" , "NS1996", 360, {0,0,0,0}, false, "Loch Awe & Inveraray"},
	{"NR8653" , "NS0918", 361, {0,0,0,0}, false, "Isle Of Arran"},
	{"NR9099" , "NS1251", 362, {0,0,0,0}, false, "Cowal West & Isle Of Bute"},
	{"NN0513" , "NS2567", 363, {0,0,0,0}, false, "Cowal East"},
	{"NN1932" , "NS4293", 364, {0,0,0,0}, false, "Loch Lomond North"},
	{"NN4233" , "NS6493", 365, {0,0,0,0}, false, "The Trossachs"},
	{"NN6310" , "NT0087", 366, {0,0,0,0}, false, "Stirling & Ochil Hills West"},
	{"NN9302" , "NT3980", 367, {0,0,0,0}, false, "Dunfermline Kirkcaldy & Glenrothes South"},
	{"NN6231" , "NN9808", 368, {0,0,0,0}, false, "Crieff Comrie & Glen Artney"},
	{"NN9828" , "NT1898", 369, {0,0,0,0}, false, "Perth & Kinross"},
	{"NO1022" , "NO4500", 370, {0,0,0,0}, false, "Glenrothes North Falkland & Lomond Hills"},
	{"NO3330" , "NT6798", 371, {0,0,0,0}, false, "St Andrews & East Fife"},
	{"NL8967" , "NM3135", 372, {0,0,0,0}, false, "Coll & Tiree"},
	{"NM2337" , "NM5511", 373, {0,0,0,0}, false, "Iona Staffa & Ross Of Mull"},
	{"NM2360" , "NM6034", 374, {0,0,0,0}, false, "Isle Of Mull North & Tobermory"},
	{"NM4044" , "NM7619", 375, {0,0,0,0}, false, "Isle Of Mull East"},
	{"NM7559" , "NN0128", 376, {0,0,0,0}, false, "Oban & North Lorn"},
	{"NM9948" , "NN3925", 377, {0,0,0,0}, false, "Loch Etive & Glen Orchy"},
	{"NN3753" , "NN7830", 378, {0,0,0,0}, false, "Ben Lawers & Glen Lyon"},
	{"NN7650" , "NO1327", 379, {0,0,0,0}, false, "Dunkeld Aberfeldy & Glen Almond"},
	{"NO1347" , "NO5019", 380, {0,0,0,0}, false, "Dundee & Sidlaw Hills"},
	{"NO1361" , "NO4339", 381, {0,0,0,0}, false, "Blairgowrie Kirriemuir & Glamis"},
	{"NO5073" , "NO8330", 382, {0,0,0,0}, false, "Arbroath Montrose & Carnoustie"},
	{"NM5463" , "NM9539", 383, {0,0,0,0}, false, "Morvern & Lochaline"},
	{"NM9968" , "NN2945", 384, {0,0,0,0}, false, "Glen Coe & Glen Etive"},
	{"NN2873" , "NN6348", 385, {0,0,0,0}, false, "Rannoch Moor & Ben Alder"},
	{"NN6071" , "NO0048", 386, {0,0,0,0}, false, "Pitlochry & Loch Tummel"},
	{"NO0092" , "NO2050", 387, {0,0,0,0}, false, "Glen Shee & Braemar"},
	{"NO1797" , "NO4061", 388, {0,0,0,0}, false, "Lochnagar Glen Muick & Glen Clova"},
	{"NO4077" , "NO6247", 389, {0,0,0,0}, false, "Forfar Brechin & Edzell"},
	{"NM3983" , "NM8060", 390, {0,0,0,0}, false, "Ardnamurchan"},
	{"NM8081" , "NN1059", 391, {0,0,0,0}, false, "Ardgour & Strontian"},
	{"NN0583" , "NN3561", 392, {0,0,0,0}, false, "Ben Nevis & Fort William"},
	{"NN3490" , "NN6568", 393, {0,0,0,0}, false, "Ben Alder Loch Ericht & Loch Laggan"},
	{"NN6390" , "NO0067", 394, {0,0,0,0}, false, "Atholl"},
	{"NJ4002" , "NO6275", 395, {0,0,0,0}, false, "Glen Esk & Glen Tanar"},
	{"NO6292" , "NO9270", 396, {0,0,0,0}, false, "Stonehaven Inverbervie & Laurencekirk"},
	{"NG1407" , "NM5077", 397, {0,0,0,0}, false, "Rum Eigg Muck Canna & Sanday"},
	{"NG6002" , "NN0079", 398, {0,0,0,0}, false, "Loch Morar & Mallaig"},
	{"NG9804" , "NN2074", 399, {0,0,0,0}, false, "Loch Arkaig"},
	{"NH1711" , "NN3981", 400, {0,0,0,0}, false, "Loch Lochy & Glen Roy"},
	{"NH3710" , "NN5980", 401, {0,0,0,0}, false, "Loch Laggan & Creag Meagaidh"},
	{"NH5810" , "NN8588", 402, {0,0,0,0}, false, "Strathspey"},
	{"NH8423" , "NO0787", 403, {0,0,0,0}, false, "Cairn Gorm & Aviemore"},
	{"NJ0322" , "NO2688", 404, {0,0,0,0}, false, "Braemar Tomintoul Glen Avon"},
	{"NJ2619" , "NO6696", 405, {0,0,0,0}, false, "Aboyne Alford & Strathdon"},
	{"NJ6114" , "NO9891", 406, {0,0,0,0}, false, "Aberdeen & Banchory"},
	{"NG1268" , "NG3435", 407, {0,0,0,0}, false, "Skye Dunvegan"},
	{"NG3182" , "NG5350", 408, {0,0,0,0}, false, "Skye Trotternish & The Storr"},
	{"NG4764" , "NG6927", 409, {0,0,0,0}, false, "Raasay Rona & Scalpay"},
	{"NG2750" , "NG5730", 410, {0,0,0,0}, false, "Skye Portree & Bracadale"},
	{"NG3033" , "NG6310", 411, {0,0,0,0}, false, "Skye Cuillin Hills"},
	{"NG5529" , "NM8099", 412, {0,0,0,0}, false, "Skye Sleat"},
	{"NG6931" , "NM9797", 413, {0,0,0,0}, false, "Knoydart Loch Hourn & Loch Duich"},
	{"NG9333" , "NH1600", 414, {0,0,0,0}, false, "Glen Shiel & Kintail Forest"},
	{"NH1534" , "NH3703", 415, {0,0,0,0}, false, "Glen Affric & Glen Moriston"},
	{"NH3750" , "NH7708", 416, {0,0,0,0}, false, "Inverness Loch Ness & Culloden"},
	{"NH5732" , "NH8410", 417, {0,0,0,0}, false, "Monadhliath Mountains North & Strathdearn"},
	{"NH7442" , "NJ0422", 418, {0,0,0,0}, false, "Lochindorb Grantown-on-spey & Carrbridge"},
	{"NH9952" , "NJ2319", 419, {0,0,0,0}, false, "Grantown-on-spey & Hills Of Cromdale"},
	{"NJ2233" , "NJ6813", 420, {0,0,0,0}, false, "Correen Hills & Glenlivet"},
	{"NJ6634" , "NK0711", 421, {0,0,0,0}, false, "Ellon & Inverurie"},
	{"NH7361" , "NJ0040", 422, {0,0,0,0}, false, "Nairn & Cawdor"},
	{"NH9373" , "NJ2651", 423, {0,0,0,0}, false, "Elgin Forres & Lossiemouth"},
	{"NJ2270" , "NJ4533", 424, {0,0,0,0}, false, "Buckie & Keith"},
	{"NJ4570" , "NJ6833", 425, {0,0,0,0}, false, "Huntly & Cullen"},
	{"NJ6868" , "NJ9134", 426, {0,0,0,0}, false, "Banff Macduff & Turriff"},
	{"NJ9168" , "NK1531", 427, {0,0,0,0}, false, "Peterhead & Fraserburgh"},
	{"NG6865" , "NG9127", 428, {0,0,0,0}, false, "Kyle Of Lochalsh Plockton & Applecross"},
	{"NG9057" , "NH1330", 429, {0,0,0,0}, false, "Glen Carron & West Monar"},
	{"NH1057" , "NH3230", 430, {0,0,0,0}, false, "Loch Monar Glen Cannich & Glen Strathfarrar"},
	{"NH3256" , "NH5428", 431, {0,0,0,0}, false, "Glen Urquhart & Strathglass"},
	{"NH5268" , "NH8246", 432, {0,0,0,0}, false, "Black Isle"},
	{"NG7276" , "NH0353", 433, {0,0,0,0}, false, "Torridon Beinn Eighe & Liathach"},
	{"NB7200" , "NG9470", 434, {0,0,0,0}, false, "Gairloch & Loch Ewe"},
	{"NG9399" , "NH1857", 435, {0,0,0,0}, false, "An Teallach & Slioch"},
	{"NH1296" , "NH3557", 436, {0,0,0,0}, false, "Beinn Dearg & Loch Fannich"},
	{"NH3293" , "NH5656", 437, {0,0,0,0}, false, "Ben Wyvis & Strathpeffer"},
	{"NH5592" , "NH9668", 438, {0,0,0,0}, false, "Dornoch & Tain"},
	{"NB9023" , "NH3094", 439, {0,0,0,0}, false, "Coigach & Summer Isles"},
	{"NC3035" , "NH5091", 440, {0,0,0,0}, false, "Glen Cassley & Glen Oykel"},
	{"NC4910" , "NH9590", 441, {0,0,0,0}, false, "Lairg Bonar Bridge & Golspie"},
	{"NC0036" , "NC3713", 442, {0,0,0,0}, false, "Assynt & Lochinver"},
	{"NC5035" , "NC7208", 443, {0,0,0,0}, false, "Ben Klibreck & Ben Armine"},
	{"NC7130" , "ND1809", 444, {0,0,0,0}, false, "Helmsdale & Strath Of Kildonan"},
	{"NC1152" , "NC3931", 445, {0,0,0,0}, false, "Foinaven Arkle Kylesku & Scourie"},
	{"NC1676" , "NC4652", 446, {0,0,0,0}, false, "Durness & Cape Wrath"},
	{"NC3970" , "NC6232", 447, {0,0,0,0}, false, "Ben Hope Ben Loyal & Kyle Of Tongue"},
	{"NC5870" , "NC8130", 448, {0,0,0,0}, false, "Bettyhill Strathnaver & Loch Loyal"},
	{"NC8070" , "ND0030", 449, {0,0,0,0}, false, "Strath Halladale & Strathy Point"},
	{"ND0056" , "ND4030", 450, {0,0,0,0}, false, "Wick & The Flow Country"},
	{"ND0080" , "ND4152", 451, {0,0,0,0}, false, "Thurso & John O' Groats"},
	{"NF5316" , "NL8179", 452, {0,0,0,0}, false, "Barra & Vatersay"},
	{"NF7059" , "NF9307", 453, {0,0,0,0}, false, "Benbecula & South Uist"},
	{"NF5785" , "NG0358", 454, {0,0,0,0}, false, "North Uist & Berneray"},
	{"NA8704" , "NG2581", 455, {0,0,0,0}, false, "South Harris"},
	{"NA8720" , "NB2500", 456, {0,0,0,0}, false, "North Harris & Loch Seaforth"},
	{"NB2326" , "NG4596", 457, {0,0,0,0}, false, "South East Lewis"},
	{"NA9745" , "NB2419", 458, {0,0,0,0}, false, "West Lewis"},
	{"NB1644" , "NB5824", 459, {0,0,0,0}, false, "Central Lewis & Stornoway"},
	{"NA6867" , "NB5744", 460, {0,0,0,0}, false, "North Lewis"},
	{"HY3325" , "ND6376", 461, {0,0,0,0}, false, "Orkney East Mainland"},
	{"HY1706" , "ND4484", 462, {0,0,0,0}, false, "Orkney Hoy South Walls & Flotta"},
	{"HY2131" , "HY4303", 463, {0,0,0,0}, false, "Orkney West Mainland"},
	{"HY3556" , "HY5725", 464, {0,0,0,0}, false, "Orkney Westray Papa Westray Rousay Egilsay & Wyre"},
	{"HY5058" , "HY8015", 465, {0,0,0,0}, false, "Orkney Sanday Eday North Ronaldsay & Stronsay"},
	{"HU1846" , "HZ5668", 466, {0,0,0,0}, false, "Shetland Mainland South"},
	{"HT9363" , "HU5336", 467, {0,0,0,0}, false, "Shetland Mainland Central"},
	{"HU4080" , "HU7158", 468, {0,0,0,0}, false, "Shetland Mainland North East"},
	{"HU0999" , "HU4360", 469, {0,0,0,0}, false, "Shetland Mainland North West"},
	{"HP4321" , "HU6877", 470, {0,0,0,0}, false, "Shetland Unst Yell & Fetlar"},
	{"NA0010" , "NF2090", 471, {0,0,0,0}, false, "St. Kilda"},
};

void GetExplorerExtents (long lngExplorer, CString& strWestNorth, CString& strEastSouth, CString& strMap)
{
	for (int i=0; i < N_EXPLORER_MAPS; i++) {
		if (lngExplorer == maps[i].lngExplorer) {
			strWestNorth = maps[i].strCoord1;
			strEastSouth = maps[i].strCoord2;
			strMap.Format("Explorer%03d",maps[i].lngExplorer);
			return;
		}
	}
}
int GetExplorerMapNumber(int nMapNumber, int iDelta)
{
	if (nMapNumber <= 1) return 1;

	int i=0;
	while (nMapNumber > maps[i].lngExplorer) {
		i++;
		if (nMapNumber == maps[i].lngExplorer) {
			return maps[i].lngExplorer;
		}
	}
	/*
	for (i=0; i < N_EXPLORER_MAPS; i++) {
		if (nMapNumber == maps[i].lngExplorer) {
			return maps[i].lngExplorer;
		}
	}
	*/

	if (i > N_EXPLORER_MAPS) {
		return maps[N_EXPLORER_MAPS-1].lngExplorer;
	} else {
		if (iDelta > 0) {
			return maps[i-1].lngExplorer;
		} else {
			return maps[i].lngExplorer;
		}
	}
}

CString GetExplorerMapName (long lngExplorer)
{
	for (int i=0; i < N_EXPLORER_MAPS; i++) {
		if (lngExplorer == maps[i].lngExplorer) {
			return maps[i].strLocation;
		}
	}
	return ("N/A");
}

#define	N_LANDRANGER_MAPS	204

struct LandrangerMaps {
	char *	strCoord1;
	char *	strCoord2;
	long	lngExplorer;
	RECT	rRectangle;
	bool	bExplorer;
	char *	strLocation;
} LandrangerMaps[N_LANDRANGER_MAPS] = {
	{"HP690200", "HU290800",  1, {0,0,0,0}, false, "Shetland, Yell, Unst & Fetlar"},
	{"HU730960", "HU330560",  2, {0,0,0,0}, false, "Shetland, Sullom Voe & Whalsay"},
	{"HU540870", "HU140470",  3, {0,0,0,0}, false, "Shetland - North Mainland"},
	{"HU600470", "HU200070",  4, {0,0,0,0}, false, "Shetland - South Mainland"},
	{"HY800600", "HY400200",  5, {0,0,0,0}, false, "Orkney - Northern Isles"},
	{"HY610360", "ND210960",  6, {0,0,0,0}, false, "Orkney - Mainland"},
	{"HY550100", "ND150700",  7, {0,0,0,0}, false, "Orkney - Southern Isles"},
	{"NB570660", "NB170260",  8, {0,0,0,0}, false, "Stornoway & North Lewis"},
	{"NC520800", "NC120400",  9, {0,0,0,0}, false, "Cape Wrath, Durness & Scourie"},
	{"NC920800", "NC520400", 10, {0,0,0,0}, false, "Strathnaver, Bettyhill & Tongne"},
	{"ND320690", "NC920290", 11, {0,0,0,0}, false, "Thurso & Dunbeath"},
	{"ND400790", "ND000390", 12, {0,0,0,0}, false, "Thurso & Wick, John O'Groats"},
	{"NB350430", "NA950030", 13, {0,0,0,0}, false, "West Lewis & North Harris"},
	{"NB450260", "NG050860", 14, {0,0,0,0}, false, "Tarbert & Loch Seaforth"},
	{"NC360400", "NB960000", 15, {0,0,0,0}, false, "Loch Assynt, Lochinver & Kylesku"},
	{"NC760400", "NC360000", 16, {0,0,0,0}, false, "Lairg & Loch Shin, Loch Naver"},
	{"ND160400", "NC760000", 17, {0,0,0,0}, false, "Helmsdale & Strath of Kildonan"},
	{"NB090030", "NF690630", 18, {0,0,0,0}, false, "Sound of Harris & St Kilda"},
	{"NC140000", "NG740600", 19, {0,0,0,0}, false, "Gairloch & Ullapool, Loch Maree"},
	{"NC540000", "NH140600", 20, {0,0,0,0}, false, "Beinn Dearg & Loch Broom, Ben Wyvis"},
	{"NC940000", "NH540600", 21, {0,0,0,0}, false, "Dornoch & Alness, Invergordon & Tain"},
	{"NF970630", "NF570230", 22, {0,0,0,0}, false, "Benbecula & South Uist"},
	{"NG530760", "NG130360", 23, {0,0,0,0}, false, "North Skye, Dunvegan & Portree"},
	{"NG900700", "NG500300", 24, {0,0,0,0}, false, "Raasay & Applecross, Loch Torridon & Plockton"},
	{"NH300600", "NG900200", 25, {0,0,0,0}, false, "Glen Carron & Glen Affric"},
	{"NH700600", "NH300200", 26, {0,0,0,0}, false, "Inverness & Strathglass Area"},
	{"NJ100700", "NH700300", 27, {0,0,0,0}, false, "Nairn & Forres, River Findhorn"},
	{"NJ500730", "NJ100330", 28, {0,0,0,0}, false, "Elgin, Dufftown & Surrounding Area"},
	{"NJ850700", "NJ450300", 29, {0,0,0,0}, false, "Banff & Surrounding Area"},
	{"NK170700", "NJ770300", 30, {0,0,0,0}, false, "Fraserburgh, Peterhead & District"},
	{"NF900230", "NL500830", 31, {0,0,0,0}, false, "Barra & South Uist, Vatersay & Eriskay"},
	{"NG700400", "NG300000", 32, {0,0,0,0}, false, "South Skye & Cullin Hills"},
	{"NH100300", "NM700900", 33, {0,0,0,0}, false, "Loch Alsh, Glen Shiel & Loch Hourn"},
	{"NH500200", "NN100800", 34, {0,0,0,0}, false, "Fort Augustus, Glen Albyn & Glen Roy"},
	{"NH900300", "NN500900", 35, {0,0,0,0}, false, "Kingussie & Monadhliath Mountains"},
	{"NJ250330", "NN850930", 36, {0,0,0,0}, false, "Grantown & Aviemore, Cairngorm Mountains"},
	{"NJ650330", "NO250930", 37, {0,0,0,0}, false, "Strathdon"},
	{"NK050300", "NO650900", 38, {0,0,0,0}, false, "Aberdeen, Inverurie & Pitmedden"},
	{"NG600100", "NM200700", 39, {0,0,0,0}, false, "Rum, Eigg & Muck"},
	{"NH000000", "NM600600", 40, {0,0,0,0}, false, "Mallaig & Glenfinnan, Loch Shiel"},
	{"NN400900", "NN000500", 41, {0,0,0,0}, false, "Ben Nevis, Fort William & Glen Coe"},
	{"NN800900", "NN400500", 42, {0,0,0,0}, false, "Glen Garry & Loch Rannoch Area"},
	{"NJ200000", "NN800600", 43, {0,0,0,0}, false, "Braemar & Blair Atholl"},
	{"NJ600000", "NO200600", 44, {0,0,0,0}, false, "Ballater, Glen Clova & Surrounding Area"},
	{"NK000000", "NO600600", 45, {0,0,0,0}, false, "Stonehaven & Banchory"},
	{"NM320730", "NL920330", 46, {0,0,0,0}, false, "Coll & Tiree"},
	{"NM600730", "NM200330", 47, {0,0,0,0}, false, "Tobermory & North Mull"},
	{"NM600500", "NM200100", 48, {0,0,0,0}, false, "Iona & West Mull, Ulva"},
	{"NN000600", "NM600200", 49, {0,0,0,0}, false, "Oban & East Mull"},
	{"NN400500", "NN000100", 50, {0,0,0,0}, false, "Glen Orchy & Loch Etive"},
	{"NN800600", "NN400200", 51, {0,0,0,0}, false, "Loch Tay & Glen Dochart"},
	{"NO100600", "NN700200", 52, {0,0,0,0}, false, "Pitlochry to Crieff"},
	{"NO340600", "NN940200", 53, {0,0,0,0}, false, "Blairgowrie & Forest of Alyth"},
	{"NO740600", "NO340200", 54, {0,0,0,0}, false, "Dundee & Montrose, Forfar & Arbroath"},
	{"NN040200", "NR640800", 55, {0,0,0,0}, false, "Lochgilphead & Loch Awe"},
	{"NN440220", "NS040820", 56, {0,0,0,0}, false, "Loch Lomond & Inveraray"},
	{"NN840220", "NS440820", 57, {0,0,0,0}, false, "Stirling & The Trossachs"},
	{"NO240300", "NS840900", 58, {0,0,0,0}, false, "Perth & Alloa, Auchterarder"},
	{"NO640300", "NT240900", 59, {0,0,0,0}, false, "St Andrews, Kirkcaldy & Glenrothes"},
	{"NR500800", "NR100400", 60, {0,0,0,0}, false, "Islay"},
	{"NM710020", "NR310620", 61, {0,0,0,0}, false, "Jura & Colonsay"},
	{"NS000800", "NR600400", 62, {0,0,0,0}, false, "North Kintyre & Tarbert"},
	{"NS400820", "NS000420", 63, {0,0,0,0}, false, "Firth of Clyde Area"},
	{"NS800850", "NS400450", 64, {0,0,0,0}, false, "Glasgow, Motherwell & Airdrie"},
	{"NT200900", "NS800500", 65, {0,0,0,0}, false, "Falkirk & Linlithgow, Dunfermline"},
	{"NT560900", "NT160500", 66, {0,0,0,0}, false, "Edinburgh, Penicuik & North Berwick"},
	{"NT960900", "NT560500", 67, {0,0,0,0}, false, "Duns, Dunbar & Eyemouth Area"},
	{"NR970400", "NR570000", 68, {0,0,0,0}, false, "South Kintyre & Campbeltown"},
	{"NS150530", "NR750130", 69, {0,0,0,0}, false, "Isle of Arran"},
	{"NS550450", "NS150050", 70, {0,0,0,0}, false, "Ayr, Kilmarnock & Troon"},
	{"NS950450", "NS550050", 71, {0,0,0,0}, false, "Lanark & Upper Nithsdale Area"},
	{"NT200600", "NS800200", 72, {0,0,0,0}, false, "Upper Clyde Valley, Biggar & Lanark"},
	{"NT600600", "NT200200", 73, {0,0,0,0}, false, "Peebles, Galashiels & Selkirk"},
	{"NT970600", "NT570200", 74, {0,0,0,0}, false, "Kelso & Coldstream, Jedburgh & Duns"},
	{"NU300600", "NT900200", 75, {0,0,0,0}, false, "Berwick-upon-Tweed & Surrounding Area"},
	{"NS350100", "NW950700", 76, {0,0,0,0}, false, "Girvan, Ballantrae & Barrhill"},
	{"NS750100", "NX350700", 77, {0,0,0,0}, false, "Dalmellington & New Galloway, Galloway Forest Park"},
	{"NT150200", "NX750800", 78, {0,0,0,0}, false, "Nithsdale & Annandale Area"},
	{"NT550200", "NY150800", 79, {0,0,0,0}, false, "Hawick & Eskdale, Langholm"},
	{"NT950200", "NY550800", 80, {0,0,0,0}, false, "Cheviot Hills & Kielder Forest Area"},
	{"NU350200", "NY950800", 81, {0,0,0,0}, false, "Alnwick, Morpeth & Surrounding Area"},
	{"NX350700", "NW950300", 82, {0,0,0,0}, false, "Stranraer & Glenluce, The Rhins"},
	{"NX750700", "NX350300", 83, {0,0,0,0}, false, "Newton Stewart & Kirkcudbright, Gatehouse of Fleet"},
	{"NY050800", "NX650400", 84, {0,0,0,0}, false, "Dumfries & Castle Douglas"},
	{"NY450800", "NY050400", 85, {0,0,0,0}, false, "Carlisle & Solway Firth, Gretna Green"},
	{"NY850800", "NY450400", 86, {0,0,0,0}, false, "Haltwhistle & Brampton, Newcastle & Alston"},
	{"NZ070800", "NY670400", 87, {0,0,0,0}, false, "Hexham & Haltwhistle"},
	{"NZ470800", "NZ070400", 88, {0,0,0,0}, false, "Newcastle upon Tyne, Durham & Sunderland"},
	{"NY300400", "NX900000", 89, {0,0,0,0}, false, "West Cumbria, Cockermouth & Wast Water"},
	{"NY570400", "NY170000", 90, {0,0,0,0}, false, "Penrith & Keswick, Ambleside"},
	{"NY970400", "NY570000", 91, {0,0,0,0}, false, "Appleby-in-Westmorland"},
	{"NZ200400", "NY800000", 92, {0,0,0,0}, false, "Barnard Castle & Surrounding Area"},
	{"NZ600400", "NZ200000", 93, {0,0,0,0}, false, "Middlesbrough, Darlington & Hartlepool"},
	{"OV000250", "SE600850", 94, {0,0,0,0}, false, "Whitby & Esk Dale, Robin Hood's Bay"},
	{"NX530050", "SC130650", 95, {0,0,0,0}, false, "Isle of Man"},
	{"NY430000", "SD030600", 96, {0,0,0,0}, false, "Barrow-in-Furness & South Lakeland"},
	{"NY660000", "SD260600", 97, {0,0,0,0}, false, "Kendal & Morecambe, Windermere & Lancaster"},
	{"NZ060000", "SD660600", 98, {0,0,0,0}, false, "Wensleydale & Upper Wharfedale"},
	{"NZ460000", "SE060600", 99, {0,0,0,0}, false, "Northallerton & Ripon, Pateley Bridge & Leyburn"},
	{"NZ860000", "SE460600", 100, {0,0,0,0}, false, "Malton, Pickering, Helmsley & Easingwold"},
	{"OV260000", "SE860600", 101, {0,0,0,0}, false, "Scarborough, Bridlington & Filey"},
	{"SD660600", "SD260200", 102, {0,0,0,0}, false, "Preston & Blackpool, Lytham St Anne's"},
	{"SE000600", "SD600200", 103, {0,0,0,0}, false, "Blackburn & Burnley, Clitheroe & Skipton"},
	{"SE400600", "SE000200", 104, {0,0,0,0}, false, "Leeds & Bradford, Harrogate & Ilkley"},
	{"SE800600", "SE400200", 105, {0,0,0,0}, false, "York & Selby"},
	{"TA030600", "SE630200", 106, {0,0,0,0}, false, "Market Weighton & Surrounding Area"},
	{"TA400600", "TA000200", 107, {0,0,0,0}, false, "Kingston upon Hull & Surrounding Area"},
	{"SD600200", "SJ200800", 108, {0,0,0,0}, false, "Liverpool, Southport & Wigan"},
	{"SE000200", "SJ600800", 109, {0,0,0,0}, false, "Manchester, Bolton & Warrington"},
	{"SE400200", "SK000800", 110, {0,0,0,0}, false, "Sheffield & Huddersfield, Glossop & Holmfirth"},
	{"SE700200", "SK300800", 111, {0,0,0,0}, false, "Sheffield & Doncaster, Rotherham, Barnsley & Thorne"},
	{"TA100250", "SK700850", 112, {0,0,0,0}, false, "Scunthorpe & Gainsborough"},
	{"TA500260", "TF100860", 113, {0,0,0,0}, false, "Grimsby, Louth & Market Rasen"},
	{"SC600000", "SH200600", 114, {0,0,0,0}, false, "Anglesey"},
	{"SH800850", "SH400450", 115, {0,0,0,0}, false, "Snowdon & Surrounding Area"},
	{"SJ200850", "SH800450", 116, {0,0,0,0}, false, "Denbigh & Colwyn Bay"},
	{"SJ600800", "SJ200400", 117, {0,0,0,0}, false, "Chester & Wrexham, Ellesmere Port"},
	{"SK000800", "SJ600400", 118, {0,0,0,0}, false, "Stoke-on-Trent & Macclesfield"},
	{"SK400800", "SK000400", 119, {0,0,0,0}, false, "Buxton & Matlock , Bakewell & Dove Dale"},
	{"SK800900", "SK400500", 120, {0,0,0,0}, false, "Mansfield & Worksop, Sherwood Forest"},
	{"TF180900", "SK780500", 121, {0,0,0,0}, false, "Lincoln & Newark-on-Trent"},
	{"TF580900", "TF180500", 122, {0,0,0,0}, false, "Skegness & Horncastle"},
	{"SH500600", "SH100200", 123, {0,0,0,0}, false, "Lleyn Peninsula"},
	{"SH900450", "SH500050", 124, {0,0,0,0}, false, "Porthmadog & Dolgellau"},
	{"SJ200450", "SH800050", 125, {0,0,0,0}, false, "Bala & Lake Vyrnwy, Berwyn"},
	{"SJ600400", "SJ200000", 126, {0,0,0,0}, false, "Shrewsbury & Oswestry"},
	{"SK000400", "SJ600000", 127, {0,0,0,0}, false, "Stafford & Telford, Ironbridge"},
	{"SK400480", "SK000080", 128, {0,0,0,0}, false, "Derby & Burton upon Trent"},
	{"SK800500", "SK400100", 129, {0,0,0,0}, false, "Nottingham & Loughborough, Melton Mowbray"},
	{"TF200500", "SK800100", 130, {0,0,0,0}, false, "Grantham, Sleaford & Bourne"},
	{"TF600500", "TF200100", 131, {0,0,0,0}, false, "Boston & Spalding"},
	{"TG000500", "TF600100", 132, {0,0,0,0}, false, "North West Norfolk, Kings Lynn & Fakenham"},
	{"TG400500", "TG000100", 133, {0,0,0,0}, false, "North East Norfolk, Cromer & Wroxham"},
	{"TG570300", "TM170900", 134, {0,0,0,0}, false, "Norwich & The Broads, Great Yarmouth"},
	{"SH900050", "SN500650", 135, {0,0,0,0}, false, "Aberystwyth & Machynlleth"},
	{"SJ200050", "SN800650", 136, {0,0,0,0}, false, "Newtown, Llanidloes & Surrounding Area"},
	{"SJ600000", "SO200600", 137, {0,0,0,0}, false, "Ludlow & Church Stretton, Wenlock Edge"},
	{"SJ850000", "SO450600", 138, {0,0,0,0}, false, "Kidderminster & Wyre Forest"},
	{"SK250080", "SO850680", 139, {0,0,0,0}, false, "Birmingham & Wolverhampton"},
	{"SK650100", "SP250700", 140, {0,0,0,0}, false, "Leicester, Coventry & Rugby"},
	{"TF050100", "SP650700", 141, {0,0,0,0}, false, "Kettering, Corby & Surrounding Area"},
	{"TF450140", "TL050740", 142, {0,0,0,0}, false, "Peterborough, Market Deeping & Chatteris"},
	{"TF770140", "TL370740", 143, {0,0,0,0}, false, "Ely & Wisbech, Downham Market"},
	{"TG170100", "TL770700", 144, {0,0,0,0}, false, "Thetford & Diss, Breckland & Wymondham"},
	{"SN400600", "SN000200", 145, {0,0,0,0}, false, "Cardigan & Mynydd Preseli"},
	{"SN800650", "SN400250", 146, {0,0,0,0}, false, "Lampeter, Llandovery & Surrounding Area"},
	{"SO100800", "SN700400", 147, {0,0,0,0}, false, "Elan Valley & Builth Wells"},
	{"SO500800", "SO100400", 148, {0,0,0,0}, false, "Presteigne & Hay-on-Wye Area"},
	{"SO730680", "SO330280", 149, {0,0,0,0}, false, "Hereford & Leominster, Bromyard & Ledbury"},
	{"SP130680", "SO730280", 150, {0,0,0,0}, false, "Worcester & The Malverns, Evesham & Tewkesbury"},
	{"SP530700", "SP130300", 151, {0,0,0,0}, false, "Stratford-upon-Avon, Warwick & Banbury"},
	{"SP930700", "SP530300", 152, {0,0,0,0}, false, "Northampton & Milton Keynes, Buckingham & Daventry"},
	{"TL330740", "SP930340", 153, {0,0,0,0}, false, "Bedford & Huntingdon, St Neots & Biggleswade"},
	{"TL730740", "TL330340", 154, {0,0,0,0}, false, "Cambridge & Newmarket, Saffron Walden"},
	{"TM130740", "TL730340", 155, {0,0,0,0}, false, "Bury St Edmunds, Sudbury & Stowmarket"},
	{"TM530900", "TM130500", 156, {0,0,0,0}, false, "Saxmundham, Aldeburgh & Southwold"},
	{"SN050410", "SM650010", 157, {0,0,0,0}, false, "St david's & Haverfordwest"},
	{"SN290300", "SR890900", 158, {0,0,0,0}, false, "Tenby & Pembroke"},
	{"SN690250", "SS290850", 159, {0,0,0,0}, false, "Swansea & Gower, Carmarthen"},
	{"SO090450", "SN690050", 160, {0,0,0,0}, false, "Brecon Beacons"},
	{"SO490450", "SO090050", 161, {0,0,0,0}, false, "Abergavenny & The Black Mountains"},
	{"SO890280", "ST490880", 162, {0,0,0,0}, false, "Gloucester & Forest of Dean"},
	{"SP290300", "ST890900", 163, {0,0,0,0}, false, "Cheltenham & Cirencester, Stow-on-the-Wold"},
	{"SP690300", "SU290900", 164, {0,0,0,0}, false, "Oxford, Chipping Norton & Bicester"},
	{"TL000350", "SU600950", 165, {0,0,0,0}, false, "Aylesbury & Leighton Buzzard, Thame & Berkhamsted"},
	{"TL400340", "TQ000940", 166, {0,0,0,0}, false, "Luton, Hertford & Surrounding Area"},
	{"TL800340", "TQ400940", 167, {0,0,0,0}, false, "Chelmsford, Harlow & Bishops Stortford"},
	{"TM200340", "TQ800940", 168, {0,0,0,0}, false, "Colchester, Halstead & Maldon"},
	{"TM470500", "TM070100", 169, {0,0,0,0}, false, "Ipswich & The Naze, Clacton-on-sea"},
	{"SO090050", "SS690650", 170, {0,0,0,0}, false, "Vale of Glamorgan, Rhondda & Porthcawl"},
	{"SO490050", "ST090650", 171, {0,0,0,0}, false, "Cardiff & Newport, Pontypool"},
	{"ST800950", "ST400550", 172, {0,0,0,0}, false, "Bristol & Bath, Thornbury & Chew Magna"},
	{"SU200950", "ST800550", 173, {0,0,0,0}, false, "Swindon & Devizes, Marlborough & Trowbridge"},
	{"SU600950", "SU200550", 174, {0,0,0,0}, false, "Newbury & Wantage, Hungerford & Didcot"},
	{"TQ000950", "SU600550", 175, {0,0,0,0}, false, "Reading, Windsor & Surrounding Area"},
	{"TL350000", "SU950600", 176, {0,0,0,0}, false, "West London, Rickmansworth & Staines"},
	{"TL700000", "TQ300600", 177, {0,0,0,0}, false, "East London, Billericay & Gravesend"},
	{"TR050950", "TQ650550", 178, {0,0,0,0}, false, "Thames Estuary, Rochester & Southend-on-sea"},
	{"TR430730", "TR030330", 179, {0,0,0,0}, false, "Canterbury & East Kent, Dover & Margate"},
	{"SS800520", "SS400120", 180, {0,0,0,0}, false, "Barnstaple & Ilfracombe, Lynton and Bideford"},
	{"ST200520", "SS800120", 181, {0,0,0,0}, false, "Minehead & Brendon Hills, Dulverton & Tiverton"},
	{"ST600700", "ST200300", 182, {0,0,0,0}, false, "Weston-Super-Mare, Bridgwater & Wells"},
	{"ST890550", "ST490150", 183, {0,0,0,0}, false, "Yeovil & Frome, Shaftsbury & Shepton Mallet"},
	{"SU290550", "ST890150", 184, {0,0,0,0}, false, "Salisbury & The Plain, Amesbury"},
	{"SU690560", "SU290160", 185, {0,0,0,0}, false, "Winchester & Basingstoke, Andover & Romsey"},
	{"TQ050650", "SU650250", 186, {0,0,0,0}, false, "Aldershot & Guildford, Camberley & Haselmere"},
	{"TQ450650", "TQ050250", 187, {0,0,0,0}, false, "Dorking, Reigate & Crawley Area"},
	{"TQ850650", "TQ450250", 188, {0,0,0,0}, false, "Maidstone & The Weald, Royal Tunbridge Wells"},
	{"TR250550", "TQ850150", 189, {0,0,0,0}, false, "Ashford & Romney Marsh, Rye and Folkestone"},
	{"SS470270", "SX070870", 190, {0,0,0,0}, false, "Bude, Clovelly & Surrounding Area"},
	{"SS870120", "SX470720", 191, {0,0,0,0}, false, "Okehampton & North Dartmoor, Crediton & Bovey Tracey"},
	{"ST270120", "SX870720", 192, {0,0,0,0}, false, "Exeter, Sidmouth, Exmouth & Teignmouth"},
	{"ST500300", "SY100900", 193, {0,0,0,0}, false, "Taunton & Lyme Regis, Chard & Bridport"},
	{"ST890150", "SY490750", 194, {0,0,0,0}, false, "Dorchester & Weymouth, Cerne Abbas & Bere Regis"},
	{"SU290150", "SY890750", 195, {0,0,0,0}, false, "Bournemouth & Purbeck, Wimbourne Minster & Ringwood"},
	{"SU690160", "SZ290760", 196, {0,0,0,0}, false, "Solent & Isle of Wight, Southampton & Portsmouth"},
	{"TQ090300", "SZ690900", 197, {0,0,0,0}, false, "Chichester & The South Downs, Bognor Regis & Arundel"},
	{"TQ490370", "TV090970", 198, {0,0,0,0}, false, "Brighton & Lewes, Haywards Heath"},
	{"TQ890340", "TV490940", 199, {0,0,0,0}, false, "Eastbourne & Hastings, Battle & Heathfield"},
	{"SX150900", "SW750500", 200, {0,0,0,0}, false, "Newquay & Bodmin, Camelford & St Austell"},
	{"SX550870", "SX150470", 201, {0,0,0,0}, false, "Plymouth & Launceston, Tavistock & Looe"},
	{"SX950720", "SX550320", 202, {0,0,0,0}, false, "Torbay & South Dartmoor, Totnes & Salcombe"},
	{"SW720510", "SW320110", 203, {0,0,0,0}, false, "Land's End & Isles of Scilly, St Ives and Lizard Point"},
	{"SX120540", "SW720140", 204, {0,0,0,0}, false, "Truro & Falmouth, Roseland Peninsula"},
											
//	{"NA0010" , "NF2090", 471, {0,0,0,0}, false, "St. Kilda"},
};

void GetLandrangerExtents (long lngExplorer, CString& strWestNorth, CString& strEastSouth, CString& strMap)
{
	for (int i=0; i < N_LANDRANGER_MAPS; i++) {
		if (lngExplorer == LandrangerMaps[i].lngExplorer) {
			strWestNorth = LandrangerMaps[i].strCoord1;
			strEastSouth = LandrangerMaps[i].strCoord2;
			strMap.Format("Landranger%03d",LandrangerMaps[i].lngExplorer);
			return;
		}
	}
}

int GetLandrangerMapNumber(int nMapNumber)
{
	if (nMapNumber <= 1) return 1;
	if (nMapNumber >= N_LANDRANGER_MAPS) return N_LANDRANGER_MAPS;
	return nMapNumber;
}

CString GetLandrangerMapName (long lngExplorer)
{
	for (int i=0; i < N_LANDRANGER_MAPS; i++) {
		if (lngExplorer == LandrangerMaps[i].lngExplorer) {
			return LandrangerMaps[i].strLocation;
		}
	}
	return ("N/A");
}
