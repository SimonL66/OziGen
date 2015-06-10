#include "stdafx.h"
#include "Explorer.h"
#include "func.h"

#include "..\MapLib\mmmath.h"
#include "..\MapLib\map_conv.h"

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

void GetExtents (long lngExplorer, CString& strWestNorth, CString& strEastSouth, CString& strMap)
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
