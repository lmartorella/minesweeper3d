


#ifdef __cplusplus
extern "C" {
	int		mouseButton (UINT, int, int, int * unlock);
	int		mouseMove(int dx, int dy);
	void	changeWindowSize(int width, int height);

	void	updateDisplay();

	int		gameInit(int rebuild);
	void	gameClose(int destroy);

	int		oglInit ();
	void	oglClose ();

	void	pause();
	void	unpause();

	void	timerSet(int t);
}
#else
	extern int	mouseButton (UINT, int, int, int * unlock);
	extern int  mouseMove(int dx, int dy);
	extern void changeWindowSize(int width, int height);

	extern void	updateDisplay();

	extern int 	gameInit(int rebuild);
	extern void	gameClose(int destroy);

	extern int	oglInit ();
	extern void	oglClose ();

	extern void	pause();
	extern void	unpause();

	extern void	timerSet(int t);
#endif

