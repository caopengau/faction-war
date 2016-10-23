// this module defines player's callable functions and related functions

/****************************************************************************/
/* this function create a player 
	arg1: the id for new added player
	arg2,3: the xlocation,ylocation for its base
	arg4: the initial maximum size for its army
*/
Player createplayer(int player_id, char sym_base, int x, int y){
	Player * player;
	player = (Player *) malloc (1 * sizeof(Player));
	player->player_id = player_id;
	player->base = sym_base;
	player->x = x-1; player->y = y-1;
	player->sight = 4;
	player->unit_list = (Unit**)malloc(ARMYSIZE*sizeof(Unit *));
	player->hostile_spotted = (Unit**)malloc(ARMYSIZE*sizeof(Unit *));
	player->no_of_units = 0;
	player->units_alive = 0;
	player->no_of_hostile = 0;
	player->resource = 30;
	player->no_of_expansion = 0;
	player->expansion_list = (Expansion **) malloc (MAPSIZE*MAPSIZE/200+1);
	return *player;
}

/****************************************************************************/
/* this function return the player if player's base is on the coordinate */
Player* locate_player(int x, int y, Player * player){
	if(x== player->x && y== player->y){
		return player;
	}
	return 0;
}

/****************************************************************************/
/* this function add a unit to a player */
void player_recruit(Player * player, Unit * unit){
	(player->no_of_units)++;
	player->units_alive++;
	(player->unit_list)[player->no_of_units-1] = unit;
	unit->player_id = player->player_id;
	unit->unit_id = player->no_of_units;
}

/****************************************************************************/
/* this function recruit unit according to the player's command */
Unit* recruit(Player * p1, char ** command){
	if(!command[1]){
		printf("What type of troop?\n");
		printf("\t%-10s%5s\n","Unit", "Cost");
		printf("\t%-10s%5d\n","Swordman", 10);
		printf("\t%-10s%5d\n","Archer", 10);
		printf("\t%-10s%5d\n","Horseman", 10);
		getcommand(command+1, 10);
	}
	Unit * unit = NULL;
	if(strcmp(command[1], "Archer")==0 || strcmp(command[1], "archer")==0){
		unit = Archer(0, 0, p1);
	}else if(strcmp(command[1], "swordman")==0 || strcmp(command[1], "Swordman")==0){
		unit = Swordman(0,0, p1);
	}else if(strcmp(command[1], "horseman")==0 || strcmp(command[1], "Horseman")==0){
		unit = Horseman(0,0, p1);
	}else{
		printf("Invalid troop!\n"); return 0;
	}
	if(p1->resource<unit->cost){
		printf("Not enough resource!\n"); return 0;
	}
	player_recruit(p1, unit); return unit;
}

/****************************************************************************/
/* this function print the object that occupy the cordinate */
void printobj(Player * player, Unit * unit){
	fillwith((PIXEL_SIZE-3)/2, ' ');
	if(unit) printf("%d%c%d", unit->player_id, unit->unit_symbol, unit->unit_id);
	else if(player){
		printf("%d%c%c", player->player_id, player->base, player->base);
	}
	fillwith(PIXEL_SIZE-3-(PIXEL_SIZE-3)/2, ' ');
}

/****************************************************************************/
/* this function print the map pixel, units and playerbase if any */
void getmap(Player * p1, Player * p2){
	int x, y, *xmins, *ymins, *xmaxs, *ymaxs, in_sight, no_of_units;
	no_of_units = p1->no_of_units;
	Unit* found_unit;
	Player* found_player;
	
	p1->no_of_hostile = 0;
	buildsight(&xmins, &ymins, &xmaxs, &ymaxs, p1);
	
	printf("\nBATTLE GROUND!\n");
	// map top axis label
	printaxis(); printf("\n"); print_border(); 
	printf("\t\tTerrain Legend\n");
	// map body
	for(y = 0; y < MAPSIZE; y++){
		printf("%*d|", PIXEL_SIZE, y+1);
		for(x = 0; x < MAPSIZE; x++){
			in_sight = checksight(xmins, ymins, xmaxs, ymaxs,
									x, y, no_of_units+1);
			if(!WARFOGENABLED) in_sight = 1;
			if(in_sight){
				if((found_unit = locate_unit(x, y, p1->unit_list, p1->no_of_units))){	// friendly units
					printobj(p1, found_unit);
				}else if((found_unit = locate_unit(x, y, p2->unit_list, p2->no_of_units))){	// enemy units
					addhostile(found_unit, p1);
					printobj(p2, found_unit);
				}else if((found_player = locate_player(x, y, p1))){
					printobj(found_player, 0);
				}else if((found_player = locate_player(x, y, p1))){
					printobj(found_player, 0);
				}else{	// print the terrain
					fillwith(PIXEL_SIZE/2, ' ');
					printf("%c", find_terrain(x, y, TMAP, TARRAY));
					fillwith(PIXEL_SIZE-PIXEL_SIZE/2-1, ' ');
				}
			}else{
				fillwith(PIXEL_SIZE, MAPFOGMATERIAL);
			}
		}
		printf("|%*d", PIXEL_SIZE, y+1);
		if(y<TERRAIN_VARIETY){
			printf("\t\t%c : %s", (TARRAY+y)->terrain_symbol, (TARRAY+y)->terrain_name);
		}
		if(PIXEL_SIZE/5){
			fillwith(PIXEL_SIZE/5, '\n');	// vertical adjust to large pixel choice
			printf("%*c", PIXEL_SIZE+1, '|');
			fillwith(MAPSIZE*PIXEL_SIZE, ' '); printf("|");
			
		}
		printf("\n");
	}
	// map bottom axis label
	 print_border(); printf("\n"); printaxis(); printf("\n");
}
