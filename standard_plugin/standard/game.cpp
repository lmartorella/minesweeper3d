
#include "stdafx.h"

#define THIS_IS_A_MODULE
#include "..\..\library.h"


void	GetGameName (char * name)
{
	strcpy (name, "Standard game");
}



// ------------------------------------------------------- SWEEPER
// ------------------------------------------------------- SWEEPER
// ------------------------------------------------------- SWEEPER
// ------------------------------------------------------- SWEEPER


static void	totalSweeper(int uncover, struct MINESWEEPER_MAP * map)
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
static int		secureSweeper (int idx, struct MINESWEEPER_MAP * map)
{
	int i = -1, neigh;
	int end = 0;

#ifdef _DEBUG
	if (map->place[idx] & MAP_PLACE_COVERED == 0)
		__asm {int 3}
#endif

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




int		MouseButton				   (int msg, 
									int * buttons, 
									int * gameStatus,
									SELECTFACE_TYPE SelectFace_callback,
									struct MINESWEEPER_MAP * map)
{
	int hit, k, n, err, flag, a, ended;

	ended = *gameStatus == GAME_STATUS_ENDED_LOST || *gameStatus == GAME_STATUS_ENDED_WIN;

	switch (msg) {
	case WM_LBUTTONDOWN:
		*buttons |= MINE_LBUTTON;
		return 0;

	case WM_RBUTTONDOWN:
		*buttons |= MINE_RBUTTON;
		if (ended)
			return 0;

		////////////////////////////////////////////////////
		SelectFace_callback (&hit);
		////////////////////////////////////////////////////

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

		////////////////////////////////////////////////////
		SelectFace_callback (&hit);
		////////////////////////////////////////////////////
		if (hit == -1)
			return 0;

		if (msg == WM_LBUTTONUP && *buttons == MINE_LBUTTON) {
			*buttons = 0;
			if ((!(map->place[hit] & MAP_PLACE_COVERED)) ||
				map->place[hit] & MAP_PLACE_FLAG) 
				return 0;

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







int		GetTextureName	(int place, GLuint * tex)
{
	return 0;
}


DWORD		PrepareTextures (int filtering)
{
	return 0;
}

int		FreeTextures ()
{
	return 1;
}



