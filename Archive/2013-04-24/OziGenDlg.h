// OziGenDlg.h : header file
//

#pragma once

#include "..\MapLib\Track.h"

#include "MyTabCtrl.h"

#include "TabTrack.h"
#include "TabGrid.h"
#include "TabMap.h"


void Global_AddToResultsWindow(const char *format, ...);

enum MapSource {
	MapNone,
	MapFromTrack,
	MapFromExplorer,
	MapFromGridRef,
};

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

	afx_msg void OnOK();
	
	afx_msg void OnEnKillfocusEditTrackInflate();
	afx_msg void OnEnKillfocusEditGridRefInflate();
	afx_msg void OnEnKillfocusEditDatabaseFolder();
	afx_msg void OnEnKillfocusEditMap();
	afx_msg void OnEnKillfocusEditDrawOrder();
	afx_msg void OnEnKillfocusEditWest();
	afx_msg void OnEnKillfocusEditEast();
	afx_msg void OnEnKillfocusEditNorth();
	afx_msg void OnEnKillfocusEditSouth();

	afx_msg void OnBnClickedButtonExplorer();
	afx_msg void OnBnClickedButtonTrack();
	afx_msg void OnBnClickedButtonOutput();
	afx_msg void OnBnClickedButtonGenerate();
	afx_msg void OnBnClickedButtonGridRef();
	afx_msg void OnBnClickedButtonGpsTrack();
	afx_msg void OnBnClickedButtonGpsMap();
	afx_msg void OnBnClickedCheckOffline();
	afx_msg void OnBnClickedCheckReplace();
	afx_msg void OnBnClickedCheckOptimize();
	afx_msg void OnBnClickedRadioMap();

	afx_msg void OnBnClickedRadioDatabase();
	afx_msg void OnBnClickedRadioCoords();
	
	afx_msg void OnCbnSelchangeComboTilesize();
	afx_msg void OnCbnSelchangeComboTracksize();
	afx_msg void OnCbnSelchangeComboMapType();

public:


	void AddToResultsWindow(const char *format, ...);

	void DisplayMapData();

	void DisplayGridRef(CRect MyRect);

	void DisplayGridSize();

	void SetTileSize();
	

	CMyTabCtrl	m_tabMyTabCtrl;

	CString m_strResults;
	CString m_strOutputDir;

	CString m_strWest;
	CString m_strEast;
	CString m_strNorth;
	CString m_strSouth;

	CString m_strMapName;
	CString m_strGridSize;
	
	CString m_strTrackName;

	CString m_strGridRef;
	CString m_strTile;

	CRect m_rectBoundaries;

	CTrack m_MyTrack;

	CEdit m_ctl_edit_results;

	CRect m_mapBoundaries;

	long m_nDrawOrder;

	long m_nTrackInflate;

	long m_nExplorerMap;

	long m_nGridRefInflate;

	int m_nTileSize;

	int m_nMapType;

	int m_nTrackSize;

	long m_tileWidth;
	long m_tileHeight;

	BOOL m_bOffline;
	BOOL m_bReplace;
	BOOL m_bOptimize;

	MapSource m_MyMapSource;

	CString m_strMapFolder;

	CTabTrack * pTabTrack;
	CTabGrid * pTabGrid;
	CTabMap * pTabMap;

	afx_msg void OnDeltaposSpinNorth(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinSouth(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinEast(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinWest(NMHDR *pNMHDR, LRESULT *pResult);


	void UpdateDatum();
};

