// HallsOfFameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "library.h"
#include "MineSweeper3D.h"
#include "HallsOfFameDlg.h"
#include "vars.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern	"C" GLOBAL_VARS vars;

/////////////////////////////////////////////////////////////////////////////
// CHallsOfFameDlg dialog


CHallsOfFameDlg::CHallsOfFameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHallsOfFameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHallsOfFameDlg)
	//}}AFX_DATA_INIT
}


void CHallsOfFameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHallsOfFameDlg)
	DDX_Control(pDX, IDC_LIST2, m_list);
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

	// Aggiusta la listbox
	m_list.InsertColumn (0, "Map name", LVCFMT_LEFT, 100);
	m_list.InsertColumn (1, "Sweeper", LVCFMT_LEFT, 100);
	m_list.InsertColumn (2, "Time", LVCFMT_LEFT, 100);
	m_list.InsertColumn (3, "Date", LVCFMT_LEFT, 100);

	// Ciclo dei moduli
	for (int i=0, c = 0; i<vars.nMapModules; i++) {
		LVITEM item;
		item.mask = LVIF_TEXT;
/*		item.iItem = c++;
		item.iSubItem = 0;
		item.pszText = vars.records[i].moduleName;
		m_list.InsertItem (&item);
*/
		for (int j=0; j<vars.records[i].nMaps; j++) {
			item.iItem = c++;
			item.iSubItem = 0;
			item.pszText = vars.records[i].records[j].mapName;
			m_list.InsertItem (&item);
			item.iSubItem = 1;
			item.pszText = vars.records[i].records[j].name;
			m_list.SetItem (&item);
			item.iSubItem = 2;
			CString rec;
			rec.Format ("%u", vars.records[i].records[j].record); 
			item.pszText = (char*)LPCTSTR(rec);
			m_list.SetItem (&item);
			item.iSubItem = 3;
			item.pszText = getDate (vars.records[i].records[j].date);
			m_list.SetItem (&item);
		}
	}


	UpdateData (FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
