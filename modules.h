

#pragma once

// WINDOWS SPECIFIC
#define MINE_MODULE HMODULE
#define MINE_MODULE_INVALID NULL
// WINDOWS SPECIFIC end

#define MINE_MODULE_NAMESIZE		32
#define MINE_MODULE_FILENAMESIZE	256

#define	MAX_MODULE_COUNT			64

#define MAX_MODULE_MAPS_COUNT		32
#define MAX_MAPS_COUNT				512

#define MAX_GAMETYPE_COUNT			32




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


struct	MINE_MODULE_GAMEDESC {
	char						moduleName[MINE_MODULE_NAMESIZE];
	char						moduleFile[MINE_MODULE_FILENAMESIZE];
	char						name [MINE_MODULE_NAMESIZE];
	DWORD						code;

	struct MINE_MODULE_GAMEDESC * next;
};




#ifdef __cplusplus
	extern "C" {
		DWORD		ReadLibrary (const char * const name);
		void		DestroyDescriptorLists ();
		DWORD		UpdateMapFunctions (DWORD code);
		DWORD		SelectGameType (MINE_MODULE_GAMEDESC * game);
	}
#else
	extern DWORD		ReadLibrary (const char * const name);
	extern void			DestroyDescriptorLists ();
	extern DWORD		UpdateMapFunctions (DWORD code);
	extern DWORD		SelectGameType (struct MINE_MODULE_GAMEDESC * game);
#endif


