#pragma once

// WMS - Web Map Service - http://en.wikipedia.org/wiki/Web_Map_Service
// TMS - Tile Map Service - http://en.wikipedia.org/wiki/Tile_Map_Service
// WTMS - Tile Map Service - http://en.wikipedia.org/wiki/Web_Map_Tile_Service

//	Austria

//	Belgium
#define WMS_NGI_BE_10	"http://www.ngi.be/testbed/wms/top10r_l08_fr?LAYERS=0&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image%2Fpng&SRS=EPSG%3A3812&BBOX=658799.999,632559.8865,659439.999,633199.8865&WIDTH=256&HEIGHT=256"
#define WMS_NGI_BE_50	"http://www.ngi.be/testbed/wms/top50r_l08_fr?LAYERS=0&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image%2Fpng&SRS=EPSG%3A3812&BBOX=658799.999,632559.8865,659439.999,633199.8865&WIDTH=256&HEIGHT=256"
#define WMS_NGI_BE_100	"http://www.ngi.be/testbed/wms/top100r_l08_fr?LAYERS=0&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image%2Fpng&SRS=EPSG%3A3812&BBOX=658799.999,632559.8865,659439.999,633199.8865&WIDTH=256&HEIGHT=256"

//	Britain

//	Czech Rebuplic
#define WMS_CUZK_50		"http://geoportal.cuzk.cz/WMS_ZM50_PUB/WMService.aspx?LAYERS=GR_ZM50&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image%2Fpng&SRS=EPSG%3A32633&BBOX=535149,5485393,536301,5486545&WIDTH=256&HEIGHT=256"
#define WMS_CUZK_500	"http://geoportal.cuzk.cz/WMS_PREHLEDKY/WMService.aspx?LAYERS=GR_MCR500&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image%2Fpng&SRS=EPSG%3A32633&BBOX=530413,5480913,536813,5487313&WIDTH=256&HEIGHT=256"
#define WMS_CUZK_2500	"http://geoportal.cuzk.cz/WMS_PREHLEDKY/WMService.aspx?LAYERS=GR_CR4&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image%2Fpng&SRS=EPSG%3A32633&BBOX=535533,5479633,566253,5510353&WIDTH=256&HEIGHT=256"

//	Eire

//	France

//	Germany
#define WMS_NRW_DTK10	"http://www.wms.nrw.de/geobasis/adv_dtk10?LAYERS=DTK10&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image%2Fpng&SRS=EPSG%3A31467&BBOX=3419312,5702088,3420080,5702856&WIDTH=256&HEIGHT=256"
#define WMS_NRW_DTK25	"http://www.wms.nrw.de/geobasis/adv_dtk25?LAYERS=DTK25vi&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image%2Fpng&SRS=EPSG%3A31467&BBOX=3419312,5702088,3420080,5702856&WIDTH=256&HEIGHT=256"
#define WMS_NRW_DLM50	"http://www.wms.nrw.de/wms/dlm50?LAYERS=dlm_50&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image%2Fpng&SRS=EPSG%3A31467&BBOX=3419312,5702088,3420080,5702856&WIDTH=256&HEIGHT=256"

// ToDo: Code up EPGS:31468 projection
//#define WMS_MV_DTK10	"http://www.geodaten-mv.de/dienste/adv_dtk10?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=mv_dtk10&BBOX=4461349,5943902,4461708,5944140&SRS=EPSG:31468&FORMAT=image/png&WIDTH=300&HEIGHT=200&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage"
//#define WMS_MV_DTK25	"http://www.geodaten-mv.de/dienste/adv_dtk25?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=mv_dtk25&BBOX=4482102,5997652,4483095,5998314&SRS=EPSG:31468&FORMAT=image/png&WIDTH=300&HEIGHT=200&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage"
//#define WMS_MV_DTK50	"http://www.geodaten-mv.de/dienste/adv_dtk50?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=mv_dtk50&BBOX=4461245,5943634,4463517,5945149&SRS=EPSG:31468&FORMAT=image/png&WIDTH=300&HEIGHT=200&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage"
//#define WMS_MV_DTK100	"http://www.geodaten-mv.de/dienste/adv_dtk100?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=mv_dtk100&BBOX=4465029,5946945,4467951,5948893&SRS=EPSG:31468&FORMAT=image/png&WIDTH=300&HEIGHT=200&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage"
//#define WMS_MV_DTK250	"http://www.geodaten-mv.de/dienste/gdimv_uek250?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=gdimv_uek250&BBOX=4457891,5940891,4466007,5946302&SRS=EPSG:31468&FORMAT=image/png&WIDTH=300&HEIGHT=200&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage"
//#define WMS_MV_DTK750	"http://www.geodaten-mv.de/dienste/gdimv_uek750?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=gdimv_uek750&BBOX=4457749,5932873,4477382,5945962&SRS=EPSG:31468&FORMAT=image/png&WIDTH=300&HEIGHT=200&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage"
//#define WMS_MV_DTK1000	"http://www.geodaten-mv.de/dienste/gdimv_uek1000?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=gdimv_uek1000&BBOX=4444333,5934008,4477055,5954409&SRS=EPSG:31468&FORMAT=image/png&WIDTH=300&HEIGHT=200&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage"

// Hacked to EPGS:31467 to make it work.
#define WMS_MV_DTK10	"http://www.geodaten-mv.de/dienste/adv_dtk10?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=mv_dtk10&BBOX=4461349,5943902,4461708,5944140&SRS=EPSG:31467&FORMAT=image/png&WIDTH=300&HEIGHT=200&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage"
#define WMS_MV_DTK25	"http://www.geodaten-mv.de/dienste/adv_dtk25?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=mv_dtk25&BBOX=4482102,5997652,4483095,5998314&SRS=EPSG:31467&FORMAT=image/png&WIDTH=300&HEIGHT=200&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage"
#define WMS_MV_DTK50	"http://www.geodaten-mv.de/dienste/adv_dtk50?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=mv_dtk50&BBOX=4461245,5943634,4463517,5945149&SRS=EPSG:31467&FORMAT=image/png&WIDTH=300&HEIGHT=200&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage"
#define WMS_MV_DTK100	"http://www.geodaten-mv.de/dienste/adv_dtk100?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=mv_dtk100&BBOX=4465029,5946945,4467951,5948893&SRS=EPSG:31467&FORMAT=image/png&WIDTH=300&HEIGHT=200&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage"

// Other MV Maps...
// Offene RegionalKarte MV (ORKa-MV)
// "http://geo.sv.rostock.de/geodienste/stadtplan/wms?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=stadtplan&BBOX=4444628,5959658,4471448,5977478&SRS=EPSG:31468&FORMAT=image/png&WIDTH=300&HEIGHT=200&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage"
// TopoMV - maﬂstabsunabh‰ngige Webkarte
// "http://www.geodaten-mv.de/dienste/gdimv_topomv?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=gdimv_topomv&BBOX=4460609,5943243,4464396,5945605&SRS=EPSG:31468&FORMAT=image/jpeg&WIDTH=300&HEIGHT=200&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage"
// "http://www.geodaten-mv.de/dienste/gdimv_topomv?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=gdimv_topomv&BBOX=4460609,5943243,4464396,5945605&SRS=EPSG:31468&FORMAT=image/jpeg&WIDTH=300&HEIGHT=200&STYLES=grau&EXCEPTIONS=application/vnd.ogc.se_inimage"
// Kombinierte Digitale Topographische Karten und Gebietskarten 1:10.000 - 1:1.000.000
// "http://www.geodaten-mv.de/dienste/gdimv_dtk?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=gdimv_dtk&BBOX=4461349,5943902,4461708,5944140&SRS=EPSG:31468&FORMAT=image/jpeg&WIDTH=300&HEIGHT=200&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage"
// "http://www.geodaten-mv.de/dienste/gdimv_dtk?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=gdimv_dtk&BBOX=4461349,5943902,4461708,5944140&SRS=EPSG:31468&FORMAT=image/jpeg&WIDTH=300&HEIGHT=200&STYLES=grau&EXCEPTIONS=application/vnd.ogc.se_inimage"
// Historische Topographische Karte 1:25.000 1990-1996 (Normalblattschnitt)
// "http://www.geodaten-mv.de/dienste/tk25n_laiv_wms?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=mv_tk25n&BBOX=4482102,5997652,4483095,5998314&SRS=EPSG:31468&FORMAT=image/jpeg&WIDTH=300&HEIGHT=200&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage"
// Historische Topographische Karte 1:25.000 1981-1987 (Ausgabe Staat)
// "http://www.geodaten-mv.de/dienste/tk25as_laiv_wms?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=mv_tk25as&BBOX=4482102,5997652,4483095,5998314&SRS=EPSG:31468&FORMAT=image/jpeg&WIDTH=300&HEIGHT=200&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage"
// Historische Topographische Karte 1:25.000 1877-1889 (Messtischbl‰tter)
// "http://www.geodaten-mv.de/dienste/tk25m_laiv_wms?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=mv_tk25m&BBOX=4482102,5997652,4483095,5998314&SRS=EPSG:31468&FORMAT=image/jpeg&WIDTH=300&HEIGHT=200&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage"

#define WMS_BKG_200K	"http://sg.geodatenzentrum.de/wms_dtk200?LAYERS=DTK200&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image%2Fjpeg&SRS=EPSG%3A31467&BBOX=3779760,5802440,3797680,5820360&WIDTH=256&HEIGHT=256"
#define WMS_BKG_500K	"http://sg.geodatenzentrum.de/wms_dtk500?LAYERS=DTK500&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image%2Fjpeg&SRS=EPSG%3A31467&BBOX=3726000,5805000,3790000,5869000&WIDTH=256&HEIGHT=256"
#define WMS_BKG_1000K	"http://sg.geodatenzentrum.de/wms_dtk1000?LAYERS=DTK1000&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image%2Fjpeg&SRS=EPSG%3A31467&BBOX=3726000,5805000,3790000,5869000&WIDTH=256&HEIGHT=256"
#define WMS_BKG_2500K	"http://sg.geodatenzentrum.de/wms_d2500?LAYERS=d2500&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image%2Fjpeg&SRS=EPSG%3A31467&BBOX=3726000,5805000,3790000,5869000&WIDTH=256&HEIGHT=256"

#define WMS_DTK500		"http://sg.geodatenzentrum.de/wms_dtk500?LAYERS=DTK500&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image%2Fpng&SRS=EPSG%3A31467&BBOX=3779760,5802440,3797680,5820360&WIDTH=256&HEIGHT=256"

//	Italy

//	Luxembourg

//	Netherlands

//	Northern Ireland

//	Poland

//	Portugal
#define WMS_SC_100		"http://mapas.igeo.pt/wms/sc100k?LAYERS=sc100k&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image%2Fjpeg&SRS=EPSG%3A3763&BBOX=15859.375,15742.1875,18242.1875,18125&WIDTH=256&HEIGHT=256"
#define WMS_SC_500		"http://mapas.igeo.pt/wms/sc500k?LAYERS=sc500k&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image%2Fjpeg&SRS=EPSG%3A3763&BBOX=49218.75,208750,58750,218281.25&WIDTH=256&HEIGHT=256"


//	Slovenia
#define WMS_DTK_50		"http://prostor2.gov.si/ows2-m-pub/ows?SERVICE=WMS&LAYERS=raster_gurs_pub%3ASI.GURS.RASTRI.DTK50&VERSION=1.3&EXCEPTIONS=INIMAGE&REQUEST=GetMap&STYLES=&FORMAT=image%2Fpng&CRS=EPSG%3A3912&BBOX=499112.58295571,95460.6322456,500392.58295571,96740.6322456&WIDTH=256&HEIGHT=256"
#define WMS_DPK_250		"http://prostor2.gov.si:80/ows2-m-pub/ows?SERVICE=WMS&LAYERS=raster_gurs_pub%3ASI.GURS.RASTRI.DPK250&VERSION=1.3&EXCEPTIONS=INIMAGE&REQUEST=GetMap&STYLES=&FORMAT=image%2Fpng&CRS=EPSG%3A3912&BBOX=497832.58295571,89060.6322456,504232.58295571,95460.6322456&WIDTH=256&HEIGHT=256"
#define WMS_DPK_500		"http://prostor2.gov.si:80/ows2-m-pub/ows?SERVICE=WMS&LAYERS=raster_gurs_pub%3ASI.GURS.RASTRI.DPK500&VERSION=1.3&EXCEPTIONS=INIMAGE&REQUEST=GetMap&STYLES=&FORMAT=image%2Fpng&CRS=EPSG%3A3912&BBOX=497832.58295571,89060.6322456,510632.58295571,101860.6322456&WIDTH=256&HEIGHT=256"
#define WMS_DPK_1000	"http://prostor2.gov.si:80/ows2-m-pub/ows?SERVICE=WMS&LAYERS=raster_gurs_pub%3ASI.GURS.RASTRI.DPK1000&VERSION=1.3&EXCEPTIONS=INIMAGE&REQUEST=GetMap&STYLES=&FORMAT=image%2Fpng&CRS=EPSG%3A3912&BBOX=446632.58295571,25060.6322456,523432.58295571,101860.6322456&WIDTH=256&HEIGHT=256"

//	Spain
#define WMS_IGN_ES		"http://www.idee.es/wms/MTN-Raster/MTN-Raster?LAYERS=mtn_rasterizado&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image%2Fpng&SRS=EPSG%3A23030&BBOX=446000,4410000,448000,4412000&WIDTH=200&HEIGHT=200"

//	Switzerland

//	Catalonia
#define WMS_MTC_10		"http://shagrat.icc.es/lizardtech/iserv/ows?LAYERS=mtc10m&EXCEPTIONS=application%2Fvnd.ogc.se_xml&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image%2Fpng&SRS=EPSG%3A23031&BBOX=383446,4639842,383830,4640226&WIDTH=256&HEIGHT=256"
#define WMS_MTC_25		"http://shagrat.icc.es/lizardtech/iserv/ows?LAYERS=mtc25m&EXCEPTIONS=application%2Fvnd.ogc.se_xml&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image%2Fpng&SRS=EPSG%3A23031&BBOX=382678,4639458,383446,4640226&WIDTH=256&HEIGHT=256"
#define WMS_MTC_50		"http://shagrat.icc.es/lizardtech/iserv/ows?LAYERS=mtc50m&EXCEPTIONS=application%2Fvnd.ogc.se_xml&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image%2Fpng&SRS=EPSG%3A23031&BBOX=381910,4639970,383190,4641250&WIDTH=256&HEIGHT=256"
#define WMS_MTC_250		"http://shagrat.icc.es:80/lizardtech/iserv/ows?LAYERS=mtc250m&EXCEPTIONS=application%2Fvnd.ogc.se_xml&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image%2Fpng&SRS=EPSG%3A23031&BBOX=387030,4632290,393430,4638690&WIDTH=256&HEIGHT=256"
#define WMS_MTC_500		"http://shagrat.icc.es:80/lizardtech/iserv/ows?LAYERS=mtc500m&EXCEPTIONS=application%2Fvnd.ogc.se_xml&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image%2Fpng&SRS=EPSG%3A23031&BBOX=451030,4638690,489430,4677090&WIDTH=256&HEIGHT=256"
#define WMS_MTC_1000	"http://shagrat.icc.es:80/lizardtech/iserv/ows?LAYERS=mtc1000m&EXCEPTIONS=application%2Fvnd.ogc.se_xml&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image%2Fpng&SRS=EPSG%3A23031&BBOX=412630,4638690,489430,4715490&WIDTH=256&HEIGHT=256"

//	Open Street Map

// Kompass
// http://ec2.cdn.ecmaps.de/WmsGateway.ashx.jpg?Experience=kompass&MapStyle=KOMPASS%20Touristik&TileX=17018&TileY=10975&ZoomLevel=15

/*
	{WMS_NRW_DE_10,	"geobasis/adv_dtk10", "DTK10", "png"},
	{WMS_NRW_DE_25,	"geobasis/adv_dtk25", "DTK25vi", "png"},
	{WMS_NRW_DE_50,	"wms/dlm50", "dlm_50", "png"},

// http://www.geodaten-mv.de/dienste/adv_dtk50?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=mv_dtk50&BBOX=4461245,5943634,4463517,5945149&SRS=EPSG:31468&FORMAT=image/jpeg&WIDTH=300&HEIGHT=200&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage
	{WMS_MV_DE_50K,		"dienste/adv_dtk50", "mv_dtk50", "png"},
	{WMS_MV_DE_25K,		"dienste/adv_dtk25", "mv_dtk25", "png"},
	{WMS_MV_DE_10K,		"dienste/adv_dtk10", "mv_dtk10", "png"},



	{WMS_DTK_50,		"ows2-m-pub/ows", "raster_gurs_pub:SI.GURS.RASTRI.DTK50", "png"},

	{WMS_HIGHLAND_25K,	"geoserver/wms", "osgb:25k", "png"},
	{WMS_HIGHLAND_50K,	"geoserver/wms", "osgb:50k", "png"},



// http://sg.geodatenzentrum.de/wms_dtk500?LAYERS=DTK500&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image%2Fjpeg&SRS=EPSG%3A31467&BBOX=3779760,5802440,3797680,5820360&WIDTH=256&HEIGHT=256
	{WMS_DE_BKG_200K, "wms_dtk200", "DTK200", "png"},
	{WMS_DE_BKG_500K, "wms_dtk500", "DTK500", "png"},
	{WMS_DE_BKG_1000K, "wms_dtk1000", "DTK1000", "png"},
// http://sg.geodatenzentrum.de/wms_d2500?LAYERS=d2500&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image%2Fjpeg&SRS=EPSG%3A31467&BBOX=3726000,5805000,3790000,5869000&WIDTH=256&HEIGHT=256"
	{WMS_DE_BKG_2500K, "wms_d2500", "d2500", "png"},

// http://www.idee.es/wms/MTN-Raster/MTN-Raster?LAYERS=mtn_rasterizado&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image%2Fjpeg&SRS=EPSG%3A23030&BBOX=446000,4410000,448000,4412000&WIDTH=200&HEIGHT=200"
	{WMS_IGN_ES_50K,	"wms/MTN-Raster/MTN-Raster", "mtn_rasterizado", "png"}
*/

