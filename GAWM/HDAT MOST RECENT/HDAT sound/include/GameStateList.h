/*-------------------------------------------------------------------------- -
Project Title         :        CS 230: Project 2 Part 2
File Name             :        GameStateList.h
Author                :        Shawn Masters
Creation Date         :        1/30/16
Purpose               :        Header file for the game states.
History
- 1/30/16             :        Added file header.
© Copyright 1996 - 2016, DigiPen Institute of Technology(USA).All rights reserved.
----------------------------------------------------------------------------*/

#ifndef GAME_STATE_LIST_H
#define GAME_STATE_LIST_H

// ---------------------------------------------------------------------------
// game state list

enum
{
	// list of all game states

	// World 1
	GS_ASTEROIDS_LEVEL1 = 0,
	GS_ASTEROIDS_LEVEL2,
	GS_ASTEROIDS_LEVEL3,	// Boss Level

							// World 2
	GS_ASTEROIDS_LEVEL4,
	GS_ASTEROIDS_LEVEL5,
	GS_ASTEROIDS_LEVEL6,	// Boss Level

							// World 3
	GS_ASTEROIDS_LEVEL7,
	GS_ASTEROIDS_LEVEL8,
	GS_ASTEROIDS_LEVEL9,	// Boss Level

							// World 4
	GS_ASTEROIDS_LEVEL10,
	GS_ASTEROIDS_LEVEL11,
	GS_ASTEROIDS_LEVEL12,	// Boss Level

	GS_ASTEROIDS_START_SCREEN,
	GS_ASTEROIDS_SPLASH_SCREEN,
	GS_ASTEROIDS_VICTORY_SCREEN,
	GS_ASTEROIDS_CREDITS_SCREEN,
	GS_ASTEROIDS_WIN_SCREEN,

	// special game state. Do not change
	GS_RESTART,
	GS_QUIT,
	GS_NUM
};


// ---------------------------------------------------------------------------

#endif // GAME_STATE_LIST_H