


#ifdef __cplusplus
extern "C" {
	void	changeWindowSize(int width, int height);
	DWORD	rebuildTextures();
	DWORD	buildStructure ();
	DWORD	postBuildMap();
	void	postDestroyMap ();
	void	placeNumbers ();

	int		gameInit(int rebuild);
	void	gameClose(int destroy);
	void	resetGame();
	DWORD	newGame(DWORD code);

	DWORD	mine_init ();
	void	mine_exit ();

	void	hideCrosshair();
	void	showCrosshair();
	void	timerSet(int t);

	void	updateDisplay();
	int		processMouseButton (UINT, int, int, int * unlock);

	void	default_DestroyMap (int, MINESWEEPER_MAP *);
}
#else

	extern void  changeWindowSize(int width, int height);
	extern DWORD rebuildTextures();
	extern DWORD buildStructure ();
	extern DWORD postBuildMap();
	extern void	postDestroyMap ();
	extern void	placeNumbers ();

	extern int 	gameInit(int rebuild);
	extern void	gameClose(int destroy);
	extern void	resetGame();
	extern DWORD newGame(DWORD code);

	extern DWORD mine_init();
	extern void	mine_exit();

	extern void	showCrosshair();
	extern void	hideCrosshair();
	extern void	timerSet(int t);
	
	extern void	updateDisplay();
	extern int	mouseButton (UINT, int, int, int * unlock);

	extern void	default_DestroyMap (int, struct MINESWEEPER_MAP *);
#endif

