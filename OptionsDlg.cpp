// OptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MineSweeper3D.h"
#include "OptionsDlg.h"
#include "vars.h"
#include "strings.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog


extern	INI_VARS ini;


COptionsDlg::COptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionsDlg)
	m_lang = _T("");
	m_sens = _T("");
	//}}AFX_DATA_INIT
}


void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsDlg)
	DDX_Control(pDX, IDC_SLIDER1, m_slider);
	DDX_Control(pDX, IDC_COMBO1, m_combo);
	DDX_Text(pDX, IDC_LANG_STATIC, m_lang);
	DDX_Text(pDX, IDC_SENS_STATIC, m_sens);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
	//{{AFX_MSG_MAP(COptionsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg message handlers

BOOL COptionsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Setta stringhe
	SetWindowText (GetString (MIDS_OPTIONDLG));
	m_lang = GetString (MIDS_LANGUAGE);
	m_sens = GetString (MIDS_SENS);

	// Enumera i linguaggi
	CFileFind finder;
	if (finder.FindFile (CString(DATA_DIRECTORY) + "\\*.lng", 0)) {
		while (finder.FindNextFile()) {
			m_combo.AddString (finder.GetFileTitle());
		}
		m_combo.AddString (finder.GetFileTitle());
	}
	// Seleziona quella già inserita	
	m_combo.SelectString (0, ini.main_language);

	m_slider.SetRange (1, 100);
	m_slider.SetPos (int((ini.main_xSens - 0.1) / (3 - 0.1) * 100.0));

	UpdateData (FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionsDlg::OnCancel()
{
	CDialog::OnCancel();
}


void COptionsDlg::OnOK() 
{
	int idx = m_combo.GetCurSel();
	if (idx >= 0) {
		CString name;
		m_combo.GetLBText (idx, name);
		strcpy (ini.main_language, name);
	}

	GLfloat sens = GLfloat(m_slider.GetPos () / 100.0 * (3.0-0.1) + 0.1);
	ini.main_xSens = ini.main_ySens = sens;

	char ininame[128];
	if (SaveINI(ininame) != 0) 
		AfxMessageBox (GetString (MIDS_MAIN_STORESETTINGS), 
					   MB_OK | MB_ICONEXCLAMATION);

	
	CDialog::OnOK();
}
