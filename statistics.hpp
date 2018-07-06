
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

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdint>

namespace stats {

float min ( float* data, std::size_t n ) noexcept;
double ddmin ( double* data, std::size_t n ) noexcept;
float max ( float* data, std::size_t n ) noexcept;
double ddmax ( double* data, std::size_t n ) noexcept;
float mean ( float* data, std::size_t n ) noexcept;
double dmean ( float* data, std::size_t n ) noexcept;
double ddmean ( double* data, std::size_t n ) noexcept;
float variance ( float* data, std::size_t n ) noexcept;
double dvariance ( float* data, std::size_t n ) noexcept;
double ddvariance ( double* data, std::size_t n ) noexcept;
float covariance ( float* data1, float* data2, std::size_t n ) noexcept;
float covariance_5 ( float* data1, float* data2, std::size_t n, float fmean1_s, float fmean2_s ) noexcept;
float sd ( float* data, std::size_t n ) noexcept;
double sd_3 ( float* data, std::size_t n, double dmean_s ) noexcept;
float skew ( float* data, std::size_t n ) noexcept;
float kurtosis ( float* data, std::size_t n ) noexcept;
float correlation ( float* data1, float* data2, std::size_t n ) noexcept;
float absdev ( float* data, std::size_t n ) noexcept;

}