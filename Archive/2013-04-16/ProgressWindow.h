#pragma once

// CProgressWindow dialog

#include "resource.h"		// main symbols

class CProgressWindow : public CDialog
{
	DECLARE_DYNAMIC(CProgressWindow)

public:
	CProgressWindow(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProgressWindow();

	void Initialize();

	void ResetProgressBar(CString strCaption, int nLimit);

	void SetRange(int nUpperLimt);
	void UpdateProgress(int nIndex, int nUpperLimit);

	void ProgressBar();

// Dialog Data
	enum { IDD = IDD_PROGRESS_WINDOW };

	bool m_Cancelled;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_progress_bar;
	afx_msg void OnBnClickedCancel();
	CString m_edit_message;

	int m_nIndex;
	int m_nLimit;

	CString m_strCaption;
};
