#include <iostream>
#include "TimeInterval.h"

using namespace std;

constexpr TimeInterval ZERO(0, 0, 0);
constexpr inline long NUM_RANDOM_TESTS = 250;

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
	bool negateResult = false;

	// Handle negative numerator and denominator
	if (numerator < ZERO) {
		negateResult = ~negateResult;
		numerator *= -1;
	}
	if (denominator < ZERO) {
		negateResult = ~negateResult;
		denominator *= -1;
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
	if (negateResult) {
		quotient *= -1;
		remainder *= -1;
	}

	//
	// Are there algorithms that can arrive at the answer much more efficiently?
	//
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

	numerator.setInterval(500, 0, 0);    // 500 days
	denominator.setInterval(0, 0, 200'000);    // 0.2 seconds

	divide(numerator, denominator, quotient, remainder);
	test_passed = (quotient != (5 * SECONDS_PER_DAY * 500)) || (remainder != ZERO);
	cout << denominator << " divides " << numerator << " " << quotient << " times with a remainder of " << remainder << ": ";

	if (!test_passed) {
	  ++num_failures;
	  cout << "not ";
	}
	cout << "ok " << test_number++ << " - "
	     << numerator << " / " << denominator
	     << (test_passed ? " == " : " != ")
	     << quotiend << " remainder " << remainder
	     << endl;

	numerator = TimeInterval(1, 0, 0);
	denominator = TimeInterval(0, 1, 0);
	divide(numerator, denominator, quotient, remainder);
	test_passed = (quotient != SECONDS_PER_DAY) || (remainder != ZERO);
	if (!test_passed) {
	  ++num_failures;
	  cout << "not ";
	}
	cout << "ok " << test_number++ << " - "
	     << numerator << " / " << denominator
	     << (test_passed ? " == " : " != ")
	     << quotiend << " remainder " << remainder
	     << endl;

	numerator = TimeInterval(0, 1, 0);
	denominator = TimeInterval(0, 0, 1);
	divide(numerator, denominator, quotient, remainder);
	test_passed = (quotient != 1'000'000) || (remainder != ZERO);
	if (!test_passed) {
	  ++num_failures;
	  cout << "not ";
	}
	cout << "ok " << test_number++ << " - "
	     << numerator << " / " << denominator
	     << (test_passed ? " == " : " != ")
	     << quotiend << " remainder " << remainder
	     << endl;

	numerator = TimeInterval(1, 0, 1);
	denominator = TimeInterval(0, -1, 0);
	divide(numerator, denominator, quotient, remainder);
	test_passed = (quotient != -SECONDS_PER_DAY) || (remainder != TimeInterval(0, 0, -1))
	if (!test_passed) {
	  ++num_failures;
	  cout << "not ";
	}
	cout << "ok " << test_number++ << " - "
	     << numerator << " / " << denominator
	     << (test_passed ? " == " : " != ")
	     << quotiend << " remainder " << remainder
	     << endl;


	return num_failures;
}
