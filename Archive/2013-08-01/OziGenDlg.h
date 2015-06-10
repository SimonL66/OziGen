// OziGenDlg.h : header file
//

#pragma once

#include "..\MapLib\Track.h"

#include "MyTabCtrl.h"

#include "TabTrack.h"
#include "TabGrid.h"
#include "TabMap.h"


enum RadioDatabase {
	DBASE_TRACKLOGS_2,
	DBASE_TRACKLOGS_3,
	DBASE_LOCAL
};

enum RadioMapType {
	MAP_OZI,
	MAP_KMZ,
	MAP_JNX,
	MAP_JPR,
	MAP_QCT,
	MAP_PKG
};

enum ComboMapType {
	MAP_RASTER,
	MAP_VECTOR,
	MAP_STREET,
};

enum RadioCountry {
	COUNTRY_AT,
	COUNTRY_NGI, 
	COUNTRY_OSGB,
	COUNTRY_OSI,
	COUNTRY_IGN,
	COUNTRY_DE,
	COUNTRY_IT,
	COUNTRY_LUX,
	COUNTRY_NL,
	COUNTRY_OSNI,
	COUNTRY_PT,
	COUNTRY_ES,
	COUNTRY_CH,
};

enum MapScale {

	MAP_SCALE_10K,
	MAP_SCALE_25K,
	MAP_SCALE_50K,
	MAP_SCALE_100K,
	MAP_SCALE_250K,

	MAP_SCALE_VML,
	MAP_SCALE_SV,
	MAP_SCALE_VMD,
	MAP_SCALE_MS,
	MAP_SCALE_OV,
};

enum GPSModel {
	GPS_MODEL_MONTANA,
	GPS_MODEL_OREGON,
	GPS_MODEL_DAKOTA,
	GPS_MODEL_ETREX,
	GPS_MODEL_GPS_6X,
	GPS_MODEL_OREGON_6X
};

enum CoordSystem {
	COORD_SYSTEM_LAT_LON,
	COORD_SYSTEM_GRID,
	COORD_SYSTEM_OS
};

enum DatabaseGraphicsType {
	GFX_TYPE_BMP,
	GFX_TYPE_JPEG,
	GFX_TYPE_PNG_8,
	GFX_TYPE_PNG_24,
	GFX_TYPE_PNG_32
};

void Global_AddToResultsWindow(const char *format, ...);

// COziGenDlg dialog
class COziGenDlg : public CDialog
{
// Construction
public:
	COziGenDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_OZIGEN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:

	afx_msg void OnEnKillfocusEditDatabaseFolder();
	afx_msg void OnBnClickedButtonOutput();

	afx_msg void OnEnKillfocusEditWest();
	afx_msg void OnEnKillfocusEditEast();
	afx_msg void OnEnKillfocusEditNorth();
	afx_msg void OnEnKillfocusEditSouth();

	afx_msg void OnDeltaposSpinNorth(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinSouth(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinEast(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinWest(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnEnKillfocusEditDrawOrder();

	afx_msg void OnBnClickedCheckOffline();
	afx_msg void OnBnClickedCheckReplace();
	afx_msg void OnBnClickedCheckOptimize();

	afx_msg void OnBnClickedRadioMap();
	afx_msg void OnBnClickedRadioDatabase();
	afx_msg void OnBnClickedRadioCoords();
	afx_msg void OnBnClickedRadioMapScale();
	
	afx_msg void OnCbnSelchangeComboTilesize();
	afx_msg void OnCbnSelchangeComboTracksize();

	afx_msg void OnBnClickedButtonGenerate();

public:

	void AddToResultsWindow(const char *format, ...);

	void DisplayMapData();

	void DisplayGridRef(CRect MyRect);

	void DisplayGridSize();

	void SetTileSize();
	
	void UpdateDatum();

	void UpdateMapScaleRadioButtons();

	CMyTabCtrl	m_tabMyTabCtrl;

	CString m_strResults;
	CString m_strOutputDir;

	CString m_strWest;
	CString m_strEast;
	CString m_strNorth;
	CString m_strSouth;

	CString m_strMapName;
	CString m_strGridSize;
	
	CRect m_rectBoundaries;

	CTrack m_MyTrack;

	CEdit m_ctl_edit_results;

	CRect m_mapBoundaries;

	int m_nPreviousCoordsSystem;

	long m_nDrawOrder;

	int m_nTileSize;
	int m_nTrackSize;

	long m_tileWidth;
	long m_tileHeight;

	BOOL m_bOffline;
	BOOL m_bReplace;
	BOOL m_bOptimize;

	CString m_strMapFolder;

	CTabTrack * pTabTrack;
	CTabGrid * pTabGrid;
	CTabMap * pTabMap;

int m_nFlavour;
int m_nCountry_new;
int m_nCountry;
int m_nScale;

	int m_nDatabase;
	int m_nMapType;

	int m_nGpsModel;
	int m_nCoordSystem;

	afx_msg void OnCbnSelchangeComboCountry();
	afx_msg void OnCbnSelchangeComboGfxType();
};
