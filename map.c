

#include "stdafx2.h"
#include "map.h"


#define X .525731112119133606f
#define Z .850650808352039932f
static struct MINESWEEPER_VERTEX icoVdata[12] = {   
   {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},   
   {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},   
   {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
};
static struct MINESWEEPER_FACE icoIndices[20] = {
   {0,4,1,-1,-1,-1,-1,-1,-1,-1,-1,-1}, 
   {0,9,4,-1,-1,-1,-1,-1,-1,-1,-1,-1}, 
   {9,5,4,-1,-1,-1,-1,-1,-1,-1,-1,-1}, 
   {4,5,8,-1,-1,-1,-1,-1,-1,-1,-1,-1}, 
   {4,8,1,-1,-1,-1,-1,-1,-1,-1,-1,-1},   
   {8,10,1,-1,-1,-1,-1,-1,-1,-1,-1,-1}, 
   {8,3,10,-1,-1,-1,-1,-1,-1,-1,-1,-1}, 
   {5,3,8,-1,-1,-1,-1,-1,-1,-1,-1,-1}, 
   {5,2,3,-1,-1,-1,-1,-1,-1,-1,-1,-1}, 
   {2,7,3,-1,-1,-1,-1,-1,-1,-1,-1,-1},   
   {7,10,3,-1,-1,-1,-1,-1,-1,-1,-1,-1}, 
   {7,6,10,-1,-1,-1,-1,-1,-1,-1,-1,-1}, 
   {7,11,6,-1,-1,-1,-1,-1,-1,-1,-1,-1}, 
   {11,0,6,-1,-1,-1,-1,-1,-1,-1,-1,-1}, 
   {0,1,6,-1,-1,-1,-1,-1,-1,-1,-1,-1},
   {6,1,10,-1,-1,-1,-1,-1,-1,-1,-1,-1}, 
   {9,0,11,-1,-1,-1,-1,-1,-1,-1,-1,-1}, 
   {9,11,2,-1,-1,-1,-1,-1,-1,-1,-1,-1}, 
   {9,2,5,-1,-1,-1,-1,-1,-1,-1,-1,-1}, 
   {7,2,11,-1,-1,-1,-1,-1,-1,-1,-1,-1} 
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


int		buildMap (struct MINESWEEPER_MAP * map, DWORD type)
{
	map->type = type;

	switch (type) {
	case MAP_ICOSAHEDRON:
		map->nPlaces = 20;
		map->nVertexes = 12;
	
		map->face = icoIndices;
		map->vertex = icoVdata;
	
		map->place = (int*) malloc (sizeof(int) * 20);
		break;
	case MAP_CUBE:
		map->nPlaces = 6;
		map->nVertexes = 8;
	
		map->face = cubeIndices;
		map->vertex = cubeVdata;
	
		map->place = (int*) malloc (sizeof(int) * 6);
		break;
	case MAP_VINS:
		vinsFunction (map);
		map->place = (int*) malloc (sizeof(int) * 80);

		break;
	default:
		return 0;
	}
	return 1;
}




int		destroyMap (struct MINESWEEPER_MAP * map)
{
	free (map->place);
	map->place = NULL;

	if (map->type == MAP_ICOSAHEDRON || map->type == MAP_CUBE)
		return 1;
	else if (map->type == MAP_VINS) {
		free (map->face);
		free (map->vertex);
		map->face = NULL;
		map->vertex = NULL;
		return 1;
	}
	else
		return 0;
	return 1;
}





int	enumerateNeighbor (struct MINESWEEPER_MAP * map, UINT idx, UINT * buffer, int bufferSize)
{
	int i, j, k, t = 0, bufferOk = 1, found;
	
	if (buffer == NULL || bufferSize <= 0)
		bufferOk = 0;

	for (i = 0; i < map->nPlaces; i++) {
		found = 0;
		for (j = 0; j < MAX_VERTEX_FACE && found == 0; j++) {
			if (map->face[i].v[j] == -1) 
				break;

			for (k = 0; k < MAX_VERTEX_FACE && found == 0; k++)
				if (map->face[idx].v[k] == -1) 
					break;
				else if (map->face[i].v[j] == map->face[idx].v[k])
						found = 1;
		}
		
		if (found && (map->place[i] & MAP_PLACE_MINE)) {
			if (bufferOk && t < bufferSize)
				buffer[t] = i;
			t++;
		}
	}
	return t;
}





void prepareMap (struct MINESWEEPER_MAP * map, int mines)
{
	int i, j;
		
	// Mette mine
	for (i = 0; i < map->nPlaces; i++)
		map->place[i] = MAP_PLACE_COVERED;

	for (i = 0; i < mines; i++) {
		do {
			j = (int)ceil(((double)rand() / RAND_MAX) * map->nPlaces);
		} while (map->place[j] & MAP_PLACE_MINE);
		map->place[j] |= MAP_PLACE_MINE;
	}

	// Ora setta i numeri
	for (i = 0; i < map->nPlaces; i++) {
		map->place[i] += enumerateNeighbor (map, i, NULL, 0);
	}
}			






