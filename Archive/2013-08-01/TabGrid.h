// TabGrid.h : header file
//
#pragma once

class CTabGrid : public CDialog
{
// Construction
public:
	CTabGrid(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTabGrid)
	enum { IDD = IDD_TAB_GRID };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabGrid)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabGrid)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void UpdateGridExample();
	void UpdateGridReference();

	afx_msg void OnEnKillfocusEditGridRefInflate();
	afx_msg void OnEnKillfocusEditGridRef();
	afx_msg void OnBnClickedOk();
	CString m_strGridRef;
	int m_nGridRefInflate;
	CString m_strGridExample;
	afx_msg void OnDeltaposSpinGrid(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonMagic();
};
