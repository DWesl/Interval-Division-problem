#include <functional>
#include <iostream>
#include <limits>
#include <chrono>
#include <tuple>

#include "TimeInterval.h"
#include "TimeIntervalDivMod.hh"

constexpr inline const static long DIGIT_BASE = 10;

/**
 * \brief Divides one time interval by another
 *
 * Given two TimeInterval objects -- a numerator and a denominator -- computes 
 * the quotient (as a long integer) and the remainder (as another TimeInterval)
 * when numerator is divided by denominator.
 *
 */
std::tuple<long, TimeInterval>
divmod_increment(const TimeInterval &numerator,
		 const TimeInterval &denominator)
{
  if (numerator < TimeInterval(0, 0, 0)) {
    std::tuple<long, TimeInterval> result = divmod_increment(-1 * numerator, denominator);
    return std::make_tuple(-1 * std::get<0>(result), -1 * std::get<1>(result));
  }
  if (denominator < TimeInterval(0, 0, 0)) {
    std::tuple<long, TimeInterval> result = divmod_increment(numerator, -1 * denominator);
    return std::make_tuple(-1 * std::get<0>(result), -1 * std::get<1>(result));
  }
  if (denominator == TimeInterval(0, 0, 0)) {
    throw std::invalid_argument("Zero division error");
  }
	long quotient = 0;
	TimeInterval remainder = numerator;

	//
	// The simplest algorithm...
	//
	while (remainder >= denominator) {
		remainder -= denominator;
		if (quotient == std::numeric_limits<long>::max()) {
		  throw std::overflow_error("Quotient doesn't fit in result type");
		}
		quotient++;
	}

	//
	// Are there edge cases that aren't handled by the simple algorithm?
	//

	//
	// Are there algorithms that can arrive at the answer much more efficiently?
	//
	return std::make_tuple(quotient, remainder);
}

std::tuple<long, TimeInterval>
divmod_binary_search(const TimeInterval &numerator,
		     const TimeInterval &denominator)
{
  if (numerator < TimeInterval(0, 0, 0)) {
    std::tuple<long, TimeInterval> result = divmod_binary_search(-1 * numerator, denominator);
    return std::make_tuple(-1 * std::get<0>(result), -1 * std::get<1>(result));
  }
  if (denominator < TimeInterval(0, 0, 0)) {
    std::tuple<long, TimeInterval> result = divmod_binary_search(numerator, -1 * denominator);
    return std::make_tuple(-1 * std::get<0>(result), -1 * std::get<1>(result));
  }
  if (denominator == TimeInterval(0, 0, 0)) {
    throw std::invalid_argument("Zero division error");
  }

  long quotient = 1;
  TimeInterval remainder = numerator;
  long quotient_low = 1, quotient_high = 1;

  while (quotient_low * denominator < numerator) {
    quotient_low *= DIGIT_BASE;
    if (quotient_low > std::numeric_limits<long>::max() / DIGIT_BASE) {
      throw std::overflow_error("Quotient doesn't fit in result type");
    }
  }
  quotient_high = quotient_low;
  quotient = quotient_low;
  quotient_low /= DIGIT_BASE;

  // Binary search
  while (quotient_low + 1 < quotient_high) {
    quotient = quotient_low + (quotient_high - quotient_low) / 2;
    TimeInterval product = quotient * denominator;
    if (product < numerator) {
      quotient_low = quotient;
    } else if (product > numerator) {
      quotient_high = quotient;
    } else {
      // numerator == denominator * quotient
      break;
    }
  }
  if (quotient_high * denominator <= numerator) {
    quotient = quotient_high;
  }

  remainder = numerator - quotient * denominator;
  return std::make_tuple(quotient, remainder);
}

/** Long division of a TimeInterval by a TimeInterval
 *
 * \param numerator
 * \param denominator
 */
std::tuple<long, TimeInterval>
divmod_long_division(TimeInterval numerator, TimeInterval denominator) {
  if (numerator < TimeInterval(0, 0, 0)) {
    std::tuple<long, TimeInterval> result = divmod_long_division(-1 * numerator, denominator);
    return std::make_tuple(-1 * std::get<0>(result), -1 * std::get<1>(result));
  }
  if (denominator < TimeInterval(0, 0, 0)) {
    std::tuple<long, TimeInterval> result = divmod_long_division(numerator, -1 * denominator);
    return std::make_tuple(-1 * std::get<0>(result), -1 * std::get<1>(result));
  }
  if (denominator == TimeInterval(0, 0, 0)) {
    throw std::invalid_argument("Zero division error");
  }

  long quotient = 0;
  TimeInterval remainder = numerator;
  // TODO: handle negative values
  if (denominator <= numerator) {
    // Normal long division
    // First, find out what digit to start with.
    long place_value = DIGIT_BASE;
    while (denominator * place_value < remainder) {
      place_value *= DIGIT_BASE;
      if (place_value > std::numeric_limits<long>::max() / DIGIT_BASE) {
	throw std::overflow_error("Quotient doesn't fit in result type");
      }
    }
    place_value /= DIGIT_BASE;

    while (remainder > denominator) {
      long here_digit = 0;
      // TODO: replace with binary search
      for (here_digit = 0; here_digit < DIGIT_BASE; ++here_digit) {
	if (here_digit * place_value * denominator > remainder) {
	  --here_digit;
	  break;
	}
      }
      quotient += here_digit * place_value;
      remainder -= here_digit * place_value * denominator;
      place_value /= DIGIT_BASE;
    }
  }
  return std::make_tuple(quotient, remainder);
}


int main(int argc, char *argv[]) {
  std::function<std::tuple<long, TimeInterval>(TimeInterval, TimeInterval)> implementations[3] = {
    divmod_increment,
    divmod_long_division,
    divmod_binary_search
  };
  std::chrono::steady_clock timer;
  std::chrono::duration<double> elapsed_times[3];
  long failures = 0;

  for (long i = 0; i < 3; ++i) {
    std::cout << "Function number " << i << std::endl;
    auto start_time = timer.now();
    std::function<std::tuple<long, TimeInterval>(TimeInterval, TimeInterval)> divmod = implementations[i];
    for (long j = 0; j < 1'000; ++j) {
      std::tuple<long, TimeInterval> result;
      result = divmod(TimeInterval(1, 0, 0), TimeInterval(0, 100, 0));
      if ((std::get<0>(result) != 24 * 36) || (std::get<1>(result) != TimeInterval(0, 0, 0))) {
	failures |= 1 << 6 * i;
      }
      result = divmod(TimeInterval(0, 1, 0), TimeInterval(0, 0, 1'000));
      if ((std::get<0>(result) != 1'000) || (std::get<1>(result) != TimeInterval(0, 0, 0))) {
	failures |= 2 << 6 * i;
      }
      result = divmod(TimeInterval(1, 0, 1), TimeInterval(0, -100, 0));
      if ((std::get<0>(result) != -24 * 36) || (std::get<1>(result) != TimeInterval(0, 0, -1))) {
	failures |= 4 << 6 * i;
      }

      result = divmod(TimeInterval(1, 0, 0), TimeInterval(0, 1, 0));
      if ((std::get<0>(result) != 24 * 3600) || (std::get<1>(result) != TimeInterval(0, 0, 0))) {
	failures |= 8 << 6 * i;
      }
      result = divmod(TimeInterval(0, 1, 0), TimeInterval(0, 0, 1));
      if ((std::get<0>(result) != 1'000'000) || (std::get<1>(result) != TimeInterval(0, 0, 0))) {
	failures |= 16 << 6 * i;
      }
      result = divmod(TimeInterval(1, 0, 1), TimeInterval(0, -1, 0));
      if ((std::get<0>(result) != -24 * 3600) || (std::get<1>(result) != TimeInterval(0, 0, -1))) {
	failures |= 32 << 6 * i;
      }
    }
    auto end_time = timer.now();
    elapsed_times[i] = end_time - start_time;
  }
  std::cout << "\nIncrement:     " << elapsed_times[0].count()
	    << "\nLong division: " << elapsed_times[1].count()
	    << "\nBinary Search: " << elapsed_times[2].count()
	    << std::endl;
  return failures;
}
