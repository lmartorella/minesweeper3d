

#include "stdafx.h"
#include "strings.h"
#include "stdio.h"
#include "vars.h"


extern	INI_VARS ini;



struct	StringResource {
	DWORD	code;
	char	* string;
};



StringResource stable [] = {
	{ MIDS_VARS_NOINIFILE,			"IDS_VARS_NOINIFILE"		},
	{ MIDS_VARS_UNKNOWNVAR,			"IDS_VARS_UNKNOWNVAR"		},

	{ MIDS_GAME_TEXBPPERROR,		  "IDS_GAME_TEXBPPERROR"		},
	{ MIDS_GAME_TEXFILENOTFOUND		, "IDS_GAME_TEXFILENOTFOUND"		},
	{ MIDS_GAME_TEXFILEERROR         , "IDS_GAME_TEXFILEERROR"			},
	{ MIDS_MODULEMAP_PREPARETEXTURE  , "IDS_MODULEMAP_PREPARETEXTURE"	},
	{ MIDS_GAME_MAPDEFERR            , "IDS_GAME_MAPDEFERR"				},
	{ MIDS_GAME_SELECTION            , "IDS_GAME_SELECTION"				},
	{ MIDS_MODULE_MAPSELECTION       , "IDS_MODULE_MAPSELECTION"			},
	{ MIDS_MAP_TOONEIGHBOURS         , "IDS_MAP_TOONEIGHBOURS"			},
	{ MIDS_MODULE_CANTLOAD           , "IDS_MODULE_CANTLOAD"				},
	{ MIDS_MODULE_INVALIDLIBRARY     , "IDS_MODULE_INVALIDLIBRARY"		},
	{ MIDS_MODULE_VERSION			, "IDS_MODULE_VERSION"				},
	{ MIDS_MODULE_INVALIDMAPMOD		, "IDS_MODULE_INVALIDMAPMOD"		},
	{ MIDS_MODULE_TOOMAPS			, "IDS_MODULE_TOOMAPS"				},	
	{ MIDS_MODULE_INVALIDGAMEMOD     , "IDS_MODULE_INVALIDGAMEMOD"		},	
	{ MIDS_OPENGL_CHOOSEPIXELFORMAT  , "IDS_OPENGL_CHOOSEPIXELFORMAT"	},
	{ MIDS_OPENGL_SETPIXELFORMAT     , "IDS_OPENGL_SETPIXELFORMAT"		},
	{ MIDS_MAIN_PERFTIMER            , "IDS_MAIN_PERFTIMER"				},
	{ MIDS_MAIN_STORESETTINGS        , "IDS_MAIN_STORESETTINGS"			},
	{ MIDS_ABOUT_CONTENT            , "IDS_ABOUT_CONTENT"				},
	{ MIDS_MAIN_STANDARDLIBRARY      , "IDS_MAIN_STANDARDLIBRARY"		},
	{ MIDS_MAIN_MENUITEMS            , "IDS_MAIN_MENUITEMS"				},
	{ MIDS_VARS_MAPENUM              , "IDS_VARS_MAPENUM"				},
	{ MIDS_VARS_DEFAULTSETTING       , "IDS_VARS_DEFAULTSETTING"			},
	{ MIDM_GAME						, "IDM_GAME"						},
	{ MIDM_RESTART_GAME				, "IDM_RESTART_GAME"				},
	{ MIDM_NEW_GAME					, "IDM_NEW_GAME"					},
	{ MIDM_EXIT						, "IDM_EXIT"						},
	{ MIDM_VIEW						, "IDM_VIEW"						},
	{ MIDM_HALLSOFFAME				, "IDM_HALLSOFFAME"					},
	{ MIDM_HELP						, "IDM_HELP"						},
	{ MIDM_ABOUT						, "IDM_ABOUT"						},
	
	{ MIDS_ABOUT_TITLE			,	"IDS_ABOUT_TITLE"	},
	{ MIDS_HALLSOFFAME_TITLE	,	"IDS_HALLSOFFAME_TITLE" },
	{ MIDS_WINDLG_TITLE			 , "IDS_WINDLG_TITLE" },
	{ MIDS_WINDLG_MESSAGE			 , "IDS_WINDLG_MESSAGE" },

	{ MIDS_HOF_MAPNAME				, "IDS_HOF_MAPNAME" },
	{ MIDS_HOF_SWEEPERNAME			, "IDS_HOF_SWEEPERNAME" },
	{ MIDS_HOF_TIME					, "IDS_HOF_TIME" },
	{ MIDS_HOF_DATE					, "IDS_HOF_DATE" },
	
	{ IDS____LAST					, ""								}
};









int		ReadAssignment (char * name, char * string, int size, FILE * file)
{
	char * buffer = new char[size];

	do {
		// Siamo all'inizio di una linea
		if (fgets (buffer, size, file) == NULL) {
			delete[] buffer;
			return 0;
		}

		int l = strlen(buffer);
		if (l == 0)
			continue;

		// Arriva alla fine della linea
		if (buffer[l - 1] != '\n') {
			while (fgetc(file) != '\n')
				if (feof (file)) {
					delete[] buffer;
					return 0;
				}
		}
		else
			buffer[l - 1] = 0;
		
		// Decode
		char temp[128];
		if (sscanf (buffer, "%128s", temp) < 1)
			continue;
		if (temp[0] == '#' || temp[0] == '=')
			continue;

		char * eq = strchr (buffer, '=');
		char * eq2 = eq;
		if (eq == NULL) {
			delete[] buffer;
			return 0;
		}

		*eq = 0;			// spezza in due stringhe
		eq++;
		while (*eq == ' ')
			eq++;
		// Gestisce i caratteri speciali in "string"
		while (1) {
			if (*eq == 0) {
				*string = 0;
				break;
			}
			if (*eq != '\\')
				*string = *eq;
			else {
				eq++;
				if (eq[0] == 't')
					*string = '\t';
				else if (eq[0] == 'n')
					*string = '\n';
				else if (eq[0] == 'r')
					*string = '\r';
				else if (eq[0] == '\\')
					*string = '\\';
				else
					eq--;
			}
			eq++;
			string++;
		}

		eq2--;
		while (*eq2 == ' ')
			eq2--;
		*(eq2 + 1) = 0;
		strcpy (name, buffer);

		delete[] buffer;
		return 1;
	} while (1);
}






int		ReadStringDef (DWORD * code, char * string, int size, FILE * file)
{
	char * name = new char [size];
	if (!ReadAssignment (name, string, size, file))
		return 0;

	int ii = 0;
	while (stable[ii].code != IDS____LAST) {
		if (strcmp (name, stable[ii].string) == 0) {
			*code = stable[ii].code;
			delete[] name;
			return 1;
		}
		ii++;
	}
	delete[] name;
	return 1;
}



/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


#define NUM (IDS____LAST - IDS____FIRST - 1)
const char * about = "MineSweeper 3D - Version 0.61";

StringResource strings [NUM];


const char * const GetString (DWORD code)
{
	for (int i=0; i<NUM; i++) 
		if (strings[i].code == code)
			return strings[i].string;
	return "Unknown error code / Codice di errore sconosciuto";
}


int		AddString (DWORD code, const char * const str)
{
	for (int i=0; i<NUM; i++) {
		if (strings[i].code == code) {
			if (strings[i].string != NULL)
				return 0;
			strings[i].string = new char [strlen(str)+1];
			strcpy (strings[i].string, str);
			return 1;
		}
	}
	return 0;
}




int			LoadStrings ()
{
	for (int i=0; i<NUM; i++) {
		strings[i].code = IDS____FIRST+i+1; 
		strings[i].string = NULL; 
	}

	char filename2[128];
	strcpy (filename2, DATA_DIRECTORY);
	strcat (filename2, ini.main_language);
	FILE * file = fopen (filename2, "rt");
	if (file == NULL)
		return 0;
	if (!AddString (MIDS_ABOUT_CONTENT, about))
		return 0;

	int numb = 0;
	DWORD code;
	char string[512];

	while (ReadStringDef (&code, string, 512, file)) {
		if (!AddString (code, string))
			return 0;
		numb++;
	}

	fclose (file);
	return (numb == NUM - 1);
}


void	DeleteStrings()
{
	for (int i=0; i<NUM; i++)
		delete[] strings[i].string;
}

