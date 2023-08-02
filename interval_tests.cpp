#include <iostream>
#include <limits>

#include "TimeInterval.h"

using namespace std;

const TimeInterval ZERO(0, 0, 0);

constexpr long DIGIT_BASE = 10;

constexpr long SECONDS_PER_HOUR = 3'600;
constexpr long HOURS_PER_DAY = 24;
constexpr long SECONDS_PER_DAY = SECONDS_PER_HOUR * HOURS_PER_DAY;

/**
 * \brief Divides one time interval by another
 *
 * Given two TimeInterval objects -- a numerator and a denominator -- computes 
 * the quotient (as a long integer) and the remainder (as another TimeInterval)
 * when numerator is divided by denominator.
 *
 */
void divide(const TimeInterval &numerator,
            const TimeInterval &denominator,
            long &quotient,
            TimeInterval &remainder)
{
	quotient = 0;
	remainder = numerator;

	// Handle negative numerator and denominator
	if (numerator < ZERO) {
		divide(-1 * numerator, denominator, quotient, remainder);
		quotient *= -1;
		remainder *= -1;
		return;
	}
	if (denominator < ZERO) {
		divide(numerator, -1 * denominator, quotient, remainder);
		quotient *= -1;
		remainder *= -1;
		return;
	}
	if (denominator == ZERO) {
		throw std::invalid_argument("Zero division error");
	}

	//
	// Long division, roughly as taught in grade school
	//
	if (denominator <= numerator) {
		// First, find out what digit to start with.
		// Humans do this with heuristics analogous to natural logarithms
		// This checks successive powers of ten until one is too big
		// then steps back one
		long place_value = DIGIT_BASE;
		while (denominator * place_value < remainder) {
		  place_value *= DIGIT_BASE;
		  if (place_value > std::numeric_limits<long>::max() / DIGIT_BASE) {
		    throw std::overflow_error("Quotient doesn't fit in result type");
		  }
		}
		place_value /= DIGIT_BASE;

		// Now the core loop of long division
		// For each place (thousands, hundreds, tens, ones)
		// find what digit goes there, then go on to the next
		// This just checks each possible digit in sequence
		// Binary search might be faster
		while (remainder > denominator) {
			long here_digit = 0;
			for (here_digit = 0; here_digit < DIGIT_BASE; ++here_digit) {
			  if (here_digit * place_value * denominator > remainder) {
			    // We've found a digit which is too big; Go back
			    // to the previous one, which keeps the
			    // remainder positive.
			    --here_digit;
			    break;
			  }
			}
			// Add the new digit to the quotient, subtract the
			// now-explained part from the working remainder.
			// Fewer working digits means faster normalize()
			quotient += here_digit * place_value;
			remainder -= here_digit * place_value * denominator;
			place_value /= DIGIT_BASE;
		}
	}

	//
	// Are there edge cases that aren't handled by the simple algorithm?
	//

	//
	// Are there algorithms that can arrive at the answer much more efficiently?
	//
}

inline bool test_known_division(const TimeInterval &numerator,
				const TimeInterval &denominator,
				long expected_quotient,
				const TimeInterval &expected_remainder,
				long test_number) {
	bool test_passed = true;
	long actual_quotient;
	TimeInterval actual_remainder;
	divide(numerator, denominator, actual_quotient, actual_remainder);
	test_passed = (actual_quotient == expected_quotient)
	  && (actual_remainder == expected_remainder);

	if (!test_passed) {
	  cout << "not ";
	}
	cout << "ok " << test_number << " - "
	     << numerator << " / " << denominator
	     << " == "
	     << actual_quotient << " remainder " << actual_remainder;
	if (!test_passed) {
	  cout << " expected " << expected_quotient
	       << " remainder " << expected_remainder;
	}
	cout << endl;
	return test_passed;
}

int main(int argc, char **argv)
{
	TimeInterval numerator;
	TimeInterval denominator;
	TimeInterval remainder;
	long quotient;
	int num_failures = 0;
	bool test_passed = true;
	long test_number = 1;

	//
	// Add tests here to verify the correctness of your implementation
	// of interval division
	//
	cout << "1..7" << endl;

	numerator.setInterval(500, 0, 0);    // 500 days
	denominator.setInterval(0, 0, 200'000);    // 0.2 seconds

	long expected_quotient = (5 * SECONDS_PER_DAY * 500);
	TimeInterval expected_remainder = ZERO;
	test_passed = test_known_division(numerator, denominator,
					  expected_quotient, expected_remainder,
					  test_number++);
	if (!test_passed) {
	  ++num_failures;
	}

	numerator = TimeInterval(1, 0, 0);
	denominator = TimeInterval(0, 1, 0);
	expected_quotient = SECONDS_PER_DAY;
	expected_remainder = ZERO;
	test_passed = test_known_division(numerator, denominator,
					  expected_quotient, expected_remainder,
					  test_number++);
	if (!test_passed) {
	  ++num_failures;
	}

	numerator = TimeInterval(0, 1, 0);
	denominator = TimeInterval(0, 0, 1);
	expected_quotient = 1'000'000;
	expected_remainder = ZERO;
	test_passed = test_known_division(numerator, denominator,
					  expected_quotient, expected_remainder,
					  test_number++);
	if (!test_passed) {
	  ++num_failures;
	}

	numerator = TimeInterval(1, 0, 1);
	denominator = TimeInterval(0, -1, 0);
	expected_quotient = -SECONDS_PER_DAY;
	expected_remainder = TimeInterval(0, 0, -1);
	test_passed = test_known_division(numerator, denominator,
					  expected_quotient, expected_remainder,
					  test_number++);
	if (!test_passed) {
	  ++num_failures;
	}

	numerator = TimeInterval(1, 0, 0);
	denominator = TimeInterval(0, 100, 0);
	expected_quotient = SECONDS_PER_DAY / 100;
	expected_remainder = ZERO;
	test_passed = test_known_division(numerator, denominator,
					  expected_quotient, expected_remainder,
					  test_number++);
	if (!test_passed) {
	  ++num_failures;
	}

	numerator = TimeInterval(0, 1, 0);
	denominator = TimeInterval(0, 0, 1'000);
	expected_quotient = 1'000;
	expected_remainder = ZERO;
	test_passed = test_known_division(numerator, denominator,
					  expected_quotient, expected_remainder,
					  test_number++);
	if (!test_passed) {
	  ++num_failures;
	}

	numerator = TimeInterval(1, 0, 1);
	denominator = TimeInterval(0, -100, 0);
	expected_quotient = -SECONDS_PER_DAY / 100;
	expected_remainder = TimeInterval(0, 0, -1);
	test_passed = test_known_division(numerator, denominator,
					  expected_quotient, expected_remainder,
					  test_number++);
	if (!test_passed) {
	  ++num_failures;
	}



	return num_failures;
}
