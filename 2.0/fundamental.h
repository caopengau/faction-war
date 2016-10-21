// game fundamental functions module

/****************************************************************************/
/* obvious */
void any_key_to_continue(){
	printf("press enter to continue...\n");
	getchar();
}

/****************************************************************************/
/* this function gets raw input command string from the player */
int getcommand(char ** entry, int buffer){
	int i, c;
	char *command = (char*) malloc (buffer * sizeof(char));
	*entry = command;
	printf(">");
	for(i = 0; i < buffer-1; i++){
		c = getchar();
		if(c=='\n'){
			command[i] = 0; return i;
		}
		command[i] = c;
	}
	command[i] = 0;
	return buffer;
}

/****************************************************************************/
/* this function will get two int from the player */
int get2numbers(int *x, int *y){
	int i=0, j=0;
	char c, temp[2][5];
	memset(temp[i], 0, 10);
	while((c=getchar())!='\n'){
		if(isdigit(c)){
			temp[i][j] = c;
			j++;
			if(j==4) break;
		}
		else if(j){
			temp[i][j] = 0;
			i++; j= 0;
			if(i==2) break;
		}
	}
	temp[i][j] = 0;
	if((*x = atoi(temp[0])) && (*y = atoi(temp[1]))){
		return 1;	// two valid numbers
	}
	return 0;	// invalid number(s)
}

/****************************************************************************/
/* this function parse the raw command into maximum of 3 parts:
	1.unit 2.action 3.target(location)
*/
char ** parsecommand(char * command, int n_sub){
	int i=0, j=0, k = 0, n, allnum = 1;
	char ** commands;
	commands = (char **) malloc (3 * sizeof(char *));
	commands[0] = (char *) malloc (20 * sizeof(char));
	commands[1] = (char *) malloc (20 * sizeof(char));
	commands[2] = (char *) malloc (20 * sizeof(char));
	for(i = 0; command[i] && j < 20 && k < n_sub; i++){
		if(command[i]==' ' && k < n_sub-1){
			commands[k][j] = 0; 
			for(n = 0; n < j; n++){
				if(!isdigit(commands[k][n])){allnum = 0;}
			}
			if(allnum){ k++; continue;}	// just a x coordinate, so go for y.
			j = 0; k++; continue;
		}
		commands[k][j] = command[i];
		j++; 
	}
	commands[k][j] = 0;
	if(k<2) commands[k+1] = 0;
	return commands;
}

/****************************************************************************/
/* this function convert 2 string-form int to 2 int */
int convert2numbers(char * command, int * x, int * y){
	char temp[5];
	int i, j = 0;
	for(i = 0; command[i]; i++){
		if(!isdigit(command[i])){
			temp[i] = 0;
			*x = atoi(temp); j = 0; continue;
		}
		temp[j] = command[i]; j++;
	}
	temp[j] = 0; *y = atoi(temp);
	return 1;
}

/****************************************************************************/
/* this funciton prints the available commands a player can call */
void helpmanual(){
	printf("\t/**********************************************************/\n");
	printf("\tselect a soldier/target by its full name appeared on the board\n");
	printf("\t%-10sTo get this help manual for available command turn\n", "help");
	printf("\t%-10sTo get latest status of the battle\n", "get");
	printf("\t%-10sTo end your turn\n", "end/quit");
	printf("\t%-10sTo dis-select the current unit\n", "back");
	printf("\t%-10sTo view the current status of all visible units\n", "report");
	printf("\t/**********************************************************/\n");
}

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
#define TMAP create_tmap()	// terrain info of all pixel in map
#define TARRAY init_terrain() // types of terrain used in map generation
/****************************************************************************/
/* this function finds the unit occuping the cordinate (x,y)*/
Unit* locate_unit(int x, int y, Player * player){
	int i;
	Unit** unit_list = player->unit_list;
	for(i = 0; i < player->no_of_units; i++){
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
			unfit = locate_unit(x, y, player) || x>=MAPSIZE || y>=MAPSIZE ||
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
	archer->lifebar = 15;
	archer->hitpoint = 15;
	archer->attack = 10;
	archer->attackrange = 5;
	archer->defense = 1;
	archer->movementbar = 4;
	archer->movement = 4;
	archer->sight = 5;
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
	swordman->lifebar = 25;
	swordman->hitpoint = 25;
	swordman->attack = 7;
	swordman->attackrange = 1;
	swordman->defense = 5;
	swordman->movementbar = 5;
	swordman->movement = 5;
	swordman->sight = 4;
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
	horseman->attack = 8;
	horseman->attackrange = 2;
	horseman->defense = 3;
	horseman->movementbar = 8;
	horseman->movement = 8;
	horseman->sight = 8;
	if(x&&y){
		horseman->x = x-1;
		horseman->y = y-1;
	}else{
		recruitspot(horseman, player);
	}
	return horseman;
}

/****************************************************************************/
/* this function create a player 
	arg1: the id for new added player
	arg2,3: the xlocation,ylocation for its base
	arg4: the initial maximum size for its army
*/
Player createplayer(int player_id, char sym_base, int x, int y, int armysize){
	Player * player;
	player = (Player *) malloc (1 * sizeof(Player));
	player->player_id = player_id;
	player->base = sym_base;
	player->x = x-1; player->y = y-1;
	player->sight = 4;
	player->unit_list = (Unit**)malloc(armysize*sizeof(Unit *));
	player->hostile_spotted = (Unit**)malloc(armysize*sizeof(Unit *));
	player->no_of_units = 0;
	player->units_alive = 0;
	player->no_of_hostile = 0;
	return *player;
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
/* this function print n times character c */
void fillwith(int n, char c){
	int i;
	for(i = 0; i < n; i++){
		printf("%c", c);
	}
}

/****************************************************************************/
/* print [|||||  ] as the current/max, total being the entire length */
void printbar(int max, int current, int total){
	printf(" [");
	fillwith((total-2)*current/max, 219);		// 124 = bar
	fillwith(total - (total-2)*current/max-2, ' ');
	printf("]");
}

/****************************************************************************/
/* this funciton prints all visible units' stats for the player */
void report(Player * p1){
	int i;
	printf("\n%5s%15s%8s%10s%10s%10s%10s%8s\n", "Side", "UnitType", 
			"Unitid", "Hitpoint", "Attack", "Defense", "Movement", "x,y");
	Unit * unit;
	for(i=0; i < p1->no_of_units; i++){	// p1's units
		if(p1->unit_list[i]){ 
			unit = p1->unit_list[i];
			printf("%5d%12s(%c)%8d%10d%10d%10d%10d%5d,%2d\n", unit->player_id, 
				unit->unit_type, unit->unit_symbol, unit->unit_id,
				unit->hitpoint, unit->attack, unit->defense, 
				unit->movement, unit->x+1, unit->y+1);
		}
	}
	for(i=0; i < p1->no_of_hostile; i++){	// units hostile to p1
		if(p1->hostile_spotted[i]){
			unit = p1->hostile_spotted[i];
			printf("%5d%12s(%c)%8d%10d%10d%10d%10d%5d,%2d\n", unit->player_id, 
				unit->unit_type, unit->unit_symbol, unit->unit_id,
				unit->hitpoint, unit->attack, unit->defense, 
				unit->movement, unit->x+1, unit->y+1);
		}
	}
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
/****************************************************************************/
/* this function construct sight-range of player with all its units' sight */
void buildsight(int **xmins, int **ymins, int **xmaxs, int **ymaxs, Player * player){
	// build the sight range for a player with all the owned units
	int i, no_of_units = player->no_of_units;
	Unit ** unit_list = player->unit_list;
	*xmins = (int*) malloc ((no_of_units+1) * sizeof(int));
	*ymins = (int*) malloc ((no_of_units+1) * sizeof(int));
	*xmaxs = (int*) malloc ((no_of_units+1) * sizeof(int));
	*ymaxs = (int*) malloc ((no_of_units+1) * sizeof(int));
	for(i = 0; i < no_of_units; i++){
		*(*xmins+i)=(*(unit_list+i))->x - (*(unit_list+i))->sight;
		*(*xmaxs+i)=(*(unit_list+i))->x + (*(unit_list+i))->sight;
		*(*ymins+i)=(*(unit_list+i))->y - (*(unit_list+i))->sight;
		*(*ymaxs+i)=(*(unit_list+i))->y + (*(unit_list+i))->sight;
	}
	*(*xmins+i)=player->x - player->sight;	// vision from player's base
	*(*xmaxs+i)=player->x + player->sight;
	*(*ymins+i)=player->y - player->sight;
	*(*ymaxs+i)=player->y + player->sight;
}

/****************************************************************************/
/* this function print the cordinate axis label */
void printaxis(){
	int i;
	fillwith(PIXEL_SIZE+1, ' ');
	for(i = 0; i < MAPSIZE; i++){
		printf("%*d", PIXEL_SIZE, i+1);
	}
}

/****************************************************************************/
void print_border(){
	fillwith(PIXEL_SIZE+1, ' ');
	fillwith(MAPSIZE*PIXEL_SIZE, '-');
}

/****************************************************************************/
/* this function print the object that occupy the cordinate */
void printobj(Player * player, Unit * unit){
	if(unit) printf("%d%c%d", unit->player_id, unit->unit_symbol, unit->unit_id);
	else if(player){
		printf("%d%c%c", player->player_id, player->base, player->base);
	}
	fillwith(PIXEL_SIZE-3, ' ');
}

/****************************************************************************/
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
									x, y, no_of_units);
			if(!WARFOGENABLED) in_sight = 1;
			if(in_sight){
				if((found_unit = locate_unit(x, y, p1))){	// friendly units
					printobj(p1, found_unit);
				}else if((found_unit = locate_unit(x, y, p2))){	// enemy units
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
		printf("\n");
	}
	// map bottom axis label
	printaxis(); printf("\n");
}

/****************************************************************************/
/* this function check for commands of the player */
int checkcommand(char ** command, Player * p1, Player * p2){
	if(strcmp(command[0], "quit")==0 || strcmp(command[0], "end")==0){
		return 1;
	}else if(strcmp(command[0], "back")==0){
		return 2;
	}else if(strcmp(command[0], "report")==0){
		report(p1); return 3;
	}else if(strcmp(command[0], "help")==0){
		helpmanual(); return 0;
	}else if(strcmp(command[0], "get")==0){
		getmap(p1, p2); return 3;
	}
	return 0;
}

/****************************************************************************/
/* this function will remove the hostile unit if it is killed */
void killhostile(Player * p1, Unit * unit){
	int i;
	for(i = 0; i < p1->no_of_hostile; i++){
		if(unit==p1->hostile_spotted[i]) p1->hostile_spotted[i] = 0;
	}
}

/****************************************************************************/
/* this funciton restores unit's action and movement points for the player */
void pre_turn(Player *p1){
	int i;
	Unit ** unit_list = p1->unit_list;
	p1->no_of_hostile = 0;	// no hostile units seen
	for(i = 0; i < p1->no_of_units; i++){	// restore all action points for units
		if(unit_list[i]){
			memset(unit_list[i]->action, 0, 3);
			unit_list[i]->movement = unit_list[i]->movementbar;
		}
	}
}

/****************************************************************************/
/* this function finds and print all available action of the unit */
int available_action(Unit * unit, Player * p1){
	printf("\n%s%d selected at (%d,%d)\n", unit->unit_type, unit->unit_id, 
		unit->x+1,unit->y+1);
	if(unit->action[0]&&unit->action[1]&&unit->action[2]){
		printf("Out of actions!!!\n");
		return 0;
	}
	printf("What needs to be done? (key in the action):\n");
	if(!unit->action[0]){
		printf("\tmove\t(%d movement points left)\n", unit->movement);
	}
	if(!unit->action[1] && p1->no_of_hostile){
		printf("\tattack\n");
	}
	if(!unit->action[2]){
		printf("\trest\t(regenerate 1 hp)\n");
	}
	printf("\t%-10sTo dis-select current unit\n", "back");
	return 1;
}
/****************************************************************************/
/* this function is called for a unit to perform move action */
int move_unit(Unit * unit, char *command){
	if(unit->action[0]) {
		printf("no movement point available\n");
		any_key_to_continue(); return 0; // can't move
	}
	int x, y, temp, guard;	// guard for 2 valid int input from the player
	if(!command){
		printf("Where do u want to move into? x y\n>");
		guard = get2numbers(&x, &y);
	}else{
		guard = convert2numbers(command, &x, &y);
	}
	
	if(guard){
		temp = x>0 && x <=MAPSIZE && y>0 && y<=MAPSIZE && 
			abs(x-1-unit->x) <= unit->movement &&
			abs(y-1-unit->y) <= unit->movement;
		if(temp){
			(unit->action)[2] = 1;
			unit->movement -= abs(x-1-unit->x) > abs(y-1-unit->y)? abs(x-1-unit->x): abs(y-1-unit->y);
			unit->x = x-1; unit->y = y-1;
			printf("moved into (%d, %d)\n", unit->x+1, unit->y+1);
			if(unit->movement <= 0) unit->action[0] = 1;
			return 1;	// move successful
		}else{
			printf("move failed, target location not in reach\n");
			any_key_to_continue();
			return 2;	// can move, but the location is not reachable
		}
	}else{
		printf("Invalid target location!\n"); any_key_to_continue();
	}
	return 1;	// Invalid target need to ask for new command
	
}

/****************************************************************************/
/* this function is called for a unit to perform attack */
int attack_unit(Unit * unit, Player * p1, Player * p2, char * hostile){
	if(unit->action[1]) {
		printf("no attack point available!\n"); 
		any_key_to_continue(); return 0;
	}
	if(!p1->no_of_hostile){
		printf("no enemy spotted!\n");
		any_key_to_continue(); return 0;
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
		hostile = 0; any_key_to_continue(); return 0;
	}
	Unit * target = (p2->unit_list)[unit_id-1];
	int range = unit->attackrange;
	if(abs(unit->x - target->x) <= range && abs(unit->y - target->y) <= range){
		printf("attack successful!\n");
		int damage = unit->attack - target->defense;
		target->hitpoint -= damage;
		printf("%d damage dealt\n", damage); any_key_to_continue();
		if(target->hitpoint <= 0){
			killhostile(p1, target);	// remove from hostile spotted list
			free(target);
			p2->unit_list[unit_id-1] = 0;
			printf("target killed!\n");
			p2->units_alive -= 1;
			if(p2->units_alive == 0){
				return 1;
			}
			any_key_to_continue(); report(p1); return 0;
		}
		unit->action[0] = 1; unit->action[1] = 1; unit->action[2] = 1;
		report(p1); return 0;
	}
	printf("attack failed! target not in range\n");
	any_key_to_continue(); return 0;
}

/****************************************************************************/
/* this function is called for a unit to perform rest action */
int rest_unit(Unit * unit){
	if(unit->action[2]) {
		printf("other actions taken, cannot rest\n");
		any_key_to_continue(); return 0;
	}else{
		unit->hitpoint += 2;
		unit->action[0] = 1;
		unit->action[1] = 1;
		unit->action[2] = 1;
		if(unit->hitpoint>unit->lifebar) unit->hitpoint = unit->lifebar;
		printf("Rested and hitpoints recovered\n"); 
		any_key_to_continue(); return 1;
	}
}
