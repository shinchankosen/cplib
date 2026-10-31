// https://github.com/shinchankosen/cplib/blob/main/src/fps/relaxed_convolution.hpp
template<typename T> class RelaxedConvolution {
    int N, pos;
    vector<T> f, g, buf;
    vector<vector<tuple<int, int, int, int>>> event;
    void dfs1(int le, int ri) {
        if(ri - le == 1) {
            event[le].push_back({le, le + 1, 0, 1});
            return;
        }
        int mid = (le + ri) / 2;
        event[mid].push_back({le, mid, mid - le, ri - le});
        event[ri].push_back({mid, ri, mid - le, ri - le});
        dfs1(le, mid);
        dfs1(mid, ri);
    }
    void dfs2(int le, int ri) {
        if(ri - le == 1) {
            event[le].push_back({0, 1, le, le + 1});
            return;
        }
        int mid = (le + ri) / 2;
        event[mid].push_back({mid - le, ri - le, le, mid});
        event[ri].push_back({mid - le, ri - le, mid, ri});
        dfs2(le, mid);
        dfs2(mid, ri);
    }
    void dfs(int len) {
        if(len == 1) {
            event[0].push_back({0, 1, 0, 1});
            return;
        }
        int mid = len / 2;
        event[len].push_back({mid, len, mid, len});
        dfs(mid);
        dfs1(mid, len);
        dfs2(mid, len);
    }

    public:
    RelaxedConvolution(int n) {
        N = 1, pos = 0;
        while(N < n) N *= 2;
        f.resize(N);
        g.resize(N);
        buf.resize(N);
        event.resize(N + 1);
        dfs(N);
    }
    T get(T x, T y) {
        f[pos] = x, g[pos] = y;
        for(auto [fl, fr, gl, gr] : event[pos]) {
            vector<T> A({f.begin() + fl, f.begin() + fr});
            vector<T> B({g.begin() + gl, g.begin() + gr});
            auto ret = conv(A, B);
            int sz = ret.size();
            for(int i = 0; i < sz; i++) {
                if(i + fl + gl >= N) break;
                buf[i + fl + gl] += ret[i];
            }
        }
        return buf[pos++];
    }
};