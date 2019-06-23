#include <vector>
#include <unordered_map>
#include <map>
#include <iostream>
#include <random>
#include <tuple>
#include "Timer.h"
#include "IntervalGenerator.h"
#include "SequenceSummer.h"

void TimeAddTest(unsigned long long max) {
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
	if (sum == (max * max)) {
		std::cout << "Sum is correct!" << std::endl;
	} else {
		std::cout << "Sum is incorrect!" << std::endl;
	}
	std::cout << "calculated " << sum << " in " << timer.ElapsedNanoseconds() << " ns." << std::endl;
}

void Sort(unsigned long long& A, unsigned long long& B) {
	if (A > B) {
		unsigned long long temp;
		temp = A;
		A = B;
		B = temp;
	}
}

unsigned long long NthOddNumber(unsigned long long N) {
	return 1 + (2 * N);
}

template<typename T>
void RandomizedTest(unsigned long long max, int repetitions, T& summer) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<unsigned long long> dis(0, max);
	unsigned long long lower, upper, lowerOdd, upperOdd, storedLower, storedUpper, lowerSquared, upperSquared;
	unsigned long long diff, sum, correct = 0, correctValues = 0;
	std::vector<std::tuple<unsigned long long, unsigned long long>> incorrectValues;
	for (int i = 0; i < repetitions; i++) {
		lower = dis(gen);
		upper = dis(gen);
		Sort(lower, upper);
		lowerOdd = NthOddNumber(lower);
		upperOdd = NthOddNumber(upper);
		storedLower = summer.Get(lower);
		storedUpper = summer.Get(upper);
		if (storedLower == lowerOdd) {
			correctValues++;
		} else {
			incorrectValues.push_back(std::make_tuple(storedLower, lowerOdd));
		}
		if (storedUpper == upperOdd) {
			correctValues++;
		} else {
			incorrectValues.push_back(std::make_tuple(storedUpper, upperOdd));
		}
		lowerSquared = lower * lower;
		upperSquared = upper * upper;
		diff = (upperSquared - lowerSquared);
		sum = summer.SumRange(lower, upper);
		if (sum == diff) {
			correct++;
		}
	}
	std::cout << "Randomized testing results: " << (correct / repetitions) * 100 << "%, correct values: ";
	std::cout << correctValues << ", repetitions*2: " << (repetitions * 2) << std::endl;
	std::cout << "Incorrect values: " << std::endl;
	for (std::tuple<unsigned long long, unsigned long long>& v : incorrectValues) {
		std::cout << "Expected: " << std::get<0>(v) << " Stored: " << std::get<1>(v) << std::endl;
	}
}

void StorageTest(unsigned long long repetitions) {
	std::cout << "Storage Test:" << std::endl;
	SequenceSummer<unsigned long long, 32> summer;
	for (unsigned long long i = 0; i < repetitions; i++) {
		summer.Set(i, i);
	}
	unsigned long long stored;
	for (unsigned long long i = 0; i < repetitions; i++) {
		stored = summer.Get(i);
		if (stored != i) {
			std::cout << "Incorrect value for " << i << " : " << stored << std::endl;
		}
	}
}

void BigTest() {
	BlockTimer timer;
	SequenceSummer<unsigned long long, 32> summer;
	unsigned long long max = 1 << 13;
	TimeAddTest(max);
	StorageTest(100);
	timer.Start();
	unsigned long long current = 1;
	for (unsigned long long i = 0; i < max; i++) {
		summer.Set(i, current);
		current += 2;
	}
	//	RandomizedTest<SequenceSummer<unsigned long long, 32>>(max, 1000, summer);
	timer.Pause();
	std::cout << "done setting " << max << " numbers in " << timer.ElapsedMilliseconds() << " ms." << std::endl;
	timer.Start();
	unsigned long long total = summer.SumRange(1, max - 1);
	timer.Pause();
	std::cout << std::endl << "Sum: " << total << " computed in: " << timer.ElapsedNanoseconds() << " ns.";
	std::cout << std::endl << "Sum: " << summer.SumRange(4, 6);
	std::cout << std::endl << "Sum: " << summer.SumRange(4, max - 3);
	std::cout << std::endl << "Sum: " << summer.SumRange(4, 4);
	std::cout << std::endl << "ree";
	std::cout << std::endl << "Sum: " << summer.SumRange(0, max);
	std::cout << std::endl << "4: " << summer.Get(4);
	std::cout << std::endl << "Sum: " << summer.SumRange(0, max);
	std::cout << std::endl << "Sum: " << summer.SumRange(1, max - 1);
	std::cout << std::endl << "sizeof(std::size_t) = " << sizeof(std::size_t);
	std::cout << std::endl;
	summer.Set(0, 0);
	for (unsigned long long i = 0; i < 10; i++) {
		std::cout << summer.Get(i) << ",";
	}
	std::cout << std::endl;
	//std::cin.get();
}

void TableTest(unsigned long long max) {
	IntervalGenerator<unsigned long long, 32> gen;
	for (unsigned long long i = 0; i < max; i++) {
		std::unordered_map<Interval<unsigned long long>, unsigned long long, typename Interval<unsigned long long>::Hasher> storage;
		//typedef std::unordered_map<Interval<unsigned long long>, unsigned long long, typename Interval<unsigned long long>::Hasher>::const_iterator IteratorType;
		Interval<unsigned long long> j, k;
		j = gen.GetLowestInterval(i);
		k = gen.GetIntervals(i)[0];
		if (!(j == k)) {
			std::cout << "Unequal intervals found!" << std::endl;
		}
		if (j.Start != k.Start) {
			std::cout << "Starts not equal!" << std::endl;
		}
		if (j.PowerOfTwo != k.PowerOfTwo) {
			std::cout << "Powers not equal!" << std::endl;
		}
		storage[j] = 3;
		unsigned long long u = storage[k];
		if (u != 3) {
			std::cout << "storage failed " << u << std::endl;
		}
		//IteratorType iterator = storage.find(k);
		//if (iterator != storage.end()) {
			
		//}
	}
	std::cout << std::endl << "Table test complete" << std::endl;
	std::unordered_map<Interval<unsigned long long>, unsigned long long, typename Interval<unsigned long long>::Hasher> storage;
	unsigned long long g = storage[gen.GetLowestInterval(1)];
	std::cout << "G:" << g << std::endl;
}

int main(int argc, char** argv) {
	BigTest();
	//TableTest(12);
	std::cout << std::endl;
	//std::cin.get();
}
