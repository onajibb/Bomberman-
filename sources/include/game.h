/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef GAME_H_
#define GAME_H_

#include <player.h>
#include <map.h>
#include <monster.h>

// Abstract data type
struct game;

// Create a new game
struct game* game_new();

// Free a game
void game_free(struct game* game);

// Return the player of the current game
struct player* game_get_player(struct game* game);

// Return the monster of the current game
struct monster* game_get_monster(struct game* game);

// Return the current map
struct map* game_get_current_map(struct game* game);

// Display the game on the screen
void game_display(struct game* game);

// update
int game_update(struct game* game );

void monster_update(struct game* game,int* temps , int* temps_entree);



struct game* game_next_map(struct game* game);

struct game* game_previous_map(struct game* game);


// bomb
struct bomb * game_get_bomb(struct game* game);
#endif /* GAME_H_ */
