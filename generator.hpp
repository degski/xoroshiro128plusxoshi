
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

#include "xoroshiro-meo.hpp"
#include "xoroshiro.hpp"
#include "splitmix.hpp"
#include "sfc.hpp"
#include "lehmer.hpp"
#include <Z:/VC/x64/include/pcg/pcg_random.hpp>
#include <Z:/VC/x64/include/integer_utils.hpp>

#include "generator_cache.hpp"


// using Generator = meo::xoroshiro128plus64;
// using Generator = v2::xoroshiro128plus64;
using Generator = splitmix64;
// using Generator = sfc64;
// using Generator = mcg128_fast;
// using Generator = mcg128; testing at the moment
// using Generator = pcg64;
// using Generator = iu::xoroshiro4x128plusavx;
// using Generator = iu::xoroshiro128plus64;
