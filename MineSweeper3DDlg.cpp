// MineSweeper3DDlg.cpp : implementation file
//

#include "stdafx.h"

#include "map.h"
#include "display.h"
#include "vars.h"

#include "MineSweeper3D.h"
#include "MineSweeper3DDlg.h"
#include "HallsOfFameDlg.h"
#include "WonDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern "C" {
	extern MINESWEEPER_MAP map;
	extern MINESWEEPER_MAPTYPE mapType[];
	extern int mapReady;

	extern int youWin, youLose, youWinRecord;

	extern struct GLOBAL_VARS vars;
}

/////////////////////////////////////////////////////////////////////////////
// CMineSweeper3DApp

BEGIN_MESSAGE_MAP(CMineSweeper3DApp, CWinApp)
	//{{AFX_MSG_MAP(CMineSweeper3DApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMineSweeper3DApp construction

CMineSweeper3DApp::CMineSweeper3DApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMineSweeper3DApp object

CMineSweeper3DApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMineSweeper3DApp initialization

BOOL CMineSweeper3DApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CMineSweeper3DDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}



/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_version;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_version = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_VERSION, m_version);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
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
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_COMMAND(IDM_FILTERING, OnFiltering)
	ON_WM_RBUTTONDBLCLK()
	ON_WM_LBUTTONDBLCLK()
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
    pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |/* accelerator | */
		               PFD_DOUBLEBUFFER;
    pfd.iPixelType   = PFD_TYPE_RGBA;
    pfd.cDepthBits   = 16;
    pfd.cColorBits   = 16;
	pfd.dwLayerMask	 = PFD_MAIN_PLANE;

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

#define EVENTTIMER 1

BOOL CMineSweeper3DDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	LoadSettings ();

	srand (time(NULL));

    if (!PrepareOpenGL()) 
		exit (1);

	pDC = GetDC();
    hRC = wglCreateContext(pDC->m_hDC);
    wglMakeCurrent(pDC->m_hDC, hRC);

	if (!QueryPerformanceFrequency (&perfPeriod)) {
		AfxMessageBox ("Performance Timer not supported by hardware", MB_OK);
		exit (1);
	}
	perfPeriod.QuadPart /= 10;			// decimi di secondo
	gaming = false;
	SetTimer (EVENTTIMER, 500, NULL);	// per rinfresco passivo

	cursorSetting = false;
	poll = false;

	if (!oglInit())
		exit (1);

	if (vars.filtering) 
		CheckMenuItem (GetMenu()->m_hMenu, IDM_FILTERING, MF_CHECKED | MF_BYCOMMAND);
	else
		CheckMenuItem (GetMenu()->m_hMenu, IDM_FILTERING, MF_UNCHECKED | MF_BYCOMMAND);

	return TRUE;  // return TRUE  unless you set the focus to a control
}



  
		

void	CMineSweeper3DDlg::NewGame (DWORD typeC)
{
	gaming = false;

	for (int i = 0; i < 4; i++)
		if (mapType[i].commandType == typeC)
			break;

	if (i >= 4)
		return;

	bool changed = (int)map.typeIndex != i;
	gameClose(changed == true);

	if (changed) {
		destroyMap ();
		if (!buildMap (i)) {
			AfxMessageBox ("Error building map", MB_OK);
			exit (1);
		}
	}

	if (!prepareMap (mapType[i].mines, changed == true)) {				// mette le mine
		AfxMessageBox ("Too neighbour in map preparation", MB_OK);
		exit (1);
	}

	if (!gameInit(changed == true))
		exit (1);

	QueryPerformanceCounter (&pauseCount);
	lastPerfCount = pauseCount;
	timerSet (0);

	PostMessage (WM_PAINT);
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
		// Conta i decimi di secondo.
		if (gaming) {
			LARGE_INTEGER li;

			QueryPerformanceCounter (&li);
			int c = int((li.QuadPart - lastPerfCount.QuadPart) / perfPeriod.QuadPart);
			timerSet (c);
		}

	    static PAINTSTRUCT ps;
		updateDisplay();
	    SwapBuffers(pDC->m_hDC);				/* nop if singlebuffered */
		BeginPaint(&ps);
		EndPaint(&ps);

		poll = false;
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
	if (!StoreSettings ()) 
		AfxMessageBox ("Unable to write CFG file.");

	gaming = false;
	gameClose(1);
	destroyMap ();
	oglClose();

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

	RECT rect;
	GetClientRect (&rect);
	changeWindowSize(rect.right, rect.bottom);
	PostMessage(WM_PAINT, 0, 0);

	cursorSetting = true;
}


void CMineSweeper3DDlg::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	if (gaming)
		ButtonDown (WM_RBUTTONDOWN, nFlags, point);
	CDialog::OnRButtonDblClk(nFlags, point);
}

void CMineSweeper3DDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (gaming)
		ButtonDown (WM_LBUTTONDOWN, nFlags, point);
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CMineSweeper3DDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (gaming)
		ButtonDown (WM_LBUTTONDOWN, nFlags, point);
	CDialog::OnLButtonDown(nFlags, point);
}

void CMineSweeper3DDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if (gaming)
		ButtonDown (WM_RBUTTONDOWN, nFlags, point);
	CDialog::OnRButtonDown(nFlags, point);
}

void CMineSweeper3DDlg::ButtonDown(DWORD button, UINT nFlags, CPoint point)
{
	int unlock;

	SetCapture();
	mx = point.x;
	my = point.y;
	if (button== WM_LBUTTONDOWN)
	    mouseButtonState |= 1;
	if (button == WM_RBUTTONDOWN)
	    mouseButtonState |= 2;
	if (mouseButton (button, mx, my, &unlock))
		PostMessage(WM_PAINT, 0, 0);

	if (unlock) {
		PauseGaming();
		CtrlForWin();
		PostMessage (WM_PAINT);
	}
}


void CMineSweeper3DDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (gaming)
		ButtonUp (WM_LBUTTONUP, nFlags, point);
	else if (mapReady)
		StartGaming();
	CDialog::OnLButtonUp(nFlags, point);
}

void CMineSweeper3DDlg::OnRButtonUp(UINT nFlags, CPoint point) 
{
	if (gaming)
		ButtonUp (WM_RBUTTONUP, nFlags, point);
	CDialog::OnRButtonUp(nFlags, point);
}


void CMineSweeper3DDlg::ButtonUp(DWORD button, UINT nFlags, CPoint point)
{
	int unlock;
	ReleaseCapture();
	mouseButtonState = 0;
	if (mouseButton (button, point.x, point.y, &unlock))
		PostMessage(WM_PAINT, 0, 0);
	if (unlock) {
		PauseGaming();
		CtrlForWin();
		PostMessage (WM_PAINT);
	}
}



void CMineSweeper3DDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	static int x, y;
	static CPoint p;

	if (gaming)
		if (cursorSetting) {
			GetCursorPos (&p);
			x = p.x, y = p.y;	
			cursorSetting = false;
		}
		else {
			GetCursorPos (&p);
			if ((x-p.x) != 0 || (y-p.y) != 0) {
				if (mouseMove(x - p.x, y - p.y))
					PostMessage(WM_PAINT, 0, 0);
				else 
					cursorSetting = true;
				
				SetCursorPos (x, y);
			}
		}

	CDialog::OnMouseMove(nFlags, point);
}



BOOL CMineSweeper3DDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	CAboutDlg * dlgAbout;
	CHallsOfFameDlg * dlgFame;
	int i;

	switch (wParam) {
	case IDM_ABOUTBOX:
		dlgAbout = new CAboutDlg;
		dlgAbout->DoModal();
		delete (dlgAbout);
		return TRUE;
	case IDM_FILE_EXIT:
		PostQuitMessage(0);
		return TRUE;
	case IDM_ICO1:
	case IDM_ICO2:
	case IDM_ICO3:
	case IDM_ICO4:
		NewGame (wParam);
		return TRUE;

	case IDM_HALLSOFFAME:
		dlgFame = new CHallsOfFameDlg;
		for (i = 0; i < NUMMAPTYPE; i++)
			dlgFame->fame[i] = vars.hallsOfFame[i];
		dlgFame->DoModal();
		delete (dlgFame);
		return TRUE;

	default:
		return CDialog::OnCommand(wParam, lParam);
	}
}



void CMineSweeper3DDlg::StartGaming()
{
	RECT rect;
	POINT p;

	// Coordinate schermo della finestra
	GetClientRect (&rect);
	p.x = rect.right / 2;
	p.y = rect.bottom / 2;
	ClientToScreen (&p);
	SetCursorPos (p.x, p.y);
	cursorSetting = true;

	ClientToScreen (&rect);
	ClipCursor (&rect);
	ShowCursor (FALSE);
	unpause();

	LARGE_INTEGER t;

	QueryPerformanceCounter (&t);
	t.QuadPart -= pauseCount.QuadPart;			// tempo di pausa
	lastPerfCount.QuadPart += t.QuadPart;
	gaming = true;
}


void CMineSweeper3DDlg::PauseGaming()
{
	QueryPerformanceCounter (&pauseCount);
	gaming = false;
	ClipCursor (NULL);
	ShowCursor (TRUE);
	pause();
}



BOOL CMineSweeper3DDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP) {
		if (int(pMsg->wParam) == VK_ESCAPE && pMsg->message == WM_KEYUP) {
			PauseGaming();
			PostMessage(WM_PAINT, 0, 0);
		}
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}





void CMineSweeper3DDlg::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent != EVENTTIMER)
		CDialog::OnTimer(nIDEvent);

	if (poll && gaming)
		PostMessage (WM_PAINT);
	poll = true;
}


BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strAbout;
	strAbout.LoadString(IDS_VERSION);

	m_version = "MineSweeper 3D Versione " + strAbout;
	UpdateData (FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMineSweeper3DDlg::CtrlForWin()
{
	if (youWinRecord) {
		CWonDlg dlg;
		dlg.DoModal();
		strncpy (vars.hallsOfFame[map.typeIndex].name, dlg.m_name, MAX_NAMELENGHT);
		PostMessage (WM_COMMAND, IDM_HALLSOFFAME);

		youWinRecord = 0;
	}
}

void CMineSweeper3DDlg::OnFiltering() 
{
	HMENU menu = GetMenu()->m_hMenu;
	DWORD checked = CheckMenuItem (menu, IDM_FILTERING, MF_CHECKED | MF_BYCOMMAND);
	if (checked == MF_CHECKED) {
		CheckMenuItem (menu, IDM_FILTERING, MF_UNCHECKED | MF_BYCOMMAND);
		vars.filtering = 0;
	}
	else 
		vars.filtering = 1;

	if (!rebuildTextures())
		exit (1);
}

