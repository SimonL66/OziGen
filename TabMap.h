// TabMap.h : header file
//
#pragma once

class CTabMap : public CDialog
{
// Construction
public:
	CTabMap(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTabMap)
	enum { IDD = IDD_TAB_MAP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabMap)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabMap)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	int m_nMapType;
	int m_nExplorerMap;
	afx_msg void OnEnKillfocusEditMap();
	afx_msg void OnCbnSelchangeComboMapType();
	CString m_strMapName;
	afx_msg void OnDeltaposSpinMap(NMHDR *pNMHDR, LRESULT *pResult);
	void VerifyMapNumber(int iDelta);
	void UpdateMapName();

	BOOL m_bMapPrefix;
	BOOL m_bMapName;
};
