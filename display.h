


#ifdef __cplusplus
extern "C" {
	int		mouseButton (UINT, int, int);
	int		mouseMove(int dx, int dy);
	void	changeWindowSize(int width, int height);

	void	updateDisplay();

	int		gameInit(struct MINESWEEPER_MAP * map);
	void	gameClose();

	int		oglInit ();
	void	oglClose ();

	void	pause();
	void	unpause();

	void	timerSet(int t);
}
#else
	extern int	mouseButton (UINT, int, int);
	extern int  mouseMove(int dx, int dy);
	extern void changeWindowSize(int width, int height);

	extern void	updateDisplay();

	extern int 	gameInit(struct MINESWEEPER_MAP * map, HDC hDC);
	extern void	gameClose();

	extern int	oglInit ();
	extern void	oglClose ();

	extern void	pause();
	extern void	unpause();

	extern void	timerSet(int t);
#endif

