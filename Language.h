#if !defined(AFX_LANGUAGE_H__75C13A3B_35AF_4383_9E14_4A7F09ADEA50__INCLUDED_)
#define AFX_LANGUAGE_H__75C13A3B_35AF_4383_9E14_4A7F09ADEA50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Language.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLanguage dialog

class CLanguage : public CDialog
{
// Construction
public:
	CLanguage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLanguage)
	enum { IDD = IDD_LANG_DIALOG };
	CComboBox	m_combo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLanguage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLanguage)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void OnCancel();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LANGUAGE_H__75C13A3B_35AF_4383_9E14_4A7F09ADEA50__INCLUDED_)
