#pragma once

template<typename T>
class Interval {
private:
	T PowerOf2(int power) const {
		return (1 << power);
	}
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
		bool equal = (eql1 & eql2);
		return equal;
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
