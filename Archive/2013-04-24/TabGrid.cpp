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

#include "GetIGN.h"
#include "GetOSPro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabGrid dialog


CTabGrid::CTabGrid(CWnd* pParent /*=NULL*/)
	: CDialog(CTabGrid::IDD, pParent)
	, m_strGridRef(_T(""))
	, m_nGridRefInflate(0)
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
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabGrid message handlers

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
		dLat = atof(strCoord1);
		if (strCoord1.FindOneOf("sS") > 0) {
			dLat = -dLat;
		}
	} else if (strCoord2.FindOneOf("nNsS") > 0) {
		dLat = atof(strCoord2);
		if (strCoord2.FindOneOf("sS") > 0) {
			dLat = -dLat;
		}
	} else {
		dLat = atof(strCoord1);
	}

	if (strCoord1.FindOneOf("eEwW") > 0) {
		dLon = atof(strCoord1);
		if (strCoord1.FindOneOf("wW") > 0) {
			dLon = -dLon;
		}
	} else if (strCoord2.FindOneOf("eEwW") > 0) {
		dLon = atof(strCoord2);
		if (strCoord2.FindOneOf("wW") > 0) {
			dLon = -dLon;
		}
	} else {
		dLon = atof(strCoord2);
	}

	return true;
}


void CTabGrid::OnBnClickedOk()
{
	UpdateData(true);

COziGenDlg	* pParent;
pParent = (COziGenDlg *)(this->GetParent()->GetParent());

	int nCoords = pParent->GetCheckedRadioButton(IDC_RADIO_LAT_LON, IDC_RADIO_OS_COORDS);

	long easting=0, northing=0;

	if (g_CoordinateSystem == COORD_OSGB) {

		if (nCoords == IDC_RADIO_LAT_LON) {
			
			double dLat, dLon;
			if (!ExtractLatLon(m_strGridRef, dLat, dLon)) {
				pParent->AddToResultsWindow("Error: Invalid Lat Lon %s",m_strGridRef);
				return;
			}

			double d_northing;
			double d_easting;
			LatLon_to_NorthingEasting(dLat, dLon, &d_northing, &d_easting);

			northing = (long) d_northing;
			easting = (long) d_easting;

		} else {

			if (OSref_to_easting_northing(m_strGridRef, &easting, &northing) == 0) {
				pParent->AddToResultsWindow("Error: Invalid Grid Ref. %s",m_strGridRef);
				return;
			}
		}

	} else if (g_CoordinateSystem == COORD_LAMBERT93) {

		if (nCoords == IDC_RADIO_LAT_LON) {

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
		
		} else {
			pParent->AddToResultsWindow("Lambert Currently Not Supported!");
			return;
		}
	}
	if ((g_CoordinateSystem == COORD_LAMBERT93) && !IsInIGNDatabase(northing, easting)) {

		pParent->AddToResultsWindow("Error: the map for %s is not supported in the IGN Database",m_strGridRef);
		return;
	}

	if ((g_CoordinateSystem == COORD_OSGB) && !IsInOSGBDatabase(northing, easting)) {

		pParent->AddToResultsWindow("Error: the map for %s is not supported in the OSGB Database",m_strGridRef);
		return;
	}

	long eMin, nMin, eMax, nMax;

	eMin = easting/1000*1000;
	nMin = northing/1000*1000;
	eMax = (easting+999)/1000*1000;
	nMax = (northing+999)/1000*1000;

	CRect MyRect = CRect(eMin, nMax, eMax, nMin);
	if (m_nGridRefInflate > 0) {
		MyRect.InflateRect(m_nGridRefInflate * 1000, -m_nGridRefInflate * 1000);
	}

	pParent->m_mapBoundaries = MyRect;

	pParent->DisplayGridRef(MyRect);
	pParent->DisplayMapData();

	pParent->UpdateData(true);

	CString str;
	str.Format("map_%d_%d",easting,northing);

	pParent->m_strMapName = str;
	pParent->UpdateData(false);

	pParent->AddToResultsWindow("Determined Map Extents for Grid Ref. %s",m_strGridRef);

//	m_MyMapSource = MapFromGridRef;
}

void CTabGrid::OnEnKillfocusEditGridRefInflate()
{
COziGenDlg	* pParent;
pParent = (COziGenDlg *)(this->GetParent()->GetParent());

	UpdateData(true);
	AfxGetApp()->WriteProfileInt("Main", "GridRefInflation", m_nGridRefInflate);
//	UpdateData(false);

	pParent->m_nGridRefInflate = m_nGridRefInflate;
	pParent->UpdateData(false);
}


void CTabGrid::OnEnKillfocusEditGridRef()
{
COziGenDlg	* pParent;
pParent = (COziGenDlg *)(this->GetParent()->GetParent());

	UpdateData(true);
	AfxGetApp()->WriteProfileString("Main", "GridReference", m_strGridRef);
//	UpdateData(false);

	pParent->m_strGridRef = m_strGridRef;
	pParent->UpdateData(false);
}

void CTabGrid::UpdateGridExample()
{
/*
#define IDC_RADIO_LAT_LON               1079
#define IDC_RADIO_GRID                  1080
#define IDC_RADIO_OS_COORDS             1081

#define IDC_RADIO_OSGB                  1066
#define IDC_RADIO_OSNI                  1067
#define IDC_RADIO_OSI                   1068
#define IDC_RADIO_IGN                   1069
#define IDC_RADIO_CH                    1070
*/
COziGenDlg	* pParent;
pParent = (COziGenDlg *)(this->GetParent()->GetParent());

	int nCoords = pParent->GetCheckedRadioButton(IDC_RADIO_LAT_LON, IDC_RADIO_OS_COORDS);
	int nDatum = pParent->GetCheckedRadioButton(IDC_RADIO_OSGB, IDC_RADIO_CH);

	if (nDatum == IDC_RADIO_IGN) {
		if (nCoords == IDC_RADIO_GRID) m_strGridExample.Format("e.g. Lambert 123 456");
		if (nCoords == IDC_RADIO_LAT_LON) m_strGridExample.Format("e.g. 48.8742° N, 2.3470° E");
	} else if (nDatum == IDC_RADIO_OSGB) {
		if (nCoords == IDC_RADIO_GRID) m_strGridExample.Format("e.g. British! SE 123 456");
		if (nCoords == IDC_RADIO_LAT_LON) m_strGridExample.Format("e.g. 51.5171° N, 0.1062° W");
	} else {
		m_strGridExample.Format("N/A");
	}
	UpdateData(false);
}

void CTabGrid::OnDeltaposSpinGrid(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	UpdateData(true);
	m_nGridRefInflate -= pNMUpDown->iDelta;
	UpdateData(false);

	*pResult = 0;
}
