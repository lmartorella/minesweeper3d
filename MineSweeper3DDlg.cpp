// MineSweeper3DDlg.cpp : implementation file
//

#include "stdafx.h"

#include "library.h"
#include "game.h"
#include "vars.h"
#include "modules.h"
#include "strings.h"

#include "MineSweeper3D.h"
#include "MineSweeper3DDlg.h"
#include "HallsOfFameDlg.h"
#include "WonDlg.h"
#include "AboutDlg.h"
#include "OptionsDlg.h"



	extern GLOBAL_VARS	vars;
	extern INI_VARS		ini;
	
	extern MINE_MODULE_MAPDESC		* mapDescriptorList;
	extern  RECORD					* recordArray;

	extern unsigned int				timer;


static     HGLRC			hRC;						// opengl context 
static     HPALETTE			hPalette;					// custom palette (if needed) 


DWORD	prepareOpenGL(HDC hDC)
{
    PIXELFORMATDESCRIPTOR pfd;

    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize        = sizeof(pfd);
    pfd.nVersion     = 1;
    pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL /*| PFD_GENERIC_ACCELERATED */| 
		               PFD_DOUBLEBUFFER;
    pfd.iPixelType   = PFD_TYPE_RGBA;
    pfd.cDepthBits   = 16;
    pfd.cColorBits   = 16;
	pfd.dwLayerMask	 = PFD_MAIN_PLANE;

    int pf = ChoosePixelFormat(hDC, &pfd);
    if (pf == 0) 
		return MIDS_OPENGL_CHOOSEPIXELFORMAT;
 
    if (SetPixelFormat(hDC, pf, &pfd) == FALSE) 
		return MIDS_OPENGL_SETPIXELFORMAT;

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
    
		if (pfd.iPixelType == PFD_TYPE_RGBA) {
		    int redMask = (1 << pfd.cRedBits) - 1;
		    int greenMask = (1 << pfd.cGreenBits) - 1;
		    int blueMask = (1 << pfd.cBlueBits) - 1;
		    int i;

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

    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);
	return 0;
}




DWORD	unprepareOpenGL(HDC hDC)
{
	wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);

    if (hPalette)
		DeleteObject(hPalette);
	return 0;
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

	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// CMineSweeper3DDlg dialog

CMineSweeper3DDlg::CMineSweeper3DDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMineSweeper3DDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMineSweeper3DDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);

	mx = my = 0;
	mapReady = false;
	pDC = NULL;
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
	ON_WM_RBUTTONDBLCLK()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




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


	// INI
	char ininame[128];
	DWORD iniErr = LoadINI(ininame);

	// Stringhe - messaggi di errore - menus
	if (!LoadStrings ()) {
		AfxMessageBox ("Error in loading language\r\nErrore nel caricamento del linguaggio");
		exit(1);
	}
	
	if (iniErr != 0) {
		char temp[128];
		sprintf (temp, GetString (iniErr), ininame);
		AfxMessageBox (temp, MB_OK | MB_ICONEXCLAMATION);
	}

	// Apertura plugins - aggiornamento menu
	gaming = false;
	activeMap = 0;
	mapCodeProgressive = ID_NEWMAP_GAME;
	LoadPlugins();

	// In base ai plugins disponibili...
	char appData [MAX_PATH];
	if (SHGetSpecialFolderPath (NULL, 
							appData,
							CSIDL_APPDATA,
							1) != TRUE)
			strcpy (appData, ".\\");
	else {
		strcat (appData, "\\MineSweeper3D");
		CreateDirectory (appData, NULL);
		strcat (appData, "\\");
	}

	DWORD err = LoadSettings (appData);
    if (err) 
		AfxMessageBox (GetString(err), MB_OK | MB_ICONEXCLAMATION);
	
	// ...costruisce menu
	MINESetMenus();

	// OpenGL
	pDC = GetDC();
	err = prepareOpenGL(pDC->m_hDC);
    if (err) {
		AfxMessageBox (GetString(err), MB_OK | MB_ICONSTOP);
		DeleteStrings();
		exit (1);
	}

	// Timers
	srand (time(NULL));

	LARGE_INTEGER perfPeriod;
	if (!QueryPerformanceFrequency (&perfPeriod)) {
		AfxMessageBox (GetString(MIDS_MAIN_PERFTIMER), MB_OK | MB_ICONSTOP);
		DeleteStrings();
		exit (1);
	}

	SetTimer (EVENTTIMER, 
			  1000 / ini.perf_internalTimer, 
			  NULL);								// per rinfresco tastini, timer gen.

	minPeriodRefresh.QuadPart = (ini.perf_maxFPS > 0) ?
		perfPeriod.QuadPart / ini.perf_maxFPS
		: 0;										// max FPS
	
	cursorSetting = false;
	poll = 0;

	// Init game
	err = mine_init();
	if (err) {
		AfxMessageBox (GetString(err), MB_OK | MB_ICONSTOP);
		DeleteStrings();
		exit (1);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
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
	else if (pDC) {
		// Conta i decimi di secondo.
	    static PAINTSTRUCT ps;
		LARGE_INTEGER li;
		QueryPerformanceCounter (&li);

		if (li.QuadPart - lastFrameRendered.QuadPart >= minPeriodRefresh.QuadPart ||
			li.QuadPart < lastFrameRendered.QuadPart) {

			lastFrameRendered = li;

			if (!ended && firstClick) {
				DWORD d = GetTickCount();
				timer = ((d - lastPerfCount) / 10);		// Centesimi di secondo
			}

			renderer();
		    SwapBuffers(pDC->m_hDC);				
			poll = 0;
		}

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
	DWORD err = StoreSettings();
	if (err) 
		AfxMessageBox (GetString(err), MB_OK | MB_ICONEXCLAMATION);

	gaming = false;
	gameClose();
	mine_exit();

	if (pDC) {
		unprepareOpenGL (pDC->m_hDC);
	    ReleaseDC(pDC);
	}
	DestroyDescriptorLists ();
	

	DeleteStrings();
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
	int unlock, status;

	SetCapture();
	mx = point.x;
	my = point.y;
	if (processMouseButton (button, mx, my, &unlock, &status))
		PostMessage(WM_PAINT, 0, 0);

	if (unlock) {
		PauseGaming();
		PostMessage (WM_PAINT);
	}

//	CtrlForEnd(status);
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
	int unlock, status;
	ReleaseCapture();
//	mouseButtonState = 0;
	if (processMouseButton (button, point.x, point.y, &unlock, &status))
		PostMessage(WM_PAINT, 0, 0);
	if (unlock) {
		PauseGaming();
		PostMessage (WM_PAINT);
	}

	CtrlForEnd(status);
}




void	CMineSweeper3DDlg::CtrlForEnd (int status)
{
	if (status & GAME_ENDED) {
		ended = true;
		PauseGaming();
	}
	if (status & GAME_RECORD && winner) {
		winner = 0;
		Record();
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

	switch (wParam) {
	case IDM_ABOUTBOX:
	case MIDM_ABOUT:
		dlgAbout = new CAboutDlg;
		dlgAbout->DoModal();
		delete (dlgAbout);
		return TRUE;
	case MIDM_EXIT:
		PostQuitMessage(0);
		return TRUE;
	case MIDM_HALLSOFFAME:
		dlgFame = new CHallsOfFameDlg;
		dlgFame->DoModal();
		delete (dlgFame);
		return TRUE;
	case MIDM_RESTART_GAME:
		NewGame (activeMap);	
		return TRUE;
	case MIDM_HELP:
		GotoURL("help\\index.html", 1);
		return TRUE;
	case MIDM_OPTIONS:
		{
			COptionsDlg dlg;
			dlg.DoModal();
			if (!LoadStrings ()) {
				AfxMessageBox ("Error in loading language\r\nErrore nel caricamento del linguaggio");
				exit(1);
			}
			MINESetMenus();
		}

	default:
		if (wParam >= ID_NEWMAP_GAME && wParam < ID_NEWMAP_GAME + MAX_MAPS_COUNT) {
			NewGame (wParam);
			return TRUE;
		}
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
	while (ShowCursor (FALSE) >= 0);
	showCrosshair();

	if (!firstClick) {
		lastPerfCount = GetTickCount ();
		firstClick = true;
	}
	gaming = true;
}


void CMineSweeper3DDlg::PauseGaming()
{
	gaming = false;
	ClipCursor (NULL);
	while (ShowCursor (TRUE) < 0);
	hideCrosshair();
	PostMessage(WM_PAINT, 0, 0);
}



BOOL CMineSweeper3DDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP) {
		if (int(pMsg->wParam) == VK_ESCAPE && pMsg->message == WM_KEYUP) {
			PauseGaming();
			PostMessage(WM_PAINT, 0, 0);
		}
		else if (int(pMsg->wParam) == VK_F2 && pMsg->message == WM_KEYDOWN) {
			if (activeMap != 0)
				NewGame (activeMap);	
		}
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}





void CMineSweeper3DDlg::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent != EVENTTIMER)
		CDialog::OnTimer(nIDEvent);

	if (timerProc() || (++poll >= ini.perf_idleRefresh && mapReady)) {
		PostMessage (WM_PAINT);
		poll = 0;
	}
}






void CMineSweeper3DDlg::OnOK()
{
}





void CMineSweeper3DDlg::LoadPlugins()
{
	CFileFind finder;
	BOOL work = finder.FindFile (PLUGINS_DIRECTORY);
	while (work) {
		work = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		if (finder.IsDirectory())
			continue;
		// Trovata una dll.

		DWORD err = ReadLibrary (finder.GetFilePath(), &mapCodeProgressive);
		if (err) {
			CString e, f;
			e = GetString (err);
			f.Format (e, finder.GetFilePath());
			AfxMessageBox (f, MB_OK | MB_ICONEXCLAMATION);
		}
	}
}



void CMineSweeper3DDlg::Record()
{
	CWonDlg dlg;
	dlg.DoModal();

	strncpy (recordArray->name, dlg.m_name, MINE_MODULE_NAMESIZE);
	
	SYSTEMTIME t;
	GetSystemTime (&t);
	recordArray->date = buildDate(t.wDay, t.wMonth, t.wYear);
}





































// MFC**************
// FindMenuItem() will find a menu item string from the specified
// popup menu and returns its position (0-based) in the specified
// popup menu. It returns -1 if no such menu item string is found.

int FindMenuItem(CMenu* Menu, const char * const MenuString)
{
   int count = Menu->GetMenuItemCount();
   for (int i = 0; i < count; i++) {
      CString str;
      if (Menu->GetMenuString(i, str, MF_BYPOSITION) &&
         (strcmp(str, MenuString) == 0))
         return i;
   }
   return -1;
}
int FindMenuItem(CMenu* Menu, DWORD command)
{
   int count = Menu->GetMenuItemCount();
   for (int i = 0; i < count; i++) {
      CString str;
      if (Menu->GetMenuItemID(i) == command)
         return i;
   }
   return -1;
}



void	CMineSweeper3DDlg::NewGame (DWORD code)
{
	gaming = false;
	ended = false;
	timer = 0;

	PauseGaming();

	DWORD err = newGame (code);
	if (err) {
		AfxMessageBox(GetString(err), MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	activeMap = code;
	winner = 1;

	mouseMove (0, 0);

	lastFrameRendered.QuadPart = 0;
	firstClick = false;

	RECT rect;
	GetClientRect (&rect);
	changeWindowSize(rect.right, rect.bottom);
	PostMessage (WM_PAINT);
	mapReady = true;

	CMenu * menu = GetMenu ();
	int f = FindMenuItem (menu, GetString(MIDM_GAME));
	menu = menu->GetSubMenu(f);
	MINEEnableMenuItem (MIDM_RESTART_GAME, 1);
}

int		CMineSweeper3DDlg::MINECheckMenuItem (DWORD code, int ch)
{
	DWORD comm = ch ? MF_CHECKED : MF_UNCHECKED;
	DWORD r = CheckMenuItem (GetMenu()->m_hMenu, code, MF_BYCOMMAND | ch);
	return (r == MF_CHECKED) ? 1 : 0;
}

void	CMineSweeper3DDlg::MINEEnableMenuItem (DWORD code, int ch)
{
	DWORD comm = ch ? MF_ENABLED : MF_GRAYED;
	EnableMenuItem (GetMenu()->m_hMenu, MF_BYCOMMAND | code, comm);
}





void	CMineSweeper3DDlg::MINESetMenus ()
{
	// Struttura attuale del menu
	static CMenu menu, menuGame, submenuGame, viewMenu, helpMenu;
	menuGame.DestroyMenu();
	menuGame.CreateMenu();
	submenuGame.DestroyMenu();
	submenuGame.CreateMenu();
	menu.DestroyMenu();
	menu.CreateMenu();
	viewMenu.DestroyMenu();
	viewMenu.CreateMenu();
	helpMenu.DestroyMenu();
	helpMenu.CreateMenu();


	menuGame.InsertMenu (-1, MF_BYPOSITION | MF_STRING | MF_GRAYED, 
						    MIDM_RESTART_GAME, 
							GetString (MIDM_RESTART_GAME));


	// Menu mappe
	MINE_MODULE_MAPDESC * p = mapDescriptorList;
	while (p != NULL) { 
		for (unsigned int i = 0; i < p->nMaps; i++)
			submenuGame.InsertMenu (-1, MF_BYPOSITION | MF_STRING, 
									 p->mapDesc[i].code, 
									 p->mapDesc[i].name);
		p = p->next;
		if (p != NULL)
			submenuGame.InsertMenu (-1, MF_BYPOSITION | MF_SEPARATOR);
	}

	menuGame.InsertMenu (-1, MF_BYPOSITION | MF_POPUP | MF_STRING, 
						    (DWORD)submenuGame.m_hMenu, 
							GetString (MIDM_NEW_GAME));
	menuGame.InsertMenu (-1, MF_BYPOSITION | MF_SEPARATOR);
	menuGame.InsertMenu (-1, MF_BYPOSITION | MF_STRING, MIDM_OPTIONS,
							GetString (MIDM_OPTIONS));
	menuGame.InsertMenu (-1, MF_BYPOSITION | MF_SEPARATOR);
	menuGame.InsertMenu (-1, MF_BYPOSITION | MF_STRING, 
						    MIDM_EXIT, 
							GetString (MIDM_EXIT));

	menu.InsertMenu		(-1, MF_BYPOSITION | MF_STRING | MF_POPUP, 
						    DWORD(menuGame.m_hMenu), 
							GetString (MIDM_GAME));

	viewMenu.InsertMenu (-1, MF_BYPOSITION | MF_STRING,
							MIDM_HALLSOFFAME,
							GetString(MIDM_HALLSOFFAME));
	menu.InsertMenu		(-1, MF_BYPOSITION | MF_STRING | MF_POPUP, 
						    DWORD(viewMenu.m_hMenu), 
							GetString (MIDM_VIEW));

	helpMenu.InsertMenu (-1, MF_BYPOSITION | MF_STRING,
							MIDM_HELP,
							GetString(MIDM_HELP));
	helpMenu.InsertMenu (-1, MF_BYPOSITION | MF_SEPARATOR);
	helpMenu.InsertMenu (-1, MF_BYPOSITION | MF_STRING,
							MIDM_ABOUT,
							GetString(MIDM_ABOUT));
	menu.InsertMenu		(-1, MF_BYPOSITION | MF_STRING | MF_POPUP, 
						    DWORD(helpMenu.m_hMenu), 
							GetString (MIDM_HELP));
	
	SetMenu(&menu);
}



