// https://github.com/shinchankosen/cplib/blob/main/src/math/modpow128.hpp
// depends on src/math/modpow_modinv.hpp
// MOD が 2^32 以上でも使える modpow
long long modpow128(long long a, long long n, long long MOD) {
    __int128_t res = 1, mul = a % MOD;
    if(n < 0) {
        n = -n;
        mul = modinv(mul, MOD);
    }
    while(n > 0) {
        if(n & 1) res = res * mul % MOD;
        mul = mul * mul % MOD;
        n >>= 1;
    }
    return (long long)res;
}
