// this module defines units' actions

/* this function finds the unit occuping the cordinate (x,y)*/
Unit* locate_unit(int x, int y, Unit** unit_list, int no_of_units){
	int i;
	for(i = 0; i < no_of_units; i++){
		if(unit_list[i]){
			if(x==(unit_list[i])->x && y==(unit_list[i])->y){
				return unit_list[i];
			}
		}
	}
	return 0;
}

/****************************************************************************/
/* this function choose the nearby pixel around player base as the
birth coordinate for the soldier recruited */
void recruitspot(Unit * unit, Player * player){
	int i, j, x, y, unfit, validxy[8][2], valid = 0;
	for(i = -1; i < 2; i++){	// generates all possible spawn coordinate
		for(j = -1; j < 2; j++){
			if(i==0&&j==0){continue;}
			x = player->x + j;
			y = player->y + i;
			unfit = locate_unit(x, y, player->unit_list, player->no_of_units) || x>=MAPSIZE || y>=MAPSIZE ||
					x<0 || y<0 || (x==player->x && y==player->y);
			if(!unfit){
				validxy[valid][0] = x;
				validxy[valid][1] = y; valid++;
			}
		}
	}
	if(!valid){
		printf("failed to recruited!\n");
	}else{	// pick a random coordinate
		srand(time(NULL));
		i = rand()%valid;
		unit->x = validxy[i][0];
		unit->y = validxy[i][1];
	}
}

/****************************************************************************/
/* this function create a archer unit */
Unit * Archer(int x, int y, Player * player){
	Unit *archer;
	archer = (Unit*) malloc(1 * sizeof(Unit));
	archer->unit_type = "Archer";
	archer->unit_symbol = 'A';
	archer->lifebar = 20;
	archer->hitpoint = 20;
	archer->attack = 10;
	archer->attackrange = 5;
	archer->defense = 0;
	archer->movementbar = 4;
	archer->movement = 0;
	archer->sight = 6;
	archer->cost = 10;
	if(x&&y){
		archer->x = x-1;
		archer->y = y-1;
	}else{
		recruitspot(archer, player);
	}
	return archer;
}

/****************************************************************************/
/* this function create a swordman unit */
Unit * Swordman(int x, int y, Player * player){
	Unit *swordman;
	swordman = (Unit*) malloc(1 * sizeof(Unit));
	swordman->unit_type = "Swordman";
	swordman->unit_symbol = 'S';
	swordman->lifebar = 24;
	swordman->hitpoint = 24;
	swordman->attack = 6;
	swordman->attackrange = 1;
	swordman->defense = 4;
	swordman->movementbar = 5;
	swordman->movement = 0;
	swordman->sight = 5;
	swordman->cost = 10;
	if(x&&y){
		swordman->x = x-1;
		swordman->y = y-1;
	}else{
		recruitspot(swordman, player);
	}
	return swordman;
}

/****************************************************************************/
/* this function create a horseman unit */
Unit * Horseman(int x, int y, Player * player){
	Unit *horseman;
	horseman = (Unit*) malloc(1 * sizeof(Unit));
	horseman->unit_type = "Horseman";
	horseman->unit_symbol = 'H';
	horseman->lifebar = 20;
	horseman->hitpoint = 20;
	horseman->attack = 7;
	horseman->attackrange = 1;
	horseman->defense = 1;
	horseman->movementbar = 8;
	horseman->movement = 0;
	horseman->sight = 8;
	horseman->cost = 10;
	if(x&&y){
		horseman->x = x-1;
		horseman->y = y-1;
	}else{
		recruitspot(horseman, player);
	}
	return horseman;
}

/****************************************************************************/
/* this funcition print the info of a unit (assistant function to report())*/
void printunit(Unit * unit){
	printf("\t"); fillwith(76, '-'); printf("\n");
	printf("\t%5d%12s%6d%c%d%10d%10d%10d%10d%5d,%2d\n", 
		unit->player_id, unit->unit_type, unit->player_id, 
		unit->unit_symbol, unit->unit_id, unit->hitpoint, 
		unit->attack, unit->defense, unit->movement, 
		unit->x+1, unit->y+1);
}
/****************************************************************************/
/* this funciton prints all visible units' stats for the player */
void report(Player * p1){
	int i; Unit * unit;
	printf("\n\t%5s%12s%8s%10s%10s%10s%10s%8s\n", "Side", "UnitType", 
			"Unitid", "Hitpoint", "Attack", "Defense", "Movement", "x,y");
	for(i=0; i < p1->no_of_units; i++){	// p1's units
		if(p1->unit_list[i]){
			unit = p1->unit_list[i]; printunit(unit);
		}
	}
	for(i=0; i < p1->no_of_hostile; i++){	// units hostile to p1
		if(p1->hostile_spotted[i]){
			unit = p1->hostile_spotted[i]; printunit(unit);
		}
	}
	printf("\t"); fillwith(76, '-'); printf("\n");
}

/****************************************************************************/
/* this function is called for a unit to perform move action */
int move_unit(Unit * unit, char *command, Player * p1, Player * p2){
	if(unit->action[0]) {
		printf("no movement point available\n");
		enter_to_continue(); return 0; // can't move
	}
	int x, y, temp, guard;	// guard for 2 valid int input from the player
	if(!command){
		printf("Where do u want to move into? x y\n>");
		guard = get2numbers(&x, &y);
	}else{
		guard = convert2numbers(command, &x, &y);
	}
	
	if(guard){
		temp = x>0 && x <= MAPSIZE && y>0 && y<= MAPSIZE && 
			abs(x-1-unit->x) <= unit->movement && !locate_unit(x-1, y-1, p1->unit_list, p1->no_of_units) &&
			abs(y-1-unit->y) <= unit->movement && !locate_unit(x-1, y-1, p2->unit_list, p2->no_of_units);
		if(temp){
			(unit->action)[2] = 1;
			unit->movement -= abs(x-1-unit->x) > abs(y-1-unit->y)? abs(x-1-unit->x): abs(y-1-unit->y);
			unit->x = x-1; unit->y = y-1;
			printf("moved into (%d, %d)\n", unit->x+1, unit->y+1);
			if(unit->movement <= 0) unit->action[0] = 1;
			return 1;	// move successful
		}else{
			printf("move failed, target location not in reach\n");
			enter_to_continue();
			return 2;	// can move, but the location is not reachable
		}
	}else{
		printf("Invalid target location!\n"); enter_to_continue();
	}
	return 1;	// Invalid target need to ask for new command
	
}

/****************************************************************************/
/* this function will settle the death of a unit */
void killhostile(Player * p1, Unit * unit, Player * p2){
	int i; 
	printf("target %d%c%d killed\n", unit->player_id, unit->unit_symbol, 
									unit->unit_id);
	for(i = 0; i < p1->no_of_hostile; i++){
		if(unit==p1->hostile_spotted[i]) p1->hostile_spotted[i] = 0;
	}
	free(unit);
	p2->units_alive -= 1;
}

/****************************************************************************/
/* this function is called for a unit to perform attack */
int attack_unit(Unit * unit, Player * p1, Player * p2, char * hostile){
	if(unit->action[1]) {
		printf("no attack point available!\n"); 
		enter_to_continue(); return 0;
	}
	if(!p1->no_of_hostile){
		printf("no enemy spotted!\n");
		enter_to_continue(); return 0;
	}
	char *command;
	if(!hostile){
		printf("What is the target\n");
		getcommand(&command, 10);
	}else{
		command = hostile;
	}
	int unit_id = atoi(command+strlen(command)-1);
	if(unit_id>p2->no_of_units||unit_id<1){
		printf("attack failed! Invalid target\n");
		hostile = 0; enter_to_continue(); return 0;
	}
	Unit * target = (p2->unit_list)[unit_id-1];
	int range = unit->attackrange;
	if(abs(unit->x - target->x) <= range && abs(unit->y - target->y) <= range){
		int damage = unit->attack - target->defense;	// damage formula
		target->hitpoint -= damage;
		unit->action[0] = 1; unit->action[1] = 1; unit->action[2] = 1;
		unit->movement = 0;	// action change
		printf("attack successful!\n%d damage dealt to %d%c%d\n", 
			damage, target->player_id, target->unit_symbol, target->unit_id);
		enter_to_continue();
		if(target->hitpoint <= 0){
			killhostile(p1, target, p2); // death of the unit
			p2->unit_list[unit_id-1] = 0;
			if(p2->units_alive == 0) return 10;	// victory condition
			enter_to_continue(); report(p1); return 0;
		}
		report(p1); return 0;
	}
	printf("attack failed! target not in range\n");
	enter_to_continue(); return 0;
}

/****************************************************************************/
/* this function is called for a unit to perform rest action */
int rest_unit(Unit * unit){
	if(unit->action[2]) {
		printf("other actions taken, cannot rest\n");
		enter_to_continue(); return 0;
	}else{
		unit->hitpoint += 2;
		unit->action[0] = 1;
		unit->action[1] = 1;
		unit->action[2] = 1;
		if(unit->hitpoint>unit->lifebar) unit->hitpoint = unit->lifebar;
		printf("Rested and hitpoints recovered\n"); 
		enter_to_continue(); return 1;
	}
}

/****************************************************************************/
/* this function add the hostile unit to the player's list */
void addhostile(Unit *unit, Player * p1){
	int i, has=0;
	for(i = 0; i < p1->no_of_hostile; i++){
		if(unit==p1->hostile_spotted[i]) has = 1;
	}
	if(!has){
		p1->hostile_spotted[p1->no_of_hostile] = unit;
		p1->no_of_hostile += 1;
	}
}

/****************************************************************************/
/* this function decides if the attack option is possible for the unit */
int find_attackable(Unit * unit, Player * p1, Unit ** atk_list){
	int x, y, i = 0; 
	Unit * temp;
	for(y= unit->y - unit->attackrange; y <= unit->y + unit->attackrange; y++){
		for(x= unit->x - unit->attackrange; x <= unit->x + unit->attackrange; x++){
			if((temp = locate_unit(x, y, p1->hostile_spotted,
													p1->no_of_hostile))){
				atk_list[i++] = temp;
			}
		}
	}
	atk_list[i] = 0;
	return !atk_list[0];
}

/****************************************************************************/
/* this function finds and print all available action of the unit */
int available_action(Unit * unit, Player * p1, char ** commands){
	printf("%s%d selected at (%d,%d)\n", unit->unit_type, unit->unit_id, 
		unit->x+1,unit->y+1);
	if(unit->action[0]&&unit->action[1]&&unit->action[2]){
		printf("Out of actions!!!\n");
		return 0;
	}
	if(!*commands) printf("\nWhat needs to be done? (key in the action):");
	if(!unit->action[0] && !*commands){
		printf("\n\tmove\t(%d movement points left)", unit->movement);
	}
	if(!unit->action[1] && !*commands){
		Unit ** atk_list = (Unit**) malloc (ARMYSIZE*sizeof(Unit*)); int i;
		if(!find_attackable(unit, p1, atk_list)){
			printf("\n\tattack\t{");
			for(i = 0; atk_list[i]; i++){
				printf(" %d%c%d ", atk_list[i]->player_id, atk_list[i]->unit_symbol, atk_list[i]->unit_id);
			}
			printf("}");
		}
	}
	if(!unit->action[2] && !*commands){
		printf("\n\trest\t(regenerate 1 hp)");
	}
	if(!*commands) printf("\n\t%-10sTo dis-select current unit\n", "back");
	return 1;
}

/****************************************************************************/
/* this function construct sight-range of player with all its units' sight */
void buildsight(int **xmins, int **ymins, int **xmaxs, int **ymaxs, Player * player){
	// build the sight range for a player with all the owned units
	int i, j = 0, units_alive = player->units_alive;
	Unit ** unit_list = player->unit_list;
	*xmins = (int*) malloc ((units_alive+1) * sizeof(int));
	*ymins = (int*) malloc ((units_alive+1) * sizeof(int));
	*xmaxs = (int*) malloc ((units_alive+1) * sizeof(int));
	*ymaxs = (int*) malloc ((units_alive+1) * sizeof(int));
	for(i = 0; i < player->no_of_units; i++){
		if(*(unit_list+i)){
			*(*xmins+j)=(*(unit_list+i))->x - (*(unit_list+i))->sight;
			*(*xmaxs+j)=(*(unit_list+i))->x + (*(unit_list+i))->sight;
			*(*ymins+j)=(*(unit_list+i))->y - (*(unit_list+i))->sight;
			*(*ymaxs+j)=(*(unit_list+i))->y + (*(unit_list+i))->sight; j++;
		}
	}
	*(*xmins+j)=player->x - player->sight;	// vision from player's base
	*(*xmaxs+j)=player->x + player->sight;
	*(*ymins+j)=player->y - player->sight;
	*(*ymaxs+j)=player->y + player->sight;
}

/****************************************************************************/
/* this function check if the pixel is in any unit's sight range */
int checksight(int *xmins, int *ymins, int *xmaxs, int *ymaxs,
			   int x, int y, int no_of_units){
	int i;
	for(i = 0; i < no_of_units; i++){
		if(x>=xmins[i]&&y>=ymins[i]&&x<=xmaxs[i]&&y<=ymaxs[i]){
			return 1;
		}
	}
	return 0;
}
