// https://github.com/shinchankosen/cplib/blob/main/src/data_structure/treap.hpp
// mmを使う場合, 追記が必要
// friend bool operator==(const mm& a, const mm& b) { return a.x == b.x; }
template<class S, S (*op)(S, S), S (*e)(), class F, S (*mapping)(F, S, int), F (*composition)(F, F), F (*id)()>
struct Treap {
    private:
    mt19937_64 mt;
    uniform_int_distribution<uint64_t> rand;
    vector<S> value, acc;
    vector<F> lazy;
    vector<ll> priority;
    vector<int> cnt, lch, rch;
    vector<bool> lazy_rev;
    int new_node(S v, ll p) {
        value.push_back(v);
        acc.push_back(e());
        lazy.push_back(id());
        priority.push_back(p);
        cnt.push_back(0);
        lazy_rev.push_back(false);
        lch.push_back(-1);
        rch.push_back(-1);
        return value.size() - 1;
    }

    int root = -1;
    int get_cnt(int t) { return t == -1 ? 0 : cnt[t]; }
    S get_acc(int t) { return t == -1 ? e() : acc[t]; }
    int update(int t) {
        if(t == -1) return t;
        cnt[t] = 1 + get_cnt(lch[t]) + get_cnt(rch[t]);
        acc[t] = op(get_acc(lch[t]), op(value[t], get_acc(rch[t])));
        return t;
    }
    int push(int t) {
        if(t == -1) return t;
        if(lazy_rev[t]) {
            lazy_rev[t] = false;
            swap(lch[t], rch[t]);
            if(lch[t] != -1) lazy_rev[lch[t]] = !lazy_rev[lch[t]];
            if(rch[t] != -1) lazy_rev[rch[t]] = !lazy_rev[rch[t]];
        }
        if(lazy[t] != id()) {
            if(lch[t] != -1) {
                lazy[lch[t]] = composition(lazy[t], lazy[lch[t]]);
                acc[lch[t]] = mapping(lazy[t], acc[lch[t]], get_cnt(lch[t]));
            }
            if(rch[t] != -1) {
                lazy[rch[t]] = composition(lazy[t], lazy[rch[t]]);
                acc[rch[t]] = mapping(lazy[t], acc[rch[t]], get_cnt(rch[t]));
            }
            value[t] = mapping(lazy[t], value[t], 1);
            lazy[t] = id();
        }
        return update(t);
    }

    int merge(int l, int r) {
        push(l);
        push(r);
        if(l == -1) return r;
        if(r == -1) return l;
        if(priority[l] > priority[r]) {
            rch[l] = merge(rch[l], r);
            return update(l);
        } else {
            lch[r] = merge(l, lch[r]);
            return update(r);
        }
    }
    pair<int, int> split(int t, int k) {
        if(t == -1) return make_pair(-1, -1);
        push(t);
        int implicit_key = get_cnt(lch[t]) + 1;
        if(k < implicit_key) {
            auto s = split(lch[t], k);
            lch[t] = s.second;
            return make_pair(s.first, update(t));
        } else {
            auto s = split(rch[t], k - implicit_key);
            rch[t] = s.first;
            return make_pair(update(t), s.second);
        }
    }

    int insert(int t, int k, int n) {
        auto s = split(t, k);
        return merge(merge(s.first, n), s.second);
    }

    int apply(int t, int l, int r, F f) {
        auto [t1, tt] = split(t, l);
        auto [t2, t3] = split(tt, r - l);
        lazy[t2] = composition(f, lazy[t2]);
        acc[t2] = mapping(f, acc[t2], get_cnt(t2));
        return merge(merge(t1, t2), t3);
    }  // 905a19 (Unordered)

    int _erase(int t, int k) {
        auto [tt, t3] = split(t, k + 1);
        auto [t1, t2] = split(tt, k);
        return merge(t1, t3);
    }  // 92ef20 (Common)

    int erase_range(int t, int l, int r) {
        auto [tt, t3] = split(t, r);
        auto [t1, t2] = split(tt, l);
        return merge(t1, t3);
    }  // 77074b (Common)

    pair<S, int> query(int t, int l, int r) {
        auto [t1, tt] = split(t, l);
        auto [t2, t3] = split(tt, r - l);
        S ret = acc[t2];
        return make_pair(ret, merge(merge(t1, t2), t3));
    }  //  fe8e6c (Common)

    int set(int t, int k, S v) {
        auto [tt, t3] = split(t, k + 1);
        auto [t1, t2] = split(tt, k);
        push(t2);
        value[t2] = v;
        update(t2);
        return merge(merge(t1, t2), t3);
    }  // 31b211 (Unordered)

    int _find(int t, S x, int offset, bool left = true) {
        if(op(get_acc(t), x) == x) {
            return -1;
        } else {
            if(left) {
                if(lch[t] != -1 && op(acc[lch[t]], x) != x) {
                    return find(lch[t], x, offset, left);
                } else {
                    return (op(value[t], x) != x) ? offset + get_cnt(lch[t])
                                                            : find(rch[t], x, offset + get_cnt(lch[t]) + 1, left);
                }
            } else {
                if(rch[t] != -1 && op(acc[rch[t]], x) != x) {
                    return find(rch[t], x, offset + get_cnt(lch[t]) + 1, left);
                } else {
                    return (op(value[t], x) != x) ? offset + get_cnt(lch[t]) : find(lch[t], x, offset, left);
                }
            }
        }
    }  // b0c65b (Common)

    int reverse(int t, int l, int r) {
        auto [t1, tt] = split(t, l);
        auto [t2, t3] = split(tt, r - l);
        lazy_rev[t2] = !lazy_rev[t2];
        return merge(merge(t1, t2), t3);
    }  // 3f67e3 (Unordered)

    int rotate(int t, int l, int m, int r) {
        t = reverse(t, l, r);
        t = reverse(t, l, l + r - m);
        return reverse(t, l + r - m, r);
    }  // a5a67c (Unordered)

    int lower_search(int t, S x) {
        int ret = 0;
        while(t != -1) {
            if(x <= value[t]) {
                t = lch[t];
            } else {
                ret += get_cnt(lch[t]) + 1;
                t = rch[t];
            }
        }
        return ret;
    }  // 0ef7d9 (Ordered)

    int upper_search(int t, S x) {
        int ret = 0;
        while(t != -1) {
            if(x < value[t]) {
                t = lch[t];
            } else {
                ret += get_cnt(lch[t]) + 1;
                t = rch[t];
            }
        }
        return ret;
    }  // f91898 (Ordered)

    public:
    Treap() : Treap(0) {}
    Treap(int N) : Treap(vector<S>(N, e())) {}
    Treap(vector<S> V) {
        mt = mt19937_64(chrono::steady_clock::now().time_since_epoch().count());
        rand = uniform_int_distribution<uint64_t>(1, 1e18);
        for(auto v : V) { push_back(v); }
    }

    size_t size() { return size_t(get_cnt(root)); }  // f63788 (Common)

    void insert(int ind, S x) { root = insert(root, ind, new_node(x, rand(mt))); }  // dc467c (UnOrdered)

    void push_back(S x) { root = insert(root, int(size()), new_node(x, rand(mt))); }  // 7fa616 (Unordered)

    void ordered_insert(S x) {
        int ind = lower_search(root, x);
        insert(ind, x);
    }  // 539d77 (Ordered)

    // Count elements in [lower, upper)
    int value_range_cnt(S lower, S upper) {
        int L = lower_search(root, lower);
        int R = lower_search(root, upper);
        return R - L;
    }  // 2d4406 (Ordered)

    // Sum of elements in [lower, upper)
    S value_range_prod(S lower, S upper) {
        int L = lower_search(root, lower);
        int R = lower_search(root, upper);
        if(L == R) return e();
        return query(L, R);
    }  // 27b9d4 (Ordered)

    // erase element x cnt times (cnt = -1 -> erase all x)
    int erase_value(S x, int cnt = -1) {
        int L = lower_search(root, x);
        int R = upper_search(root, x);
        if(cnt != -1) chmin(R, L + cnt);
        root = erase_range(root, L, R);
        return R - L;
    }  // 5c60fd (Ordered)

    int lower_search(S x) { return lower_search(root, x); }  // 9731cc (Ordered)

    int upper_search(S x) { return upper_search(root, x); }  // ac5aa0 (Ordered)

    void apply(int l, int r, F f) { root = apply(root, l, r, f); }  // 905a19 (Unordered)

    void erase(int ind) { root = _erase(root, ind); }
    // ff257f (Common)

    void erase(int l, int r) {
        auto [tt, t3] = split(root, r);
        auto [t1, t2] = split(tt, l);
        root = merge(t1, t3);
    }  // f9ff4a (Common)

    // l .. r-1 -> r-1 .. l
    void reverse(int l, int r) { root = reverse(root, l, r); }  // 40df7d (Unordered)

    // l .. m-1, m .. r-1 -> m .. r-1, l .. m-1
    void rotate(int l, int m, int r) { root = rotate(root, l, m, r); }  // e21b85 (Unordered)

    void set(int k, S v) { root = set(root, k, v); }  // 4ae943 (Unordered)

    // min k \in [l,r) such that op(tr[k], x) != x
    int find(int l, int r, S x, bool left = true) {
        auto [t1, tt] = split(root, l);
        auto [t2, t3] = split(tt, r - l);
        int ret = _find(t2, x, l, left);
        if(ret == -1) ret = r;
        root = merge(merge(t1, t2), t3);
        return ret;
    }  // 4f1699 (Common)

    S prod(int l, int r) {
        if(l == r) return S(0);
        auto [t, rt] = query(root, l, r);
        root = rt;
        return t;
    }  // c46ac4 (Common)

    S operator[](int ind) {
        auto [tt, t3] = split(root, ind + 1);
        auto [t1, t2] = split(tt, ind);
        S ret = acc[t2];
        root = merge(merge(t1, t2), t3);
        return ret;
    }  // d2546e (Common)
};