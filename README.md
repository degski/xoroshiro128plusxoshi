
# xoroshiro2

Improved xoroshiro generator, better at **zero cost** (Intel Ci3-5005U (Broadwell) CPU), compiler [`LLVM-7.0.0-r336178-win64`](https://llvm.org/builds/).

Included a modified xoroshiro implementation by Melissa E. O'Neill, the author of the excelent [pcg](http://www.pcg-random.org/) familiy of prng's. Don't miss her [blog](http://www.pcg-random.org/blog/), which is a gold-mine of information (and code) and is updated regulary.


## Results

### Speed-test

#### Context

* Intel Ci3-5005U (Broadwell) CPU
* Windows 10-1803-x64 in `diagnostics mode` (core services only), and Windows Defender off
* Compiler: [`LLVM-7.0.0-r336178-win64`](https://llvm.org/builds/)
* Command-line: `clang-cl -fuse-ld=lld -flto=thin  /D "NDEBUG" /D "_CONSOLE" /D "NOMINMAX" /D "_UNICODE" /D "UNICODE" -Xclang -fcxx-exceptions /Ox /Oi /MT main.cpp statistics.cpp -Xclang -std=c++2a -Xclang -ffast-math -mmmx  -msse  -msse2 -msse3 -mssse3 -msse4.1 -msse4.2 -mavx -mavx2`
* Generator tested: the improved 2018  `xoroshiro128plus64 v1`
* 100'000'000 numbers generated, 512 runs

#### Results

     generator                    fastest (ms)    mean (ms)

    * meo-v1                        300            314
    * shift 48                      300            316
    * shift 32                      300            317
    * shift 16                      300            313


### Practrand results

#### Shift 16

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


#### Shift 32

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


#### Shift 48
