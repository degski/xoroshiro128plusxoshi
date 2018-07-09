
# xoroshiro2

Improved xoroshiro generator, better at **zero cost** (Intel Ci3-5005U (Broadwell) CPU), compiler [`LLVM-7.0.0-r336178-win64`](https://llvm.org/builds/).

Included a modified xoroshiro implementation by Melissa E. O'Neill, the author of the excellent [pcg](http://www.pcg-random.org/) familiy of prng's. Don't miss her [blog](http://www.pcg-random.org/blog/), which is a gold-mine of information (and code) and is updated regulary.

tl;dr: the output function of `xoroshiro128plus` has been modified as per below:

    rtype operator()()
    {
            const itype result = base::s0_ + base::s1_;

            base::advance();

            // Melissa E. O'Neill:
            // return result >> ( base::ITYPE_BITS - base::RTYPE_BITS );
            // degski:
            return ( ( result >> 16 ) ^ result ) >> ( base::ITYPE_BITS - base::RTYPE_BITS );
    }


## Results


### Speed-test

#### Context

* Intel Ci3-5005U (Broadwell) CPU
* Windows 10-1803-x64 in `diagnostics mode` (core services only), and Windows Defender off
* Compiler: [`LLVM-7.0.0-r336178-win64`](http://prereleases.llvm.org/win-snapshots/LLVM-7.0.0-r336178-win64.exe)
* Command-line: `clang-cl -fuse-ld=lld -flto=thin  /D "NDEBUG" /D "_CONSOLE" /D "NOMINMAX" /D "_UNICODE" /D "UNICODE" -Xclang -fcxx-exceptions /Ox /Oi /MT main.cpp statistics.cpp -Xclang -std=c++2a -Xclang -ffast-math -mmmx  -msse  -msse2 -msse3 -mssse3 -msse4.1 -msse4.2 -mavx -mavx2`
* Generator tested: the improved 2018  `xoroshiro128plus64 v1`
* 100'000'000 numbers generated, 512 runs


#### Results

     generator                    fastest (ms)    mean (ms)

    * pcg64                         372            373
    * sfc (Chris Doty-Humphrey)     357            365 *)
    * mcg128 (Lehmer (meo))         308            313 *)
    * splitmix64                    305            309
    * mcg128_fast (Lehmer (meo))    303            308 *)
    * xoroshiro128plus64-v1         300            314
    * shift 48                      300            316
    * shift 32                      300            317
    * shift 16                      300            313

*) not yet tested in `diagnostic mode`, will shave of some ms.


### Practrand results

#### Xoroshiro128plus64-v1

    RNG_test using PractRand version 0.93
    RNG = RNG_stdin64, seed = 0x6259a10f
    test set = normal, folding = standard (64 bit)

    rng=RNG_stdin64, seed=0x6259a10f
    length= 128 megabytes (2^27 bytes), time= 3.9 seconds
      Test Name                         Raw       Processed     Evaluation
      [Low1/64]BRank(12):256(2)         R= +3748  p~=  3e-1129    FAIL !!!!!!!!
      [Low1/64]BRank(12):384(1)         R= +5405  p~=  3e-1628    FAIL !!!!!!!!
      ...and 146 test result(s) without anomalies

    rng=RNG_stdin64, seed=0x6259a10f
    length= 256 megabytes (2^28 bytes), time= 9.3 seconds
      Test Name                         Raw       Processed     Evaluation
      [Low4/64]BRank(12):768(1)         R= +1272  p~=  5.4e-384   FAIL !!!!!!!
      [Low1/64]BRank(12):256(2)         R= +3748  p~=  3e-1129    FAIL !!!!!!!!
      [Low1/64]BRank(12):384(1)         R= +5405  p~=  3e-1628    FAIL !!!!!!!!
      [Low1/64]BRank(12):512(1)         R= +8161  p~=  1e-2457    FAIL !!!!!!!!
      ...and 155 test result(s) without anomalies

    rng=RNG_stdin64, seed=0x6259a10f
    length= 512 megabytes (2^29 bytes), time= 18.4 seconds
      Test Name                         Raw       Processed     Evaluation
      [Low4/64]BRank(12):768(1)         R= +1272  p~=  5.4e-384   FAIL !!!!!!!
      [Low4/64]BRank(12):1K(1)          R= +2650  p~=  9.8e-799   FAIL !!!!!!!
      [Low1/64]Gap-16:A                 R=  -3.5  p =1-1.5e-3   unusual
      [Low1/64]BRank(12):256(2)         R= +3748  p~=  3e-1129    FAIL !!!!!!!!
      [Low1/64]BRank(12):384(1)         R= +5405  p~=  3e-1628    FAIL !!!!!!!!
      [Low1/64]BRank(12):512(1)         R= +8161  p~=  1e-2457    FAIL !!!!!!!!
      ...and 163 test result(s) without anomalies

    rng=RNG_stdin64, seed=0x6259a10f
    length= 1 gigabyte (2^30 bytes), time= 35.7 seconds
      Test Name                         Raw       Processed     Evaluation
      [Low4/64]BRank(12):768(1)         R= +1272  p~=  5.4e-384   FAIL !!!!!!!
      [Low4/64]BRank(12):1K(1)          R= +2650  p~=  9.8e-799   FAIL !!!!!!!
      [Low1/64]DC6-9x1Bytes-1           R=  +5.4  p =  8.8e-3   unusual
      [Low1/64]BRank(12):256(2)         R= +3748  p~=  3e-1129    FAIL !!!!!!!!
      [Low1/64]BRank(12):384(1)         R= +5405  p~=  3e-1628    FAIL !!!!!!!!
      [Low1/64]BRank(12):512(2)         R=+11541  p~=  2e-3475    FAIL !!!!!!!!
      [Low1/64]BRank(12):768(1)         R=+13672  p~=  1e-4116    FAIL !!!!!!!!
      ...and 173 test result(s) without anomalies

    rng=RNG_stdin64, seed=0x6259a10f
    length= 2 gigabytes (2^31 bytes), time= 63.2 seconds
      Test Name                         Raw       Processed     Evaluation
      [Low4/64]BRank(12):768(1)         R= +1272  p~=  5.4e-384   FAIL !!!!!!!
      [Low4/64]BRank(12):1K(2)          R= +3748  p~=  3e-1129    FAIL !!!!!!!!
      [Low4/64]BRank(12):1536(1)        R= +5405  p~=  3e-1628    FAIL !!!!!!!!
      [Low1/64]BRank(12):256(2)         R= +3748  p~=  3e-1129    FAIL !!!!!!!!
      [Low1/64]BRank(12):384(1)         R= +5405  p~=  3e-1628    FAIL !!!!!!!!
      [Low1/64]BRank(12):512(2)         R=+11541  p~=  2e-3475    FAIL !!!!!!!!
      [Low1/64]BRank(12):768(1)         R=+13672  p~=  1e-4116    FAIL !!!!!!!!
      [Low1/64]BRank(12):1K(1)          R=+19183  p~=  1e-5775    FAIL !!!!!!!!
      ...and 183 test result(s) without anomalies

    rng=RNG_stdin64, seed=0x6259a10f
    length= 4 gigabytes (2^32 bytes), time= 121 seconds
      Test Name                         Raw       Processed     Evaluation
      [Low16/64]BRank(12):3K(1)         R= +1272  p~=  5.4e-384   FAIL !!!!!!!
      [Low4/64]BRank(12):768(1)         R= +1272  p~=  5.4e-384   FAIL !!!!!!!
      [Low4/64]BRank(12):1K(2)          R= +3748  p~=  3e-1129    FAIL !!!!!!!!
      [Low4/64]BRank(12):1536(1)        R= +5405  p~=  3e-1628    FAIL !!!!!!!!
      [Low4/64]BRank(12):2K(1)          R= +8161  p~=  1e-2457    FAIL !!!!!!!!
      [Low1/64]BRank(12):256(2)         R= +3748  p~=  3e-1129    FAIL !!!!!!!!
      [Low1/64]BRank(12):384(1)         R= +5405  p~=  3e-1628    FAIL !!!!!!!!
      [Low1/64]BRank(12):512(2)         R=+11541  p~=  2e-3475    FAIL !!!!!!!!
      [Low1/64]BRank(12):768(1)         R=+13672  p~=  1e-4116    FAIL !!!!!!!!
      [Low1/64]BRank(12):1K(1)          R=+19183  p~=  1e-5775    FAIL !!!!!!!!
      ...and 191 test result(s) without anomalies

    rng=RNG_stdin64, seed=0x6259a10f
    length= 8 gigabytes (2^33 bytes), time= 248 seconds
      Test Name                         Raw       Processed     Evaluation
      [Low16/64]FPF-14+6/16:cross       R=  +4.0  p =  1.0e-3   unusual
      [Low16/64]BRank(12):3K(1)         R= +1272  p~=  5.4e-384   FAIL !!!!!!!
      [Low16/64]BRank(12):4K(1)         R= +2650  p~=  9.8e-799   FAIL !!!!!!!
      [Low4/64]DC6-9x1Bytes-1           R=  -4.1  p =1-5.6e-3   unusual
      [Low4/64]BRank(12):768(1)         R= +1272  p~=  5.4e-384   FAIL !!!!!!!
      [Low4/64]BRank(12):1K(2)          R= +3748  p~=  3e-1129    FAIL !!!!!!!!
      [Low4/64]BRank(12):1536(1)        R= +5405  p~=  3e-1628    FAIL !!!!!!!!
      [Low4/64]BRank(12):2K(1)          R= +8161  p~=  1e-2457    FAIL !!!!!!!!
      [Low1/64]BRank(12):256(4)         R= +5300  p~=  1e-2819    FAIL !!!!!!!!
      [Low1/64]BRank(12):384(1)         R= +5405  p~=  3e-1628    FAIL !!!!!!!!
      [Low1/64]BRank(12):512(2)         R=+11541  p~=  2e-3475    FAIL !!!!!!!!
      [Low1/64]BRank(12):768(1)         R=+13672  p~=  1e-4116    FAIL !!!!!!!!
      [Low1/64]BRank(12):1K(2)          R=+27129  p~=  1e-8167    FAIL !!!!!!!!
      [Low1/64]BRank(12):1536(1)        R=+30205  p~=  1e-9093    FAIL !!!!!!!!
      ...and 198 test result(s) without anomalies

    rng=RNG_stdin64, seed=0x6259a10f
    length= 16 gigabytes (2^34 bytes), time= 467 seconds
      Test Name                         Raw       Processed     Evaluation
      [Low16/64]BRank(12):3K(1)         R= +1272  p~=  5.4e-384   FAIL !!!!!!!
      [Low16/64]BRank(12):4K(1)         R= +2650  p~=  9.8e-799   FAIL !!!!!!!
      [Low4/64]BRank(12):768(1)         R= +1272  p~=  5.4e-384   FAIL !!!!!!!
      [Low4/64]BRank(12):1K(2)          R= +3748  p~=  3e-1129    FAIL !!!!!!!!
      [Low4/64]BRank(12):1536(1)        R= +5405  p~=  3e-1628    FAIL !!!!!!!!
      [Low4/64]BRank(12):2K(2)          R=+11541  p~=  2e-3475    FAIL !!!!!!!!
      [Low4/64]BRank(12):3K(1)          R=+13672  p~=  1e-4116    FAIL !!!!!!!!
      [Low1/64]BRank(12):256(4)         R= +5300  p~=  1e-2819    FAIL !!!!!!!!
      [Low1/64]BRank(12):384(1)         R= +5405  p~=  3e-1628    FAIL !!!!!!!!
      [Low1/64]BRank(12):512(2)         R=+11541  p~=  2e-3475    FAIL !!!!!!!!
      [Low1/64]BRank(12):768(1)         R=+13672  p~=  1e-4116    FAIL !!!!!!!!
      [Low1/64]BRank(12):1K(2)          R=+27129  p~=  1e-8167    FAIL !!!!!!!!
      [Low1/64]BRank(12):1536(1)        R=+30205  p~=  1e-9093    FAIL !!!!!!!!
      [Low1/64]BRank(12):2K(1)          R=+41227  p~= 0           FAIL !!!!!!!!
      ...and 209 test result(s) without anomalies

    rng=RNG_stdin64, seed=0x6259a10f
    length= 32 gigabytes (2^35 bytes), time= 767 seconds
      Test Name                         Raw       Processed     Evaluation
      [Low16/64]BRank(12):3K(1)         R= +1272  p~=  5.4e-384   FAIL !!!!!!!
      [Low16/64]BRank(12):4K(2)         R= +3748  p~=  3e-1129    FAIL !!!!!!!!
      [Low16/64]BRank(12):6K(1)         R= +5405  p~=  3e-1628    FAIL !!!!!!!!
      [Low4/64]BRank(12):768(1)         R= +1272  p~=  5.4e-384   FAIL !!!!!!!
      [Low4/64]BRank(12):1K(2)          R= +3748  p~=  3e-1129    FAIL !!!!!!!!
      [Low4/64]BRank(12):1536(1)        R= +5405  p~=  3e-1628    FAIL !!!!!!!!
      [Low4/64]BRank(12):2K(2)          R=+11541  p~=  2e-3475    FAIL !!!!!!!!
      [Low4/64]BRank(12):3K(1)          R=+13672  p~=  1e-4116    FAIL !!!!!!!!
      [Low4/64]BRank(12):4K(1)          R=+19183  p~=  1e-5775    FAIL !!!!!!!!
      [Low1/64]BRank(12):256(4)         R= +5300  p~=  1e-2819    FAIL !!!!!!!!
      [Low1/64]BRank(12):384(1)         R= +5405  p~=  3e-1628    FAIL !!!!!!!!
      [Low1/64]BRank(12):512(2)         R=+11541  p~=  2e-3475    FAIL !!!!!!!!
      [Low1/64]BRank(12):768(1)         R=+13672  p~=  1e-4116    FAIL !!!!!!!!
      [Low1/64]BRank(12):1K(2)          R=+27129  p~=  1e-8167    FAIL !!!!!!!!
      [Low1/64]BRank(12):1536(1)        R=+30205  p~=  1e-9093    FAIL !!!!!!!!
      [Low1/64]BRank(12):2K(1)          R=+41227  p~= 0           FAIL !!!!!!!!
      ...and 217 test result(s) without anomalies

    rng=RNG_stdin64, seed=0x6259a10f
    length= 64 gigabytes (2^36 bytes), time= 1370 seconds
      Test Name                         Raw       Processed     Evaluation
      BRank(12):12K(1)                  R= +1272  p~=  5.4e-384   FAIL !!!!!!!
      [Low16/64]BRank(12):3K(1)         R= +1272  p~=  5.4e-384   FAIL !!!!!!!
      [Low16/64]BRank(12):4K(2)         R= +3748  p~=  3e-1129    FAIL !!!!!!!!
      [Low16/64]BRank(12):6K(1)         R= +5405  p~=  3e-1628    FAIL !!!!!!!!
      [Low16/64]BRank(12):8K(1)         R= +8161  p~=  1e-2457    FAIL !!!!!!!!
      [Low4/64]BRank(12):768(1)         R= +1272  p~=  5.4e-384   FAIL !!!!!!!
      [Low4/64]BRank(12):1K(2)          R= +3748  p~=  3e-1129    FAIL !!!!!!!!
      [Low4/64]BRank(12):1536(1)        R= +5405  p~=  3e-1628    FAIL !!!!!!!!
      [Low4/64]BRank(12):2K(2)          R=+11541  p~=  2e-3475    FAIL !!!!!!!!
      [Low4/64]BRank(12):3K(1)          R=+13672  p~=  1e-4116    FAIL !!!!!!!!
      [Low4/64]BRank(12):4K(1)          R=+19183  p~=  1e-5775    FAIL !!!!!!!!
      [Low1/64]BRank(12):256(4)         R= +5300  p~=  1e-2819    FAIL !!!!!!!!
      [Low1/64]BRank(12):384(1)         R= +5405  p~=  3e-1628    FAIL !!!!!!!!
      [Low1/64]BRank(12):512(4)         R=+16322  p~=  1e-8681    FAIL !!!!!!!!
      [Low1/64]BRank(12):768(1)         R=+13672  p~=  1e-4116    FAIL !!!!!!!!
      [Low1/64]BRank(12):1K(2)          R=+27129  p~=  1e-8167    FAIL !!!!!!!!
      [Low1/64]BRank(12):1536(1)        R=+30205  p~=  1e-9093    FAIL !!!!!!!!
      [Low1/64]BRank(12):2K(2)          R=+58303  p~= 0           FAIL !!!!!!!!
      [Low1/64]BRank(12):3K(1)          R=+63271  p~= 0           FAIL !!!!!!!!
      ...and 225 test result(s) without anomalies


#### Splitmix64

    RNG_test using PractRand version 0.93
    RNG = RNG_stdin64, seed = 0x842e3c04
    test set = normal, folding = standard (64 bit)

    rng=RNG_stdin64, seed=0x842e3c04
    length= 128 megabytes (2^27 bytes), time= 2.3 seconds
      Test Name                         Raw       Processed     Evaluation
      [Low16/64]Gap-16:B                R=  -4.1  p =1-1.7e-3   unusual
      ...and 147 test result(s) without anomalies

    rng=RNG_stdin64, seed=0x842e3c04
    length= 256 megabytes (2^28 bytes), time= 5.4 seconds
      no anomalies in 159 test result(s)

    rng=RNG_stdin64, seed=0x842e3c04
    length= 512 megabytes (2^29 bytes), time= 10.6 seconds
      Test Name                         Raw       Processed     Evaluation
      [Low4/64]DC6-9x1Bytes-1           R=  -3.9  p =1-7.4e-3   unusual
      ...and 168 test result(s) without anomalies

    rng=RNG_stdin64, seed=0x842e3c04
    length= 1 gigabyte (2^30 bytes), time= 20.3 seconds
      no anomalies in 180 test result(s)

    rng=RNG_stdin64, seed=0x842e3c04
    length= 2 gigabytes (2^31 bytes), time= 39.0 seconds
      no anomalies in 191 test result(s)

    rng=RNG_stdin64, seed=0x842e3c04
    length= 4 gigabytes (2^32 bytes), time= 74.8 seconds
      no anomalies in 201 test result(s)

    rng=RNG_stdin64, seed=0x842e3c04
    length= 8 gigabytes (2^33 bytes), time= 147 seconds
      no anomalies in 212 test result(s)

    rng=RNG_stdin64, seed=0x842e3c04
    length= 16 gigabytes (2^34 bytes), time= 289 seconds
      no anomalies in 223 test result(s)

    rng=RNG_stdin64, seed=0x842e3c04
    length= 32 gigabytes (2^35 bytes), time= 563 seconds
      no anomalies in 233 test result(s)

    rng=RNG_stdin64, seed=0x842e3c04
    length= 64 gigabytes (2^36 bytes), time= 1144 seconds
      no anomalies in 244 test result(s)

    rng=RNG_stdin64, seed=0x842e3c04
    length= 128 gigabytes (2^37 bytes), time= 2331 seconds
      no anomalies in 255 test result(s)

    I'll test some more.


#### Pcg64

    pcg64 was not tested. I don't have enough time to wait for it to fail.


#### Xoroshiro128plus64-v1 + Shift 16

    RNG_test using PractRand version 0.93
    RNG = RNG_stdin64, seed = 0xdb4dda77
    test set = normal, folding = standard (64 bit)

    rng=RNG_stdin64, seed=0xdb4dda77
    length= 128 megabytes (2^27 bytes), time= 2.3 seconds
      no anomalies in 148 test result(s)

    rng=RNG_stdin64, seed=0xdb4dda77
    length= 256 megabytes (2^28 bytes), time= 5.4 seconds
      no anomalies in 159 test result(s)

    rng=RNG_stdin64, seed=0xdb4dda77
    length= 512 megabytes (2^29 bytes), time= 10.6 seconds
      no anomalies in 169 test result(s)

    rng=RNG_stdin64, seed=0xdb4dda77
    length= 1 gigabyte (2^30 bytes), time= 20.4 seconds
      no anomalies in 180 test result(s)

    rng=RNG_stdin64, seed=0xdb4dda77
    length= 2 gigabytes (2^31 bytes), time= 38.8 seconds
      no anomalies in 191 test result(s)

    rng=RNG_stdin64, seed=0xdb4dda77
    length= 4 gigabytes (2^32 bytes), time= 73.8 seconds
      no anomalies in 201 test result(s)

    rng=RNG_stdin64, seed=0xdb4dda77
    length= 8 gigabytes (2^33 bytes), time= 146 seconds
      no anomalies in 212 test result(s)

    rng=RNG_stdin64, seed=0xdb4dda77
    length= 16 gigabytes (2^34 bytes), time= 287 seconds
      no anomalies in 223 test result(s)

    rng=RNG_stdin64, seed=0xdb4dda77
    length= 32 gigabytes (2^35 bytes), time= 569 seconds
      Test Name                         Raw       Processed     Evaluation
      [Low1/64]Gap-16:A                 R=  +4.8  p =  1.2e-3   unusual
      ...and 232 test result(s) without anomalies

    rng=RNG_stdin64, seed=0xdb4dda77
    length= 64 gigabytes (2^36 bytes), time= 1135 seconds
      Test Name                         Raw       Processed     Evaluation
      BRank(12):12K(1)                  R= +1272  p~=  5.4e-384   FAIL !!!!!!!
      ...and 243 test result(s) without anomalies


#### Xoroshiro128plus64-v1 + Shift 21 (I tested some other 'odd' values, with similar results)

    RNG_test using PractRand version 0.93
    RNG = RNG_stdin64, seed = 0x3b9d8777
    test set = normal, folding = standard (64 bit)

    rng=RNG_stdin64, seed=0x3b9d8777
    length= 128 megabytes (2^27 bytes), time= 2.4 seconds
      no anomalies in 148 test result(s)

    rng=RNG_stdin64, seed=0x3b9d8777
    length= 256 megabytes (2^28 bytes), time= 5.6 seconds
      no anomalies in 159 test result(s)

    rng=RNG_stdin64, seed=0x3b9d8777
    length= 512 megabytes (2^29 bytes), time= 11.4 seconds
      no anomalies in 169 test result(s)

    rng=RNG_stdin64, seed=0x3b9d8777
    length= 1 gigabyte (2^30 bytes), time= 23.6 seconds
      Test Name                         Raw       Processed     Evaluation
      BCFN(2+0,13-1,T)                  R=  -6.8  p =1-1.8e-3   unusual
      [Low1/64]BCFN(2+0,13-5,T)         R=  +8.6  p =  1.5e-3   unusual
      ...and 178 test result(s) without anomalies

    rng=RNG_stdin64, seed=0x3b9d8777
    length= 2 gigabytes (2^31 bytes), time= 44.6 seconds
      no anomalies in 191 test result(s)

    rng=RNG_stdin64, seed=0x3b9d8777
    length= 4 gigabytes (2^32 bytes), time= 81.8 seconds
      no anomalies in 201 test result(s)

    rng=RNG_stdin64, seed=0x3b9d8777
    length= 8 gigabytes (2^33 bytes), time= 157 seconds
      no anomalies in 212 test result(s)

    rng=RNG_stdin64, seed=0x3b9d8777
    length= 16 gigabytes (2^34 bytes), time= 305 seconds
      no anomalies in 223 test result(s)

    rng=RNG_stdin64, seed=0x3b9d8777
    length= 32 gigabytes (2^35 bytes), time= 593 seconds
      no anomalies in 233 test result(s)

    rng=RNG_stdin64, seed=0x3b9d8777
    length= 64 gigabytes (2^36 bytes), time= 1192 seconds
      Test Name                         Raw       Processed     Evaluation
      BRank(12):12K(1)                  R= +1272  p~=  5.4e-384   FAIL !!!!!!!
      ...and 243 test result(s) without anomalies


#### Xoroshiro128plus64-v1 + Shift 32

    RNG_test using PractRand version 0.93
    RNG = RNG_stdin64, seed = 0x47700e2c
    test set = normal, folding = standard (64 bit)

    rng=RNG_stdin64, seed=0x47700e2c
    length= 128 megabytes (2^27 bytes), time= 2.3 seconds
      Test Name                         Raw       Processed     Evaluation
      [Low1/64]BCFN(2+0,13-6,T)         R=  -5.7  p =1-1.1e-3   unusual
      ...and 147 test result(s) without anomalies

    rng=RNG_stdin64, seed=0x47700e2c
    length= 256 megabytes (2^28 bytes), time= 5.4 seconds
      no anomalies in 159 test result(s)

    rng=RNG_stdin64, seed=0x47700e2c
    length= 512 megabytes (2^29 bytes), time= 10.6 seconds
      no anomalies in 169 test result(s)

    rng=RNG_stdin64, seed=0x47700e2c
    length= 1 gigabyte (2^30 bytes), time= 20.9 seconds
      no anomalies in 180 test result(s)

    rng=RNG_stdin64, seed=0x47700e2c
    length= 2 gigabytes (2^31 bytes), time= 42.4 seconds
      no anomalies in 191 test result(s)

    rng=RNG_stdin64, seed=0x47700e2c
    length= 4 gigabytes (2^32 bytes), time= 77.5 seconds
      no anomalies in 201 test result(s)

    rng=RNG_stdin64, seed=0x47700e2c
    length= 8 gigabytes (2^33 bytes), time= 149 seconds
      Test Name                         Raw       Processed     Evaluation
      BCFN(2+2,13-0,T)                  R=  +9.4  p =  1.4e-4   mildly suspicious
      Gap-16:B                          R=  +4.4  p =  1.1e-3   unusual
      ...and 210 test result(s) without anomalies

    rng=RNG_stdin64, seed=0x47700e2c
    length= 16 gigabytes (2^34 bytes), time= 291 seconds
      no anomalies in 223 test result(s)

    rng=RNG_stdin64, seed=0x47700e2c
    length= 32 gigabytes (2^35 bytes), time= 577 seconds
      no anomalies in 233 test result(s)

    rng=RNG_stdin64, seed=0x47700e2c
    length= 64 gigabytes (2^36 bytes), time= 1162 seconds
      Test Name                         Raw       Processed     Evaluation
      BRank(12):12K(1)                  R= +1272  p~=  5.4e-384   FAIL !!!!!!!
      ...and 243 test result(s) without anomalies


#### Xoroshiro128plus64-v1 + Shift 48

    RNG_test using PractRand version 0.93
    RNG = RNG_stdin64, seed = 0xd2d51fe2
    test set = normal, folding = standard (64 bit)

    rng=RNG_stdin64, seed=0xd2d51fe2
    length= 128 megabytes (2^27 bytes), time= 2.3 seconds
      no anomalies in 148 test result(s)

    rng=RNG_stdin64, seed=0xd2d51fe2
    length= 256 megabytes (2^28 bytes), time= 5.4 seconds
      no anomalies in 159 test result(s)

    rng=RNG_stdin64, seed=0xd2d51fe2
    length= 512 megabytes (2^29 bytes), time= 10.6 seconds
      no anomalies in 169 test result(s)

    rng=RNG_stdin64, seed=0xd2d51fe2
    length= 1 gigabyte (2^30 bytes), time= 20.4 seconds
      no anomalies in 180 test result(s)

    rng=RNG_stdin64, seed=0xd2d51fe2
    length= 2 gigabytes (2^31 bytes), time= 38.8 seconds
      Test Name                         Raw       Processed     Evaluation
      [Low16/64]FPF-14+6/16:cross       R=  -2.4  p =1-4.0e-4   unusual
      ...and 190 test result(s) without anomalies

    rng=RNG_stdin64, seed=0xd2d51fe2
    length= 4 gigabytes (2^32 bytes), time= 73.8 seconds
      no anomalies in 201 test result(s)

    rng=RNG_stdin64, seed=0xd2d51fe2
    length= 8 gigabytes (2^33 bytes), time= 145 seconds
      no anomalies in 212 test result(s)

    rng=RNG_stdin64, seed=0xd2d51fe2
    length= 16 gigabytes (2^34 bytes), time= 286 seconds
      no anomalies in 223 test result(s)

    rng=RNG_stdin64, seed=0xd2d51fe2
    length= 32 gigabytes (2^35 bytes), time= 564 seconds
      no anomalies in 233 test result(s)

    rng=RNG_stdin64, seed=0xd2d51fe2
    length= 64 gigabytes (2^36 bytes), time= 1186 seconds
      Test Name                         Raw       Processed     Evaluation
      BRank(12):12K(1)                  R= +1272  p~=  5.4e-384   FAIL !!!!!!!
      ...and 243 test result(s) without anomalies
