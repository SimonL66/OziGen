// TabTrack.cpp : implementation file
//

#include "stdafx.h"

#include "OziGen.h"
#include "TabTrack.h"

#include "OziGenDlg.h"

#include "func.h"
#include "ozi.h"
#include "gps.h"

#include "..\MapLib\CoordConv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabTrack dialog


CTabTrack::CTabTrack(CWnd* pParent /*=NULL*/)
	: CDialog(CTabTrack::IDD, pParent)
	, m_nTrackInflate(0)
	, m_rectBoundaries(0,0,0,0)
	, m_strTrackName(_T(""))
{
	//{{AFX_DATA_INIT(CTabTrack)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTabTrack::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabTrack)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_TRACK_INFLATE, m_nTrackInflate);
	DDX_Text(pDX, IDC_EDIT_TRACK_NAME, m_strTrackName);
}


BEGIN_MESSAGE_MAP(CTabTrack, CDialog)
	//{{AFX_MSG_MAP(CTabTrack)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_GPS_TRACK, &CTabTrack::OnBnClickedButtonGpsTrack)
	ON_BN_CLICKED(IDC_BUTTON_TRACK, &CTabTrack::OnBnClickedButtonTrack)
	ON_EN_KILLFOCUS(IDC_EDIT_TRACK_INFLATE, &CTabTrack::OnEnKillfocusEditTrackInflate)
	ON_BN_CLICKED(IDOK, &CTabTrack::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabTrack message handlers

void CTabTrack::OnBnClickedButtonTrack()
{
COziGenDlg	* pParent;
pParent = (COziGenDlg *)(this->GetParent()->GetParent());

	// Transfer values from dialog box to member variables
	UpdateData(true);

	TCHAR szFilters[]=	_T("Mapping Files (*.gpx;*.mmo;*.trl;*.txf)|*.gpx;*.mmo;*.trl;*.txf|")
						_T("GPS Exchange Files (*.gpx)|*.gpx|")
						_T("Memory-Map Files (*.mmo)|*.mmo|")
						_T("Tracklog Files (*.trl)|*.trl|")
						_T("Maptech Files (*.txf)|*.txf|")
						_T("All Files (*.*)|*.*||");

	// Create an Open dialog; the default file name extension is .mmo & .trl
	CFileDialog fileDlg(TRUE , NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, szFilters);

	if (fileDlg.DoModal() == IDOK) {

		POSITION pos = fileDlg.GetStartPosition();
		while (pos != NULL) {

			CString strPathName = fileDlg.GetNextPathName(pos);
			CString strFileName = GetFilename(strPathName);

			m_MyTrack.RemoveAll();

			if (m_MyTrack.ReadTrack(strPathName)) {

				pParent->m_MyTrack.ReadTrack(strPathName);

				m_rectBoundaries = m_MyTrack.GetTrackExtents((f_ll2en)OZIGEN_LatLon_to_EastingNorthing);

				CString strOziPath = CreateOziPathName(m_strOutputDir, strFileName);
/*				
				CRect MyRect = m_rectBoundaries;
				if (m_nTrackInflate > 0) {
					MyRect.InflateRect(m_nTrackInflate * 1000, -m_nTrackInflate * 1000);
				}
*/	
//				DisplayGridRef(MyRect);
//				DisplayMapData();

				UpdateData(true);
				m_strMapName = RemoveSuffix(strFileName);
				m_strTrackName = m_strMapName;
				UpdateData(false);

//				AddToResultsWindow("Determined Map Extents for %s", strFileName);

//				m_MyMapSource = MapFromTrack;

			} else {
				pParent->AddToResultsWindow("Error: Invalid Track %s", strFileName);
			}
		}
	}
}


void CTabTrack::OnEnKillfocusEditTrackInflate()
{
COziGenDlg	* pParent;
pParent = (COziGenDlg *)(this->GetParent()->GetParent());

	UpdateData(true);
	AfxGetApp()->WriteProfileInt("Main", "TrackInflation", m_nTrackInflate);
//	UpdateData(false);
	
	pParent->m_nTrackInflate = m_nTrackInflate;
	pParent->UpdateData(false);
/*
	if (!m_rectBoundaries.IsRectNull()) {
		CRect MyRect = m_rectBoundaries;
		if (m_nTrackInflate > 0) {
			MyRect.InflateRect(m_nTrackInflate * 1000, -m_nTrackInflate * 1000);
		}
		DisplayGridRef(MyRect);
		DisplayMapData();


		if (m_strMapName) {
			UpdateData(true);
			m_strMapName = m_strTrackName;
			UpdateData(false);
		}
	}
*/
}

void CTabTrack::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
//	OnOK();
COziGenDlg	* pParent;
	pParent = (COziGenDlg *)(this->GetParent()->GetParent());
//	pParent->m_strEastSouth = "east south";
//	pParent->UpdateData(false);

//	CRect MyRect = m_rectBoundaries;

	if (m_MyTrack.GetCount() <= 0) {
		MessageBox("Track Not Loaded!");
		return;
	}

	m_rectBoundaries = m_MyTrack.GetTrackExtents((f_ll2en)OZIGEN_LatLon_to_EastingNorthing);

	if (m_nTrackInflate > 0) {
		m_rectBoundaries.InflateRect(m_nTrackInflate * 1000, -m_nTrackInflate * 1000);
	}

/*
	char strRef1_2[256];
	char strRef2_2[256];	

	easting_northing_to_OSref2(m_rectBoundaries.left, m_rectBoundaries.top, strRef1_2);
	easting_northing_to_OSref2(m_rectBoundaries.right, m_rectBoundaries.bottom, strRef2_2);
*/

	pParent->m_mapBoundaries = m_rectBoundaries;
	pParent->m_strMapName = m_strMapName;

/*
	pParent->m_strWest = strRef1_2;
	pParent->m_strEast = strRef2_2;
	pParent->m_strNorth = strRef1_2;
	pParent->m_strSouth = strRef2_2;
*/
	pParent->UpdateData(false);

	pParent->DisplayMapData();
}

void CTabTrack::OnBnClickedButtonGpsTrack()
{
	CString strTrackFolder = GetTrackFolderFromGPS();

	// Transfer values from dialog box to member variables
	UpdateData(true);

	TCHAR szFilters[]=	_T("Mapping Files (*.gpx;*.mmo;*.trl;*.txf)|*.gpx;*.mmo;*.trl;*.txf|")
						_T("GPS Exchange Files (*.gpx)|*.gpx|")
						_T("Memory-Map Files (*.mmo)|*.mmo|")
						_T("Tracklog Files (*.trl)|*.trl|")
						_T("Maptech Files (*.txf)|*.txf|")
						_T("All Files (*.*)|*.*||");

	// Create an Open dialog; the default file name extension is .mmo & .trl
	CFileDialog fileDlg(TRUE , NULL, strTrackFolder+"\\Select File", OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, szFilters);

	if (fileDlg.DoModal() == IDOK) {

		POSITION pos = fileDlg.GetStartPosition();
		while (pos != NULL) {

			CString strPathName = fileDlg.GetNextPathName(pos);
			CString strFileName = GetFilename(strPathName);

			m_MyTrack.RemoveAll();

			if (m_MyTrack.ReadTrack(strPathName)) {

				m_rectBoundaries = m_MyTrack.m_Extents;

				CString strOziPath = CreateOziPathName(m_strOutputDir, strFileName);
/*				
				CRect MyRect = m_rectBoundaries;
				if (m_nTrackInflate > 0) {
					MyRect.InflateRect(m_nTrackInflate * 1000, -m_nTrackInflate * 1000);
				}
*/

//				DisplayGridRef(MyRect);
//				DisplayMapData();

				UpdateData(true);
				m_strMapName = RemoveSuffix(strFileName);
				m_strTrackName = m_strMapName;
				UpdateData(false);

//				AddToResultsWindow("Determined Map Extents for %s", strFileName);

//				m_MyMapSource = MapFromTrack;

			} else {
//				AddToResultsWindow("Error: Invalid Track %s", strFileName);
			}
		}
	}
}
