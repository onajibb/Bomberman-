/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef MAP_H_
#define MAP_H_

enum cell_type {
	CELL_EMPTY=0x00,   		//  0000 0000
	CELL_SCENERY=0x10, 		//  0001 0000
	CELL_BOX=0x20,   		//  0010 0000
	CELL_DOOR=0x30,      	//  0011 0000
	CELL_KEY=0x40,       	//  0100 0000
	CELL_BONUS=0x50, 		// 	0101 0000
	CELL_MONSTER=0x60, 		// 	0110 0000
	CELL_BOMB=0x70,
	CELL_BOMBERWOMAN=0x80 	   		
};

enum door_type{
	OPENED = 1, //0001
	CLOSED = 0  //0010
};

enum bonus_type {
	BONUS_BOMB_RANGE_DEC=1,
	BONUS_BOMB_RANGE_INC=2,
	BONUS_BOMB_NB_DEC=3,
	BONUS_BOMB_NB_INC=4,
	BONUS_LIFE=5

};



enum bomb_type{             //etat de la bombe
	BOMB_1 = 1,  //0001
	BOMB_2 = 2,  //0010
	BOMB_3 = 4,  //0100
	BOMB_4 = 8,   //1000
	EXPLODE = 10,  //1010
	EXPLODE_DECBOMB =  11, //1011
	EXPLODE_INCBOMB = 12, //1100
	EXPLODE_LIFE = 13 //1101
};

enum scenery_type {
	SCENERY_STONE = 1,    // 0001
	SCENERY_TREE  = 2,    // 0010
	SCENERY_PRINCESS = 4  // 0100
};

enum next_level_door
{
	LEVEL_0=0x00,
	LEVEL_1=0x02,
	LEVEL_2=0x04,
	LEVEL_3=0x06,
	LEVEL_4=0x08,
	LEVEL_5=0x0A,
	LEVEL_6=0x0C,
	LEVEL_7=0x0E
};


enum compose_type {
	CELL_TREE     = CELL_SCENERY | SCENERY_TREE,
	CELL_STONE    = CELL_SCENERY | SCENERY_STONE,
	CELL_PRINCESS = CELL_SCENERY | SCENERY_PRINCESS,

	CELL_BOMB_1 = CELL_BOMB | BOMB_1,
	CELL_BOMB_2 = CELL_BOMB | BOMB_2,
	CELL_BOMB_3 = CELL_BOMB | BOMB_3,
	CELL_BOMB_4 = CELL_BOMB | BOMB_4,
	CELL_EXPLOSION = CELL_BOMB | EXPLODE,
	CELL_EXPLOSION_DECBOMB = CELL_BOMB | EXPLODE_DECBOMB,
	CELL_EXPLOSION_INCBOMB = CELL_BOMB | EXPLODE_INCBOMB,
	CELL_EXPLOSION_LIFE    = CELL_BOMB | EXPLODE_LIFE,

    CELL_BOX_RANGEINC = CELL_BOX | BONUS_BOMB_RANGE_INC,
    CELL_BOX_RANGEDEC = CELL_BOX | BONUS_BOMB_RANGE_DEC,
	CELL_BOX_BOMBINC  = CELL_BOX | BONUS_BOMB_NB_INC,
    CELL_BOX_BOMBDEC  = CELL_BOX | BONUS_BOMB_NB_DEC,
    CELL_BOX_LIFE     = CELL_BOX | BONUS_LIFE,

    CELL_BONUS_RANGEINC = CELL_BONUS | BONUS_BOMB_RANGE_INC,
    CELL_BONUS_RANGEDEC = CELL_BONUS | BONUS_BOMB_RANGE_DEC,
	CELL_BONUS_BOMBINC  = CELL_BONUS | BONUS_BOMB_NB_INC,
    CELL_BONUS_BOMBDEC  = CELL_BONUS | BONUS_BOMB_NB_DEC,
    CELL_BONUS_LIFE     = CELL_BONUS | BONUS_LIFE,

	CELL_DOOR_OPENED = CELL_DOOR | OPENED,
	CELL_DOOR_CLOSED = CELL_DOOR | CLOSED,
	CELL_DOOR_LEVEL_0_OPENED = CELL_DOOR | LEVEL_0 |OPENED,
	CELL_DOOR_LEVEL_1_OPENED = CELL_DOOR | LEVEL_1 |OPENED,
	CELL_DOOR_LEVEL_2_OPENED = CELL_DOOR | LEVEL_2 |OPENED,
	CELL_DOOR_LEVEL_3_OPENED = CELL_DOOR | LEVEL_3 |OPENED,
	CELL_DOOR_LEVEL_4_OPENED = CELL_DOOR | LEVEL_4 |OPENED,
	CELL_DOOR_LEVEL_5_OPENED = CELL_DOOR | LEVEL_5 |OPENED,
	CELL_DOOR_LEVEL_6_OPENED = CELL_DOOR | LEVEL_6 |OPENED,
	CELL_DOOR_LEVEL_7_OPENED = CELL_DOOR | LEVEL_7 |OPENED,
	CELL_DOOR_LEVEL_1_CLOSED = CELL_DOOR | LEVEL_1 |CLOSED,
	CELL_DOOR_LEVEL_2_CLOSED = CELL_DOOR | LEVEL_2 |CLOSED,
	CELL_DOOR_LEVEL_3_CLOSED = CELL_DOOR | LEVEL_3 |CLOSED,
	CELL_DOOR_LEVEL_4_CLOSED = CELL_DOOR | LEVEL_4 |CLOSED,
	CELL_DOOR_LEVEL_5_CLOSED = CELL_DOOR | LEVEL_5 |CLOSED,
	CELL_DOOR_LEVEL_6_CLOSED = CELL_DOOR | LEVEL_6 |CLOSED,
	CELL_DOOR_LEVEL_7_CLOSED = CELL_DOOR | LEVEL_7 |CLOSED,







};

struct map;

// Create a new empty map
struct map* map_new(int width, int height);
void map_free(struct map* map);


// Return the height and width of a map
int map_get_width(struct map* map);
int map_get_height(struct map* map);

// Return the type of a cell
enum cell_type map_get_cell_type(struct map* map, int x, int y);
enum compose_type  map_get_compose_type(struct map* map, int x, int y);

// Set the type of a cell
void  map_set_cell_type(struct map* map, int x, int y, enum cell_type type);

// Test if (x,y) is within the map
int map_is_inside(struct map* map, int x, int y);

// Return a default static map
struct map* map_get_static();

struct map * open_map(char);
struct map* read_map(int level);


// Display the map on the screen
void map_display(struct map* map);



int chamge_map(void);
int change_map(void);
int map_get_level(struct map* map);



//bomb
void display_bomb(struct map*map,int x, int y, unsigned char type);
void display_door(struct map*map,int x, int y, unsigned char type);
#endif /* MAP_H_ */
