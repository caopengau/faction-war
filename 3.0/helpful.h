/* this module defines helpful functions to reduce program redundancy */

/****************************************************************************/
/* obvious functionality suggested by its name */
void enter_to_continue(){
	printf("press enter to continue..."); getchar();
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
/* print [|||||  ] as the current/max, total being the entire length */
void printbar(int max, int current, int total){
	printf(" [");
	fillwith((total-2)*current/max, 219);		// 124 = bar
	fillwith(total - (total-2)*current/max-2, ' ');
	printf("]");
}

/****************************************************************************/
/* this function print the cordinate axis label */
void printaxis(){
	int i;
	fillwith(PIXEL_SIZE+1, ' ');
	for(i = 0; i < MAPSIZE; i++){
		fillwith(PIXEL_SIZE/2-1, ' ');
		printf("%*d", PIXEL_SIZE-(PIXEL_SIZE/2-1)*2, i+1);
		fillwith(PIXEL_SIZE/2-1, ' ');
		
	}
}

/****************************************************************************/
/* this function prints the horizontal border for the map */
void print_border(){
	fillwith(PIXEL_SIZE+1, ' '); fillwith(MAPSIZE*PIXEL_SIZE, '-');
}