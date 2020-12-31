#pragma once
#include <string>
#include "utility.hpp"
#include "switch-generic.hpp"

constexpr auto media_func = 6;
constexpr auto default_priority_6 = media_func;

typedef enum {
	TIME_RESTRICTION_NONE,
	TIME_RESTRICTION_SHORTER,
	TIME_RESTRICTION_LONGER,
	TIME_RESTRICTION_REMAINING_SHORTER,
	TIME_RESTRICTION_REMAINING_LONGER,
} time_restriction;
