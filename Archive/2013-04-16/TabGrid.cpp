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
}


BEGIN_MESSAGE_MAP(CTabGrid, CDialog)
	//{{AFX_MSG_MAP(CTabGrid)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_EN_KILLFOCUS(IDC_EDIT_GRID_REF_INFLATE, &CTabGrid::OnEnKillfocusEditGridRefInflate)
	ON_EN_KILLFOCUS(IDC_EDIT_GRID_REF, &CTabGrid::OnEnKillfocusEditGridRef)
	ON_BN_CLICKED(IDOK, &CTabGrid::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabGrid message handlers

bool ExtractGridRef(CString str, double& dLat, double& dLon)
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


	if (g_CoordinateSystem == COORD_OSGB) {

		long easting, northing;
if (true/* LatLon selected*/) {

		double d_northing;
		double d_easting;
		d_easting = atof(m_strGridRef);

		int nPos = m_strGridRef.Find(",");

		if (nPos == -1) {
			pParent->AddToResultsWindow("Error: Invalid Grid Ref. %s",m_strGridRef);
			return;
		}

		d_northing = atof(m_strGridRef.Mid(nPos+1));
	
	double latitude = d_easting;
	double longitude = d_northing;

	LatLon_to_NorthingEasting(latitude, longitude, &d_northing, &d_easting);

	northing = (long) d_northing;
	easting = (long) d_easting;

} else {

		if (OSref_to_easting_northing(m_strGridRef, &easting, &northing) == 0) {
			pParent->AddToResultsWindow("Error: Invalid Grid Ref. %s",m_strGridRef);
			return;
		}
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
		str = "map_" + m_strGridRef;
		for (int i=0; i < str.GetLength(); i++) {
			if (isspace(str[i]))
				str.SetAt(i,'_');
		}
		pParent->m_strMapName = str;
		pParent->UpdateData(false);

		pParent->AddToResultsWindow("Determined Map Extents for Grid Ref. %s",m_strGridRef);
	
	} else if (g_CoordinateSystem == COORD_LAMBERT93) {

		long easting, northing;

		if (true/* LatLon selected*/) {

			double dLat, dLon;

			if (!ExtractGridRef(m_strGridRef, dLat, dLon)) {
				pParent->AddToResultsWindow("Error: Invalid Coordinate %s",m_strGridRef);
				return;
			}

			double d_northing;
			double d_easting;
		
			WGS84ToLambert93(dLat, dLon, &d_northing, &d_easting);

			northing = (long) d_northing;
			easting = (long) d_easting;
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
	}

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
