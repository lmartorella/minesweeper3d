// MineSweeper3DDlg.h : header file
//

#pragma once

#include "map.h"
#include "display.h"

/////////////////////////////////////////////////////////////////////////////
// CMineSweeper3DDlg dialog

class CMineSweeper3DDlg : public CDialog
{
// Construction
public:
	CMineSweeper3DDlg(CWnd* pParent = NULL);	// standard constructor
	bool	PrepareOpenGL();

// Dialog Data
	//{{AFX_DATA(CMineSweeper3DDlg)
	enum { IDD = IDD_MINESWEEPER3D_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMineSweeper3DDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

    HGLRC				hRC;							// opengl context 
	HPALETTE			hPalette;					/* custom palette (if needed) */
	MINESWEEPER_MAP		map;
	CDC			*		pDC;

	// Generated message map functions
	//{{AFX_MSG(CMineSweeper3DDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void ButtonUp (DWORD button, UINT nFlags, CPoint point);
	UINT mouseButtonState;
	int my;
	int mx;
	void ButtonDown (DWORD button, UINT nFlags, CPoint point);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.