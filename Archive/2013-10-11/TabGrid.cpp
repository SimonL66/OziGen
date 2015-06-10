// TabGrid.cpp : implementation file
//

#include "stdafx.h"

#include "OziGen.h"
#include "TabGrid.h"

#include "OziGenDlg.h"

#include "..\MapLib\CoordConv.h"
#include "..\MapLib\Lambert93.h"
#include "..\MapLib\mmmath.h"

#include "func.h"
#include "Database.h"

#include "IGN.h"
#include "OSPro.h"

#include "CustomMap.h"

extern CCustomMap MyMap;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int long2tilex(double lon, int z);
extern int lat2tiley(double lat, int z);


/////////////////////////////////////////////////////////////////////////////
// CTabGrid dialog

CTabGrid::CTabGrid(CWnd* pParent /*=NULL*/)
	: CDialog(CTabGrid::IDD, pParent)
	, m_strGridRef(_T(""))
	, m_nGridRefInflate(10)
	, m_strGridExample(_T(""))
{
	//{{AFX_DATA_INIT(CTabGrid)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTabGrid::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabGrid)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_GRID_REF, m_strGridRef);
	DDX_Text(pDX, IDC_EDIT_GRID_REF_INFLATE, m_nGridRefInflate);
	DDX_Text(pDX, IDC_STATIC_EXAMPLE, m_strGridExample);
}


BEGIN_MESSAGE_MAP(CTabGrid, CDialog)
	//{{AFX_MSG_MAP(CTabGrid)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_EN_KILLFOCUS(IDC_EDIT_GRID_REF_INFLATE, &CTabGrid::OnEnKillfocusEditGridRefInflate)
	ON_EN_KILLFOCUS(IDC_EDIT_GRID_REF, &CTabGrid::OnEnKillfocusEditGridRef)
	ON_BN_CLICKED(IDOK, &CTabGrid::OnBnClickedOk)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_GRID, &CTabGrid::OnDeltaposSpinGrid)
	ON_BN_CLICKED(IDC_BUTTON_USE, &CTabGrid::OnBnClickedButtonUse)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabGrid message handlers

bool ExtractEastingNorthing(CString str, long & nEasting, long & nNorthing)
{
	CString strCoord1;
	CString strCoord2;

	int nPos = str.Find(",");

	if (nPos == -1) {
		return false;
	}

	strCoord1 = str.Left(nPos);
	strCoord2 = str.Mid(nPos+1);

	nEasting  = atoi(strCoord1);
	nNorthing = atoi(strCoord2);

	return true;
}

bool ExtractLatLon(CString str, double& dLat, double& dLon)
{
	CString strCoord1;
	CString strCoord2;

	int nPos = str.Find(",");

	if (nPos == -1) {
		return false;
	}

	strCoord1 = str.Left(nPos);
	strCoord2 = str.Mid(nPos+1);

	if (strCoord1.FindOneOf("nNsS") > 0) {

		// DM:  48°13.834'N, 3°20.787'W
		// DMS: 48°13'50.0"N, 3°20'47.2"W
		// Deg: 48.23056°N,  3.34645°W
		// Deg: 48.23056, -3.34645
		dLat = atof(strCoord1);
		if (strCoord1.Find("°") && strCoord1.Find("'")) {
			double dSeconds = 0.0;
			if (strCoord1.Find("\"")) {
				int nSecondPos = strCoord1.Find("'")+1;
				dSeconds = atof(strCoord1.Mid(nSecondPos));
			}
			int nMinutePos = strCoord1.Find("°")+1;
			double dMinutes = atof(strCoord1.Mid(nMinutePos));
			double dFraction = ((dMinutes * 60.0) + dSeconds) / 3600.0;
			dLat = dLat + dFraction;
		}
		if (strCoord1.FindOneOf("sS") > 0) {
			dLat = -dLat;
		}

	} else if (strCoord2.FindOneOf("nNsS") > 0) {
		dLat = atof(strCoord2);
		if (strCoord2.Find("°") && strCoord2.Find("'")) {
			double dSeconds = 0.0;
			if (strCoord2.Find("\"")) {
				int nSecondPos = strCoord2.Find("'")+1;
				dSeconds = atof(strCoord2.Mid(nSecondPos));
			}
			int nMinutePos = strCoord2.Find("°")+1;
			double dMinutes = atof(strCoord1.Mid(nMinutePos));
			double dFraction = ((dMinutes * 60.0) + dSeconds) / 3600.0;
			dLat = dLat + dFraction;
		}
		if (strCoord2.FindOneOf("sS") > 0) {
			dLat = -dLat;
		}
	} else {
		dLat = atof(strCoord1);
	}

	if (strCoord1.FindOneOf("eEwW") > 0) {
		dLon = atof(strCoord1);
		if (strCoord1.Find("°") && strCoord1.Find("'")) {
			double dSeconds = 0.0;
			if (strCoord1.Find("\"")) {
				int nSecondPos = strCoord1.Find("'")+1;
				dSeconds = atof(strCoord1.Mid(nSecondPos));
			}
			int nMinutePos = strCoord1.Find("°")+1;
			double dMinutes = atof(strCoord1.Mid(nMinutePos));
			double dFraction = ((dMinutes * 60.0) + dSeconds) / 3600.0;
			dLon = dLon + dFraction;
		}
		if (strCoord1.FindOneOf("wW") > 0) {
			dLon = -dLon;
		}
	} else if (strCoord2.FindOneOf("eEwW") > 0) {
		dLon = atof(strCoord2);
		if (strCoord2.Find("°") && strCoord2.Find("'")) {
			double dSeconds = 0.0;
			if (strCoord2.Find("\"")) {
				int nSecondPos = strCoord2.Find("'")+1;
				dSeconds = atof(strCoord2.Mid(nSecondPos));
			}
			int nMinutePos = strCoord2.Find("°")+1;
			double dMinutes = atof(strCoord2.Mid(nMinutePos));
			double dFraction = ((dMinutes * 60.0) + dSeconds) / 3600.0;
			dLon = dLon + dFraction;
		}
		if (strCoord2.FindOneOf("wW") > 0) {
			dLon = -dLon;
		}
	} else {
		dLon = atof(strCoord2);
	}

	return true;
}

bool ExtractGridRef(CString strGridRef, long& nEasting, long& nNorthing, int nCoordinateSystem, bool bLatLonSelected, CString& strErrorMsg)
{
	if (bLatLonSelected) {

		double dLat, dLon;

		if (!ExtractLatLon(strGridRef, dLat, dLon)) {
			strErrorMsg.Format("Error: Invalid Lat Lon %s",strGridRef);
			return false;
		}

		double dEasting;
		double dNorthing;

		OZIGEN_LatLon_to_NorthingEasting(dLat, dLon, &dNorthing, &dEasting);

		nEasting = (long) dEasting;
		nNorthing = (long) dNorthing;
		
	} else {

		if (!ExtractEastingNorthing(strGridRef, nEasting, nNorthing)) {
			strErrorMsg.Format("Error: Invalid Coord %s",strGridRef);
			return false;
		}
	}

	if ((MyMap.GetCountry() == COUNTRY_FR) && !IsInIGNDatabase(nNorthing, nEasting)) {
		strErrorMsg.Format("Error: the map for %s is not supported in the IGN Database",strGridRef);
		return false;
	}

	if ((MyMap.GetCountry() == COUNTRY_UK) && !IsInOSGBDatabase(nNorthing, nEasting)) {
		strErrorMsg.Format("Error: the map for %s is not supported in the OSGB Database",strGridRef);
		return false;
	}

	return true;
}

void CTabGrid::OnBnClickedOk()
{
	UpdateData(true);

COziGenDlg	* pParent;
pParent = (COziGenDlg *)(this->GetParent()->GetParent());

	if (MyMap.GetDetail() == -1) {
		pParent->AddToResultsWindow("Error: Invalid Map Scale");
		return;
	}

	long easting=0, northing=0;


	bool bLatLonSelected =  (pParent->m_nCoordSystem == COORD_SYSTEM_LAT_LON);
	CString strErrorMessage;

	if (!ExtractGridRef(m_strGridRef, easting, northing, MyMap.GetCoordinateSystem(), bLatLonSelected, strErrorMessage))
	{
		pParent->AddToResultsWindow(strErrorMessage);
		return;
	}

//	long eMin, nMin, eMax, nMax;
	CRect MyRect; /*= CRect(eMin, nMax, eMax, nMin);*/

	if ((MyMap.GetProjection() == 3857) || (MyMap.GetProjection() == 900913)) {

		MyRect.left = easting;
		MyRect.top = (int)(northing+0.9)/1;
		MyRect.right = (int)(easting+0.9)/1;
		MyRect.bottom = northing;

		if (m_nGridRefInflate > 0) {
			MyRect.InflateRect(m_nGridRefInflate, -m_nGridRefInflate);
		}

		if (MyRect.right == MyRect.left) MyRect.right++;
		if (MyRect.top == MyRect.bottom) MyRect.top++;

	} else {

		int nMeters = MyMap.GetMetresPerTile();
		if (nMeters < 1000)
			nMeters = 1000;

		MyRect.left = easting;
		MyRect.top = northing;
		MyRect.right = easting;
		MyRect.bottom = northing;
/*
		MyRect.left = easting/nMeters*nMeters;
		MyRect.top = (northing+nMeters-1)/nMeters*nMeters;
		MyRect.right = (easting+nMeters-1)/nMeters*nMeters;
		MyRect.bottom = northing/nMeters*nMeters;
*/
		if (m_nGridRefInflate > 0) {
			MyRect.InflateRect(m_nGridRefInflate * 1000, -m_nGridRefInflate * 1000);
		}

		MyRect.left = MyRect.left/nMeters*nMeters;
		MyRect.top = (MyRect.top+nMeters-1)/nMeters*nMeters;
		MyRect.right = (MyRect.right+nMeters-1)/nMeters*nMeters;
		MyRect.bottom = MyRect.bottom/nMeters*nMeters;
	}

	pParent->m_mapBoundaries = MyRect;

	pParent->DisplayGridRef(MyRect);
	pParent->DisplayMapData();

	pParent->UpdateData(true);

	CString str;
/*
	str.Format("%s%s_%s_%d_%d",
		MyMap.GetInternetCode(),MyMap.GetMapScaleText(pParent->m_nDetail),MyMap.GetMapFlavourText(pParent->m_nCountry,pParent->m_nProduct),easting,northing);
*/
	pParent->m_strMapName = MyMap.GetMapName(MyRect);
	pParent->UpdateData(false);

	pParent->AddToResultsWindow("Determined Map Extents for Grid Ref. %s",m_strGridRef);
}

void CTabGrid::OnEnKillfocusEditGridRefInflate()
{
	UpdateData(true);
	AfxGetApp()->WriteProfileInt("Main", "GridRefInflation", m_nGridRefInflate);
	UpdateData(false);
}


void CTabGrid::OnEnKillfocusEditGridRef()
{
COziGenDlg	* pParent;
pParent = (COziGenDlg *)(this->GetParent()->GetParent());

UpdateData(true);
//	AfxGetApp()->WriteProfileString("Main", "GridReference", m_strGridRef);
	AfxGetApp()->WriteProfileString("Main", MyMap.GetRegistryValueName(), m_strGridRef);
	MyMap.WriteProfile(m_strGridRef);
	UpdateData(false);
}

void CTabGrid::UpdateGridExample()
{
	COziGenDlg	* pParent;
	pParent = (COziGenDlg *)(this->GetParent()->GetParent());

	if (pParent->m_nCoordSystem == COORD_SYSTEM_LAT_LON) {

		m_strGridExample = MyMap.GetExampleLatLon();

	} else if (pParent->m_nCoordSystem == COORD_SYSTEM_GRID) {

		double dLat, dLon;

		if (ExtractLatLon(MyMap.GetExampleLatLon(), dLat, dLon)) {

			double dEasting;
			double dNorthing;

			OZIGEN_LatLon_to_NorthingEasting(dLat, dLon, &dNorthing, &dEasting);

			m_strGridExample.Format("%d,%d", (long) dEasting, (long) dNorthing);
		}
	}

	UpdateData(false);
}

void CTabGrid::UpdateGridReference()
{
COziGenDlg	* pParent;
pParent = (COziGenDlg *)(this->GetParent()->GetParent());

	long easting=0, northing=0;

	if (MyMap.GetCountry() == COUNTRY_UK)  {

		if (pParent->m_nPreviousCoordsSystem == COORD_SYSTEM_LAT_LON) {
			
			if ((pParent->m_nCoordSystem) == COORD_SYSTEM_GRID) {

				// Switch from Lat/Lon to OS Grid.

				double dLat, dLon;
				if (!ExtractLatLon(m_strGridRef, dLat, dLon)) {
					pParent->AddToResultsWindow("Error: Invalid Lat Lon %s",m_strGridRef);
					return;
				}

				char buffer[256];
				latitude_longitude_to_OSref5(dLat, dLon, buffer);
				m_strGridRef = buffer;
			}

		} else {

			if (pParent->m_nCoordSystem == COORD_SYSTEM_LAT_LON) {

				long easting, northing;
				OSref_to_easting_northing(m_strGridRef, &easting, &northing);
				double lat, lon;
				easting_northing_to_latitude_longitude(easting, northing, &lat, &lon);
				m_strGridRef.Format("%f, %f",lat,lon);
			}
		}

	} else if (MyMap.GetCountry() == COUNTRY_FR) {

		if (pParent->m_nPreviousCoordsSystem == COORD_SYSTEM_LAT_LON) {

			if ((pParent->m_nCoordSystem) == COORD_SYSTEM_GRID) {

				double dLat, dLon;

				if (!ExtractLatLon(m_strGridRef, dLat, dLon)) {
					pParent->AddToResultsWindow("Error: Invalid Lat Lon %s",m_strGridRef);
					return;
				}

				double d_northing;
				double d_easting;
			
				WGS84ToLambert93(dLat, dLon, &d_northing, &d_easting);

				northing = (long) d_northing;
				easting = (long) d_easting;

				m_strGridRef.Format("%07d, %06d", easting, northing);
	
			} 
		
		} else {
			
			if (pParent->m_nCoordSystem == COORD_SYSTEM_LAT_LON) {
				if (!ExtractEastingNorthing(m_strGridRef, easting, northing)) {
					pParent->AddToResultsWindow("Error: Invalid Lambert93 Coord %s",m_strGridRef);
					return;
				}

				double dLat;
				double dLon;
				Lambert93ToWGS84(northing, easting, &dLat, &dLon);

				m_strGridRef.Format("%f, %f", dLat,dLon);
			}
		}
	} else if (MyMap.GetCountry() == COUNTRY_BE) {

		if (pParent->m_nPreviousCoordsSystem == COORD_SYSTEM_LAT_LON) {

			if ((pParent->m_nCoordSystem) == COORD_SYSTEM_GRID) {

				double dLat, dLon;

				if (!ExtractLatLon(m_strGridRef, dLat, dLon)) {
					pParent->AddToResultsWindow("Error: Invalid Lat Lon %s",m_strGridRef);
					return;
				}

				double d_northing;
				double d_easting;
			
				WGS84ToLambertBelgium2008(dLat, dLon, &d_northing, &d_easting);

				northing = (long) d_northing;
				easting = (long) d_easting;

				m_strGridRef.Format("%07d, %06d", easting, northing);
	
			} 
		
		} else {
			
			if (pParent->m_nCoordSystem == COORD_SYSTEM_LAT_LON) {
				if (!ExtractEastingNorthing(m_strGridRef, easting, northing)) {
					pParent->AddToResultsWindow("Error: Invalid Lambert93 Coord %s",m_strGridRef);
					return;
				}

				double dLat;
				double dLon;

				LambertBelgium2008ToWGS84(northing, easting, &dLat, &dLon);

				m_strGridRef.Format("%f, %f", dLat,dLon);
			}
		}
	}
	AfxGetApp()->WriteProfileString("Main", MyMap.GetRegistryValueName(), m_strGridRef);
	UpdateData(false);
}

void CTabGrid::OnDeltaposSpinGrid(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	UpdateData(true);

	int nTargetInflate = m_nGridRefInflate - pNMUpDown->iDelta;

	if (nTargetInflate >= 0) {
		m_nGridRefInflate = nTargetInflate;
		AfxGetApp()->WriteProfileInt("Main", "GridRefInflation", m_nGridRefInflate);
		UpdateData(false);
	}

	*pResult = 0;
}

void CTabGrid::OnBnClickedButtonUse()
{
	COziGenDlg	* pParent;
	pParent = (COziGenDlg *)(this->GetParent()->GetParent());

	if (pParent->m_nCoordSystem == COORD_SYSTEM_GRID)		m_strGridRef = MyMap.GetExampleGrid();
	if (pParent->m_nCoordSystem == COORD_SYSTEM_LAT_LON)	m_strGridRef = MyMap.GetExampleLatLon();

	MyMap.WriteProfile(m_strGridRef);

	UpdateData(false);
}

/*
		double dLat, dLon;

		if (!ExtractLatLon(m_strGridRef, dLat, dLon)) {
			return;
		}

		int z = 15;
		int x = long2tilex(dLon, z);
		int y = lat2tiley(dLat, z);
*/

/*
	CString buffer;
	FILE * fp1 = fopen("si.rxf", "w+");
	if (fp1 == NULL) {
		printf("Couldn't Create Maptech Route\n");
		return;
	}

	FILE * fp2 = fopen("si.txt", "w+");
	if (fp2 == NULL) {
		printf("Couldn't Create Text File\n");
		return;
	}

	int nO = 644000;
	int eO = 747000;

	for (int n=2; n > 0; n-- ) {
		for (int e=0; e < 2; e++) {

			int routeX[5];
			int routeY[5];

			routeX[0] = (e*1000)+eO;		routeY[0] = (n*1000)+nO;
			routeX[1] = (e*1000)+eO+1000;	routeY[1] = (n*1000)+nO;
			routeX[2] = (e*1000)+eO+1000;	routeY[2] = (n*1000)+nO+1000;
			routeX[3] = (e*1000)+eO;		routeY[3] = (n*1000)+nO+1000;
			routeX[4] = (e*1000)+eO;		routeY[4] = (n*1000)+nO;

			double lat, lon;
			for (int i=0; i < 5; i++) {
				int c;
				LambertBelgium2008ToWGS84(routeY[i], routeX[i], &lat, &lon);
				c = (i == 4) ? 0 : i;
				buffer.Format("%f, %f, \"%d.%d.%1d\", \"\", \"\", \"%d.%d\", ff0000, 0, 37\n",
								lat, lon,
								n, e, c,
								n, e);
				
				fprintf(fp1,buffer);
			}
			buffer.Format("http://www.ngi.be/topomapviewer/services/NGI_map/RD_TOP50R_CACHE/MapServer/tile/7/%d/%d\n", ((e*1000+eO)/10)-50325, ((n*1000+nO)/10)-43270);
			fprintf(fp2,buffer);
		}
	}
	fclose(fp1);
	fclose(fp2);
*/

