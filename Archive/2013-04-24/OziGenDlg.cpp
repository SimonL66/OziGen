// OziGenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OziGen.h"
#include "OziGenDlg.h"

#include "ozi.h"
#include "jnx.h"
#include "CreateKMZ.h"
#include "func.h"

#include "ProgressWindow.h"
#include "Database.h"

#include "GPS.h"

#include "Explorer.h"
#include "Landranger.h"

#include "GetOSPro.h"
#include "GetIGN.h"

#include "math.h"
#include <direct.h>

#include "..\MapLib\map_conv.h"

#include "..\MapLib\Track.h"
#include "..\MapLib\Route.h"
#include "..\MapLib\Maptech.h"
#include "..\MapLib\CoordConv.h"

#include "TabTrack.h"
#include "TabGrid.h"
#include "TabMap.h"

#include "MakeQCT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CString g_strMapDatabaseFolder;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// COziGenDlg dialog




COziGenDlg::COziGenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COziGenDlg::IDD, pParent)
	, m_strResults(_T(""))
	, m_strOutputDir(_T(""))
	, m_strWest(_T(""))
	, m_strEast(_T(""))
	, m_strNorth(_T(""))
	, m_strSouth(_T(""))
	, m_nTrackInflate(1)
	, m_strMapName(_T(""))
	, m_nExplorerMap(1)
	, m_strGridSize(_T(""))
	, m_strGridRef(_T(""))
	, m_nGridRefInflate(10)
	, m_nTileSize(3)
	, m_nTrackSize(3)
	, m_nMapType(0)
	, m_nDrawOrder(50)
	, m_bOptimize(FALSE)
	, m_bOffline(TRUE)
	, m_bReplace(FALSE)
	, m_strMapFolder(_T("C:\\OziGenDatabase"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_MyMapSource = MapNone;
}

void COziGenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TAB_CTRL, m_tabMyTabCtrl);

	DDX_Text(pDX, IDC_EDIT_OUTPUT, m_strOutputDir);

	DDX_Text(pDX, IDC_EDIT_WEST,  m_strWest);
	DDX_Text(pDX, IDC_EDIT_EAST,  m_strEast);
	DDX_Text(pDX, IDC_EDIT_NORTH, m_strNorth);
	DDX_Text(pDX, IDC_EDIT_SOUTH, m_strSouth);

	//	DDX_Text(pDX, IDC_EDIT_TRACK_INFLATE, m_nTrackInflate);

	DDX_Text(pDX, IDC_EDIT_DRAW_ORDER, m_nDrawOrder);

	//	DDX_Text(pDX, IDC_EDIT_MAP, m_nExplorerMap);
	DDX_Text(pDX, IDC_EDIT_MAP_NAME, m_strMapName);
	DDX_Text(pDX, IDC_STATIC_GRID_SIZE, m_strGridSize);

	DDX_Text(pDX, IDC_EDIT_RESULTS, m_strResults);

	DDX_Control(pDX, IDC_EDIT_RESULTS, m_ctl_edit_results);

	//	DDX_Text(pDX, IDC_EDIT_GRID_REF, m_strGridRef);
	//	DDX_Text(pDX, IDC_EDIT_GRID_REF_INFLATE, m_nGridRefInflate);
	DDX_CBIndex(pDX, IDC_COMBO_TILE_SIZE, m_nTileSize);
	//	DDX_CBIndex(pDX, IDC_COMBO_MAP_TYPE, m_nMapType);

	if (!pDX->m_bSaveAndValidate) {
		m_ctl_edit_results.LineScroll(m_ctl_edit_results.GetLineCount());
	}
	DDX_CBIndex(pDX, IDC_COMBO_TRACK_SIZE, m_nTrackSize);

	DDX_Check(pDX, IDC_CHECK_OPTIMIZE, m_bOptimize);

	DDX_Check(pDX, IDC_CHECK_OFFLINE, m_bOffline);
	DDX_Check(pDX, IDC_CHECK_REPLACE, m_bReplace);
	DDX_Text(pDX, IDC_EDIT_MAP_FOLDER, m_strMapFolder);
}

BEGIN_MESSAGE_MAP(COziGenDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
//	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP

//	ON_BN_CLICKED(IDC_BUTTON_EXPLORER, &COziGenDlg::OnBnClickedButtonExplorer)
//	ON_BN_CLICKED(IDC_BUTTON_GRID_REF, &COziGenDlg::OnBnClickedButtonGridRef)
//	ON_EN_KILLFOCUS(IDC_EDIT_TRACK_INFLATE, &COziGenDlg::OnEnKillfocusEditTrackInflate)
//	ON_EN_KILLFOCUS(IDC_EDIT_GRID_REF_INFLATE, &COziGenDlg::OnEnKillfocusEditGridRefInflate)
//	ON_CBN_SELCHANGE(IDC_COMBO_MAP_TYPE, &COziGenDlg::OnCbnSelchangeComboMapType)

	ON_EN_KILLFOCUS(IDC_EDIT_DRAW_ORDER, &COziGenDlg::OnEnKillfocusEditDrawOrder)
	ON_EN_KILLFOCUS(IDC_EDIT_MAP, &COziGenDlg::OnEnKillfocusEditMap)
	ON_EN_KILLFOCUS(IDC_EDIT_WEST, &COziGenDlg::OnEnKillfocusEditWest)
	ON_EN_KILLFOCUS(IDC_EDIT_EAST, &COziGenDlg::OnEnKillfocusEditEast)
	ON_EN_KILLFOCUS(IDC_EDIT_NORTH, &COziGenDlg::OnEnKillfocusEditNorth)
	ON_EN_KILLFOCUS(IDC_EDIT_SOUTH, &COziGenDlg::OnEnKillfocusEditSouth)
	ON_EN_KILLFOCUS(IDC_EDIT_MAP_FOLDER, &COziGenDlg::OnEnKillfocusEditDatabaseFolder)

	ON_BN_CLICKED(IDC_BUTTON_OUTPUT, &COziGenDlg::OnBnClickedButtonOutput)
	ON_BN_CLICKED(IDC_BUTTON_GENERATE, &COziGenDlg::OnBnClickedButtonGenerate)
	ON_BN_CLICKED(IDC_BUTTON_TRACK, &COziGenDlg::OnBnClickedButtonTrack)
	ON_BN_CLICKED(IDC_RADIO_MAP_OZI, &COziGenDlg::OnBnClickedRadioMap)
	ON_BN_CLICKED(IDC_RADIO_MAP_KMZ, &COziGenDlg::OnBnClickedRadioMap)
	ON_BN_CLICKED(IDC_RADIO_MAP_JNX, &COziGenDlg::OnBnClickedRadioMap)
	ON_BN_CLICKED(IDC_RADIO_MAP_PKG, &COziGenDlg::OnBnClickedRadioMap)
	ON_BN_CLICKED(IDC_RADIO_MAP_JPR, &COziGenDlg::OnBnClickedRadioMap)
	ON_BN_CLICKED(IDC_RADIO_MAP_QCT, &COziGenDlg::OnBnClickedRadioMap)
	ON_BN_CLICKED(IDC_RADIO_DBASE_TRACKLOGS_2, &COziGenDlg::OnBnClickedRadioDatabase)
	ON_BN_CLICKED(IDC_RADIO_DBASE_TRACKLOGS_3, &COziGenDlg::OnBnClickedRadioDatabase)
	ON_BN_CLICKED(IDC_RADIO_DBASE_LOCAL, &COziGenDlg::OnBnClickedRadioDatabase)
	ON_BN_CLICKED(IDC_RADIO_OSGB, &COziGenDlg::OnBnClickedRadioDatabase)
	ON_BN_CLICKED(IDC_RADIO_OSNI, &COziGenDlg::OnBnClickedRadioDatabase)
	ON_BN_CLICKED(IDC_RADIO_OSI , &COziGenDlg::OnBnClickedRadioDatabase)
	ON_BN_CLICKED(IDC_RADIO_IGN , &COziGenDlg::OnBnClickedRadioDatabase)
	ON_BN_CLICKED(IDC_RADIO_CH  , &COziGenDlg::OnBnClickedRadioDatabase)
	ON_BN_CLICKED(IDC_BUTTON_GPS_TRACK, &COziGenDlg::OnBnClickedButtonGpsTrack)
	ON_BN_CLICKED(IDC_BUTTON_GPS_MAP, &COziGenDlg::OnBnClickedButtonGpsMap)
	ON_BN_CLICKED(IDC_CHECK_OFFLINE, &COziGenDlg::OnBnClickedCheckOffline)
	ON_BN_CLICKED(IDC_CHECK_REPLACE, &COziGenDlg::OnBnClickedCheckReplace)
	ON_BN_CLICKED(IDC_CHECK_OPTIMIZE, &COziGenDlg::OnBnClickedCheckOptimize)

	ON_BN_CLICKED(IDC_RADIO_LAT_LON, &COziGenDlg::OnBnClickedRadioCoords)
	ON_BN_CLICKED(IDC_RADIO_GRID, &COziGenDlg::OnBnClickedRadioCoords)
	ON_BN_CLICKED(IDC_RADIO_OS_COORDS, &COziGenDlg::OnBnClickedRadioCoords)

	ON_CBN_SELCHANGE(IDC_COMBO_TILE_SIZE, &COziGenDlg::OnCbnSelchangeComboTilesize)
	ON_CBN_SELCHANGE(IDC_COMBO_TRACK_SIZE, &COziGenDlg::OnCbnSelchangeComboTracksize)

	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_NORTH, &COziGenDlg::OnDeltaposSpinNorth)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SOUTH, &COziGenDlg::OnDeltaposSpinSouth)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_EAST, &COziGenDlg::OnDeltaposSpinEast)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_WEST, &COziGenDlg::OnDeltaposSpinWest)
END_MESSAGE_MAP()

// COziGenDlg message handlers

BOOL COziGenDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

//GetDlgItem(IDC_BUTTON_GPS_MAP)->ShowWindow(false);
//GetDlgItem(IDC_BUTTON_GRID_REF)->ShowWindow(false);
//GetDlgItem(IDC_EDIT_GRID_REF)->ShowWindow(false);

	m_tabMyTabCtrl.InsertItem(0, _T("Track:1"));
	m_tabMyTabCtrl.InsertItem(1, _T("Grid:2"));
	m_tabMyTabCtrl.InsertItem(2, _T("Map:3"));
//	m_tabMyTabCtrl.InsertItem(3, _T("Database:4"));
m_tabMyTabCtrl.Init();

pTabTrack = (CTabTrack *)m_tabMyTabCtrl.m_tabPages[0];
pTabTrack->m_nTrackInflate = AfxGetApp()->GetProfileInt("Main", "TrackInflation", 0);
pTabTrack->UpdateData(false);

pTabGrid = (CTabGrid *)m_tabMyTabCtrl.m_tabPages[1];
pTabGrid->m_nGridRefInflate = AfxGetApp()->GetProfileInt("Main", "GridRefInflation", 0);
pTabGrid->m_strGridRef = AfxGetApp()->GetProfileString("Main", "GridReference", "");
pTabGrid->UpdateData(false);

pTabMap = (CTabMap *)m_tabMyTabCtrl.m_tabPages[2];
pTabMap->m_nMapType = AfxGetApp()->GetProfileInt("Main", "MapType", 0);
pTabMap->m_nExplorerMap = AfxGetApp()->GetProfileInt("Main", "ExplorerMap", 0);
pTabMap->UpdateData(false);

//GetDlgItem(IDC_TAB_CTRL)->EnableWindow(false);

	m_strOutputDir = AfxGetApp()->GetProfileString("Main", "OutputDir", "");

	m_strMapFolder = AfxGetApp()->GetProfileString("Main", "MapDatabaseFolder", "C:\\OziGenDatabase");
	g_strMapDatabaseFolder = m_strMapFolder;

	m_nTrackInflate = AfxGetApp()->GetProfileInt("Main", "TrackInflation", 0);

	m_nGridRefInflate = AfxGetApp()->GetProfileInt("Main", "GridRefInflation", 0);
	m_strGridRef = AfxGetApp()->GetProfileString("Main", "GridReference", "");

	m_nExplorerMap = AfxGetApp()->GetProfileInt("Main", "ExplorerMap", 0);
	
	m_nTileSize = AfxGetApp()->GetProfileInt("Main", "TileSize", 0);
	SetTileSize();

	m_nTrackSize = AfxGetApp()->GetProfileInt("Main", "TrackSize", 0);
	m_nMapType = AfxGetApp()->GetProfileInt("Main", "MapType", 0);

	m_nDrawOrder = AfxGetApp()->GetProfileInt("Main", "DrawOrder", 50);

	m_bOffline = AfxGetApp()->GetProfileInt("Main", "Offline", 1);
	m_bReplace = AfxGetApp()->GetProfileInt("Main", "Replace", 0);
	m_bOptimize = AfxGetApp()->GetProfileInt("Main", "Optimize", 0);

	int nRadioMapType = AfxGetApp()->GetProfileInt("Main", "RadioMapType", IDC_RADIO_MAP_QCT);
	CheckRadioButton(IDC_RADIO_MAP_OZI, IDC_RADIO_MAP_QCT, nRadioMapType);

	bool bShowOptimizeControls = (nRadioMapType == IDC_RADIO_MAP_KMZ);
	GetDlgItem(IDC_CHECK_OPTIMIZE)->EnableWindow(bShowOptimizeControls);
	GetDlgItem(IDC_COMBO_TRACK_SIZE)->EnableWindow(bShowOptimizeControls);

	bool bShowGarminControls = ((nRadioMapType == IDC_RADIO_MAP_KMZ) | (nRadioMapType == IDC_RADIO_MAP_JNX)); 
	GetDlgItem(IDC_EDIT_DRAW_ORDER)->EnableWindow(bShowGarminControls);
	GetDlgItem(IDC_COMBO_TILE_SIZE)->EnableWindow(bShowGarminControls);

	int nRadioDatabase = AfxGetApp()->GetProfileInt("Main", "RadioDatabase", IDC_RADIO_DBASE_LOCAL);
	CheckRadioButton(IDC_RADIO_DBASE_TRACKLOGS_2, IDC_RADIO_DBASE_LOCAL, nRadioDatabase);

	int nRadioDatum = AfxGetApp()->GetProfileInt("Main", "RadioDatum", IDC_RADIO_OSGB);
	CheckRadioButton(IDC_RADIO_OSGB, IDC_RADIO_CH, nRadioDatum);

	int nRadioMapScale = AfxGetApp()->GetProfileInt("Main", "RadioMapScale", IDC_RADIO_25K);
	CheckRadioButton(IDC_RADIO_50K, IDC_RADIO_10K, nRadioMapScale);

	int nRadioGpsModel = AfxGetApp()->GetProfileInt("Main", "RadioGpsModel", IDC_RADIO_OREGON);
	CheckRadioButton(IDC_RADIO_MONTANA, IDC_RADIO_GPS_6X, nRadioGpsModel);

	int nRadioCoords = AfxGetApp()->GetProfileInt("Main", "RadioCoords", IDC_RADIO_GRID);
	CheckRadioButton(IDC_RADIO_LAT_LON, IDC_RADIO_OS_COORDS, nRadioCoords);

	bool bShowOSProOptions = (nRadioDatabase == IDC_RADIO_DBASE_LOCAL);
	GetDlgItem(IDC_CHECK_OFFLINE)->EnableWindow(bShowOSProOptions);

	if (nRadioDatabase == IDC_RADIO_DBASE_LOCAL) {
		GetDlgItem(IDC_CHECK_REPLACE)->EnableWindow(!m_bOffline);
	} else {
		GetDlgItem(IDC_CHECK_REPLACE)->EnableWindow(false);
	}

/*
	if (nRadioDatum == IDC_RADIO_IGN) {
		g_CoordinateSystem = COORD_LAMBERT93;

		m_bOffline = true;
		m_bReplace = false;

		GetDlgItem(IDC_CHECK_OFFLINE)->EnableWindow(false);
		GetDlgItem(IDC_CHECK_REPLACE)->EnableWindow(false);

		GetDlgItem(IDC_RADIO_MAP_QCT)->EnableWindow(true);
	} else {
		g_CoordinateSystem = COORD_OSGB;
		GetDlgItem(IDC_RADIO_MAP_QCT)->EnableWindow(false);
	}
*/
	UpdateDatum();

	pTabGrid->UpdateGridExample();
	pTabMap->UpdateMapName();

	if (m_bOffline) {

		GetDlgItem(IDC_CHECK_REPLACE)->EnableWindow(false);
	}

// Hack to disable Online OSPRO - set to false to skip!
if(true) {

	GetDlgItem(IDC_CHECK_OFFLINE)->ShowWindow(false);
	GetDlgItem(IDC_CHECK_REPLACE)->ShowWindow(false);
	m_bOffline = true;
	m_bReplace = false;
}

// To Remove Once the PKG/JPR/QCT routines have been completed.
GetDlgItem(IDC_RADIO_MAP_PKG)->EnableWindow(false);

// To Remove Once the GPS Model routines have been completed.
GetDlgItem(IDC_RADIO_MONTANA)->EnableWindow(false);
GetDlgItem(IDC_RADIO_OREGON)->EnableWindow(false);
GetDlgItem(IDC_RADIO_DAKOTA)->EnableWindow(false);
GetDlgItem(IDC_RADIO_ETREX)->EnableWindow(false);
GetDlgItem(IDC_RADIO_GPS_6X)->EnableWindow(false);

// To Remove Once the Map Database routines have been added.
GetDlgItem(IDC_RADIO_OSNI)->EnableWindow(false);
GetDlgItem(IDC_RADIO_OSI)->EnableWindow(false);
GetDlgItem(IDC_RADIO_CH)->EnableWindow(false);

// To Remove Once the Map Scales routines have been added.
GetDlgItem(IDC_RADIO_50K)->EnableWindow(false);
GetDlgItem(IDC_RADIO_10K)->EnableWindow(false);

// To Remove Once the Coord routines have been added.
//GetDlgItem( IDC_RADIO_LAT_LON)->EnableWindow(false);
GetDlgItem(IDC_RADIO_OS_COORDS)->EnableWindow(false);

//GetDlgItem(IDC_RADIO_DBASE_OS_PRO)->EnableWindow(false);

//GetDlgItem(IDC_BUTTON_GPS_TRACK)->EnableWindow(false);
GetDlgItem(IDC_BUTTON_GPS_MAP)->EnableWindow(false);

pTabTrack->GetDlgItem(IDC_BUTTON_GPS_TRACK)->EnableWindow(false);

	m_mapBoundaries = CRect(0,0,0,0);

	UpdateData(false);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COziGenDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void COziGenDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR COziGenDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Global_AddToResultsWindow(const char *format, ...)
{
CWinApp* pWin = AfxGetApp();
	if (pWin && ((COziGenApp*)pWin)->m_pMainWnd) {
		COziGenDlg * pDlg;
		pDlg = ((COziGenDlg*)(((COziGenApp*)pWin)->m_pMainWnd));
		va_list argptr;
		va_start(argptr, format);
		char buffer[4096];

		vsprintf(buffer, format, argptr);
		pDlg->AddToResultsWindow(buffer);
	}
}


void COziGenDlg::AddToResultsWindow(const char *format, ...)
{
	va_list argptr;
	va_start(argptr, format);
	char buffer[4096];
	long length;

	length = vsprintf(buffer, format, argptr);

	UpdateData(true);
		m_strResults = m_strResults + buffer + "\r\n";
	UpdateData(false);
}

void COziGenDlg::OnBnClickedButtonOutput()
{
	// Create an Open dialog; the default file name extension is ".my".
	CFileDialog fileDlg(TRUE, NULL, _T("Dir"), OFN_HIDEREADONLY, _T("All Files (*.*)|*.*||"));

	if (fileDlg.DoModal() == IDOK) {

		UpdateData(true);
		m_strOutputDir = GetPath(fileDlg.GetPathName());
		AfxGetApp()->WriteProfileString("Main", "OutputDir", m_strOutputDir);
		UpdateData(false);
	}
}

void COziGenDlg::OnBnClickedButtonGenerate()
{

	UpdateData(true);

	int nRadioDatabase = GetCheckedRadioButton(IDC_RADIO_DBASE_TRACKLOGS_2, IDC_RADIO_DBASE_LOCAL);
	int nRadioDatum = GetCheckedRadioButton(IDC_RADIO_OSGB, IDC_RADIO_CH);
	int nRadioType = GetCheckedRadioButton(IDC_RADIO_MAP_OZI, IDC_RADIO_MAP_QCT);

	DatabaseType nDatabase = DetermineDatabaseType(nRadioDatabase, nRadioDatum);


//CRect MyRect;

/*
	if (OSref_to_easting_northing(m_strWestNorth, &MyRect.left, &MyRect.top) == 0) {

		AddToResultsWindow("Error: Invalid Westing\\Northing %s",m_strWestNorth);
		return;
	}

	if (OSref_to_easting_northing(m_strEastSouth, &MyRect.right, &MyRect.bottom) == 0) {

		AddToResultsWindow("Error: Invalid Easting\\Southing %s",m_strEastSouth);
		return;
	}
*/

	// Ensure that Rectangle is West/North and East/South!
	if (m_mapBoundaries.right < m_mapBoundaries.left) {
		long tmp;
		tmp = m_mapBoundaries.left;
		m_mapBoundaries.left = m_mapBoundaries.right;
		m_mapBoundaries.right = tmp;
	}
	if (m_mapBoundaries.bottom > m_mapBoundaries.top) {
		long tmp;
		tmp = m_mapBoundaries.bottom;
		m_mapBoundaries.bottom = m_mapBoundaries.top;
		m_mapBoundaries.top = tmp;
	}

	if (((m_mapBoundaries.top == 0) && (m_mapBoundaries.bottom == 0))
	||  ((m_mapBoundaries.left == 0) && (m_mapBoundaries.right == 0))) {

		AddToResultsWindow("Error: Invalid Map Boundaries");
		return;
	}

	if ((g_CoordinateSystem == COORD_LAMBERT93)
		&& ((!IsInIGNDatabase(m_mapBoundaries.top, m_mapBoundaries.left)) || (!IsInIGNDatabase(m_mapBoundaries.bottom, m_mapBoundaries.right))) ){

		AddToResultsWindow("Error: the map is not supported in the IGN Database");
		return;
	}

	if ((g_CoordinateSystem == COORD_OSGB) 
		&& ((!IsInOSGBDatabase(m_mapBoundaries.top, m_mapBoundaries.left)) || (!IsInOSGBDatabase(m_mapBoundaries.bottom, m_mapBoundaries.right))) ){

		AddToResultsWindow("Error: the map is not supported in the OSGB Database");
		return;
	}

	if (m_strMapName.IsEmpty()) {
		m_strMapName = "UserMap";
	}

	CString strOziPath = CreateOziPathName(m_strOutputDir, m_strMapName);

	SetCurrentDirectory(m_strOutputDir);

	CButton *pOffline = (CButton *) GetDlgItem (IDC_CHECK_OFFLINE);
	bool bOffline = false;	
	if (pOffline->GetCheck()) {
		bOffline = true;
	}

	if (nDatabase == DB_OSGB_Local && !bOffline) {

		CTileDatabase	g_db;

		long	minx, miny, maxx, maxy;

		minx = m_mapBoundaries.left;
		maxx = m_mapBoundaries.right;
		miny = m_mapBoundaries.bottom;
		maxy = m_mapBoundaries.top;

		if (minx > maxx) {
			long t = minx;
			minx = maxx;
			maxx = t;
		}
		if (miny > maxy) {
			long t = miny;
			miny = maxy;
			maxy = t;
		}

		long collar = CalculateCollar(minx, maxx, miny, maxy);

		minx = (minx/500*500) - collar;
		miny = (miny/500*500) - collar;
		maxx = ((maxx+499)/500*500) + collar;
		maxy = ((maxy+499)/500*500) + collar;

		_mkdir(OS_PRO_DATABASE_PATH);

		CButton *pReplace = (CButton *) GetDlgItem (IDC_CHECK_REPLACE);
		bool bReplace = false;	
		if (pReplace->GetCheck()) {
			bReplace = true;
		}

		g_db.SetReadonly(false);
		LoadTiles(&g_db, minx, miny, maxx, maxy, bReplace);
		g_db.SetReadonly(true);
	}

	if (nRadioType == IDC_RADIO_MAP_OZI || nRadioType == IDC_RADIO_MAP_JPR) {

		SetCurrentDirectory(m_strOutputDir);

		if (nDatabase == DB_IGN_Local) {
			
			long	minx, miny, maxx, maxy;
			minx = m_mapBoundaries.left/1000*1000;
			miny = m_mapBoundaries.bottom/1000*1000;
			maxx = (m_mapBoundaries.right+999)/1000*1000;
			maxy = (m_mapBoundaries.top+999)/1000*1000;

			IGN_Create_PNG_JPR(m_strMapName, minx, miny, maxx, maxy);

			if (nRadioType == IDC_RADIO_MAP_JPR) {

				IGN_dumpjprfile(m_strMapName, minx, miny, maxx, maxy, (maxx-minx)/1000 * IGN_TILE_WIDTH, (maxy-miny)/1000 * IGN_TILE_WIDTH, NULL);
			
			} else {

				IGN_Create_OZI_MAP(m_strMapName, minx, miny, maxx, maxy);
			}

		} else {

			Create_OZI_PNG(m_strMapName, m_mapBoundaries.left/1000, m_mapBoundaries.right/1000, m_mapBoundaries.bottom/1000, m_mapBoundaries.top/1000, nRadioDatabase);
			DumpOziFile(strOziPath, m_mapBoundaries);
		}

			AddToResultsWindow("Created OZI files for %s",m_strMapName);

	} else if (nRadioType == IDC_RADIO_MAP_KMZ) {

		if (m_bOptimize) {
			CString strMaptechName;
			strMaptechName = m_strOutputDir + "\\" + m_strMapName + ".txf";
			if (!IsFilePresent(strMaptechName)) {
				WriteMaptechTrack(strMaptechName, m_MyTrack);
				if (DecMapKMZ(m_strOutputDir, m_strMapName, m_mapBoundaries, m_tileWidth, m_tileHeight, nDatabase, m_nTrackSize, m_nDrawOrder)) {
					AddToResultsWindow("Created Optimized KMZ file for %s",m_strMapName);
				} else {
					AddToResultsWindow("Couldn't create KMZ file for %s",m_strMapName);
				}
				DeleteFile(strMaptechName);
			} else {
				AddToResultsWindow("Error: Temporary maptech file %s.txf already exits!",m_strMapName);
			}
		} else {
			if (DecMapKMZ(m_strOutputDir, m_strMapName, m_mapBoundaries, m_tileWidth, m_tileHeight, nDatabase, m_nTrackSize, m_nDrawOrder)) {
				AddToResultsWindow("Created KMZ file for %s",m_strMapName);
			} else {
				AddToResultsWindow("Couldn't create KMZ file for %s",m_strMapName);
			}
		}
	} else if (nRadioType == IDC_RADIO_MAP_JNX) {
		if (DecMapJNX(m_strOutputDir, m_strMapName, m_mapBoundaries, m_tileWidth, m_tileHeight, nDatabase, m_nDrawOrder)) {
			AddToResultsWindow("Created JNX files for %s",m_strMapName);
		} else {
				AddToResultsWindow("Couldn't create JNX file for %s",m_strMapName);
		}
	} else if (nRadioType == IDC_RADIO_MAP_JPR) {		

		SetCurrentDirectory(m_strOutputDir);
		Create_OZI_PNG(m_strMapName, m_mapBoundaries.left/1000, m_mapBoundaries.right/1000, m_mapBoundaries.bottom/1000, m_mapBoundaries.top/1000, nRadioDatabase);
//		dumpjpr_file(m_strMapName, MyRect.left, MyRect.right, MyRect.bottom, MyRect.top, NULL);
//		Create_PNG_JPR(m_strMapName, MyRect.left, MyRect.right, MyRect.bottom, MyRect.top);
//		AddToResultsWindow("Created JPR files for %s",m_strMapName);

	} else if (nRadioType == IDC_RADIO_MAP_QCT) {		

//		char strCmd[256];
//		sprintf(strCmd,"MakeQCTIGN -o %d %d %d %d \"%s.qct\"",m_mapBoundaries.top, m_mapBoundaries.left, m_mapBoundaries.bottom, m_mapBoundaries.right, m_strMapName);
//		AddToResultsWindow("%s",strCmd);
//		system(strCmd);
		if (nDatabase == DB_IGN_Local) {
			SetCurrentDirectory(m_strOutputDir);
			(void) MakeQCT_main(m_mapBoundaries.top, m_mapBoundaries.left, m_mapBoundaries.bottom, m_mapBoundaries.right, m_strMapName+".qct");
		} else {
			AddToResultsWindow("Error: Make QCT  Not supported for OSGB Maps!");
		}

	} else {
		AddToResultsWindow("Error: Unknown Output Type for %s",m_strMapName);
	}
}

void COziGenDlg::OnEnKillfocusEditTrackInflate()
{
	UpdateData(true);
	AfxGetApp()->WriteProfileInt("Main", "TrackInflation", m_nTrackInflate);
	UpdateData(false);

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
}

void COziGenDlg::OnOK()
{
}

void COziGenDlg::OnBnClickedButtonExplorer()
{
	CString strWestNorth;
	CString strEastSouth;
	CString strMap;

	if (m_nMapType == 0) {
		GetExtents (m_nExplorerMap, strWestNorth, strEastSouth, strMap);
	} else {
		GetLandrangerExtents (m_nExplorerMap, strWestNorth, strEastSouth, strMap);
	}

	UpdateData(true);

	m_strWest = strWestNorth;
	m_strEast = strEastSouth;
	m_strNorth = strWestNorth;
	m_strSouth = strEastSouth;

	m_strMapName = strMap;
	UpdateData(false);


	UpdateData(true);

/*
	CRect MyRect;
	
	if (OSref_to_easting_northing(m_strWestNorth, &MyRect.left, &MyRect.top) == 0) {

		AddToResultsWindow("Error: Invalid Westing\\Northing %s",m_strWestNorth);
		return;
	}

	if (OSref_to_easting_northing(m_strEastSouth, &MyRect.right, &MyRect.bottom) == 0) {

		AddToResultsWindow("Error: Invalid Easting\\Southing %s",m_strEastSouth);
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
*/

	if (m_strMapName.IsEmpty()) {
		m_strMapName = "UserMap";
	}

	DisplayMapData();
	AddToResultsWindow("Determined Map Extents for %s",m_strMapName);

	m_MyMapSource = MapFromExplorer;
}


void COziGenDlg::OnEnKillfocusEditMap()
{
	UpdateData(true);
	AfxGetApp()->WriteProfileInt("Main", "ExplorerMap", m_nExplorerMap);
	UpdateData(false);
}

void COziGenDlg::DisplayGridSize()
{
	CRect MyRect;

	UpdateData(true);

//	if (m_strWestNorth.IsEmpty() || OSref_to_easting_northing(m_strWestNorth, &MyRect.left, &MyRect.top) == 0) {
//		m_strGridSize = "";
//	} else if (m_strEastSouth.IsEmpty() || OSref_to_easting_northing(m_strEastSouth, &MyRect.right, &MyRect.bottom) == 0) {
//		m_strGridSize = "";
//	} else {
		DisplayMapData();
//	}

	UpdateData(false);
}

void COziGenDlg::OnEnKillfocusEditWest()
{
	UpdateData(true);

//	if (error)
//		AddToResultsWindow("Error: Invalid Westing %s",m_strWest);
//		CWnd * pWnd = GetDlgItem(IDC_EDIT_WEST);
//		if (pWnd != NULL)
//			pWnd->SetFocus();
//		return;
//	else
		m_mapBoundaries.left = atoi(m_strWest);
		DisplayGridSize();
}

void COziGenDlg::OnEnKillfocusEditEast()
{
	UpdateData(true);
/*
	if (!m_strEastSouth.IsEmpty() && OSref_to_easting_northing(m_strEastSouth, &MyRect.right, &MyRect.bottom) == 0) {

		AddToResultsWindow("Error: Invalid Easting\\Southing %s",m_strEastSouth);
		CWnd * pWnd = GetDlgItem(IDC_EDIT_EAST_SOUTH);
		if (pWnd != NULL)
			pWnd->SetFocus();
		return;
	}
*/
		m_mapBoundaries.right = atoi(m_strEast);
		DisplayGridSize();
}

void COziGenDlg::OnEnKillfocusEditNorth()
{
	UpdateData(true);
/*
	if (!m_strWestNorth.IsEmpty() && OSref_to_easting_northing(m_strWestNorth, &MyRect.left, &MyRect.top) == 0) {

		AddToResultsWindow("Error: Invalid Westing\\Northing %s",m_strWestNorth);
		CWnd * pWnd = GetDlgItem(IDC_EDIT_WEST_NORTH);
		if (pWnd != NULL)
			pWnd->SetFocus();
		return;
	}
*/
		m_mapBoundaries.top = atoi(m_strNorth);
		DisplayGridSize();
}

void COziGenDlg::OnEnKillfocusEditSouth()
{
	UpdateData(true);
/*
	if (!m_strEastSouth.IsEmpty() && OSref_to_easting_northing(m_strEastSouth, &MyRect.right, &MyRect.bottom) == 0) {

		AddToResultsWindow("Error: Invalid Easting\\Southing %s",m_strEastSouth);
		CWnd * pWnd = GetDlgItem(IDC_EDIT_EAST_SOUTH);
		if (pWnd != NULL)
			pWnd->SetFocus();
		return;
	}
*/
		m_mapBoundaries.bottom = atoi(m_strSouth);
		DisplayGridSize();
}

void COziGenDlg::OnBnClickedButtonGridRef()
{
	UpdateData(true);

	long easting, northing;

	if (OSref_to_easting_northing(m_strGridRef, &easting, &northing) == 0) {
		AddToResultsWindow("Error: Invalid Grid Ref. %s",m_strGridRef);
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

	DisplayGridRef(MyRect);
	DisplayMapData();

	UpdateData(true);
	m_strMapName = "map_" + m_strGridRef;
	for (int i=0; i < m_strMapName.GetLength(); i++) {
		if (isspace(m_strMapName[i]))
			m_strMapName.SetAt(i,'_');
	}
	UpdateData(false);

	AddToResultsWindow("Determined Map Extents for Grid Ref. %s",m_strGridRef);

	m_MyMapSource = MapFromGridRef;
}

void COziGenDlg::OnCbnSelchangeComboTilesize()
{
	UpdateData(true);
	SetTileSize();
	DisplayMapData();
	AfxGetApp()->WriteProfileInt("Main", "TileSize", m_nTileSize);
	UpdateData(false);
}

void COziGenDlg::OnCbnSelchangeComboTracksize()
{
	UpdateData(true);
	AfxGetApp()->WriteProfileInt("Main", "TrackSize", m_nTrackSize);
	UpdateData(false);
}

void COziGenDlg::DisplayGridRef(CRect MyRect)
{
	UpdateData(true);

	char strRef1_2[256];
	char strRef2_2[256];

	easting_northing_to_OSref2(MyRect.left, MyRect.top, strRef1_2);
	easting_northing_to_OSref2(MyRect.right, MyRect.bottom, strRef2_2);
	
//	m_strWestNorth = strRef1_2;
//	m_strEastSouth = strRef2_2;

	UpdateData(false);
}

void COziGenDlg::DisplayMapData()
{
	UpdateData(true);

//	CRect MyRect;

//MyRect = m_mapBoundaries;
//	if (OSref_to_easting_northing(m_strWestNorth, &MyRect.left, &MyRect.top) == 0) return;
//	if (OSref_to_easting_northing(m_strEastSouth, &MyRect.right, &MyRect.bottom) == 0) return;

	char strBuffer[256];
	
	long GridWidth = labs((m_mapBoundaries.right-m_mapBoundaries.left));
	long GridHeight = labs((m_mapBoundaries.bottom-m_mapBoundaries.top));

	double dWidth = GridWidth / (m_tileWidth * 2.5);
	double dHeight = GridHeight / (m_tileHeight * 2.5);

	long nWidth = (long) ceil(dWidth);
	long nHeight = (long) ceil(dHeight);

	int nRadio = GetCheckedRadioButton(IDC_RADIO_MAP_OZI, IDC_RADIO_MAP_QCT);

	if (nRadio == IDC_RADIO_MAP_OZI) {


		sprintf (strBuffer,"%d km x %d km", GridWidth / 1000, GridHeight / 1000);
	} else {
/*
		Attempt to Calc. tiles count on the fly - takes too long!
		CString strMaptechName;
		strMaptechName = m_strOutputDir + "\\" + m_strMapName + ".txf";
		if (!IsFilePresent(strMaptechName)) {
				WriteMaptechTrack(strMaptechName, m_MyTrack);
		}
		int ntiles = GetCustomMapTileCount(m_mapBoundaries.left, m_mapBoundaries.right, m_mapBoundaries.top, m_mapBoundaries.bottom, strMaptechName, m_tileWidth, m_tileHeight, m_nTrackSize);
*/
		sprintf (strBuffer,"%d km x %d km - %d Tiles", 
			GridWidth / 1000, GridHeight / 1000, nWidth * nHeight);
	}

	m_strGridSize = strBuffer;

	m_strWest.Format("%d",m_mapBoundaries.left);
	m_strEast.Format("%d",m_mapBoundaries.right);
	m_strNorth.Format("%d",m_mapBoundaries.top);
	m_strSouth.Format("%d",m_mapBoundaries.bottom);

	UpdateData(false);
}

void COziGenDlg::OnEnKillfocusEditGridRefInflate()
{
	UpdateData(true);
	AfxGetApp()->WriteProfileInt("Main", "GridRefInflation", m_nGridRefInflate);

	long easting, northing;

	if (OSref_to_easting_northing(m_strGridRef, &easting, &northing) == 0) {
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

	DisplayGridRef(MyRect);
	DisplayMapData();

	UpdateData(false);
}

void COziGenDlg::OnBnClickedRadioMap()
{
	UpdateData(true);

	DisplayMapData();

	int nRadioMapType = GetCheckedRadioButton(IDC_RADIO_MAP_OZI, IDC_RADIO_MAP_QCT);
	AfxGetApp()->WriteProfileInt("Main", "RadioMapType", nRadioMapType);

	bool bShowOptimizeControls = (nRadioMapType == IDC_RADIO_MAP_KMZ);
	GetDlgItem(IDC_CHECK_OPTIMIZE)->EnableWindow(bShowOptimizeControls);
	GetDlgItem(IDC_COMBO_TRACK_SIZE)->EnableWindow(bShowOptimizeControls);

	bool bShowGarminControls = ((nRadioMapType == IDC_RADIO_MAP_KMZ) | (nRadioMapType == IDC_RADIO_MAP_JNX)); 
	GetDlgItem(IDC_EDIT_DRAW_ORDER)->EnableWindow(bShowGarminControls);
	GetDlgItem(IDC_COMBO_TILE_SIZE)->EnableWindow(bShowGarminControls);

	UpdateData(false);
}

void COziGenDlg::OnBnClickedRadioDatabase()
{
	UpdateData(true);

	int nRadioDatabase = GetCheckedRadioButton(IDC_RADIO_DBASE_TRACKLOGS_2, IDC_RADIO_DBASE_LOCAL);
	AfxGetApp()->WriteProfileInt("Main", "RadioDatabase", nRadioDatabase);

	int nRadioDatum = GetCheckedRadioButton(IDC_RADIO_OSGB, IDC_RADIO_CH);
	AfxGetApp()->WriteProfileInt("Main", "RadioDatum", nRadioDatum);

	if (nRadioDatum == IDC_RADIO_IGN) {

		g_CoordinateSystem = COORD_LAMBERT93;


	} else {

		g_CoordinateSystem = COORD_OSGB;

	}

	pTabGrid->UpdateGridExample();

	bool bShowOSProOptions = (nRadioDatabase == IDC_RADIO_DBASE_LOCAL);
	GetDlgItem(IDC_CHECK_REPLACE)->EnableWindow(bShowOSProOptions);
	GetDlgItem(IDC_CHECK_OFFLINE)->EnableWindow(bShowOSProOptions);

	if (nRadioDatum != IDC_RADIO_OSGB) {

		GetDlgItem(IDC_RADIO_DBASE_TRACKLOGS_2)->EnableWindow(false);
		GetDlgItem(IDC_RADIO_DBASE_TRACKLOGS_3)->EnableWindow(false);
	
		nRadioDatabase = IDC_RADIO_DBASE_LOCAL;
		CheckRadioButton(IDC_RADIO_DBASE_TRACKLOGS_2, IDC_RADIO_DBASE_LOCAL, nRadioDatabase);
		AfxGetApp()->WriteProfileInt("Main", "RadioDatabase", nRadioDatabase);

		m_bOffline = true;
		m_bReplace = false;

		GetDlgItem(IDC_CHECK_OFFLINE)->EnableWindow(false);
		GetDlgItem(IDC_CHECK_REPLACE)->EnableWindow(false);

		GetDlgItem(IDC_RADIO_MAP_QCT)->EnableWindow(true);

	} else {

		GetDlgItem(IDC_RADIO_DBASE_TRACKLOGS_2)->EnableWindow(true);
		GetDlgItem(IDC_RADIO_DBASE_TRACKLOGS_3)->EnableWindow(true);

		GetDlgItem(IDC_CHECK_OFFLINE)->EnableWindow(true);
		GetDlgItem(IDC_CHECK_REPLACE)->EnableWindow(true);

		GetDlgItem(IDC_RADIO_MAP_QCT)->EnableWindow(false);
	}

	UpdateData(false);
}

void COziGenDlg::OnBnClickedRadioCoords()
{
	UpdateData(true);

	int nRadioCoords = GetCheckedRadioButton(IDC_RADIO_LAT_LON, IDC_RADIO_OS_COORDS);
	AfxGetApp()->WriteProfileInt("Main", "RadioCoords", nRadioCoords);

	pTabGrid->UpdateGridExample();

	UpdateData(false);
}

void COziGenDlg::SetTileSize()
{
	if (m_nTileSize == 0) {
		m_tileWidth = 256;
		m_tileHeight = 256;
	} else if (m_nTileSize == 1) {
		m_tileWidth = 512;
		m_tileHeight = 512;
	} else if (m_nTileSize == 2) {
		m_tileWidth = 840;
		m_tileHeight = 840;
	} else if (m_nTileSize == 3) {
		m_tileWidth = 1024;
		m_tileHeight = 1024;
	} else if (m_nTileSize == 4) {
		m_tileWidth = 512;
		m_tileHeight = 2048;
	} else if (m_nTileSize == 5) {
		m_tileWidth = 2048;
		m_tileHeight = 512;
	} else if (m_nTileSize == 6) {
		m_tileWidth = 256;
		m_tileHeight = 4096;
	} else if (m_nTileSize == 7) {
		m_tileWidth = 4096;
		m_tileHeight = 256;
	} else if (m_nTileSize == 8) {
		m_tileWidth = 800;
		m_tileHeight = 1200;
	} else {
		m_tileWidth = 1024;
		m_tileHeight = 1024;
	}
}

void COziGenDlg::OnCbnSelchangeComboMapType()
{
	UpdateData(true);
	AfxGetApp()->WriteProfileInt("Main", "MapType", m_nMapType);
	UpdateData(false);
}

void COziGenDlg::OnBnClickedButtonGpsTrack()
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
				
				CRect MyRect = m_rectBoundaries;
				if (m_nTrackInflate > 0) {
					MyRect.InflateRect(m_nTrackInflate * 1000, -m_nTrackInflate * 1000);
				}
	
				DisplayGridRef(MyRect);
				DisplayMapData();

				UpdateData(true);
				m_strMapName = RemoveSuffix(strFileName);
				m_strTrackName = m_strMapName;
				UpdateData(false);

				AddToResultsWindow("Determined Map Extents for %s", strFileName);

				m_MyMapSource = MapFromTrack;

			} else {
				AddToResultsWindow("Error: Invalid Track %s", strFileName);
			}
		}
	}
}

void COziGenDlg::OnBnClickedButtonTrack()
{
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

				m_rectBoundaries = m_MyTrack.m_Extents;

				CString strOziPath = CreateOziPathName(m_strOutputDir, strFileName);
				
				CRect MyRect = m_rectBoundaries;
				if (m_nTrackInflate > 0) {
					MyRect.InflateRect(m_nTrackInflate * 1000, -m_nTrackInflate * 1000);
				}
	
				DisplayGridRef(MyRect);
				DisplayMapData();

				UpdateData(true);
				m_strMapName = RemoveSuffix(strFileName);
				m_strTrackName = m_strMapName;
				UpdateData(false);

				AddToResultsWindow("Determined Map Extents for %s", strFileName);

				m_MyMapSource = MapFromTrack;

			} else {
				AddToResultsWindow("Error: Invalid Track %s", strFileName);
			}
		}
	}
}

void COziGenDlg::OnBnClickedButtonGpsMap()
{
	CString strMapFolder;
	CString strCustomMapType;

	int nRadioType = GetCheckedRadioButton(IDC_RADIO_MAP_OZI, IDC_RADIO_MAP_QCT);

	if (nRadioType == IDC_RADIO_MAP_KMZ) {

		strCustomMapType = "CustomMap";

	} else if (nRadioType == IDC_RADIO_MAP_JNX) {

		strCustomMapType = "Birdseye";
	}

	strMapFolder.Empty();
	strMapFolder = GetMapFolderFromGPS(strCustomMapType);
	
	if (strMapFolder.IsEmpty()) {

		OnBnClickedButtonOutput();

	} else {

		UpdateData(true);
		m_strOutputDir = strMapFolder;
		AfxGetApp()->WriteProfileString("Main", "OutputDir", m_strOutputDir);
		UpdateData(false);
	}
}

void COziGenDlg::OnEnKillfocusEditDatabaseFolder()
{
	UpdateData(true);
	AfxGetApp()->WriteProfileString("Main", "MapDatabaseFolder", m_strMapFolder);
	g_strMapDatabaseFolder = m_strMapFolder;
	UpdateData(false);
}

void COziGenDlg::OnEnKillfocusEditDrawOrder()
{
	UpdateData(true);
	AfxGetApp()->WriteProfileInt("Main", "DrawOrder", m_nDrawOrder);
	UpdateData(false);
}

void COziGenDlg::OnBnClickedCheckOffline()
{
	GetDlgItem(IDC_CHECK_REPLACE)->EnableWindow(m_bOffline);

	UpdateData(true);
	AfxGetApp()->WriteProfileInt("Main", "Offline", m_bOffline);
	UpdateData(false);
}

void COziGenDlg::OnBnClickedCheckReplace()
{
	UpdateData(true);
	AfxGetApp()->WriteProfileInt("Main", "Replace", m_bReplace);
	UpdateData(false);
}

void COziGenDlg::OnBnClickedCheckOptimize()
{
	UpdateData(true);
	AfxGetApp()->WriteProfileInt("Main", "Optimize", m_bOptimize);
	UpdateData(false);
}

void COziGenDlg::OnDeltaposSpinEast(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_mapBoundaries.left -= 1000 * pNMUpDown->iDelta;
	m_strEast.Format("%d",m_mapBoundaries.left);
	DisplayGridSize();
	UpdateData(false);
	*pResult = 0;
}

void COziGenDlg::OnDeltaposSpinNorth(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_mapBoundaries.top -= 1000 * pNMUpDown->iDelta;
	m_strNorth.Format("%d",m_mapBoundaries.top);
	DisplayGridSize();
	UpdateData(false);
	*pResult = 0;
}

void COziGenDlg::OnDeltaposSpinWest(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_mapBoundaries.right -= 1000 * pNMUpDown->iDelta;
	m_strWest.Format("%d",m_mapBoundaries.right);
	DisplayGridSize();
	UpdateData(false);
	*pResult = 0;
}

extern "C" {
void Lambert93ToWGS84(double northing, double easting, double * latitude, double * longitude);
void WGS84ToLambert93(double latitude, double longitude, double * northing, double * easting);
}

void COziGenDlg::OnDeltaposSpinSouth(NMHDR *pNMHDR, LRESULT *pResult)
{
	double latitude, longitude;
	double northing, easting;
	northing=6000000; easting=0;
	Lambert93ToWGS84(northing, easting, &latitude, &longitude);
	_RPT4(_CRT_WARN, "N%f E%f, lat=%f lon=%f\n", northing, easting, latitude, longitude);
	northing=6000000; easting=1300000;
	Lambert93ToWGS84(northing, easting, &latitude, &longitude);
	_RPT4(_CRT_WARN, "N%f E%f, lat=%f lon=%f\n", northing, easting, latitude, longitude);
	northing=7200000; easting=0;
	Lambert93ToWGS84(northing, easting, &latitude, &longitude);
	_RPT4(_CRT_WARN, "N%f E%f, lat=%f lon=%f\n", northing, easting, latitude, longitude);
	northing=7200000; easting=1300000;
	Lambert93ToWGS84(northing, easting, &latitude, &longitude);
	_RPT4(_CRT_WARN, "N%f E%f, lat=%f lon=%f\n", northing, easting, latitude, longitude);
/*
N6000000.000000 E0.000000, lat=40.773244 lon=-5.274410
N6000000.000000 E1300000.000000, lat=40.860434 lon=10.099236
N7200000.000000 E0.000000, lat=51.491411 lon=-7.076480
N7200000.000000 E1300000.000000, lat=51.597421 lon=11.649413
*/

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_mapBoundaries.bottom -= 1000 * pNMUpDown->iDelta;
	m_strSouth.Format("%d",m_mapBoundaries.bottom);
	DisplayGridSize();
	UpdateData(false);
	*pResult = 0;
}

void COziGenDlg::UpdateDatum()
{
	int nRadioDatum = GetCheckedRadioButton(IDC_RADIO_OSGB, IDC_RADIO_CH);

	if (nRadioDatum == IDC_RADIO_IGN) {
		g_CoordinateSystem = COORD_LAMBERT93;

		m_bOffline = true;
		m_bReplace = false;

		GetDlgItem(IDC_CHECK_OFFLINE)->EnableWindow(false);
		GetDlgItem(IDC_CHECK_REPLACE)->EnableWindow(false);

		GetDlgItem(IDC_RADIO_MAP_QCT)->EnableWindow(true);
	} else {
		g_CoordinateSystem = COORD_OSGB;
		GetDlgItem(IDC_RADIO_MAP_QCT)->EnableWindow(false);

		int nRadioType = GetCheckedRadioButton(IDC_RADIO_MAP_OZI, IDC_RADIO_MAP_QCT);
		if (nRadioType == IDC_RADIO_MAP_QCT) {
			CheckRadioButton(IDC_RADIO_MAP_OZI, IDC_RADIO_MAP_QCT, IDC_RADIO_MAP_OZI);
		}
	}
}