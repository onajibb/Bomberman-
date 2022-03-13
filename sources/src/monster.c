#include <SDL/SDL_image.h>
#include <assert.h>
#include <map.h>
#include <monster.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


struct monster {              
	int x;
	int y;
	enum direction direction;
	int monster_is_alive;
	int level;
	struct monster* next_monster;
	
};

struct monster* monster_init() {
	
	struct monster* monster = malloc(sizeof(*monster));
	
	if (!monster)
		error("Memory error");

	assert(monster);
	
	monster->direction = SOUTH;
	monster->next_monster = NULL;
	
	return monster;
}

void monster_set_position(struct monster* monster, int x, int y) {
		assert(monster);
		monster->x = x;
		monster->y = y;
	}


void monster_free(struct monster* monster) {
	assert(monster);
	free(monster);
}

int monster_get_x(struct monster* monster) {
	assert(monster != NULL);
	return monster->x;
}

int monster_get_level(struct monster* monster) {
	assert(monster != NULL);
	return monster->level;
}


int monster_is_alive(struct monster* monster)
	{
		assert(monster);
		return monster->monster_is_alive;
	}

int monster_get_y(struct monster* monster) {
	assert(monster != NULL);
	return monster->y;
}

int monster_get_direction(struct monster* monster){
	assert(monster);
	return monster->direction;
}

struct monster* monster_get_next_monster(struct monster* monster){
	assert(monster);
	return monster->next_monster;
}


void monster_set_current_way(struct monster* monster, enum direction way) {
	assert(monster);
	monster->direction = way;
}

struct monster* monster_fix_position(struct monster* monster,int x, int y, int level){
	
	assert(monster);
	
	while(monster_get_next_monster(monster))
	{
		monster = monster_get_next_monster(monster);
	}

	monster->x=x;
	monster->y=y;
	monster->next_monster=monster_init();
	monster->monster_is_alive=1;
	monster->level=level;

	return monster;
}

struct monster* kill_monster(struct monster* monster,int x, int y)
	{
		assert(monster);

		struct monster* temp=monster;
			
			while(monster_get_next_monster(temp))
			{
				if(monster_get_x(temp)==x && monster_get_y(temp)==y)
					temp->monster_is_alive=0;
				temp = monster_get_next_monster(temp);
			}
		return monster;
	}

void create_monsters_fom_map(struct monster* monster, struct map* map)
	{

		assert(monster);
		assert(map);
		

		for (int i = 0; i < map_get_width(map); i++)
			{
	  			for (int j = 0; j < map_get_height(map); j++) 
	  				{
			    	if (map_get_cell_type(map, i, j) == CELL_MONSTER) 
			    		{
			      			monster_fix_position(monster, i, j,map_get_level(map));
			      			
			      			
			      			
			    		}
	  				}
			}
	}

static int monster_move_aux(struct monster* monster, struct map* map, int x, int y) {

	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		return 1;
		break;

	case CELL_BOX:
		return 1;
		break;

	case CELL_BONUS:
		break;


	default:
		break;
	}
	return 1;
}


int monster_move(struct monster* monster, struct map* map) {
	int x = monster->x;
	int y = monster->y;
	int move = 0;
	if(monster_is_alive(monster))
	{
		switch (monster->direction) {
		case NORTH:
			if (monster_move_aux(monster, map, x, y - 1))                                               // on est dans la carte 
				{
					if (map_get_cell_type(map, x, y-1)==CELL_EMPTY && monster->y - 1 >= 0)				// La case de dessus est vide et le monster est dans la carte
						{	
							monster->y--;
							map_set_cell_type(map, x, y,CELL_EMPTY);
							map_set_cell_type(map, x, y-1,CELL_MONSTER);
							move = 1;
				
						}
				}
			
			
			break;

		case SOUTH:
			if (monster_move_aux(monster, map, x, y + 1))                                               
				{
					if (map_get_cell_type(map, x, y+1)==CELL_EMPTY  && monster->y + 1 < map_get_height(map))    
						{
							monster->y++;
							map_set_cell_type(map, x, y,CELL_EMPTY);
							map_set_cell_type(map, x, y+1,CELL_MONSTER);
							move = 1;
						}
				}
			
			
			break;

		case WEST:
			if (monster_move_aux(monster, map, x-1, y ))                                               
				{
					if (map_get_cell_type(map, x-1, y)==CELL_EMPTY  && monster-> x - 1 >= 0 )
						{
							monster->x--;
							map_set_cell_type(map, x, y,CELL_EMPTY);
							map_set_cell_type(map, x-1, y,CELL_MONSTER);
							move = 1;
						}
				}
			
			break;

		case EAST:
			if (monster_move_aux(monster, map, x+1, y ))                                               
			{
				if (map_get_cell_type(map, x+1, y)==CELL_EMPTY && monster-> x + 1 < map_get_width(map) )
					{
						monster->x++;
						map_set_cell_type(map, x, y,CELL_EMPTY);
						map_set_cell_type(map, x+1, y,CELL_MONSTER);
						move = 1;
					}
			}
			
			break;
		}
	}

	return move;
}

void monsters_display(struct monster* monster, struct map* map) {
	struct monster *temp=monster;
	while(monster_get_next_monster(temp))
	{
		
		assert(monster);
		if(monster_is_alive(temp) && monster_get_level(temp)==map_get_level(map))
			window_display_image(sprite_get_monster(temp->direction),temp->x * SIZE_BLOC, temp->y * SIZE_BLOC);
		
		temp=temp->next_monster;
	}
}
void monsters_next_move(struct monster* monster,struct map* map)
	{
		while(monster_get_next_monster(monster))
		{
			int direction=rand()%4 ;
			
			if(monster_get_level(monster)==map_get_level(map))
				{
					monster_set_current_way(monster, direction);
					monster_move( monster,  map);
				}
			monster=monster->next_monster;
		}
		
	}



	