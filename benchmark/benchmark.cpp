
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

    splitmix64 g ( [ ] ( ) { std::random_device rdev; return ( static_cast<std::uint64_t> ( rdev ( ) ) << 32 ) | rdev ( ); } ( ) );
    uniform_int_distribution_fast<std::int64_t> d ( -100, 100 );

    std::cout << d ( g ) << nl;
    std::cout << d ( g ) << nl;
    std::cout << d ( g ) << nl;

    std::cout << d.a ( ) << nl;

    auto par = d.param ( );

    uniform_int_distribution_fast<std::int64_t> e ( -100, 100 );

    std::cout << std::boolalpha << ( d == e ) << nl;
    std::cout << e.a ( ) << nl;

    std::uint64_t f = 123;
    std::int64_t h = 5;

    auto j = f + h;

    return EXIT_SUCCESS;
}
