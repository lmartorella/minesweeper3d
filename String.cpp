

#include "stdafx.h"

#include "strings.h"

struct	StringResource {
	DWORD	code;
	char	* string;
};


#define NUMS 23
static StringResource table[23] = {
{   IDS_VERSION, "0.60" },

{    IDS_GAME_TEXFILENOTFOUND ,"Standard Texfile not found" },
{    IDS_GAME_TEXFILEERROR   ,"Error reading standard Texfile"},
{    IDS_MODULEMAP_PREPARETEXTURE 
                            ,"Error in preparing texture in external module"},
{    IDS_GAME_MAPDEFERR      ,"Map definition error"},
{    IDS_GAME_SELECTION      ,"Selection stack error"},
{    IDS_MODULE_MAPSELECTION ,"Error in selection map type"},
{    IDS_MAP_TOONEIGHBOURS   ,"Too neighbours in map place "},
{    IDS_MODULE_CANTLOAD     ,"Can't load %s library"},
{    IDS_MODULE_INVALIDLIBRARY 
                            ,"The library %s ins't a valid Minesweeper library"},
{    IDS_MODULE_VERSION      ,"The library %s is not a version supported"},
{    IDS_MODULE_INVALIDMAPMOD ,"The library %s contains an invalid map module"},
{    IDS_MODULE_TOOMAPS      ,"Too maps in %s library"},
{    IDS_MODULE_INVALIDGAMEMOD ,"The library %s contains an invalid game module"},
{    IDS_OPENGL_CHOOSEPIXELFORMAT 
                            ,"ChoosePixelFormat() failed:\r\nCannot find a suitable pixel format"},
{    IDS_OPENGL_SETPIXELFORMAT 
                            ,"SetPixelFormat() failed:\r\nCannot set format specified"},
{    IDS_MAIN_PERFTIMER      ,"Performance Timer not supported by hardware"},
{    IDS_MAIN_STORESETTINGS  ,"Cannot store settings"},
{    IDS_ABOUT               ,"MineSweeper 3D - Version "},
{    IDS_MAIN_STANDARDLIBRARY 
                            ,"Cannot find standard library! (plugins\\standard.dll)"},
{    IDS_MAIN_MENUITEMS      ,"Error in handling menu items"},
{    IDS_VARS_MAPENUM        ,"Map handling error in saving data"},
{    IDS_VARS_DEFAULTSETTING ,"Default setting loaded"} };






const char * const GetString (DWORD code)
{
	for (int i=0; i<23; i++) 
		if (table[i].code == code)
			return table[i].string;
	return "Unknown error code";		
}