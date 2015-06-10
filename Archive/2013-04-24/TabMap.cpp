// TabMap.cpp : implementation file
//

#include "stdafx.h"

#include "OziGen.h"
#include "TabMap.h"

#include "OziGenDlg.h"

#include "Explorer.h"
#include "Landranger.h"
#include "AnquetRegionalAreas.h"

#include "..\MapLib\CoordConv.h"

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
	, m_nExplorerMap(0)
	, m_strMapName(_T(""))
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
		GetExtents (m_nExplorerMap, strWestNorth, strEastSouth, strMap);
	} else if (m_nMapType == 1) {
		GetLandrangerExtents (m_nExplorerMap, strWestNorth, strEastSouth, strMap);
	} else if (m_nMapType == 2) {
		GetNationalParkExtents (m_nExplorerMap, strWestNorth, strEastSouth, strMap);
	} else {
		GetRegionalAreaExtents (m_nExplorerMap, strWestNorth, strEastSouth, strMap);
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
	pParent->m_strMapName = strMap;

	pParent->CheckRadioButton(IDC_RADIO_OSGB, IDC_RADIO_CH, IDC_RADIO_OSGB);
	pParent->UpdateData(false);

	pParent->UpdateDatum();
	pParent->UpdateData(false);

	pParent->DisplayMapData();
	pParent->AddToResultsWindow("Determined Map Extents for %s",pParent->m_strMapName);

//	m_MyMapSource = MapFromExplorer;

		// TODO: Add your control notification handler code here
//		OnOK();
	}

void CTabMap::OnEnKillfocusEditMap()
{
COziGenDlg	* pParent;
pParent = (COziGenDlg *)(this->GetParent()->GetParent());

	UpdateData(true);
	AfxGetApp()->WriteProfileInt("Main", "ExplorerMap", m_nExplorerMap);
	pParent->m_nExplorerMap = m_nExplorerMap;
	pParent->UpdateData(false);
	UpdateMapName();
	UpdateData(false);
}

void CTabMap::OnCbnSelchangeComboMapType()
{
COziGenDlg	* pParent;
pParent = (COziGenDlg *)(this->GetParent()->GetParent());

	UpdateData(true);
	AfxGetApp()->WriteProfileInt("Main", "MapType", m_nMapType);
	pParent->m_nMapType = m_nMapType;
	UpdateMapName();
	pParent->UpdateData(false);
}

void CTabMap::OnDeltaposSpinMap(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_nExplorerMap -= pNMUpDown->iDelta;

	UpdateMapName();

	UpdateData(false);
	*pResult = 0;
}

void CTabMap::UpdateMapName()
{
	if (m_nMapType == 0) {
		m_strMapName.Format("%s", GetMapName(m_nExplorerMap));
	} else if (m_nMapType == 1)  {
		m_strMapName.Format("%s", GetLanrangerMapName(m_nExplorerMap));
	} else if (m_nMapType == 2) {
		m_strMapName.Format("%s", GetNationalParkMapName(m_nExplorerMap));
	} else {
		m_strMapName.Format("%s", GetRegionalAreaMapName(m_nExplorerMap));
	}
	UpdateData(false);
}