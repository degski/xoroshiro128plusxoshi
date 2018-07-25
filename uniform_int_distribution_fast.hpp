
// MIT License
//
// C++17-compliant uniform_int_distribution_fast, based on bounded_rand-function
// as per the paper by Daniel Lemire:
// https://arxiv.org/abs/1805.10941.
// and optimizations added to bounded_rand by Melissa E. O'Neill:
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

#include <functional>
#include <limits>
#include <random>
#include <type_traits>

// requires clang/gcc for the moment (24.07.2018).

namespace ext {

template<typename IntType = int>
class uniform_int_distribution_fast;

namespace detail {

template<typename IT> struct unsign_integer { };
template<> struct unsign_integer<std::uint16_t> { using type = std::uint16_t; };
template<> struct unsign_integer<std::uint32_t> { using type = std::uint32_t; };
template<> struct unsign_integer<std::uint64_t> { using type = std::uint64_t; };
template<> struct unsign_integer<std::int16_t > { using type = std::uint16_t; };
template<> struct unsign_integer<std::int32_t > { using type = std::uint32_t; };
template<> struct unsign_integer<std::int64_t > { using type = std::uint64_t; };

template<typename IT> struct double_width_integer { };
template<> struct double_width_integer<std::uint16_t> { using type = std::uint32_t; };
template<> struct double_width_integer<std::uint32_t> { using type = std::uint64_t; };
template<> struct double_width_integer<std::uint64_t> { using type = __uint128_t; };

template<typename Gen>
struct generator_reference : public std::reference_wrapper<Gen> {

    using std::reference_wrapper<Gen>::reference_wrapper;

    using result_type = typename Gen::result_type;

    [[ nodiscard ]] static constexpr result_type min ( ) noexcept { return Gen::min ( ); }
    [[ nodiscard ]] static constexpr result_type max ( ) noexcept { return Gen::max ( ); }
};

template<typename Gen, typename UnsignedResultType>
struct bits_engine : public std::independent_bits_engine<generator_reference<Gen>, std::numeric_limits<UnsignedResultType>::digits, UnsignedResultType> {
    explicit bits_engine ( Gen & gen ) : std::independent_bits_engine<generator_reference<Gen>, std::numeric_limits<UnsignedResultType>::digits, UnsignedResultType> ( gen ) { }
};

template<typename IntType, typename Distribution>
struct param_type {

    using result_type = IntType;
    using distribution_type = Distribution;

    using unsigned_result_type = typename unsign_integer<result_type>::type;

    friend class ::ext::uniform_int_distribution_fast<result_type>;

    explicit param_type ( result_type min_, result_type max_ ) noexcept :
        min ( min_ ),
        range ( max_ - min_ + 1 ) {
        assert ( min_ < max_ );
    }

    [[ nodiscard ]] constexpr bool operator == ( const param_type & rhs ) const noexcept {
        return min == rhs.min and range == rhs.range;
    }

    [[ nodiscard ]] constexpr bool operator != ( const param_type & rhs ) const noexcept {
        return not ( *this == rhs );
    }

    [[ nodiscard ]] constexpr result_type a ( ) const noexcept {
        return min;
    }

    [[ nodiscard ]] constexpr result_type b ( ) const noexcept {
        return range + min - 1;
    }

    private:

    result_type min;
    unsigned_result_type range;
};
}

template<typename IntType>
class uniform_int_distribution_fast : public detail::param_type<IntType, uniform_int_distribution_fast<IntType>> {

    static_assert ( ( ( sizeof ( IntType ) > 1 ) and ( sizeof ( IntType ) <= 8 ) ), "char (8-bit) not supported." );

    friend struct detail::param_type<IntType, uniform_int_distribution_fast<IntType>>;

    public:

    using result_type = IntType;

    private:

    using unsigned_result_type = typename detail::unsign_integer<result_type>::type;
    using double_width_unsigned_result_type = typename detail::double_width_integer<unsigned_result_type>::type;

    [[ nodiscard ]] constexpr unsigned_result_type range_max ( ) const noexcept {
        return unsigned_result_type { 1 } << ( sizeof ( unsigned_result_type ) * 8 - 1 );
    }

    public:

    using param_type = detail::param_type<result_type, uniform_int_distribution_fast>;

    explicit uniform_int_distribution_fast ( ) noexcept :
        param_type ( std::numeric_limits<unsigned_result_type>::min ( ), std::numeric_limits<unsigned_result_type>::max ( ) ) {
    }
    explicit uniform_int_distribution_fast ( result_type a, result_type b = std::numeric_limits<result_type>::max ( ) ) noexcept :
        param_type ( a, b ) {
        assert ( b > a );
    }
    explicit uniform_int_distribution_fast ( const param_type & params_ ) noexcept :
        param_type ( params_ ) {
    }

    void reset ( ) const noexcept { }

    template<typename Gen>
    [[ nodiscard ]] result_type operator ( ) ( Gen & rng ) const noexcept {
        if constexpr ( Gen::max ( ) < std::numeric_limits<unsigned_result_type>::max ( ) ) {
            // std::cout << "bits" << nl;
            static detail::bits_engine<Gen, unsigned_result_type> bits ( rng );
            unsigned_result_type x = bits ( );
            if ( param_type::range >= range_max ( ) ) {
                do {
                    x = rng ( );
                } while ( x >= param_type::range );
                return result_type ( x ) + param_type::min;
            }
            double_width_unsigned_result_type m = double_width_unsigned_result_type ( x ) * double_width_unsigned_result_type ( param_type::range );
            unsigned_result_type l = unsigned_result_type ( m );
            if ( l < param_type::range ) {
                unsigned_result_type t = -param_type::range;
                t -= param_type::range;
                if ( t >= param_type::range ) {
                    t %= param_type::range;
                }
                while ( l < t ) {
                    x = bits ( );
                    m = double_width_unsigned_result_type ( x ) * double_width_unsigned_result_type ( param_type::range );
                    l = unsigned_result_type ( m );
                }
            }
            return result_type ( m >> std::numeric_limits<unsigned_result_type>::digits ) + param_type::min;
        }
        else {
            // std::cout << "raw" << nl;
            unsigned_result_type x = rng ( );
            if ( param_type::range >= range_max ( ) ) {
                do {
                    x = rng ( );
                } while ( x >= param_type::range );
                return result_type ( x ) + param_type::min;
            }
            double_width_unsigned_result_type m = double_width_unsigned_result_type ( x ) * double_width_unsigned_result_type ( param_type::range );
            unsigned_result_type l = unsigned_result_type ( m );
            if ( l < param_type::range ) {
                unsigned_result_type t = -param_type::range;
                t -= param_type::range;
                if ( t >= param_type::range ) {
                    t %= param_type::range;
                }
                while ( l < t ) {
                    x = rng ( );
                    m = double_width_unsigned_result_type ( x ) * double_width_unsigned_result_type ( param_type::range );
                    l = unsigned_result_type ( m );
                }
            }
            return result_type ( m >> std::numeric_limits<unsigned_result_type>::digits ) + param_type::min;
        }
    }

    [[ nodiscard ]] param_type param ( ) const noexcept {
        return *this;
    }

    void param ( const param_type & params ) noexcept {
        *this = params;
    }
};
}
