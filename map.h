

#pragma once


#define MAX_VERTEX_FACE 12



#define PLACE_MINE	-1
#define PLACE_CLEAR  0


struct	MINESWEEPER_VERTEX {
		float	x, y, z;
};

struct	MINESWEEPER_FACE {
		int		v[MAX_VERTEX_FACE];
};


struct	MINESWEEPER_MAP {

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

extern void buildIcosahedron (struct MINESWEEPER_MAP * map);
extern void	vinsFunction (struct MINESWEEPER_MAP * map);
extern void buildCube (struct MINESWEEPER_MAP * map);

