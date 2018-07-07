
#pragma once

#define __AVX2__ 1

#include "xoroshiro_meo.hpp"
#include "xoroshiro2.hpp"
#include "splitmix.hpp"

// using Generator = meo::xoroshiro128plus64;
// using Generator = v2::xoroshiro128plus64;
using Generator = splitmix64;
