#include <SDL/SDL_image.h>
#include <assert.h>
#include <map.h>
#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>

struct bomb;

void bomb_meca(struct game*game);

int bomb_get_next(struct bomb* bomb);

struct bomb* bomb_add(struct bomb*bomb, struct game* game);

struct bomb* bomb_init();

int bomb_get_y(struct bomb*bomb);

int bomb_get_x(struct bomb*bomb);

void set_fire(struct game* game, int x, int y);

void stop_fire(struct map*map,int x, int y);

void player_gets_harmed(struct game* game, struct map* map);

void display_bonus_aux(struct map* map, int x, int y); 
