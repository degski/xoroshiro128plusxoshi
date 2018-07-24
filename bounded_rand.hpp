

// MIT License
//
// bounded_rand functions, based on the paper by Daniel Lemire
// https://arxiv.org/abs/1805.10941, with a mod by Melissa E. O'Neill.
//
// Copyright (c) 2018 Melissa E. O'Neill
//
// http://www.pcg-random.org/posts/bounded-rands.html
//
// Copyright (c) 2018 degski
//
// I just put together this file, from the code provided on Melissa E. O'Neill's
// blog/github. All errors are mine.
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

#include <cassert>
#include <cstdint>


// requires clang/gcc.

template<typename Gen>
std::uint32_t bounded_rand ( Gen & rng, const std::uint32_t i, std::uint32_t range ) noexcept {
    assert ( ( std::int32_t ( range ) - std::int32_t ( i ) ) > 0 );
    range -= i;
    std::uint32_t x = rng ( );
    std::uint64_t m = std::uint64_t ( x ) * std::uint64_t ( range );
    std::uint32_t l = std::uint32_t ( m );
    if ( l < range ) {
        std::uint32_t t = -range;
        if ( t >= range ) {
            t -= range;
            if ( t >= range ) {
                t %= range;
            }
        }
        while ( l < t ) {
            x = rng ( );
            m = std::uint64_t ( x ) * std::uint64_t ( range );
            l = std::uint32_t ( m );
        }
    }
    return ( m >> 32 ) + i;
}

template<typename Gen>
std::uint64_t bounded_rand ( Gen & rng, const std::uint64_t i, std::uint64_t range ) noexcept {
    assert ( ( std::int64_t ( range ) - std::int64_t ( i ) ) > 0 );
    range -= i;
    std::uint64_t x = rng ( );
    __uint128_t m = __uint128_t ( x ) * __uint128_t ( range );
    std::uint64_t l = std::uint64_t ( m );
    if ( l < range ) {
        std::uint64_t t = -range;
        if ( t >= range ) {
            t -= range;
            if ( t >= range )
                t %= range;
        }
        while ( l < t ) {
            x = rng ( );
            m = __uint128_t ( x ) * __uint128_t ( range );
            l = std::uint64_t ( m );
        }
    }
    return ( m >> 64 ) + i;
}
