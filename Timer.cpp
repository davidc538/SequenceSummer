#include "Timer.h"

void Timer::Start() {
	start = std::chrono::system_clock::now();
}
void Timer::Pause() {
	end = std::chrono::system_clock::now();
}
unsigned long long Timer::ElapsedNanoseconds() const {
	std::chrono::duration<double> elapsed_s = end - start;
	auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed_s);
	return time.count();
}
unsigned long long Timer::ElapsedMilliseconds() const {
	std::chrono::duration<double> elapsed_s = end - start;
	auto time = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_s);
	return time.count();
}
