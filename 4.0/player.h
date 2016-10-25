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
	player->resource = 30; player->base_income = 2;
	player->no_of_expansion = 0;
	player->expansion_list = (int *) malloc((MAPSIZE*MAPSIZE/200+1)*sizeof(int));
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
	(player->no_of_units)++; player->units_alive++;
	(player->unit_list)[player->no_of_units-1] = unit;
	unit->player_id = player->player_id;
	unit->unit_id = player->no_of_units;
	player->resource -= unit->cost;
}

/****************************************************************************/
/* this function recruit unit according to the player's command */
Unit * recruit(Player * p1, char ** command){
	if(p1->no_of_units == ARMYSIZE){
		printf("Maximum recruitment reached\n"); return 0;
	}
	if(!command[1]){
		printf("select the type of troop for the new recruit?\n");
		printf("\t%-15s%5s\n","Unit", "Cost");
		printf("\t%-15s%5d\n","Swordman(s)", 10);
		printf("\t%-15s%5d\n","Archer(a)", 10);
		printf("\t%-15s%5d\n","Horseman(h)", 10);
		getcommand(command+1, 10);
		char ** cmd1 = command+1;
		cmd1 = parsecommand(command[1], 2);
		command[1] = cmd1[0]; command[2] = cmd1[1];
	}
	int x=0,y=0,i=0;	// coordinate for new recruit
	if(command[2]){
		convert2numbers(command[2], &x, &y);
		while(abs(x-1-p1->x > 1)||abs(y-1-p1->y > 1)){
			printf("Sorry the spot chosen is too far from your base! Please choose the spot again\n");
			get2numbers(&x, &y); i++;
			if(i>2){ printf("random spot for the recruit!\n"); x=y=0; break;}
		}
	}
	Unit * unit = 0;
	if(strcmp(command[1], "a")==0 || strcmp(command[1], "archer")==0){
		unit = Archer(x, y, p1);
	}else if(strcmp(command[1], "s")==0 || strcmp(command[1], "swordman")==0){
		unit = Swordman(x,y, p1);
	}else if(strcmp(command[1], "h")==0 || strcmp(command[1], "horseman")==0){
		unit = Horseman(x,y, p1);
	}else{
		printf("Invalid troop! choose a valid troop\n"); return 0;
	}
	if(p1->resource<unit->cost){
		printf("Not enough resource!\n"); return 0;
	}
	player_recruit(p1, unit);
	printf("%s %d%c%d recruited at %d,%d\n", unit->unit_type, p1->player_id,
		unit->unit_symbol, unit->unit_id, unit->x+1, unit->y+1);
	return unit;
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
void legend(){
	printf("\tTerrain Legend\n");
	int i;
	for(i = 0; i < TERRAIN_VARIETY; i++){
		printf("\t\t%c : %s\n", (TARRAY+i)->terrain_symbol, (TARRAY+i)->terrain_name);
	}
	printf("\n\t\t$ : expansion\n");
}

/****************************************************************************/
/* this function modifies incomes of a player at the begining of the turn */
void income(Player *p1){
	Expansion * expansion; int i;
	printf("+%d income from base\n", p1->base_income); p1->resource += 2;	// income from base
	for(i=0; i < MAPSIZE*MAPSIZE/200+1; i++){
		expansion = EMAP[i];
		if(expansion->player_id == p1->player_id){
			printf("+%d income from %d%c%d\n", expansion->income, expansion->player_id, expansion->expansion_symbol, expansion->expansion_id);
			p1->resource += EMAP[i]->income;
		}
	}
}

/****************************************************************************/
/* this funciton restores unit's action and movement points for the player */
void pre_turn(Player *p1){
	int i; Unit ** unit_list = p1->unit_list; Expansion * expansion;
	for(i = 0; i < p1->no_of_units; i++){
		if(unit_list[i]){
			if((expansion = locate_expansion(unit_list[i]->x, unit_list[i]->y))){
				// p1->expansion_list[p1->no_of_expansion] = expansion;
				if(expansion->player_id != p1->player_id){
					printf("Expansion %d%c%d secured by %d%c%d!\n", 
						expansion->player_id, expansion->expansion_symbol, expansion->expansion_id,
						unit_list[i]->player_id, unit_list[i]->unit_symbol, unit_list[i]->unit_id);
					enter_to_continue();
				}
				expansion->player_id = p1->player_id;
			}
		}
	}
	
	p1->no_of_hostile = 0;	// no hostile units seen
	for(i = 0; i < p1->no_of_units; i++){	// restore all action points for units
		if(unit_list[i]){
			memset(unit_list[i]->action, 0, 3);
			unit_list[i]->movement = unit_list[i]->movementbar;
		}
	}
	income(p1);
}

/****************************************************************************/
/* this function print the map pixel, units and playerbase if any */
void getmap(Player * p1, Player * p2){
	int x, y, *xmins, *ymins, *xmaxs, *ymaxs, in_sight, no_of_units;
	Expansion * expansion;
	no_of_units = p1->no_of_units;
	Unit* found_unit;
	Player* found_player;
	
	p1->no_of_hostile = 0;
	buildsight(&xmins, &ymins, &xmaxs, &ymaxs, p1);
	
	printf("\nBATTLE GROUND!\n");
	// map top axis label
	printaxis(); printf("\n"); print_border(); printf("\n");
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
				}else if((found_player = locate_player(x, y, p2))){
					printobj(found_player, 0);
				}else{	// print the terrain
					fillwith(PIXEL_SIZE/2-1, ' ');
					if((expansion = locate_expansion(x, y))){
						printf("%d%c%d", expansion->player_id, expansion->expansion_symbol, expansion->expansion_id);
						fillwith(PIXEL_SIZE-PIXEL_SIZE/2-2, ' ');
					}else{
						printf("%c", find_terrain(x, y));
						fillwith(PIXEL_SIZE-PIXEL_SIZE/2, ' ');
					}
				}
			}else{	// not in sight range, print fog
				if((expansion = locate_expansion(x, y))){
					fillwith(PIXEL_SIZE/2-1, ' ');
					printf("%d%c%d", expansion->player_id, expansion->expansion_symbol, expansion->expansion_id);
					fillwith(PIXEL_SIZE-PIXEL_SIZE/2-2, ' ');
				}else{
					fillwith(PIXEL_SIZE, MAPFOGMATERIAL);					
				}
			}
		}
		printf("|%*d", PIXEL_SIZE, y+1);
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

/****************************************************************************/
/* this funciton prints the available commands a player can call */
void helpmanual(){
	printf("\n"); fillwith(MAPSIZE*PIXEL_SIZE, '+'); printf("\n\n");
	printf("\tselect a soldier/target by its full name appeared on the board\n\n");
	printf("\tCommand\t\tHotkey\tExplanation\n");
	printf("\t%-10s\tr\tTo recruit a troop\n\n", "recruit");
	printf("\t%-10s\th\tTo get this help manual for available command turn\n\n", "help");
	printf("\t%-10s\t\tTo get latest status of the battle\n\n", "get");
	printf("\t%-10s\t\tTo end your turn\n\n", "end/quit");
	printf("\t%-10s\tb\tTo dis-select the current unit\n\n", "back");
	printf("\t%-10s\trp\tTo view the current status of all visible units\n\n", "report");
	printf("\t%-10s\tl\tTo view the legend of the map\n","legend");
	printf("\n"); fillwith(MAPSIZE*PIXEL_SIZE, '+'); printf("\n\n");
}

/****************************************************************************/
/* this function check for commands of the player */
int checkcommand(char ** commands, Player * p1, Player * p2){
	if(strcmp(commands[0], "quit")==0 || strcmp(commands[0], "end")==0){
		commands[0] = 0; return 1;
	}else if(strcmp(commands[0], "recruit")==0 || strcmp(commands[0], "r")==0){
		recruit(p1, commands); commands[0] = 0; return 3;
	}else if(strcmp(commands[0], "back")==0 || strcmp(commands[0], "b")==0){
		commands[0] = 0; return 2;
	}else if(strcmp(commands[0], "report")==0||strcmp(commands[0], "rp")==0){
		report(p1); commands[0] = 0; return 3;
	}else if(strcmp(commands[0], "help")==0 || strcmp(commands[0], "h")==0){
		helpmanual(); commands[0] = 0; return 3;
	}else if(strcmp(commands[0], "get")==0){
		getmap(p1, p2); commands[0] = 0; return 3;
	}else if(strcmp(commands[0], "legend")==0 || strcmp(commands[0], "l")==0){
		legend(); return 3;
	}
	return 0;
}
