// HallsOfFameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "map.h"
#include "MineSweeper3D.h"
#include "HallsOfFameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHallsOfFameDlg dialog


CHallsOfFameDlg::CHallsOfFameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHallsOfFameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHallsOfFameDlg)
	m_value1 = _T("");
	m_value2 = _T("");
	m_value3 = _T("");
	m_value4 = _T("");
	//}}AFX_DATA_INIT
}


void CHallsOfFameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHallsOfFameDlg)
	DDX_Text(pDX, IDC_PUN1, m_value1);
	DDX_Text(pDX, IDC_PUN2, m_value2);
	DDX_Text(pDX, IDC_PUN3, m_value3);
	DDX_Text(pDX, IDC_PUN4, m_value4);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHallsOfFameDlg, CDialog)
	//{{AFX_MSG_MAP(CHallsOfFameDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHallsOfFameDlg message handlers

BOOL CHallsOfFameDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_value1.Format ("%d\t%s", fame[0].time, fame[0].name);
	m_value2.Format ("%d\t%s", fame[1].time, fame[1].name);
	m_value3.Format ("%d\t%s", fame[2].time, fame[2].name);
	m_value4.Format ("%d\t%s", fame[3].time, fame[3].name);

	UpdateData (FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
