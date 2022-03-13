#ifndef MONSTER_H_
#define MONSTER_H_

#include <map.h>
#include <constant.h>
#include <sprite.h>

struct monster;

struct monster* monster_init();  // initialiser La position du monstre 

void monster_set_position(struct monster *monster, int x, int y);	

void monster_free(struct monster* monster);

int monster_get_x(struct monster* monster);

int monster_get_y(struct monster* monster);

void monster_set_current_way(struct monster* monster, enum direction way);

int monster_move(struct monster* monster, struct map* map) ;

void monsters_display(struct monster* monster, struct map* map);

void monsters_next_move(struct monster*monster,struct map* map);

int monster_get_direction(struct monster* monster);

struct monster* monster_get_next_monster(struct monster* monster);

struct monster* monster_fix_position(struct monster* monster,int x, int y, int level);

struct monster* kill_monster(struct monster* monster,int x, int y);

int monster_is_alive(struct monster* monster);

void create_monsters_fom_map(struct monster* monster, struct map* map);

int monster_get_level(struct monster* monster); 

#endif