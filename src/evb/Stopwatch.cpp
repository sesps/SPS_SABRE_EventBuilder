/*
	Stopwatch.cpp
	Simple class designed to provide timing info on parts of the process.
	Only for use in development.

	Written by G.W. McCann Oct. 2020
*/
#include "Stopwatch.h"

namespace EventBuilder {

	Stopwatch::Stopwatch() 
	{
		start_time = Clock::now();
		stop_time = start_time;
	}
	
	Stopwatch::~Stopwatch() {}
	
	void Stopwatch::Start() 
	{
		start_time = Clock::now();
	}
	
	void Stopwatch::Stop() 
	{
		stop_time = Clock::now();
	}
	
	double Stopwatch::GetElapsedSeconds() 
	{
		return std::chrono::duration_cast<std::chrono::duration<double>>(stop_time-start_time).count();
	}
	
	double Stopwatch::GetElapsedMilliseconds() 
	{
		return std::chrono::duration_cast<std::chrono::duration<double>>(stop_time-start_time).count()*1000.0;
	}

}