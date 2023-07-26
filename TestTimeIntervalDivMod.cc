#include <iostream>
#include <random>

#include "TimeIntervalDivMod.hh"

constexpr inline long NUM_TESTS = 10;

bool testRecoversDividendDivisor(TimeInterval dividend, TimeInterval divisor) {
  std::tuple<long, TimeInterval> result = divmod(dividend, divisor);
  if (result[1] > divisor) {
    return false;
  }
  if (result[0] * divisor + result[1] == dividend) {
    return true;
  } else {
    return false;
  }
}

int main(int argc, char *argv[]) {
  std::default_random_engine generator;
  std::uniform_int_distribution<long> distribution(0, 86_399);
  long failures = 0;
  for (long i = 0; i < NUM_TESTS; ++i) {
    TimeInterval dividend(distribution(generator), distribution(generator), distribution(generator));
    TimeInterval divisor(distribution(generator), distribution(generator), distribution(generator));
    if (!testRecoversDividendDivisor(dividend, divisor)) {
      std::tuple<long, TimeInterval> result = divmod(dividend, divisor);
      std::cout << "Test failure: " << dividend << " != " << result[0] << " * " << divisor << " + " result[1] << std::endl;
    }
  }
  return failures;
}
