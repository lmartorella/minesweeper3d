#if !defined(AFX_HALLSOFFAMEDLG_H__713BB8CC_93A3_483D_A2B8_532571744448__INCLUDED_)
#define AFX_HALLSOFFAMEDLG_H__713BB8CC_93A3_483D_A2B8_532571744448__INCLUDED_

#include "vars.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HallsOfFameDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHallsOfFameDlg dialog

class CHallsOfFameDlg : public CDialog
{
// Construction
public:
	SUPERSWEEPER fame[4];
	CHallsOfFameDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHallsOfFameDlg)
	enum { IDD = IDD_HALLSOFFAME };
	CString	m_value1;
	CString	m_value2;
	CString	m_value3;
	CString	m_value4;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHallsOfFameDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHallsOfFameDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HALLSOFFAMEDLG_H__713BB8CC_93A3_483D_A2B8_532571744448__INCLUDED_)
