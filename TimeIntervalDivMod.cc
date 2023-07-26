#include <tuple>

#include "TimeIntervalDivMod.hh"

constexpr inline long DIGIT_BASE = 10;

/** Long division of a TimeInterval by a TimeInterval
 *
 * \param dividend
 * \param divisor
 */
std::tuple<long, TimeInterval> divmod(TimeInterval dividend, TimeInterval divisor) {
  long quotient = 0;
  TimeInterval remainder = dividend;
  // TODO: handle negative values
  if (divisor <= dividend) {
    // Normal long division
    // First, find out what digit to start with.
    long place_value = DIGIT_BASE;
    while (divisor * place_value < remainder) {
      place_value *= DIGIT_BASE;
    }
    place_value /= DIGIT_BASE;
    while (remainder > divisor) {
      long here_digit;
      // TODO: replace with binary search
      for (here_digit = 0; here_digit < DIGIT_BASE; ++here_digit) {
	if (here_digit * place_value * divisor > remainder) {
	  --here_digit;
	  break;
	}
      }
      quotient += here_digit * place_value;
      remainder -= here_digit * place_value * divisor;
      place_value /= DIGIT_BASE;
    }
  }
  return std::make_tuple(quotient, remainder);
}
