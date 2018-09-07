#include "hcs_timer.h"
#include "hcs_helpers.h"

namespace hcs
{
	Timer::Timer()
	{
		LARGE_INTEGER t;
		QueryPerformanceFrequency(&t);
		frequency = t.QuadPart;

		Reset();
	}
	void Timer::Reset()
	{
		LARGE_INTEGER t;
		QueryPerformanceCounter(&t);
		startTime = t.QuadPart;
		currentCallToUpdate = t.QuadPart;
		lastCallToUpdate = t.QuadPart;
	}
	void Timer::Update()
	{
		lastCallToUpdate = currentCallToUpdate;
		LARGE_INTEGER t;
		QueryPerformanceCounter(&t);
		currentCallToUpdate = t.QuadPart;
	}
	double Timer::GetTimeTotal()
	{
		double d = (double)(currentCallToUpdate - startTime);
		return d / (double)frequency;
	}
	double Timer::GetTimeDelta()
	{
		double d = (double)(currentCallToUpdate - lastCallToUpdate);
		return d / (double)frequency;
	}
}