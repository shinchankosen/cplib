// https://github.com/shinchankosen/cplib/blob/main/src/math/primality.hpp
long long modpow128(long long a, long long n, long long MOD) {
    __int128_t res = 1, mul = a % MOD;
    while(n > 0) {
        if(n & 1) res = res * mul % MOD;
        mul = mul * mul % MOD;
        n >>= 1;
    }
    return (long long)res;
}

bool is_prime(ll N) {
    if(N == 2) return true;
    if(N == 1 || N % 2 == 0) return false;
    ll s = 0;
    ll d = N - 1;
    while(d % 2 == 0) {
        s++;
        d /= 2;
    }
    vector<ll> tests = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for(auto a : tests) {
        if(a == N) continue;
        ll X = modpow128(a, d, N);
        int r = 0;
        if(X == 1) { continue; }
        while(X != N - 1) {
            X = modpow128(X, 2, N);
            r++;
            if(X == 1 || r == s) return false;
        }
    }
    return true;
}