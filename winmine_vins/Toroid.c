
#include "stdafx.h"

#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <GL/gl.h>			/* OpenGL header file */
#include <GL/glu.h>			/* OpenGL utilities header file */

#define  THIS_IS_A_MODULE
#include "..\library.h"



#define	NUMERO_MAPPE_MODULO	4

static char *nomeMappa[NUMERO_MAPPE_MODULO] = {
	"Toroide 1",
	"Toroide 2",
	"Toroide 3",
	"Toroide 4"
};

static struct proprietaToroide {
	/* raggio dal centro degli assi al centro della sezione */
	float raggioToroide;
	/* raggio della sezione */
	float raggioSezione;

	int numSezioni;
	int numLatiSezione;

	int numVertex;		/* = numSezioni * numLatiSezione */
	int numTriangoli;	/* = 2 * numVertex */

	int numMine;
};

#define LS	20		/* numLatiSezione */
static struct proprietaToroide propToro[NUMERO_MAPPE_MODULO] = {
/*	rToro	rSez	nSez	nLSez	nVert	nTri	nMine */
	{0,		0,		20,		LS,		400,	800,	20},
	{0,		0,		30,		LS,		600,	1200,	30},
	{0,		0,		40,		LS,		800,	1600,	40},
	{0,		0,		60,		LS,		1200,	2400,	50}
};

/* array dei vertici */
static float *vertici = NULL;
/* array dei triangoli-vertici */
static int *triangoli = NULL;
/* array delle sezioni */
static int *sezioni = NULL;


/* sensibilità del mouse */
#define X_SENS 2.25e-03f
#define Y_SENS 2.25e-03f


/* angoli di posizione del Toroide */
static GLfloat alfa;
static GLfloat beta;


/* OPENGL */
#define Z_VIEW -4.0
#define Z_MIN  0.1f
//#define Z_MAX  10.0f
#define Z_MAX  10.0f
#define FOV    32.0f		



/*******************************************
********							********
********	Funzioni Ausiliarie		********
********							********
*******************************************/


static void buildArray (const struct proprietaToroide propToro[NUMERO_MAPPE_MODULO], int number)
{
	// float vertici[N_VERTEX][3]
	vertici = malloc(sizeof(float) * propToro[number].numVertex * 3);
	// int triangoli[N_TRIANGOLI][3]
	triangoli = malloc(sizeof(int) * propToro[number].numTriangoli * 3);
	// int sezioni[N_SEZIONI][N_LATI]
	sezioni = malloc(sizeof(int) * propToro[number].numSezioni * propToro[number].numLatiSezione);
}


static void destroyArray ()
{
	free(vertici);
	vertici = NULL;
	free(triangoli);
	triangoli = NULL;
	free(sezioni);
	sezioni = NULL;
}


static void multMatVet (const float m[4][4], float v[4]) 
{
	int i, j;
	float r[4];

	for (i = 0; i < 3; i++)	{			// i riga
		r[i] = 0.0f;
		for (j = 0; j < 3; j++)			// j colonna
			r[i] += m[i][j] * v[j];
	}

	v[0] = r[0];
	v[1] = r[1];
	v[2] = r[2];
	v[3] = r[3];
}


static void	mult (GLfloat * m1, GLfloat * m2, GLfloat * r)
{
	int i, j, k;
	
	for (i = 0; i < 4; i++)				// i riga
		for (j = 0; j < 4; j++) {		// j colonna
			r[i + j * 4] = 0;
			for (k = 0; k < 4; k++)
				r[i + j * 4] += m1[i + k * 4] * m2[j * 4 + k];
		}
}


/***********************************************
********								********
********	Fine Funzioni Ausiliarie	********
********								********
***********************************************/









void	GetMineInfo (int *version, struct MINE_MODULE_INFO *module)
{
	strcpy (module->name, "Toroidi");
	*version = 0x100;
}


void	GetMapCount (int * count)
{
	*count = 4;
}


void	GetMapName (int number, char * name)
{
	strcpy (name, "Toro");
}


void	BuildMap (int number, struct MINESWEEPER_MAP * map)
{
}


void	ResetMap (int number)
{
}


void	DestroyMap (int number, struct MINESWEEPER_MAP * map)
{
}


int		MouseMove (int number, int dx, int dy)
{
	return 1;
}


void	SetCameraParams (int number, int width, int height)
{
}












