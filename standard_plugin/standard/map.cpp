

#include "stdafx.h"

#define THIS_IS_A_MODULE
#include "..\..\library.h"



/* OPENGL */
#define Z_VIEW -3.6f
#define Z_MIN  0.1f
#define Z_MAX  10.0f
#define FOV    32.0f		

#define X_SENS 2.25e-03f
#define Y_SENS 2.25e-03f


GLfloat	rot[16];



void	GetMineInfo (int * versione, struct MINE_MODULE_INFO * m)
{
	strcpy (m->name, "minesw/original/standard");
	*versione = 0x100;
}



void	GetMapCount (int * count)
{
	*count = 3;			
}


static char  names[3][32] = {   "2-Icosahedron (80)", 
							    "3-Icosahedron (320)", 
								"4-Icosahedron (1280)" };


void	GetMapName (int number, char * name)
{
	strcpy (name, names[number]);
}




#define X .525731112119133606f
#define Z .850650808352039932f


/*  numVert80 = numVert20 + numLati20 = 12 + 30 = 42 */
/*  numLati80 = numLati20 * 2 + numFacce20 * 3
			  = 30 * 2 + 20 * 3 = 60 + 60 = 120 */

/*  numVert320 = numVert80 + numLati80 = 42 + 120 = 162 */
/*  numLati320 = numLati80 * 2 + numFacce80 * 3
			   = 120 * 2 + 80 * 3 = 240 + 240 = 480 */


/*   12-edro     12 vert,  20 facce,  30 lati */
/*   80-edro	 42 vert,  80 facce, 120 lati */
/*  320-edro	162 vert, 320 facce, 480 lati  */


/* array dei vertici */
static float vdata[12][3] = {   
   {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},   
   {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},   
   {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
};

/* array dei triangoli-vertici */
static int tindices[20][3] = {
   {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},   
   {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},   
   {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
   {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} 
};

/* array dei lati */
static int lindex[30][3] = {
	{0,1,-1}, {0,4,-1}, {0,6,-1}, {0,9,-1}, {0,11,-1},
	{1,4,-1}, {1,6,-1}, {1,8,-1}, {1,10,-1}, {2,3,-1},
	{2,5,-1}, {2,7,-1}, {2,9,-1}, {2,11,-1}, {3,5,-1},
	{3,7,-1}, {3,8,-1}, {3,10,-1}, {4,5,-1}, {4,8,-1},
	{4,9,-1}, {5,8,-1}, {5,9,-1}, {6,7,-1}, {6,10,-1},
	{6,11,-1}, {7,10,-1}, {7,11,-1}, {8,10,-1}, {9,11,-1},
};




static float *vdataAct;
static int *tindicesAct;
static int *lindexAct;
static int propAct[3];

static float *vdataNew;
static int *tindicesNew;
static int *lindexNew;
static int propNew[3];



/******** Creazione/Distruzione Array ************/
void buildArrayAct (int numVertex, int numFaces, int numSides) {
	propAct[0] = numVertex;
	propAct[1] = numFaces;
	propAct[2] = numSides;

	vdataAct = (float*)malloc(sizeof(float) * 3 * propAct[0]);
	tindicesAct = (int*)malloc(sizeof(int) * 3 * propAct[1]);
	lindexAct = (int*)malloc (sizeof(int) * 3 * propAct[2]);
}

void deleteArrayAct () {
	free (vdataAct);
	free (tindicesAct);
	free (lindexAct);
}

void buildArrayNew () {
	/* numVerticiNew = numVerticiAct + numLatiAct */
	propNew[0] = propAct[0] + propAct[2];
	/* numFacceNew = numFacceAct * 4 */
	propNew[1] = propAct[1] * 4;
	/* numLatiNew = numLatiAct * 4 */
	propNew[2] = propAct[2] * 4;

	vdataNew = (float*)malloc(sizeof(float) * 3 * propNew[0]);
	tindicesNew = (int*)malloc(sizeof(int) * 3 * propNew[1]);
	lindexNew = (int*)malloc (sizeof(int) * 3 * propNew[2]);
}

void deleteArrayNew () {
	free (vdataNew);
	free (tindicesNew);
	free (lindexNew);
}

void initialize() {
	int i;

	buildArrayAct (12, 20, 30);

	for (i=0; i<12; i++) {
		vdataAct[i*3 + 0] = vdata[i][0];
		vdataAct[i*3 + 1] = vdata[i][1];
		vdataAct[i*3 + 2] = vdata[i][2];
	}
	for (i=0; i<20; i++) {
		tindicesAct[i*3 + 0] = tindices[i][0];
		tindicesAct[i*3 + 1] = tindices[i][1];
		tindicesAct[i*3 + 2] = tindices[i][2];
	}
	for (i=0; i<30; i++) {
		lindexAct[i*3 + 0] = lindex[i][0];
		lindexAct[i*3 + 1] = lindex[i][1];
		lindexAct[i*3 + 2] = lindex[i][2];
	}
}

void aggiornaMatrix() {
	int i;
	/* creo la nuova matrice Actual */ 
	deleteArrayAct();
	buildArrayAct(propNew[0], propNew[1], propNew[2]);
	for (i=0; i<propAct[0]; i++) {
		vdataAct[i*3 + 0] = vdataNew[i*3 + 0];
		vdataAct[i*3 + 1] = vdataNew[i*3 + 1];
		vdataAct[i*3 + 2] = vdataNew[i*3 + 2];
	}
	for (i=0; i<propAct[1]; i++) {
		tindicesAct[i*3 + 0] = tindicesNew[i*3 + 0];
		tindicesAct[i*3 + 1] = tindicesNew[i*3 + 1];
		tindicesAct[i*3 + 2] = tindicesNew[i*3 + 2];
	}
	for (i=0; i<propAct[2]; i++) {
		lindexAct[i*3 + 0] = lindexNew[i*3 + 0];
		lindexAct[i*3 + 1] = lindexNew[i*3 + 1];
		lindexAct[i*3 + 2] = lindexNew[i*3 + 2];
	}
	/* distruggo la matrice New */
	deleteArrayNew();
}
/******** Fine Creazione/Distruzione Array ************/



/********** Creazione Nuovi Triangoli *************/
void normalize(float v[3]) {   
	GLfloat d = (GLfloat)sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
	if (d == 0.0) {
		exit(1);
	}
	v[0] /= d; v[1] /= d; v[2] /= d;
}

void subdivide(int index, int *lastVertex, int *lastTriangle, int *lastSide)
{
	float vA[3][3];		/* [indici dei vertici][coordinate x,y,z] */
						/* vA[0][..] = v0 */
						/* vA[1][..] = v1 */
						/* vA[2][..] = v2 */

	float vAB[3][3];	/* [indici dei vertici][coordinate x,y,z] */
						/* vAB[0][..] = v0 -> v1 */
						/* vAB[1][..] = v1 -> v2 */
						/* vAB[2][..] = v2 -> v0 */
	int newVertex[3];
	int i, j, ind1, ind2;

	/* carico in vA[..][..] i vertici del triangolo "index" */
	for (i = 0; i < 3; i++) {
		ind1 = index * 3;

		/* vA[x][i] = vdata[tindicesAct[ind][x]][i]
		/*          = vdata[tindices[ind*3 + x]][i]
		/*          = vdata[tindices[ind*3 + x]*3 + i]

		/* ind2 = [tindicesAct[index][0]]  [i] */
		ind2 = tindicesAct[ind1 + 0] * 3 + i;
		vA[0][i] = vdataAct[ind2];
		/* ind2 = [tindicesAct[index][1]]  [i] */
		ind2 = tindicesAct[ind1 + 1] * 3 + i;
		vA[1][i] = vdataAct[ind2];
		/* ind2 = [tindicesAct[index][2]]  [i] */
		ind2 = tindicesAct[ind1 + 2] * 3 + i;
		vA[2][i] = vdataAct[ind2];
	}

	
	/* calcola i vertici intermedi */
	ind1 = index * 3;
	for (j=0; j<3; j++) {
		/* vertice tra v(j) e v(j+1) alias vAB[j]*/ 
		for (i=0; i<propAct[2]; i++) {
			/* trova il lato corrispondente ai 2 vertici */
			ind2 = i * 3;
			if ((tindicesAct[ind1 + j] == lindexAct[ind2 + 0] && tindicesAct[ind1 + ((j+1)%3)] == lindexAct[ind2 + 1]) 
				|| (tindicesAct[ind1 + j] == lindexAct[ind2 + 1] && tindicesAct[ind1 + ((j+1)%3)] == lindexAct[ind2 + 0])) {
				/* trovato il lato */
				/* controlla se esiste già un vertice intermedio */
				if (lindexAct[ind2 + 2] == -1) {
					/* non esiste vertice intermedio */
					/* ne crea uno nuovo e lo inserisce nell'array vdataNew */
					vAB[j][0] = vA[j][0] + vA[(j+1)%3][0];
					vAB[j][1] = vA[j][1] + vA[(j+1)%3][1];
					vAB[j][2] = vA[j][2] + vA[(j+1)%3][2];
					normalize(vAB[j]);

					lindexAct[ind2 + 2] = *lastVertex;
					vdataNew[lindexAct[ind2 + 2] * 3 + 0] = vAB[j][0];
					vdataNew[lindexAct[ind2 + 2] * 3 + 1] = vAB[j][1];
					vdataNew[lindexAct[ind2 + 2] * 3 + 2] = vAB[j][2];
					(*lastVertex)++;
					
					/* creo i 2 nuovi lati derivati dal nuovo vertice */
					lindexNew[((*lastSide) * 3) + 0] = lindexAct[ind2 + 0];
					lindexNew[((*lastSide) * 3) + 1] = lindexAct[ind2 + 2];
					lindexNew[((*lastSide) * 3) + 2] = -1;
					(*lastSide) = (*lastSide) + 1;
					lindexNew[((*lastSide) * 3) + 0] = lindexAct[ind2 + 1];
					lindexNew[((*lastSide) * 3) + 1] = lindexAct[ind2 + 2];
					lindexNew[((*lastSide) * 3) + 2] = -1;
					(*lastSide) = (*lastSide) + 1;
				}
				else {
					/* esiste vertice intermedio */
					/* assegna a vAB[0] il valore del vertice */
					vAB[j][0] = vdataNew[lindexAct[ind2 + 2] * 3 + 0];
					vAB[j][1] = vdataNew[lindexAct[ind2 + 2] * 3 + 1];
					vAB[j][2] = vdataNew[lindexAct[ind2 + 2] * 3 + 2];
				}
				newVertex[j] = lindexAct[ind2 + 2];
				break;
				/* prepearato vertice vAB[j] */
			}
		}
		/* fine creazione vertici vAB[][] */
	}



	/* creo i quattro nuovi triangoli e li inserisco in tindicesNew */
	/* 1^ triangolo - Parallelo a index, mantiene l'indice */
	tindicesNew[index * 3 + 0] = newVertex[0];
	tindicesNew[index * 3 + 1] = newVertex[1];
	tindicesNew[index * 3 + 2] = newVertex[2];
	/* 2^ triangolo */
	tindicesNew[(*lastTriangle) * 3 + 0] = tindicesAct[index * 3 + 0];	/* v[0] */
	tindicesNew[(*lastTriangle) * 3 + 1] = newVertex[0]; /* vAB[0] */
	tindicesNew[(*lastTriangle) * 3 + 2] = newVertex[2]; /* vAB[2] */
	(*lastTriangle) = (*lastTriangle) + 1;
	/* 3^ triangolo */
	tindicesNew[(*lastTriangle) * 3 + 0] = tindicesAct[index * 3 + 1];	/* v[1] */
	tindicesNew[(*lastTriangle) * 3 + 1] = newVertex[1]; /* vAB[1] */
	tindicesNew[(*lastTriangle) * 3 + 2] = newVertex[0]; /* vAB[0] */
	(*lastTriangle) = (*lastTriangle) + 1;
	/* 4^ triangolo */
	tindicesNew[(*lastTriangle) * 3 + 0] = tindicesAct[index * 3 + 2];	/* v[2] */
	tindicesNew[(*lastTriangle) * 3 + 1] = newVertex[2]; /* vAB[2] */
	tindicesNew[(*lastTriangle) * 3 + 2] = newVertex[1]; /* vAB[1] */
	(*lastTriangle) = (*lastTriangle) + 1;


	/* creo gli ultimi 3 lati e li inserisco in lindexNew */
	lindexNew[((*lastSide) * 3) + 0] = newVertex[0];
	lindexNew[((*lastSide) * 3) + 1] = newVertex[1];
	lindexNew[((*lastSide) * 3) + 2] = -1;
	(*lastSide) = (*lastSide) + 1;
	lindexNew[((*lastSide) * 3) + 0] = newVertex[1];
	lindexNew[((*lastSide) * 3) + 1] = newVertex[2];
	lindexNew[((*lastSide) * 3) + 2] = -1;
	(*lastSide) = (*lastSide) + 1;
	lindexNew[((*lastSide) * 3) + 0] = newVertex[2];
	lindexNew[((*lastSide) * 3) + 1] = newVertex[0];
	lindexNew[((*lastSide) * 3) + 2] = -1;
	(*lastSide) = (*lastSide) + 1;
}
/********** Fine Creazione Nuovi Triangoli *************/




static int mines [3] = { 8, 32, 128 };



void	BuildMap (int number, struct MINESWEEPER_MAP * map)
{
	int i, j;

	number++;
		initialize();

		for (j=0; j<number; j++) {
			/* preciclo di chiamata subdivide */
			buildArrayNew();
			for (i=0; i<propAct[0]; i++) {
				vdataNew[i * 3 + 0] = vdataAct[i * 3 + 0];
				vdataNew[i * 3 + 1] = vdataAct[i * 3 + 1];
				vdataNew[i * 3 + 2] = vdataAct[i * 3 + 2];
			}
			/* ciclo di chiamata subdivide */
			propNew[0] = propAct[0];
			propNew[1] = propAct[1];
			propNew[2] = 0;
			for (i = 0; i < propAct[1]; i++) 
				subdivide(i, &propNew[0], &propNew[1], &propNew[2]);

			aggiornaMatrix();
		}
	
		map->nPlaces = propAct[1];
		map->nVertexes = propAct[0];
		map->vertex = (struct MINESWEEPER_VERTEX*) malloc (sizeof (struct MINESWEEPER_VERTEX) * propAct[0]);
		map->face = (struct MINESWEEPER_FACE*) malloc (sizeof (struct MINESWEEPER_FACE) * propAct[1]);

		for (i = 0; i < propAct[0]; i++) {
			map->vertex[i].x = vdataAct[i * 3 + 0];
			map->vertex[i].y = vdataAct[i * 3 + 1];
			map->vertex[i].z = vdataAct[i * 3 + 2];
		}

		for (i = 0; i < propAct[1]; i++) {
			map->face[i].v[0] = tindicesAct[i * 3 + 0];
			map->face[i].v[1] = tindicesAct[i * 3 + 1];
			map->face[i].v[2] = tindicesAct[i * 3 + 2];
			for (j = 3; j < 12; j++)
				map->face[i].v[j] = -1;
		}
		deleteArrayAct();

	map->initialMines = mines[number-1]; 
}




void	DestroyMap (int idx, struct MINESWEEPER_MAP * map)
{
	if (idx != 0) {
		free (map->face);
		free (map->vertex);
	}
	map->face = NULL;
	map->vertex = NULL;
}





// ------------------------------------------------ MOUSE EVENTS
// ------------------------------------------------ MOUSE EVENTS
// ------------------------------------------------ MOUSE EVENTS
// ------------------------------------------------ MOUSE EVENTS



void	mult (GLfloat * m1, GLfloat * m2, GLfloat * r)
{
	int i, j, k;
	
	for (i = 0; i < 4; i++)				// i riga
		for (j = 0; j < 4; j++) {		// j colonna
			r[i + j * 4] = 0;
			for (k = 0; k < 4; k++)
				r[i + j * 4] += m1[i + k * 4] * m2[j * 4 + k];
		}
}




int		MouseMove(int idx, int dx, int dy)
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
	if (idx == 0)
		glTranslatef (-0.5, -0.5, -0.5);

	return 1;
}




void	SetCameraParams (int dummy, int width, int height)
{
	gluPerspective(FOV, (float) width / height, Z_MIN, Z_MAX);
}


void	ResetMap (int i)
{
	memset (rot, 0, 16 * sizeof (GLfloat));
	rot[0] = rot[5] = rot[10] = rot[15] = 1.0;
}


