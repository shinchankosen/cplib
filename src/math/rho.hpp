// https://github.com/shinchankosen/cplib/blob/main/src/math/rho.hpp
// depends on src/math/modpow128.hpp, src/math/primality.hpp
ll find_prime_factor(ll N) {
    using i128 = __int128_t;
    if(N % 2 == 0) { return 2; }
    int b = int(sqrt(sqrt(sqrt(N)))) + 1;
    for(ll c = 1; c < N; c++) {
        auto f = [&](ll a) -> ll { return modpow128(a, 2, N) + c; };
        ll y = 6;
        ll g = 1;
        i128 q = 1;
        int r = 1;
        int k = 0;
        ll ys = 0;
        ll x = 0;
        while(g == 1) {
            x = y;
            while(k < 3 * r / 4) {
                y = f(y);
                k++;
            }
            while(k < r && g == 1) {
                ys = y;
                for(ll i = 0; i < min(b, r - k); i++) {
                    y = f(y);
                    q *= abs(x - y);
                    q %= N;
                }
                g = gcd(ll(q), N);
                k += b;
            }
            k = r;
            r *= 2;
        }
        if(g == N) {
            g = 1;
            y = ys;
            while(g == 1) {
                y = f(y);
                g = gcd(abs(x - y), N);
            }
        }
        if(g == N) { continue; }
        if(is_prime(g)) { return g; }
        if(is_prime(N / g)) { return N / g; }
        return find_prime_factor(g);
    }
    assert(false);
}

map<ll, int> factorize(ll N) {
    map<ll, int> ret;
    while(!is_prime(N) && N > 1) {
        ll p = find_prime_factor(N);
        int s = 0;
        while(N % p == 0) {
            N /= p;
            s++;
        }
        ret[p] = s;
    }
    if(N > 1) { ret[N] = 1; }
    return ret;
}