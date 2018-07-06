
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

#include "plf_nanotimer.h"
#include "xoroshiro_meo.hpp"
#include "xoroshiro2.hpp"


#define VOLATILE volatile


namespace ver = v2;


int main ( ) {

    plf::nanotimer timer;
    double results = 0.0;

    std::uint64_t cnt = 1;

    ver::xoroshiro128plus64 gen1 ( 0xBE1C0467EBA5FAC1 );
    ver::xoroshiro128plus64 gen2 ( 0xBE2C0467EBA5FAC2 );
    ver::xoroshiro128plus64 gen3 ( 0xBE3C0467EBA5FAC3 );
    ver::xoroshiro128plus64 gen4 ( 0xBE4C0467EBA5FAC4 );
    ver::xoroshiro128plus64 gen5 ( 0xBE5C0467EBA5FAC5 );

    while ( cnt-- ) {

        std::int64_t cnt1 = 1'000'000'000, cnt2 = cnt1, cnt3 = cnt1, cnt4 = cnt1, cnt5 = cnt1;

        VOLATILE std::uint64_t acc1 = 0;
        timer.start ( );
        while ( cnt1-- ) acc1 += gen1 ( );
        results = timer.get_elapsed_ms ( ); std::cout << "Timing: " << ( std::uint64_t ) results << " milliseconds." << std::endl;

        VOLATILE std::uint64_t acc2 = 0;
        timer.start ( );
        while ( cnt2-- ) acc2 += gen2 ( );
        results = timer.get_elapsed_ms ( ); std::cout << "Timing: " << ( std::uint64_t ) results << " milliseconds." << std::endl;

        VOLATILE std::uint64_t acc3 = 0;
        timer.start ( );
        while ( cnt3-- ) acc3 += gen3 ( );
        results = timer.get_elapsed_ms ( ); std::cout << "Timing: " << ( std::uint64_t ) results << " milliseconds." << std::endl;

        VOLATILE std::uint64_t acc4 = 0;
        timer.start ( );
        while ( cnt4-- ) acc4 += gen4 ( );
        results = timer.get_elapsed_ms ( ); std::cout << "Timing: " << ( std::uint64_t ) results << " milliseconds." << std::endl;

        VOLATILE std::uint64_t acc5 = 0;
        timer.start ( );
        while ( cnt5-- ) acc1 += gen5 ( );
        results = timer.get_elapsed_ms ( ); std::cout << "Timing: " << ( std::uint64_t ) results << " milliseconds." << std::endl;
    }
}
