

// Header for building map module for Minesweeper 3D Ver 0.61

#ifndef MINE_MODULE_HEADER
#define MINE_MODULE_HEADER

// This file is valid for module of version 1.0 (0x100)


// MAP *****************************************************************************
#define MAX_VERTEX_FACE 12								// max. num of vertex per face
#define MAX_NEIGHBOURS  12								// max. num of edges per face

struct	MINESWEEPER_VERTEX {
		GLfloat		x, y, z;							// a vertex
};

struct	MINESWEEPER_FACE {
		int		v[MAX_VERTEX_FACE];						// face definition
};

struct	MINESWEEPER_NEIGHBOUR {
		int		n[MAX_NEIGHBOURS];						// edge definition
};

struct	MINESWEEPER_MAP {

		DWORD	code;									// Map code

		int		nPlaces;								// places count
		int		nVertexes;								// vertexes count
		int		initialMines;							// mines count

		struct MINESWEEPER_VERTEX	* vertex;			// vertex array
		struct MINESWEEPER_FACE	* face;					// faces array

		struct MINESWEEPER_NEIGHBOUR *	neighbour;		// edges array
		int *	place;									// place content
		int		nMines;									// actual mines number
		int		nChecked;								// flags
};

// MAP *****************************************************************************




// MODULE *****************************************************************************

struct	MINE_MODULE_INFO {
	char		modname[32];							// module name
	char		author[32];								// module author 
	char		topic[1024];							// other info...
};


	
// For function explanation see SDK (library.txt)
#ifdef	THIS_IS_A_MODULE
extern "C" {
	void	_declspec (dllexport) GetMineInfo (int * version, MINE_MODULE_INFO * m);	
	void	_declspec (dllexport) GetMapCount (int * count);	
	void	_declspec (dllexport) GetMapName (int mapIdx, char * name);	
	void	_declspec (dllexport) BuildMap (int mapIdx, MINESWEEPER_MAP *);
	void	_declspec (dllexport) DestroyMap (int mapIdx, MINESWEEPER_MAP *);
	int		_declspec (dllexport) MouseMove (int mapIdx, GLfloat, GLfloat);
	void	_declspec (dllexport) SetCameraParams (int mapIdx, int, int);
	void	_declspec (dllexport) ResetMap (int mapIdx);
}
#endif		// THIS_IS_A_MODULE

#endif		// MINE_MODULE_HEADER
	
