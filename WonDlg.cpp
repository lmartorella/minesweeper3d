// WonDlg.cpp : implementation file
//

#include "stdafx.h"

#include "MineSweeper3D.h"
#include "WonDlg.h"

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
	//}}AFX_DATA_INIT
}


void CWonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWonDlg)
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	DDX_Text(pDX, IDC_EDIT1, m_name);
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

	m_edit.SetFocus();
	return FALSE;  // Focus specificato
}
