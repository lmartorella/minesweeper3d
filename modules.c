


#include "stdafx2.h"
#include "library.h"
#include "modules.h"
#include "display.h"
#include "vars.h"

#include "resource.h"



// Mappa
extern	int						mapIndex;
extern	BUILDMAP_TYPE			p_BuildMap;
extern	DESTROYMAP_TYPE			p_DestroyMap;
extern	MOUSEMOVE_TYPE			p_MouseMove;
extern	SETCAMERAPARAMS_TYPE	p_SetCameraParams;
extern	RESETMAP_TYPE			p_ResetMap;

// Gioco
extern	PREPAREMAP_TYPE			p_PrepareMap;
extern	MOUSEBUTTON_TYPE		p_MouseButton;

// Textures
extern	GETTEXTURENAME_TYPE		p_GetTextureName;
extern	PREPARETEXTURES_TYPE	p_PrepareTextures;
extern	FREETEXTURES_TYPE		p_FreeTextures;

extern	struct RECORD *			recordArray;

// WINDOWS SPECIFIC
typedef void (*MINE_FUNCTION_DEFAULT)(void);
MINE_MODULE		OpenFileLib (const char * const name)
{
	return LoadLibrary (name);
}
void			CloseFileLib (MINE_MODULE mod)
{	
	FreeLibrary (mod);
}
MINE_FUNCTION_DEFAULT GetFunctionAddress (MINE_MODULE module, const char * const funcName)
{
	return (MINE_FUNCTION_DEFAULT)GetProcAddress (module, funcName);
}
// WINDOWS SPECIFIC end


static	int		mapCode = ID_NEWMAP_GAME;
static	int		gametypeCode = ID_GAMETYPE;
static	int		textypeCode = ID_TEXTYPE;

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------


struct	MINE_MODULE_MAPDESC * mapDescriptorList = NULL;
struct	MINE_MODULE_GAMEDESC * gameDescriptorList = NULL;
MINE_MODULE		mapModuleOpened = MINE_MODULE_INVALID;
MINE_MODULE		gameModuleOpened = MINE_MODULE_INVALID;



DWORD	ReadLibrary (const char * const name)
{
	struct MINE_MODULE_INFO info;
	struct MINE_MODULE_MAPDESC * mapDesc;
	struct MINE_MODULE_GAMEDESC * gameDesc;
	unsigned int ver, i;
	
	// Funzioni
	GETMINEINFO_TYPE GetMineInfo;
	GETMAPCOUNT_TYPE GetMapCount;
	GETMAPNAME_TYPE GetMapName;
	MINE_FUNCTION_DEFAULT generic;
	MOUSEMOVE_TYPE MouseMove;
	SETCAMERAPARAMS_TYPE SetCameraParams;
	DESTROYMAP_TYPE DestroyMap;
	RESETMAP_TYPE ResetMap;

	GETGAMENAME_TYPE GetGameName;
	PREPAREMAP_TYPE PrepareMap;
	MOUSEBUTTON_TYPE MouseButton;
	GETTEXTURENAME_TYPE GetTextureName;
	PREPARETEXTURES_TYPE PrepareTextures;
	FREETEXTURES_TYPE FreeTextures;

	MINE_MODULE	module = OpenFileLib (name);
	if (module == MINE_MODULE_INVALID) 
		return IDS_MODULE_CANTLOAD;
	
	// Aperta
	GetMineInfo = (GETMINEINFO_TYPE) GetFunctionAddress (module, "GetMineInfo");
	if (GetMineInfo == MINE_MODULE_INVALID) 
		return IDS_MODULE_INVALIDLIBRARY;

	GetMineInfo (&ver, &info);
	if (ver != 0x100) 
		return IDS_MODULE_VERSION;

	// E' una mappa?
	generic = GetFunctionAddress (module, "BuildMap");
	if (generic != MINE_MODULE_INVALID) {

		// MAPPA
		GetMapCount = (GETMAPCOUNT_TYPE) GetFunctionAddress (module, "GetMapCount");
		GetMapName = (GETMAPNAME_TYPE) GetFunctionAddress (module, "GetMapName");
		MouseMove = (MOUSEMOVE_TYPE) GetFunctionAddress (module, "MouseMove");
		SetCameraParams = (SETCAMERAPARAMS_TYPE) GetFunctionAddress (module, "SetCameraParams");
		DestroyMap = (DESTROYMAP_TYPE) GetFunctionAddress (module, "DestroyMap");
		ResetMap = (RESETMAP_TYPE) GetFunctionAddress (module, "ResetMap");

		if (GetMapCount == MINE_MODULE_INVALID || GetMapName == MINE_MODULE_INVALID
			|| MouseMove == MINE_MODULE_INVALID || SetCameraParams == MINE_MODULE_INVALID ||
			DestroyMap == MINE_MODULE_INVALID || ResetMap == MINE_MODULE_INVALID ) 

			return IDS_MODULE_INVALIDMAPMOD;

		// Mappa ok
		mapDesc = (struct MINE_MODULE_MAPDESC*)malloc(sizeof (struct MINE_MODULE_MAPDESC));
		strncpy (mapDesc->moduleName, info.name, MINE_MODULE_NAMESIZE);
		strcpy (mapDesc->moduleFile, name); 
		
		GetMapCount (&mapDesc->nMaps);
		
		if (mapDesc->nMaps > MAX_MODULE_MAPS_COUNT) 
			return IDS_MODULE_TOOMAPS;

		mapDesc->mapDesc = (struct MINE_MAPDESC *) malloc (sizeof (struct MINE_MAPDESC) * mapDesc->nMaps);
		for (i = 0; i < mapDesc->nMaps; i++) {
			GetMapName (i, mapDesc->mapDesc[i].name);
			mapDesc->mapDesc[i].code = mapCode++;
		}
		mapDesc->next = mapDescriptorList;
		mapDescriptorList = mapDesc;
	}

	// E' una gametype?
	generic = GetFunctionAddress (module, "GetGameName");
	if (generic != MINE_MODULE_INVALID) {

		GetGameName = (GETGAMENAME_TYPE) generic;
		PrepareMap = (PREPAREMAP_TYPE) GetFunctionAddress (module, "PrepareMap");
		MouseButton = (MOUSEBUTTON_TYPE) GetFunctionAddress (module, "MouseButton");
		GetTextureName = (GETTEXTURENAME_TYPE) GetFunctionAddress (module, "GetTextureName");
		PrepareTextures = (PREPARETEXTURES_TYPE) GetFunctionAddress (module, "PrepareTextures");
		FreeTextures = (FREETEXTURES_TYPE) GetFunctionAddress (module, "FreeTextures");

		if (PrepareMap == MINE_MODULE_INVALID || MouseButton == MINE_MODULE_INVALID ||
			GetTextureName == MINE_MODULE_INVALID || PrepareTextures == MINE_MODULE_INVALID 
			|| FreeTextures == MINE_MODULE_INVALID) 

			return IDS_MODULE_INVALIDGAMEMOD;

		// Gametype ok
		gameDesc = (struct MINE_MODULE_GAMEDESC*)malloc(sizeof (struct MINE_MODULE_GAMEDESC));
		strncpy (gameDesc->moduleName, info.name, MINE_MODULE_NAMESIZE);
		strcpy (gameDesc->moduleFile, name); 

		GetGameName (gameDesc->name);
		gameDesc->next = gameDescriptorList;
		gameDescriptorList = gameDesc;

		gameDesc->code = gametypeCode++;
	}

	CloseFileLib (module);
	return 0;
}




void	DestroyDescriptorLists ()
{
	struct MINE_MODULE_MAPDESC * map = mapDescriptorList;
	struct MINE_MODULE_GAMEDESC * game = gameDescriptorList;

	while (map != NULL) {
		map = map->next;
		free (mapDescriptorList);
		mapDescriptorList = map;
	}
	while (game != NULL) {
		game = game->next;
		free (gameDescriptorList);
		gameDescriptorList = game;
	}
	
	mapDescriptorList = NULL;
	gameDescriptorList = NULL;
}






DWORD	UpdateMapFunctions (DWORD code)
{
	unsigned int i;
	struct MINE_MODULE_MAPDESC * p = mapDescriptorList;

	if (mapModuleOpened != MINE_MODULE_INVALID)
		CloseFileLib (mapModuleOpened);

	// Cercare modulo e assegnare le funzioni p_...
	while (p != NULL) {
		for (i=0; i<p->nMaps; i++)
			if (p->mapDesc[i].code == code)
				break;

		if (i < p->nMaps) {
			mapIndex = i;

			mapModuleOpened = OpenFileLib (p->moduleFile);
			if (mapModuleOpened == MINE_MODULE_INVALID)
				return 0;

			p_BuildMap = (BUILDMAP_TYPE) GetFunctionAddress (mapModuleOpened, "BuildMap");
			p_MouseMove = (MOUSEMOVE_TYPE) GetFunctionAddress (mapModuleOpened, "MouseMove");
			p_SetCameraParams = (SETCAMERAPARAMS_TYPE) GetFunctionAddress (mapModuleOpened, "SetCameraParams");
			p_DestroyMap = (DESTROYMAP_TYPE) GetFunctionAddress (mapModuleOpened, "DestroyMap");
			p_ResetMap = (RESETMAP_TYPE) GetFunctionAddress (mapModuleOpened, "ResetMap");

			recordArray = GetRecordArray (p->moduleName, mapIndex);
			return 0;
		}
		p = p->next;
	}
	return IDS_MODULE_MAPSELECTION;
}


DWORD	SelectGameType (struct MINE_MODULE_GAMEDESC * game)
{
	if (gameModuleOpened != MINE_MODULE_INVALID)
		CloseFileLib (gameModuleOpened);

	gameModuleOpened = OpenFileLib (game->moduleFile);
	if (gameModuleOpened == MINE_MODULE_INVALID)
		return 0;

	p_PrepareMap = (PREPAREMAP_TYPE) GetFunctionAddress (gameModuleOpened, "PrepareMap");
	p_MouseButton = (MOUSEBUTTON_TYPE) GetFunctionAddress (gameModuleOpened, "MouseButton");
	p_GetTextureName = (GETTEXTURENAME_TYPE) GetFunctionAddress (gameModuleOpened, "GetTextureName");
	p_PrepareTextures = (PREPARETEXTURES_TYPE) GetFunctionAddress (gameModuleOpened, "PrepareTextures");
	p_FreeTextures = (FREETEXTURES_TYPE) GetFunctionAddress (gameModuleOpened, "FreeTextures");

	return rebuildTextures(); 
}



