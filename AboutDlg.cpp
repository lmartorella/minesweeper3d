
#include "stdafx.h"
#include "MineSweeper3D.h"
#include "AboutDlg.h"
#include "strings.h"


CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_version = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_LINK, m_link);
	DDX_Text(pDX, IDC_VERSION, m_version);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_version = GetString(MIDS_ABOUT_CONTENT);
	SetWindowText (GetString (MIDS_ABOUT_TITLE));
	UpdateData (FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}





