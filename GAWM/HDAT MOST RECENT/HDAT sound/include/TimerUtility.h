/*-------------------------------------------------------------------------- -
Project Title         :        HDAT
File Name             :        TimerUtility.h
Author                :        Mohammad Amro
Creation Date         :        2/18/16
Purpose               :        Prototypes struct for Timer functions.
History
- 1/30/16             :        Added file header.
© Copyright 1996 - 2016, DigiPen Institute of Technology(USA).All rights reserved.
----------------------------------------------------------------------------*/

#ifndef TIMER_UTILITY_H
#define TIMER_UTILITY_H

#define DONE_COUNTING	1
#define STILL_COUNTING	0

// ---------------------------------------------------------------------------

typedef struct 
{
	double duration;		// Duration of timer
	double current_time;	// The number to decrement and reset to the desired duration
	unsigned int id;

		// DONE_COUNTING if timer needs to be reset
	unsigned int doneCounting;		// Flag indicating timer needs to be reset (1 full cycle happened)
}Timer;

	// Initialize the struct for use
Timer* ConstructTimer(double timerDuration);
	
	// Re-assign the desired timer
void SetTimerDuration(Timer* TickingTimer, double duration);

	// Update counter based on frametime and flag if we exceed the duration
void UpdateTimer(Timer* TickingTimer);

	// Reset counter when flagged (Our duty to call this after we've checked for the flag after updating the timer!
void ResetTimer(Timer* TickingTimer);

	// Free the struct, call when we're done with the timer
void DestroyTimer(Timer** DoneTimer);

// Free the struct, call when we're done with the timer
Timer* CreateComponent_Timer(Timer* pTLocation, double duration);

	// Free the struct, call when we're done with the timer
void RemoveComponent_Timer(Timer* DoneTimer);


// ---------------------------------------------------------------------------

#endif


