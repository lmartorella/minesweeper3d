

#include "stdafx2.h"
#include "map.h"
#include "resource.h"

struct 	MINESWEEPER_MAP		map = { -1,
                                    0, 0,
									NULL, NULL,
									NULL,
									NULL,
									0, 0 };
int		mapReady = 0;


struct MINESWEEPER_MAPTYPE mapType[NUMMAPTYPE] = {	
				{	IDM_ICO1, 2, MAP_NUPICOSAHEDRON, 0 },
				{   IDM_ICO2, 8, MAP_NUPICOSAHEDRON, 1 },
				{   IDM_ICO3, 48, MAP_NUPICOSAHEDRON, 2 },
				{   IDM_ICO4, 192, MAP_NUPICOSAHEDRON, 3 },
/*				{   IDM_V1, 10, MAP_V, 0 }*/
};



static struct MINESWEEPER_VERTEX cubeVdata[8] = {   
   {1, 1, 1}, {-1, 1, 1}, {1, -1, 1}, {-1, -1, 1},   
   {1, 1, -1}, {-1, 1, -1}, {1, -1, -1}, {-1, -1, -1}
};
static struct MINESWEEPER_FACE cubeIndices[6] = {
   {1,0,2,3,-1,-1,-1,-1,-1,-1,-1,-1}, 
   {5,4,0,1,-1,-1,-1,-1,-1,-1,-1,-1}, 
   {0,4,6,2,-1,-1,-1,-1,-1,-1,-1,-1}, 
   {4,5,7,6,-1,-1,-1,-1,-1,-1,-1,-1}, 
   {2,6,7,3,-1,-1,-1,-1,-1,-1,-1,-1}, 
   {1,3,7,5,-1,-1,-1,-1,-1,-1,-1,-1}, 
};


int		buildMap (DWORD typeIndex)
{
	map.typeIndex = typeIndex;

	switch (mapType[typeIndex].type) {
	case MAP_NUPICOSAHEDRON:
		buildN_UpIcosahedron (&map, mapType[typeIndex].param);
		break;
	case MAP_CUBE:
		map.nPlaces = 6;
		map.nVertexes = 8;
	
		map.face = cubeIndices;
		map.vertex = cubeVdata;
		break;
	default:
		return 0;
	}

	map.neighbour = (struct MINESWEEPER_NEIGHBOUR*) malloc (sizeof (struct MINESWEEPER_NEIGHBOUR) * map.nPlaces);
	map.place = (int*) malloc (sizeof(int) * map.nPlaces);
	mapReady = 1;
	return 1;
}




int		destroyMap ()
{
	free (map.place);
	free (map.neighbour);
	map.place = NULL;
	map.neighbour = NULL;

	if (mapType[map.typeIndex].type == MAP_CUBE)
		return 1;
	else if (mapType[map.typeIndex].type == MAP_NUPICOSAHEDRON) {
		free (map.face);
		free (map.vertex);
		map.face = NULL;
		map.vertex = NULL;
		return 1;
	}
	else
		return 0;
	return 1;
}





int	enumerateNeighbours (int idx, int * buffer, int bufferSize)
{
	int i, j, k, t = 0, bufferOk = 1, found;
	
	if (buffer == NULL || bufferSize <= 0)
		bufferOk = 0;

	for (i = 0; i < map.nPlaces; i++) {
		if (i == idx) 
			continue;
		found = 0;
		for (j = 0; j < MAX_VERTEX_FACE && found == 0; j++) {
			if (map.face[i].v[j] == -1) 
				break;

			for (k = 0; k < MAX_VERTEX_FACE && found == 0; k++)
				if (map.face[idx].v[k] == -1) 
					break;
				else if (map.face[i].v[j] == map.face[idx].v[k])
						found = 1;
		}
		
		if (found) {
			if (bufferOk && t < bufferSize)
				buffer[t] = i;
			t++;
		}
	}
	return t;
}





int	prepareMap (int mines, int rebuild)
{
	int i, j, c;

	map.nMines = map.nTotalMines = mines;
	map.nChecked = 0;

	// Setta i vicini
	if (rebuild)
		for (i = 0; i < map.nPlaces; i++) {
			c = enumerateNeighbours (i, map.neighbour[i].n, MAX_NEIGHBOURS);
			if (c > MAX_NEIGHBOURS) 
				return 0;
			for (j = c; j < MAX_NEIGHBOURS; j++)
				map.neighbour[i].n[j] = -1;
		}
	
	// Mette mine
	for (i = 0; i < map.nPlaces; i++)
		map.place[i] = MAP_PLACE_COVERED;

	for (i = 0; i < mines; i++) {
		do {
			j = (int)(((double)(rand()) / RAND_MAX) * map.nPlaces);
		} while (j < 0 || j >= map.nPlaces || map.place[j] & MAP_PLACE_MINE);
		map.place[j] |= MAP_PLACE_MINE;
	}

	// Ora setta i numeri
	for (i = 0; i < map.nPlaces; i++) {
		for (j = 0; j < MAX_NEIGHBOURS && map.neighbour[i].n[j] != -1; j++)
			if (map.place[map.neighbour[i].n[j]] & MAP_PLACE_MINE)
				map.place[i] ++; 
	}

	return 1;
}		






