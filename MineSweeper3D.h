// MineSweeper3D.h : main header file for the MINESWEEPER3D application
//

#pragma once


#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMineSweeper3DApp:
// See MineSweeper3D.cpp for the implementation of this class
//

class CMineSweeper3DApp : public CWinApp
{
public:
	CMineSweeper3DApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMineSweeper3DApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMineSweeper3DApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

