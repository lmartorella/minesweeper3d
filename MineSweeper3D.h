// MineSweeper3D.h : main header file for the MINESWEEPER3D application
//

#if !defined(AFX_MINESWEEPER3D_H__9689DD3D_16EF_4157_93B7_3E4A8A30ECA4__INCLUDED_)
#define AFX_MINESWEEPER3D_H__9689DD3D_16EF_4157_93B7_3E4A8A30ECA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

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

#endif // !defined(AFX_MINESWEEPER3D_H__9689DD3D_16EF_4157_93B7_3E4A8A30ECA4__INCLUDED_)
