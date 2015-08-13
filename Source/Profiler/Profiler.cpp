#include "Profiler.h"

namespace quokka
{
	Profiler* Profiler::Instance = new Profiler();
	Profiler* GProfiler() { return Profiler::GetInstance(); } 
}