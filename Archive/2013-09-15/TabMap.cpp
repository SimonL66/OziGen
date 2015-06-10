// TabMap.cpp : implementation file
//

#include "stdafx.h"

#include "OziGen.h"
#include "TabMap.h"

#include "OziGenDlg.h"

#include "MapExtents.h"

#include "func.h"

#include "..\MapLib\CoordConv.h"

#include "..\MapLib\Route.h"
#include "..\MapLib\Maptech.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabMap dialog


CTabMap::CTabMap(CWnd* pParent /*=NULL*/)
	: CDialog(CTabMap::IDD, pParent)
	, m_nMapType(0)
	, m_nExplorerMap(1)
	, m_strMapName(_T(""))

	, m_bMapPrefix(FALSE)
	, m_bMapName(FALSE)
{
	//{{AFX_DATA_INIT(CTabMap)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTabMap::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabMap)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_CBIndex(pDX, IDC_COMBO_MAP_TYPE, m_nMapType);
	DDX_Text(pDX, IDC_EDIT_MAP, m_nExplorerMap);
	DDX_Text(pDX, IDC_STATIC_MAP_NAME, m_strMapName);
	DDX_Check(pDX, IDC_CHECK_PREFIX, m_bMapPrefix);
	DDX_Check(pDX, IDC_CHECK_LONG_NAME, m_bMapName);
}


BEGIN_MESSAGE_MAP(CTabMap, CDialog)
	//{{AFX_MSG_MAP(CTabMap)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CTabMap::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_EDIT_MAP, &CTabMap::OnEnKillfocusEditMap)
	ON_CBN_SELCHANGE(IDC_COMBO_MAP_TYPE, &CTabMap::OnCbnSelchangeComboMapType)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MAP, &CTabMap::OnDeltaposSpinMap)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabMap message handlers

void CTabMap::OnBnClickedOk()
{
COziGenDlg	* pParent;
pParent = (COziGenDlg *)(this->GetParent()->GetParent());

	CString strWestNorth;
	CString strEastSouth;
	CString strMap;

	if (m_nMapType == 0) {
		GetExplorerExtents (m_nExplorerMap, strWestNorth, strEastSouth, strMap);
	} else if (m_nMapType == 1) {
		GetLandrangerExtents (m_nExplorerMap, strWestNorth, strEastSouth, strMap);
	} else if (m_nMapType == 2) {
		GetNationalParkExtents (m_nExplorerMap, strWestNorth, strEastSouth, strMap);
	} else if (m_nMapType == 3) {
		GetRegionalAreaExtents (m_nExplorerMap, strWestNorth, strEastSouth, strMap);
	} else if (m_nMapType == 4) {
		GetLargeRegionalAreaExtents (m_nExplorerMap, strWestNorth, strEastSouth, strMap);
	}

	UpdateData(true);

	CRect MyRect;

	if (OSref_to_easting_northing(strWestNorth, &MyRect.left, &MyRect.top) == 0) {

		pParent->AddToResultsWindow("Error: Invalid Westing\\Northing %s",strWestNorth);
		return;
	}

	if (OSref_to_easting_northing(strEastSouth, &MyRect.right, &MyRect.bottom) == 0) {

		pParent->AddToResultsWindow("Error: Invalid Easting\\Southing %s",strEastSouth);
		return;
	}

	// Ensure that Rectangle is West/North and East/South!
	if (MyRect.right < MyRect.left) {
		long tmp;
		tmp = MyRect.left;
		MyRect.left = MyRect.right;
		MyRect.right = tmp;
	}
	if (MyRect.bottom > MyRect.top) {
		long tmp;
		tmp = MyRect.bottom;
		MyRect.bottom = MyRect.top;
		MyRect.top = tmp;
	}

	pParent->m_mapBoundaries = MyRect;

	CString strMapName;
	
	if (m_bMapPrefix) {
		strMapName = strMap;
		if (m_bMapName) strMapName = strMapName + "_" + RemoveSpaces(m_strMapName);
	} else {
		if (m_bMapName) {
			strMapName.Format("%03d_%s", m_nExplorerMap, RemoveSpaces(m_strMapName));
		}  else {
			strMapName = strMap;
		}
	}

	pParent->m_strMapName = strMapName;

	pParent->m_nCountry = COUNTRY_UK;
	pParent->UpdateData(false);

	pParent->UpdateDatum();
	pParent->UpdateData(false);

	pParent->DisplayMapData();

CRoute MyRoute;

MyRoute.Bound(strMap, MyRect.left, MyRect.top, MyRect.right, MyRect.bottom);

CString strFile;
strFile = strMap+".rxf";

WriteMaptechRoute(strFile, MyRoute);

	pParent->AddToResultsWindow("Determined Map Extents for %s",pParent->m_strMapName);
}

void CTabMap::OnEnKillfocusEditMap()
{
	UpdateData(true);
	AfxGetApp()->WriteProfileInt("Main", "ExplorerMap", m_nExplorerMap);
	UpdateMapName();
	UpdateData(false);
}

void CTabMap::OnCbnSelchangeComboMapType()
{
	UpdateData(true);
	AfxGetApp()->WriteProfileInt("Main", "MapType", m_nMapType);
	m_nExplorerMap = 1;
	AfxGetApp()->WriteProfileInt("Main", "ExplorerMap", m_nExplorerMap);
	UpdateData(false);
	UpdateMapName();
}

void CTabMap::OnDeltaposSpinMap(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	UpdateData(true);
//	m_nExplorerMap -= pNMUpDown->iDelta;
	VerifyMapNumber(pNMUpDown->iDelta);
	AfxGetApp()->WriteProfileInt("Main", "ExplorerMap", m_nExplorerMap);
	UpdateMapName();

	*pResult = 0;
}

void CTabMap::VerifyMapNumber(int iDelta)
{
	int nTargetMapNumber = m_nExplorerMap - iDelta;
	if (m_nMapType == 0) {
		m_nExplorerMap = GetExplorerMapNumber(nTargetMapNumber, iDelta);
	} else if (m_nMapType == 1)  {
		m_nExplorerMap = GetLandrangerMapNumber(nTargetMapNumber);
	} else if (m_nMapType == 2) {
		m_nExplorerMap = GetNationalParkMapNumber(nTargetMapNumber);
	} else if (m_nMapType == 3) {
		m_nExplorerMap = GetRegionalAreaMapNumber(nTargetMapNumber);
	} else if (m_nMapType == 4) {
		m_nExplorerMap = GetLargeRegionalAreaMapNumber(nTargetMapNumber);
	}
	UpdateData(false);
}

void CTabMap::UpdateMapName()
{
	if (m_nMapType == 0) {
		m_strMapName.Format("%s", GetExplorerMapName(m_nExplorerMap));
	} else if (m_nMapType == 1)  {
		m_strMapName.Format("%s", GetLandrangerMapName(m_nExplorerMap));
	} else if (m_nMapType == 2) {
		m_strMapName.Format("%s", GetNationalParkMapName(m_nExplorerMap));
	} else if (m_nMapType == 3) {
		m_strMapName.Format("%s", GetRegionalAreaMapName(m_nExplorerMap));
	} else if (m_nMapType == 4) {
		m_strMapName.Format("%s", GetLargeRegionalAreaMapName(m_nExplorerMap));
	}
	UpdateData(false);
}