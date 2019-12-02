#include "Time.h"

Time::Time()
{
	// Initialising variables
	m_counts_per_second = 0.0;
	m_delta_time = 0.0;

	m_frame_count= 0;
	fps = 0;

	m_counter_start = 0;
	m_prev_delta_time = 0;

}

Time::~Time()
{

}

void Time::Execute()
{
	// We increase it every tick
	m_frame_count++;

	if (GetTime() > 1.0f)
	{
		// We set the fps
		fps = m_frame_count;
		m_frame_count = 0;


		StartTimer();
	}

	// Delta Time is calculated
	m_delta_time = CalculateDeltaTime();

}

void Time::StartTimer()
{
	LARGE_INTEGER frequencyCount;

	// Retrieves the frequency of the performance
	// counter
	QueryPerformanceFrequency(&frequencyCount);

	// Storing the frequency
	m_counts_per_second = double(frequencyCount.QuadPart);

	// Retrieving the current value of the performance counter
	QueryPerformanceCounter(&frequencyCount);
	m_counter_start = frequencyCount.QuadPart;

}

double Time::GetTime()
{
	LARGE_INTEGER currentTime;

	QueryPerformanceCounter(&currentTime);

	return double(currentTime.QuadPart - m_counter_start) / m_counts_per_second;
}

double Time::CalculateDeltaTime()
{
	LARGE_INTEGER currentTime;
	__int64 tickCount;
	QueryPerformanceCounter(&currentTime);

	// Retrieving the difference between the current performance
	// counter and the previous delta time
	tickCount = currentTime.QuadPart - m_prev_delta_time;
	m_prev_delta_time = currentTime.QuadPart;

	// Clamping it to zero
	if (tickCount < 0.0f)
	{
		tickCount = 0.0f;
	}

	return float(tickCount) / m_counts_per_second;
}

double Time::GetDeltaTime()
{
	return m_delta_time;
}

int Time::GetFPS()
{
	return fps;
}