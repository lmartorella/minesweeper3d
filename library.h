

// Header esportabile

#ifndef MINE_MODULE_HEADER
#define MINE_MODULE_HEADER


// MAP *****************************************************************************
#define MAX_VERTEX_FACE 12
#define MAX_NEIGHBOURS  12

struct	MINESWEEPER_VERTEX {
		float	x, y, z;
};

struct	MINESWEEPER_FACE {
		int		v[MAX_VERTEX_FACE];
};

struct	MINESWEEPER_NEIGHBOUR {
		int		n[MAX_NEIGHBOURS];
};

struct	MINESWEEPER_MAP {

	// Tipo
		DWORD	code;

	// Geometria, ci pensa BuildMap
		int		nPlaces;
		int		nVertexes;
		int		initialMines;

		struct MINESWEEPER_VERTEX	* vertex;
		/* respect Consistent orientation of vertex for culling */
		struct MINESWEEPER_FACE	* face;

	// Gioco, ci pensa il main
		struct MINESWEEPER_NEIGHBOUR *	neighbour;
		int *	place;		// piazzole. -1 se c'è mina
		int		nMines;
		int		nChecked;
};

// MAP *****************************************************************************




// MODULE *****************************************************************************

struct	MINE_MODULE_INFO {
	char		name[32];
};


typedef void (*GETMINEINFO_TYPE)(unsigned int *, MINE_MODULE_INFO *);
typedef void (*GETMAPCOUNT_TYPE)(unsigned int *);
typedef void (*GETMAPNAME_TYPE)(int, char *);
typedef void (*BUILDMAP_TYPE)(int, MINESWEEPER_MAP *);
typedef void (*DESTROYMAP_TYPE)(int, MINESWEEPER_MAP *);
typedef void (*RESETMAP_TYPE)(int);
typedef int	 (*MOUSEMOVE_TYPE)(int, int, int);
typedef void (*SETCAMERAPARAMS_TYPE)(int, int, int);




#ifdef	THIS_IS_A_MODULE
extern "C" {
	void	_declspec (dllexport) GetMineInfo (int * version, MINE_MODULE_INFO * m);	
	void	_declspec (dllexport) GetMapCount (int * count);	
	void	_declspec (dllexport) GetMapName (int mapIdx, char * name);	
	void	_declspec (dllexport) BuildMap (int mapIdx, MINESWEEPER_MAP *);
	void	_declspec (dllexport) DestroyMap (int mapIdx, MINESWEEPER_MAP *);
	int		_declspec (dllexport) MouseMove (int mapIdx, int, int);
	void	_declspec (dllexport) SetCameraParams (int mapIdx, int, int);
	void	_declspec (dllexport) ResetMap (int mapIdx);
}
#endif		// THIS_IS_A_MODULE

	
#endif		// main
