

#pragma once



#define CFGFILE_VER 1
#define MAX_NAMELENGHT 32

struct SUPERSWEEPER {
	int		time;
	char	name [MAX_NAMELENGHT];
};

struct	GLOBAL_VARS {
	struct SUPERSWEEPER hallsOfFame[NUMMAPTYPE];
};



#ifdef __cplusplus 
extern "C" {
	void	LoadSettings();
	int		StoreSettings ();
}
#else
	extern void		LoadSettings();
	extern int		StoreSettings ();
#endif