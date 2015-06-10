// TabMap.cpp : implementation file
//

#include "stdafx.h"

#include "OziGen.h"
#include "TabMap.h"

#include "OziGenDlg.h"

#include "Explorer.h"
#include "Landranger.h"

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
}


BEGIN_MESSAGE_MAP(CTabMap, CDialog)
	//{{AFX_MSG_MAP(CTabMap)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CTabMap::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_EDIT_MAP, &CTabMap::OnEnKillfocusEditMap)
	ON_CBN_SELCHANGE(IDC_COMBO_MAP_TYPE, &CTabMap::OnCbnSelchangeComboMapType)
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
	} else {
		GetLandrangerExtents (m_nExplorerMap, strWestNorth, strEastSouth, strMap);
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
//	UpdateData(false);

	pParent->m_nExplorerMap = m_nExplorerMap;
	pParent->UpdateData(false);
}

void CTabMap::OnCbnSelchangeComboMapType()
{
COziGenDlg	* pParent;
pParent = (COziGenDlg *)(this->GetParent()->GetParent());

	UpdateData(true);
	AfxGetApp()->WriteProfileInt("Main", "MapType", m_nMapType);
//	UpdateData(false);

	pParent->m_nMapType = m_nMapType;
	pParent->UpdateData(false);
}
