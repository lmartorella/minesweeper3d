#if !defined(AFX_WONDLG_H__49F7DAD4_6290_4EA4_BEEA_B66C196F4E31__INCLUDED_)
#define AFX_WONDLG_H__49F7DAD4_6290_4EA4_BEEA_B66C196F4E31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WonDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWonDlg dialog

class CWonDlg : public CDialog
{
// Construction
public:
	CWonDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWonDlg)
	enum { IDD = IDD_RECORD };
	CEdit	m_edit;
	CString	m_name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWonDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWonDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WONDLG_H__49F7DAD4_6290_4EA4_BEEA_B66C196F4E31__INCLUDED_)
