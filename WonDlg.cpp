// WonDlg.cpp : implementation file
//

#include "stdafx.h"

#include "MineSweeper3D.h"
#include "WonDlg.h"
#include "strings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWonDlg dialog


CWonDlg::CWonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWonDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWonDlg)
	m_name = _T("");
	m_static1 = _T("");
	//}}AFX_DATA_INIT
}


void CWonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWonDlg)
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	DDX_Text(pDX, IDC_EDIT1, m_name);
	DDX_Text(pDX, IDC_STATIC1, m_static1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWonDlg, CDialog)
	//{{AFX_MSG_MAP(CWonDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWonDlg message handlers

BOOL CWonDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowText (GetString (MIDS_WINDLG_TITLE));
	m_static1 = GetString (MIDS_WINDLG_MESSAGE);
	UpdateData (FALSE);

	m_edit.SetFocus();
	return FALSE;  // Focus specificato
}
