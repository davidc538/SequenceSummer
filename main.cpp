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
bool DiffOfSquaresTest(unsigned long long lower,
					   unsigned long long upper, T& summer) {
	unsigned long long lowerOdd = NthOddNumber(lower);
	unsigned long long upperOdd = NthOddNumber(upper);
	unsigned long long storedLower = summer.Get(lower);
	unsigned long long storedUpper = summer.Get(upper);
   	unsigned long long lowerSquared = lower * lower;
   	unsigned long long upperSquared = upper * upper;
   	unsigned long long diff = (upperSquared - lowerSquared);
   	unsigned long long sum = summer.SumRange(lower, upper);
	bool lowerCorrect = (storedLower == lowerOdd);
   	bool upperCorrect = (storedUpper == upperOdd);
   	bool answerCorrect = (sum == diff);
	bool ret = (lowerCorrect & upperCorrect & answerCorrect);
	if (!ret) {
		using namespace std;
		cout << "Failed Diff Of Squares Test:" << endl;
		cout << "lowerOdd     " << lowerOdd << endl;
		cout << "storedLower: " << storedLower << endl;
		cout << "upperOdd:    " << upperOdd << endl;
		cout << "storedUpper: " << storedUpper << endl;
		cout << "lowerSquared:" << lowerSquared << endl;
		cout << "upperSquared:" << upperSquared << endl;
		cout << "diff:        " << diff << endl;
		cout << "sum:         " << sum << endl;
	}
	return ret;
}

template<typename T>
void MainTest(unsigned long long max, int repetitions, T& summer) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<unsigned long long> dis(0, max);
	unsigned long long lower, upper;
	bool allTestsPassed = true;
	for (unsigned long long i = 1; i < max; i++) {
		allTestsPassed = allTestsPassed & DiffOfSquaresTest<T>(0, i, summer);
		allTestsPassed = allTestsPassed & DiffOfSquaresTest<T>(i, max, summer);
	}
	for (int i = 0; i < repetitions; i++) {
		lower = dis(gen);
		upper = dis(gen);
		Sort(lower, upper);
	}
}

template<typename T>
void FillSummerWithFirstNOddNumbers(T& summer, unsigned long long max) {
	for (unsigned long long i = 0; i <= max; i++) {
		summer.Set(i, NthOddNumber(i));
	}
}

int main(int argc, char** argv) {
	unsigned long long max = 1 << 25;
	typedef SequenceSummer<unsigned long long, 32> Summer;
	Summer summer;
	Timer timer;
	timer.Start();
	FillSummerWithFirstNOddNumbers<Summer>(summer, max);
	timer.Pause();
	std::cout << "Set " << max << " numbers in " << timer.ElapsedMilliseconds() << " ms." << std::endl;
	timer.Start();
	MainTest<Summer>(max, 1000, summer);
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
