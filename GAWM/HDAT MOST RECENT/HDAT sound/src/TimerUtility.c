/*-------------------------------------------------------------------------- -
Project Title         :        HDAT
File Name             :        TimerUtility.c
Author                :        Mohammad Amro
Creation Date         :        2/18/16
Purpose               :        Defines struct for Timer functions.
History
- 1/30/16             :        Added file header.
© Copyright 1996 - 2016, DigiPen Institute of Technology(USA).All rights reserved.
----------------------------------------------------------------------------*/

#include "TimerUtility.h"
#include "GameState_AsteroidsDefines.h"

static unsigned int id = 0;

Timer* ConstructTimer(double timerDuration)
{
		// Create the resultant struct
	Timer *TStruct = (Timer *) calloc(1, sizeof(Timer));

	int *i = malloc(sizeof(int));
	*i = 2;

		// Initialize the struct
	TStruct->duration = timerDuration;
	TStruct->doneCounting = STILL_COUNTING;
	TStruct->current_time = 0.0;

	TStruct->id = id++;

		// Return it to the caller
	return TStruct;
}

	// Decrement the counter and flag
void UpdateTimer(Timer* TickingTimer)
{
		// Increment the timer
	TickingTimer->current_time += frameTime;

		// Check if the timer exceeded the desired duration
	if (TickingTimer->current_time >= TickingTimer->duration)
	{
			// Flag the the struct
		TickingTimer->doneCounting = DONE_COUNTING;
	}
	else
	{
		TickingTimer->doneCounting = STILL_COUNTING;
	}
}

	// Indicate to the client if the timer has been reached
	// Flag = 1 or DONE_COUNTING
	// Flag = 0 or STILL_COUNTING
void ResetTimer(Timer* TickingTimer)
{
		// Reset the flag
	TickingTimer->doneCounting = STILL_COUNTING;

		// Reset the timer
	TickingTimer->current_time = 0.0;
}

	// Update desired duration
void SetTimerDuration(Timer* TickingTimer, double duration)
{
		// Reset the flag
	TickingTimer->duration = duration;
}

	// Free the memory
void DestroyTimer(Timer* DoneTimer)
{
		// Only delete if pointer is pointing at something
	if (DoneTimer != 0)
	{
		free(DoneTimer);
		DoneTimer = 0;
	}
}

// ---------------------------------------------------------------------------

Timer* CreateComponent_Timer(Timer* pTimerLocation, double duration)
{
		// If the pointer is unoccupied, construct 1 there
	pTimerLocation = ConstructTimer(duration);

	return pTimerLocation;
}

// ---------------------------------------------------------------------------

void RemoveComponent_Timer(Timer* pTimerLocation)
{
	DestroyTimer(pTimerLocation);
}