
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

#include <string>
#include <type_traits>

#include "pcg/pcg_random.hpp"
#include "xoroshiro.hpp"
#include "xoroshiro128plusxoshi.hpp"
#include "splitmix.hpp"
#include "sfc.hpp"
#include "lehmer.hpp"


// using Generator = meo::xoroshiro128plus64;
// using Generator = degski::xoroshiro128plus64xoshi16;
// using Generator = degski::xoroshiro128plus64xoshi21;
// using Generator = degski::xoroshiro128plus64xoshi24;
// using Generator = degski::xoroshiro128plus64xoshi32;
// using Generator = degski::xoroshiro128plus64xoshi48;
// using Generator = splitmix64;
// using Generator = sfc64;
// using Generator = mcg128_fast;
// using Generator = mcg128;
using Generator = pcg64;

namespace detail {

template<typename Gen>
inline std::string generator_name_impl ( ) noexcept { // where's the introspection?

    if constexpr ( std::is_same<Gen, splitmix64>::value ) {
        return std::string ( "splitmix64" );
    }
    else if constexpr ( std::is_same<Gen, sfc64>::value ) {
        return std::string ( "sfc64" );
    }
    else if constexpr ( std::is_same<Gen, mcg128_fast>::value ) {
        return std::string ( "mcg128_fast" );
    }
    else if constexpr ( std::is_same<Gen, mcg128>::value ) {
        return std::string ( "mcg128" );
    }
    else if constexpr ( std::is_same<Gen, meo::xoroshiro128plus64>::value ) {
        return std::string ( "xoroshiro128plus64 v1" );
    }
    else if constexpr ( std::is_same<Gen, pcg64>::value ) {
            return std::string ( "pcg64" );
    }
    else {
        return std::string ( "xoroshiro128plus64xoshi" ) + std::string ( std::to_string ( Gen::shift ( ) ) );
    }
}
}

inline std::string generator_name ( ) noexcept {
    return detail::generator_name_impl<Generator> ( );
}
