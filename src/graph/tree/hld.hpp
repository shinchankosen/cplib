// https://github.com/shinchankosen/cplib/blob/main/src/graph/tree/hld.hpp
class HLDcomposition {
    private:
    int V;
    vector<vector<int>> G;
    vector<int> stsize, parent, pathtop, in, out;
    int root;
  
    void build_stsize(int u, int p) {
        stsize[u] = 1, parent[u] = p;
        for(auto&& v : G[u]) {
                if(v == p) {
                        swap(v, G[u].back());
                        break;
                }
        }
        for(auto&& v : G[u]) {
            if(v == p) {
                continue;
            }
            build_stsize(v, u);
            stsize[u] += stsize[v];
            if(stsize[v] > stsize[G[u][0]]) swap(v, G[u][0]);
        }
    }  // 5a09a1

    void build_path(int u, int p, int& tm) {
        in[u] = tm++;
        for(auto v : G[u]) {
            if(v == p) continue;
            pathtop[v] = (v == G[u][0] ? pathtop[u] : v);
            build_path(v, u, tm);
        }
        out[u] = tm;
    }  // 9074ec

    public:
    void add_edge(int u, int v) {
        G[u].push_back(v);
        G[v].push_back(u);
    }

    void build(int _root = 0) {
        root = _root;
        int tm = 0;
        build_stsize(root, -1);
        pathtop[root] = root;
        build_path(root, -1, tm);
    }  // d594c1

    inline int index(int a) { return in[a]; }

    int lca(int a, int b) {
        int pa = pathtop[a], pb = pathtop[b];
        while(pa != pb) {
            if(in[pa] > in[pb]) {
                a = parent[pa], pa = pathtop[a];
            } else {
                b = parent[pb], pb = pathtop[b];
            }
        }
        if(in[a] > in[b]) swap(a, b);
        return a;
    }  // 2e2c69

    pair<int, int> subtree_query(int a) { return {in[a], out[a]}; }

    vector<tuple<int, int, bool>> path_query(int from, int to) {
        int pf = pathtop[from], pt = pathtop[to];
        using T = tuple<int, int, bool>;
        deque<T> front, back;
        while(pf != pt) {
            if(in[pf] > in[pt]) {
                front.push_back({in[pf], in[from] + 1, true});
                from = parent[pf], pf = pathtop[from];
            } else {
                back.push_front({in[pt], in[to] + 1, false});
                to = parent[pt], pt = pathtop[to];
            }
        }
        if(in[from] > in[to]) front.push_back({in[to], in[from] + 1, true});
        else front.push_back({in[from], in[to] + 1, false});
        vector<T> ret;
        while(!front.empty()) {
            ret.push_back(front.front());
            front.pop_front();
        }
        while(!back.empty()) {
            ret.push_back(back.front());
            back.pop_front();
        }
        return ret;
    }  // 47b288

    HLDcomposition(int node_size)
            : V(node_size), G(V), stsize(V, 0), parent(V, -1), pathtop(V, -1), in(V, -1), out(V, -1) {}
};

// 使用例: パス・部分木への加算、更新、総和 (src/data_structure/lazy_segtree.hpp と組み合わせる)
// using S = pair<ll, ll>;
//
// S op(S a, S b) {
//     return S{a.first + b.first, a.second + b.second};
// }
//
// S e() {return S{0, 0};}
//
// S mapping(S a, S b) {
//     if(a.first == 1) {
//         return S{a.second * b.second, b.second};
//     }
//     return S{b.first + a.second * b.second, b.second};
// }
//
// S composition(S a, S b) {
//     if(a.first == 1) return a;
//     return S{b.first, b.second + a.second};
// }
//
// void solve() {
//     ll n, q; cin >> n >> q;
//     vector<ll> a(n);
//     rep(i, n) cin >> a[i];
//     HLDcomposition hld(n);
//     rep(i, n - 1) {
//         int x, y; cin >> x >> y;
//         x --; y --;
//         hld.add_edge(x, y);
//     }
//     hld.build();
//     lazy_segtree<S, op, e, S, mapping, composition, e> seg(n);
//     rep(i, n) seg.set(hld.index(i), S{a[i], 1});
//     rep(_, q) {
//         int t; cin >> t;
//         if(t == 1) {
//             ll x, y, w; cin >> x >> y >> w;
//             x --; y --;
//             for(auto [l, r, f] : hld.path_query(x, y)) seg.apply(l, r, S{0, w});
//         } else if(t == 2) {
//             ll x, y, w; cin >> x >> y >> w;
//             x --; y --;
//             for(auto [l, r, f] : hld.path_query(x, y)) seg.apply(l, r, S{1, w});
//         } else if(t == 3) {
//             ll x, w; cin >> x >> w;
//             x --;
//             auto [l, r] = hld.subtree_query(x);
//             seg.apply(l, r, S{0, w});
//         } else if(t == 4) {
//             ll x, w; cin >> x >> w;
//             x --;
//             auto [l, r] = hld.subtree_query(x);
//             seg.apply(l, r, S{1, w});
//         } else if(t == 5) {
//             ll ans = 0;
//             ll x, y; cin >> x >> y;
//             x --; y --;
//             for(auto [l, r, f] : hld.path_query(x, y)) ans += seg.prod(l, r).first;
//             cout << ans << endl;
//         } else {
//             ll ans = 0;
//             ll x; cin >> x;
//             x --;
//             auto [l, r] = hld.subtree_query(x);
//             ans += seg.prod(l, r).first;
//             cout << ans << endl;
//         }
//     }
// }
