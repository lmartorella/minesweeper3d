

#ifndef _VARS_MINESWEEPER_
#define _VARS_MINESWEEPER_


#include "modules.h"

#define CFGFILE_VER 0x100

extern	const char * const DATA_DIRECTORY;
extern	const char * const PLUGINS_DIRECTORY;


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
	int		visible;

	struct RECORD * records;		// tanti quante sono le mappe
};


struct	GLOBAL_VARS {
	int								nMapModules;
	struct MINE_VARS_MAPRECORD *	records; 
};



struct	INI_VARS {
	// Textures
	int		tex_width, tex_height;
	int		tex_interleave;
	char 	tex_file [128];

	// Perf
	int		perf_internalTimer;			// in Win9x max 18.2 Hz
	int		perf_maxFPS;				// può essere 0
	int		perf_idleRefresh;			// cicli internal per refresh passivo

	// Graphics
	int		graph_filtering;			// 0 o 1
	int		graph_bpptexture;			// 16, 24 o 32

	// Lights
	GLfloat		light_borderShininess;
	GLfloat		light_faceShininess;
	GLfloat		light_borderDiffuse [4];
	GLfloat		light_faceDiffuse [4];
	GLfloat		light_faceDiffuseCovered [4];
	GLfloat		light_borderSpecular [4];
	GLfloat		light_faceSpecular [4];

	// Main
	char		main_language [128];
	GLfloat		main_xSens;
	GLfloat		main_ySens;
};


	DWORD	LoadSettings(const char * const appData);
	DWORD	StoreSettings ();
	char *	getDate (UINT date);
	UINT    buildDate (unsigned int day, unsigned int month, unsigned int year);
	RECORD	*	GetRecordArray (const char * const moduleName, int mapIdx);

	DWORD	LoadINI(char * name);


#endif
