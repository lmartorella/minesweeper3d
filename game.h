

#ifndef _GAME_MINESWEEPER_
#define _GAME_MINESWEEPER_




#define MAP_PLACE_CLEAR     0
#define	MAP_PLACE_MINE		0x80000000
#define	MAP_PLACE_COVERED   0x40000000
#define MAP_PLACE_FLAG		0x20000000
#define MAP_PLACE_FALSEFLAG 0x10000000
#define	MAP_PLACE_NUMMASK   0x00ff


#define		GAME_NORMAL					0x00000000
#define		GAME_ENDED					0x00000001
#define		GAME_WIN					0x00000010
#define		GAME_RECORD					0x00000100
#define		GAME_STOP					0x00001000


#define		MINE_LBUTTON			1
#define		MINE_RBUTTON			2
#define		MINE_UNLOCK				0x80000000


typedef void (*GETMINEINFO_TYPE)(unsigned int *, MINE_MODULE_INFO *);
typedef void (*GETMAPCOUNT_TYPE)(unsigned int *);
typedef void (*GETMAPNAME_TYPE)(int, char *);
typedef void (*BUILDMAP_TYPE)(int, MINESWEEPER_MAP *);
typedef void (*DESTROYMAP_TYPE)(int, MINESWEEPER_MAP *);
typedef void (*RESETMAP_TYPE)(int);
typedef int	 (*MOUSEMOVE_TYPE)(int, GLfloat, GLfloat);
typedef void (*SETCAMERAPARAMS_TYPE)(int, int, int);


	void	changeWindowSize(int width, int height);
	DWORD	rebuildTextures();
	DWORD	postBuildMap();
	void	postDestroyMap ();
	void	placeNumbers ();

	int		gameInit(int rebuild);
	void	gameClose();
	DWORD	newGame(DWORD code);

	DWORD	mine_init ();
	void	mine_exit ();

	void	hideCrosshair();
	void	showCrosshair();

	int		timerProc();

	DWORD	renderer();
	int		processMouseButton (UINT, int, int, int * unlock, int * status);
	int		mouseMove (int dx, int dy);

#endif