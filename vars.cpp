

// WINDOWS SPECIFIC & OPENGL!!
#include "stdafx.h"

#include "library.h"
#include "vars.h"
#include "modules.h"
#include "strings.h"


static	char * iniFile = "config.txt";
static  char completeVarsFile [256];

		const char * const DATA_DIRECTORY = "data\\";
		const char * const PLUGINS_DIRECTORY = "plugins\\*.dll";



struct GLOBAL_VARS vars;
struct INI_VARS ini;


extern struct	MINE_MODULE_MAPDESC * mapDescriptorList;
RECORD *		recordArray;


enum	TTYPE { TINT, TSTRING, GLFLOAT, GLFLOAT4 };
struct	VarName {
	void	* var;
	char	* name;
	TTYPE	type;
};



static VarName		nametable [] = {
	{ &ini.graph_filtering,			"graph_filtering",			TINT },
	{ &ini.graph_bpptexture,		"graph_bpptexture",			TINT },
	{ &ini.main_language,			"main_language",			TSTRING },
	{ &ini.perf_idleRefresh,		"perf_idleRefresh",			TINT },
	{ &ini.perf_internalTimer,		"perf_internalTimer",		TINT },
	{ &ini.perf_maxFPS,				"perf_maxFPS",				TINT },
	{ &ini.tex_file,				"tex_file",					TSTRING },
	{ &ini.tex_height,				"tex_height",				TINT },
	{ &ini.tex_interleave,			"tex_interleave",			TINT },
	{ &ini.tex_width,				"tex_width",				TINT },

	{ &ini.light_borderShininess,	"light_borderShininess",	GLFLOAT  },
	{ &ini.light_faceShininess,		"light_faceShininess",		GLFLOAT	 },
	{ ini.light_borderDiffuse,		"light_borderDiffuse",		GLFLOAT4 },
	{ ini.light_faceDiffuse,		"light_faceDiffuse",		GLFLOAT4 },
	{ ini.light_faceDiffuseCovered,	"light_faceDiffuseCovered",	GLFLOAT4 },
	{ ini.light_borderSpecular,		"light_borderSpecular",		GLFLOAT4 },
	{ ini.light_faceSpecular,		"light_faceSpecular",		GLFLOAT4 },

	{ &ini.main_xSens,				"main_xSens",				GLFLOAT },
	{ &ini.main_ySens,				"main_ySens",				GLFLOAT },

	{ NULL,							"",							TINT }	};





DWORD		LoadINI(char * extname)
{
	ini.graph_filtering = 1;			
	ini.graph_bpptexture = 16;

	strcpy (ini.main_language, "english");
	ini.main_xSens = ini.main_ySens = 1.0f;
	
	// Perf
	ini.perf_internalTimer = 15;
	ini.perf_maxFPS = 45;				
	ini.perf_idleRefresh = 5;
	
	// tex
	ini.tex_width = ini.tex_height = 32;
	ini.tex_interleave = 0;
	ini.tex_file[0] = 0;

	// light
	ini.light_borderShininess = 30.0f;
	ini.light_faceShininess = 30.0f;
	
	ini.light_borderDiffuse[0] = 
		ini.light_borderDiffuse[1] = 
		ini.light_borderDiffuse[2] = 
		ini.light_borderDiffuse[3] = 0.4f;

	ini.light_borderSpecular[0] = 0.7f;
	ini.light_borderSpecular[1] = 0.9f;
	ini.light_borderSpecular[2] = 1.0f;
	ini.light_borderSpecular[3] = 1.0f;

    ini.light_faceDiffuse[0] =
	    ini.light_faceDiffuse[1] =
		ini.light_faceDiffuse[2] =
		ini.light_faceDiffuse[3] = 0.9f;
    ini.light_faceDiffuseCovered[0] =
	    ini.light_faceDiffuseCovered[1] =
		ini.light_faceDiffuseCovered[2] =
		ini.light_faceDiffuseCovered[3] = 0.3f;

	ini.light_faceSpecular[0] = 0.07f; 
	ini.light_faceSpecular[1] = 0.09f;
	ini.light_faceSpecular[2] = 0.1f;
	ini.light_faceSpecular[3] = 1.0f;

	FILE * file = fopen (iniFile, "rt");
	if (!file)
		return MIDS_VARS_NOINIFILE;

	char name [128], string [128];
	while (ReadAssignment (name, string, 128, file)) {
		// Cerca nome
		int idx = 0;
		while (nametable[idx].var != NULL && strcmp (name, nametable[idx].name) != 0)
			idx++;
		if (nametable[idx].var == NULL) {
			strcpy (extname, name);
			return MIDS_VARS_UNKNOWNVAR;
		}
		if (nametable[idx].type == TSTRING) 
			strcpy ((char*)nametable[idx].var, string);
		else if (nametable[idx].type == TINT) 
			sscanf (string, "%d", (int*)nametable[idx].var);
		else if (nametable[idx].type == GLFLOAT) 
			sscanf (string, "%f", (GLfloat*)nametable[idx].var);
		else if (nametable[idx].type == GLFLOAT4) {
			GLfloat * array = (GLfloat*)nametable[idx].var;
			sscanf (string, "%f %f %f %f", array, array + 1, array + 2, array + 3);
		}
	}
	fclose (file);
	return 0;
}	




DWORD		SaveINI (char * extname)
{
	char newName[128];
	strcpy (newName, iniFile);
	strcat (newName, "2");
	FILE * i = fopen (iniFile, "rt");
	FILE * o = fopen (newName, "wt");
	if (!i || !o)
		return MIDS_MAIN_STORESETTINGS;

	char name [128], string [128];
	while (ReadAssignment (name, string, 128, i, o)) {
		// Cerca nome
		int idx = 0;
		while (nametable[idx].var != NULL && strcmp (name, nametable[idx].name) != 0)
			idx++;
		if (nametable[idx].var == NULL) {
			strcpy (extname, name);
			return MIDS_VARS_UNKNOWNVAR;
		}

		fprintf (o, "%s = ", name);
		if (nametable[idx].type == TSTRING) 
			fprintf (o, "%s\n", nametable[idx].var);
		else if (nametable[idx].type == TINT) 
			fprintf (o, "%d\n", *((int*)nametable[idx].var));
		else if (nametable[idx].type == GLFLOAT) 
			fprintf (o, "%f\n", *((GLfloat*)nametable[idx].var));
		else if (nametable[idx].type == GLFLOAT4) {
			GLfloat * array = (GLfloat*)nametable[idx].var;
			fprintf (o, "%f %f %f %f\n", array, array + 1, array + 2, array + 3);
		}
	}
	fclose (o);
	fclose (i);

	// Cancella il vecchio file.
	if (DeleteFile (iniFile))
		MoveFile (newName, iniFile);


	return 0;
}









static void	DefaultSettings ()
{
	vars.nMapModules = 0;
	vars.records = NULL;
}



static	int	find (const char * const name)
{
	int i;
	for (i = 0; i < vars.nMapModules; i++)
		if (strcmp (name, vars.records[i].moduleName) == 0) {
			vars.records[i].visible = 1;
			return 1;
		}
	return 0;
}




UINT  buildDate (unsigned int day, unsigned int month, unsigned int year)
{
	return ((year << 16) + (month << 8) + day); 
}

char * getDate (UINT date)
{
	static char buffer [32], temp[32];
	itoa (date & 0xff, buffer, 10);
	strcat (buffer, "/");
	itoa ((date >> 8) & 0xff, temp, 10);
	strcat (buffer, temp);
	strcat (buffer, "/");
	itoa ((date >> 16) & 0xffff, temp, 10);
	strcat (buffer, temp);
	return buffer;
}



static DWORD UpdateSettings ()
{
// Va creata e modificata la lista delle mappe con i relativi record (in caso non c'è da
// file, creare ogni record).
	MINE_MODULE_MAPDESC * map;
	MINE_VARS_MAPRECORD * newarray;
	unsigned int count = vars.nMapModules, i, j;

	// Moduli mappa
	map = mapDescriptorList;
	while (map != NULL) {
		if (!find(map->moduleName))
			count++;
		map = map->next;
	}

	newarray = new MINE_VARS_MAPRECORD [count];
	memcpy (newarray, vars.records, sizeof (struct MINE_VARS_MAPRECORD) * vars.nMapModules);

	// Ora scrive le nuove del gioco corrente, creandole e azzerandole
	i = vars.nMapModules;
	map = mapDescriptorList;
	while (map != NULL) {
		if (!find(map->moduleName)) {
			strcpy (newarray[i].moduleName, map->moduleName);
			newarray[i].nMaps = map->nMaps;
			newarray[i].visible = 1;
			newarray[i].records = new RECORD [map->nMaps];
			for (j = 0; j < map->nMaps; j++) {
				strncpy (newarray[i].records[j].mapName, map->mapDesc[j].name, MINE_MODULE_NAMESIZE);
				strcpy (newarray[i].records[j].name, "Anonymous");
				newarray[i].records[j].record = 9999; // secondi
				newarray[i].records[j].date = buildDate (8, 11, 1976);
			}
			i++;
		}
		map = map->next;
	}
	// Distrugge vecchio array, ma non i sottoarray...!!!
	delete vars.records;
	vars.records = newarray;
	vars.nMapModules = count;

	return (i != count) ? MIDS_VARS_MAPENUM : 0;
}



static	UINT	calcCRC (struct RECORD * record)
{
	int i;
	UINT crc = record->date + record->record;
	for (i = 0; i < MINE_MODULE_NAMESIZE; i++)
		crc += record->name[i] + record->mapName[i];
	return crc;
}


struct RECORD	*	GetRecordArray (const char * const moduleName, int mapIdx)
{
	int i;
	for (i = 0; i < vars.nMapModules; i++)
		if (strcmp (vars.records[i].moduleName, moduleName) == 0)
			break;
	if (i >= vars.nMapModules)
		return NULL;
	return vars.records[i].records + mapIdx;
}




static int OpenSettings()
{
	WORD ver;
	int i, j;

	FILE * file;
	file = fopen (completeVarsFile, "rb");
	if (file == NULL)
		return 0;

	if (fread (&ver, sizeof (WORD), 1, file) != 1 || ver != CFGFILE_VER)
		return 0;

	if (fread (&vars, sizeof (struct GLOBAL_VARS), 1, file) != 1) 
		return 0;
		
	// C'è da leggere la lista delle mappe con i record
	vars.records = new MINE_VARS_MAPRECORD [vars.nMapModules];
	if (fread (vars.records, sizeof (struct MINE_VARS_MAPRECORD), vars.nMapModules, file) != (UINT)vars.nMapModules) {
		free (vars.records);
		vars.records = NULL;
		return 0;
	}

	for (i=0; i<vars.nMapModules; i++) {
		if (vars.records[i].nMaps < 0 || vars.records[i].nMaps > MAX_MODULE_MAPS_COUNT) 
			break;

		vars.records[i].visible = 0;
		vars.records[i].records = new RECORD [vars.records[i].nMaps];
		if (fread (vars.records[i].records, sizeof(struct RECORD), vars.records[i].nMaps, file) != (UINT)vars.records[i].nMaps)
			break;

		for (j=0; j<vars.records[i].nMaps; j++)
			if (calcCRC(vars.records[i].records + j) != vars.records[i].records[j].crc)
				break;
		if (j < vars.records[i].nMaps)
			break;
	}

	if (i < vars.nMapModules) {
		for (j = 0; j < i; j++)
			delete vars.records[j].records;
		delete vars.records;
		vars.records = NULL;
		return 0;
	}

	fclose (file);

	return 1;
}



DWORD	LoadSettings(const char * const appData)
{
	int def = 0;
	DWORD err;

	strcpy (completeVarsFile, appData);
	strcat (completeVarsFile, "persist.dat");

	if (!OpenSettings()) {
		DefaultSettings();
		def = 1;
	}
	err = UpdateSettings();
	if (err)
		return err;

	if (!def) return 0;
		else return MIDS_VARS_DEFAULTSETTING;
}



DWORD   StoreSettings ()
{
	int i, j;

	DWORD ver = CFGFILE_VER;

	FILE * file;
	file = fopen (completeVarsFile, "wb");
	if (file == NULL)
		return MIDS_MAIN_STORESETTINGS;
	if (fwrite (&ver, sizeof (WORD), 1, file) != 1)
		return MIDS_MAIN_STORESETTINGS;
	if (fwrite (&vars, sizeof (vars), 1, file) != 1)
		return MIDS_MAIN_STORESETTINGS;

	// C'è da scrivere la lista delle mappe con i record
	if (fwrite (vars.records, sizeof (struct MINE_VARS_MAPRECORD), vars.nMapModules, file) != (UINT)vars.nMapModules)
		return MIDS_MAIN_STORESETTINGS;

	for (i=0; i<vars.nMapModules; i++) {
		for (j=0; j<vars.records[i].nMaps; j++)
			vars.records[i].records[j].crc = calcCRC(vars.records[i].records + j);

		if (fwrite (vars.records[i].records, sizeof(struct RECORD), vars.records[i].nMaps, file) != (UINT)vars.records[i].nMaps)
			return MIDS_MAIN_STORESETTINGS;
		delete vars.records[i].records;
	}
	delete vars.records;
	vars.records = NULL;
	fclose (file);

	return 0;
}

