/*
MultiFactions
New Features:
1. income and recruit system

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
#define ARMYSIZE 9

/* program dependency */
#include "helpful.h"			// functions to reduce program redundancy */
#include "commandline.h"	// interaction with player
#include "mystructs.h"		// struct and typedef
#include "units.h"			// units related function
#include "terrain.h"		// terrain system
#include "player.h"			// player functions
#include "fundamental.h"	// other basic game functions

int main(){	
	/* brief the players */
	printf("\nWelcome to Faction War 2.0 alpha version, developed by Cao Peng, Oct 2016.\n\n");
	printf("Note: in this version\n1. Terrains has no effects except for the visual differences in map\n");
	printf("2. No zone of control which means the all units can move in and out of any unoccupied spot\n\n");
	enter_to_continue();
	printf("Do you want to start a 1v1 game?\n");
	enter_to_continue();
	printf("Are you sure? you need exactly 2 players to play the game! unless you can play it left brain vs right brain\n");
	enter_to_continue();
	printf("Okay, Let's go!\n");
	printf("Generating game...\n\n");
	Player p1 = createplayer(1, '&', 2, 2);
	
	// Unit *archer1 = Archer(0, 0, &p1);	/* testing units */
	// player_recruit(&p1, archer1);
	// Unit *swordman1 = Swordman(0,0, &p1);
	// player_recruit(&p1, swordman1);
	// Unit *horseman1 = Horseman(0,0, &p1);
	// player_recruit(&p1, horseman1);
	
	Player p2 = createplayer(2, '&', 19, 19);
	// Unit *archer2 = Archer(0, 0, &p2);	/* testing units */
	// player_recruit(&p2, archer2);
	// Unit *swordman2 = Swordman(0,0, &p2);
	// player_recruit(&p2, swordman2);
	// Unit *horseman2 = Horseman(0,0, &p2);
	// player_recruit(&p2, horseman2);
	
	printf("Mission: find and destroy all enemy units\n\n");
	enter_to_continue();
	while(1){
		if(turn(&p1, &p2)==10){
			printf("\n/****************/\nVictor is player %d!\n/****************/\n", p1.player_id);
			break;
		}
		if(turn(&p2, &p1)==10){
			printf("\n/****************/\nVictor is player %d!\n/****************/\n", p2.player_id);
			break;
		}
		
	}
	return 0;
}
