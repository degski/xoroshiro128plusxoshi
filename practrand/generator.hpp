
// MIT License
//
// Copyright (c) 2018 degski
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#define __AVX2__ 1

#include <random>
#include <string>
#include <type_traits>

#include "../pcg/pcg_random.hpp"
#include "../xoroshiro.hpp"
#include "../xoroshiro128plusxoshi.hpp"
#include "../splitmix.hpp"
#include "../sfc.hpp"
#include "../lehmer.hpp"


// using Generator = meo::xoroshiro128plus64;
// using Generator = degski::xoroshiro128plus64xoshi16;
// using Generator = degski::xoroshiro128plus64xoshi21;
// using Generator = degski::xoroshiro128plus64xoshi24;
// using Generator = degski::xoroshiro128plus64xoshi32;
// using Generator = degski::xoroshiro128plus64xoshi48;

// using Generator = degski::xoroshiro128plus64xoshi16starxoshi16;
// using Generator = degski::xoroshiro128plus64xoshi32starxoshi16;
// using Generator = degski::xoroshiro128plus64xoshi32starxoshi32;

// using Generator = splitmix64;
// using Generator = sfc64;
// using Generator = mcg128_fast;
// using Generator = mcg128;
using Generator = pcg64;