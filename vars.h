

#ifndef _VARS_MINESWEEPER_
#define _VARS_MINESWEEPER_


#include "modules.h"

#define CFGFILE_VER 0x306


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
	int						filtering;
	struct MINE_VARS_MAPRECORD *	records; 
};



struct	INI_VARS {
	int		singleTexWidth, singleTexHeight;
	int		interleaveX;
	char 	texFileName [128];
	
	GLfloat precision;
};


	DWORD	LoadSettings();
	DWORD	StoreSettings ();
	char *	getDate (UINT date);
	UINT    buildDate (unsigned int day, unsigned int month, unsigned int year);
	RECORD	*	GetRecordArray (const char * const moduleName, int mapIdx);


#endif
