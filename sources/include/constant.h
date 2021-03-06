/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef CONSTANT_H_
#define CONSTANT_H_

#define WINDOW_NAME "[PG110] Bombeirb 2018-2019"

// Size (# of pixels) of a cell of the map
#define SIZE_BLOC       40

// Size (# of pixels) of banner
#define LINE_HEIGHT	4
#define BANNER_HEIGHT	40

// Time management
#define DEFAULT_GAME_FPS 30

// map size
#define STATIC_MAP_WIDTH  12
#define STATIC_MAP_HEIGHT 12

enum direction {
	NORTH = 0,
	EAST,  // 1
	SOUTH, // 2
	WEST   // 3
};

// Time

#define TIME_LEVEL_1 2000
#define TIME_LEVEL_2 1700
#define TIME_LEVEL_3 1500
#define TIME_LEVEL_4 1300
#define TIME_LEVEL_5 1100
#define TIME_LEVEL_6 900
#define TIME_LEVEL_7 700
#define TIME_LEVEL_8 500


#endif /* CONSTANT */
