struct character{
	char OBJ[2];
	int HP;
	int Moves;
	int Attack;
	int Attack_Range;
	int Position[2];
	struct archer *unit[5];
	int State;
} character_default = {"", 10, 5, 2, 1, {1,1}, {}, 1};

struct archer{
	char OBJ[2];
	int HP;
	int Moves;
	int Attack;
	int Attack_Range;
	int Position[2];
	int State;
} archer_default = {"A ", 5, 5, 4, 3, {1,2}, 1};

typedef struct character Character;
typedef struct archer Archer;
