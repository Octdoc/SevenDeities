#pragma once

namespace hcs
{
	class Timer
	{
		long long startTime;
		long long lastCallToUpdate;
		long long currentCallToUpdate;
		long long frequency;

	public:
		Timer();
		void Reset();
		void Update();
		double GetTimeTotal();
		double GetTimeDelta();
	};
}