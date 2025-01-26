#pragma once

#include <chrono>
#include <thread>
#include <iostream>
#include <stdexcept>
#include <cmath>

class Clock {
public:
	Clock();
	~Clock();

	// initializes clock
	void init();

	// begins frame timer
	void startFrame();

	// ns since start of frame
	long getFrameDelta();

	// nanoseconds the programs been running
	// max long long value: 9223372036854775807 -- in nanosec = ~2562047 hours = ~292 years
	long long getProgramTimeNs();

	// for simplicity sake
	float getProgramSeconds();

	// sleep calling thread for specified duration
	void waitNs(long ns);

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start_;
	std::chrono::time_point<std::chrono::high_resolution_clock> frameStart_;

	long long totalNs_ = 0;

};
