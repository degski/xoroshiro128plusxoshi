
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

#include <cstdint>


template<typename Generator>
struct generator_cache : private Generator {

    using result_type = typename Generator::result_type;
    using size_type = std::size_t;

    generator_cache ( ) : Generator ( ) { }
    generator_cache ( result_type v ) : Generator ( v ) { }
    template<typename SeedSeq>
    generator_cache ( const SeedSeq & seq ) : Generator ( seq ) { }
    template<typename It>
    generator_cache ( It & first_, It last ) : Generator ( first_, last ) { }

    result_type operator ( ) ( ) noexcept {
        if ( m_index == data_size ( ) ) {
            Generator::generate ( std::begin ( m_data ), std::end ( m_data ) );
            // std::generate ( std::begin ( m_data ), std::end ( m_data ), [ this ] ( ) { return Generator::operator ( ) ( ); } );
            m_index = 1;
            return m_data [ 0 ];
        }
        return m_data [ m_index++ ];
    }

    static inline constexpr result_type min ( ) noexcept { return Generator::min ( ); }
    static inline constexpr result_type max ( ) noexcept { return Generator::max ( ); }
    static inline constexpr std::size_t data_size ( ) noexcept { return std::size_t { 64 } / sizeof ( result_type ); } // assumes a 64 byte cache.

    result_type m_data [ data_size ( ) ];
    size_type m_index = data_size ( );
};
