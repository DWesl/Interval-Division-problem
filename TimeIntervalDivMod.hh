#include <tuple>

#include "TimeInterval.h"

#ifndef TIME_INTERVAL_DIVMOD
#define TIME_INTERVAL_DIVMOD

std::tuple<long, TimeInterval> divmod(TimeInterval dividend, TimeInterval divisor);

#endif
