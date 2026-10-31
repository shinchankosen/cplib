// https://github.com/shinchankosen/cplib/blob/main/src/algorithm/min_plus_convolution.hpp
template<typename T, typename F> vector<int> monotone_minima(int h, int w, const F& f) {
    vector<pair<int, T>> dp(h, pair(-1, T()));
    auto rec = [&](auto&& rec, int u, int d, int l, int r) {
        if(u > d) return;
        int mid = (u + d) >> 1;
        auto& [idx, mi] = dp[mid];
        idx = l, mi = f(mid, l);
        for(int i = l + 1; i <= r; i++)
            if(chmin(mi, f(mid, i))) idx = i;
        rec(rec, u, mid - 1, l, idx);
        rec(rec, mid + 1, d, idx, r);
    };
    rec(rec, 0, h - 1, 0, w - 1);
    vector<int> ret;
    for(auto [idx, val] : dp) ret.push_back(idx);
    return ret;
}

template<typename T>  // B is convex. if both A and B are convex, do greedy.
vector<T> minplus_conv(vector<T>& A, vector<T>& B) {
    int n = A.size(), m = B.size();
    if(n == 0 && m == 0) return {};
    vector<T> C(n + m - 1);

    const ll inf = INF;
    auto select = [&](int i, int j) -> T {
        if(i < j) return inf;
        if(i - j >= m) return inf;
        return A[j] + B[i - j];
    };
    vector<int> J = monotone_minima<T>(n + m - 1, n, select);
    for(int i = 0; i < n + m - 1; i++) {
        T x = A[J[i]], y = B[i - J[i]];
        if(x < inf && y < inf) C[i] = x + y;
    }
    return C;
}
