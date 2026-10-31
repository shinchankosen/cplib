// https://github.com/shinchankosen/cplib/blob/main/src/algorithm/larsch.hpp
// depends on src/algorithm/min_plus_convolution.hpp
template<typename T> vector<T> monge_shortest_path(int N, const function<T(int, int)>& f) {
    T INF = (T{1} << (sizeof(T) * 8 - 2)) - 1;
    vector<T> dp(N + 1, INF);
    vector<int> x(N + 1, 0);
    auto check = [&](int from, int to) {
        if(from >= to) return;
        T cost = f(from, to);
        if(dp[from] + cost < dp[to]) dp[to] = dp[from] + cost, x[to] = from;
    };
    auto dfs = [&](auto rc, int l, int r) -> void {
        if(l + 1 >= r) return;
        int m = (l + r) / 2;
        for(int i = x[l]; i <= x[r]; i++) check(i, m);
        rc(rc, l, m);
        for(int i = l + 1; i <= m; i++) check(i, r);
        rc(rc, m, r);
    };
    dp[0] = 0, check(0, N), dfs(dfs, 0, N);
    return dp;
}

// [min, max] は閉区間を入力する
template<typename T, bool get_min = true> pair<ll, T> golden_section_search(const function<T(ll)>& f, ll min, ll max) {
    assert(min <= max);
    ll a = min - 1, x, b;
    {
        ll s = 1, t = 2;
        while(t < max - min + 2) swap(s += t, t);
        x = a + t - s, b = a + t;
    }
    T fx = f(x), fy;
    while(a + b != 2 * x) {
        ll y = a + b - x;
        if(max < y || (fy = f(y), get_min ? fx < fy : fx > fy)) {
            b = a;
            a = y;
        } else {
            a = x;
            x = y;
            fx = fy;
        }
    }
    return {x, fx};
}

// upper : max abs(辺数を 1 増減させたときのコストの変化)
ll monge_d_edge_shortest_path(int N, int D, ll upper, const function<ll(int, int)>& f) {
    using T = __int128_t;
    upper = abs(upper);
    auto dp = [&](ll x) -> T {
        auto g = [&](int from, int to) -> T { return f(from, to) + x; };
        T cost = monge_shortest_path<T>(N, g)[N];
        return cost - T{1} * D * x;
    };
    auto [_, res] = golden_section_search<T, false>(dp, -upper, upper);
    return res;
}

vector<ll> enumerate_monge_d(int N, const function<ll(int, int)>& f, ll unreached = (1LL << 62) - 1) {
    using T = __int128_t;
    T INF = (T{1} << (sizeof(T) * 8 - 2)) - 1;
    vector<ll> ans(N + 1, unreached);
    vector<T> dp(N + 1, INF);
    dp[0] = 0;
    for(int d = 1; d <= N; d++) {
        vector<int> midx =
                monotone_minima<T>(N + 1, N + 1, [&](int j, int i) -> T { return i < j ? dp[i] + f(i, j) : INF; });
        for(int i = N; i >= d; i--) dp[i] = dp[midx[i]] + f(midx[i], i);
        ans[d] = dp[N];
    }
    return ans;
}
