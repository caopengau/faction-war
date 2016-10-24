// this module defines structures and typedef

typedef struct terrain Terrain;
struct terrain{
	int id_terrain;
	char *terrain_name;
	char terrain_symbol;
};

typedef struct unit Unit;
struct unit{
	int player_id;
	int unit_id;
	char *unit_type;
	char unit_symbol;	// symbol of that type of unit printed on map
	int lifebar;		// maximum hitpoint
	int hitpoint;		// current hitpoint
	char action[3];		// action for replenished each turn, move/attack/rest
	int x;
	int y;				// x,y coordinate of the unit
	int movementbar;	// number of pixel it can move in one turn
	int movement;		// movement left
	int attack;
	int attackrange;
	int defense;
	int sight;			// the number of pixel it can see in the fog
	int cost;
};

typedef struct expansion Expansion;
struct expansion{
	int player_id;	// owner of the expansion
	char expansion_symbol;
	int expansion_id;
	int income;		// gold generated per turn
	int x;
	int y;			// x,y coordinate of the expansion
};

typedef struct player Player;
struct player{
	int player_id;
	char base;
	int x; int y;
	int sight;
	int no_of_units;
	int units_alive;
	int no_of_hostile;
	Unit **unit_list;
	Unit **hostile_spotted;
	int resource;
	int no_of_expansion;
	int * expansion_list;
};