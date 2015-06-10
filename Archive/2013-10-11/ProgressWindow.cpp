// ProgressWindow.cpp : implementation file
//

#include "stdafx.h"
#include "OziGen.h"
#include "ProgressWindow.h"
#include "func.h"

// CProgressWindow dialog

IMPLEMENT_DYNAMIC(CProgressWindow, CDialog)

CProgressWindow::CProgressWindow(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressWindow::IDD, pParent)
	, m_edit_message(_T(""))
{
	m_Cancelled = false;
}

CProgressWindow::~CProgressWindow()
{
	CDialog::DestroyWindow();
}

void CProgressWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_progress_bar);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_edit_message);
}


BEGIN_MESSAGE_MAP(CProgressWindow, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CProgressWindow::OnBnClickedCancel)
END_MESSAGE_MAP()


// CProgressWindow message handlers

void CProgressWindow::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	m_Cancelled = true;
	OnCancel();
}

void CProgressWindow::Initialize()
{
	Create(IDD_PROGRESS_WINDOW);
	ShowWindow(SW_SHOW);

SetWindowText("OziGen");

	m_edit_message = "Initialising";
	m_progress_bar.SetRange(0, 100);
	m_progress_bar.SetPos(0);
	UpdateData(FALSE);

	DoEvents();
}

void CProgressWindow::ResetProgressBar(CString strCaption, int nLimit)
{
	m_nIndex = 1;
	m_nLimit = nLimit;
	m_strCaption = strCaption;

	m_edit_message.Format("%Initialising");
	m_progress_bar.SetRange(1, m_nLimit);
	m_progress_bar.SetPos(1);
	UpdateData(FALSE);

	DoEvents();
}

void CProgressWindow::SetRange(int nUpperLimt)
{
	m_progress_bar.SetRange(0, nUpperLimt);
}

void CProgressWindow::UpdateProgress(int nIndex, int nUpperLimit)
{
	m_progress_bar.SetPos(nIndex);
	m_edit_message.Format("Processing %d of %d", nIndex, nUpperLimit);
	UpdateData(FALSE);
}

bool CProgressWindow::ProgressBar()
{
	if (m_nIndex <= m_nLimit) {
		m_progress_bar.SetPos(m_nIndex);
		m_edit_message.Format("%s %d of %d", m_strCaption, m_nIndex, m_nLimit);
		UpdateData(FALSE);
		m_nIndex++;
	}

	DoEvents();

	if (m_Cancelled) {
		return false;
	} else {
		return true;
	}
}
