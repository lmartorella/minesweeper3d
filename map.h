

#pragma once


#define MAX_VERTEX_FACE 12


#define MAP_PLACE_CLEAR     0
#define	MAP_PLACE_MINE		0x80000000
#define	MAP_PLACE_COVERED   0x40000000
#define	MAP_PLACE_NUMMASK   0xffff

struct	MINESWEEPER_VERTEX {
		float	x, y, z;
};

struct	MINESWEEPER_FACE {
		int		v[MAX_VERTEX_FACE];
};


struct	MINESWEEPER_MAP {

	// Discriminante
		DWORD	type;

	// Geometria
		int		nPlaces;
		int		nVertexes;

		struct MINESWEEPER_VERTEX	* vertex;
		/* respect Consistent orientation of vertex for culling */
		struct MINESWEEPER_FACE	* face;
		
	
	// Gioco
		UINT *	place;		// piazzole. -1 se c'è mina
		int		nMines;
};




#define	MAP_NUPICOSAHEDRON	1
#define	MAP_CUBE		    2



#ifdef __cplusplus
extern "C" {
	int	buildMap (MINESWEEPER_MAP * map, DWORD type, DWORD param);
	int	 destroyMap (MINESWEEPER_MAP * map);

	void prepareMap (MINESWEEPER_MAP * map, int mines);	
	void	vinsFunction (MINESWEEPER_MAP * map);		
	void	buildN_UpIcosahedron (MINESWEEPER_MAP * map, int n);
}
#else
	extern int	buildMap (struct MINESWEEPER_MAP * map, DWORD type, DWORD param);
	extern int  destroyMap (struct MINESWEEPER_MAP * map);

	extern void prepareMap (struct MINESWEEPER_MAP * map, int mines);	
	extern void	vinsFunction (struct MINESWEEPER_MAP * map);		
	extern void	buildN_UpIcosahedron (struct MINESWEEPER_MAP * map, int n);
#endif


