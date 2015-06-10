// TabTrack.h : header file
//
#pragma once

#include "..\MapLib\Track.h"

class CTabTrack : public CDialog
{
// Construction
public:
	CTabTrack(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTabTrack)
	enum { IDD = IDD_TAB_TRACK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCTabTrack)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCTabTrack)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonGpsTrack();
	afx_msg void OnBnClickedButtonTrack();
	afx_msg void OnEnKillfocusEditTrackInflate();
	afx_msg void OnBnClickedOk();

public:
	int m_nTrackInflate;
	CTrack m_MyTrack;
	CRect m_rectBoundaries;

	CString m_strOutputDir;
	CString m_strMapName;
	CString m_strTrackName;
};
