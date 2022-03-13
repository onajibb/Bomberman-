/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <assert.h>
#include <time.h>
#include <bomb.h>
#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>

#include <player.h>
#include <unistd.h>
#include <map.h>


struct game {
	struct map** maps;       // the game's map
	short levels;        // nb maps of the game
	short level;
	struct player* player;
	struct monster* monster;
	struct bomb* bomb;
};


struct game* game_new(void) {
	sprite_load(); // load sprites into process memory

	struct game* game = malloc(sizeof(*game));
	game->maps = malloc(sizeof(struct game));
	game->maps[0] = read_map(0);
	game->maps[1] = read_map(1);
	game->maps[2] = read_map(2);
	game->maps[3] = read_map(3);
	game->maps[4] = read_map(4);
	game->maps[5] = read_map(5);
	game->maps[6] = read_map(6);
	game->maps[7] = read_map(7);
	

	game->levels = 7;
	game->level = 0;
	game->player = player_init(3,6);
	game->bomb = bomb_init();

	// Set default location of the player
	player_set_position(game->player, 1, 0);
	create_monsters_fom_map(game->monster, game->maps[0]);
	create_monsters_fom_map(game->monster, game->maps[1]);
	create_monsters_fom_map(game->monster, game->maps[2]);
	create_monsters_fom_map(game->monster, game->maps[3]);
	create_monsters_fom_map(game->monster, game->maps[4]);
	create_monsters_fom_map(game->monster, game->maps[5]);
	create_monsters_fom_map(game->monster, game->maps[6]);
	create_monsters_fom_map(game->monster, game->maps[7]);




	return game;
}

void game_free(struct game* game) {
	assert(game);

	player_free(game->player);
	for (int i = 0; i < game->levels; i++)
		map_free(game->maps[i]);
}

struct game* game_next_map(struct game* game)
	{
		game->level = game->level +1;
		return game;

	}

struct game* game_previous_map(struct game* game)
	{
		game->level = game->level -1;
		return game;

	}



struct map* game_get_current_map(struct game* game) {
	assert(game);
	return game->maps[game->level];
}


struct player* game_get_player(struct game* game) {
	assert(game);
	return game->player;
}

struct monster* game_get_monster(struct game* game) {
	assert(game);
	return game->monster;
}



void game_banner_display(struct game* game) {
	assert(game);

	struct map* map = game_get_current_map(game);

	int y = (map_get_height(map)) * SIZE_BLOC;
	for (int i = 0; i < map_get_width(map); i++)
		window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);

	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 8 * SIZE_BLOC) / 4;
	int x = white_bloc;
	y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
	window_display_image(sprite_get_banner_life(), x, y);

	x = white_bloc + SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_nb_lives(game_get_player(game))), x, y);

	x = 2*white_bloc + 2* SIZE_BLOC;
	window_display_image(sprite_get_banner_bomb(), x, y);

	x = 2 * white_bloc + 3 * SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_nb_bomb(game_get_player(game))), x, y);

	x = 3 * white_bloc + 4 * SIZE_BLOC;
	window_display_image(sprite_get_banner_range(), x, y);

	x = 3 * white_bloc +  5* SIZE_BLOC;
	window_display_image(sprite_get_number(3), x, y);
	x= 4* white_bloc + 5* SIZE_BLOC;
	window_display_image(sprite_get_key(),x,y);
	x= 4* white_bloc + 6*SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_nb_key(game_get_player(game))),x,y);

}

struct bomb* game_get_bomb(struct game* game){
	assert(game);
	return game->bomb;

}

void game_display(struct game* game) {

	assert(game);
	window_clear();
	game_banner_display(game);
	map_display(game_get_current_map(game));
	player_display(game->player);
	monsters_display(game->monster,game_get_current_map(game));
	window_refresh();
	bomb_meca(game);
}



static short input_keyboard(struct game* game) {
	SDL_Event event;
	struct player* player = game_get_player(game);
	struct map* map = game_get_current_map(game);
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return 1;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				return 1;
			case SDLK_UP:
				player_set_current_way(player, NORTH);
				player_move(player, map);
				break;
			case SDLK_DOWN:
				player_set_current_way(player, SOUTH);
				player_move(player, map);
				break;
			case SDLK_RIGHT:
				player_set_current_way(player, EAST);
				player_move(player, map);
				break;
			case SDLK_LEFT:
				player_set_current_way(player, WEST);
				player_move(player, map);
				break;
			case SDLK_SPACE:
				game->bomb = bomb_add(game->bomb,game);
				break;
				default:
				break;
				}
				break;
	}
}
	return 0;
}

int game_update(struct game* game) {
	if (input_keyboard(game))
		return 1; // exit game
	//update(game_get_current_map (game), temps , temps_entree);
	
	static int t=0;
	t+=1;

	static int x=0;
	x+=1;


	if(chamge_map()-t)
		{
			game = game_next_map(game);
		
		t+=1;
		}
	if(change_map()-x)
			{
				game = game_previous_map(game);
			
			x+=1;
			}

	int time=SDL_GetTicks();
	static int temps_entree =0;
	monster_update( game, &time, &temps_entree);
	//bomb_meca(game);
	return 0;


}




void monster_update(struct game* game, int* temps , int* temps_entree){

if(*temps_entree+2000<*temps)
	{
	monsters_next_move(game_get_monster(game), game_get_current_map( game));
	*temps_entree=*temps;
}
}




