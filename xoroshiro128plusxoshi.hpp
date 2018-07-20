#ifndef XOROSHIRO2_HPP_INCLUDED
#define XOROSHIRO2_HPP_INCLUDED 1

/*
 * A C++ implementation of a family of Xoroshiro generators.
 *
 * See:
 *     https://en.wikipedia.org/wiki/Xoroshiro128%2B
 *     http://xoroshiro.di.unimi.it/xoroshiro128plus.c
 *
 * Based on the design and public domain implementation of
 * Xoroshiro128+ by written in 2016 by David Blackman and Sebastiano
 * Vigna.  Also updated for the 2018 V1.0 versions of Xoroshiro.
 *
 * Xoroshiro128+ fails PractRand and TestU01 because of weak low order
 * bits.  Blackman and Vigna recommend users take care to avoid
 * relying on the low bits.  See their original source for details.
 * My test results are included below.
 *
 * Xoroshiro128+ is a variant on the ideas given by George Marsaglia
 * for XorShift, and as such it is possible to use the same
 * binary-matrix techniques to create the necessary magic constants to
 * define other members of the Xoroshiro family at different bit
 * sizes.  In particular, Xoroshiro64+ may be sufficient for some
 * applications and will run better on some 32-bit machines.
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017-18 Melissa E. O'Neill
 * Copyright (c) 2018 degski
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#include <cstdint>

namespace degski {

namespace xoroshiro_detail {

template <typename itype, typename rtype,
          unsigned int a, unsigned int b, unsigned int c>
class xoroshiro {
protected:
    itype s0_, s1_;

    static constexpr unsigned int ITYPE_BITS = 8*sizeof(itype);
    static constexpr unsigned int RTYPE_BITS = 8*sizeof(rtype);

    static inline itype rotl(const itype x, int k) {
        return (x << k) | (x >> (ITYPE_BITS - k));
    }

public:
    using result_type = rtype;

    static constexpr result_type min() { return 0; }
    static constexpr result_type max() { return ~ result_type(0); }

    xoroshiro(itype s0 = itype(0xc1f651c67c62c6e0),
              itype s1 = itype(0x30d89576f866ac9f))
        // Easter-egg seed value for Xoroshiro128+ to remind users that
        // they should seed their PRNGs properly.
        : s0_(s0), s1_((s0 || s1) ? s1 : 1)
    {
            // Nothing (else) to do.
    }

    void advance()
    {
        s1_ ^= s0_;
        s0_ = rotl(s0_, a) ^ s1_ ^ (s1_ << b);
        s1_ = rotl(s1_, c);
    }

    bool operator==(const xoroshiro& rhs)
    {
        return (s0_ == rhs.s0_) && (s1_ == rhs.s1_);
    }

    bool operator!=(const xoroshiro& rhs)
    {
        return !operator==(rhs);
    }

    // Not (yet) implemented:
    //   - arbitrary jumpahead (doable, but annoying to write).
    //   - I/O
    //   - Seeding from a seed_seq.
};


template <typename itype, typename rtype,
          unsigned int a, unsigned int b, unsigned int c, unsigned int shi>
class xoroshiro_plus_xoshi : public xoroshiro<itype,rtype,a,b,c> {
private:
    using base = xoroshiro<itype,rtype,a,b,c>;
public:
    using base::base;

    static constexpr unsigned int shift ( ) noexcept { return shi; }
    /*
    // Vigna & Blackman:
    rtype operator()()
    {
        const itype result = base::s0_ + base::s1_;
        base::advance();
        return result >> ( base::ITYPE_BITS - base::RTYPE_BITS );
    }
    */
    // xoroshiro128plusxoshi:
    rtype operator()()
    {
        const itype result = base::s0_ + base::s1_;
        base::advance();
        return ( ( result >> shi ) ^ result ) >> ( base::ITYPE_BITS - base::RTYPE_BITS );
    }
};


template <typename itype, typename rtype,
    unsigned int a, unsigned int b, unsigned int c, unsigned int shi1, unsigned int shi2>
    class xoroshiro_plus_xoshistarxoshi : public xoroshiro<itype, rtype, a, b, c> {
        private:
        using base = xoroshiro<itype, rtype, a, b, c>;
        public:
        using base::base;

        static constexpr unsigned int shift ( ) noexcept { return shi; }

        /*
        // Vigna & Blackman:
        rtype operator()()
        {
        const itype result = base::s0_ + base::s1_;
        base::advance();
        return result >> ( base::ITYPE_BITS - base::RTYPE_BITS );
        }
        */

        rtype operator()( ) {
            itype result = base::s0_ + base::s1_;
            base::s1_ ^= base::s0_;
            base::s0_ = base::rotl ( base::s0_, a ) ^ base::s1_ ^ ( base::s1_ << b );
            result ^= result >> shi1;
            base::s1_ = base::rotl ( base::s1_, c );
            result *= itype { 0x1AEC805299990163 };
            return ( ( result >> shi2 ) ^ result ) >> ( base::ITYPE_BITS - base::RTYPE_BITS );
        }
};

template <typename itype, typename rtype,
          unsigned int a, unsigned int b, unsigned int c, itype mult>
class xoroshiro_star : public xoroshiro<itype,rtype,a,b,c> {
private:
    using base = xoroshiro<itype,rtype,a,b,c>;
public:
    using base::base;

    rtype operator()()
    {
        const itype result_star = base::s0_ * mult;

        base::advance();

        return result_star >> (base::ITYPE_BITS - base::RTYPE_BITS);
    }
};

template <typename itype, typename rtype,
          unsigned int a, unsigned int b, unsigned int c,
          itype mult1, unsigned int orot, itype mult2>
class xoroshiro_starstar : public xoroshiro<itype,rtype,a,b,c> {
private:
    using base = xoroshiro<itype,rtype,a,b,c>;
public:
    using base::base;

    rtype operator()()
    {
        const itype result_ss = base::rotl(base::s0_ * mult1, orot) * mult2;

        base::advance();

        return result_ss >> (base::ITYPE_BITS - base::RTYPE_BITS);
    }
};

} // namespace xoroshiro_detail

// - 128 state bits, uint64_t output, period 2^128 - 1
template<unsigned int shift>
using xoroshiro128plus64xoshiv0_1 =
    xoroshiro_detail::xoroshiro_plus_xoshi<uint64_t, uint64_t, 55, 14, 36, shift>;

template<unsigned int shift>
using xoroshiro128plus64xoshiv1_0 =
    xoroshiro_detail::xoroshiro_plus_xoshi<uint64_t, uint64_t, 24, 16, 37, shift>;

using xoroshiro128plus64xoshi5  = xoroshiro128plus64xoshiv1_0<5>;
using xoroshiro128plus64xoshi21 = xoroshiro128plus64xoshiv1_0<21>;
using xoroshiro128plus64xoshi16 = xoroshiro128plus64xoshiv1_0<16>;
using xoroshiro128plus64xoshi24 = xoroshiro128plus64xoshiv1_0<24>;
using xoroshiro128plus64xoshi32 = xoroshiro128plus64xoshiv1_0<32>;
using xoroshiro128plus64xoshi48 = xoroshiro128plus64xoshiv1_0<48>;

template<unsigned int shift1, unsigned int shift2>
using xoroshiro128plus64xoshistarxoshiv0_1 =
xoroshiro_detail::xoroshiro_plus_xoshistarxoshi<uint64_t, uint64_t, 55, 14, 36, shift1, shift2>;

template<unsigned int shift1, unsigned int shift2>
using xoroshiro128plus64xoshistarxoshiv1_0 =
xoroshiro_detail::xoroshiro_plus_xoshistarxoshi<uint64_t, uint64_t, 24, 16, 37, shift1, shift2>;

using xoroshiro128plus64xoshi16starxoshi16 = xoroshiro128plus64xoshistarxoshiv1_0<16, 16>;
using xoroshiro128plus64xoshi32starxoshi16 = xoroshiro128plus64xoshistarxoshiv1_0<32, 16>;
using xoroshiro128plus64xoshi32starxoshi32 = xoroshiro128plus64xoshistarxoshiv1_0<32, 32>;


using xoroshiro128starstar64v1_0 =
    xoroshiro_detail::xoroshiro_starstar<uint64_t, uint64_t, 24, 16, 37,
                                         5, 7, 9>;

using xoroshiro128starstar64 = xoroshiro128starstar64v1_0;

}

// This is a variation on the above generator which tries to avoid the
// problems with weak low-order bits by discarding the low-order bits,
// to return only the high 32 bits.  It passes TestU01.  It does pass
// PractRand at normal test sizes, but eventually fails if given an
// extensive test.
//
// PractRand (extended: ./RNG_test stdin32 -te 1 -tf 2 -tlmax 50)
//
//| RNG_test using PractRand version 0.93
//| RNG = RNG_stdin32, seed = 0x79972d1f
//| test set = expanded, folding = extra
//|
//| rng=RNG_stdin32, seed=0x79972d1f
//| length= 128 megabytes (2^27 bytes), time= 2.8 seconds
//|   no anomalies in 891 test result(s)
//:
//:
//| rng=RNG_stdin32, seed=0x79972d1f
//| length= 256 terabytes (2^48 bytes), time= 4445499 seconds
//|   no anomalies in 2017 test result(s)
//|
//| rng=RNG_stdin32, seed=0x79972d1f
//| length= 512 terabytes (2^49 bytes), time= 8789501 seconds
//|   Test Name                         Raw       Processed     Evaluation
//|   BCFN_FF(2+9):freq                 R= +13.7  p~=   6e-18     FAIL !
//|   ...and 2050 test result(s) without anomalies

// - 128 state bits, uint32_t output, period 2^128 - 1
#if 0
using xoroshiro128plus32v0_1 =
    xoroshiro_detail::xoroshiro_plus<uint64_t, uint32_t, 55, 14, 36>;

using xoroshiro128plus32v1_0 =
    xoroshiro_detail::xoroshiro_plus<uint64_t, uint32_t, 24, 16, 37>;

using xoroshiro128plus32 = xoroshiro128plus32v1_0;
#endif
//// Variations ////

// These are variations on the above theme. The constants are
// mathematically correct and are designed to have a similar size
// relationships to the constants for Xoroshiro128+.  Empirical
// testing shows them to be acceptable, but it is possible that there
// are even better constants.

// These variations have similar properties to the ones above.  The
// first variant fails statistical tests due to weak low-order bits,
// and the second probably passes stringent statistical tests.

// 256 //
/*
// - 256 state bits, __uint128_t output, period 2^256 - 1
using xoroshiro256plus128 =
    xoroshiro_detail::xoroshiro_plus<__uint128_t, __uint128_t, 105, 36, 70>;

// - 256 state bits, uint64_t output, period 2^256 - 1
using xoroshiro256plus64 =
    xoroshiro_detail::xoroshiro_plus<__uint128_t, uint64_t, 105, 36, 70>;
*/
// 64 //

// - 64 state bits, uint32_t output, period 2^64 - 1
#if 0
using xoroshiro64plus32vMEO1 =
    xoroshiro_detail::xoroshiro_plus<uint32_t, uint32_t, 27, 7, 20>;

using xoroshiro64plus32v1_0 =
    xoroshiro_detail::xoroshiro_plus<uint32_t, uint32_t, 26, 9, 13>;

using xoroshiro64plus32 = xoroshiro64plus32v1_0;

using xoroshiro64star32v1_0 =
    xoroshiro_detail::xoroshiro_star<uint32_t, uint32_t, 26, 9, 13, 0x9E3779BB>;

using xoroshiro64star32 = xoroshiro64star32v1_0;

using xoroshiro64starstar32v1_0 =
    xoroshiro_detail::xoroshiro_starstar<uint32_t, uint32_t, 26, 9, 13,
                                         0x9E3779BB, 5, 5>;

using xoroshiro64starstar32 = xoroshiro64starstar32v1_0;


// - 64 state bits, uint16_t output, period 2^64 - 1
using xoroshiro64plus16vMEO1 =
    xoroshiro_detail::xoroshiro_plus<uint32_t, uint16_t, 27, 7, 20>;

using xoroshiro64plus16v1_0 =
    xoroshiro_detail::xoroshiro_plus<uint32_t, uint16_t, 26, 9, 13>;

using xoroshiro64plus16 = xoroshiro64plus16v1_0;

// 32 //

// - 32 state bits, uint16_t output, period 2^32 - 1
using xoroshiro32plus16 =
    xoroshiro_detail::xoroshiro_plus<uint16_t, uint16_t, 13, 5, 10>;

using xoroshiro32star16 =
    xoroshiro_detail::xoroshiro_star<uint16_t, uint16_t, 13, 5, 10, 0x79BB>;

// - 32 state bits, uint8_t output, period 2^32 - 1
using xoroshiro32plus8 =
    xoroshiro_detail::xoroshiro_plus<uint16_t, uint8_t, 13, 5, 10>;

// 16 //

// - 16 state bits, uint8_t output, period 2^16 - 1
using xoroshiro16plus8 =
    xoroshiro_detail::xoroshiro_plus<uint8_t, uint8_t, 4, 7, 3>;

using xoroshiro16star8 =
    xoroshiro_detail::xoroshiro_star<uint8_t, uint8_t, 4, 7, 3, 0xBB>;

}
#endif
#endif // XOROSHIRO2_HPP_INCLUDED
