// Language.cpp : implementation file
//

#include "stdafx.h"
#include "MineSweeper3D.h"
#include "Language.h"
#include "vars.h"
#include "strings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern INI_VARS ini;

/////////////////////////////////////////////////////////////////////////////
// CLanguage dialog


CLanguage::CLanguage(CWnd* pParent /*=NULL*/)
	: CDialog(CLanguage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLanguage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLanguage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLanguage)
	DDX_Control(pDX, IDC_COMBO_LANG, m_combo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLanguage, CDialog)
	//{{AFX_MSG_MAP(CLanguage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLanguage message handlers

BOOL CLanguage::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Enumera i linguaggi
	CFileFind finder;
	if (finder.FindFile (CString(DATA_DIRECTORY) + "\\*.lng", 0)) {
		while (finder.FindNextFile()) {
			m_combo.AddString (finder.GetFileTitle());
		}
		m_combo.AddString (finder.GetFileTitle());
	}
	m_combo.SelectString (0, ini.main_language);

	// Seleziona quella già inserita
	return TRUE;  
}



void CLanguage::OnCancel()
{
}


void CLanguage::OnOK()
{
	int idx = m_combo.GetCurSel();
	if (idx >= 0) {
		CString name;
		m_combo.GetLBText (idx, name);
		strcpy (ini.main_language, name);
	}

	char ininame[128];
	if (SaveINI(ininame) != 0) 
		AfxMessageBox (GetString (MIDS_MAIN_STORESETTINGS), 
					   MB_OK | MB_ICONEXCLAMATION);

	CDialog::OnOK();
}


