

#ifndef _MODULES_MINESWEEPER_
#define _MODULES_MINESWEEPER_



#define MINE_MODULE_NAMESIZE		32
#define MINE_MODULE_FILENAMESIZE	256

#define	MAX_MODULE_COUNT			64
#define MAX_MODULE_MAPS_COUNT		32
#define MAX_MAPS_COUNT				512





struct		MINE_MAPDESC {
		char		name [MINE_MODULE_NAMESIZE];
		DWORD		code;
};


struct	MINE_MODULE_MAPDESC {
	char						moduleName[MINE_MODULE_NAMESIZE];
	char						moduleFile[MINE_MODULE_FILENAMESIZE];
	unsigned int				nMaps;
	struct MINE_MAPDESC			* mapDesc;

	struct MINE_MODULE_MAPDESC * next;
};





DWORD		ReadLibrary (const char * const name, DWORD * code);
void		DestroyDescriptorLists ();
DWORD		UpdateMapFunctions (DWORD code);



#endif