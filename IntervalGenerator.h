#pragma once
#include "Interval.h"

template<typename T, int Power>
class IntervalGenerator {
private:
	T PowerOf2(int power) const {
		return (1 << power);
	}
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
			}
			else if (reaches_both) {
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
	Interval<T> GetLowestInterval(T index) const {
		Interval<T> interval;
		interval.Start = index;
		interval.PowerOfTwo = 0;
		return interval;
	}
};
