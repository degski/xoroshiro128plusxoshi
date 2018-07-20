
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

#include <array>
#include <fstream>
#include "iostream.hpp" // <iostream> + nl, sp etc. defined...
#include <iterator>
#include <list>
#include <map>
#include <random>
#include <string>
#include <type_traits>
#include <vector>


#include <benchmark/benchmark.h>

#ifdef _WIN32
#pragma comment ( lib, "Shlwapi.lib" )
#endif

#include "../splitmix.hpp"


template<class Container>
void benchmark_emplace_back_random ( benchmark::State & state ) noexcept {
    using value_type = typename Container::value_type;
    Container data;
    splitmix64 gen;
    for ( auto _ : state ) {
        state.PauseTiming ( );
        benchmark::DoNotOptimize ( data.data ( ) );
        data = Container ( std::uniform_int_distribution<std::size_t> ( 0, 8 ) ( gen ), value_type { 1 } );
        benchmark::ClobberMemory ( );
        const std::size_t l = std::uniform_int_distribution<std::size_t> ( 1, 256 ) ( gen );
        state.ResumeTiming ( );
        for ( std::int64_t i = 0; i < l; ++i ) {
            benchmark::DoNotOptimize ( data.data ( ) );
            data.emplace_back ( ( value_type ) i );
            benchmark::ClobberMemory ( );
        }
    }
}


using tested_value_type = std::uint8_t;

BENCHMARK_TEMPLATE ( benchmark_emplace_back_random, std::vector<tested_value_type> )

->Args ( { 1, 1 } )
->Args ( { 1, 4 } )
->Args ( { 1, 8 } )
->Args ( { 1, 16 } )
->Args ( { 1, 32 } )
->Args ( { 1, 64 } )
->Args ( { 4, 1 } )
->Args ( { 4, 4 } )
->Args ( { 4, 8 } )
->Args ( { 4, 16 } )
->Args ( { 4, 32 } )
->Args ( { 4, 64 } )
->Args ( { 8, 1 } )
->Args ( { 8, 4 } )
->Args ( { 8, 8 } )
->Args ( { 8, 16 } )
->Args ( { 8, 32 } )
->Args ( { 8, 64 } )
->Args ( { 16, 1 } )
->Args ( { 16, 4 } )
->Args ( { 16, 8 } )
->Args ( { 16, 16 } )
->Args ( { 16, 32 } )
->Args ( { 16, 64 } )

->Repetitions ( 16 )
->ReportAggregatesOnly ( true )
;
/*
BENCHMARK_TEMPLATE ( benchmark_emplace_back_random, podder<tested_value_type> )

->Args ( { 1, 1 } )
->Args ( { 1, 4 } )
->Args ( { 1, 8 } )
->Args ( { 1, 16 } )
->Args ( { 1, 32 } )
->Args ( { 1, 64 } )
->Args ( { 4, 1 } )
->Args ( { 4, 4 } )
->Args ( { 4, 8 } )
->Args ( { 4, 16 } )
->Args ( { 4, 32 } )
->Args ( { 4, 64 } )
->Args ( { 8, 1 } )
->Args ( { 8, 4 } )
->Args ( { 8, 8 } )
->Args ( { 8, 16 } )
->Args ( { 8, 32 } )
->Args ( { 8, 64 } )
->Args ( { 16, 1 } )
->Args ( { 16, 4 } )
->Args ( { 16, 8 } )
->Args ( { 16, 16 } )
->Args ( { 16, 32 } )
->Args ( { 16, 64 } )

->Repetitions ( 16 )
->ReportAggregatesOnly ( true )
;
*/