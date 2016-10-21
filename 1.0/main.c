/* Faction War 1.0 - the prototype of a turned based game
Features:
1. a map of reasonable sizeo
2. 2 players each controling a hero
3. Turned system

By Cao Peng
28 Aug 2016
*/ 

#include <stdio.h>
#include <stdlib.h>
#include "units.h"

#define OBJ1 "P1"
#define OBJ2 "P2"

typedef struct character Character;
void getmap(Character * current_player, Character * oppo_player);
void turn(Character *current_player, int choice, Character *oppo_player);

#define MAPSIZE 20
#define MAPFILLS "_ "	// materials for map background

int main(){
	////////////////////////// Greetings to Players ////////////////////
	printf("\nWelcome to Faction War 1.0, developed by Cao Peng.\n");
	printf("Your in-game objective is to eliminate all opposing unit to win!\n ");
	printf("\nDo u want to start a 1v1 game? Enter 'c' to continue...\n> ");
	char c;
	while((c=getchar())!='c'){}
	printf("\nAre u sure? u need exactly 2 players to play the game!\n");
	printf("Enter 'c' to continue...\n> ");
	while((c=getchar())!='c'){}
	
	Character p1 = character_default;
	Character p2 = character_default;
	Character * pp1=&p1;
	Character * pp2=&p2;
	p2.Position[0]=20;
	p2.Position[1]=20;
	p1.OBJ[0] = p2.OBJ[0] = 'P';
	p1.OBJ[1] = '1'; p2.OBJ[1] = '2';
	Archer a1 = archer_default;
	Archer a2 = archer_default;
	a1.OBJ[1] = '1';
	a2.OBJ[1] = '2';
	a2.Position[0] = 19; a2.Position[1] = 20;
	p1.unit[0] = & a1;
	p2.unit[0] = & a2;
	
	
	printf("\nPlayer\tHP\tMoves\tAttack\tAtkRange\tPosition\n");
	printf("%s\t%d\t%d\t%d\t%d\t%d\t%d\t%s\n", p1.OBJ, p1.HP,p1.Moves,p1.Attack,p1.Attack_Range, p1.Position[0],p1.Position[1], p1.State? "Alive" : "Dead");
	printf("%s\t%d\t%d\t%d\t%d\t%d\t%d\t%s\n", p2.OBJ, p2.HP,p2.Moves,p2.Attack,p2.Attack_Range, p2.Position[0],p2.Position[1], p2.State? "Alive" : "Dead");
	
	printf("Initializing Environment:\n\n");
	getmap(pp1, pp2);
	printf("Game begin!\n");
	int whosturn=1;

	// game starts...
	while(p1.HP>0 && p2.HP>0){
		printf("===================================================================\
		\nTurn %d\t", (whosturn+1)/2);
		int choice=2;
		
		if(whosturn%2){
			printf("%s's turn\n", p1.OBJ);
			printf("%s: What's the order, commander? (0 to move; 1 to attack; 2 to rest)\n> ", p1.OBJ);
			if(scanf("%d",&choice));
			printf("Your choice is %d\n", choice);
			turn(pp1,choice,pp2);
			
			char c;
			printf("Enter 'c' to confirm...\n > ");
			while((c=getchar())!='c'){}
			
			getmap(pp1, pp2);

		}else{
			printf("%s's turn\n", p2.OBJ);
			printf("%s: What's the order, commander? (0 to move; 1 to attack; 2 to rest)\n> ", p2.OBJ);
			if(scanf("%d",&choice));
			printf("Your choice is %d\n", choice);
			turn(pp2,choice,pp1);
			
			char c;
			printf("Enter 'c' to confirm...\n > ");
			while((c=getchar())!='c'){}
			
			getmap(pp2, pp1);
		}
		
		printf("\nCurrent stats report:\n");
		printf("Player\tHP\tMoves\tAttack\tRange\tPosition(x,y)\tState\n");
	printf("%s\t%d\t%d\t%d\t%d\t(%d,%d)\t\t%s\n", p1.OBJ, p1.HP,p1.Moves,p1.Attack,p1.Attack_Range, p1.Position[0],p1.Position[1], p1.State? "Alive" : "Dead");
	printf("%s\t%d\t%d\t%d\t%d\t(%d,%d)\t\t%s\n", p2.OBJ, p2.HP,p2.Moves,p2.Attack,p2.Attack_Range, p2.Position[0],p2.Position[1], p2.State? "Alive" : "Dead");
	
		whosturn++;
	}
	if(p1.HP<=0){
		printf("\nCongratulations to player %s, you have won!!!\n", p2.OBJ);
	}else{
		printf("\nCongratulations to player %s, you have won!!!\n", p1.OBJ);
	}
	return 1;

}

void getmap(Character * current_player, Character * oppo_player){
	int p1[]={current_player->Position[0],current_player->Position[1]};
	int p2[]={oppo_player->Position[0],oppo_player->Position[1]};
	int i=0;	// horizontal coordinate indicator
	int n_space=sizeof(MAPFILLS);
	
	printf("%.*s",n_space,"                                 ");
	for(i=1;i<MAPSIZE+1;i++){
		printf("%2d ", i);
	}
	printf("\n%.*s",n_space,"                                 ");
	for(i=0;i<MAPSIZE*3;i++){
		printf("_");
	}
	printf("\n");
	
	
	// vertical coordinate indicator
	for(i=1;i<MAPSIZE+1;i++){
		printf("%2d|",i);
		
		int j=0;	// print p1 or p2 's position;
		for(j=1;j<MAPSIZE+1;j++){
			
			if(p1[0]==i&&p1[1]==j){
				printf("%s ",current_player->OBJ);
			}else if(p2[0]==i&&p2[1]==j){
				printf("%s ",oppo_player->OBJ);
			}else if(current_player->unit[0]->Position[0] == i && current_player->unit[0]->Position[1] == j){
				printf("%s ",current_player->unit[0]->OBJ);
			}else if(oppo_player->unit[0]->Position[0] == i && oppo_player->unit[0]->Position[1] == j){
				printf("%s ",oppo_player->unit[0]->OBJ);
			}
			else{
				printf("%3s",MAPFILLS);
			}
		}
		
		printf("|%2d\n",i);
	}
	printf("%.*s",n_space,"                                 ");
	for(i=0;i<MAPSIZE*3;i++){
		printf("-");
	}
	printf("\n");
	
	printf("%.*s",n_space,"                                 ");
	for(i=1;i<MAPSIZE+1;i++){
		printf("%2d ", i);
	}
	printf("\n");
	
}

void turn(Character * current_player, int choice, Character * oppo_player){
	int x_c=current_player->Position[0];
	int y_c=current_player->Position[1];
	int x_o=oppo_player->Position[0];
	int y_o=oppo_player->Position[1];
	int row;	// move target x-coordinate
	int col;	// ...y...
	
	switch(choice){
		case 0:	// Move
			printf("Where do u want to move into? (row [space] column)\n> ");
			if(scanf("%d %d", &row, &col)==2){
				printf("Moving into (X,Y) = (%d, %d) ?\n", row, col);
			}
			
			if((abs(row-x_c)<=5&&abs(col-y_c)<=5)&& !(row==x_o&&col==y_o)){
				current_player->Position[0]=row;
				current_player->Position[1]=col;
				
			}else{
				printf("Move failed\n");
			}
			
			break;
		
		case 1:	// Attack

			if(abs(x_c-x_o)<=(current_player->Attack_Range) && abs(y_c-y_o)<=(current_player->Attack_Range)){
				printf("Attack successfull\n");
				oppo_player->HP-=current_player->Attack;
				
			}else{
				printf("Attack failed\n");
				printf("Target not in attack range. Your attack range is %d\n",current_player->Attack_Range);
			}
			break;
			
		default:	// Rest
			printf("You rested\n\n");
			break;
	}
}
