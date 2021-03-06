// MyTabCtrl.cpp : implementation file
//
/////////////////////////////////////////////////////
// This class is provided as is and Ben Hill takes no
// responsibility for any loss of any kind in connection
// to this code.
/////////////////////////////////////////////////////
// Is is meant purely as a educational tool and may
// contain bugs.
/////////////////////////////////////////////////////
// ben@shido.fsnet.co.uk
// http://www.shido.fsnet.co.uk
/////////////////////////////////////////////////////
// Thanks to a mystery poster in the C++ forum on 
// www.codeguru.com I can't find your name to say thanks
// for your Control drawing code. If you are that person 
// thank you very much. I have been able to use some of 
// you ideas to produce this sample application.
/////////////////////////////////////////////////////

#include "stdafx.h"
//#include "MyTabExample.h"
#include "OziGen.h"
#include "MyTabCtrl.h"
#include "TabTrack.h"
#include "TabGrid.h"
#include "TabMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyTabCtrl

CMyTabCtrl::CMyTabCtrl()
{
	m_tabPages[0]=new CTabTrack;
	m_tabPages[1]=new CTabGrid;
	m_tabPages[2]=new CTabMap;

	m_nNumberOfPages=3;
}

CMyTabCtrl::~CMyTabCtrl()
{
	for(int nCount=0; nCount < m_nNumberOfPages; nCount++){
		delete m_tabPages[nCount];
	}
}

void CMyTabCtrl::Init()
{
//	m_tabCurrent=0;
	m_tabCurrent = AfxGetApp()->GetProfileInt("Main", "CurrentTab", 0);

	m_tabPages[0]->Create(IDD_TAB_TRACK, this);
	m_tabPages[1]->Create(IDD_TAB_GRID, this);
	m_tabPages[2]->Create(IDD_TAB_MAP, this);

	SetCurFocus(m_tabCurrent);

/*
	m_tabPages[0]->ShowWindow(SW_HIDE);
	m_tabPages[1]->ShowWindow(SW_HIDE);
	m_tabPages[2]->ShowWindow(SW_HIDE);
*/
	for(int nCount=0; nCount < m_nNumberOfPages; nCount++) {

		if (m_tabCurrent == nCount) {
			m_tabPages[nCount]->ShowWindow(SW_SHOW);
		} else {
			m_tabPages[nCount]->ShowWindow(SW_HIDE);
		}
	}

//	m_tabPages[m_tabCurrent]->ShowWindow(SW_SHOW);

	m_tabPages[m_tabCurrent]->SetFocus();

	SetRectangle();
}

void CMyTabCtrl::SetRectangle()
{
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	GetClientRect(&tabRect);
	GetItemRect(0, &itemRect);

	nX=itemRect.left;
	nY=itemRect.bottom+1;
	nXc=tabRect.right-itemRect.left-1;
	nYc=tabRect.bottom-nY-1;
/*
	m_tabPages[0]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
	for(int nCount=1; nCount < m_nNumberOfPages; nCount++){
		m_tabPages[nCount]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
	}
*/
	for(int nCount=0; nCount < m_nNumberOfPages; nCount++){
		if (m_tabCurrent == nCount) {
			m_tabPages[nCount]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
		} else {
			m_tabPages[nCount]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
		}
	}
}

BEGIN_MESSAGE_MAP(CMyTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CMyTabCtrl)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyTabCtrl message handlers

void CMyTabCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CTabCtrl::OnLButtonDown(nFlags, point);

	if(m_tabCurrent != GetCurFocus()){
		m_tabPages[m_tabCurrent]->ShowWindow(SW_HIDE);
		m_tabCurrent=GetCurFocus();
		m_tabPages[m_tabCurrent]->ShowWindow(SW_SHOW);
		m_tabPages[m_tabCurrent]->SetFocus();

		AfxGetApp()->WriteProfileInt("Main", "CurrentTab", m_tabCurrent);
	}
}
