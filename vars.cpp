

// WINDOWS SPECIFIC & OPENGL!!
#include "stdafx.h"

#include "library.h"
#include "vars.h"
#include "modules.h"
#include "strings.h"


static char * varsFile = "3dmine.cfg";
struct GLOBAL_VARS vars;
struct INI_VARS ini;


extern struct	MINE_MODULE_MAPDESC * mapDescriptorList;
RECORD *	recordArray;



static void	DefaultSettings ()
{
	vars.filtering = 1;
	vars.nMapModules = 0;
	vars.records = NULL;
}



static	int	find (const char * const name)
{
	int i;
	for (i = 0; i < vars.nMapModules; i++)
		if (strcmp (name, vars.records[i].moduleName) == 0)
			return 1;
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

	return (i != count) ? IDS_VARS_MAPENUM : 0;
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
	file = fopen (varsFile, "rb");
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



DWORD	LoadSettings()
{
	int def = 0;
	DWORD err;

	if (!OpenSettings()) {
		DefaultSettings();
		def = 1;
	}
	err = UpdateSettings();
	if (err)
		return err;

	if (!def) return 0;
		else return IDS_VARS_DEFAULTSETTING;
}



DWORD   StoreSettings ()
{
	int i, j;

	DWORD ver = CFGFILE_VER;

	FILE * file;
	file = fopen (varsFile, "wb");
	if (file == NULL)
		return IDS_MAIN_STORESETTINGS;
	if (fwrite (&ver, sizeof (WORD), 1, file) != 1)
		return IDS_MAIN_STORESETTINGS;
	if (fwrite (&vars, sizeof (vars), 1, file) != 1)
		return IDS_MAIN_STORESETTINGS;

	// C'è da scrivere la lista delle mappe con i record
	if (fwrite (vars.records, sizeof (struct MINE_VARS_MAPRECORD), vars.nMapModules, file) != (UINT)vars.nMapModules)
		return IDS_MAIN_STORESETTINGS;

	for (i=0; i<vars.nMapModules; i++) {
		for (j=0; j<vars.records[i].nMaps; j++)
			vars.records[i].records[j].crc = calcCRC(vars.records[i].records + j);

		if (fwrite (vars.records[i].records, sizeof(struct RECORD), vars.records[i].nMaps, file) != (UINT)vars.records[i].nMaps)
			return IDS_MAIN_STORESETTINGS;
		delete vars.records[i].records;
	}
	delete vars.records;
	vars.records = NULL;
	fclose (file);

	return 0;
}


