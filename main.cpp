#include <vector>
#include <unordered_map>
#include <map>
#include <iostream>
#include <chrono>

class BlockTimer {
private:
	std::chrono::time_point<std::chrono::system_clock> start, end;
public:
	void Start() {
		start = std::chrono::system_clock::now();
	}
	void Pause() {
		end = std::chrono::system_clock::now();
	}
	unsigned long long ElapsedNanoseconds() const {
		std::chrono::duration<double> elapsed_s = end - start;
		auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed_s);
		return time.count();
	}
	unsigned long long ElapsedMilliseconds() const {
		std::chrono::duration<double> elapsed_s = end - start;
		auto time = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_s);
		return time.count();
	}
};

unsigned long long PowerOf2(int power) {
	return (1 << power);
}

template<typename T>
class Interval {
public:
	int PowerOfTwo;
	T Start;
	T Length() const {
		T ret = PowerOf2(PowerOfTwo);
		return ret;
	}
	T End() const {
		T length = Length();
		T ret = Start + length - 1;
		return ret;
	}
	bool operator==(const Interval<T>& other) const {
		bool eql1 = (Start == other.Start);
		bool eql2 = (PowerOfTwo == other.PowerOfTwo);
		return (eql1 && eql2);
	}
	struct Hasher {
		std::size_t operator()(const Interval<T>& interval) const {
			std::size_t ret;
			ret = interval.Start | (1 << (interval.PowerOfTwo - 1));
			return ret;
		}
	};
	struct Comparator {
		bool operator()(const Interval<T>& lhs, const Interval<T>& rhs) const {
			if (lhs.Start < rhs.Start) {
				return true;
			} else if (lhs.Start > rhs.Start) {
				return false;
			} else if (lhs.PowerOfTwo < rhs.PowerOfTwo) {
				return true;
			} else if (lhs.PowerOfTwo > rhs.PowerOfTwo) {
				return false;
			} else {
				return false;
			}
		}
	};
};

template<typename T, int Power>
class IntervalGenerator {
public:
	static T GreatestMultipleOfKLessThanOrEqualToN(T N, T K) {
		T ret = N - (N % K);
		return ret;
	}
	std::vector<Interval<T>> GetIntervals(T N) const {
		std::vector<Interval<T>> ret;
		for (int i = 0; i < Power; i++) {
			Interval<T> ToAdd;
			ToAdd.PowerOfTwo = i;
			ToAdd.Start = GreatestMultipleOfKLessThanOrEqualToN(N, PowerOf2(i));
			ret.push_back(ToAdd);
		}
		return ret;
	}
private:
	Interval<T> GetBiggestInterval(T start, T end) const {
		Interval<T> ret;
		ret.PowerOfTwo = ret.Start = 0;
		for (int i = 0; i < Power; i++) {
			T multiple = PowerOf2(i);
			T interval_start = GreatestMultipleOfKLessThanOrEqualToN(start, multiple);
			bool overflows_left = (interval_start < start);
			bool overflows_right = (interval_start + multiple > end);
			bool reaches_left = (interval_start <= start);
			bool reaches_right = (interval_start + multiple >= end);
			bool overflows_both = (overflows_left | overflows_right);
			bool reaches_both = (reaches_left & reaches_right);
			if (overflows_both) {
				ret.PowerOfTwo = i - 1;
			} else if (reaches_both) {
				ret.PowerOfTwo = i;
			}
			if (overflows_both | reaches_both) {
				ret.Start = GreatestMultipleOfKLessThanOrEqualToN(start, PowerOf2(ret.PowerOfTwo));
				return ret;
			}
		}
		return ret;
	}
public:
	std::vector<Interval<T>> GetIntervals(T start, T end) const {
		std::vector<Interval<T>> ret;
		T iter = start;
		do {
			Interval<T> interval = GetBiggestInterval(iter, end);
			iter = interval.End() + 1;
			ret.push_back(interval);
		} while (iter < end);
		return ret;
	}
};

template<typename T, int Power>
class SequenceSummer {
private:
	IntervalGenerator<T, Power> gen;
	std::unordered_map<Interval<T>, T, typename Interval<T>::Hasher> storage;
	//std::map<Interval<T>, T, typename Interval<T>::Comparator> storage;
	bool IsIntervalSet(const Interval<T>& interval) const {
		bool ret = (storage.find(interval) != storage.end());
		return ret;
	}
	void EnsureIntervalIsSet(const Interval<T>& interval) {
		if (!IsIntervalSet(interval)) {
			storage[interval] = 0;
		}
	}
	void AddToInterval(const Interval<T>& interval, T value) {
		EnsureIntervalIsSet(interval);
		storage[interval] += value;
	}
	T IntervalSum(const Interval<T>& interval) const {
		auto iter = storage.find(interval);
		if (iter == storage.end()) {
			return 0;
		}
		return iter->second;
	}
public:
	void Set(unsigned long long index, T value) {
		std::vector<Interval<T>> intervals = gen.GetIntervals(index);
		T current = IntervalSum(intervals[0]);
		T difference = value - current;
		for (const Interval<T>& interval : intervals) {
			AddToInterval(interval, difference);
		}
	}
	T Get(unsigned long long index) const {
		Interval<T> interval;
		interval.Start = index;
		interval.PowerOfTwo = 0;
		T ret = IntervalSum(interval);
		return ret;
	}
	T SumRange(unsigned long long start, unsigned long long end) const {
		if (start == end) {
			return Get(start);
		}
		T ret = 0;
		std::vector<Interval<T>> intervals = gen.GetIntervals(start, end);
		for (const Interval<T>& interval : intervals) {
			if (IsIntervalSet(interval)) {
				T interval_sum = IntervalSum(interval);
				ret += interval_sum;
			}
		}
		return ret;
	}
};

void TimeAdd(unsigned long long max) {
	std::vector<unsigned long long> nums;
	unsigned long long current = 1;
	for (unsigned long long i = 0; i < max; i++) {
		nums.push_back(current);
		current += 2;
	}
	unsigned long long sum = 0;
	BlockTimer timer;
	timer.Start();
	for (const unsigned long long& i : nums) {
		sum += i;
	}
	timer.Pause();
	std::cout << "calculated " << sum << " in " << timer.ElapsedNanoseconds() << " ns." << std::endl;
}

int main(int argc, char** argv) {
	BlockTimer timer;
	SequenceSummer<unsigned long long, 32> summer;
	unsigned long long max = 1 << 20;
	TimeAdd(max);
	timer.Start();
   	unsigned long long current = 1;
	for (unsigned long long i = 0; i < max; i++) {
		summer.Set(i, current);
		current += 2;
	}
	timer.Pause();
	std::cout << "done setting " << max << " numbers in " << timer.ElapsedMilliseconds() << " ms." << std::endl;
	timer.Start();
	unsigned long long total = summer.SumRange(0, max - 1);
	timer.Pause();
	std::cout << std::endl << "Sum: " << total << " computed in: " << timer.ElapsedNanoseconds() << " ns.";
	std::cout << std::endl << "Sum: " << summer.SumRange(4, 6);
	std::cout << std::endl << "Sum: " << summer.SumRange(4, max -3);
	std::cout << std::endl << "Sum: " << summer.SumRange(4, 4);
	std::cout << std::endl << "Sum: " << summer.SumRange(0, max);
	summer.Set(0, 0);
	std::cout << std::endl << "Sum: " << summer.SumRange(0, max);
	std::cout << std::endl << "Sum: " << summer.SumRange(1, max - 1);
	std::cout << std::endl << "sizeof(std::size_t) = " << sizeof(std::size_t);
	// IntervalGenerator test
	IntervalGenerator<unsigned long long, 32> gen;
	std::vector<Interval<unsigned long long>> intervals = gen.GetIntervals(1, max - 1);
	for (const Interval<unsigned long long>& interval : intervals) {
		std::cout << interval.Start << ", " << interval.PowerOfTwo << std::endl;
	}
	std::cout << std::endl;
	//std::cin.get();
}
