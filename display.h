


#ifdef __cplusplus
extern "C" {
	BOOL mouseButton (UINT, int, int);
	BOOL mouseMove(int state, int dx, int dy);
	void changeWindowSize(int width, int height);

	void	updateDisplay();

	BOOL	oglInit(struct MINESWEEPER_MAP * map, HDC hDC);
	void	oglClose();
}
#else
	extern BOOL mouseButton (UINT, int, int);
	extern BOOL mouseMove(int state, int dx, int dy);
	extern void changeWindowSize(int width, int height);

	extern void	updateDisplay();

	extern BOOL	oglInit(struct MINESWEEPER_MAP * map, HDC hDC);
	extern void	oglClose();
#endif

