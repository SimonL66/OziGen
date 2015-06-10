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

#include "OSPro.h"
#include "IGN.h"
#include "NGI.h"
#include "Kompass.h"

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
extern CString g_strMapScale;
extern CString g_strMapFlavour;
extern CString g_strDatabasePath;

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
	, m_strMapName(_T(""))
	, m_strGridSize(_T(""))
	, m_nTileSize(3)
	, m_nTrackSize(3)
	, m_nDrawOrder(50)
	, m_bOptimize(FALSE)
	, m_bOffline(TRUE)
	, m_bReplace(FALSE)
	, m_strMapFolder(_T("C:\\OziGenDatabase"))
	, m_nDatabase(DBASE_LOCAL)
	, m_nMapType(MAP_OZI)
	, m_nFlavour(MAP_RASTER)
	, m_nCountry(COUNTRY_OSGB)
	, m_nCountry_new(COUNTRY_OSGB)
	, m_nScale(MAP_SCALE_25K)
	, m_nGpsModel(GPS_MODEL_OREGON)
	, m_nCoordSystem(COORD_SYSTEM_GRID)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
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

	DDX_Text(pDX, IDC_EDIT_DRAW_ORDER, m_nDrawOrder);

	DDX_Text(pDX, IDC_EDIT_MAP_NAME, m_strMapName);
	DDX_Text(pDX, IDC_STATIC_GRID_SIZE, m_strGridSize);

	DDX_Text(pDX, IDC_EDIT_RESULTS, m_strResults);

	DDX_Control(pDX, IDC_EDIT_RESULTS, m_ctl_edit_results);

	DDX_CBIndex(pDX, IDC_COMBO_TILE_SIZE, m_nTileSize);
	DDX_CBIndex(pDX, IDC_COMBO_TRACK_SIZE, m_nTrackSize);

	DDX_CBIndex(pDX, IDC_COMBO_COUNTRY, m_nCountry_new);
	DDX_CBIndex(pDX, IDC_COMBO_GFX_TYPE, m_nFlavour);

	if (!pDX->m_bSaveAndValidate) {
		m_ctl_edit_results.LineScroll(m_ctl_edit_results.GetLineCount());
	}

	DDX_Check(pDX, IDC_CHECK_OPTIMIZE, m_bOptimize);

	DDX_Check(pDX, IDC_CHECK_OFFLINE, m_bOffline);
	DDX_Check(pDX, IDC_CHECK_REPLACE, m_bReplace);
	DDX_Text(pDX, IDC_EDIT_MAP_FOLDER, m_strMapFolder);
	
	DDX_Radio (pDX, IDC_RADIO_DBASE_TRACKLOGS_2, m_nDatabase);
	DDX_Radio (pDX, IDC_RADIO_MAP_OZI, m_nMapType);
	DDX_Radio (pDX, IDC_RADIO_AT, m_nCountry);
	DDX_Radio (pDX, IDC_RADIO_10K, m_nScale);
	DDX_Radio (pDX, IDC_RADIO_MONTANA, m_nGpsModel);
	DDX_Radio (pDX, IDC_RADIO_LAT_LON, m_nCoordSystem);
}

BEGIN_MESSAGE_MAP(COziGenDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
//	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP

	ON_EN_KILLFOCUS(IDC_EDIT_DRAW_ORDER, &COziGenDlg::OnEnKillfocusEditDrawOrder)
	ON_EN_KILLFOCUS(IDC_EDIT_WEST, &COziGenDlg::OnEnKillfocusEditWest)
	ON_EN_KILLFOCUS(IDC_EDIT_EAST, &COziGenDlg::OnEnKillfocusEditEast)
	ON_EN_KILLFOCUS(IDC_EDIT_NORTH, &COziGenDlg::OnEnKillfocusEditNorth)
	ON_EN_KILLFOCUS(IDC_EDIT_SOUTH, &COziGenDlg::OnEnKillfocusEditSouth)
	ON_EN_KILLFOCUS(IDC_EDIT_MAP_FOLDER, &COziGenDlg::OnEnKillfocusEditDatabaseFolder)

	ON_BN_CLICKED(IDC_BUTTON_OUTPUT, &COziGenDlg::OnBnClickedButtonOutput)
	ON_BN_CLICKED(IDC_BUTTON_GENERATE, &COziGenDlg::OnBnClickedButtonGenerate)

	ON_BN_CLICKED(IDC_RADIO_MAP_OZI, &COziGenDlg::OnBnClickedRadioMap)
	ON_BN_CLICKED(IDC_RADIO_MAP_KMZ, &COziGenDlg::OnBnClickedRadioMap)
	ON_BN_CLICKED(IDC_RADIO_MAP_JNX, &COziGenDlg::OnBnClickedRadioMap)
	ON_BN_CLICKED(IDC_RADIO_MAP_JPR, &COziGenDlg::OnBnClickedRadioMap)
	ON_BN_CLICKED(IDC_RADIO_MAP_QCT, &COziGenDlg::OnBnClickedRadioMap)
	ON_BN_CLICKED(IDC_RADIO_MAP_PKG, &COziGenDlg::OnBnClickedRadioMap)

	ON_BN_CLICKED(IDC_RADIO_DBASE_TRACKLOGS_2, &COziGenDlg::OnBnClickedRadioDatabase)
	ON_BN_CLICKED(IDC_RADIO_DBASE_TRACKLOGS_3, &COziGenDlg::OnBnClickedRadioDatabase)
	ON_BN_CLICKED(IDC_RADIO_DBASE_LOCAL, &COziGenDlg::OnBnClickedRadioDatabase)

	ON_BN_CLICKED(IDC_RADIO_AT  , &COziGenDlg::OnBnClickedRadioDatabase)
	ON_BN_CLICKED(IDC_RADIO_NGI , &COziGenDlg::OnBnClickedRadioDatabase)
	ON_BN_CLICKED(IDC_RADIO_OSGB, &COziGenDlg::OnBnClickedRadioDatabase)
	ON_BN_CLICKED(IDC_RADIO_OSi , &COziGenDlg::OnBnClickedRadioDatabase)
	ON_BN_CLICKED(IDC_RADIO_IGN , &COziGenDlg::OnBnClickedRadioDatabase)
	ON_BN_CLICKED(IDC_RADIO_DE  , &COziGenDlg::OnBnClickedRadioDatabase)
	ON_BN_CLICKED(IDC_RADIO_IT  , &COziGenDlg::OnBnClickedRadioDatabase)
	ON_BN_CLICKED(IDC_RADIO_LU  , &COziGenDlg::OnBnClickedRadioDatabase)
	ON_BN_CLICKED(IDC_RADIO_NL  , &COziGenDlg::OnBnClickedRadioDatabase)

	ON_BN_CLICKED(IDC_RADIO_10K,  &COziGenDlg::OnBnClickedRadioMapScale)
	ON_BN_CLICKED(IDC_RADIO_25K,  &COziGenDlg::OnBnClickedRadioMapScale)
	ON_BN_CLICKED(IDC_RADIO_50K,  &COziGenDlg::OnBnClickedRadioMapScale)
	ON_BN_CLICKED(IDC_RADIO_100K, &COziGenDlg::OnBnClickedRadioMapScale)
	ON_BN_CLICKED(IDC_RADIO_250K, &COziGenDlg::OnBnClickedRadioMapScale)
	ON_BN_CLICKED(IDC_RADIO_VML,  &COziGenDlg::OnBnClickedRadioMapScale)
	ON_BN_CLICKED(IDC_RADIO_SV,   &COziGenDlg::OnBnClickedRadioMapScale)
	ON_BN_CLICKED(IDC_RADIO_VDM,  &COziGenDlg::OnBnClickedRadioMapScale)
	ON_BN_CLICKED(IDC_RADIO_MS,   &COziGenDlg::OnBnClickedRadioMapScale)
	ON_BN_CLICKED(IDC_RADIO_OV,   &COziGenDlg::OnBnClickedRadioMapScale)

	ON_BN_CLICKED(IDC_CHECK_OFFLINE,  &COziGenDlg::OnBnClickedCheckOffline)
	ON_BN_CLICKED(IDC_CHECK_REPLACE,  &COziGenDlg::OnBnClickedCheckReplace)
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

	ON_CBN_SELCHANGE(IDC_COMBO_COUNTRY, &COziGenDlg::OnCbnSelchangeComboCountry)

	ON_CBN_SELCHANGE(IDC_COMBO_GFX_TYPE, &COziGenDlg::OnCbnSelchangeComboGfxType)
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
//pTabGrid->m_strGridRef = AfxGetApp()->GetProfileString("Main", "GridReference", "");
pTabGrid->UpdateData(false);

pTabMap = (CTabMap *)m_tabMyTabCtrl.m_tabPages[2];
pTabMap->m_nMapType = AfxGetApp()->GetProfileInt("Main", "MapType", 0);
pTabMap->m_nExplorerMap = AfxGetApp()->GetProfileInt("Main", "ExplorerMap", 0);
pTabMap->UpdateData(false);

//GetDlgItem(IDC_TAB_CTRL)->EnableWindow(false);

	m_strOutputDir = AfxGetApp()->GetProfileString("Main", "OutputDir", "");

	m_strMapFolder = AfxGetApp()->GetProfileString("Main", "MapDatabaseFolder", "C:\\OziGenDatabase");
	g_strMapDatabaseFolder = m_strMapFolder;

	m_nTileSize = AfxGetApp()->GetProfileInt("Main", "TileSize", 0);
	SetTileSize();

	m_nTrackSize = AfxGetApp()->GetProfileInt("Main", "TrackSize", 0);

	m_nDrawOrder = AfxGetApp()->GetProfileInt("Main", "DrawOrder", 50);

	m_bOffline = AfxGetApp()->GetProfileInt("Main", "Offline", 1);
	m_bReplace = AfxGetApp()->GetProfileInt("Main", "Replace", 0);
	m_bOptimize = AfxGetApp()->GetProfileInt("Main", "Optimize", 0);

	m_nDatabase = AfxGetApp()->GetProfileInt("Main", "RadioDatabase", DBASE_LOCAL);
	m_nMapType = AfxGetApp()->GetProfileInt("Main", "RadioMapType", MAP_OZI);
	m_nCountry = AfxGetApp()->GetProfileInt("Main", "RadioCountry", COUNTRY_OSGB);
	m_nScale = AfxGetApp()->GetProfileInt("Main", "RadioMapScale", MAP_SCALE_25K);
	m_nGpsModel = AfxGetApp()->GetProfileInt("Main", "RadioGpsModel", GPS_MODEL_OREGON);
	m_nCoordSystem = AfxGetApp()->GetProfileInt("Main", "RadioCoordSystem", COORD_SYSTEM_GRID);

	m_nCountry_new = AfxGetApp()->GetProfileInt("Main", "RadioCountry", COUNTRY_OSGB);
	m_nFlavour = AfxGetApp()->GetProfileInt("Main", "RadioFlavour", MAP_RASTER);

pTabGrid->m_strGridRef = AfxGetApp()->GetProfileString("Main", GetGridReferenceRegistryValues(m_nCountry), "");

	bool bShowOptimizeControls = (m_nMapType == MAP_KMZ);
	GetDlgItem(IDC_CHECK_OPTIMIZE)->EnableWindow(bShowOptimizeControls);
	GetDlgItem(IDC_COMBO_TRACK_SIZE)->EnableWindow(bShowOptimizeControls);

	bool bShowGarminControls = ((m_nMapType == MAP_KMZ) | (m_nMapType == MAP_JNX)); 
	GetDlgItem(IDC_EDIT_DRAW_ORDER)->EnableWindow(bShowGarminControls);
	GetDlgItem(IDC_COMBO_TILE_SIZE)->EnableWindow(bShowGarminControls);

	bool bShowOSProOptions = (m_nDatabase == DBASE_LOCAL);
	GetDlgItem(IDC_CHECK_OFFLINE)->EnableWindow(bShowOSProOptions);

	if (m_nDatabase == DBASE_LOCAL) {
		GetDlgItem(IDC_CHECK_REPLACE)->EnableWindow(!m_bOffline);
	} else {
		GetDlgItem(IDC_CHECK_REPLACE)->EnableWindow(false);
	}

	UpdateDatum();

	pTabGrid->UpdateGridExample();
	pTabMap->UpdateMapName();

	if (m_bOffline) {

		GetDlgItem(IDC_CHECK_REPLACE)->EnableWindow(false);
	}

// Hack to disable Online OSPRO - set to false to skip!
if(false) {

	GetDlgItem(IDC_CHECK_OFFLINE)->ShowWindow(false);
	GetDlgItem(IDC_CHECK_REPLACE)->ShowWindow(false);
	m_bOffline = true;
	m_bReplace = false;
}

// To Remove Once the PKG creation routines have been completed.
GetDlgItem(IDC_RADIO_MAP_PKG)->EnableWindow(false);

// To Remove Once the GPS Model routines have been completed.
GetDlgItem(IDC_RADIO_MONTANA)->EnableWindow(false);
GetDlgItem(IDC_RADIO_OREGON)->EnableWindow(false);
GetDlgItem(IDC_RADIO_DAKOTA)->EnableWindow(false);
GetDlgItem(IDC_RADIO_ETREX)->EnableWindow(false);
GetDlgItem(IDC_RADIO_GPS_6X)->EnableWindow(false);
GetDlgItem(IDC_RADIO_OREGON_6X)->EnableWindow(false);


// To Remove Once the Country Database routines have been added.
//GetDlgItem(IDC_RADIO_AT)->EnableWindow(false);
//GetDlgItem(IDC_RADIO_DE)->EnableWindow(false);
//GetDlgItem(IDC_RADIO_IT)->EnableWindow(false);
//GetDlgItem(IDC_RADIO_NL)->EnableWindow(false);

// To Remove Once the Map Scales routines have been added.
//GetDlgItem(IDC_RADIO_50K)->EnableWindow(false);
//GetDlgItem(IDC_RADIO_10K)->EnableWindow(false);

// To Remove Once the Coord routines have been added.
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

	DatabaseType nDatabase = DetermineDatabaseType(m_nDatabase, m_nCountry);

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

	if ((g_CoordinateSystem == COORD_FRANCE_LAMBERT_1993)
		&& ((!IsInIGNDatabase(m_mapBoundaries.top, m_mapBoundaries.left))
		&& (!IsInIGNDatabase(m_mapBoundaries.bottom, m_mapBoundaries.right))
		&& (!IsInIGNDatabase(m_mapBoundaries.CenterPoint().y, m_mapBoundaries.CenterPoint().x))) ){

		AddToResultsWindow("Error: the map is not supported in the IGN Database");
		return;
	}

	if ((g_CoordinateSystem == COORD_OSGB) 
		&& ((!IsInOSGBDatabase(m_mapBoundaries.top, m_mapBoundaries.left))
		&& (!IsInOSGBDatabase(m_mapBoundaries.bottom, m_mapBoundaries.right))
		&& (!IsInOSGBDatabase(m_mapBoundaries.CenterPoint().y, m_mapBoundaries.CenterPoint().x))) ){

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

	if ((nDatabase == DB_OSGB_Local && !bOffline)
	||	(nDatabase == DB_NGI_Local/*  && !bOffline*/) 
	||	(nDatabase == DB_Kompass_Local/*  && !bOffline*/) 
	){

int nDatum = GetDatum(m_nCountry);
int nMapScale = GetMapScale(m_nScale);
int nPixelsPerTile = GetTilePixelWidth(m_nFlavour, m_nCountry, m_nScale);
int nMetersPerTile = GetTileMeterWidth(m_nFlavour, m_nCountry, m_nScale);

		CTileDatabase	g_db(nDatum, nMetersPerTile, nMapScale, nPixelsPerTile);

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


		CButton *pReplace = (CButton *) GetDlgItem (IDC_CHECK_REPLACE);
		bool bReplace = false;	
		if (pReplace->GetCheck()) {
			bReplace = true;
		}

g_db.SetReadonly(false);

MakeDatabaseFolders(m_nFlavour, m_nCountry, m_nScale);
g_db.SetDatum(m_nCountry);

	if (nDatabase == DB_OSGB_Local) {

		long collar = CalculateCollar(minx, maxx, miny, maxy);

		minx = (minx/500*500) - collar;
		miny = (miny/500*500) - collar;
		maxx = ((maxx+499)/500*500) + collar;
		maxy = ((maxy+499)/500*500) + collar;

		OSPro_LoadTiles(&g_db, minx, miny, maxx, maxy, bReplace, m_nFlavour, m_nScale);

	} else if (nDatabase == DB_NGI_Local) {

//			minx = m_mapBoundaries.left/1000*1000;
//			miny = m_mapBoundaries.bottom/1000*1000;
//			maxx = (m_mapBoundaries.right+999)/1000*1000;
//			maxy = (m_mapBoundaries.top+999)/1000*1000;
		NGI_LoadTiles(&g_db, minx, miny, maxx, maxy, bReplace, m_nScale);

	} else if (nDatabase == DB_Kompass_Local) {

		Kompass_LoadTiles(&g_db, minx, miny, maxx, maxy, bReplace, m_nScale);
	}

g_db.SetReadonly(true);
	}

	if (m_nMapType == MAP_OZI || m_nMapType == MAP_JPR) {

		SetCurrentDirectory(m_strOutputDir);

		if (nDatabase == DB_IGN_Local) {
			
			long	minx, miny, maxx, maxy;
			minx = m_mapBoundaries.left/1000*1000;
			miny = m_mapBoundaries.bottom/1000*1000;
			maxx = (m_mapBoundaries.right+999)/1000*1000;
			maxy = (m_mapBoundaries.top+999)/1000*1000;

			IGN_Create_PNG_JPR(m_strMapName, minx, miny, maxx, maxy);

			if (m_nMapType == MAP_JPR) {

				IGN_dumpjprfile(m_strMapName, minx, miny, maxx, maxy, (maxx-minx)/1000 * IGN_TILE_WIDTH, (maxy-miny)/1000 * IGN_TILE_WIDTH, NULL);
			
			} else {

				IGN_Create_OZI_MAP(m_strMapName, minx, miny, maxx, maxy);
			}

		} else {

			Create_OZI_PNG(m_strMapName, m_mapBoundaries.left, m_mapBoundaries.right, m_mapBoundaries.bottom, m_mapBoundaries.top, m_nDatabase, m_nFlavour, m_nCountry, m_nScale);
//			Create_OZI_PNG(m_strMapName, m_mapBoundaries.left/1000, m_mapBoundaries.right/1000, m_mapBoundaries.bottom/1000, m_mapBoundaries.top/1000, m_nDatabase, m_nMapScale, m_nCountry);
			DumpOziFile(strOziPath, m_mapBoundaries);
		}

			AddToResultsWindow("Created OZI files for %s",m_strMapName);

	} else if (m_nMapType == MAP_KMZ) {

		if (m_bOptimize) {
			CString strMaptechName;
			strMaptechName = m_strOutputDir + "\\" + m_strMapName + ".txf";
			if (!IsFilePresent(strMaptechName)) {
				WriteMaptechTrack(strMaptechName, m_MyTrack);
				if (DecMapKMZ(m_strOutputDir, m_strMapName, m_mapBoundaries, m_tileWidth, m_tileHeight, nDatabase, m_nTrackSize, m_nDrawOrder, m_nFlavour, m_nCountry, m_nScale)) {
					AddToResultsWindow("Created Optimized KMZ file for %s",m_strMapName);
				} else {
					AddToResultsWindow("Couldn't create KMZ file for %s",m_strMapName);
				}
				DeleteFile(strMaptechName);
			} else {
				AddToResultsWindow("Error: Temporary maptech file %s.txf already exits!",m_strMapName);
			}
		} else {
			if (DecMapKMZ(m_strOutputDir, m_strMapName, m_mapBoundaries, m_tileWidth, m_tileHeight, nDatabase, m_nTrackSize, m_nDrawOrder, m_nFlavour, m_nCountry, m_nScale)) {
				AddToResultsWindow("Created KMZ file for %s",m_strMapName);
			} else {
				AddToResultsWindow("Couldn't create KMZ file for %s",m_strMapName);
			}
		}
	} else if (m_nMapType == MAP_JNX) {
		if (DecMapJNX(m_strOutputDir, m_strMapName, m_mapBoundaries, m_tileWidth, m_tileHeight, nDatabase, m_nDrawOrder, m_nFlavour, m_nCountry, m_nScale)) {
			AddToResultsWindow("Created JNX files for %s",m_strMapName);
		} else {
				AddToResultsWindow("Couldn't create JNX file for %s",m_strMapName);
		}
	} else if (m_nMapType == MAP_JPR) {		

		SetCurrentDirectory(m_strOutputDir);


		Create_OZI_PNG(m_strMapName, m_mapBoundaries.left, m_mapBoundaries.right, m_mapBoundaries.bottom, m_mapBoundaries.top, m_nDatabase, m_nFlavour, m_nCountry, m_nScale);
//		Create_OZI_PNG(m_strMapName, m_mapBoundaries.left/1000, m_mapBoundaries.right/1000, m_mapBoundaries.bottom/1000, m_mapBoundaries.top/1000, m_nDatabase, m_nMapScale, m_nCountry);

//		dumpjpr_file(m_strMapName, MyRect.left, MyRect.right, MyRect.bottom, MyRect.top, NULL);
//		Create_PNG_JPR(m_strMapName, MyRect.left, MyRect.right, MyRect.bottom, MyRect.top);
//		AddToResultsWindow("Created JPR files for %s",m_strMapName);

	} else if (m_nMapType == MAP_QCT) {		

		SetCurrentDirectory(m_strOutputDir);
		(void) MakeQCT_main(m_mapBoundaries.top, m_mapBoundaries.left, m_mapBoundaries.bottom, m_mapBoundaries.right, m_strMapName+".qct", m_nScale);
		AddToResultsWindow("Created QCT file for %s",m_strMapName);

	} else {

		AddToResultsWindow("Error: Unknown Output Type for %s",m_strMapName);
	}
}

void COziGenDlg::DisplayGridSize()
{
	UpdateData(true);
	DisplayMapData();
	UpdateData(false);
}

void COziGenDlg::OnEnKillfocusEditWest()
{
	UpdateData(true);
	m_mapBoundaries.left = atoi(m_strWest);
	DisplayGridSize();
}

void COziGenDlg::OnEnKillfocusEditEast()
{
	UpdateData(true);
	m_mapBoundaries.right = atoi(m_strEast);
	DisplayGridSize();
}

void COziGenDlg::OnEnKillfocusEditNorth()
{
	UpdateData(true);
	m_mapBoundaries.top = atoi(m_strNorth);
	DisplayGridSize();
}

void COziGenDlg::OnEnKillfocusEditSouth()
{
	UpdateData(true);
	m_mapBoundaries.bottom = atoi(m_strSouth);
	DisplayGridSize();
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

	UpdateData(false);
}

void COziGenDlg::DisplayMapData()
{
	UpdateData(true);

	char strBuffer[256];
	
	long GridWidth = labs((m_mapBoundaries.right-m_mapBoundaries.left));
	long GridHeight = labs((m_mapBoundaries.bottom-m_mapBoundaries.top));

	double dWidth = GridWidth / (m_tileWidth * 2.5);
	double dHeight = GridHeight / (m_tileHeight * 2.5);

	long nWidth = (long) ceil(dWidth);
	long nHeight = (long) ceil(dHeight);

	if (m_nMapType == MAP_OZI) {

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

void COziGenDlg::OnBnClickedRadioMap()
{
	UpdateData(true);

	AfxGetApp()->WriteProfileInt("Main", "RadioMapType", m_nMapType);

	DisplayMapData();

	bool bShowOptimizeControls = (m_nMapType == MAP_KMZ);
	GetDlgItem(IDC_CHECK_OPTIMIZE)->EnableWindow(bShowOptimizeControls);
	GetDlgItem(IDC_COMBO_TRACK_SIZE)->EnableWindow(bShowOptimizeControls);

	bool bShowGarminControls = ((m_nMapType == MAP_KMZ) | (m_nMapType == MAP_JNX)); 
	GetDlgItem(IDC_EDIT_DRAW_ORDER)->EnableWindow(bShowGarminControls);
	GetDlgItem(IDC_COMBO_TILE_SIZE)->EnableWindow(bShowGarminControls);

	UpdateData(false);
}

void COziGenDlg::OnBnClickedRadioDatabase()
{
	UpdateData(true);

	AfxGetApp()->WriteProfileInt("Main", "RadioDatabase", m_nDatabase);
	AfxGetApp()->WriteProfileInt("Main", "RadioCountry", m_nCountry);

m_nCountry_new = m_nCountry;

pTabGrid->m_strGridRef = AfxGetApp()->GetProfileString("Main", GetGridReferenceRegistryValues(m_nCountry), "");

	if (m_nCountry == COUNTRY_IGN) {

		g_CoordinateSystem = COORD_FRANCE_LAMBERT_1993;

	} else if (m_nCountry == COUNTRY_NGI) {

		g_CoordinateSystem = COORD_BELGIUM_LAMBERT_2008;

	} else if (m_nCountry == COUNTRY_DE) {

		g_CoordinateSystem = COORD_KOMPASS;

	} else {

		g_CoordinateSystem = COORD_OSGB;
	}

	pTabGrid->UpdateGridExample();

	bool bShowOSProOptions = (m_nDatabase == DBASE_LOCAL);
	GetDlgItem(IDC_CHECK_REPLACE)->EnableWindow(bShowOSProOptions);
	GetDlgItem(IDC_CHECK_OFFLINE)->EnableWindow(bShowOSProOptions);

	if (m_nCountry != COUNTRY_OSGB) {

		GetDlgItem(IDC_RADIO_DBASE_TRACKLOGS_2)->EnableWindow(false);
		GetDlgItem(IDC_RADIO_DBASE_TRACKLOGS_3)->EnableWindow(false);
	
		m_nDatabase = DBASE_LOCAL;

		AfxGetApp()->WriteProfileInt("Main", "RadioDatabase", m_nDatabase);

		m_bOffline = true;
		m_bReplace = false;

		GetDlgItem(IDC_CHECK_OFFLINE)->EnableWindow(false);
		GetDlgItem(IDC_CHECK_REPLACE)->EnableWindow(false);

	} else {

		GetDlgItem(IDC_RADIO_DBASE_TRACKLOGS_2)->EnableWindow(true);
		GetDlgItem(IDC_RADIO_DBASE_TRACKLOGS_3)->EnableWindow(true);

		GetDlgItem(IDC_CHECK_OFFLINE)->EnableWindow(true);
		GetDlgItem(IDC_CHECK_REPLACE)->EnableWindow(true);

	}

	UpdateData(false);
}

void COziGenDlg::OnBnClickedRadioCoords()
{
	m_nPreviousCoordsSystem = m_nCoordSystem;
	UpdateData(true);

	AfxGetApp()->WriteProfileInt("Main", "RadioCoordSystem", m_nCoordSystem);

	pTabGrid->UpdateGridReference();
	pTabGrid->UpdateGridExample();

	UpdateData(false);
}

void COziGenDlg::OnBnClickedRadioMapScale()
{
	UpdateData(true);
	AfxGetApp()->WriteProfileInt("Main", "RadioMapScale", m_nScale);
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
/*
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
*/
/*
N6000000.000000 E0.000000, lat=40.773244 lon=-5.274410
N6000000.000000 E1300000.000000, lat=40.860434 lon=10.099236
N7200000.000000 E0.000000, lat=51.491411 lon=-7.076480
N7200000.000000 E1300000.000000, lat=51.597421 lon=11.649413
*/

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	UpdateData(true);
	m_mapBoundaries.bottom -= 1000 * pNMUpDown->iDelta;
	m_strSouth.Format("%d",m_mapBoundaries.bottom);
	DisplayGridSize();
	UpdateData(false);
	*pResult = 0;
}

void COziGenDlg::UpdateDatum()
{
	if (m_nCountry == COUNTRY_OSGB) {
		g_CoordinateSystem = COORD_OSGB;
	} else {

		if (m_nCountry == COUNTRY_IGN) {
			g_CoordinateSystem = COORD_FRANCE_LAMBERT_1993;
		} else if (m_nCountry == COUNTRY_NGI) {
			g_CoordinateSystem = COORD_BELGIUM_LAMBERT_2008;
		} else if (m_nCountry == COUNTRY_DE) {
			g_CoordinateSystem = COORD_KOMPASS;
		}

		m_bOffline = true;
		m_bReplace = false;

		GetDlgItem(IDC_CHECK_OFFLINE)->EnableWindow(false);
		GetDlgItem(IDC_CHECK_REPLACE)->EnableWindow(false);
	}

	UpdateMapScaleRadioButtons();
}

void COziGenDlg::UpdateMapScaleRadioButtons()
{
	for (int i=0; i < NUMBER_MAPS_SCALES; i++) {
		
		int nShow = GetTileMeterWidth(m_nFlavour, m_nCountry, i);

		if (nShow == -1)
			GetDlgItem(IDC_RADIO_10K+i)->EnableWindow(false);
		else
			GetDlgItem(IDC_RADIO_10K+i)->EnableWindow(true);
	}
}

void COziGenDlg::OnCbnSelchangeComboCountry()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	AfxGetApp()->WriteProfileInt("Main", "RadioCountry", m_nCountry_new);
	m_nCountry = m_nCountry_new;

pTabGrid->m_strGridRef = AfxGetApp()->GetProfileString("Main", GetGridReferenceRegistryValues(m_nCountry), "");

	UpdateDatum();
	UpdateData(false);
}

void COziGenDlg::OnCbnSelchangeComboGfxType()
{
	UpdateData(true);
	UpdateMapScaleRadioButtons();
	AfxGetApp()->WriteProfileInt("Main", "RadioFlavour", m_nFlavour);
	UpdateData(false);
}
