
# xoroshiro2

Improved xoroshiro generator, better at **zero cost** (Intel Ci3-5005U (Broadwell) CPU), compiler [`LLVM-7.0.0-r336178-win64`](https://llvm.org/builds/).

Included a modified xoroshiro implementation by Melissa E. O'Neill, the author of the excelent [pcg](http://www.pcg-random.org/) familiy of prng's. Don't miss her [blog](http://www.pcg-random.org/blog/), which is a gold-mine of information (and code) and is updated regulary.

Iff results can not be confirmed, this repo will be removed.


## Results

#### Speed-test

##### Context

* Intel Ci3-5005U (Broadwell) CPU
* Windows 10-1803-x64 in `diagnostics mode` (core services only), and Windows Defender off
* Compiler: [`LLVM-7.0.0-r336178-win64`](https://llvm.org/builds/)
* Command-line: `clang-cl -fuse-ld=lld -flto=thin  /D "NDEBUG" /D "_CONSOLE" /D "NOMINMAX" /D "_UNICODE" /D "UNICODE" -Xclang -fcxx-exceptions /Ox /Oi /MT main.cpp statistics.cpp -Xclang -std=c++2a -Xclang -ffast-math -mmmx  -msse  -msse2 -msse3 -mssse3 -msse4.1 -msse4.2 -mavx -mavx2`
* Generator tested: the improved 2018  `xoroshiro128plus64 v1`
* 100'000'000 numbers generated, 512 runs


##### Results

     **generator**            **fastest (ms)**   **mean (ms)**

    * meo-v1                        300           314
    * shift 48                      300           316
    * shift 32                      300           317
    * shift 16                      300           313 