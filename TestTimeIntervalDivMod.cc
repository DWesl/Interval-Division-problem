#include <iostream>
#include <random>

#include "TimeIntervalDivMod.hh"

constexpr inline long NUM_TESTS = 200;

bool testRecoversDividendDivisor(TimeInterval dividend, TimeInterval divisor) {
  std::tuple<long, TimeInterval> result = divmod(dividend, divisor);
  if (std::get<1>(result) > divisor) {
    return false;
  }
  if (std::get<0>(result) * divisor + std::get<1>(result) == dividend) {
    return true;
  } else {
    return false;
  }
}

int main(int argc, char *argv[]) {
  std::default_random_engine generator;
  std::uniform_int_distribution<long> distribution(0, 86'399);
  long failures = 0;

  std::cout << "1.." << NUM_TESTS << std::endl;
  for (long i = 0; i < NUM_TESTS; ++i) {
    TimeInterval dividend(distribution(generator), distribution(generator), distribution(generator));
    TimeInterval divisor(distribution(generator), distribution(generator), distribution(generator));
    std::tuple<long, TimeInterval> quotient_remainder = divmod(dividend, divisor);
    bool test_passed = testRecoversDividendDivisor(dividend, divisor);
    if (!test_passed) {
      ++failures;
      std::cout << "not ";
    }
    std::cout << "ok " << i+1 << " - " << dividend
	      << (test_passed ? " == " : " != ")
	      << std::get<0>(quotient_remainder) << " * " << divisor
	      << " + " << std::get<1>(quotient_remainder) << std::endl;
  }

  return failures;
}
