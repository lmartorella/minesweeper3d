

#include "stdafx2.h"
#include "map.h"



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


int		buildMap (struct MINESWEEPER_MAP * map, DWORD type, DWORD param)
{
	map->type = type;

	switch (type) {
	case MAP_NUPICOSAHEDRON:
		buildN_UpIcosahedron (map, param);
		break;
	case MAP_CUBE:
		map->nPlaces = 6;
		map->nVertexes = 8;
	
		map->face = cubeIndices;
		map->vertex = cubeVdata;
		break;
	default:
		return 0;
	}
	return 1;
}




int		destroyMap (struct MINESWEEPER_MAP * map)
{
	free (map->place);
	free (map->neighbour);
	map->place = NULL;
	map->neighbour = NULL;

	if (map->type == MAP_CUBE)
		return 1;
	else if (map->type == MAP_NUPICOSAHEDRON) {
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





int	enumerateNeighbours (struct MINESWEEPER_MAP * map, int idx, int * buffer, int bufferSize)
{
	int i, j, k, t = 0, bufferOk = 1, found;
	
	if (buffer == NULL || bufferSize <= 0)
		bufferOk = 0;

	for (i = 0; i < map->nPlaces; i++) {
		if (i == idx) 
			continue;
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
		
		if (found) {
			if (bufferOk && t < bufferSize)
				buffer[t] = i;
			t++;
		}
	}
	return t;
}





int	prepareMap (struct MINESWEEPER_MAP * map, int mines)
{
	int i, j, c;

	map->nMines = mines;

	// Vicini
	map->neighbour = (struct MINESWEEPER_NEIGHBOUR*) malloc (sizeof (struct MINESWEEPER_NEIGHBOUR) * map->nPlaces);
	map->place = (int*) malloc (sizeof(int) * map->nPlaces);

	// Setta i vicini
	for (i = 0; i < map->nPlaces; i++) {
		c = enumerateNeighbours (map, i, map->neighbour[i].n, MAX_NEIGHBOURS);
		if (c > MAX_NEIGHBOURS) 
			return 0;
		for (j = c; j < MAX_NEIGHBOURS; j++)
			map->neighbour[i].n[j] = -1;
	}
	
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
		for (j = 0; j < MAX_NEIGHBOURS && map->neighbour[i].n[j] != -1; j++)
			if (map->place[map->neighbour[i].n[j]] & MAP_PLACE_MINE)
				map->place[i] ++; 
	}

	return 1;
}		






