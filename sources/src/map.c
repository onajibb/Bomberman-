/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Oumaima The BEAST
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <map.h>
#include <constant.h>
#include <misc.h>
#include <sprite.h>
#include <window.h>

struct map {
	int width;
	int height;
	unsigned char* grid;
	int level;
};

#define CELL(i,j) ( (i) + (j) * map->width)

struct map* map_new(int width, int height)
{
	assert(width > 0 && height > 0);

	struct map* map = malloc(sizeof *map);
	if (map == NULL )
		error("map_new : malloc map failed");

	map->width = width;
	map->height = height;

	map->grid = malloc(height * width);
	if (map->grid == NULL) {
		error("map_new : malloc grid failed");
	}

	// Grid cleaning
	int i, j;
	for (i = 0; i < width; i++)
	  for (j = 0; j < height; j++)
	    map->grid[CELL(i,j)] = CELL_EMPTY;

	return map;
}

struct map * open_map(char level){
	FILE* new_map = NULL;
	char map_name[10] = "map/map_";
	map_name[8] = level;
	map_name[9] = '\0';
	new_map = fopen(map_name, "r+");
	int width = 0;
	int height = 0;
	fscanf(new_map,"%d\n:%d\n",&width, &height);
		struct map * map = map_new(width, height);
		int type_cell;
		for(int i=0; i < width; i++){
			for(int j = 0; j < height; j++){
				fscanf(new_map, "%d", &type_cell);
				map->grid[CELL(i,j)] = type_cell;
			}
		}

		fclose(new_map);
		return map;

}


struct map* read_map(int level){
	
	FILE* new_map = NULL;
	
	switch (level) 
		{

			case 0:
			new_map = fopen("map/map_0","r+");
			break;
			case 1:
			new_map = fopen("map/map_1","r+");
			break;
			case 2:
			new_map = fopen("map/map_2","r+");
			break;
			case 3:
			new_map = fopen("map/map_3","r+");
			break;
			case 4:
			new_map = fopen("map/map_4","r+");
			break;
			case 5:
			new_map = fopen("map/map_5","r+");
			break;
			case 6:
			new_map = fopen("map/map_6","r+");
			break;
			case 7:
			new_map = fopen("map/map_7","r+");
			break;



		}
	
	int width = 0;
	int height = 0;

	
	fscanf(new_map,"%d\n:%d\n",&width, &height);
	struct map * map = map_new(width, height);
	map->level=level;
	int type_cell;
	
	for(int j=0; j < width; j++)
		{
			for(int i = 0; i < height; i++)
				{
					fscanf(new_map, "%d", &type_cell);
					map->grid[CELL(i,j)] = type_cell;
				}
		}

		fclose(new_map);
		return map;

}


int map_is_inside(struct map* map, int x, int y)
{
	assert(x <= map->width && y <= map->height );     /*map*/
	return 1;
}

void map_free(struct map *map)
{
	if (map == NULL )
		return;
	free(map->grid);
	free(map);
}

int map_get_width(struct map* map)
{
	assert(map != NULL);
	return map->width;
}

int map_get_height(struct map* map)
{
	assert(map);
	return map->height;
}

int map_get_level(struct map* map)
{
	assert(map);
	return map->level;
}


enum cell_type map_get_cell_type(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 0xf0;
}

enum compose_type map_get_compose_type(struct map* map, int x, int y)
	{
		assert(map && map_is_inside(map,x,y));
		return map->grid[CELL(x,y)] & 0xff;
	}

void map_set_cell_type(struct map* map, int x, int y, enum cell_type type)
{
	assert(map && map_is_inside(map, x, y));
	map->grid[CELL(x,y)] = type;
}

void display_bonus(struct map* map, int x, int y, unsigned char type)
{
	// bonus is encoded with the 4 most significant bits
	switch (type & 0x0f) {
	case BONUS_BOMB_RANGE_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_INC), x, y);
		break;

	case BONUS_BOMB_RANGE_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_DEC), x, y);
		break;

	case BONUS_BOMB_NB_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_NB_DEC), x, y);
		break;

	case BONUS_BOMB_NB_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_NB_INC), x, y);
		break;

	case BONUS_LIFE:
		window_display_image(sprite_get_bonus(BONUS_LIFE),x,y);
	}
}

void display_scenery(struct map* map, int x, int  y, unsigned char type)
{
	switch (type & 0x0f) { // sub-types are encoded with the 4 less significant bits
	case SCENERY_STONE:
		window_display_image(sprite_get_stone(), x, y);
		break;

	case SCENERY_TREE:
		window_display_image(sprite_get_tree(), x, y);
		break;
	}
}

void display_bomb(struct map*map,int x, int y, unsigned char type){
	switch (type & 0x0f)
	{
	case BOMB_1:
		window_display_image(sprite_get_bomb1(),x,y);
		break;
	case BOMB_2:
		window_display_image(sprite_get_bomb2(),x,y);
		break;
	case BOMB_3:
		window_display_image(sprite_get_bomb3(),x,y);
		break;
	case BOMB_4:
		window_display_image(sprite_get_bomb4(),x,y);
		break;
	case EXPLODE:
	window_display_image(sprite_get_explosion(),x,y);
		break;
	case EXPLODE_LIFE:
		window_display_image(sprite_get_explosion(),x,y);
		break;
	case EXPLODE_DECBOMB:
		window_display_image(sprite_get_explosion(),x,y);
		break;
	case EXPLODE_INCBOMB:
		window_display_image(sprite_get_explosion(),x,y);
		break;
	default:
		break;
	}
}



struct map* map_get_static(void)
{
	struct map* map = map_new(STATIC_MAP_WIDTH, STATIC_MAP_HEIGHT);

	unsigned char themap[STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT] = {
	  CELL_MONSTER, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_MONSTER, CELL_EMPTY, CELL_EMPTY,   CELL_EMPTY, CELL_MONSTER, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
	  CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_KEY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_BOX, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_BOX_LIFE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_BOX, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_BOX_BOMBDEC, CELL_EMPTY, CELL_STONE, CELL_BOX, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_MONSTER, CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY , CELL_EMPTY, CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_TREE, CELL_BOX, CELL_TREE, CELL_EMPTY, CELL_BOX_BOMBINC, CELL_EMPTY, CELL_EMPTY,  CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_TREE, CELL_TREE, CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_BOX_BOMBINC, CELL_EMPTY, CELL_EMPTY,  CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_BOX_LIFE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE,  CELL_BOX_LIFE, CELL_EMPTY,
	  CELL_EMPTY ,  CELL_MONSTER, CELL_DOOR_CLOSED, CELL_EMPTY, CELL_MONSTER, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
		};

	for (int i = 0; i < STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT; i++)
		map->grid[i] = themap[i];

	return map;
}


void display_door(struct map*map,int x, int y, unsigned char type){
	switch (type & 0x01)
	{
	case OPENED:
		window_display_image(sprite_get_door_opened(),x,y);
		break;
	case CLOSED:
		window_display_image(sprite_get_door_closed(),x,y);
		break;
	default:
		break;
	}
}


void map_display(struct map* map)
{
	assert(map != NULL);
	assert(map->height > 0 && map->width > 0);

	int x, y;
	for (int i = 0; i < map->width; i++) {
	  for (int j = 0; j < map->height; j++) {
	    x = i * SIZE_BLOC;
	    y = j * SIZE_BLOC;

	    unsigned char type = map->grid[CELL(i,j)];

	    switch (type & 0xf0) {
		case CELL_SCENERY:
		  display_scenery(map, x, y, type);
		  break;
	   case CELL_BOX:
	      window_display_image(sprite_get_box(), x, y);

	      break;
	  case CELL_BONUS:
	      display_bonus(map, x, y, type);
	      break;
	   case CELL_KEY:
	      window_display_image(sprite_get_key(), x, y);
	      break;
	   case CELL_DOOR:
	      display_door(map,x, y,type);
	      break;
		case CELL_BOMB:
				display_bomb(map,x,y,type);

			break;
		case CELL_BOMBERWOMAN:
			window_display_image(sprite_get_bomberwoman(),x,y);

	    }
	  }
	}
}

int chamge_map(void)
{
	static int i =0;
	i=i+1;
	return i; 
}

int change_map(void)
{
	static int i =0;
	i=i+1;
	return i; 
}