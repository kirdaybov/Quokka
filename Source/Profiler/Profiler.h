#pragma once 
#include <iostream>
#include <string>
#include <map>

#include "Common/Singletone.h"
#include "Common/Timer.h"

namespace quokka
{
	struct ProfilerStamp
	{
		ProfilerStamp() : Start(0.f), Stop(0.f), Sum(0.f) {}
		float Start;
		float Stop;
		float Sum;
	};

	class Profiler : public Singletone<Profiler>
	{
	public:
		void Start(std::string Name)
		{
			ProfilerStamp ps;
			ps.Start = Timer()->GetGlobalTime();
			Stamps[Name] = ps;
		}

		void Stop(std::string Name)
		{
			Stamps[Name].Stop = Timer()->GetGlobalTime();
			Stamps[Name].Sum = Stamps[Name].Sum + (Stamps[Name].Stop - Stamps[Name].Start);
		}

		void Print()
		{
			for(std::map<std::string, ProfilerStamp>::const_iterator it = Stamps.begin(); it != Stamps.end(); ++it)
			{
				std::cout << std::endl << it->first << ": " << it->second.Sum << " seconds";
			}
		}

	private:
		std::map<std::string, ProfilerStamp> Stamps;
	};

	Profiler* GProfiler();
}