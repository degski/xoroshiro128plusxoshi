

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
#include <ciso646>
#include <cstdint>
#include <cstdlib>

#include <limits>
#include <random>
#include <type_traits>


// requires clang/gcc.

template<typename IntType = int>
struct uniform_int_distribution_fast {

    static_assert ( ( ( sizeof ( IntType ) > 1 ) and ( sizeof ( IntType ) <= 8 ) ), "char not supported." );

    using result_type = IntType;

    private:

    template<typename IT> struct unsign { };
    template<> struct unsign<std::uint16_t> { using type = std::uint16_t; };
    template<> struct unsign<std::uint32_t> { using type = std::uint32_t; };
    template<> struct unsign<std::uint64_t> { using type = std::uint64_t; };
    template<> struct unsign<std::int16_t > { using type = std::uint16_t; };
    template<> struct unsign<std::int32_t > { using type = std::uint32_t; };
    template<> struct unsign<std::int64_t > { using type = std::uint64_t; };

    template<typename IT> struct double_width { };
    template<> struct double_width<std::uint16_t> { using type = std::uint32_t; };
    template<> struct double_width<std::uint32_t> { using type = std::uint64_t; };
    template<> struct double_width<std::uint64_t> { using type = __uint128_t; };

    using unsigned_result_type = typename unsign<result_type>::type;
    using double_width_unsigned_result_type = typename double_width<unsigned_result_type>::type;

    public:

    explicit uniform_int_distribution_fast ( ) noexcept :
        i ( 0 ),
        range ( std::numeric_limits<unsigned_result_type>::max ( ) ) {
    }
    explicit uniform_int_distribution_fast ( result_type a, result_type b = std::numeric_limits<result_type>::max ( ) ) noexcept :
        i ( a ),
        range ( b - a ) {
        assert ( b > a );
    }
    // explicit uniform_int_distribution_fast ( const param_type & params );

    result_type a ( ) const noexcept {
        return i;
    }
    result_type b ( ) const noexcept {
        return range + i;
    }

    template<typename Gen>
    [[ nodiscard ]] result_type operator ( ) ( Gen & rng ) noexcept {
        unsigned_result_type x = rng ( );
        if ( range >= ( unsigned_result_type { 1 } << ( sizeof ( unsigned_result_type ) * 8 - 1 ) ) ) {
            while ( x >= range )
                x = rng ( );
            return x;
        }
        double_width_unsigned_result_type m = x;
        m *= range;
        unsigned_result_type l = m;
        if ( l < range ) {
            unsigned_result_type t = -range;
            if ( t >= range ) {
                t -= range;
                if ( t >= range ) {
                    t %= range;
                }
            }
            while ( l < t ) {
                m = rng ( );
                m *= range;
                l = m;
            }
        }
        m >>= ( sizeof ( unsigned_result_type ) * 8 );
        m += i;
        return m;
    }

    private:

    result_type i;
    unsigned_result_type range;
};
