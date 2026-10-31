// https://github.com/shinchankosen/cplib/blob/main/src/data_structure/potential_dsu.hpp
template<typename Abel> struct potential_dsu {
    using T = typename Abel::T;
    int tCount;
    vector<int> p, rank;
    vector<T> potential;
    int N;
    potential_dsu(int size) {
        N = size;
        p.resize(N, -1);
        rank.resize(N, 0);
        potential.resize(N, Abel::e());
        tCount = N;
    }

    bool same(int x, int y) { return leader(x) == leader(y); }

    // w[y] - w[x] = w
    void merge(int x, int y, T w) {
        w = Abel::op(w, get_potential(x));
        w = Abel::op(w, Abel::inv(get_potential(y)));
        link(leader(x), leader(y), w);
    }

    int leader(int x) {
        if(p[x] < 0) return x;
        int l = leader(p[x]);
        potential[x] = Abel::op(potential[x], potential[p[x]]);
        return p[x] = l;
    }

    T get_potential(int x) {
        leader(x);
        return potential[x];
    }

    // w[y] - w[x]
    T diff(int x, int y) { return Abel::op(get_potential(y), Abel::inv(get_potential(x))); }

    int count() { return tCount; }

    int size(int a) {
        // assert(0 <= a && a < _n);
        return -p[leader(a)];
    }

    vector<vector<int>> groups() {
        vector<int> leader_buf(N), group_size(N);
        for(int i = 0; i < N; i++) {
            leader_buf[i] = leader(i);
            group_size[leader_buf[i]]++;
        }
        vector<vector<int>> result(N);
        for(int i = 0; i < N; i++) result[i].reserve(group_size[i]);
        for(int i = 0; i < N; i++) result[leader_buf[i]].push_back(i);
        result.erase(remove_if(result.begin(), result.end(), [&](const vector<int>& v) { return v.empty(); }),
                         result.end());
        return result;
    }

    private:
    void link(int x, int y, T w) {
        if(x == y) return;
        tCount--;
        if(rank[x] < rank[y]) {
            swap(x, y);
            w = Abel::inv(w);
        }
        p[x] += p[y];
        p[y] = x;
        if(rank[x] == rank[y]) rank[x]++;
        tCount--;
        potential[y] = w;
    }
};

/*
struct Abel {
    using T = int;
    static T e() { return 0; }
    static T op(T a, T b) { return a + b; }
    static T inv(T a) { return -a; }
};

potential_dsu<Abel> dsu(N);
*/