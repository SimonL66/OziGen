#include "stdafx.h"
#include "Explorer.h"
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

CString GetRegionalAreaMapName (int nMapNumber)
{
	for (int i=0; i < N_REGIONAL_AREA_MAPS; i++) {
		if (nMapNumber == RegionalAreaMaps[i].nRegionalArea) {
			return RegionalAreaMaps[i].strLocation;
		}
	}
	return ("N/A");
}

/*

{"NY 03 42", "SD 57 77", 000, {0,0,0,0}, false, "[National Parks\Lake District National Park]"}
{"SD 92 12", "SK 33 44", 000, {0,0,0,0}, false, "[National Parks\Peak District National Park]"}
{"NY 61 10", "SE 18 50", 000, {0,0,0,0}, false, "[National Parks\Yorkshire Dales National Park]"}
{"SH 46 80", "SO 03 94", 000, {0,0,0,0}, false, "[National Parks\Snowdonia National Park]"}
{"SS 46 52", "SY 10 55", 000, {0,0,0,0}, false, "[National Parks\Dartmoor and Exmoor National Park]"}
{"SN 61 46", "SO 37 00", 000, {0,0,0,0}, false, "[National Parks\Brecon Beacons National Park]"}
{"NH 49 37", "NO 52 66", 000, {0,0,0,0}, false, "[National Parks\Cairngorms National Park]"}
{"NN 06 35", "NS 72 80", 000, {0,0,0,0}, false, "[National Parks\Loch Lomond and the Trossachs National Park]"}
{"SM 58 65", "SS 47 89", 000, {0,0,0,0}, false, "[National Parks\Pembrokeshire Coast National Park]"}
{"NT 61 35", "NZ 08 64", 000, {0,0,0,0}, false, "[National Parks\Northumberland National Park]"}
{"NZ 42 26", "TA 16 75", 000, {0,0,0,0}, false, "[National Parks\North York Moors National Park]"}
{"SU 00 36", "SZ 51 89", 000, {0,0,0,0}, false, "[National Parks\New Forest National Park]"}

{"SL 70 30", "SZ 30 00", 000, {0,0,0,0}, false, "[Large Regional Areas\South West England]"}
{"SP 20 35", "TW 50 70", 000, {0,0,0,0}, false, "[Large Regional Areas\South East England]"}
{"SD 10 20", "SP 95 25", 000, {0,0,0,0}, false, "[Large Regional Areas\Central England]"}
{"SE 65 30", "TN 00 10", 000, {0,0,0,0}, false, "[Large Regional Areas\Eastern England]"}
{"NW 80 80", "TA 60 05", 000, {0,0,0,0}, false, "[Large Regional Areas\Northern England]"}
{"SB 40 20", "ST 60 60", 000, {0,0,0,0}, false, "[Large Regional Areas\Wales]"}
{"NR 80 90", "NZ 40 30", 000, {0,0,0,0}, false, "[Large Regional Areas\Scottish Lowlands and Borders]"}
{"NF 40 05", "NU 40 00", 000, {0,0,0,0}, false, "[Large Regional Areas\Central Scotland]"}
{"HV 00 40", "NN 55 10", 000, {0,0,0,0}, false, "[Large Regional Areas\North West Scotland]"}
{"HN 30 40", "NP 80 90", 000, {0,0,0,0}, false, "[Large Regional Areas\North East Scotland]"}
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
