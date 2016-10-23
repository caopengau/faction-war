// this module defines commandline interaction with player

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
/* this funciton prints the available commands a player can call */
void helpmanual(){
	printf("\n"); fillwith(MAPSIZE*PIXEL_SIZE, '+'); printf("\n\n");
	printf("\tselect a soldier/target by its full name appeared on the board\n\n");
	printf("\t%-10sTo recruit a troop\n\n", "recruit");
	printf("\t%-10sTo get this help manual for available command turn\n\n", "help");
	printf("\t%-10sTo get latest status of the battle\n\n", "get");
	printf("\t%-10sTo end your turn\n\n", "end/quit");
	printf("\t%-10sTo dis-select the current unit\n\n", "back");
	printf("\t%-10sTo view the current status of all visible units\n\n", "report");
	printf("\n"); fillwith(MAPSIZE*PIXEL_SIZE, '+'); printf("\n\n");
}
