


// WINDOWS SPECIFIC & OPENGL!!
#include "stdafx.h"

#include "library.h"
#include "modules.h"
#include "game.h"
#include "vars.h"
#include "strings.h"





// Mappa
extern	int						mapIndex;
extern	BUILDMAP_TYPE			p_BuildMap;
extern	DESTROYMAP_TYPE			p_DestroyMap;
extern	MOUSEMOVE_TYPE			p_MouseMove;
extern	SETCAMERAPARAMS_TYPE	p_SetCameraParams;
extern	RESETMAP_TYPE			p_ResetMap;

extern	RECORD *				recordArray;

typedef void (*MINE_FUNCTION_DEFAULT)(void);




//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------


struct	MINE_MODULE_MAPDESC * mapDescriptorList = NULL;
MINE_MODULE		mapModuleOpened = MINE_MODULE_INVALID;



DWORD	ReadLibrary (const char * const name, DWORD * code)
{
	struct MINE_MODULE_INFO info;
	struct MINE_MODULE_MAPDESC * mapDesc;
	unsigned int ver, i;
	
	// Funzioni
	MINE_FUNCTION_DEFAULT generic;
	GETMINEINFO_TYPE GetMineInfo;
	GETMAPCOUNT_TYPE GetMapCount;
	GETMAPNAME_TYPE GetMapName;
	MOUSEMOVE_TYPE MouseMove;
	SETCAMERAPARAMS_TYPE SetCameraParams;
	DESTROYMAP_TYPE DestroyMap;
	RESETMAP_TYPE ResetMap;

	MINE_MODULE	module = OpenFileLib (name);
	if (module == MINE_MODULE_INVALID) 
		return MIDS_MODULE_CANTLOAD;
	
	// Aperta
	GetMineInfo = (GETMINEINFO_TYPE) GetFunctionAddress (module, "GetMineInfo");
	if (GetMineInfo == MINE_MODULE_INVALID) 
		return MIDS_MODULE_INVALIDLIBRARY;

	GetMineInfo (&ver, &info);
	if (ver != 0x100) 
		return MIDS_MODULE_VERSION;

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

			return MIDS_MODULE_INVALIDMAPMOD;

		// Mappa ok
		mapDesc = new MINE_MODULE_MAPDESC;
		strncpy (mapDesc->moduleName, info.name, MINE_MODULE_NAMESIZE);
		strcpy (mapDesc->moduleFile, name); 
		
		GetMapCount (&mapDesc->nMaps);
		
		if (mapDesc->nMaps > MAX_MODULE_MAPS_COUNT) 
			return MIDS_MODULE_TOOMAPS;

		mapDesc->mapDesc = new MINE_MAPDESC [mapDesc->nMaps];
		for (i = 0; i < mapDesc->nMaps; i++) {
			GetMapName (i, mapDesc->mapDesc[i].name);
			mapDesc->mapDesc[i].code = (*code)++;
		}
		mapDesc->next = mapDescriptorList;
		mapDescriptorList = mapDesc;
	}

	CloseFileLib (module);
	return 0;
}




void	DestroyDescriptorLists ()
{
	struct MINE_MODULE_MAPDESC * map = mapDescriptorList;

	while (map != NULL) {
		map = map->next;
		delete mapDescriptorList->mapDesc;
		delete mapDescriptorList;
		mapDescriptorList = map;
	}
	mapDescriptorList = NULL;
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
	return MIDS_MODULE_MAPSELECTION;
}





