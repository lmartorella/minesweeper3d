

#include "stdafx2.h"
#include "map.h"
#include "vars.h"


static char * varsFile = "3dmine.cfg";
struct GLOBAL_VARS vars;



void	DefaultSettings ()
{
	int i;
	for (i = 0; i < NUMMAPTYPE; i++) {
		vars.hallsOfFame[i].time = 9999;
		vars.hallsOfFame[i].name[0] = 0;
	}
}


void	LoadSettings()
{
	DWORD ver;

	FILE * file;
	file = fopen (varsFile, "rb");
	if (file == NULL) 
		DefaultSettings ();
	else {
		if (fread (&ver, sizeof (DWORD), 1, file) != 1 || ver != CFGFILE_VER)
			DefaultSettings ();
		else {
			if (fread (&vars, sizeof (struct GLOBAL_VARS), 1, file) != 1) 
				DefaultSettings ();
			fclose (file);
		}
	}
}



int		StoreSettings ()
{
	DWORD ver = CFGFILE_VER;

	FILE * file;
	file = fopen (varsFile, "wb");
	if (file == NULL)
		return 0;
	if (fwrite (&ver, sizeof (DWORD), 1, file) != 1)
		return 0;
	if (fwrite (&vars, sizeof (vars), 1, file) != 1)
		return 0;
	fclose (file);

	return 1;
}