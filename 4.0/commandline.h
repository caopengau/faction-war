// this module defines commandline interaction with player

/****************************************************************************/
/* this function gets raw input command string from the player */
int getcommand(char ** entry, int buffer){
	int i, c;
	char *command = (char*) malloc (buffer * sizeof(char));
	*entry = command;
	printf(">");
	for(i = 0; i < buffer-1; i++){
		c = tolower(getchar());
		if(c=='\n'){
			command[i] = 0; return i;
		}
		command[i] = c;
	}
	command[i] = 0;
	return buffer;
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

