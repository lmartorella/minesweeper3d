

#include "stdafx2.h"
#include "library.h"
#include "vars.h"
#include "display.h"
#include "modules.h"

#include "resource.h"

#undef __DEBUG_MAP_				// Abilita l'errore "too few vertices for place"



/* Viewport window */
static 	GLuint screenViewport [4];

/* FILE IMAGE */
static char texFileName[] = "textures.raw";
static int singleTexWidth = 32, singleTexHeight = 32;
static int interleaveX = 1, interleaveY = 1;
static int texRows = 1, texColumns = 15;

/* Texture Object */
#define TEX_FLAG 1
#define TEX_FLAGCROSSED 14
#define TEX_MINE 0
#define TEX_NUMBASE 1
static GLuint * texName = NULL;
static GLfloat * texVertex = NULL;

/* MAPPA */
extern struct 	MINESWEEPER_MAP	   	map;


/* Mouse buttons */
static  int mButtons = 0;

/* GAME */
	   unsigned int timer = 0;
	   int gameStatus = GAME_STATUS_STOPPED;

static int crosshair = 1;				// per crosshair


extern struct GLOBAL_VARS vars;
extern struct RECORD * recordArray;

/* FUNCTIONS DLL */
int						mapIndex;
BUILDMAP_TYPE			p_BuildMap;
DESTROYMAP_TYPE			p_DestroyMap;
MOUSEMOVE_TYPE			p_MouseMove;
SETCAMERAPARAMS_TYPE	p_SetCameraParams;
RESETMAP_TYPE			p_ResetMap;

MOUSEBUTTON_TYPE		p_MouseButton = NULL;
PREPAREMAP_TYPE			p_PrepareMap = NULL;
GETTEXTURENAME_TYPE		p_GetTextureName = NULL;
PREPARETEXTURES_TYPE	p_PrepareTextures = NULL;
FREETEXTURES_TYPE		p_FreeTextures = NULL;


















// ------------------------------------------------------- TEXTURES
// ------------------------------------------------------- TEXTURES
// ------------------------------------------------------- TEXTURES
// ------------------------------------------------------- TEXTURES
// ------------------------------------------------------- TEXTURES



static DWORD	buildTextures()
{
	int texNum, i, j, p, width, height;

	char * buffer;
	FILE * file;

	texNum = texRows * texColumns;
	texName = (GLuint*) malloc (sizeof (GLuint) * texNum);
    
	// Legge immagine
	width = texColumns * (singleTexWidth + interleaveX);
	height = texRows * (singleTexHeight + interleaveY);

	buffer = malloc (width * height * 3);
	file = fopen (texFileName, "rb");
	if (file == NULL) 
		return IDS_GAME_TEXFILENOTFOUND;

	if (fread (buffer, width * height * 3, 1, file) != 1) 
		return IDS_GAME_TEXFILEERROR;

	fclose (file);


	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, width);

    glGenTextures(texNum, texName);

	p = 0;
	for (i = 0; i < texRows; i++) 
		for (j = 0; j < texColumns; j++, p++) {
			glBindTexture(GL_TEXTURE_2D, texName[p]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			if (vars.filtering) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
			                GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					           GL_LINEAR);
			}
			else {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
								GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					           GL_NEAREST);
			}
			glTexImage2D (GL_TEXTURE_2D,
				          0,
						  GL_RGB,
						  singleTexWidth,
						  singleTexHeight,
						  0,
						  GL_RGB,
						  GL_UNSIGNED_BYTE,
			  			  buffer + width * 3 * (i * (singleTexHeight + interleaveY)) +
			                j * 3 * (singleTexWidth + interleaveX));
			}
						  
	free (buffer);

	return p_PrepareTextures(vars.filtering);			// Extern textures
}


static void	freeTextures()
{
	free (texName);
	texName = NULL;
	p_FreeTextures();				// Extern textures
}


DWORD rebuildTextures()
{
	freeTextures();
	return buildTextures();
}


















































// ------------------------------------------------------- OBJECTS
// ------------------------------------------------------- OBJECTS
// ------------------------------------------------------- OBJECTS
// ------------------------------------------------------- OBJECTS



































































































int		processMouseButton (int msg, int x, int y, int * unlock)
{
	int redraw;

	if (gameStatus == GAME_STATUS_STOPPED) {
		*unlock = 1;
		return 0;
	}

	mButtons &= ~MINE_UNLOCK;
	redraw = p_MouseButton (msg, &mButtons, &gameStatus, selectFace, &map);
	*unlock = (mButtons & MINE_UNLOCK) ? 1 : 0;

	if (gameStatus == GAME_STATUS_ENDED_WIN) 
		if (timer / 10 < recordArray->record)
			gameStatus = GAME_STATUS_ENDED_WINRECORD;

	return redraw;
}








































































// ------------------------------------------------------- GL
// ------------------------------------------------------- GL
// ------------------------------------------------------- GL
// ------------------------------------------------------- GL



DWORD	mine_init()
{
	double angle, incAngle;
	GLfloat u, v;
	int i, j;
	DWORD err;
	
	glEnable (GL_DEPTH_TEST);

	makeRasterFont();
	if ((err = buildTextures()) != 0)
		return err;

	/* Costruisce la tabella dei vertici in coordinate u,v */
	if (texVertex != NULL)
		free (texVertex);
	texVertex = (GLfloat *) malloc (sizeof (GLfloat) * MAX_VERTEX_FACE * 2 * (MAX_VERTEX_FACE - 2));
	for (i = 3; i < MAX_VERTEX_FACE; i++) {
		incAngle = 3.141592 * 2 / i;
		angle = 3.141592 * 3 / 2 - incAngle;
		for (j = 0; j < i; j++, angle += incAngle) {
			u = (GLfloat) (cos (angle) / 2 + 0.5);
			v = (GLfloat) (sin (angle) / 2 + 0.5);
			texVertex [((i-3) * MAX_VERTEX_FACE * 2) + j * 2] = u;
			texVertex [((i-3) * MAX_VERTEX_FACE * 2) + j * 2 + 1] = v;
		}
	}

	gameStatus = GAME_STATUS_STOPPED;
	return 0;
}




void	timerSet (int t)
{
	if (gameStatus != GAME_STATUS_ENDED_WIN && gameStatus != GAME_STATUS_ENDED_LOST)
		timer = t;
}


void	resetGame()
{
	crosshair = 1;
	gameStatus = GAME_STATUS_NORMAL;
	timerSet (0);
}



void	gameClose(int destroy)
{
	if (destroy)
		freeStructure();
	gameStatus = GAME_STATUS_STOPPED;
}


void	mine_exit ()
{
	freeTextures();
	free (texVertex);
}





void	changeWindowSize(int width, int height)
{
	screenViewport[2] = width;
	screenViewport[3] = height;

    glViewport(0, 0, (GLsizei) width, (GLsizei) height);

	if (p_SetCameraParams != NULL) {
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		p_SetCameraParams (mapIndex, screenViewport[2], screenViewport[3]);
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}
}










void	hideCrosshair()
{
	crosshair = 1;
}


void	showCrosshair()
{
	crosshair = 0;
}




DWORD	newGame(DWORD code)
{
	int changed;
	DWORD err;

	changed = map.code != code;
	gameClose(changed);

	if (changed) {
		if (p_DestroyMap != NULL)
			p_DestroyMap (mapIndex, &map);
		postDestroyMap ();
	
		err = UpdateMapFunctions (code); 
		
		if (err)
			return err;
		
		p_BuildMap (mapIndex, &map);
		err = postBuildMap (); 
		if (err) 
			return err;
	}

	map.code = code;
	map.nMines = map.nChecked = 0;

	p_PrepareMap (&map, placeNumbers);					// mette le mine
	
	if (changed) 
		return buildStructure();
	return 0;
}








