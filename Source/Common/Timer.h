#pragma once 
#include "Singletone.h"
#include <chrono>

namespace quokka
{	
	class Timer
	{
	public:
		virtual void CalculateFrameTime() = 0;
		virtual float GetFrameTime() = 0;
		virtual float GetGlobalTime() = 0;
	};

	class STDChronoTimer : public Timer, public Singletone<STDChronoTimer>
	{
	public:

		typedef std::chrono::milliseconds Duration;

		STDChronoTimer()
			:Last(std::chrono::high_resolution_clock::now())
			,Start(Last)
		{}
		virtual ~STDChronoTimer(){}

		// Call only once per frame!
		virtual void CalculateFrameTime()
		{
			std::chrono::high_resolution_clock::time_point Now = std::chrono::high_resolution_clock::now();
			LastFrameDuration = std::chrono::duration_cast<Duration>(Now - Last).count();
			Last = Now;
		}

		virtual float GetFrameTime() { return LastFrameDuration/1000.0f; }

		virtual float GetGlobalTime()
		{
			std::chrono::high_resolution_clock::time_point Now = std::chrono::high_resolution_clock::now();
			return std::chrono::duration_cast<Duration>(Now - Start).count()/1000.f;
		}

	private:
		long long LastFrameDuration;
		std::chrono::high_resolution_clock::time_point Last;
		std::chrono::high_resolution_clock::time_point Start;
	};

	STDChronoTimer* STDChronoTimer::Instance = NULL;
  
  static Timer* Timer() { return STDChronoTimer::GetInstance(); }
}