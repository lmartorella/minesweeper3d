

#pragma once


#define MAX_VERTEX_FACE 12
#define MAX_NEIGHBOURS  12

#define MAP_PLACE_CLEAR     0
#define	MAP_PLACE_MINE		0x80000000
#define	MAP_PLACE_COVERED   0x40000000
#define MAP_PLACE_FLAG		0x20000000
#define	MAP_PLACE_NUMMASK   0xffff



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

	// Discriminante
		DWORD	typeIndex;

	// Geometria
		int		nPlaces;
		int		nVertexes;

		struct MINESWEEPER_VERTEX	* vertex;
		/* respect Consistent orientation of vertex for culling */
		struct MINESWEEPER_FACE	* face;


	// NEIGHBOURs
		struct MINESWEEPER_NEIGHBOUR *	neighbour;
		
	// Gioco
		int *	place;		// piazzole. -1 se c'è mina
		int		nMines;
		int		nTotalMines;
		int		nChecked;
};



#define MAP_NULL			-1
#define	MAP_NUPICOSAHEDRON	1
#define	MAP_CUBE		    2

#define NUMMAPTYPE 4

struct MINESWEEPER_MAPTYPE {
	DWORD	commandType;
	int		mines;

	DWORD	type;
	DWORD	param;
}; 




#ifdef __cplusplus
extern "C" {
	int		buildMap (DWORD typeIdx);
	int		destroyMap ();

	int		prepareMap (int mines, int rebuild);	
	void	buildN_UpIcosahedron (MINESWEEPER_MAP * map, int n);
}
#else
	extern int	buildMap (DWORD typeIdx);
	extern int  destroyMap ();

	extern int  prepareMap (int mines, int rebuild);	
	extern void	buildN_UpIcosahedron (struct MINESWEEPER_MAP * map, int n);
#endif


