

#include "stdafx.h"
#include "map.h"


#undef __DEBUG_MAP_


			/* current rotation */
static GLfloat rot[2] = {0.0, 0.0};
			/* device context */
extern HDC hDC;			


static 	GLuint screenViewport [4];
static  char message [256] = "0123456789";








// ------------------------------------------------------- FONT
// ------------------------------------------------------- FONT
// ------------------------------------------------------- FONT
// ------------------------------------------------------- FONT

static GLuint fontOffset;

GLubyte space[] =
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

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



void makeRasterFont(void)
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
   glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, space);
   glEndList();
}


void printString(char *s)
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


/* IMAGE */
static char texFileName[] = "bitlist.raw";
static int singleTexWidth = 16, singleTexHeight = 16;
static int interleaveX = 1, interleaveY = 0;
static int texRows = 1, texColumns = 12;


/* Texture Object */
#define TEX_FLAG 0
#define TEX_MINE 1
#define TEX_NUMBER_BASE 1
static GLuint * texName = NULL;

BOOL	buildTextures()
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
	if (file == NULL) {
	    MessageBox(NULL, "Texfile not found", "Error", MB_OK);
	    return FALSE;
	}
	if (fread (buffer, width * height * 3, 1, file) != 1) {
	    MessageBox(NULL, "Texfile error", "Error", MB_OK);
	    return FALSE;
	}
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
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		                   GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
				           GL_LINEAR);

			glTexImage2D (GL_TEXTURE_2D,
				          0,
						  GL_R3_G3_B2,
						  singleTexWidth,
						  singleTexHeight,
						  0,
						  GL_RGB,
						  GL_UNSIGNED_BYTE,
			  			  buffer + width * 3 * (i * (singleTexHeight + interleaveY)) +
			                j * 3 * (singleTexWidth + interleaveX));
		}
						  
	
	free (buffer);
	return TRUE;
}


void	freeTextures()
{
	free (texName);
	texName = NULL;
}






























// ------------------------------------------------------- OBJECTS
// ------------------------------------------------------- OBJECTS
// ------------------------------------------------------- OBJECTS
// ------------------------------------------------------- OBJECTS


// Strutture dati
static	struct MINESWEEPER_MAP	* mapGame;
struct	MAP_POLY {
		// Numero di triangoli
		int		nTri;
		// Array [0..nTri-1], contiene indice di faccia del triangolo, secondo la mappa del gioco
		int		* triIdx;
		// Array [0..nTri*9] contenente in ordine tutti i vertici [x,y,z] di tutti i triangoli
		GLfloat	* triV;

		// Numero di altri poliogoni (lati > 3)
		int		nOthers;
		// Array [0..nOthers-1], contiene indice di faccia del poli, secondo la mappa del gioco
		int		* othersIdx;
		// Array [0..nOthers-1], contiene il numero di facce dei poligoni enumerati
		int		* othersNVx;			
		// Array [0..] contenente in ordine tutti i vertici [x,y,z] di tutti i poly
		GLfloat * othersV;
} mapPoly = {0, NULL, NULL, 0, NULL, NULL, NULL};


BOOL	buildStructure ()
{
	// Crea la MAP_POLY
	int i, j, k, nvo;
	int triP, othP, triIP, othIP;

	nvo = 0;
	mapPoly.nTri = 0;
	mapPoly.nOthers = 0;

	// Conta triangoli e altre facce
	for (i = 0; i < mapGame->nPlaces; i++) {
		for (j = 0; j < MAX_VERTEX_FACE; j++)
			if (mapGame->face->v[j] == -1) break;
		if (j < 3) {
#ifndef __DEBUG_MAP_
		    MessageBox(NULL, "Map definition error.", "Error", MB_OK);
		    return FALSE;
#else
			continue;
#endif
		}
		else if (j == 3)
			mapPoly.nTri++;
		else 
			mapPoly.nOthers++, nvo += j; 
	}

	// Crea buffers
	if (mapPoly.nOthers > 0) {
		mapPoly.othersNVx = (int*) malloc (sizeof(int) * mapPoly.nOthers);
		mapPoly.othersV = (GLfloat*) malloc (sizeof(GLfloat) * nvo * 3);
		mapPoly.othersIdx = (int*) malloc (sizeof(int) * mapPoly.nOthers);
	}
	if (mapPoly.nTri > 0) {
		mapPoly.triV = (GLfloat*) malloc (sizeof (GLfloat) * mapPoly.nTri * 9);	
		mapPoly.triIdx = (int*) malloc (sizeof (int) * mapPoly.nTri);
	}

	// Copia
	triP = 0;
	othP = 0;
	triIP = 0;
	othIP = 0;

	for (i = 0; i < mapGame->nPlaces; i++) {
		for (j = 0; j < MAX_VERTEX_FACE; j++)
			if (mapGame->face->v[j] == -1) break;
		if (j < 3)
			continue;
		if (j == 3) {
			// Nuovo triangolo
			mapPoly.triIdx[triIP++] = i; 

			for (k = 0; k < 3; k++) {
				mapPoly.triV[triP++] = mapGame->vertex[mapGame->face[i].v[k]].x;
				mapPoly.triV[triP++] = mapGame->vertex[mapGame->face[i].v[k]].y;
				mapPoly.triV[triP++] = mapGame->vertex[mapGame->face[i].v[k]].z;
			}
		}
		else {
			// Nuovo poligono
			mapPoly.othersIdx[othIP] = i;
			mapPoly.othersNVx[othIP++] = j;
			
			for (k = 0; k < j; k++) {
				mapPoly.othersV[othP++] = mapGame->vertex[mapGame->face[i].v[k]].x;
				mapPoly.othersV[othP++] = mapGame->vertex[mapGame->face[i].v[k]].y;
				mapPoly.othersV[othP++] = mapGame->vertex[mapGame->face[i].v[k]].z;
			}
		}
	}	
	
	return TRUE;
}


void	freeMap ()
{
	if (mapPoly.othersV)
		free (mapPoly.othersV);
	if (mapPoly.othersNVx)
		free (mapPoly.othersNVx);
	if (mapPoly.othersIdx)
		free (mapPoly.othersIdx);
	if (mapPoly.triV)
		free (mapPoly.triV);
	if (mapPoly.triIdx)
		free (mapPoly.triIdx);

	mapPoly.othersV = NULL;
	mapPoly.othersNVx = NULL;
	mapPoly.othersIdx = NULL;
	mapPoly.triV = NULL;
	mapPoly.triIdx = NULL;
}
































// ------------------------------------------------------- HITS
// ------------------------------------------------------- HITS
// ------------------------------------------------------- HITS
// ------------------------------------------------------- HITS

#define MAX_HITBUFSIZE 512

int		selectFace (int x, int y)
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
    gluPickMatrix ((GLdouble) x, (GLdouble) (screenViewport[3] - y),
                    5.0, 5.0, screenViewport);
    gluPerspective(45.0, (float) screenViewport[2] / screenViewport[3], 0.001, 30.0);

    glMatrixMode (GL_MODELVIEW);
	glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -3.6f);

    glRotatef(rot[0], 1.0f, 0.0f, 0.0f);
    glRotatef(rot[1], 0.0f, 1.0f, 0.0f);

	p = 0;
	if (mapPoly.nTri > 0) {
		for (i = 0; i < mapPoly.nTri; i++) {
			glLoadName (mapPoly.triIdx[i]);
			glBegin (GL_TRIANGLES);
			glVertex3fv (mapPoly.triV + p);
			glVertex3fv (mapPoly.triV + p + 3);
			glVertex3fv (mapPoly.triV + p + 6);
			glEnd();
			p += 9;
		}
	}

	p = 0;
	for (i = 0; i < mapPoly.nOthers; i++) {
		glLoadName (mapPoly.othersIdx[i]);
		glBegin (GL_POLYGON);
		for (j = 0; j < mapPoly.othersNVx[i]; j++) {
			glVertex3fv (mapPoly.othersV + p);
			p += 3;
		}
		glEnd();
	}
	
	glPopMatrix();
    glMatrixMode (GL_PROJECTION);
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
		if (selectBuf[p] != 1) {
		    MessageBox(NULL, "Selection stack error", "Error", MB_OK);
			p += 3 + selectBuf[p];
			continue;
		}
		if (selectBuf[p+1] < z)
			z = selectBuf[p+1], hit = (int)selectBuf[p+3], c = 0;
		else if (selectBuf[p+1] == z)
			c++;
		p += 4;
	}

	return (c == 0) ? hit : -1;
}





































// ------------------------------------------------------- GL
// ------------------------------------------------------- GL
// ------------------------------------------------------- GL
// ------------------------------------------------------- GL




BOOL	oglInit(struct MINESWEEPER_MAP * map)
{
	GLint nbits[3];

	mapGame = map;

	// Cerca il numero di bit per colore disponibile
	glGetIntegerv (GL_RED_BITS, nbits);
	glGetIntegerv (GL_GREEN_BITS, nbits+1);
	glGetIntegerv (GL_BLUE_BITS, nbits+2);
	if ((1 << (nbits[0] + nbits[1] + nbits[2])) < map->nPlaces) {
	    MessageBox(NULL, "Impossible to build colormap (bitperpixel insufficient", 
				   "Error", MB_OK);
	    return FALSE;
	}

	if (!buildStructure())
		return FALSE;

	if (map->isConvex) {
		glFrontFace (map->cullingMode);
		glCullFace (GL_BACK);
		glEnable (GL_CULL_FACE);
	}
	else {
		glEnable (GL_DEPTH_TEST);
	}

	makeRasterFont();
	if (!buildTextures())
		return FALSE;

	return TRUE;
}


void	oglClose()
{
	freeTextures();
	freeMap();
}


void	normalViewMode()
{
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float) screenViewport[2] / screenViewport[3], 0.001, 30.0);
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -3.6f);
}


BOOL	mouseButton (UINT msg, int x, int y)
{
	int hit;

	if (msg == WM_LBUTTONDOWN) {
		hit = selectFace (x, y);
		if (hit == -1)
			return FALSE;
		if (mapGame->place[hit] == PLACE_MINECOVERED)
			mapGame->place[hit] = PLACE_COVERED;
		else
			mapGame->place[hit] = PLACE_MINECOVERED;
		return TRUE;
	}
	if (msg == WM_RBUTTONDOWN) {
		normalViewMode();
		return TRUE;
	}
	return FALSE;
}


BOOL	mouseMove(int state, int dx, int dy)
{
	if (state != 2)
		return FALSE;
 
	
	rot[0] += (dy * 180.0f) / 222.0f;
	rot[1] -= (dx * 180.0f) / 222.0f;
#define clamp(x) x = x > 360.0f ? x-360.0f : x < -360.0f ? x+=360.0f : x
	clamp(rot[0]);
	clamp(rot[1]);
	return TRUE;
}



void	changeWindowSize(int width, int height)
{
	screenViewport[2] = width;
	screenViewport[3] = height;

    glViewport(0, 0, (GLsizei) width, (GLsizei) height);
	normalViewMode();
}








void	updateDisplay()
{
	int i, j, p, idx;

    glClearColor (0.1f, 0.2f, 0.3f, 0.0);

    if (mapGame->isConvex)
		glClear(GL_COLOR_BUFFER_BIT);
	else
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	/* ----------------- 3D mode ------------------ */
	glPushMatrix();
    glRotatef(rot[0], 1.0f, 0.0f, 0.0f);
    glRotatef(rot[1], 0.0f, 1.0f, 0.0f);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	
	glColor3f (0,0,0);				// per linee
	
	if (mapPoly.nTri > 0) {

		p = 0;
		for (i = 0; i < mapPoly.nTri; i++) {
			glBegin (GL_LINE_STRIP);
			glVertex3fv (mapPoly.triV + p);
			glVertex3fv (mapPoly.triV + p + 3);
			glVertex3fv (mapPoly.triV + p + 6);
			glEnd();
			p += 9;
		}

		p = 0;
		for (i = 0; i < mapPoly.nTri; i++) {
			idx = mapPoly.triIdx[i];
			if (mapGame->place[idx] == PLACE_COVERED) {
				glDisable (GL_TEXTURE_2D);
				glColor3f (0.5, 0.5, 0.5);
				glBegin (GL_TRIANGLES);
				glVertex3fv (mapPoly.triV + p);
				glVertex3fv (mapPoly.triV + p + 3);
				glVertex3fv (mapPoly.triV + p + 6);
				glEnd();
			}
			else {
				glEnable (GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, texName[TEX_FLAG]);
				glBegin (GL_TRIANGLES);
				glTexCoord2f (0,0);
				glVertex3fv (mapPoly.triV + p);
				glTexCoord2f (0.5,1);
				glVertex3fv (mapPoly.triV + p + 3);
				glTexCoord2f (1,0);
				glVertex3fv (mapPoly.triV + p + 6);
				glEnd();
			}
			p += 9;
		}
		glDisable (GL_TEXTURE_2D);
	}

	p = 0;
	for (i = 0; i < mapPoly.nOthers; i++) {
		glBindTexture(GL_TEXTURE_2D, texName[rand() % 3]);
		glBegin (GL_POLYGON);
		for (j = 0; j < mapPoly.othersNVx[i]; j++) {
			glColor3f ((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
			glVertex3fv (mapPoly.othersV + p);
			p += 3;
		}
		glEnd();
	}
	

	glPopMatrix();

	/* ----------------- 2D mode ------------------ */
	glDisable (GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();					// salva la 3d proj.
    glLoadIdentity();
    glOrtho (0.0, screenViewport[2], 0.0, screenViewport[3], 0, 10.0f);

	glColor3f(1,1,0);
	glRasterPos2i(10, 10);
	printString(message);

    glPopMatrix();					// ricaric. la 3d proj.
    glMatrixMode(GL_MODELVIEW);

    glFlush();
    SwapBuffers(hDC);				/* nop if singlebuffered */
}



