


#include "stdafx.h"

#define THIS_IS_A_MODULE
#include "..\..\library.h"
#include "..\..\resource.h"



void	GetGraphicsName (char * name)
{
	strcpy (name, "Default OpenGL");
}




static     HGLRC				hRC;					// opengl context 
static     HPALETTE			hPalette;					// custom palette (if needed) 




DWORD	PrepareOpenGL(HDC hDC)
{
    PIXELFORMATDESCRIPTOR pfd;

    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize        = sizeof(pfd);
    pfd.nVersion     = 1;
    pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL /*| PFD_GENERIC_ACCELERATED */| 
		               PFD_DOUBLEBUFFER;
    pfd.iPixelType   = PFD_TYPE_RGBA;
    pfd.cDepthBits   = 16;
    pfd.cColorBits   = 16;
	pfd.dwLayerMask	 = PFD_MAIN_PLANE;

    int pf = ChoosePixelFormat(hDC, &pfd);
    if (pf == 0) 
		return IDS_OPENGL_CHOOSEPIXELFORMAT;
 
    if (SetPixelFormat(hDC, pf, &pfd) == FALSE) 
		return IDS_OPENGL_SETPIXELFORMAT;

    DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    LOGPALETTE* lpPal;

    if (pfd.dwFlags & PFD_NEED_PALETTE ||
	pfd.iPixelType == PFD_TYPE_COLORINDEX) {

		int n = 1 << pfd.cColorBits;
		if (n > 256) n = 256;

		lpPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) +
					    sizeof(PALETTEENTRY) * n);
		memset(lpPal, 0, sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * n);
		lpPal->palVersion = 0x300;
		lpPal->palNumEntries = n;

		GetSystemPaletteEntries(hDC, 0, n, &lpPal->palPalEntry[0]);
    
		if (pfd.iPixelType == PFD_TYPE_RGBA) {
		    int redMask = (1 << pfd.cRedBits) - 1;
		    int greenMask = (1 << pfd.cGreenBits) - 1;
		    int blueMask = (1 << pfd.cBlueBits) - 1;
		    int i;

			for (i = 0; i < n; ++i) {
				lpPal->palPalEntry[i].peRed = 
				    (((i >> pfd.cRedShift)   & redMask)   * 255) / redMask;
				lpPal->palPalEntry[i].peGreen = 
				    (((i >> pfd.cGreenShift) & greenMask) * 255) / greenMask;
				lpPal->palPalEntry[i].peBlue = 
				    (((i >> pfd.cBlueShift)  & blueMask)  * 255) / blueMask;
				lpPal->palPalEntry[i].peFlags = 0;
			}
		} else {
		    lpPal->palPalEntry[0].peRed = 0;
		    lpPal->palPalEntry[0].peGreen = 0;
		    lpPal->palPalEntry[0].peBlue = 0;
		    lpPal->palPalEntry[0].peFlags = PC_NOCOLLAPSE;
		    lpPal->palPalEntry[1].peRed = 255;
		    lpPal->palPalEntry[1].peGreen = 0;
		    lpPal->palPalEntry[1].peBlue = 0;
		    lpPal->palPalEntry[1].peFlags = PC_NOCOLLAPSE;
		    lpPal->palPalEntry[2].peRed = 0;
		    lpPal->palPalEntry[2].peGreen = 255;
		    lpPal->palPalEntry[2].peBlue = 0;
		    lpPal->palPalEntry[2].peFlags = PC_NOCOLLAPSE;
		    lpPal->palPalEntry[3].peRed = 0;
		    lpPal->palPalEntry[3].peGreen = 0;
		    lpPal->palPalEntry[3].peBlue = 255;
		    lpPal->palPalEntry[3].peFlags = PC_NOCOLLAPSE;
		}

		hPalette = CreatePalette(lpPal);
		if (hPalette) {
		    SelectPalette(hDC, hPalette, FALSE);
		    RealizePalette(hDC);
		}
		free(lpPal);
    }

    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);
	return 0;
}




DWORD	UnprepareOpenGL(HDC hDC)
{
	wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);

    if (hPalette)
		DeleteObject(hPalette);
	return 0;
}





struct	STRUCTUREMAP_POLY {
		// Numero di poligoni (facce)
		int		nPoly;
		// Array [0..nPoly-1], contiene indice di faccia del poli, secondo la mappa del gioco
		int		* polyIdx;
		// Array [0..nPoly-1], contiene il numero di lati dei poligoni enumerati
		int		* polyNVx;			
		// Array [0..] contenente *in ordine* tutti i vertici [x,y,z] di tutti i poly
		GLfloat * vertexes;
} mapPoly = {0, NULL, NULL, NULL};



// ------------------------------------------------------- FONT
// ------------------------------------------------------- FONT
// ------------------------------------------------------- FONT
// ------------------------------------------------------- FONT
/* FONT */
static  GLuint fontOffset;
#define CROSSHAIR 2


GLubyte space[] =
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

GLubyte crosshair_bits[] =
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
   glBitmap(9, 9, 0, 0, 0, 0, crosshair_bits);
   glEndList();
}


static void printString(char *s)
{
   glPushAttrib (GL_LIST_BIT);
   glListBase(fontOffset);
   glCallLists(strlen(s), GL_UNSIGNED_BYTE, (GLubyte *) s);
   glPopAttrib ();
}










DWORD	Renderer (int gameStatus, MINESWEEPER_MAP * map, GETTEXTPLACE_TYPE p_GetTextPlace,
				  GLfloat * texVertex, int width, int height, int timer, int record,
				  int active)
{
	int i, j, k, p, idx, m, isTexture;
	GLuint name;
	GLfloat b, g, r;
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
		return 0;
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
		m = map->place[idx];
		isTexture = p_GetTextPlace (m, &name, &b, &g, &r);
		
		// Override personal textures
		if (!isTexture) {
			glDisable (GL_TEXTURE_2D);
			glColor3f (b, g, r);
			glBegin (GL_POLYGON);
			for (j = 0; j < mapPoly.polyNVx[i]; j++, p+=3)
				glVertex3fv (mapPoly.vertexes + p);
			glEnd();
		}
		else {
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
    gluOrtho2D (0, width, 0, height);

	glColor3f(1,1,0);

	// Mine rimanenti
	glRasterPos2i(10, 10);
	itoa (map->nMines, string, 10);
	printString(string);

	// Tempo e record
	itoa ((timer < 999900) ? timer / 100 : 9999, string, 10);
	glRasterPos2i(width - 10 * (strlen (string)+1), 10);
	printString(string);

	itoa (record / 100, string, 10);
	glRasterPos2i(width - 10 * (strlen (string)+1), 23);
	printString(string);

	// Puntatore
	if (!active) {
		cross[0] = CROSSHAIR;
		cross[1] = 0;
		glRasterPos2i (width / 2 - 4, height / 2 - 4);
		printString (cross);
	}

    glPopMatrix();					// ricaric. la 3d proj.
    glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

    glFlush();
	return 0;
}





DWORD  buildStructure (MINESWEEPER_MAP * map)
{
	// Crea la MAP_POLY
	int i, j, k, nvo;
	int othP, othIP;

	nvo = 0;
	mapPoly.nPoly = 0;

	// Conta facce
	for (i = 0; i < map->nPlaces; i++) {
		for (j = 0; j < MAX_VERTEX_FACE; j++)
			if (map->face->v[j] == -1) break;
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

	for (i = 0; i < map->nPlaces; i++) {
		for (j = 0; j < MAX_VERTEX_FACE; j++)
			if (map->face->v[j] == -1) break;

		// Nuovo poligono
		mapPoly.polyIdx[othIP] = i;
		mapPoly.polyNVx[othIP++] = j;
			
		for (k = 0; k < j; k++) {
			mapPoly.vertexes[othP++] = map->vertex[map->face[i].v[k]].x;
			mapPoly.vertexes[othP++] = map->vertex[map->face[i].v[k]].y;
			mapPoly.vertexes[othP++] = map->vertex[map->face[i].v[k]].z;
		}
	}	
	
	return 0;
}


void	freeStructure ()
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



/* SELECT */
#define MAX_HITBUFSIZE 512
static  GLfloat precision = 1.0;



DWORD selectFace (int * face, GLint screenViewport[])
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



