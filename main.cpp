#include <vector>
#include <unordered_map>
#include <map>
#include <iostream>
#include <random>
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
	Timer timer;
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
	std::cout << "calculated " << sum << " in " << timer.ElapsedNanoseconds() << " ns. O(N) method" << std::endl;
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
			std::cout << "# " << lower << " : " << storedLower << "|" << lowerOdd << std::endl;
		}
		if (storedUpper == upperOdd) {
			correctValues++;
		} else {
			std::cout << "# " << upper << " : " << storedUpper << "|" << upperOdd << std::endl;
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
}

template<typename T>
void FillSummerWithFirstNOddNumbers(T& summer, unsigned long long max) {
	for (unsigned long long i = 0; i <= max; i++) {
		summer.Set(i, NthOddNumber(i));
	}
}

int main(int argc, char** argv) {
	unsigned long long max = 1 << 24;
	typedef SequenceSummer<unsigned long long, 32> Summer;
	Summer summer;
	Timer timer;
	timer.Start();
	FillSummerWithFirstNOddNumbers<Summer>(summer, max);
	timer.Pause();
	std::cout << "Set " << max << " numbers in " << timer.ElapsedMilliseconds() << " ms." << std::endl;
	timer.Start();
	RandomizedTest<Summer>(max, 1000, summer);
	timer.Pause();
	std::cout << "Time: " << timer.ElapsedNanoseconds() << " ns." << std::endl;
	TimeAddTest(max);
	timer.Start();
	unsigned long long total = summer.SumRange(0, max);
	timer.Pause();
	std::cout << "calculated " << total << " in " << timer.ElapsedNanoseconds() << " ns. O(Log(N)) Method" << std::endl;
#ifdef _MSC_VER
	std::cin.get();
#endif // MSVC
}
