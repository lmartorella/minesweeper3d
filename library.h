


#ifndef MINE_MODULE_HEADER
#define MINE_MODULE_HEADER

#include <GL\gl.h>


// MAP *****************************************************************************

#define MAX_VERTEX_FACE 12
#define MAX_NEIGHBOURS  12

#define MAP_PLACE_CLEAR     0
#define	MAP_PLACE_MINE		0x80000000
#define	MAP_PLACE_COVERED   0x40000000
#define MAP_PLACE_FLAG		0x20000000
#define MAP_PLACE_FALSEFLAG 0x10000000
#define	MAP_PLACE_NUMMASK   0x00ff


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


#define		GAME_STATUS_NORMAL				0
#define		GAME_STATUS_ENDED_LOST			1
#define		GAME_STATUS_ENDED_WIN			2
#define		GAME_STATUS_STOPPED				3
#define		GAME_STATUS_ENDED_WINRECORD 	4


#define		MINE_LBUTTON			1
#define		MINE_RBUTTON			2
#define		MINE_UNLOCK				0x80000000






// MODULE *****************************************************************************

struct	MINE_MODULE_INFO {
	char		name[32];
};


typedef void (*GETMINEINFO_TYPE)(int *, struct MINE_MODULE_INFO *);
typedef void (*GETMAPCOUNT_TYPE)(int *);
typedef void (*GETMAPNAME_TYPE)(int, char *);
typedef void (*BUILDMAP_TYPE)(int, struct MINESWEEPER_MAP *);
typedef void (*DESTROYMAP_TYPE)(int, struct MINESWEEPER_MAP *);
typedef void (*RESETMAP_TYPE)(int);
typedef int	 (*MOUSEMOVE_TYPE)(int, int, int);
typedef void (*SETCAMERAPARAMS_TYPE)(int, int, int);


typedef DWORD (*SELECTFACE_TYPE)(int*);
typedef void (*PLACENUMBERS_TYPE)();
typedef void (*GETGAMENAME_TYPE)(char *);
typedef void (*PREPAREMAP_TYPE)(struct MINESWEEPER_MAP *, PLACENUMBERS_TYPE);
typedef int	 (*MOUSEBUTTON_TYPE)(int, int *, int*, SELECTFACE_TYPE, struct MINESWEEPER_MAP *);
typedef int	 (*GETTEXTURENAME_TYPE)(int, GLuint *);
typedef DWORD(*PREPARETEXTURES_TYPE)(int);
typedef	int	 (*FREETEXTURES_TYPE)();

typedef int	  (*GETTEXTPLACE_TYPE)(DWORD,GLuint*,GLfloat*,GLfloat*,GLfloat*);
typedef void  (*GETGRAPHNAME_TYPE)(char *);
typedef DWORD (*PREPAREOPENGL_TYPE)(HDC);
typedef DWORD (*UNPREPAREOPENGL_TYPE)(HDC);
typedef DWORD (*BUILDSTRUCTURE_TYPE)(struct MINESWEEPER_MAP * map);
typedef void  (*FREESTRUCTURE_TYPE)();
typedef DWORD (*RENDERER_TYPE) (int gameStatus, struct MINESWEEPER_MAP * map, GETTEXTPLACE_TYPE p_GetTextPlace,
				  GLfloat * texVertex, int width, int height, int timer, int record,
				  int active);




#ifdef	THIS_IS_A_MODULE
#ifdef __cplusplus
extern "C" {
	void	_declspec (dllexport) GetMineInfo (int * version, MINE_MODULE_INFO * m);	
	void	_declspec (dllexport) GetMapCount (int * count);	
	void	_declspec (dllexport) GetMapName (int mapIdx, char * name);	
	void	_declspec (dllexport) BuildMap (int mapIdx, MINESWEEPER_MAP *);
	void	_declspec (dllexport) DestroyMap (int mapIdx, MINESWEEPER_MAP *);
	int		_declspec (dllexport) MouseMove (int mapIdx, int, int);
	void	_declspec (dllexport) SetCameraParams (int mapIdx, int, int);
	void	_declspec (dllexport) ResetMap (int mapIdx);



	void	_declspec (dllexport) GetGameName (char * name);	
	void	_declspec (dllexport) PrepareMap (MINESWEEPER_MAP *,
											  PLACENUMBERS_TYPE placeNumbers_callback);
	int		_declspec (dllexport) MouseButton  (int msg,
												int * buttons, 
												int * gamestate,
												SELECTFACE_TYPE selectFaceCallback,
												MINESWEEPER_MAP * map);
	int		_declspec (dllexport) GetTextureName (int, GLuint * );
	DWORD	_declspec (dllexport) PrepareTextures (int);
	int		_declspec (dllexport) FreeTextures ();

	
	void	_declspec (dllexport) GetGraphicsName (char * name);	
	DWORD	_declspec (dllexport) PrepareOpenGL (HDC hDc);
	DWORD	_declspec (dllexport) UnprepareOpenGL (HDC hDc);
	DWORD	_declspec (dllexport) BuildStructure (MINESWEEPER_MAP * map);
	void	_declspec (dllexport) FreeStructure ();
	DWORD	_declspec (dllexport) Renderer (int gameStatus, 
											MINESWEEPER_MAP * map, 
											GETTEXTPLACE_TYPE p_GetTextPlace,
											GLfloat * texVertex, 
											int width, 
											int height, 
											int timer, 
											int record,
											int active);
}
#else
	extern	void	_declspec (dllexport) GetMineInfo (int * version, struct MINE_MODULE_INFO * m);	
	extern	void	_declspec (dllexport) GetMapCount (int * count);	
	extern	void	_declspec (dllexport) GetMapName (int number, char * name);	
	extern	void	_declspec (dllexport) BuildMap (int, struct MINESWEEPER_MAP *);
	extern	void	_declspec (dllexport) DestroyMap (int, struct MINESWEEPER_MAP *);
	extern	int		_declspec (dllexport) MouseMove (int, int, int);
	extern	void	_declspec (dllexport) SetCameraParams (int, int, int);
	extern  void	_declspec (dllexport) ResetMap (int);

	extern	void	_declspec (dllexport) GetGameName (char * name);	
	extern	void	_declspec (dllexport) PrepareMap (struct MINESWEEPER_MAP *, 
													  PLACENUMBERS_TYPE placeNumbers_callback);
	extern	int		_declspec (dllexport) MouseButton  (int msg,
														int * buttons, 
														int * gamestate,
														SELECTFACE_TYPE selectFaceCallback,
														struct MINESWEEPER_MAP * map);
	extern	int		_declspec (dllexport) GetTextureName (int, GLuint * );
	extern	DWORD	_declspec (dllexport) PrepareTextures (int);
	extern  int		_declspec (dllexport) FreeTextures ();


	extern	void	_declspec (dllexport) GetGraphicsName (char * name);	
	extern	DWORD	_declspec (dllexport) PrepareOpenGL (HDC hDc);
	extern	DWORD	_declspec (dllexport) UnprepareOpenGL (HDC hDc);
	extern	DWORD	_declspec (dllexport) BuildStructure (struct MINESWEEPER_MAP * map);
	extern	void	_declspec (dllexport) FreeStructure ();
	extern  DWORD	_declspec (dllexport) Renderer (int gameStatus, 
											MINESWEEPER_MAP * map, 
											GETTEXTPLACE_TYPE p_GetTextPlace,
											GLfloat * texVertex, 
											int width, 
											int height, 
											int timer, 
											int record,
											int active);

#endif
#endif		// THIS_IS_A_MODULE

	
#endif		// main
