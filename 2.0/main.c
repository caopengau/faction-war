/*
MultiFactions
Features:
1. command system for players
2. units/troops controlled by players
3. customizable map size and terrains(no effect on stats yet)

By Cao Peng, Oct 2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>	// for srand() a seed to rand()
#include <unistd.h>	// for sleep()

/* customized setting */
#define MAPSIZE 20
#define PIXEL_SIZE 4
#define MAPFOGMATERIAL '#'
#define WARFOGENABLED 1
#define ENEMYSYMBOL '<'
#define TERRAIN_VARIETY 3	// maximum 2 in this game version

/* program dependency */
#include "units.h"	// units, player ... struct definition
#include "fundamental.h"	// basic game functions and generation

void player_recruit(Player * player, Unit * unit);
int turn(Player * p1, Player * p2);

int main(){	
	printf("\nWelcome to Faction War 2.0 alpha version, developed by Cao Peng, Oct 2016.\n\n");
	printf("Generating game...\n\n");
	Player p1 = createplayer(1, '&', 2, 2, 9);
	Unit *archer1 = Archer(0, 0, &p1);
	player_recruit(&p1, archer1);
	Unit *swordman1 = Swordman(0,0, &p1);
	player_recruit(&p1, swordman1);
	Unit *horseman1 = Horseman(0,0, &p1);
	player_recruit(&p1, horseman1);
	
	Player p2 = createplayer(2, '&', 19, 19, 9);
	Unit *archer2 = Archer(0, 0, &p2);
	player_recruit(&p2, archer2);
	Unit *swordman2 = Swordman(0,0, &p2);
	player_recruit(&p2, swordman2);
	Unit *horseman2 = Horseman(0,0, &p2);
	player_recruit(&p2, horseman2);
	
	/* perspective from player1 */
	printf("Mission: find and destroy all enemy units\n\n");
	any_key_to_continue();
	while(1){
		if(turn(&p1, &p2)==10){
			printf("/****************/\nVictor is player %d!\n/****************/\n", p1.player_id);
			break;
		}
		if(turn(&p2, &p1)==10){
			printf("/****************/\nVictor is player %d!\n/****************/\n", p2.player_id);
			break;
		}
		
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
/* a turn for one player */
int turn(Player * p1, Player * p2){
	int size = 10, i=0;
	Unit * unit;
	char * action, * raw_user_input, ** commands;
	pre_turn(p1);
	getmap(p1, p2);
	report(p1);
	printf("\nPlayer %d, take your turn\n", p1->player_id);
	printf("key in \"help\" to see a list of useful command\n");
	while(1){
		printf("Select a troop!\n");
		getcommand(&raw_user_input, 3*size);
		commands = parsecommand(raw_user_input, 3);
		if((i=checkcommand(commands, p1, p2))==1) break;
		else if(i == 3) continue;
		
		/* unit */
		if(atoi(commands[0])==p1->player_id && atoi(commands[0]+strlen(commands[0])-1) <= p1->no_of_units){
			unit = p1->unit_list[atoi(commands[0]+strlen(commands[0])-1)-1];
		}else{
			printf("Invalid troop selected!\n"); continue;
		}
		
		/* action */
		while(available_action(unit, p1)){
			if(!commands[1]){
				getcommand(&action, 2*size); commands = parsecommand(action, 2);
				commands[2] = commands[1];	// target
				commands[1] = commands[0];	// action if not empty
			}
			if(strcmp(commands[1], "move")==0){
				while((i = move_unit(unit, commands[2]))==2){
					commands[2] = 0;
				}
				if(i == 1) getmap(p1, p2);	// new vision from movement
				commands[1] = 0;			// action complete
			}else if(strcmp(commands[1], "attack")==0){
				if(attack_unit(unit, p1, p2, commands[2])) return 10;
				commands[1] = 0; commands[2] = 0;		// action complete
			}else if(strcmp(commands[1], "rest")==0){
				rest_unit(unit); commands[1] = 0;	// action complete
			}else{
				commands[1] = 0;	// invalid action ignored
				if((i = checkcommand(commands, p1, p2))==1) break;
				else if(i == 3) continue;	// report command
				else if(i == 2) break;
				printf("invalid action!\n"); 
			}
		}
	}
	return 0;
}
