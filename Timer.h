#pragma once
#include <chrono>

class BlockTimer {
private:
	std::chrono::time_point<std::chrono::system_clock> start, end;
public:
	void Start();
	void Pause();
	unsigned long long ElapsedNanoseconds() const;
	unsigned long long ElapsedMilliseconds() const;
};
