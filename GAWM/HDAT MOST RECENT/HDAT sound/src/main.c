/*-------------------------------------------------------------------------- -
Project Title         :        CS 230: Project 2 Part 2
File Name             :        main.c
Author                :        Shawn Masters
Creation Date         :        1/30/16
Purpose               :        Initialized window and calls other functions.
History
- 1/30/16             :        Added file header.
© Copyright 1996 - 2016, DigiPen Institute of Technology(USA).All rights reserved.
----------------------------------------------------------------------------*/

// ---------------------------------------------------------------------------
// includes

#include "main.h"
#include "GameState_AsteroidsDefines.h"


// ---------------------------------------------------------------------------
// Static function protoypes


// ---------------------------------------------------------------------------
// main

// Initialize the system 
AESysInitInfo sysInitInfo;

int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show)
{
	sysInitInfo.mAppInstance		= instanceH;
	sysInitInfo.mShow				= show;
	sysInitInfo.mWinWidth			= 1920; 
	sysInitInfo.mWinHeight			= 1080;
	sysInitInfo.mCreateConsole		= 1;
	sysInitInfo.mMaxFrameRate		= 60;
	sysInitInfo.mpWinCallBack		= NULL;//MyWinCallBack;
	sysInitInfo.mClassStyle			= CS_HREDRAW | CS_VREDRAW;											
	sysInitInfo.mWindowStyle		= WS_OVERLAPPEDWINDOW;//WS_POPUP | WS_VISIBLE | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	sysInitInfo.mCreateWindow		= 1;
	sysInitInfo.mWindowHandle		= NULL;
	sysInitInfo.mHandleWindowMessages = 1;

	// Initialize the system
	if (0 == AESysInit(&sysInitInfo))
		return 1;

	GameStateMgrInit(GS_ASTEROIDS_LEVEL1);

	while (CurrentLevel != GS_QUIT)
	{
		GSM_MainLoop();
	}
	// free the system
	AESysExit();

	return 1;
}

// ---------------------------------------------------------------------------