

#include "stdafx2.h"
#include "library.h"
#include "vars.h"
#include "display.h"
#include "modules.h"

#include "resource.h"

#undef __DEBUG_MAP_				// Abilita l'errore "too few vertices for place"



/* Viewport window */
static 	GLuint screenViewport [4];

/* FONT */
static  GLuint fontOffset;
#define CROSSHAIR 2

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

struct	STRUCTUREMAP_POLY {
		// Numero di poligoni (facce)
		int		nPoly;
		// Array [0..nPloy-1], contiene indice di faccia del poli, secondo la mappa del gioco
		int		* polyIdx;
		// Array [0..nPoly-1], contiene il numero di lati dei poligoni enumerati
		int		* polyNVx;			
		// Array [0..] contenente *in ordine* tutti i vertici [x,y,z] di tutti i poly
		GLfloat * vertexes;
} mapPoly = {0, NULL, NULL, NULL};

/* SELECT */
#define MAX_HITBUFSIZE 512

/* Mouse buttons */
static  int mButtons = 0;
static  GLfloat precision = 1.0;

/* GAME */
	   unsigned int timer = 0;
	   int gameStatus = GAME_STATUS_STOPPED;

static int gamePaused = 1;				// per crosshair


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


// ------------------------------------------------------- FONT
// ------------------------------------------------------- FONT
// ------------------------------------------------------- FONT
// ------------------------------------------------------- FONT


GLubyte space[] =
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

GLubyte crosshair[] =
	{ 0x08, 0x0, 0x1c, 0, 0x2a, 0, 0x41, 0, 0xeb, 0x80, 0x41, 0, 0x2a, 0, 0x1c, 0, 0x08, 0};
GLubyte minus[] =
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


GLubyte numbers[][11] = {
    {  0xfe, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0xfe },
    {  0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10 },
    {  0xfe, 0x80, 0x80, 0x80, 0x80, 0xfe, 0x02, 0x02, 0x02, 0x02, 0xfe },
    {  0xfe, 0x2,  0x2,  0x2,  0x02, 0xfe, 0x2,  0x2,  0x2,  0x2, 0xfe },
    {  0x02, 0x02, 0x02, 0x02, 0x02, 0xfe, 0x82, 0x82, 0x82, 0x82, 0x82 },
    {  0xfe, 0x02, 0x02, 0x02, 0x02, 0xfe, 0x80, 0x80, 0x80, 0x80, 0xfe },
    {  0xfe, 0x82, 0x82, 0x82, 0x82, 0xfe, 0x80, 0x80, 0x80, 0x80, 0xfe },
    {  0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0xfe },
    {  0xfe, 0x82, 0x82, 0x82, 0x82, 0xfe, 0x82, 0x82, 0x82, 0x82, 0xfe },
    {  0xfe, 0x02, 0x02, 0x02, 0x02, 0xfe, 0x82, 0x82, 0x82, 0x82, 0xfe },
};
GLubyte letters[][11] = {
    {  0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x18},
    {  0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
    {  0x7e, 0xe7, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},
    {  0xfc, 0xce, 0xc7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc7, 0xce, 0xfc},
    {  0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xc0, 0xff},
    {  0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xff},
    {  0x7e, 0xe7, 0xc3, 0xc3, 0xcf, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},
    {  0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
    {  0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e},
    {  0x7c, 0xee, 0xc6, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06},
    {  0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xe0, 0xf0, 0xd8, 0xcc, 0xc6, 0xc3},
    {  0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0},
    {  0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xdb, 0xff, 0xff, 0xe7, 0xc3},
    {  0xc7, 0xc7, 0xcf, 0xcf, 0xdf, 0xdb, 0xfb, 0xf3, 0xf3, 0xe3, 0xe3},
    {  0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xe7, 0x7e},
    {  0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
    {  0x3f, 0x6e, 0xdf, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c},
    {  0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe},
    {  0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0xe0, 0xc0, 0xc0, 0xe7, 0x7e},
    {  0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xff},
    {  0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
    {  0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
    {  0xc3, 0xe7, 0xff, 0xff, 0xdb, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3},
    {  0xc3, 0x66, 0x66, 0x3c, 0x3c, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3},
    {  0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3},
    {  0xff, 0xc0, 0xc0, 0x60, 0x30, 0x7e, 0x0c, 0x06, 0x03, 0x03, 0xff}
};



static void makeRasterFont(void)
{
   GLuint i, j;
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);    
   fontOffset = glGenLists (128);
   for (i = 0,j = '0'; i < 10; i++,j++) {
      glNewList(fontOffset + j, GL_COMPILE);
      glBitmap(8, 11, 0, 0, 10, 0, numbers[i]);
      glEndList();
   }
   for (i = 0,j = 'A'; i < 26; i++,j++) {
      glNewList(fontOffset + j, GL_COMPILE);
      glBitmap(8, 11, 0, 0, 10, 0, letters[i]);
      glEndList();
   }
   
   glNewList(fontOffset + ' ', GL_COMPILE);
   glBitmap(8, 11, 0, 0, 10.0, 0.0, space);
   glEndList();
   glNewList(fontOffset + '-', GL_COMPILE);
   glBitmap(8, 11, 0, 0, 10.0, 0.0, minus);
   glEndList();
	
   glNewList(fontOffset + CROSSHAIR, GL_COMPILE);
   glBitmap(9, 9, 0, 0, 0, 0, crosshair);
   glEndList();
}


static void printString(char *s)
{
   glPushAttrib (GL_LIST_BIT);
   glListBase(fontOffset);
   glCallLists(strlen(s), GL_UNSIGNED_BYTE, (GLubyte *) s);
   glPopAttrib ();
}






















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



static DWORD  buildStructure ()
{
	// Crea la MAP_POLY
	int i, j, k, nvo;
	int othP, othIP;

	nvo = 0;
	mapPoly.nPoly = 0;

	// Conta facce
	for (i = 0; i < map.nPlaces; i++) {
		for (j = 0; j < MAX_VERTEX_FACE; j++)
			if (map.face->v[j] == -1) break;
		if (j < 3) 
			return IDS_GAME_MAPDEFERR;
		mapPoly.nPoly++, nvo += j; 
	}

	// Crea buffers
	if (mapPoly.nPoly > 0) {
		mapPoly.polyNVx = (int*) malloc (sizeof(int) * mapPoly.nPoly);
		mapPoly.vertexes = (GLfloat*) malloc (sizeof(GLfloat) * nvo * 3);
		mapPoly.polyIdx = (int*) malloc (sizeof(int) * mapPoly.nPoly);
	}

	// Copia
	othP = 0;
	othIP = 0;

	for (i = 0; i < map.nPlaces; i++) {
		for (j = 0; j < MAX_VERTEX_FACE; j++)
			if (map.face->v[j] == -1) break;

		// Nuovo poligono
		mapPoly.polyIdx[othIP] = i;
		mapPoly.polyNVx[othIP++] = j;
			
		for (k = 0; k < j; k++) {
			mapPoly.vertexes[othP++] = map.vertex[map.face[i].v[k]].x;
			mapPoly.vertexes[othP++] = map.vertex[map.face[i].v[k]].y;
			mapPoly.vertexes[othP++] = map.vertex[map.face[i].v[k]].z;
		}
	}	
	
	return 0;
}


static void	freeStructure ()
{
	if (mapPoly.vertexes)
		free (mapPoly.vertexes);
	if (mapPoly.polyNVx)
		free (mapPoly.polyNVx);
	if (mapPoly.polyIdx)
		free (mapPoly.polyIdx);

	mapPoly.vertexes = NULL;
	mapPoly.polyNVx = NULL;
	mapPoly.polyIdx = NULL;
}
































// ------------------------------------------------------- HITS
// ------------------------------------------------------- HITS
// ------------------------------------------------------- HITS
// ------------------------------------------------------- HITS


static DWORD selectFace (int * face)
{
	int i, j, p, hit, c;
	static GLuint selectBuf[MAX_HITBUFSIZE];
    GLint hits;
	GLuint z;
	static char buffer[20];

    glSelectBuffer (MAX_HITBUFSIZE, selectBuf);
    glRenderMode (GL_SELECT);

    glInitNames();
    glPushName(0);

    glMatrixMode (GL_PROJECTION);
	glPushMatrix();
    glLoadIdentity ();

    gluPickMatrix ((GLdouble)screenViewport[2] / 2.0, (GLdouble)screenViewport[3] / 2.0,
                    precision, precision, screenViewport);

	p_SetCameraParams (mapIndex, screenViewport[2], screenViewport[3]); 

	p = 0;
	for (i = 0; i < mapPoly.nPoly; i++) {
		glLoadName (mapPoly.polyIdx[i]);
		glBegin (GL_POLYGON);
		for (j = 0; j < mapPoly.polyNVx[i]; j++) {
			glVertex3fv (mapPoly.vertexes + p);
			p += 3;
		}
		glEnd();
	}
	
	glPopMatrix();
    glMatrixMode (GL_MODELVIEW);

	glFlush ();
    hits = glRenderMode (GL_RENDER);

	// Processa gli hits
	p = 0;
	z = (GLuint)-1;
	hit = -1;
	c = 0;
	for (i = 0; i < hits; i++) {
		if (selectBuf[p] != 1) 
			return IDS_GAME_SELECTION;
		if (selectBuf[p+1] < z)
			z = selectBuf[p+1], hit = (int)selectBuf[p+3], c = 0;
		else if (selectBuf[p+1] == z)
			c++;
		p += 4;
	}

	*face = (c == 0) ? hit : -1;
	return 0;
}





































































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



DWORD	oglInit()
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
	gamePaused = 1;
	gameStatus = GAME_STATUS_NORMAL;
	timerSet (0);
}



void	gameClose(int destroy)
{
	if (destroy)
		freeStructure();
	gameStatus = GAME_STATUS_STOPPED;
}


void	oglClose ()
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










void	pause()
{
	gamePaused = 1;
}


void	unpause()
{
	gamePaused = 0;
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






void	updateDisplay()
{
	int i, j, k, p, idx, m, ext;
	GLuint name;
	char string[10];
	char cross[2];

	if (gameStatus == GAME_STATUS_ENDED_LOST)
	    glClearColor (0.6f, 0, 0, 0);
	else if (gameStatus == GAME_STATUS_ENDED_WIN)
		glClearColor (0.0f, 0.4f, 0.0f, 0);
	else 
		glClearColor (0.1f, 0.2f, 0.3f, 0);
		
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (gameStatus == GAME_STATUS_STOPPED) {
	    glFlush();
		return;
	}

	/* ----------------- 3D mode ------------------ */
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	
	glColor3f (0,0,0);				// per linee

	// ***************************** WIREFRAME
	p = 0;
	for (i = 0; i < mapPoly.nPoly; i++) {
		glBegin (GL_LINE_STRIP);
		for (j = 0; j < mapPoly.polyNVx[i]; j++) {
			glVertex3fv (mapPoly.vertexes + p);
			p += 3;
		}
		glEnd();
	}
		
		
	// ****************************** TEXTURING
	p = 0;
	for (i = 0; i < mapPoly.nPoly; i++) {
		idx = mapPoly.polyIdx[i];
		m = map.place[idx];

		ext = p_GetTextureName (m, &name);
		// Override personal textures
		if (m & MAP_PLACE_COVERED && !(m & MAP_PLACE_FLAG) && !ext) {
			glDisable (GL_TEXTURE_2D);
			glColor3f (0.5, 0.5, 0.5);
			glBegin (GL_POLYGON);
			for (j = 0; j < mapPoly.polyNVx[i]; j++, p+=3)
				glVertex3fv (mapPoly.vertexes + p);
			glEnd();
		}
		else if (m == 0 && !ext)
		{
			glDisable (GL_TEXTURE_2D);
			glColor3f (0.75, 0.75, 0.75);
			glBegin (GL_POLYGON);
			for (j = 0; j < mapPoly.polyNVx[i]; j++, p+=3)
				glVertex3fv (mapPoly.vertexes + p);
			glEnd();
		}
		else {
			if (!ext) {
				if (m & MAP_PLACE_FALSEFLAG)
					name = texName[TEX_FLAGCROSSED];
				else if (m & MAP_PLACE_FLAG)
					name = texName[TEX_FLAG];
				else if (m & MAP_PLACE_MINE)
					name = texName[TEX_MINE];
				else
					name = texName[TEX_NUMBASE + (m & MAP_PLACE_NUMMASK)];
			}

			glEnable (GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, name);

			glBegin (GL_POLYGON);
			k = MAX_VERTEX_FACE * 2 * (mapPoly.polyNVx[i] - 3);
			for (j = 0; j < mapPoly.polyNVx[i]; j++, k+=2, p+=3) {
				glTexCoord2f (texVertex[k], texVertex[k + 1]);
				glVertex3fv (mapPoly.vertexes + p);
			}
			glEnd();
		}
	}

	/* ----------------- 2D mode ------------------ */
	glDisable (GL_TEXTURE_2D);
	
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();					// salva la 3d proj.
    glLoadIdentity();
    gluOrtho2D (0, screenViewport[2], 0, screenViewport[3]);

	glColor3f(1,1,0);

	// Mine rimanenti
	glRasterPos2i(10, 10);
	itoa (map.nMines, string, 10);
	printString(string);

	// Tempo e record
	itoa ((timer < 99990) ? timer / 10 : 9999, string, 10);
	glRasterPos2i(screenViewport[2] - 10 * (strlen (string)+1), 10);
	printString(string);

	itoa (recordArray->record, string, 10);
	glRasterPos2i(screenViewport[2] - 10 * (strlen (string)+1), 23);
	printString(string);

	// Puntatore
	if (!gamePaused) {
		cross[0] = CROSSHAIR;
		cross[1] = 0;
		glRasterPos2i (screenViewport[2] / 2 - 4, screenViewport[3] / 2 - 4);
		printString (cross);
	}

    glPopMatrix();					// ricaric. la 3d proj.
    glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

    glFlush();
}

