

extern void mouseButton (UINT, int, int);
extern BOOL mouseMove(int state, int dx, int dy);
extern void changeWindowSize(int width, int height);

extern void	updateDisplay();

extern BOOL	oglInit(struct MINESWEEPER_MAP map);
extern void	oglClose();

