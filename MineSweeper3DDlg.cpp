// MineSweeper3DDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MineSweeper3D.h"
#include "MineSweeper3DDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMineSweeper3DDlg dialog

CMineSweeper3DDlg::CMineSweeper3DDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMineSweeper3DDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMineSweeper3DDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


	mx = my = 0;
	mouseButtonState = 0;
}

void CMineSweeper3DDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMineSweeper3DDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMineSweeper3DDlg, CDialog)
	//{{AFX_MSG_MAP(CMineSweeper3DDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_CHAR()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




bool	CMineSweeper3DDlg::PrepareOpenGL()
{
	CDC * pDC = GetDC();
	HDC hDC = pDC->m_hDC;

    PIXELFORMATDESCRIPTOR pfd;

    /* there is no guarantee that the contents of the stack that become
       the pfd are zeroed, therefore _make sure_ to clear these bits. */
    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize        = sizeof(pfd);
    pfd.nVersion     = 1;
    pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | 
		               PFD_GENERIC_ACCELERATED | PFD_DOUBLEBUFFER;
    pfd.iPixelType   = PFD_TYPE_RGBA;
    pfd.cDepthBits   = 32;
    pfd.cColorBits   = 16;

    int pf = ChoosePixelFormat(hDC, &pfd);
    if (pf == 0) {
		AfxMessageBox("ChoosePixelFormat() failed:  "
		   "Cannot find a suitable pixel format.", MB_OK); 
		return false;
    } 
 
    if (SetPixelFormat(hDC, pf, &pfd) == FALSE) {
		MessageBox("SetPixelFormat() failed:  "
			   "Cannot set format specified.", MB_OK);
		return false;
    } 

    DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    LOGPALETTE* lpPal;

    if (pfd.dwFlags & PFD_NEED_PALETTE ||
	pfd.iPixelType == PFD_TYPE_COLORINDEX) {

		int n = 1 << pfd.cColorBits;
		if (n > 256) n = 256;

		lpPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) +
					    sizeof(PALETTEENTRY) * n);
		memset(lpPal, 0, sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * n);
		lpPal->palVersion = 0x300;
		lpPal->palNumEntries = n;

		GetSystemPaletteEntries(hDC, 0, n, &lpPal->palPalEntry[0]);
    
		/* if the pixel type is RGBA, then we want to make an RGB ramp,
		   otherwise (color index) set individual colors. */
		if (pfd.iPixelType == PFD_TYPE_RGBA) {
		    int redMask = (1 << pfd.cRedBits) - 1;
		    int greenMask = (1 << pfd.cGreenBits) - 1;
		    int blueMask = (1 << pfd.cBlueBits) - 1;
		    int i;

			/* fill in the entries with an RGB color ramp. */
			for (i = 0; i < n; ++i) {
				lpPal->palPalEntry[i].peRed = 
				    (((i >> pfd.cRedShift)   & redMask)   * 255) / redMask;
				lpPal->palPalEntry[i].peGreen = 
				    (((i >> pfd.cGreenShift) & greenMask) * 255) / greenMask;
				lpPal->palPalEntry[i].peBlue = 
				    (((i >> pfd.cBlueShift)  & blueMask)  * 255) / blueMask;
				lpPal->palPalEntry[i].peFlags = 0;
			}
		} else {
		    lpPal->palPalEntry[0].peRed = 0;
		    lpPal->palPalEntry[0].peGreen = 0;
		    lpPal->palPalEntry[0].peBlue = 0;
		    lpPal->palPalEntry[0].peFlags = PC_NOCOLLAPSE;
		    lpPal->palPalEntry[1].peRed = 255;
		    lpPal->palPalEntry[1].peGreen = 0;
		    lpPal->palPalEntry[1].peBlue = 0;
		    lpPal->palPalEntry[1].peFlags = PC_NOCOLLAPSE;
		    lpPal->palPalEntry[2].peRed = 0;
		    lpPal->palPalEntry[2].peGreen = 255;
		    lpPal->palPalEntry[2].peBlue = 0;
		    lpPal->palPalEntry[2].peFlags = PC_NOCOLLAPSE;
		    lpPal->palPalEntry[3].peRed = 0;
		    lpPal->palPalEntry[3].peGreen = 0;
		    lpPal->palPalEntry[3].peBlue = 255;
		    lpPal->palPalEntry[3].peFlags = PC_NOCOLLAPSE;
		}

		hPalette = CreatePalette(lpPal);
		if (hPalette) {
		    SelectPalette(hDC, hPalette, FALSE);
		    RealizePalette(hDC);
		}
		free(lpPal);
    }

    ReleaseDC(pDC);
	return true;
}




/////////////////////////////////////////////////////////////////////////////
// CMineSweeper3DDlg message handlers

BOOL CMineSweeper3DDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	srand (time(NULL));

    if (!PrepareOpenGL()) 
		exit (1);

	pDC = GetDC();
    hRC = wglCreateContext(pDC->m_hDC);
    wglMakeCurrent(pDC->m_hDC, hRC);

	if (!buildMap (&map, MAP_NUPICOSAHEDRON, 3)) {
		AfxMessageBox ("Error building map", MB_OK);
		exit (1);
	}

	prepareMap (&map, 8);				// mette le mine

	if (!oglInit(&map, pDC->m_hDC))
		exit (1);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMineSweeper3DDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMineSweeper3DDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else {
	    static PAINTSTRUCT ps;
		updateDisplay();
		BeginPaint(&ps);
		EndPaint(&ps);
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMineSweeper3DDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CMineSweeper3DDlg::DestroyWindow() 
{
	oglClose();
	destroyMap (&map);
    wglMakeCurrent(NULL, NULL);
    ReleaseDC(pDC);
    wglDeleteContext(hRC);

    if (hPalette)
		DeleteObject(hPalette);
	
	return CDialog::DestroyWindow();
}

void CMineSweeper3DDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	RECT rect;
	GetClientRect (&rect);
	changeWindowSize(rect.right, rect.bottom);
	PostMessage(WM_PAINT, 0, 0);
}

void CMineSweeper3DDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	switch (nChar) {
	case 27:			/* ESC key */
	    PostQuitMessage(0);
	    break;
	}
	CDialog::OnChar(nChar, nRepCnt, nFlags);
}

void CMineSweeper3DDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	ButtonDown (WM_LBUTTONDOWN, nFlags, point);
	CDialog::OnLButtonDown(nFlags, point);
}

void CMineSweeper3DDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	ButtonDown (WM_RBUTTONDOWN, nFlags, point);
	CDialog::OnRButtonDown(nFlags, point);
}

void CMineSweeper3DDlg::ButtonDown(DWORD button, UINT nFlags, CPoint point)
{
	SetCapture();
	mx = point.x;
	my = point.y;
	if (button== WM_LBUTTONDOWN)
	    mouseButtonState |= 1;
	if (button == WM_RBUTTONDOWN)
	    mouseButtonState |= 2;
	if (mouseButton (button, mx, my))
		PostMessage(WM_PAINT, 0, 0);
}


void CMineSweeper3DDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ButtonUp (WM_RBUTTONUP, nFlags, point);
	CDialog::OnLButtonUp(nFlags, point);
}

void CMineSweeper3DDlg::OnRButtonUp(UINT nFlags, CPoint point) 
{
	ButtonUp (WM_RBUTTONUP, nFlags, point);
	CDialog::OnRButtonUp(nFlags, point);
}


void CMineSweeper3DDlg::ButtonUp(DWORD button, UINT nFlags, CPoint point)
{
	ReleaseCapture();
	mouseButtonState = 0;
	if (mouseButton (button, point.x, point.y))
		PostMessage(WM_PAINT, 0, 0);
}



void CMineSweeper3DDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (mouseButtonState) {
		int omx = mx;
		int omy = my;
		mx = point.x;
		my = point.y;
		if (mouseMove(mouseButtonState, omx - mx, my - omy))
			PostMessage(WM_PAINT, 0, 0);
	}	
	CDialog::OnMouseMove(nFlags, point);
}

