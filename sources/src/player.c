/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>


struct player {
	int x, y;
	enum direction direction;
	int bombs;
	int lives;
	int key;
	char level;
};

struct player* player_init(int bombs, int lives) {
	struct player* player = malloc(sizeof(*player));
	if (!player)
		error("Memory error");

	player->direction = SOUTH;
	player->bombs = bombs;
	player->lives = lives;
	player->key = 0;
	player->level = '0';
	return player;
}



void player_set_position(struct player *player, int x, int y) {
		assert(player);
		player->x = x;
		player->y = y;
	}

int player_get_direction(struct player* player){
	assert(player != NULL);
	return player->direction;
}

void player_free(struct player* player) {
	assert(player);
	free(player);
}

int player_get_x(struct player* player) {
	assert(player != NULL);
	return player->x;
}

int player_get_y(struct player* player) {
	assert(player != NULL);
	return player->y;
}

void player_set_current_way(struct player* player, enum direction way) {
	assert(player);
	player->direction = way;
}

int player_get_nb_bomb(struct player* player) {
	assert(player);
	return player->bombs;
}

void player_inc_nb_bomb(struct player* player) {
	assert(player);
	player->bombs += 1;
}

void player_dec_nb_bomb(struct player* player) {
	assert(player);
	player->bombs -= 1;
}

int player_get_nb_lives(struct player* player) {
	assert(player);
	return player->lives;
}

void player_inc_nb_lives(struct player* player) {
	assert(player);
	player->lives += 1;
}

void player_dec_nb_lives(struct player* player) {
	assert(player);
	player->lives -= 1;
}

void player_get_key(struct player* player){
	assert(player);
	player->key = 1;
}

void player_remove_key(struct player* player){
	assert(player);
	player->key = 0;
}

int player_get_nb_key(struct player* player){
		assert(player);
		return player->key;
}

char player_get_level(struct player*player){
	assert(player);
	return player->level;
}

static int player_move_aux(struct player* player, struct map* map, int x, int y) {

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

	case CELL_DOOR:
		return 1;
		break;
	case CELL_MONSTER:
		break;

	default:
		break;
	}
	return 1;
}

int player_move(struct player* player, struct map* map) {
	int x = player->x;
	int y = player->y;
	int move = 0;
	

	switch (player->direction) {
	case NORTH:
		if (player_move_aux(player, map, x, y - 1))                                               // on est dans la carte
			{
				if ((map_get_cell_type(map, x, y-1)==CELL_EMPTY || map_get_cell_type(map, x, y-1)==CELL_BOMB || map_get_compose_type(map, x, y-1)==CELL_DOOR_OPENED) && player->y - 1 >= 0){					  // La case de dessus est vide et le joueur est dans la carte
					player->y--;

				move = 1;}
				if (map_get_cell_type(map, x, y-1)==CELL_MONSTER && player->y - 1 >= 0)
					{
						player->y--;

					}
				if (map_get_cell_type(map, x, y-1)==CELL_BOMBERWOMAN && player->y - 1 >= 0)
					{
						player->y--;

					}
				if (map_get_compose_type(map, x, y-1)==CELL_BONUS_LIFE && player->y - 1 >= 0)
					{
						player->y--;

						if(player_get_nb_lives(player)<8){
							player_inc_nb_lives(player);
							map_set_cell_type(map,x,y-1,CELL_EMPTY);
						}

					}
				if (map_get_compose_type(map, x, y-1)==CELL_BONUS_BOMBINC && player->y - 1 >= 0)
					{
						player->y--;


						player_inc_nb_bomb(player);
						map_set_cell_type(map,x,y-1,CELL_EMPTY);
					}
				if (map_get_compose_type(map, x, y-1)==CELL_BONUS_BOMBDEC && player->y - 1 >= 0)
					{
						player->y--;


						player_dec_nb_bomb(player);
						map_set_cell_type(map,x,y-1,CELL_EMPTY);
					}


				if (map_get_cell_type(map, x, y-1)==CELL_BOX && map_get_cell_type(map,x,y-2)==CELL_EMPTY && y-1 !=0)
					{
						map_set_cell_type(map, x, y-1, CELL_EMPTY);
						map_set_cell_type(map, x, y-2, CELL_BOX);
						if(player->y - 1 >=0)
							player->y--;

					}
				if(map_get_cell_type(map, x, y-1)==CELL_KEY && player->y - 1 >= 0){

					player_get_key(player);
					map_set_cell_type(map,x,y-1,CELL_EMPTY);
					player->y--;

				}
				if (map_get_compose_type(map, x, y-1)==CELL_DOOR_CLOSED && player->y - 1 >= 0 && player_get_nb_key(player) == 1)
					{
						player->y--;
						map_set_cell_type(map,x,y-1,CELL_DOOR_OPENED);
						player_remove_key(player);
						player->level = player->level + 1;
						map_free(map);
						
					}
				if ((map_get_compose_type(map, x, y-1)& 0xF1) == CELL_DOOR_OPENED && player->y - 1 >= 0 )
						{
							
							
							
							
							if((map_get_compose_type(map, x, y-1)& 0x0F)>>1 > map_get_level(map))
								chamge_map();
							
							if((map_get_compose_type(map, x, y-1)& 0x0F)>>1 < map_get_level(map))
								change_map();
							
							player->y--;
					}

			}
		break;

	case SOUTH:
		if (player_move_aux(player, map, x, y + 1))
			{
				if ((map_get_cell_type(map, x, y+1)==CELL_EMPTY || map_get_cell_type(map, x, y+1)==CELL_BOMB|| map_get_compose_type(map, x, y+1)==CELL_DOOR_OPENED) && player-> y + 1 < map_get_height(map)){
					player->y++;

				move = 1;}
				if (map_get_cell_type(map, x, y+1)==CELL_MONSTER && player-> y + 1 < map_get_height(map))
					{
						player->y++;


					}
				if (map_get_cell_type(map, x, y+1)==CELL_BOMBERWOMAN && player-> y + 1 < map_get_height(map))
					{
						player->y++;


					}

				if (map_get_compose_type(map, x, y+1)==CELL_BONUS_LIFE && player-> y + 1 < map_get_height(map))
					{
						player->y++;

						if(player_get_nb_lives(player)<8){
							player_inc_nb_lives(player);
							map_set_cell_type(map,x,y+1,CELL_EMPTY);}
					}

				if (map_get_compose_type(map, x, y+1)==CELL_BONUS_BOMBINC && player-> y + 1 < map_get_height(map))
					{
						player->y++;

						player_inc_nb_bomb(player);
						map_set_cell_type(map,x,y+1,CELL_EMPTY);
					}
				if (map_get_compose_type(map, x, y+1)==CELL_BONUS_BOMBDEC && player-> y + 1 < map_get_height(map))
					{
						player->y++;

						player_dec_nb_bomb(player);
						map_set_cell_type(map,x,y+1,CELL_EMPTY);
					}

				if (map_get_cell_type(map, x, y+1)==CELL_BOX && map_get_cell_type(map,x,y+2)==CELL_EMPTY && y+1 != map_get_height(map)-1 )
					{
						map_set_cell_type(map, x, y+1, CELL_EMPTY);
						map_set_cell_type(map, x, y+2, CELL_BOX);
						if(player->y + 1 < map_get_height(map))
							player->y++;

					}
				if(map_get_cell_type(map, x, y+1)==CELL_KEY && player-> y + 1 < map_get_height(map)){

								player_get_key(player);
								map_set_cell_type(map,x,y+1,CELL_EMPTY);
								player->y++;
				}

			if ((map_get_compose_type(map, x, y+1)& 0xF1) == CELL_DOOR_OPENED && player->y + 1  < map_get_height(map) )
					{
						
						
						
						
						if((map_get_compose_type(map, x, y+1)& 0x0F)>>1 > map_get_level(map))
							chamge_map();
						
						if((map_get_compose_type(map, x, y+1)& 0x0F)>>1 < map_get_level(map))
							change_map();
						
						player->y++;

					}
					
			}

		break;

	case WEST:
		if (player_move_aux(player, map, x-1, y ))
			{

				if ((map_get_cell_type(map, x-1, y)==CELL_EMPTY || map_get_cell_type(map, x-1, y)==CELL_BOMB || map_get_compose_type(map, x-1, y)==CELL_DOOR_OPENED) && player-> x - 1 >= 0 ){
					player->x--;

				move = 1;
				}
				if (map_get_cell_type(map, x-1, y)==CELL_MONSTER && player-> x- 1 >= 0)
					{
						player->x--;

					}
				if (map_get_cell_type(map, x-1, y)==CELL_BOMBERWOMAN && player-> x- 1 >= 0)
					{
						player->x--;

					}

				if (map_get_compose_type(map, x-1, y)==CELL_BONUS_LIFE && player-> x- 1 >= 0)
					{
						player->x--;

						if(player_get_nb_lives(player)<8){
							player_inc_nb_lives(player);
							map_set_cell_type(map,x-1,y,CELL_EMPTY);}

					}

				if (map_get_compose_type(map, x-1, y)==CELL_BONUS_BOMBINC && player-> x- 1 >= 0)
									{
										player->x--;

										player_inc_nb_bomb(player);
										map_set_cell_type(map,x-1,y,CELL_EMPTY);

									}
				if (map_get_compose_type(map, x-1, y)==CELL_BONUS_BOMBDEC && player-> x- 1 >= 0)
									{
										player->x--;

										player_dec_nb_bomb(player);
										map_set_cell_type(map,x-1,y,CELL_EMPTY);

									}


				if (map_get_cell_type(map, x-1, y)==CELL_BOX && map_get_cell_type(map,x-2,y)==CELL_EMPTY && x-1 !=0)
						{
						map_set_cell_type(map, x-1, y, CELL_EMPTY);
						map_set_cell_type(map, x-2, y, CELL_BOX);
						if(player->x - 1 >=0)
							player->x--;
						move = 1;

					}
					if(map_get_cell_type(map, x-1, y)==CELL_KEY && player-> x - 1 >= 0){

									player_get_key(player);
									map_set_cell_type(map,x-1,y,CELL_EMPTY);
									player->x--;
					}
					if ((map_get_compose_type(map, x-1, y) & 0xF1) == 0x30 && player->x - 1 >= 0 && player_get_nb_key(player) == 1)
						{
							player->x--;
							map_set_cell_type(map,x-1,y,CELL_DOOR_LEVEL_0_OPENED);
							player_remove_key(player);
							player->level = player->level + 1;

							map_free(map);
							map = open_map(player_get_level(player));
						}

					if ((map_get_compose_type(map, x-1, y)& 0xF1) == CELL_DOOR_OPENED && player->x - 1 >= 0  )
						{
							
							if((map_get_compose_type(map, x-1, y)& 0x0F)>>1 > map_get_level(map))
								chamge_map();
							
							if((map_get_compose_type(map, x-1, y)& 0x0F)>>1 < map_get_level(map))
								change_map();
							
							player->x--;
						}
			}
		break;

	case EAST:
		if (player_move_aux(player, map, x+1, y ))
			{
				if ((map_get_cell_type(map, x+1, y)==CELL_EMPTY || map_get_cell_type(map, x+1, y)==CELL_BOMB || map_get_compose_type(map, x+1, y)==CELL_DOOR_OPENED) && player-> x + 1 < map_get_width(map) ){
					player->x++;

				move = 1;
			}
				if (map_get_cell_type(map, x+1, y)==CELL_MONSTER && player-> x + 1 < map_get_height(map))
					{
						player->x++;

					}
				if (map_get_cell_type(map, x+1, y)==CELL_BOMBERWOMAN && player-> x + 1 < map_get_height(map))
					{
						player->x++;

					}

				if (map_get_compose_type(map, x+1, y)==CELL_BONUS_LIFE && player-> x + 1 < map_get_height(map))
					{
						player->x++;

						if(player_get_nb_lives(player)<8){
							player_inc_nb_lives(player);
							map_set_cell_type(map,x+1,y,CELL_EMPTY);
						}
					}

				if (map_get_compose_type(map, x+1, y)==CELL_BONUS_BOMBINC && player-> x + 1 < map_get_height(map))
									{
										player->x++;

										player_inc_nb_bomb(player);
										map_set_cell_type(map,x+1,y,CELL_EMPTY);

									}
				if (map_get_compose_type(map, x+1, y)==CELL_BONUS_BOMBDEC && player-> x + 1 < map_get_height(map))
									{
										player->x++;

										player_dec_nb_bomb(player);
										map_set_cell_type(map,x+1,y,CELL_EMPTY);

									}


				if (map_get_cell_type(map, x+1, y)==CELL_BOX && map_get_cell_type(map,x+2,y)==CELL_EMPTY && y+1 != map_get_width(map)+1 )
					{
						map_set_cell_type(map, x+1, y, CELL_EMPTY);
						map_set_cell_type(map, x+2, y, CELL_BOX);
						if(player->x + 1 < map_get_width(map))
							player->x++;
						move = 1;

				 	}
					if(map_get_cell_type(map, x+1, y)==CELL_KEY && player-> x + 1 <= map_get_width(map)){

									player_get_key(player);
									map_set_cell_type(map,x+1,y,CELL_EMPTY);
									player->x++;
					}
					if (map_get_compose_type(map, x+1, y)==CELL_DOOR_CLOSED && player->x + 1  < map_get_width(map) && player_get_nb_key(player) == 1)
						{
							player->x++;
							map_set_cell_type(map,x+1,y,CELL_DOOR_OPENED);
							player_remove_key(player);
							player->level = player->level + 1;

							map_free(map);
							map = open_map(player_get_level(player));
						}
					if ((map_get_compose_type(map, x+1, y)& 0xF1) == CELL_DOOR_OPENED && player->x + 1  < map_get_width(map)  )
						{
							
							if((map_get_compose_type(map, x+1, y)& 0x0F)>>1 > map_get_level(map))
								chamge_map();
							
							if((map_get_compose_type(map, x+1, y)& 0x0F)>>1 < map_get_level(map))
								change_map();
							
							player->x++;
						}
			}
		break;
	}
	if (move) {
		if(!(map_get_cell_type(map, x, y)==CELL_BOMB || map_get_cell_type(map,x,y)==CELL_MONSTER) )
			map_set_cell_type(map, x, y, CELL_EMPTY);

	}
	return move;
}

void player_display(struct player* player) {
	assert(player);
	window_display_image(sprite_get_player(player->direction),
			player->x * SIZE_BLOC, player->y * SIZE_BLOC);
}
