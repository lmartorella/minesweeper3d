

// WINDOWS SPECIFIC & OPENGL!!
#include "stdafx.h"

#include "library.h"
#include "strings.h"
#include "game.h"




// Qui è conservata la mappa

struct 	MINESWEEPER_MAP		map = { 0,
                                    0, 0, 0,
									NULL, NULL,
									NULL, 
									NULL, 
									0, 0 };






static int	enumerateNeighbours (int idx, int * buffer, int bufferSize)
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








void	postDestroyMap()
{
	if (map.neighbour)
		delete map.neighbour;
	map.neighbour = NULL;
	if (map.place)
		delete map.place;
	map.place = NULL;
}



DWORD	postBuildMap ()
{
	int i, j, k;
	map.neighbour = new MINESWEEPER_NEIGHBOUR [map.nPlaces];
	map.place = new int [map.nPlaces];

	for (i=0; i<map.nPlaces; i++) {
		if ((k = enumerateNeighbours (i, map.neighbour[i].n, MAX_NEIGHBOURS)) > MAX_NEIGHBOURS)
			return IDS_MAP_TOONEIGHBOURS;
		for (j = k; j < MAX_NEIGHBOURS; j++)
			map.neighbour[i].n[j] = -1;
	}
	return 0;
}




void	placeNumbers()
{
	int i, j;
	for (i = 0; i < map.nPlaces; i++) {
		for (j = 0; j < MAX_NEIGHBOURS && map.neighbour[i].n[j] != -1; j++)
			if (map.place[map.neighbour[i].n[j]] & MAP_PLACE_MINE)
				map.place[i] ++; 
	}
}




