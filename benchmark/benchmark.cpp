
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

#include <ciso646>
#include <cstddef>

#include "iostream.hpp" // <iostream> + nl, sp etc. defined...
#include <random>


#include <benchmark/benchmark.h>

#ifdef _WIN32
#pragma comment ( lib, "Shlwapi.lib" )
#endif

// #include "../pcg/pcg_random.hpp"
#include "../xoroshiro.hpp"
#include "../xoroshiro128plusxoshi.hpp"
#include "../splitmix.hpp"
#include "../sfc.hpp"
#include "../lehmer.hpp"
#include "../uniform_int_distribution_fast.hpp"


template<class Gen>
void bm_generator_clobber ( benchmark::State & state ) noexcept {
    static std::uint64_t seed = 0xBE1C0467EBA5FAC;
    seed *= 0x1AEC805299990163;
    seed ^= ( seed >> 32 );
    Gen gen ( seed );
    typename Gen::result_type a = 0;
    benchmark::DoNotOptimize ( &a );
    for ( auto _ : state ) {
        for ( int i = 0; i < 128; ++i ) {
            a += gen ( );
            benchmark::ClobberMemory ( );
        }
    }
}

template<class Gen>
void bm_generator_no_clobber ( benchmark::State & state ) noexcept {
    static std::uint64_t seed = 0xBE1C0467EBA5FAC;
    seed *= 0x1AEC805299990163;
    seed ^= ( seed >> 32 );
    Gen gen ( seed );
    typename Gen::result_type a = 0;
    benchmark::DoNotOptimize ( &a );
    for ( auto _ : state ) {
        for ( int i = 0; i < 128; ++i ) {
            a += gen ( );
        }
    }
}

/*

constexpr int repeats = 16;

BENCHMARK_TEMPLATE ( bm_generator_clobber, std::mt19937_64 )
->Repetitions ( repeats )
->ReportAggregatesOnly ( true );

BENCHMARK_TEMPLATE ( bm_generator_clobber, pcg64 )
->Repetitions ( repeats )
->ReportAggregatesOnly ( true );

BENCHMARK_TEMPLATE ( bm_generator_clobber, sfc64 )
->Repetitions ( repeats )
->ReportAggregatesOnly ( true );

BENCHMARK_TEMPLATE ( bm_generator_clobber, meo::xoroshiro128plus64 )
->Repetitions ( repeats )
->ReportAggregatesOnly ( true );

BENCHMARK_TEMPLATE ( bm_generator_clobber, degski::xoroshiro128plus64xoshi32 )
->Repetitions ( repeats )
->ReportAggregatesOnly ( true );

BENCHMARK_TEMPLATE ( bm_generator_clobber, degski::xoroshiro128plus64xoshi32starxoshi32 )
->Repetitions ( repeats )
->ReportAggregatesOnly ( true );

BENCHMARK_TEMPLATE ( bm_generator_clobber, mcg128 )
->Repetitions ( repeats )
->ReportAggregatesOnly ( true );

BENCHMARK_TEMPLATE ( bm_generator_clobber, mcg128_fast )
->Repetitions ( repeats )
->ReportAggregatesOnly ( true );

BENCHMARK_TEMPLATE ( bm_generator_clobber, splitmix64 )
->Repetitions ( repeats )
->ReportAggregatesOnly ( true );


BENCHMARK_TEMPLATE ( bm_generator_no_clobber, std::mt19937_64 )
->Repetitions ( repeats )
->ReportAggregatesOnly ( true );

BENCHMARK_TEMPLATE ( bm_generator_no_clobber, pcg64 )
->Repetitions ( repeats )
->ReportAggregatesOnly ( true );

BENCHMARK_TEMPLATE ( bm_generator_no_clobber, sfc64 )
->Repetitions ( repeats )
->ReportAggregatesOnly ( true );

BENCHMARK_TEMPLATE ( bm_generator_no_clobber, meo::xoroshiro128plus64 )
->Repetitions ( repeats )
->ReportAggregatesOnly ( true );

BENCHMARK_TEMPLATE ( bm_generator_no_clobber, degski::xoroshiro128plus64xoshi32 )
->Repetitions ( repeats )
->ReportAggregatesOnly ( true );

BENCHMARK_TEMPLATE ( bm_generator_no_clobber, degski::xoroshiro128plus64xoshi32starxoshi32 )
->Repetitions ( repeats )
->ReportAggregatesOnly ( true );

BENCHMARK_TEMPLATE ( bm_generator_no_clobber, mcg128 )
->Repetitions ( repeats )
->ReportAggregatesOnly ( true );

BENCHMARK_TEMPLATE ( bm_generator_no_clobber, mcg128_fast )
->Repetitions ( repeats )
->ReportAggregatesOnly ( true );

BENCHMARK_TEMPLATE ( bm_generator_no_clobber, splitmix64 )
->Repetitions ( repeats )
->ReportAggregatesOnly ( true );

*/

int main ( ) {

    std::minstd_rand rng ( [ ] ( ) { std::random_device rdev; return ( static_cast<std::uint64_t> ( rdev ( ) ) << 32 ) | rdev ( ); } ( ) );
    ext::uniform_int_distribution_fast<std::int64_t> dis ( 500 );

    for ( int k = 0; k < 1000; k++ ) {
        std::cout << dis ( rng ) << std::endl;
    }

    std::cout << dis.a ( ) << " " << dis.b ( ) << nl;

    return EXIT_SUCCESS;
}


int main675656 ( ) {

    std::minstd_rand prng ( 42 );

    std::uniform_int_distribution<uint64_t> dist ( uint64_t ( 0 ), ~uint64_t ( 0 ) );

    for ( size_t i = 0; i < 10; ++i ) {

        uint64_t x = dist ( prng );

        const uint64_t m = ( uint64_t { 1 } << 31 ) - 2;

        std::cout << x << " " << ( x < m ) << std::endl;
    }

    return EXIT_SUCCESS;
}



























template<typename Rng>
uint64_t bounded_rand3 ( Rng & rng, uint64_t range ) {
    uint64_t x = rng ( );
    if ( range >= 1ull << 63 ) {
        while ( x >= range )
            x = rng ( );
        return x;
    }
    __uint128_t m = __uint128_t ( x ) * __uint128_t ( range );
    uint64_t l = uint64_t ( m );
    if ( l < range ) {
        uint64_t t = -range;
        t -= range;
        if ( t >= range )
            t %= range;
        while ( l < t ) {
            x = rng ( );
            m = __uint128_t ( x ) * __uint128_t ( range );
            l = uint64_t ( m );
        }
    }
    return m >> 64;
}


template<typename Rng>
uint64_t bounded_rand1 ( Rng & rng, uint64_t range ) {
    __uint128_t m = rng ( );
    m *= range;
    return m >> 64;
}
template<typename Rng>
uint64_t bounded_rand2 ( Rng & rng, uint64_t range ) {
    uint64_t x = rng ( );
    __uint128_t m = __uint128_t ( x ) * __uint128_t ( range );
    return m >> 64;
}
