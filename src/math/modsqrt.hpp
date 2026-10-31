// https://github.com/shinchankosen/cplib/blob/main/src/math/modsqrt.hpp
// depends on src/math/modpow_modinv.hpp
int Legendre(long long a, long long p) {
    long long ret = modpow(a, (p - 1) / 2, p);

    if(ret == p - 1) ret = -1;

    return ret;
}

long long modsqrt(long long n, long long p) {
    long long Q = p - 1;
    long long S = 0;
    while(Q % 2 == 0) {
        ++S;
        Q /= 2;
    }
    if(S == 1) { return modpow(n, (p + 1) / 4, p); }
    default_random_engine generator;
    uniform_int_distribution<long long> distribution(0, p);
    long long z = 1;
    while(Legendre(z, p) != -1) { z = distribution(generator); }
    long long c = modpow(z, Q, p);
    long long R = modpow(n, (Q + 1) / 2, p);
    long long t = modpow(n, Q, p);
    long long M = S;
    while(t != 1) {
        long long i;
        long long t2 = t;
        for(i = 1; i < M; i++) {
            t2 = t2 * t2 % p;
            if(t2 == 1) break;
        }
        long long b = modpow(c, 1LL << (M - i - 1), p);
        R = R * b % p;
        t = (t * b % p) * b % p;
        c = b * b % p;
        M = i;
    }
    return R;
}