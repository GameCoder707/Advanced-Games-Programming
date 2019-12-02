#pragma once

#include <windows.h>

class Time
{
private:

	
	double m_counts_per_second;

	// Time between each tick
	double m_delta_time;

	// Frame Count every tick
	int m_frame_count;

	// Frames per second
	int fps;

	__int64 m_counter_start;

	// Stores the delta time in the
	// previous frame
	__int64 m_prev_delta_time;

public:
	// Constructor and Destructor
	Time();
	~Time();

	// This is to store the performance
	// counter at the start of next frame
	void StartTimer();

	double GetTime();

	double CalculateDeltaTime();

	// This is to execute all other
	// time functions
	void Execute();

	// Gets delta time
	double GetDeltaTime();

	// Gets the frames per second
	int GetFPS();

};
