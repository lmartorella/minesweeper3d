

// WINDOWS SPECIFIC!!!!!!
#pragma once

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#include <afxwin.h>         // MFC core and standard components
#include <afxcmn.h>         // MFC core and standard components

#include <GL/gl.h>			/* OpenGL header file */
#include <GL/glu.h>			/* OpenGL utilities header file */

#include <math.h>
#include <stdio.h>


#define MINE_MODULE			HMODULE
#define MINE_MODULE_INVALID NULL
#define OpenFileLib(name) LoadLibrary(name)
#define CloseFileLib(mod) FreeLibrary(mod)
#define GetFunctionAddress(module,funcName) MINE_FUNCTION_DEFAULT(GetProcAddress(module,funcName))

