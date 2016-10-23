// this module defines terrain system of the game

/****************************************************************************/
/* this function create a terrain map for the map of MAPSIZE size */
int ** create_tmap(){
	int i, j, ** tmap;
	tmap = (int **) malloc (MAPSIZE/4 * sizeof(int *));
	srand(time(NULL));
	for(i = 0; i < MAPSIZE/4; i++){
		tmap[i] = (int *) malloc (MAPSIZE/4 * sizeof(int));
		for(j = 0; j < MAPSIZE/4; j++){
			tmap[i][j] = rand()%TERRAIN_VARIETY+1;
		}
	}
	return tmap;
}

/****************************************************************************/
/* this function initialize terrain used in create_map */
Terrain * init_terrain(){
	Terrain * tarray;
	tarray = (Terrain *) malloc (TERRAIN_VARIETY * sizeof(Terrain));
	switch(TERRAIN_VARIETY){
		case 3:
			tarray[2].id_terrain = 2;
			tarray[2].terrain_symbol = '~';
			tarray[2].terrain_name = "river";
		case 2:
			tarray[1].id_terrain = 2;
			tarray[1].terrain_symbol = '!';
			tarray[1].terrain_name = "forest";
		case 1:
			tarray[0].id_terrain = 1;
			tarray[0].terrain_symbol = '-';
			tarray[0].terrain_name = "flatland";
	}
	return tarray;
}

/****************************************************************************/
/* this function takes a cordinate and return its terrain symbol */
char find_terrain(int x, int y, int ** map, Terrain * terray){
	switch(map[y/4][x/4]){
		case 1:
			return terray->terrain_symbol;
		case 2:
			return (terray+1)->terrain_symbol;
		case 3:
			return (terray+2)->terrain_symbol;
	}
	return 0;
}

/****************************************************************************/
#define TMAP create_tmap()	// terrain info of all pixel in map
#define TARRAY init_terrain() // types of terrain used in map generation
/****************************************************************************/