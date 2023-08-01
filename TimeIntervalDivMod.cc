#include <limits>
#include <stdexcept>
#include <tuple>

#include "TimeIntervalDivMod.hh"

constexpr inline static long DIGIT_BASE = 10;

/** Long division of a TimeInterval by a TimeInterval
 *
 * \param dividend
 * \param divisor
 */
std::tuple<long, TimeInterval> divmod(TimeInterval dividend, TimeInterval divisor) {
  if (dividend < TimeInterval(0, 0, 0)) {
    std::tuple<long, TimeInterval> result = divmod(-1 * dividend, divisor);
    return std::make_tuple(-1 * std::get<0>(result), -1 * std::get<1>(result));
  }
  if (divisor < TimeInterval(0, 0, 0)) {
    std::tuple<long, TimeInterval> result = divmod(dividend, -1 * divisor);
    return std::make_tuple(-1 * std::get<0>(result), -1 * std::get<1>(result));
  }
  if (divisor == TimeInterval(0, 0, 0)) {
    throw std::invalid_argument("Zero division error");
  }

  long quotient = 0;
  TimeInterval remainder = dividend;
  // TODO: handle negative values
  if (divisor <= dividend) {
    // Normal long division
    // First, find out what digit to start with.
    long place_value = DIGIT_BASE;
    while (divisor * place_value < remainder) {
      place_value *= DIGIT_BASE;
      if (place_value > std::numeric_limits<long>::max() / DIGIT_BASE) {
	throw std::overflow_error("Quotient doesn't fit in result type");
      }
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
