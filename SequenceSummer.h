#pragma once
#include "IntervalGenerator.h"

template<typename T, int Power>
class SequenceSummer {
private:
	IntervalGenerator<T, Power> gen;
	typedef std::unordered_map<Interval<T>, T, typename Interval<T>::Hasher> StorageType;
	typedef typename std::unordered_map<Interval<T>, T, typename Interval<T>::Hasher>::const_iterator IteratorType;
	//typedef std::map<Interval<T>, T, typename Interval<T>::Comparator> StorageType;
	//typedef typename std::map<Interval<T>, T, typename Interval<T>::Comparator>::const_iterator IteratorType;
	StorageType storage;
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
		T current = storage[interval];
		current += value;
		storage[interval] = current;
	}
	T IntervalSum(const Interval<T>& interval) const {
		//T ret = storage[interval];
		//return ret;
		IteratorType iter = storage.find(interval);
		if (iter == storage.end()) {
			return 0;
		}
		return iter->second;
		//*/
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
		Interval<T> interval = gen.GetLowestInterval(index);
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
