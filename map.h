

#pragma once


#define MAX_VERTEX_FACE 12


#define PLACE_COVERED	  -2
#define PLACE_MINECOVERED -1
#define PLACE_CLEAR       0


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
		
		/* For culling, GL_CW or GL_CCW */
		GLenum	cullingMode;

		/* TRUE -> Culling, FALSE -> zbuffer */
		BOOL	isConvex;

		
	// Gioco
		int *	place;		// piazzole. -1 se c'è mina
		int		nMines;
};




#define	MAP_ICOSAHEDRON		1
#define	MAP_CUBE		    2

extern BOOL buildMap (struct MINESWEEPER_MAP * map, DWORD type);
extern BOOL preDestroyMap (struct MINESWEEPER_MAP * map);
extern BOOL destroyMap (struct MINESWEEPER_MAP * map);

extern void prepareMap (struct MINESWEEPER_MAP * map, int mines);				


