// this module defines other game fundamental functions module

/****************************************************************************/
/* this function check for commands of the player */
int checkcommand(char ** commands, Player * p1, Player * p2){
	if(strcmp(commands[0], "quit")==0 || strcmp(commands[0], "end")==0){
		return 1;
	}else if(strcmp(commands[0], "recruit")==0){
		Unit * unit = recruit(p1, commands);
		if(unit) printf("%s %d%c%d recruited\n", unit->unit_type,
			p1->player_id, unit->unit_symbol, unit->unit_id);
		return 3;
	}else if(strcmp(commands[0], "back")==0){
		return 2;
	}else if(strcmp(commands[0], "report")==0){
		report(p1); return 3;
	}else if(strcmp(commands[0], "help")==0){
		helpmanual(); return 3;
	}else if(strcmp(commands[0], "get")==0){
		getmap(p1, p2); return 3;
	}
	return 0;
}

/****************************************************************************/
/* this funciton restores unit's action and movement points for the player */
void pre_turn(Player *p1){
	int i;
	p1->resource += 2;	// resource calculation
	for(i = 0; i < p1->no_of_expansion; i++){
		if(p1->expansion_list[i]){
			p1->resource += (p1->expansion_list[i])->income;
		}
	}
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
/* a turn for one player */
int turn(Player * p1, Player * p2){
	int size = 10, i=0; Unit * unit;
	char * action, * raw_user_input, ** commands;
	pre_turn(p1); getmap(p1, p2); report(p1);
	
	printf("\nPlayer %d, take your turn\tgold: %d\n", p1->player_id, p1->resource);
	printf("key in \"help\" to see a list of useful command\n");
	while(1){
		getcommand(&raw_user_input, 3*size);
		commands = parsecommand(raw_user_input, 3);
		if((i=checkcommand(commands, p1, p2))==1) break;
		else if(i == 3) continue;
		
		/* select a valid unit */
		i = atoi(commands[0])==p1->player_id && isdigit(commands[0][strlen(commands[0])-1]);
		if(i && atoi(commands[0]+strlen(commands[0])-1) <= p1->no_of_units){
			unit = p1->unit_list[atoi(commands[0]+strlen(commands[0])-1)-1];
		}else{
			printf("Invalid troop selected!\n"); continue;
		}
		
		/* perform a valid action */
		while(available_action(unit, p1, commands+1)){
			if(!commands[1]){ // towards a valid target
				getcommand(&action, 2*size); commands = parsecommand(action, 2);
				commands[2] = commands[1];
				commands[1] = commands[0];
			}
			if(strcmp(commands[1], "move")==0){
				while((i = move_unit(unit, commands[2], p1, p2))==2){
					commands[2] = 0;
				}
				if(i == 1){	// new vision from movement
					printf("New sight gained!\n");
					enter_to_continue(); getmap(p1, p2);
				}
				commands[1] = 0;			// action complete
			}else if(strcmp(commands[1], "attack")==0){
				if(attack_unit(unit, p1, p2, commands[2])==10) return 10;
				commands[1] = 0; commands[2] = 0;		// action complete
			}else if(strcmp(commands[1], "rest")==0){
				rest_unit(unit); commands[1] = 0;	// action complete
			}else{
				commands[1] = 0;	// invalid action ignored
				if((i = checkcommand(commands, p1, p2))==1) break;
				else if(i == 3) continue;	// report command
				else if(i == 2) break; printf("invalid action!\n"); 
			}
		}
	}
	return 0;
}