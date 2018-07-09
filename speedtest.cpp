
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


std::pair<double, std::uint64_t> test ( ) noexcept {

    plf::nanotimer timer;
    double result = 0.0;

    std::int64_t cnt = 100'000'000;

    Generator gen ( 0xBE1C0467EBA5FAC1 );

    volatile std::uint64_t acc = 0;

    for  ( std::size_t i = 0; i < 256; ++i ) { // some warmup
        acc += gen ( );
    }

    timer.start ( );
    while ( cnt-- ) acc += gen ( );
    result = timer.get_elapsed_ms ( ); // std::cout << "Timing: " << ( std::uint64_t ) result << " milliseconds." << std::endl;

    return { result, acc };
}


int main ( ) {

    const std::size_t n = 512;

    std::vector<double> results;
    results.reserve ( n );

    std::uint64_t acc = 0;

    for  ( std::size_t i = 0; i < n; ++i ) {
        auto [ result, a ] = test ( );
        acc += a;
        results.push_back ( result );
    }

    std::cout << "Lowest : " << ( std::uint64_t ) stats::ddmin ( results.data ( ), results.size ( ) ) << " milliseconds." << std::endl;
    std::cout << "Average: " << ( std::uint64_t ) stats::ddmean ( results.data ( ), results.size ( ) ) << " milliseconds." << std::endl;
    // std::cout << "St Dev.: " << ( std::uint64_t ) std::sqrt ( stats::ddvariance ( results.data ( ), results.size ( ) ) / results.size ( ) ) << " milliseconds." << std::endl;
    std::cout << std::boolalpha << ( acc != 0 ) << std::endl;
}
