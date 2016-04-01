/*-------------------------------------------------------------------------- -
Project Title         :        CS 230: Project 2 Part 2
File Name             :        GameStateMgr.h
Author                :        Shawn Masters
Creation Date         :        1/30/16
Purpose               :        Prototypes functions for GameStatemgr.c
History
- 1/30/16             :        Added file header.
© Copyright 1996 - 2016, DigiPen Institute of Technology(USA).All rights reserved.
----------------------------------------------------------------------------*/

#ifndef GAME_STATE_MGR_H
#define GAME_STATE_MGR_H

// ---------------------------------------------------------------------------

#include "AEEngine.h"

// ---------------------------------------------------------------------------
// include the list of game states

#include "GameStateList.h"

// ---------------------------------------------------------------------------
// externs

extern unsigned int gGameStateNext;


// ---------------------------------------------------------------------------
// Function prototypes

// call this at the beginning and AFTER all game states are added to the manager
void GameStateMgrInit(unsigned int gameStateInit);

// update is used to set the function pointers
void GameStateMgrUpdate();

// Main flow
void GSM_MainLoop(void);

// ---------------------------------------------------------------------------

#endif // AE_GAME_STATE_MGR_H