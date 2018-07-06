
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

#include "statistics.hpp"
#include <limits>

namespace stats {


float min ( float* data, std::size_t n ) noexcept {

	float r = std::numeric_limits<float>::max ( );
	
	for ( std::size_t i = 0; i < n; i++ ) {
		if ( data [ i ] < r ) {
			r = data [ i ];
		}
	}
	
	return r;
}

double ddmin ( double* data, std::size_t n ) noexcept {

	double r = std::numeric_limits<double>::max ( );
	
	for ( std::size_t i = 0; i < n; i++ ) {
		if ( data [ i ] < r ) {
			r = data [ i ];
		}
	}
	
	return r;
}

float max ( float* data, std::size_t n ) noexcept {

	float r = std::numeric_limits<float>::min ( );
	
	for ( std::size_t i = 0; i < n; i++ ) {
		if ( data [ i ] > r ) {
			r = data [ i ];
		}
	}
	
	return r;
}

double ddmax ( double* data, std::size_t n ) noexcept {

	double r = std::numeric_limits<double>::min ( );
	
	for ( std::size_t i = 0; i < n; i++ ) {
		if ( data [ i ] > r ) {
			r = data [ i ];
		}
	}
	
	return r;
}


float mean ( float* data, std::size_t n ) noexcept {

    double dmean = 0.0;

    for ( std::size_t i = 0; i < n; i++ ) {
        dmean += ( ( double ) data [ i ] - dmean ) / ( i + 1 ); }

    return ( float ) dmean;
}


double dmean ( float* data, std::size_t n ) noexcept {

    double dmean = 0.0;

    for ( std::size_t i = 0; i < n; i++ ) {
        dmean += ( ( double ) data [ i ] - dmean ) / ( i + 1 ); }

    return dmean;
}


double ddmean ( double* data, std::size_t n ) noexcept {

    double dmean = 0.0;

    for ( std::size_t i = 0; i < n; i++ ) {
        dmean += ( data [ i ] - dmean ) / ( i + 1 ); }

    return dmean;
}


float variance ( float* data, std::size_t n ) noexcept {

    double dvariance = 0.0, mean = dmean ( data, n );

    for ( std::size_t i = 0; i < n; i++ ) {

        const double delta = ( ( double ) data [ i ] - mean );
        dvariance += ( delta * delta - dvariance ) / ( i + 1 );
    }

    return ( float ) dvariance;
}


double dvariance ( float* data, std::size_t n ) noexcept {

    double dvariance = 0.0, mean = dmean ( data, n );

    for ( std::size_t i = 0; i < n; i++ ) {

        const double delta = ( ( double ) data [ i ] - mean );
        dvariance += ( delta * delta - dvariance ) / ( i + 1 );
    }

    return dvariance;
}


double ddvariance ( double* data, std::size_t n ) noexcept {

    double dvariance = 0.0, mean = ddmean ( data, n );

    for ( std::size_t i = 0; i < n; i++ ) {

        const double delta = ( data [ i ] - mean );
        dvariance += ( delta * delta - dvariance ) / ( i + 1 );
    }

    return dvariance;
}


float covariance ( float* data1, float* data2, std::size_t n ) noexcept {

    double dcovariance = 0.0, dmean1 = 0.0, dmean2 = 0.0;

    for ( std::size_t i = 0; i < n; i++ ) {

        const double j = 1 / ( i + 1 );
        dmean1 += ( ( double ) data1 [ i ] - dmean1 ) * j;
        dmean2 += ( ( double ) data2 [ i ] - dmean2 ) * j;
    }

    for ( std::size_t i = 0; i < n; i++ ) {
        dcovariance += ( ( ( double ) data1 [ i ] - dmean1 ) * ( ( double ) data2 [ i ] - dmean2 ) - dcovariance ) / ( i + 1 ); }

    return ( float ) dcovariance ;
}


float covariance_5 ( float* data1, float* data2, std::size_t n, float fmean1, float fmean2 ) noexcept {

    double dcovariance = 0.0;

    // find the sum of the squares

    for ( std::size_t i = 0; i < n; i++ ) {

        const double delta1 = ( ( double ) data1 [ i ] - fmean1 );
        const double delta2 = ( ( double ) data2 [ i ] - fmean2 );
        dcovariance += ( delta1 * delta2 - dcovariance ) / ( i + 1 );
    }

    return ( float ) dcovariance ;
}


float sd ( float* data, std::size_t n ) noexcept {

    double dvariance = 0.0, mean = dmean ( data, n );

    for ( std::size_t i = 0; i < n; i++ ) {

        const double delta = ( ( double ) data [ i ] - mean );
        dvariance += ( delta * delta - dvariance ) / ( i + 1 );
    }

    return ( float ) sqrt ( dvariance );
}


double sd_3 ( float* data, std::size_t n, double dmean ) noexcept {

    double dvariance = 0.0;

    for ( std::size_t i = 0; i < n; i++ ) {

        const double delta = ( ( double ) data [ i ] - dmean );
        dvariance += ( delta * delta - dvariance ) / ( i + 1 );
    }

    return sqrt ( dvariance );
}


float skew ( float* data, std::size_t n ) noexcept {

    double dskew = 0.0, mean = dmean ( data, n ), sd = sd_3 ( data, n, mean );

    for ( std::size_t i = 0; i < n; i++ ) {

        const double x = ( ( double ) data [ i ] - mean ) / sd;
        dskew += ( x * x * x - dskew ) / ( i + 1 );
    }

    return ( float ) dskew;
}


float kurtosis ( float *data, std::size_t n ) noexcept {

    double davg = 0.0, mean = dmean ( data, n ), sd = sd_3 ( data, n, mean );

    for ( std::size_t i = 0; i < n; i++ ) {

        const double x = ( ( double ) data [ i ] - mean ) / sd;
        davg += ( x * x * x * x - davg ) / ( i + 1 );
    }

    return ( float ) ( davg - 3.0 );  // makes kurtosis zero for a Gaussian
}


float correlation ( float* data1, float* data2, std::size_t n ) noexcept {

    double sum_xsq = 0.0, sum_ysq = 0.0, sum_cross = 0.0, delta_x, delta_y, mean_x, mean_y;

    mean_x = data1 [ 0 ];
    mean_y = data2 [ 0 ];

    for ( std::size_t i = 1; i < n; ++i ) {

        const double j = 1.0f / ( ( double ) i + 1.0f );
        const double ratio = i * j;
        delta_x = ( double ) data1 [ i ] - mean_x;
        delta_y = ( double ) data2 [ i ] - mean_y;
        sum_xsq += delta_x * delta_x * ratio;
        sum_ysq += delta_y * delta_y * ratio;
        sum_cross += delta_x * delta_y * ratio;
        mean_x += delta_x * j;
        mean_y += delta_y * j;
    }

    sum_xsq = sqrt ( sum_xsq );
    sum_xsq *= sqrt ( sum_ysq );

    return ( float ) ( sum_cross / sum_xsq );
}


float absdev ( float* data, std::size_t n ) noexcept {

    double dabsdev = 0.0, mean = dmean ( data, n );

    for ( std::size_t i = 0; i < n; i++ ) {
        dabsdev += ( fabs ( ( double ) data [ i ] - mean ) - dabsdev )  / ( i + 1 ); }

    return ( float ) dabsdev;
}

}
