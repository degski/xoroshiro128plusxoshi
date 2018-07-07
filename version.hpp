
#pragma once

#define __AVX2__ 1

namespace ver = v2;

// using Generator = ver::xoroshiro128plus64;
using Generator = splitmix64;
