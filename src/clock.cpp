#include "clock.h"

Clock::Clock() {}

Clock::~Clock() {}

// initializes clock
void Clock::init() {
	// set a start timepoint
	start_ = std::chrono::high_resolution_clock::now();
}

// begins frame timer
void Clock::startFrame() {
	frameStart_ = std::chrono::high_resolution_clock::now();
}

// ns since start of frame
long Clock::getFrameDelta() {
	auto timepoint = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(timepoint - frameStart_);
	return duration.count();
}

// milliseconds the program has been active
// max long long value: 9223372036854775807 -- in microsec = ~2562047788 hours
long long Clock::getProgramTimeNs() {
	auto timepoint = std::chrono::high_resolution_clock::now();
	totalNs_ = std::chrono::duration_cast<std::chrono::nanoseconds>(timepoint - start_).count();
	return totalNs_;
}

float Clock::getProgramSeconds() {
	auto timepoint = std::chrono::high_resolution_clock::now();
	return std::chrono::duration<float, std::chrono::seconds::period>(timepoint - start_).count();
}

// sleep calling thread for specified duration
void Clock::waitNs(long ns) {
	std::this_thread::sleep_for(std::chrono::nanoseconds(ns));
}