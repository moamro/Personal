/*-------------------------------------------------------------------------- -
Project Title         :        CS 230: Project 2 Part 2
File Name             :        GameStateMgr.c
Author                :        Shawn Masters
Creation Date         :        1/30/16
Purpose               :        Runs the game state manager.
History
- 1/30/16             :        Added file header.
© Copyright 1996 - 2016, DigiPen Institute of Technology(USA).All rights reserved.
----------------------------------------------------------------------------*/

#include "GameStateMgr.h"
#include "GameState_AsteroidsDefines.h"
#include "GameState_Asteroids.h"
#include <stdio.h>

// ---------------------------------------------------------------------------
// globals

// variables to keep track the current, previous and next game state
static unsigned int	gGameStateInit;
static unsigned int	gGameStateCurr;
static unsigned int	gGameStatePrev;
unsigned int	gGameStateNext;

// pointer to functions for game state life cycles functions
void (*GameStateLoad)(void)		= 0;
void (*GameStateInit)(void)		= 0;
void (*GameStateUpdate)(void)	= 0;
void (*GameStateDraw)(void)		= 0;
void (*GameStateFree)(void)		= 0;
void (*GameStateUnload)(void)	= 0;

// ---------------------------------------------------------------------------
// Functions implementations

void GameStateMgrInit(unsigned int gameStateInit)
{
	// set the initial game state
	gGameStateInit = gameStateInit;

	// reset the current, previoud and next game
	gGameStateCurr = 
	gGameStatePrev = 
	gGameStateNext = gGameStateInit;

	// call the update to set the function pointers
	GameStateMgrUpdate();
}

// ---------------------------------------------------------------------------

void GameStateMgrUpdate()
{
	if ((gGameStateCurr == GS_RESTART) || (gGameStateCurr == GS_QUIT))
		return;

	switch (gGameStateCurr)
	{
	case GS_ASTEROIDS_LEVEL1:
		GameStateLoad = GameStateAsteroidsLoad;
		GameStateInit = GameStateAsteroidsInit;
		GameStateUpdate = GameStateAsteroidsUpdate;
		GameStateDraw = GameStateAsteroidsDraw;
		GameStateFree = GameStateAsteroidsFree;
		GameStateUnload = GameStateAsteroidsUnload;
		break;

	default:
		AE_FATAL_ERROR("invalid state!!");
	}
}

// ---------------------------------------------------------------------------


void GSM_MainLoop(void)
{
	while (gGameStateCurr != GS_QUIT)
	{
		// reset the system modules
		AESysReset();

		// If not restarting, load the gamestate
		if (gGameStateCurr != GS_RESTART)
		{
			GameStateMgrUpdate();
			GameStateLoad();
		}
		else
			gGameStateNext = gGameStateCurr = gGameStatePrev;

		// Initialize the gamestate
		GameStateInit();

		while (gGameStateCurr == gGameStateNext)
		{
			AESysFrameStart();

			AEInputUpdate();

			GameStateUpdate();

			GameStateDraw();

			AESysFrameEnd();

			// check if forcing the application to quit
			if ((0 == AESysDoesWindowExist()) || AEInputCheckTriggered(VK_ESCAPE))
			{
				gGameStateNext = GS_QUIT;
				CurrentLevel = GS_QUIT;
			}

			if (changeLevelFlag == 1)
			{
				break;
			}
		}

		GameStateFree();

		if (changeLevelFlag == 1)
		{
			CurrentLevel = NextLevel;
			gGameStateNext = GS_RESTART;
			changeLevelFlag = 0;
		}

		if (gGameStateNext != GS_RESTART)
			GameStateUnload();

		gGameStatePrev = gGameStateCurr;
		gGameStateCurr = gGameStateNext;

	}
}


// ---------------------------------------------------------------------------
