

#include "stdafx2.h"
#include "map.h"
#include "vars.h"


#undef __DEBUG_MAP_				// Abilita l'errore "too few vertices for place"



/* Viewport window */
static 	GLuint screenViewport [4];

/* FONT */
static GLuint fontOffset;
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

/* Mappa */
extern 	struct MINESWEEPER_MAP map;
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

/* SELECT */
#define MAX_HITBUFSIZE 512

/* Mouse buttons */
static  int buttons = 0;
static  GLfloat precision = 1.0;
#define X_SENS 2.25e-03f
#define Y_SENS 2.25e-03f

/* GAME */
static int gamePaused = 0;
static int game = 0;
static int time = 0;
static int ended = 0;
	   int youWin = 0, youLose = 0, youWinRecord = 0;
static GLfloat rot[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};

/* OPENGL */
#define Z_VIEW -3.6f
#define Z_MIN  0.1f
#define Z_MAX  10.0f
#define FOV    32.0f		


extern struct GLOBAL_VARS vars;




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
   glBitmap(8, 11, 0, 0, 10.0, 0.0, space);
   glEndList();
   glNewList(fontOffset + '-', GL_COMPILE);
   glBitmap(8, 11, 0, 0, 10.0, 0.0, minus);
   glEndList();
	
   glNewList(fontOffset + CROSSHAIR, GL_COMPILE);
   glBitmap(9, 9, 0, 0, 0, 0, crosshair);
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



int		buildTextures()
{
	int texNum, i, j, p, width, height;
	double angle, incAngle;
	GLfloat u, v;

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
		return 0;
	}
	if (fread (buffer, width * height * 3, 1, file) != 1) {
	    MessageBox(NULL, "Texfile error", "Error", MB_OK);
	    return 0;
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

	/* Costruisce la tabella dei vertici in coordinate u,v */
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

	return 1;
}


void	freeTextures()
{
	free (texName);
	texName = NULL;
	free (texVertex);
	texVertex = NULL;
}



int	 rebuildTextures()
{
	freeTextures();
	return buildTextures();
}





























// ------------------------------------------------------- OBJECTS
// ------------------------------------------------------- OBJECTS
// ------------------------------------------------------- OBJECTS
// ------------------------------------------------------- OBJECTS



int		buildStructure ()
{
	// Crea la MAP_POLY
	int i, j, k, nvo;
	int triP, othP, triIP, othIP;

	nvo = 0;
	mapPoly.nTri = 0;
	mapPoly.nOthers = 0;

	// Conta triangoli e altre facce
	for (i = 0; i < map.nPlaces; i++) {
		for (j = 0; j < MAX_VERTEX_FACE; j++)
			if (map.face->v[j] == -1) break;
		if (j < 3) {
#ifndef __DEBUG_MAP_
		    MessageBox(NULL, "Map definition error.", "Error", MB_OK);
		    return 0;
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

	for (i = 0; i < map.nPlaces; i++) {
		for (j = 0; j < MAX_VERTEX_FACE; j++)
			if (map.face->v[j] == -1) break;
		if (j < 3)
			continue;
		if (j == 3) {
			// Nuovo triangolo
			mapPoly.triIdx[triIP++] = i; 

			for (k = 0; k < 3; k++) {
				mapPoly.triV[triP++] = map.vertex[map.face[i].v[k]].x;
				mapPoly.triV[triP++] = map.vertex[map.face[i].v[k]].y;
				mapPoly.triV[triP++] = map.vertex[map.face[i].v[k]].z;
			}
		}
		else {
			// Nuovo poligono
			mapPoly.othersIdx[othIP] = i;
			mapPoly.othersNVx[othIP++] = j;
			
			for (k = 0; k < j; k++) {
				mapPoly.othersV[othP++] = map.vertex[map.face[i].v[k]].x;
				mapPoly.othersV[othP++] = map.vertex[map.face[i].v[k]].y;
				mapPoly.othersV[othP++] = map.vertex[map.face[i].v[k]].z;
			}
		}
	}	
	
	return 1;
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
                    precision, precision, screenViewport);
    gluPerspective(FOV, (float) screenViewport[2] / screenViewport[3], 0.001, 30.0);

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





































































// ------------------------------------------------ MOUSE EVENTS
// ------------------------------------------------ MOUSE EVENTS
// ------------------------------------------------ MOUSE EVENTS
// ------------------------------------------------ MOUSE EVENTS



void	mult (GLfloat * m1, GLfloat * m2, GLfloat * r)
{
	int i, j, k;
	
	for (i = 0; i < 3; i++)				// i riga
		for (j = 0; j < 3; j++) {		// j colonna
			r[i + j * 4] = 0;
			for (k = 0; k < 3; k++)
				r[i + j * 4] += m1[i + k * 4] * m2[j * 4 + k];
		}
}



int		mouseMove(int dx, int dy)
{
	GLfloat x, y, cf, sf, ct, st;
	static GLfloat	rotxy[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}, 
					r[16]     = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};

	y = dy * X_SENS;
	x = dx * Y_SENS;
	
	ct = (float)cos (x), st = (float)sin (x);
	cf = (float)cos (y), sf = (float)sin (y);

	rotxy[0] = ct, rotxy[1] = st*sf, rotxy[2] = -st*cf;
	rotxy[5] = cf, rotxy[6] = sf;
	rotxy[8] = st, rotxy[9] = -sf*ct, rotxy[10] = ct*cf;
	
	mult (rotxy, rot, r);
	memcpy (rot, r, sizeof (GLfloat) * 16);

	glLoadIdentity();
	glTranslatef (0.0, 0.0, Z_VIEW);
	glMultMatrixf (rot);

	return 1;
}




void	totalSweeper(int uncover)
{
	int i;
	for (i = 0; i < map.nPlaces; i++) {
		if (map.place[i] & MAP_PLACE_FLAG && !(map.place[i] & MAP_PLACE_MINE))
			map.place[i] |= MAP_PLACE_FALSEFLAG;
		if (map.place[i] & MAP_PLACE_MINE && !(map.place[i] & MAP_PLACE_FLAG))
			if (uncover)
				map.place[i] &= ~(MAP_PLACE_COVERED);
			else
				map.place[i] |= MAP_PLACE_FLAG;
	}
}




// Ritorna 1 se il campo è finito!
int		secureSweeper (int idx)
{
	int i = -1, neigh;
	int end = 0;

#ifdef _DEBUG
	if (map.place[idx] & MAP_PLACE_COVERED == 0)
		__asm {int 3}
#endif

	map.place[idx] &= (~MAP_PLACE_COVERED);
	map.nChecked++;

	if ((map.place[idx] & MAP_PLACE_NUMMASK) != 0)
		return (map.nChecked + map.nTotalMines == map.nPlaces);

	while (i++, ((neigh = map.neighbour[idx].n[i]) != -1) && i < MAX_NEIGHBOURS)
		if (map.place[neigh] & MAP_PLACE_COVERED && !(map.place[neigh] & MAP_PLACE_FLAG)) 
			if (secureSweeper (neigh))
				end = 1;

	return end;
}



int		mouseButton (UINT msg, int x, int y, int * unlock)
{
	int hit, k, n, err, flag, a;

	*unlock = 0;
	if (!game) {
		*unlock = 1;
		return 0;
	}

	switch (msg) {
	case WM_LBUTTONDOWN:
		buttons |= 1;
		return 0;

	case WM_RBUTTONDOWN:
		buttons |= 2;
		if (ended)
			return 0;

		hit = selectFace (x, y);
		if (hit == -1)
			return 0;
		if (!(map.place[hit] & MAP_PLACE_COVERED))
			return 0;

		map.place[hit] ^= MAP_PLACE_FLAG;
		if (map.place[hit] & MAP_PLACE_FLAG)
			map.nMines--;
		else
			map.nMines++;
		return 1;

	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		
		if (ended) {
			*unlock = 1;
			return 0;
		}

		hit = selectFace (x, y);
		if (hit == -1)
			return 0;

		if (msg == WM_LBUTTONUP && buttons == 1) {
			buttons = 0;
			if ((!(map.place[hit] & MAP_PLACE_COVERED)) ||
				map.place[hit] & MAP_PLACE_FLAG) 
				return 0;

			if (map.place[hit] & MAP_PLACE_MINE) {
				totalSweeper(1);
				ended = youLose = 1;
				return 1;		
			}
			if (secureSweeper (hit)) {
				ended = youWin = *unlock = 1;			// finito!
				if (time / 10 < vars.hallsOfFame[map.typeIndex].time)
					vars.hallsOfFame[map.typeIndex].time = time / 10, youWinRecord = 1;
				map.nMines = 0;
				totalSweeper(0);
			}
			return 1;
		}
		else if (msg == WM_RBUTTONUP && !(buttons & 1)) {
			buttons &= ~2;
			return 0;
		}
		else if (buttons == 3) {
			// Proc. doppio tasto
			buttons = 0;
			n = map.place[hit] & MAP_PLACE_NUMMASK;
			if (map.place[hit] & MAP_PLACE_COVERED || n == 0)
				return 0;
			
			err = flag = 0;
			for (k = 0; k < MAX_NEIGHBOURS && map.neighbour[hit].n[k] != -1; k++) {
				a = map.place[map.neighbour[hit].n[k]] & (MAP_PLACE_MINE | MAP_PLACE_FLAG);
				if (a != 0 && a != (MAP_PLACE_MINE | MAP_PLACE_FLAG))
					err++;
				if (a & MAP_PLACE_FLAG)
					flag++;
			}
			if (err == 0) {
				k = -1;
				while (k++, ((n = map.neighbour[hit].n[k]) != -1) && k < MAX_NEIGHBOURS)
					if (map.place[n] & MAP_PLACE_COVERED && !(map.place[n] & MAP_PLACE_FLAG)) 
						if (secureSweeper (n))
							ended = 1;

				if (ended) {
					youWin = *unlock = 1;			// finito!
					if (time / 10 < vars.hallsOfFame[map.typeIndex].time)
						vars.hallsOfFame[map.typeIndex].time = time / 10, youWinRecord = 1;
					map.nMines = 0;
					totalSweeper(0);
				}
				return 1;
			}
			else if (flag != n)
				return 0;
			// Esplode
			totalSweeper(1);
			ended = youLose = 1;
			return 1;
		}

	default:
		return 0;
	}
}





































// ------------------------------------------------------- GL
// ------------------------------------------------------- GL
// ------------------------------------------------------- GL
// ------------------------------------------------------- GL


void	setMatrix()
{
	glMatrixMode (GL_MODELVIEW);
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FOV, (float) screenViewport[2] / screenViewport[3], Z_MIN, Z_MAX);
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}



int		oglInit()
{
	glEnable (GL_DEPTH_TEST);

	makeRasterFont();
	if (!buildTextures())
		return 0;

	setMatrix();

	game = 0;
	return 1;
}



int		gameInit(int rebuild)
{
	int i;

	if (rebuild) {
		GLint nbits[3];

		// Cerca il numero di bit per colore disponibile
		glGetIntegerv (GL_RED_BITS, nbits);
		glGetIntegerv (GL_GREEN_BITS, nbits+1);
		glGetIntegerv (GL_BLUE_BITS, nbits+2);
		if ((1 << (nbits[0] + nbits[1] + nbits[2])) < map.nPlaces) {
		    MessageBox(NULL, "Impossible to build colormap (bitperpixel insufficient", 
					   "Error", MB_OK);
		    return 0;
		}


		if (!buildStructure())
			return 0;
	}

	setMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef (0, 0, Z_VIEW);

	gamePaused = game = 1;
	ended = youWin = youLose = youWinRecord = 0;

	for (i = 0; i < 16; i++)
		rot[i] = 0;
	rot[0] = rot[5] = rot[10] = rot[15] = 1;

	return 1;
}


void	gameClose(int destroy)
{
	if (destroy)
		freeMap();
	game = 0;
}


void	oglClose ()
{
	freeTextures();
}








void	timerSet (int t)
{
	if (!ended)
		time = t;
}







void	changeWindowSize(int width, int height)
{
	screenViewport[2] = width;
	screenViewport[3] = height;

    glViewport(0, 0, (GLsizei) width, (GLsizei) height);

	setMatrix();
}










void	pause()
{
	gamePaused = 1;
}


void	unpause()
{
	gamePaused = 0;
}





void	updateDisplay()
{
	int i, j, p, idx, m;
	char string[10];
	char cross[2];

	if (youLose)
	    glClearColor (0.6f, 0, 0, 0);
	else if (youWin)
		glClearColor (0.0f, 0.4f, 0.0f, 0);
	else 
		glClearColor (0.1f, 0.2f, 0.3f, 0);
		
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!game) {
	    glFlush();
		return;
	}

	/* ----------------- 3D mode ------------------ */
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	
	glColor3f (0,0,0);				// per linee

	// WIREFRAME
	p = 0;
	if (mapPoly.nTri > 0) 
		for (i = 0; i < mapPoly.nTri; i++) {
			glBegin (GL_LINE_STRIP);
			glVertex3fv (mapPoly.triV + p);
			glVertex3fv (mapPoly.triV + p + 3);
			glVertex3fv (mapPoly.triV + p + 6);
			glEnd();
			p += 9;
		}
	
	// TEXTURING
	if (mapPoly.nTri > 0) {
		p = 0;
		for (i = 0; i < mapPoly.nTri; i++) {
			idx = mapPoly.triIdx[i];
			if (map.place[idx] & MAP_PLACE_FALSEFLAG)
				i = i;
			if (map.place[idx] & MAP_PLACE_COVERED && !(map.place[idx] & MAP_PLACE_FLAG)) {
				glDisable (GL_TEXTURE_2D);
				glColor3f (0.5, 0.5, 0.5);
				glBegin (GL_TRIANGLES);
				glVertex3fv (mapPoly.triV + p);
				glVertex3fv (mapPoly.triV + p + 3);
				glVertex3fv (mapPoly.triV + p + 6);
				glEnd();
			}
			else if (map.place[idx] == 0)
			{
				glDisable (GL_TEXTURE_2D);
				glColor3f (0.75, 0.75, 0.75);
				glBegin (GL_TRIANGLES);
				glVertex3fv (mapPoly.triV + p);
				glVertex3fv (mapPoly.triV + p + 3);
				glVertex3fv (mapPoly.triV + p + 6);
				glEnd();
			}
			else {
				glEnable (GL_TEXTURE_2D);
				m = map.place[idx];
				if (m & MAP_PLACE_FALSEFLAG)
					m = TEX_FLAGCROSSED;
				else if (m & MAP_PLACE_FLAG)
					m = TEX_FLAG;
				else if (m & MAP_PLACE_MINE)
					m = TEX_MINE;
				else
					m = TEX_NUMBASE + (m & MAP_PLACE_NUMMASK);
				glBindTexture(GL_TEXTURE_2D, texName[m]);
				glBegin (GL_TRIANGLES);
				glTexCoord2f (texVertex[0], texVertex[1]);
				glVertex3fv (mapPoly.triV + p);
				glTexCoord2f (texVertex[2], texVertex[3]);
				glVertex3fv (mapPoly.triV + p + 3);
				glTexCoord2f (texVertex[4], texVertex[5]);
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
	itoa ((time < 99990) ? time / 10 : 9999, string, 10);
	glRasterPos2i(screenViewport[2] - 10 * (strlen (string)+1), 10);
	printString(string);

	itoa (vars.hallsOfFame[map.typeIndex].time, string, 10);
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



