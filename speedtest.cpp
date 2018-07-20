
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

#include <cstdint>
#include <iostream>
#include <limits>
#include <cmath>
#include <vector>
#include <utility>

#include "plf_nanotimer.h"
#include "statistics.hpp"
#include "generator.hpp"

template<typename Gen>
std::pair<double, std::uint64_t> test ( ) noexcept {

    plf::nanotimer timer;
    double result = 0.0;

    std::int64_t cnt = 10'000'000;

    Gen gen ( 0xBE1C0467EBA5FAC1 );

    // volatile
    std::uint64_t acc = 0;

    for  ( std::size_t i = 0; i < 1'000; ++i ) { // some warmup
        acc += gen ( );
    }

    timer.start ( );
    while ( cnt-- ) acc += gen ( );
    result = timer.get_elapsed_ms ( ); // std::cout << "Timing: " << ( std::uint64_t ) result << " milliseconds." << std::endl;

    return { result, acc };
}

template<typename Gen>
bool test_runs ( ) {

    const std::size_t n = 1'000;

    std::vector<double> results;
    results.reserve ( n );

    std::uint64_t acc = 0;

    for ( std::size_t i = 0; i < n; ++i ) {
        auto [ result, a ] = test<Gen> ( );
        acc += a;
        results.push_back ( result );
    }

    auto [ low, high, mean, variance, sd_s, sd_p ] = sf::stats ( results.data ( ), results.size ( ) );
    std::printf ( " %s: %.1f %.1f %.3f\n", generator_name ( ).c_str ( ), low, mean, sd_s );
    return acc != 0;
}


int main ( ) {

    const bool b = test_runs<Generator> ( );
    if ( !b ) {
        std::cout << " fail" << std::endl;
    }
    return 0;
}
