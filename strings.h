

#ifndef _STRINGS_MINESWEEPER_
#define _STRINGS_MINESWEEPER_


#define	IDS____FIRST					 1099
#define MIDS_VARS_NOINIFILE			     1100
#define MIDS_VARS_UNKNOWNVAR			 1101
#define MIDS_GAME_TEXBPPERROR	         1102
#define MIDS_GAME_TEXFILENOTFOUND        1103
#define MIDS_GAME_TEXFILEERROR           1104
#define MIDS_MODULEMAP_PREPARETEXTURE    1105
#define MIDS_GAME_MAPDEFERR              1106
#define MIDS_GAME_SELECTION              1107
#define MIDS_MODULE_MAPSELECTION         1108
#define MIDS_MAP_TOONEIGHBOURS           1109
#define MIDS_MODULE_CANTLOAD             1110
#define MIDS_MODULE_INVALIDLIBRARY       1111
#define MIDS_MODULE_VERSION              1112
#define MIDS_MODULE_INVALIDMAPMOD        1113
#define MIDS_MODULE_TOOMAPS              1114
#define MIDS_MODULE_INVALIDGAMEMOD       1115
#define MIDS_OPENGL_CHOOSEPIXELFORMAT    1116
#define MIDS_OPENGL_SETPIXELFORMAT       1117
#define MIDS_MAIN_PERFTIMER              1118
#define MIDS_MAIN_STORESETTINGS          1119
#define MIDS_ABOUT_CONTENT               1120
#define MIDS_MAIN_STANDARDLIBRARY        1121
#define MIDS_MAIN_MENUITEMS              1122
#define MIDS_VARS_MAPENUM                1123
#define MIDS_VARS_DEFAULTSETTING         1124
#define MIDS_ABOUT_TITLE	             1125

#define MIDM_RESTART_GAME				1126
#define MIDM_NEW_GAME					1127
#define MIDM_EXIT						1128
#define MIDM_VIEW						1129
#define MIDM_HALLSOFFAME				1130
#define MIDM_HELP						1131
#define MIDM_ABOUT						1132
#define MIDM_GAME						1133

#define MIDS_HALLSOFFAME_TITLE			1134
#define MIDS_WINDLG_TITLE				1135
#define MIDS_WINDLG_MESSAGE				1136

#define	MIDS_HOF_MAPNAME				1137
#define	MIDS_HOF_SWEEPERNAME			1138
#define	MIDS_HOF_TIME					1139
#define	MIDS_HOF_DATE					1140

#define	MIDM_OPTIONS					1141
#define MIDS_OPTIONDLG					1142
#define MIDS_LANGUAGE					1143
#define MIDS_SENS						1144

#define	IDS____LAST						1145











const char * const	GetString (DWORD code);
int					LoadStrings ();
void				DeleteStrings ();
int					ReadAssignment (char * name, 
									char * string, 
									int size, 
									FILE * file,
									FILE * out = NULL);

#endif
