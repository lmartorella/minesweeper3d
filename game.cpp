

// WINDOWS SPECIFIC & OPENGL!!
#include "stdafx.h"

#include "library.h"
#include "game.h"
#include "modules.h"
#include "strings.h"
#include "vars.h"




/* Viewport window */
static 	GLint screenViewport [4];

/* Texture Object */
#define TEXNUM 15
#define TEX_FLAG 1
#define TEX_FLAGCROSSED 14
#define TEX_MINE 0
#define TEX_NUMBASE 1
static GLuint * texName = NULL;
static GLfloat * texVertex = NULL;

/* MAPPA */
extern	MINESWEEPER_MAP	   	map;

/* Mouse buttons */
static  int mButtons = 0;

/* GAME */
static unsigned int timer = 0;
static int gameStatus = GAME_STATUS_STOPPED;
static int crosshair = 1;				// per crosshair

/* ALTRE VARIABILI, INI e CFG */
extern  GLOBAL_VARS vars;
extern  RECORD * recordArray;			// relativo a mappa selezionata
extern	INI_VARS ini;



/* FUNCTIONS DLL, MODULE MAP */
int						mapIndex;
BUILDMAP_TYPE			p_BuildMap		= NULL;
DESTROYMAP_TYPE			p_DestroyMap	= NULL;
MOUSEMOVE_TYPE			p_MouseMove		= NULL;
SETCAMERAPARAMS_TYPE	p_SetCameraParams = NULL;
RESETMAP_TYPE			p_ResetMap		= NULL;





inline	float rnd()
{ return float(rand()) / RAND_MAX; }







// ------------------------------------------------------- TEXTURES
// ------------------------------------------------------- TEXTURES
// ------------------------------------------------------- TEXTURES
// ------------------------------------------------------- TEXTURES
// ------------------------------------------------------- TEXTURES


static DWORD	buildTextures()
{
	int width, height;

	FILE * file;

	texName = new GLuint [TEXNUM];
    
	// Legge immagine
	width = TEXNUM * (ini.singleTexWidth + ini.interleaveX);
	height = ini.singleTexHeight;

	BYTE * buffer = new BYTE [width * height * 3];
	file = fopen (ini.texFileName, "rb");
	if (file == NULL) 
		return IDS_GAME_TEXFILENOTFOUND;

	if (fread (buffer, width * height * 3, 1, file) != 1) 
		return IDS_GAME_TEXFILEERROR;

	fclose (file);


	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, width);

    glGenTextures(TEXNUM, texName);

	for (int i = 0; i < TEXNUM; i++) {
		glBindTexture(GL_TEXTURE_2D, texName[i]);
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
					  ini.singleTexWidth,
					  ini.singleTexHeight,
					  0,
					  GL_RGB,
					  GL_UNSIGNED_BYTE,
		  			  buffer + i * 3 * (ini.singleTexWidth + ini.interleaveX));
	}
	delete buffer;
	return 0;			
}


static void	freeTextures()
{
	delete texName;
	texName = NULL;
}


DWORD rebuildTextures()
{
	freeTextures();
	return buildTextures();
}

















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





















































class	Point3D {
public:
	GLfloat		v[3];
	
	Point3D () {}
	Point3D (GLfloat xx, GLfloat yy, GLfloat zz) 
		 { v[0] = xx, v[1] = yy, v[2] = zz;}
	Point3D (GLfloat * p)
		 { memcpy (v, p, sizeof (GLfloat)*3); }
	
	Point3D (const MINESWEEPER_VERTEX& a)
		 { v[0] = a.x, v[1] = a.y, v[2] = a.z;}

	Point3D operator + (const Point3D& a) const
		{ return Point3D (v[0]+a.v[0], v[1]+a.v[1], v[2]+a.v[2]); }
	Point3D operator - (const Point3D& a) const
		{ return Point3D (v[0]-a.v[0], v[1]-a.v[1], v[2]-a.v[2]); }

	Point3D	 operator * (GLfloat f) const
		{ return Point3D (v[0]*f, v[1]*f, v[2]*f); }

	Point3D	 operator * (const Point3D& a) const
		{ return Point3D (v[1]*a.v[2]-v[2]*a.v[1], v[2]*a.v[0]-v[0]*a.v[2], 
		                  v[0]*a.v[1]-v[1]*a.v[0]); }

	Point3D& operator += (const Point3D& a)
		{ *this = *this + a; 
		  return *this;}

	Point3D& operator *= (GLfloat a)
		{ *this = *this * a; 
		  return *this;}


	float	Module () const
		{  return float(sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2])); }

	void	Normalize (GLfloat n)
		{  *this *= n / Module();  }
};

typedef Point3D Vector3D;






class	MapPoly {
public:
		// Numero di poligoni (facce)
		int		n;

		// Array [0..nPoly-1], contiene il numero di lati dei poligoni enumerati
		int		* edges;			
		
		// Array [0..n-1] contenente gli array di vertici [x,y,z] di tutti i tasselli
		Point3D	** modVert;

		// Array [0..n-1] contenente *in ordine* tutti i vertici [x,y,z] originali
		Point3D	** origVert;

		// Array [0..] contenente *in ordine* tutte le normali ai bordi dei tasselli [x,y,z]
		Point3D	** bordNormal;

		// Array [0..nPoly-1] contenente le normali ai tasselli alta quanto un bottone
		Point3D	*  faceNormal;

		// Altezza di ogni "tasto" [-1..0]
		GLfloat	*	hKey;

	MapPoly (MINESWEEPER_MAP * map, DWORD * error);
	~MapPoly ();
};

MapPoly * mapPoly = NULL;







MapPoly::MapPoly (MINESWEEPER_MAP * map, DWORD * err)
{
	*err = 0;

	// Crea la MAP_POLY
	n = map->nPlaces;

	// Crea buffers
	if (n > 0) {
		edges = new int [n];
		modVert = new Point3D * [n];
		origVert = new Point3D * [n];
		bordNormal = new Point3D * [n];
		faceNormal = new Point3D [n];
		hKey = new float [n];
		memset (hKey, 0, sizeof (GLfloat) * n);
	}
   
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < MAX_VERTEX_FACE; j++)
			if (map->face->v[j] == -1) 
				break;
		
		if (j >= MAX_VERTEX_FACE || j <= 2) {
			*err = IDS_GAME_MAPDEFERR;
			return;
		}
		
		// Nuovo poligono
		edges [i] = j;
			
		// Copia poligono, modificando l'altezza del tasto.
		// Calcola il punto medio dei vertici
		Point3D center(0,0,0);
		for (int k = 0; k < edges[i]; k++) 
			center += map->vertex[map->face[i].v[k]];
		center *= 1.0f / edges[i];

		// Calcola normale
		int v0 = map->face[i].v[0];
		int v1 = map->face[i].v[1];
		int v2 = map->face[i].v[2];
		Vector3D normal = (Point3D(map->vertex[v0]) - Point3D(map->vertex[v1])) * 
						  (Point3D(map->vertex[v2]) - Point3D(map->vertex[v1]));

		float edgeSize = (Point3D(map->vertex[v0]) - Point3D(map->vertex[v1])).Module();
		normal.Normalize (0.1f * edgeSize);
		faceNormal[i] = normal;

		modVert[i] = new Point3D [edges[i]];
		origVert[i] = new Point3D [edges[i]];
		bordNormal[i] = new Point3D [edges[i]];

		for (k = 0; k < edges[i]; k++) {
			Point3D v = map->vertex[map->face[i].v[k]];
			origVert[i][k] = v;

			Vector3D tmp = center - v;
			tmp.Normalize(0.1f * edgeSize);
			v += tmp + normal;
			modVert[i][k] = v;
		}
	}	
}



MapPoly::~MapPoly()
{
	for (int i=0; i<n; i++)
		delete[] origVert[i];
	delete[] origVert;

	for (i=0; i<n; i++)
		delete[] modVert[i];
	delete[] modVert;

	delete[] edges;
	delete[] faceNormal;
	delete[] hKey;

	for (i=0; i<n; i++)
		delete[] bordNormal[i];
	delete[] bordNormal;
}















static void	prepareMap ()
{
	int i, j;

	map.nMines = map.initialMines;

	// Mette mine
	for (i = 0; i < map.nPlaces; i++)
		map.place[i] = MAP_PLACE_COVERED;

	for (i = 0; i < map.initialMines; i++) {
		do {
			j = (int)(((double)(rand()) / RAND_MAX) * map.nPlaces);
		} while (j < 0 || j >= map.nPlaces || map.place[j] & MAP_PLACE_MINE);
		map.place[j] |= MAP_PLACE_MINE;
	}

	// Ora setta i numeri
	placeNumbers();
}		
















// ------------------------------------------------------- SELECT
// ------------------------------------------------------- SELECT
// ------------------------------------------------------- SELECT
// ------------------------------------------------------- SELECT
#define MAX_HITBUFSIZE 512


DWORD selectFace (int * face)
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
                    ini.precision, ini.precision, screenViewport);

	p_SetCameraParams (mapIndex, screenViewport[2], screenViewport[3]); 

	
	for (i = 0; i < mapPoly->n; i++) {
		glLoadName (i);
		glBegin (GL_POLYGON);
		for (j = 0; j < mapPoly->edges[i]; j++) 
			glVertex3fv (mapPoly->modVert[i][j].v);
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































































































// ------------------------------------------------------- GAME
// ------------------------------------------------------- GAME
// ------------------------------------------------------- GAME
// ------------------------------------------------------- GAME




static void	totalSweeper(int uncover, MINESWEEPER_MAP * map)
{
	int i;
	for (i = 0; i < map->nPlaces; i++) {
		if (map->place[i] & MAP_PLACE_FLAG && !(map->place[i] & MAP_PLACE_MINE))
			map->place[i] |= MAP_PLACE_FALSEFLAG;
		if (map->place[i] & MAP_PLACE_MINE && !(map->place[i] & MAP_PLACE_FLAG))
			if (uncover)
				map->place[i] &= ~(MAP_PLACE_COVERED);
			else
				map->place[i] |= MAP_PLACE_FLAG;
	}
}



// Ritorna 1 se il campo è finito!
static int		secureSweeper (int idx, MINESWEEPER_MAP * map)
{
	int i = -1, neigh;
	int end = 0;

	map->place[idx] &= (~MAP_PLACE_COVERED);
	map->nChecked++;

	if ((map->place[idx] & MAP_PLACE_NUMMASK) != 0)
		return (map->nChecked + map->initialMines == map->nPlaces);

	while (i++, ((neigh = map->neighbour[idx].n[i]) != -1) && i < MAX_NEIGHBOURS)
		if (map->place[neigh] & MAP_PLACE_COVERED && !(map->place[neigh] & MAP_PLACE_FLAG)) 
			if (secureSweeper (neigh, map))
				end = 1;

	return end || (map->nChecked + map->initialMines == map->nPlaces);
}




static int		MouseButton		   (int msg, 
									int * buttons, 
									int * gameStatus,
									MINESWEEPER_MAP * map)
{
	int hit, k, n, err, flag, a, ended;

	ended = *gameStatus == GAME_STATUS_ENDED_LOST || *gameStatus == GAME_STATUS_ENDED_WIN;

	switch (msg) {
	case WM_LBUTTONDOWN:
		*buttons |= MINE_LBUTTON;

		if (ended)
			return 0;
		selectFace (&hit);
		if (hit == -1)
			return 0;

		mapPoly->hKey[hit] = -1.0f;
		return 1;

	case WM_RBUTTONDOWN:
		*buttons |= MINE_RBUTTON;
		if (ended)
			return 0;

		selectFace (&hit);

		if (hit == -1)
			return 0;
		if (!(map->place[hit] & MAP_PLACE_COVERED))
			return 0;

		map->place[hit] ^= MAP_PLACE_FLAG;
		if (map->place[hit] & MAP_PLACE_FLAG)
			map->nMines--;
		else
			map->nMines++;
		return 1;

	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		
		if (ended) {
			*buttons |= MINE_UNLOCK;
			return 0;
		}

		selectFace (&hit);

		if (hit == -1)
			return 0;

		if (msg == WM_LBUTTONUP && *buttons == MINE_LBUTTON) {
			mapPoly->hKey[hit] = 0;
			*buttons = 0;
			if ((!(map->place[hit] & MAP_PLACE_COVERED)) ||
				map->place[hit] & MAP_PLACE_FLAG) 
				return 1;

			if (map->place[hit] & MAP_PLACE_MINE) {
				totalSweeper(1, map);
				*gameStatus = GAME_STATUS_ENDED_LOST;
				return 1;		
			}
			if (secureSweeper (hit, map)) {
				*gameStatus = GAME_STATUS_ENDED_WIN;
				map->nMines = 0;
				totalSweeper(0, map);
			}
			return 1;
		}
		else if (msg == WM_RBUTTONUP && !(*buttons & MINE_LBUTTON)) {
			*buttons &= ~MINE_RBUTTON;
			return 0;
		}
		else if (*buttons == (MINE_LBUTTON | MINE_RBUTTON)) {
			// Proc. doppio tasto
			*buttons = 0;
			n = map->place[hit] & MAP_PLACE_NUMMASK;
			if (map->place[hit] & MAP_PLACE_COVERED || n == 0)
				return 0;
			
			err = flag = 0;
			for (k = 0; k < MAX_NEIGHBOURS && map->neighbour[hit].n[k] != -1; k++) {
				a = map->place[map->neighbour[hit].n[k]] & (MAP_PLACE_MINE | MAP_PLACE_FLAG);
				if (a != 0 && a != (MAP_PLACE_MINE | MAP_PLACE_FLAG))
					err++;
				if (a & MAP_PLACE_FLAG)
					flag++;
			}
			if (err == 0) {
				k = -1;
				while (k++, ((n = map->neighbour[hit].n[k]) != -1) && k < MAX_NEIGHBOURS)
					if (map->place[n] & MAP_PLACE_COVERED && !(map->place[n] & MAP_PLACE_FLAG)) 
						if (secureSweeper (n, map))
							ended = 1;

				if (ended) {
					*gameStatus = GAME_STATUS_ENDED_WIN;
					map->nMines = 0;
					totalSweeper(0, map);
				}
				return 1;
			}
			else if (flag != n)
				return 0;
			// Esplode
			totalSweeper(1, map);
			*gameStatus = GAME_STATUS_ENDED_LOST;
			return 1;
		}

	default:
		return 0;
	}
}






int		processMouseButton (UINT msg, int x, int y, int * unlock, int * status)
{
	int redraw;
	*status = gameStatus;

	if (gameStatus == GAME_STATUS_STOPPED) {
		*unlock = 1;
		return 0;
	}

	mButtons &= ~MINE_UNLOCK;
	redraw = MouseButton (msg, &mButtons, &gameStatus, &map);
	*unlock = (mButtons & MINE_UNLOCK) ? 1 : 0;

	if (gameStatus == GAME_STATUS_ENDED_WIN) 
		if (timer / 100 < recordArray->record)
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
		delete texVertex;
	texVertex = new GLfloat [MAX_VERTEX_FACE * 2 * (MAX_VERTEX_FACE - 2)];
	for (i = 3; i < MAX_VERTEX_FACE; i++) {
		incAngle = 3.141592 * 2 / i;
		angle = 3.141592 * 3 / 2 - incAngle;
		for (j = 0; j < i; j++, angle += incAngle) {
			u = GLfloat (cos (angle) / 2 + 0.5);
			v = GLfloat (sin (angle) / 2 + 0.5);
			texVertex [((i-3) * MAX_VERTEX_FACE * 2) + j * 2] = u;
			texVertex [((i-3) * MAX_VERTEX_FACE * 2) + j * 2 + 1] = v;
		}
	}

	gameStatus = GAME_STATUS_STOPPED;
	crosshair = 0;
	return 0;
}



unsigned int *	getTimer ()
{
	return & timer;
}



void	resetGame()
{
	crosshair = 1;
	gameStatus = GAME_STATUS_NORMAL;
	timer = 0;
}



void	gameClose()
{
	gameStatus = GAME_STATUS_STOPPED;
}


void	mine_exit ()
{
	freeTextures();
	delete texVertex;
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
	crosshair = 0;
}


void	showCrosshair()
{
	crosshair = 1;
}




DWORD	newGame(DWORD code)
{
	int changed;
	DWORD err;

	changed = map.code != code;
	gameClose();

	if (changed) {
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

	prepareMap ();						// mette le mine
	
	if (changed) {

		glMatrixMode (GL_MODELVIEW);
		glLoadIdentity();

		GLfloat position[] = {0.5f,0.2f,1.5f,0};
		glLightfv (GL_LIGHT0, GL_POSITION, position);

		glShadeModel (GL_FLAT);
		glMaterialf (GL_FRONT, GL_SHININESS, 30.0);

		DWORD err;
		if (mapPoly)
			delete mapPoly;
		mapPoly = new MapPoly (&map, &err);
		
		p_DestroyMap (mapIndex, &map);
		return err;
	}

	return 0;
}







int		GetTextPlace (int place, GLuint * name, GLfloat * diffuse)
{
	if ((place & (MAP_PLACE_COVERED | MAP_PLACE_FLAG)) == MAP_PLACE_COVERED) {
		diffuse[0] = diffuse[1] = diffuse[2] =  0.3f;
		return 0;
	}
	diffuse[0] = diffuse[1] = diffuse[2] =  0.9f;
	if (place == 0) 
		return 0;
	else if (place & MAP_PLACE_FALSEFLAG)
		*name = texName[TEX_FLAGCROSSED];
	else if (place & MAP_PLACE_FLAG)
		*name = texName[TEX_FLAG];
	else if (place & MAP_PLACE_MINE)
		*name = texName[TEX_MINE];
	else
		*name = texName[TEX_NUMBASE + (place & MAP_PLACE_NUMMASK)];
	return 1;
}



DWORD	renderer ()
{

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
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable (GL_LIGHT0);
	glEnable (GL_LIGHTING);

	// ****************************** MODEL
	GLfloat face_specular[] = {179.0/2560, 227.0/2560, 0.1f, 1};
	GLfloat border_specular[] = {179.0/256, 227.0/256, 1.0f, 1};

	GLfloat face_diffuse[4];
	GLfloat border_diffuse[] = {0.4f, 0.4f, 0.4f, 0.4f};


	Point3D vtemp [MAX_VERTEX_FACE];


	for (int i = 0; i < mapPoly->n; i++) {
		int m = map.place[i];
		int n = mapPoly->edges[i];

		GLuint name;
		int isTexture = GetTextPlace (m, &name, face_diffuse);

		// Materiale faccia
		glMaterialfv (GL_FRONT, GL_SPECULAR, face_specular);
		glMaterialfv (GL_FRONT, GL_DIFFUSE, face_diffuse);

		// Tasto
		for (int j=0; j<n; j++)
			vtemp [j] = mapPoly->modVert[i][j] + 
					    (mapPoly->faceNormal[i] * mapPoly->hKey[i]);

		Point3D tn = mapPoly->faceNormal[i];
		tn.Normalize (1.0f);
		glNormal3fv (tn.v);

		if (!isTexture) {
			glDisable (GL_TEXTURE_2D);

			glBegin (GL_POLYGON);
			for (int j = 0; j < n; j++)
				glVertex3fv (vtemp[j].v);
			glEnd();
		}
		else {
			glEnable (GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, name);

			glBegin (GL_POLYGON);
			int k = MAX_VERTEX_FACE * 2 * (n - 3);
			for (int j = 0; j < n; j++, k+=2) {
				glTexCoord2f (texVertex[k], texVertex[k + 1]);
				glVertex3fv (vtemp[j].v);
			}
			glEnd();
		}

		// Normale ai bordi
		for (int k = 0; k < mapPoly->edges[i]; k++) {
			int k1 = (k+1) % (mapPoly->edges[i]);

			Vector3D normal = (vtemp[k1] - vtemp[k]) *
				              (mapPoly->origVert[i][k] - vtemp[k]);			

			normal.Normalize(1.0);
			mapPoly->bordNormal[i][k] = normal;
		}

		glDisable (GL_TEXTURE_2D);
		glColor3f (0.2f, 0.2f, 0.2f);
		glMaterialfv (GL_FRONT, GL_SPECULAR, border_specular);
		glMaterialfv (GL_FRONT, GL_DIFFUSE, border_diffuse);

		for (j = 0; j < n; j++) {
			glBegin (GL_POLYGON);
			glNormal3fv (mapPoly->bordNormal[i][j].v);
			glVertex3fv (mapPoly->origVert[i][j].v);
			glVertex3fv (vtemp[j].v);
			int j1 = (j+1)%n;
			glVertex3fv (vtemp[j1].v);
			glVertex3fv (mapPoly->origVert[i][j1].v);
			glEnd ();
		}
	}

	/* ----------------- 2D mode ------------------ */
	glDisable (GL_TEXTURE_2D);
	glDisable (GL_LIGHTING);
	
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();					// salva la 3d proj.
    glLoadIdentity();
    gluOrtho2D (0, screenViewport[2], 0, screenViewport[3]);

	glColor3f(1,1,0);

	// Mine rimanenti
	char string[10];
	glRasterPos2i(10, 10);
	itoa (map.nMines, string, 10);
	printString(string);

	// Tempo e record
	itoa ((timer < 999900) ? timer / 100 : 9999, string, 10);
	glRasterPos2i(screenViewport[2] - 10 * (strlen (string)+1), 10);
	printString(string);

	itoa ((recordArray->record < 9999) ? recordArray->record : 9999, string, 10);
	glRasterPos2i(screenViewport[2] - 10 * (strlen (string)+1), 23);
	printString(string);

	// Puntatore
	if (crosshair) {
		char cross[2];
		cross[0] = CROSSHAIR;
		cross[1] = 0;
		glRasterPos2i (screenViewport[2] / 2 - 4, screenViewport[3] / 2 - 4);
		printString (cross);
	}

    glPopMatrix();					// ricaric. la 3d proj.
    glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

    glFlush();
	return 0;
}




