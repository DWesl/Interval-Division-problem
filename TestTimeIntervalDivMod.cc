#include <iostream>
#include <random>

#include "TimeIntervalDivMod.hh"

constexpr inline long NUM_TESTS = 10;

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
  for (long i = 0; i < NUM_TESTS; ++i) {
    TimeInterval dividend(distribution(generator), distribution(generator), distribution(generator));
    TimeInterval divisor(distribution(generator), distribution(generator), distribution(generator));
    if (!testRecoversDividendDivisor(dividend, divisor)) {
      std::tuple<long, TimeInterval> result = divmod(dividend, divisor);
      std::cout << "Test failure: " << dividend
		<< " != " << std::get<0>(result) << " * " << divisor
		<< " + " << std::get<1>(result) << std::endl;
    }
  }
  return failures;
}
