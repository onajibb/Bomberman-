#include <SDL/SDL_image.h>
#include <assert.h>
#include <map.h>
#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <unistd.h>
#include <bomb.h>

struct bomb{
	int drop_time;
	struct bomb* next;
	struct bomb* prev;
	int x; int y;
};

struct bomb* bomb_init(){
	struct bomb * new_bomb = malloc(sizeof(*new_bomb));
	if(!new_bomb){
		error("Memory error");
	}
	assert(new_bomb);
	new_bomb->drop_time = -1;
	new_bomb->next = NULL;
	//new_bomb->prev = NULL;
	new_bomb->x = 0;
	new_bomb ->y = 0;
	return new_bomb;
}

struct bomb* bomb_add(struct bomb*bomb, struct game * game){
	assert(bomb);
	struct bomb* new_bomb = malloc(sizeof(*bomb));
	struct player * player = game_get_player(game);
	new_bomb->drop_time = SDL_GetTicks();
	new_bomb->prev = NULL;
	bomb->prev = new_bomb;
		new_bomb->x = player_get_x(player);
	new_bomb->y = player_get_y(player);
	new_bomb->next = bomb;
	return new_bomb;

}

int bomb_get_next(struct bomb* bomb){
	assert(bomb);
	if(bomb->next != NULL){
		return 1;
	}
	return 0;
}


int bomb_get_timer(struct bomb*bomb){
	assert(bomb);
	return bomb->drop_time;
}

int bomb_get_x(struct bomb*bomb){
	assert(bomb);
	return bomb->x;
}

int bomb_get_y(struct bomb*bomb){
	assert(bomb);
	return bomb->y;
}



void bomb_meca(struct game*game){
	struct bomb * bomb = game_get_bomb(game);
	struct map * map = game_get_current_map(game);
	while(bomb->next != NULL){
		int time_fixe = bomb->drop_time;
		int time = SDL_GetTicks();
		//printf("%d\n",time - bomb->drop_time);
		if(time - time_fixe <= 1000 ){
			map_set_cell_type(map,bomb->x,bomb->y,CELL_BOMB_4);
		//	printf("%d\n",time - bomb->drop_time);
}
		else if(time - time_fixe <= 2000 && time - time_fixe >= 1000){
			map_set_cell_type(map, bomb->x, bomb->y, CELL_BOMB_3);
		}
		else if(time - time_fixe <= 3000 && time - time_fixe >= 2000){
			map_set_cell_type(map, bomb->x, bomb->y, CELL_BOMB_2);
		}
		else if(time - time_fixe <= 4000 && time - time_fixe >= 3000){
			map_set_cell_type(map, bomb->x, bomb->y, CELL_BOMB_1);
		}
		else if(time - time_fixe <= 5200 && time - time_fixe >= 4000){
			  set_fire(game,bomb->x,bomb->y);
				if(time - time_fixe <= 5000 && time- time_fixe >= 4960){
					player_gets_harmed(game, game_get_current_map(game));
				}

			display_bonus_aux(map, (bomb->x)-1, bomb->y);
			display_bonus_aux(map, (bomb->x)+1, bomb->y);
			display_bonus_aux(map, bomb->x, (bomb->y)-1);
			display_bonus_aux(map, bomb->x, (bomb->y)+1);

		}
		else{
			stop_fire(map,bomb->x,bomb->y);
			stop_fire(map,(bomb->x) -1, bomb->y);
			stop_fire(map,(bomb->x) +1, bomb->y);
			stop_fire(map,bomb->x, (bomb->y)+1);
			stop_fire(map,bomb->x, (bomb->y)-1);
			struct bomb* next_bomb = bomb->next;
			struct bomb* prev_bomb = bomb->prev;
			if(prev_bomb != NULL){
				prev_bomb->next = next_bomb;
				next_bomb->prev = prev_bomb;
			}
		}
		bomb = bomb->next;

}}



	void set_fire(struct game* game, int x, int y){

		struct map * map =game_get_current_map(game);
		struct monster* monster = game_get_monster(game);
		for(int i = 0; i < 2; i++){
		 if((map_get_cell_type(map,x+i,y) == CELL_EMPTY || map_get_cell_type(map,x+i,y) == CELL_BOMB || map_get_cell_type(map,x+i,y) == CELL_BONUS || map_get_cell_type(map,x+i,y)==CELL_MONSTER ) && map_is_inside(map,x+i,y)){
			

			monster=kill_monster( monster,x+i, y);

			map_set_cell_type(map,x+i,y,CELL_EXPLOSION);
		 }
		}
		if((map_get_cell_type(map,x-1,y) == CELL_EMPTY || map_get_cell_type(map,x-1,y) == CELL_BONUS || map_get_cell_type(map,x-1,y)==CELL_MONSTER ) && map_is_inside(map,x-1,y)){
			

				monster=kill_monster( monster,x-1, y);

				map_set_cell_type(map,x-1,y,CELL_EXPLOSION);
		}
			if((map_get_cell_type(map,x,y+1) == CELL_EMPTY || map_get_cell_type(map,x,y+1) == CELL_BONUS || map_get_cell_type(map,x,y+1)==CELL_MONSTER )  && map_is_inside(map,x,y+1)){
				monster=kill_monster( monster,x, y+1);
				map_set_cell_type(map,x,y+1,CELL_EXPLOSION);
			}
			int z = y-1;
			if((map_get_cell_type(map,x,z) == CELL_EMPTY || map_get_cell_type(map,x,z) == CELL_BONUS || map_get_cell_type(map,x,z)==CELL_MONSTER ) && map_is_inside(map,x,y-1)){
				monster=kill_monster( monster,x, z);
				map_set_cell_type(map,x,z,CELL_EXPLOSION);
			}
	}


	void stop_fire(struct map*map,int x, int y){
			if(map_get_cell_type(map,x,y) == CELL_BOMB){
				switch(map_get_compose_type(map,x,y)){
					case CELL_EXPLOSION_LIFE:
						map_set_cell_type(map,x,y,CELL_BONUS_LIFE);
						break;
					case CELL_EXPLOSION_INCBOMB:
						map_set_cell_type(map,x,y,CELL_BONUS_BOMBINC);
						break;
					case CELL_EXPLOSION_DECBOMB:
						map_set_cell_type(map,x,y,CELL_BONUS_BOMBDEC);
						break;
					default:
						map_set_cell_type(map,x,y,CELL_EMPTY);
						break;
			}
		}}



void display_bonus_aux(struct map* map, int x, int y){
	if(map_get_cell_type(map,x,y) == CELL_BOX){
		switch(map_get_compose_type(map,x,y)){
			case CELL_BOX_LIFE:
				map_set_cell_type(map,x,y,CELL_EXPLOSION_LIFE);
				break;
			case CELL_BOX_BOMBDEC:
				map_set_cell_type(map,x,y,CELL_EXPLOSION_DECBOMB);
				break;
			case CELL_BOX_BOMBINC:
				map_set_cell_type(map,x,y,CELL_EXPLOSION_INCBOMB);
				break;
			default:
				map_set_cell_type(map,x,y,CELL_EXPLOSION);
				break;
		}
	}
}


void player_gets_harmed(struct game* game, struct map* map){
	struct player* player = game_get_player(game);
	int x = player_get_x(player);
	int y = player_get_y(player);
	int done = 1;
			while(map_get_compose_type(map,x,y) == CELL_EXPLOSION && player_get_nb_lives(player) > 0 && done ){
			  player_dec_nb_lives(player);
				done = 0;
	}
}







/*void display_box_bonus(struct map* map, int x, int y, int temps, int temps_fixe){
	unsigned char type = map_get_compose_type(map,x,y);
	if(temps - temps_fixe <= 5000){
		map_set_cell_type(map,x,y,CELL_EXPLOSION);
	}
	else if(temps- temps_fixe > 5000){
		map_set_cell_type(map,x,y,CELL_BONUS_LIFE);
	}
}*/



	/*while(bomb_get_next(bomb) == 1){
		int current_time = SDL_GetTicks();
		if(current_time - bomb_get_timer(bomb) < 2000){
					map_set_cell_type(game_get_current_map(game),bomb_get_x(bomb),bomb_get_y(bomb),CELL_BOMB_1);
		}
	}*/
