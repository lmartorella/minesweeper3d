


#ifdef __cplusplus
extern "C" {
	int		mouseButton (UINT, int, int);
	int		mouseMove(int dx, int dy);
	void	changeWindowSize(int width, int height);

	void	updateDisplay();

	int		oglInit(struct MINESWEEPER_MAP * map, HDC hDC);
	void	oglClose();
}
#else
	extern int	mouseButton (UINT, int, int);
	extern int  mouseMove(int dx, int dy);
	extern void changeWindowSize(int width, int height);

	extern void	updateDisplay();

	extern int 	oglInit(struct MINESWEEPER_MAP * map, HDC hDC);
	extern void	oglClose();
#endif

