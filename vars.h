

#pragma once

#include "modules.h"


#define CFGFILE_VER 0x305
#define STANDARD_GAME "minesw/original/standard"
#define STANDARD_GRAPH "minesw/original/standard"


struct	RECORD {
	char		name [MINE_MODULE_NAMESIZE];
	char		mapName [MINE_MODULE_NAMESIZE];
	UINT		record;
	UINT		date;
	UINT		crc;			// per frodi.
};


struct	MINE_VARS_MAPRECORD {
	char	moduleName [MINE_MODULE_NAMESIZE];
	int		nMaps;

	struct RECORD * records;		// tanti quante sono le mappe
};

struct	GLOBAL_VARS {
	int						nMapModules;
	char					gameSelected [MINE_MODULE_NAMESIZE];
	char					graphSelected [MINE_MODULE_NAMESIZE];
	int						filtering;
	struct MINE_VARS_MAPRECORD *	records; 
};



#ifdef __cplusplus 
extern "C" {
	DWORD	LoadSettings();
	DWORD	StoreSettings ();
	char *	getDate (UINT date);
	UINT    buildDate (unsigned int day, unsigned int month, unsigned int year);
	RECORD	*	GetRecordArray (const char * const moduleName, int mapIdx);
}
#else
	extern DWORD	LoadSettings();
	extern DWORD	StoreSettings ();
	extern char *	getDate (UINT date);
	extern UINT    buildDate (unsigned int day, unsigned int month, unsigned int year);
	extern struct RECORD * GetRecordArray (const char * const moduleName, int mapIdx);
#endif


