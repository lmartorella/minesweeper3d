

#include "stdafx.h"

#include "map.h"

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
static GLfloat vdata[12][3] = {   
   {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},   
   {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},   
   {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
};

/* array dei triangoli-vertici */
static GLuint tindices[20][3] = {
   {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},   
   {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},   
   {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
   {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} 
};

/* array dei lati */
static GLuint lindex[30][3] = {
	{0,1,-1}, {0,4,-1}, {0,6,-1}, {0,9,-1}, {0,11,-1},
	{1,4,-1}, {1,6,-1}, {1,8,-1}, {1,10,-1}, {2,3,-1},
	{2,5,-1}, {2,7,-1}, {2,9,-1}, {2,11,-1}, {3,5,-1},
	{3,7,-1}, {3,8,-1}, {3,10,-1}, {4,5,-1}, {4,8,-1},
	{4,9,-1}, {5,8,-1}, {5,9,-1}, {6,7,-1}, {6,10,-1},
	{6,11,-1}, {7,10,-1}, {7,11,-1}, {8,10,-1}, {9,11,-1},
};


/* array equivalenti per il d80 */
static GLfloat vdata80[42][3];		/* array dei vertici */
static GLuint tindices80[80][3];	/* array dei triangoli-vertici */
static GLuint lindex80[120][2];		/* array dei lati */






void normalize(float v[3]) {   
	GLfloat d = (GLfloat)sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
	if (d == 0.0) {
		exit(1);
	}
	v[0] /= d; v[1] /= d; v[2] /= d;
}



void subdivide(int index, GLuint *lastVertex, GLuint *lastTriangle)
{
	GLfloat v0[3], v1[3], v2[3];
	GLfloat v01[3], v12[3], v20[3];
	GLuint newVertex[3];
	GLint i;

	/* carico in v0,v1,v2 i vertici del triangolo "index" */
	for (i = 0; i < 3; i++) {
		v0[i] = vdata[tindices[index][0]][i];
		v1[i] = vdata[tindices[index][1]][i];
		v2[i] = vdata[tindices[index][2]][i];
	}

	
	/* da fare per i 3 vertici intermedi */

	/* vertice tra v0, v1 */ 
	for (i=0; i<20; i++) {
		/* trova il lato corrispondente ai 2 vertici */
		if ((tindices[index][0] == lindex[i][0] && tindices[index][1] == lindex[i][1]) || (tindices[index][0] == lindex[i][1] && tindices[index][1] == lindex[i][0])) {
			/* trovato il lato */
			/* controlla se esiste già un vertice intermedio */
			if (lindex[i][3] == -1) {
				/* non esiste vertice intermedio */
				/* ne crea uno nuovo e lo inserisce nell'array vdata80 */
				v01[0] = v0[0]+v1[0];
				v01[1] = v0[1]+v1[1];
				v01[2] = v0[2]+v1[2];
				normalize(v01);
				lindex[i][3] = *lastVertex;
				vdata80[lindex[i][3]][0] = v01[0];
				vdata80[lindex[i][3]][1] = v01[1];
				vdata80[lindex[i][3]][2] = v01[2];
				(*lastVertex)++;
			}
			else {
				/* esiste vertice intermedio */
				/* assegna a v01 il valore del vertice */
				v01[0] = vdata80[lindex[i][3]][0];
				v01[1] = vdata80[lindex[i][3]][1];
				v01[2] = vdata80[lindex[i][3]][2];
			}
			i=20;
			newVertex[0] = lindex[i][3];
			/* prepearato vertice v01 */
		}
	}

	/* vertice tra v1, v2 */ 
	for (i=0; i<20; i++) {
		/* trova il lato corrispondente ai 2 vertici */
		if ((tindices[index][1] == lindex[i][1] && tindices[index][2] == lindex[i][2]) || (tindices[index][1] == lindex[i][2] && tindices[index][2] == lindex[i][1])) {
			/* trovato il lato */
			/* controlla se esiste già un vertice intermedio */
			if (lindex[i][3] == -1) {
				/* non esiste vertice intermedio */
				/* ne crea uno nuovo e lo inserisce nell'array vdata80 */
				v12[0] = v1[0]+v2[0];
				v12[1] = v1[1]+v2[1];
				v12[2] = v1[2]+v2[2];
				normalize(v12);
				lindex[i][3] = *lastVertex;
				vdata80[lindex[i][3]][0] = v12[0];
				vdata80[lindex[i][3]][1] = v12[1];
				vdata80[lindex[i][3]][2] = v12[2];
				(*lastVertex)++;
			}
			else {
				/* esiste vertice intermedio */
				/* assegna a v12 il valore del vertice */
				v12[0] = vdata80[lindex[i][3]][0];
				v12[1] = vdata80[lindex[i][3]][1];
				v12[2] = vdata80[lindex[i][3]][2];
			}
			i=20;
			newVertex[1] = lindex[i][3];
			/* prepearato vertice v12 */
		}
	}

	/* vertice tra v2, v0 */ 
	for (i=0; i<20; i++) {
		/* trova il lato corrispondente ai 2 vertici */
		if ((tindices[index][2] == lindex[i][2] && tindices[index][0] == lindex[i][0]) || (tindices[index][2] == lindex[i][0] && tindices[index][0] == lindex[i][2])) {
			/* trovato il lato */
			/* controlla se esiste già un vertice intermedio */
			if (lindex[i][3] == -1) {
				/* non esiste vertice intermedio */
				/* ne crea uno nuovo e lo inserisce nell'array vdata80 */
				v20[0] = v2[0]+v0[0];
				v20[1] = v2[1]+v0[1];
				v20[2] = v2[2]+v0[2];
				normalize(v20);
				lindex[i][3] = *lastVertex;
				vdata80[lindex[i][3]][0] = v20[0];
				vdata80[lindex[i][3]][1] = v20[1];
				vdata80[lindex[i][3]][2] = v20[2];
				(*lastVertex)++;
			}
			else {
				/* esiste vertice intermedio */
				/* assegna a v20 il valore del vertice */
				v20[0] = vdata80[lindex[i][3]][0];
				v20[1] = vdata80[lindex[i][3]][1];
				v20[2] = vdata80[lindex[i][3]][2];
			}
			i=20;
			newVertex[2] = lindex[i][3];
			/* prepearato vertice v20 */
		}
	}


	/* creo i quattro nuovi triangoli e li inserisco in tindices80 */
	/* 1^ triangolo - Parallelo a index, mantiene l'indice */
	tindices80[index][0] = newVertex[0];
	tindices80[index][1] = newVertex[1];
	tindices80[index][2] = newVertex[2];
	/* 2^ triangolo */
	tindices80[(*lastTriangle)][0] = tindices[index][0];
	tindices80[(*lastTriangle)][1] = newVertex[0]; /* v01 */
	tindices80[(*lastTriangle)][2] = newVertex[2]; /* v20 */
	/* 3^ triangolo */
	tindices80[(*lastTriangle)+1][0] = tindices[index][1];
	tindices80[(*lastTriangle)+1][1] = newVertex[1]; /* v12 */
	tindices80[(*lastTriangle)+1][2] = newVertex[0]; /* v01 */
	/* 4^ triangolo */
	tindices80[(*lastTriangle)+2][0] = tindices[index][2];
	tindices80[(*lastTriangle)+2][1] = newVertex[2]; /* v20 */
	tindices80[(*lastTriangle)+2][2] = newVertex[1]; /* v12 */

	(*lastVertex) = (*lastVertex) + 3;
}



//static GLfloat vdata80[42][3];		/* array dei vertici */
//static GLuint tindices80[80][3];	/* array dei triangoli-vertici */
//static GLuint lindex80[120][2];		/* array dei lati */

void	vinsFunction (struct MINESWEEPER_MAP * map)
{
	int i, j;
	GLuint lVertex = 20;
	GLuint lTriangle = 20;

	/* cicolo di chiamata */
	for (i = 0; i < 20; i++) 
		subdivide(i, &lVertex, &lTriangle);

	map->cullingMode = GL_CCW;
	map->isConvex = TRUE;
	map->nPlaces = 80;
	map->nVertexes = 42;
	map->place = NULL;

	map->vertex = (struct MINESWEEPER_VERTEX*) malloc (sizeof (struct MINESWEEPER_VERTEX) * 42);
	map->face = (struct MINESWEEPER_FACE*) malloc (sizeof (struct MINESWEEPER_FACE) * 80);

	for (i = 0; i < 42; i++) {
		map->vertex[i].x = vdata80[i][0];
		map->vertex[i].y = vdata80[i][1];
		map->vertex[i].z = vdata80[i][2];
	}
	for (i = 0; i < 80; i++) {
		map->face[i].v[0] = tindices[i][0];
		map->face[i].v[1] = tindices[i][1];
		map->face[i].v[2] = tindices[i][2];
		for (j = 3; j < 12; j++)
			map->face[i].v[j] = -1;
	}

}


