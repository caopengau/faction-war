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
};

typedef struct terrain Terrain;
struct terrain{
	int id_terrain;
	char *terrain_name;
	char terrain_symbol;
};
