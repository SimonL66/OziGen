#pragma once

// e.g. http://<hostname>/<deploy_name>/com.esri.wms.Esrimap?VERSION=1.1.1&REQUEST=GetCapabilities&SERVICE=WMS&  

//	Austria

//	Belgium
#define WMS_NGI_BE_10	"http://www.ngi.be/testbed/wms/top10r_l08_fr?LAYERS=0&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image%2Fpng&SRS=EPSG%3A3812&BBOX=658799.999,632559.8865,659439.999,633199.8865&WIDTH=256&HEIGHT=256"
#define WMS_NGI_BE_50	"http://www.ngi.be/testbed/wms/top50r_l08_fr?LAYERS=0&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image%2Fpng&SRS=EPSG%3A3812&BBOX=658799.999,632559.8865,659439.999,633199.8865&WIDTH=256&HEIGHT=256"
#define WMS_NGI_BE_100	"http://www.ngi.be/testbed/wms/top100r_l08_fr?LAYERS=0&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image%2Fpng&SRS=EPSG%3A3812&BBOX=658799.999,632559.8865,659439.999,633199.8865&WIDTH=256&HEIGHT=256"

//	Britain
#define WMS_OS_10K	"http://osopenspacepro.ordnancesurvey.co.uk/osmapapi/ts?FORMAT=image%2Fpng&KEY=DEMOAPI&URL=http%3A%2F%2Fosopenspacewiki.ordnancesurvey.co.uk%2Fwiki%2Fextensions%2Fhtmlets%2FOSProCS.html&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&EXCEPTIONS=application%2Fvnd.ogc.se_inimage&LAYERS=1&PRODUCT=10K&SRS=EPSG%3A27700&BBOX=258000,667500,258500,668000&WIDTH=200&HEIGHT=200"
#define WMS_OS_25K	"http://osopenspacepro.ordnancesurvey.co.uk/osmapapi/ts?FORMAT=image%2Fpng&KEY=DEMOAPI&URL=http%3A%2F%2Fosopenspacewiki.ordnancesurvey.co.uk%2Fwiki%2Fextensions%2Fhtmlets%2FOSProCS.html&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&EXCEPTIONS=application%2Fvnd.ogc.se_inimage&LAYERS=2.5&PRODUCT=25K&SRS=EPSG%3A27700&BBOX=258000,667500,258500,668000&WIDTH=200&HEIGHT=200"
#define WMS_OS_50K	"http://osopenspacepro.ordnancesurvey.co.uk/osmapapi/ts?FORMAT=image%2Fpng&KEY=DEMOAPI&URL=http%3A%2F%2Fosopenspacewiki.ordnancesurvey.co.uk%2Fwiki%2Fextensions%2Fhtmlets%2FOSProCS.html&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&EXCEPTIONS=application%2Fvnd.ogc.se_inimage&LAYERS=5&PRODUCT=50K&SRS=EPSG%3A27700&BBOX=258000,667500,258500,668000&WIDTH=200&HEIGHT=200"
#define WMS_OS_250K	"http://osopenspacepro.ordnancesurvey.co.uk/osmapapi/ts?FORMAT=image%2Fpng&KEY=DEMOAPI&URL=http%3A%2F%2Fosopenspacewiki.ordnancesurvey.co.uk%2Fwiki%2Fextensions%2Fhtmlets%2FOSProCS.html&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&EXCEPTIONS=application%2Fvnd.ogc.se_inimage&LAYERS=25&PRODUCT=250K&SRS=EPSG%3A27700&BBOX=258000,667500,258500,668000&WIDTH=200&HEIGHT=200"
#define WMS_OS_MS	"http://osopenspacepro.ordnancesurvey.co.uk/osmapapi/ts?FORMAT=image%2Fpng&KEY=DEMOAPI&URL=http%3A%2F%2Fosopenspacewiki.ordnancesurvey.co.uk%2Fwiki%2Fextensions%2Fhtmlets%2FOSProCS.html&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&EXCEPTIONS=application%2Fvnd.ogc.se_inimage&LAYERS=100&PRODUCT=MS&SRS=EPSG%3A27700&BBOX=258000,667500,258500,668000&WIDTH=200&HEIGHT=200"

#define WMS_OS_VML	"http://osopenspacepro.ordnancesurvey.co.uk/osmapapi/ts?FORMAT=image%2Fpng&KEY=DEMOAPI&URL=http%3A%2F%2Fosopenspacewiki.ordnancesurvey.co.uk%2Fwiki%2Fextensions%2Fhtmlets%2FOSProCS.html&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&EXCEPTIONS=application%2Fvnd.ogc.se_inimage&LAYERS=1&PRODUCT=VML&SRS=EPSG%3A27700&BBOX=258000,667500,258500,668000&WIDTH=200&HEIGHT=200"
#define WMS_OS_VMD	"http://osopenspacepro.ordnancesurvey.co.uk/osmapapi/ts?FORMAT=image%2Fpng&KEY=DEMOAPI&URL=http%3A%2F%2Fosopenspacewiki.ordnancesurvey.co.uk%2Fwiki%2Fextensions%2Fhtmlets%2FOSProCS.html&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&EXCEPTIONS=application%2Fvnd.ogc.se_inimage&LAYERS=2.5&PRODUCT=VMD&SRS=EPSG%3A27700&BBOX=258000,667500,258500,668000&WIDTH=200&HEIGHT=200"

#define WMS_OS_SV	"http://osopenspacepro.ordnancesurvey.co.uk/osmapapi/ts?FORMAT=image%2Fpng&KEY=DEMOAPI&URL=http%3A%2F%2Fosopenspacewiki.ordnancesurvey.co.uk%2Fwiki%2Fextensions%2Fhtmlets%2FOSProCS.html&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&EXCEPTIONS=application%2Fvnd.ogc.se_inimage&LAYERS=1&PRODUCT=SV&SRS=EPSG%3A27700&BBOX=258000,667500,258500,668000&WIDTH=200&HEIGHT=200"

#define WMS_HIGHLAND_25	"http://www.walkhighlands.co.uk:8080/geoserver/wms?LAYERS=osgb:25k&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image/png&SRS=EPSG:27700&URL=http%3A%2F%2Fwww.walkhighlands.co.uk%2F&BBOX=485000,492000,486000,493000&WIDTH=200&HEIGHT=200"
#define WMS_HIGHLAND_50	"http://www.walkhighlands.co.uk:8080/geoserver/wms?LAYERS=osgb:50k&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image/png&SRS=EPSG:27700&URL=http%3A%2F%2Fwww.walkhighlands.co.uk%2F&BBOX=485000,492000,486000,493000&WIDTH=200&HEIGHT=200"

#define WMS_GETMAPPING_125	"http://www.getmapping.com/preview/preview.ashx?X=486463.0&Y=496681.5&size=125.0&productID=9&dataSet=MM"
#define WMS_GETMAPPING_250	"http://www.getmapping.com/preview/preview.ashx?X=486463.0&Y=496681.5&size=250.0&productID=9&dataSet=MM"
#define WMS_GETMAPPING_500	"http://www.getmapping.com/preview/preview.ashx?X=486463.0&Y=496681.5&size=500.0&productID=9&dataSet=MM"
#define WMS_GETMAPPING_1000	"http://www.getmapping.com/preview/preview.ashx?X=486463.0&Y=496681.5&size=1000.0&productID=9&dataSet=MM"
#define WMS_GETMAPPING_2000	"http://www.getmapping.com/preview/preview.ashx?X=486463.0&Y=496681.5&size=2000.0&productID=9&dataSet=MM"

// http://osopenspacepro.ordnancesurvey.co.uk/osmapapi/ts?FORMAT=image%2Fpng&KEY=A493C3EB96133019E0405F0ACA6056E3&URL=http%3A%2F%2Fwww.geograph.org.uk%2Fshowmap.php%3Fgridref%3DSE65239512&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&EXCEPTIONS=application%2Fvnd.ogc.se_inimage&LAYERS=1&PRODUCT=VML&SRS=EPSG%3A27700&BBOX=465250,495250,465500,495500&WIDTH=250&HEIGHT=250
// http://osopenspacepro.ordnancesurvey.co.uk/osmapapi/ts?FORMAT=image%2Fpng&KEY=A493C3EB96133019E0405F0ACA6056E3&URL=http%3A%2F%2Fwww.geograph.org.uk%2Fshowmap.php%3Fgridref%3DSE65239512&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&EXCEPTIONS=application%2Fvnd.ogc.se_inimage&LAYERS=2.5&PRODUCT=VMD&SRS=EPSG%3A27700&BBOX=465000,495500,465500,496000&WIDTH=200&HEIGHT=200
// http://osopenspacepro.ordnancesurvey.co.uk/osmapapi/ts?FORMAT=image%2Fpng&KEY=DEMOAPI&URL=http%3A%2F%2Fosopenspacewiki.ordnancesurvey.co.uk%2Fwiki%2Fextensions%2Fhtmlets%2FOSProCS.html&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&EXCEPTIONS=application%2Fvnd.ogc.se_inimage&LAYERS=2.5&PRODUCT=25K&SRS=EPSG%3A27700&BBOX=258000,667500,258500,668000&WIDTH=200&HEIGHT=200

// https://osopenspacepro.ordnancesurvey.co.uk/osmapapi/ts?FORMAT=image%2Fpng&KEY=6694613F8B469C97E0405F0AF160360A&URL=http%3A%2F%2Fwww.ordnancesurvey.co.uk%2Fbusiness-and-government%2Fproducts%2Fos-openspace%2Fpro%2Findex.html&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&LAYERS=1.75&PRODUCT=CS09&SRS=EPSG%3A27700&BBOX=315000,682625,315875,683500&WIDTH=250&HEIGHT=250

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

#define WMS_MV_DTK10	"http://www.geodaten-mv.de/dienste/adv_dtk10?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=mv_dtk10&BBOX=4461349,5943902,4461708,5944140&SRS=EPSG:31468&FORMAT=image/png&WIDTH=300&HEIGHT=200&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage"
#define WMS_MV_DTK25	"http://www.geodaten-mv.de/dienste/adv_dtk25?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=mv_dtk25&BBOX=4482102,5997652,4483095,5998314&SRS=EPSG:31468&FORMAT=image/png&WIDTH=300&HEIGHT=200&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage"
#define WMS_MV_DTK50	"http://www.geodaten-mv.de/dienste/adv_dtk50?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=mv_dtk50&BBOX=4461245,5943634,4463517,5945149&SRS=EPSG:31468&FORMAT=image/png&WIDTH=300&HEIGHT=200&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage"
#define WMS_MV_DTK100	"http://www.geodaten-mv.de/dienste/adv_dtk100?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=mv_dtk100&BBOX=4465029,5946945,4467951,5948893&SRS=EPSG:31468&FORMAT=image/png&WIDTH=300&HEIGHT=200&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage"
#define WMS_MV_DTK250	"http://www.geodaten-mv.de/dienste/gdimv_uek250?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=gdimv_uek250&BBOX=4457891,5940891,4466007,5946302&SRS=EPSG:31468&FORMAT=image/png&WIDTH=300&HEIGHT=200&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage"
#define WMS_MV_DTK750	"http://www.geodaten-mv.de/dienste/gdimv_uek750?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=gdimv_uek750&BBOX=4457749,5932873,4477382,5945962&SRS=EPSG:31468&FORMAT=image/png&WIDTH=300&HEIGHT=200&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage"
#define WMS_MV_DTK1000	"http://www.geodaten-mv.de/dienste/gdimv_uek1000?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=gdimv_uek1000&BBOX=4444333,5934008,4477055,5954409&SRS=EPSG:31468&FORMAT=image/png&WIDTH=300&HEIGHT=200&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage"

// Other MV Maps...

// Offene RegionalKarte MV (ORKa-MV)
// "http://geo.sv.rostock.de/geodienste/stadtplan/wms?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=stadtplan&BBOX=4444628,5959658,4471448,5977478&SRS=EPSG:31468&FORMAT=image/png&WIDTH=300&HEIGHT=200&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage"

// TopoMV - ma�stabsunabh�ngige Webkarte
// "http://www.geodaten-mv.de/dienste/gdimv_topomv?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=gdimv_topomv&BBOX=4460609,5943243,4464396,5945605&SRS=EPSG:31468&FORMAT=image/jpeg&WIDTH=300&HEIGHT=200&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage"
// "http://www.geodaten-mv.de/dienste/gdimv_topomv?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=gdimv_topomv&BBOX=4460609,5943243,4464396,5945605&SRS=EPSG:31468&FORMAT=image/jpeg&WIDTH=300&HEIGHT=200&STYLES=grau&EXCEPTIONS=application/vnd.ogc.se_inimage"

// Kombinierte Digitale Topographische Karten und Gebietskarten 1:10.000 - 1:1.000.000
// "http://www.geodaten-mv.de/dienste/gdimv_dtk?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=gdimv_dtk&BBOX=4461349,5943902,4461708,5944140&SRS=EPSG:31468&FORMAT=image/jpeg&WIDTH=300&HEIGHT=200&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage"
// "http://www.geodaten-mv.de/dienste/gdimv_dtk?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=gdimv_dtk&BBOX=4461349,5943902,4461708,5944140&SRS=EPSG:31468&FORMAT=image/jpeg&WIDTH=300&HEIGHT=200&STYLES=grau&EXCEPTIONS=application/vnd.ogc.se_inimage"

// Historische Topographische Karte 1:25.000 1990-1996 (Normalblattschnitt)
// "http://www.geodaten-mv.de/dienste/tk25n_laiv_wms?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=mv_tk25n&BBOX=4482102,5997652,4483095,5998314&SRS=EPSG:31468&FORMAT=image/jpeg&WIDTH=300&HEIGHT=200&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage"

// Historische Topographische Karte 1:25.000 1981-1987 (Ausgabe Staat)
// "http://www.geodaten-mv.de/dienste/tk25as_laiv_wms?REQUEST=GetMap&VERSION=1.1.1&SERVICE=WMS&LAYERS=mv_tk25as&BBOX=4482102,5997652,4483095,5998314&SRS=EPSG:31468&FORMAT=image/jpeg&WIDTH=300&HEIGHT=200&STYLES=&EXCEPTIONS=application/vnd.ogc.se_inimage"

// Historische Topographische Karte 1:25.000 1877-1889 (Messtischbl�tter)
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
#define WMS_OSM				"http://tile.openstreetmap.org/18/130584/83719.png"
#define WMS_OSM_CYCLE		"http://tile.thunderforest.com/cycle/12/2036/1308.png"
#define WMS_OSM_TRANSPORT	"http://tile.thunderforest.com/transport/12/2035/1304.png"
#define WMS_OSM_LANDSCAPE	"http://tile.thunderforest.com/landscape/12/2036/1305.png"
#define WMS_OSM_OUTDOOR		"http://tile.thunderforest.com/outdoors/18/130584/83719.png"



// Kompass
#define WMS_Kompass		"http://ec2.cdn.ecmaps.de/WmsGateway.ashx.jpg?Experience=kompass&MapStyle=KOMPASS%20Touristik&TileX=17018&TileY=10975&ZoomLevel=15"

// Finland
// "http://tiles.kartat.kapsi.fi/ortokuva?FORMAT=image/jpeg&VERSION=1.1.1&SERVICE=WMS&REQUEST=GetMap&STYLES=&SRS={proj}&WIDTH={width}&HEIGHT={height}&BBOX={bbox}"
#define WMS_FI_50	"http://tiles.kartat.kapsi.fi/peruskartta?FORMAT=image/png&&LAYERS=peruskartta&VERSION=1.1.1&SERVICE=WMS&REQUEST=GetMap&STYLES=&SRS=EPSG:3067&WIDTH=256&HEIGHT=256&BBOX=349250,6938440,351250,6940440"

/*
https://ws.nls.fi/rasteriaineistot/image?
SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&BGCOLOR=0xffffff&
STYLES=painovari&WIDTH=500&HEIGHT=500&SRS=EPSG:3067&TRANSPARENT=false&
BBOX=349250,6938440,351250,6940440&LAYERS=peruskartta&FORMAT=image/png
*/

// Bing
#define WMS_BING	"http://a1.ortho.tiles.virtualearth.net/tiles/a031311321031011.jpeg?g=52"

// SatMap
// http://tileserver.satmap.com/AT/14/12023002320313.ipic
#define WMS_SATMAP	"http://tileserver.satmap.com/AT/14/12023002320313.ipic"


//#define SATMAP_AP http://ecn.t0.tiles.virtualearth.net/tiles/h1202300312030.jpeg?g=1711&mkt=en-US&n=z
//#define SATMAP_AP http://ecn.t3.tiles.virtualearth.net/tiles/h1202300303113.jpeg?g=1711&mkt=en-US&n=z
//#define SATMAP_AP http://ecn.t1.tiles.virtualearth.net/tiles/h1202300303311.jpeg?g=1711&mkt=en-US&n=z
//#define SATMAP_AP http://ecn.t2.tiles.virtualearth.net/tiles/h1202300312032.jpeg?g=1711&mkt=en-US&n=z

//#define SATMAP_OS - http://s3-eu-west-1.amazonaws.com/ostiles/031311331.ipic
//#define SATMAP_OCM - http://tile.opencyclemap.org/cycle/13/3945/2600.png
//#define SATMAP_OSM - http://tile.openstreetmap.org/13/3943/2600.png
//#define SATMAP_AT/DE (AV Mapping) - http://s3-eu-west-1.amazonaws.com/davtiles/120230023302322.ipic


#define SATMAP_AT	"http://tileserver.satmap.com/AT/13/1202300303311.ipic"
#define SATMAP_BE	"http://tileserver.satmap.com/BE/14/12020231021132.ipic"
#define SATMAP_DE	"http://tileserver.satmap.com/DE/13/1202031303130.ipic"
#define SATMAP_IT	"http://tileserver.satmap.com/IT/13/1202302022002.ipic"
#define SATMAP_NL	"http://tileserver.satmap.com/NL/13/1202021112121.ipic"
#define SATMAP_NI	"http://tileserver.satmap.com/NI/13/0313101300333.ipic"
#define SATMAP_IE	"http://tileserver.satmap.com/IE/14/03131032003120.ipic"
#define SATMAP_ES	"http://tileserver.satmap.com/ES/10/0331121110.ipic"
#define SATMAP_CH	"http://tileserver.satmap.com/CH/12/120221300023.ipic"

#define SATMAP_FR	"http://s3-eu-west-1.amazonaws.com/france-100k/1202202132320.ipic"

#define SATMAP_RSA	"http://s3-eu-west-1.amazonaws.com/south-africa-50k/30030100112300.ipic"

#define SATMAP_BEV	"http://s3-eu-west-1.amazonaws.com/davtiles/120230023302322.ipic"


#define WMS_KRAK_10		"http://webmapservice.krak.dk/wms/krakbykort?kwuid=demo@krakwms.dk&kwpwd=uyu8tSdD&nbr&request=GetMap&VERSION=1.1.1&service=WMS&bbox=705576,6203672,705832,6203928&format=image/png&width=256&height=256&LAYERS=krakbykort&STYLES=&BGCOLOR=0xFFFFFF&TRANSPARENT=TRUE&SRS=EPSG:25832"
#define WMS_KRAK_100	"http://webmapservice.krak.dk/wms/krakregkort?kwuid=demo@krakwms.dk&kwpwd=uyu8tSdD&nbr&request=GetMap&VERSION=1.1.1&service=WMS&bbox=699688,6202904,701736,6204952&format=image/png&width=256&height=256&LAYERS=krakregkort&STYLES=&BGCOLOR=0xFFFFFF&TRANSPARENT=TRUE&SRS=EPSG:25832"
#define WMS_KRAK_1000	"http://webmapservice.krak.dk/wms/krakoversigtkort?kwuid=demo@krakwms.dk&kwpwd=uyu8tSdD&nbr&request=GetMap&VERSION=1.1.1&service=WMS&bbox=656680,6196760,673064,6213144&format=image/png&width=256&height=256&LAYERS=krakoversigtkort&STYLES=&BGCOLOR=0xFFFFFF&TRANSPARENT=TRUE&SRS=EPSG:25832"


#define WMS_SE			"http://kso.lantmateriet.se/wmsproxy/wmsproxy?LAYERS=terrangkartan&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image%2Fjpeg&SRS=EPSG%3A3006&BBOX=310592,6458400,311616,6459424&WIDTH=250&HEIGHT=250"

#define WMS_HR			"http://geoportal.dgu.hr/wms?LAYERS=TK25&FORMAT=image%2Fpng&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&SRS=EPSG%3A3765&BBOX=430337.85110505,4941446.3609969,434927.05110505,4943507.1609969&WIDTH=1639&HEIGHT=736"

#define WMS_IT			"http://wms.pcn.minambiente.it/ogc?MAP=/ms_ogc/WMS_v1.3/raster/IGM_25000.map&SERVICE=WMS&REQUEST=GetMap&FORMAT=image/png&VERSION=1.1.1&LAYERS=CB.IGM25000.32&BBOX=55500,4972000,56500,4973000&SRS=EPSG:32633&WIDTH=400&HEIGHT=400"
//#define WMS_IT			"http://wms.pcn.minambiente.it/ogc?map=/ms_ogc/WMS_v1.3/raster/IGM_25000.map&service=WMS&request=GetMap&format=image/png&version=1.1.1&layers=CB.IGM25000.32&bbox=55500,4972000,56500,4973000&srs=EPSG:32633&width=400&height=400"

#define WMS_KORT	"http://kortforsyningen.kms.dk/print_topo25?TICKET=04ffe1ee1b9c2794dd045f556b54333a&LAYERS=dtk_kort25_klassisk_2008_254dpi&TRANSPARENT=TRUE&STYLE=default&SERVICE=WMS&VERSION=1.1.1&REQUEST=GetMap&STYLES=&FORMAT=image%2Fpng&SRS=EPSG%3A25832&BBOX=559687.5,6200000,560625,6200937.5&WIDTH=256&HEIGHT=256"

#define WMS_IGN		"http://gpp3-wxs.ign.fr/sg68l6zf6zemkg3cdr0bknay/wmts?SERVICE=WMTS&REQUEST=GetTile&VERSION=1.0.0&LAYER=GEOGRAPHICALGRIDSYSTEMS.MAPS&STYLE=normal&TILEMATRIXSET=PM&TILEMATRIX=16&TILEROW=23149&TILECOL=33916&FORMAT=image/jpeg"

#define WMTS_LUX		"http://tile2.geoportail.lu/topo_gr/08/000/000/154/000/000/195.png"


#define WMTS_NGI		"http://www.ngi.be/cartoweb/1.0.0/topo/default/3812/7/116/162.png"

//#define WMS_CH		"http://wmts1.geo.admin.ch/1.0.0/ch.swisstopo.pixelkarte-farbe/default/20130213/21781/23/359/494.jpeg"

#define WMTS_CH		"http://wmts2.geo.admin.ch/1.0.0/ch.swisstopo.swisstlm3d-karte-farbe/default/20130401/21781/26/1302/1877.png"

//http://wmts2.geo.admin.ch/1.0.0/ch.swisstopo.swisstlm3d-karte-farbe/default/20130401/21781/26/1302/1877.png
// http://wmts0.geo.admin.ch/1.0.0/ch.swisstopo.pixelkarte-farbe/default/20130903/21781/21/128/184.jpeg
// http://wmts0.geo.admin.ch/1.0.0/ch.swisstopo.pixelkarte-farbe/default/20130903/21781/21/128/184.jpeg

#define WMTS_OS_OLD_25K "http://geo.nls.uk/mapdata2/os/25000/15/16259/21897.png"
#define WMTS_OS_HISTORICAL "http://t4.uk.tileserver.com/_os1/r0/14/8130/5434.jpg"
#define WMTS_OS_1930 "http://tiles.snaffle.me.uk/03/tqs/rsr/srr/ttt/qrq.jpg"

#define WMTS_OS_NPE	"http://a.ooc.openstreetmap.org/npe/13/4067/2625.png"
#define WMTS_OS_1st	"http://a.ooc.openstreetmap.org/os1/13/4066/2624.jpg"
#define WMTS_OS_7th	"http://a.ooc.openstreetmap.org/os7/13/4069/2624.jpg"

#define WMTS_ARCCIS	"http://services.arcgisonline.com/ArcGIS/rest/services/World_Topo_Map/MapServer/tile/18/84212/130283"

#define REFER_NULL	""
#define REFER_IGN	"http://maps.peterrobins.co.uk/f/m.html"
#define REFER_LUX	"http://map.geoportail.lu"
#define REFER_NGI	"http://www.ngi.be"

#define REFER_CH1903	"http://wmts1.geo.admin.ch"

#define REFER_HIGHLAND	"http://www.walkhighlands.co.uk:8080"



