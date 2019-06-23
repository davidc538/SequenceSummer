#include "Timer.h"

void BlockTimer::Start() {
	start = std::chrono::system_clock::now();
}
void BlockTimer::Pause() {
	end = std::chrono::system_clock::now();
}
unsigned long long BlockTimer::ElapsedNanoseconds() const {
	std::chrono::duration<double> elapsed_s = end - start;
	auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed_s);
	return time.count();
}
unsigned long long BlockTimer::ElapsedMilliseconds() const {
	std::chrono::duration<double> elapsed_s = end - start;
	auto time = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_s);
	return time.count();
}