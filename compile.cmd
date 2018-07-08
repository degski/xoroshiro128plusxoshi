@ clang-cl -fuse-ld=lld -flto=thin  /D "NDEBUG" /D "_CONSOLE" /D "NOMINMAX" /D "_UNICODE" /D "UNICODE" -Xclang -fcxx-exceptions /Ox /Oi /MT speedtest.cpp statistics.cpp -Xclang -std=c++2a -Xclang -ffast-math -mmmx  -msse  -msse2 -msse3 -mssse3 -msse4.1 -msse4.2 -mavx -mavx2


