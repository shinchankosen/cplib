// https://github.com/shinchankosen/cplib/blob/main/src/data_structure/li_chao_tree.hpp
template<typename T> class LiChaoTree {
    int n, sz, height;
    vector<T> xs, as, bs;
    void update(T a, T b, int k, int h) {
        int l = (k << h) & (sz - 1);
        int r = l + (1 << h);
        while(1) {
            int m = (l + r) >> 1;
            T xl = xs[l], xm = xs[m];
            bool l_update = a * xl + b < as[k] * xl + bs[k];
            bool m_update = a * xm + b < as[k] * xm + bs[k];
            if(m_update) {
                swap(as[k], a);
                swap(bs[k], b);
            }
            if(h == 0) break;
            if(l_update != m_update) {
                k = k * 2;
                r = m;
            } else {
                k = k * 2 + 1;
                l = m;
            }
            h--;
        }
    }
    public:
    // 最小値クエリのx座標や線分追加クエリの両端のx座標を先読みしてソートした配列
    LiChaoTree(const vector<T>& xs) : n(xs.size()), xs(xs) {
        sz = 1, height = 0;
        while(sz < (int)xs.size()) {
            sz <<= 1;
            height++;
        }
        this->xs.resize(sz, xs.back());
        as.assign(sz * 2, 0);
        bs.assign(sz * 2, std::numeric_limits<T>::max());
    }
    void add_line(T a, T b) { update(a, b, 1, height); }
    void add_segment(T a, T b, int l, int r) {
        if(l == r) return;
        l += sz, r += sz;
        int h = 0;
        while(l < r) {
            if(l & 1) update(a, b, l++, h);
            if(r & 1) update(a, b, --r, h);
            l >>= 1, r >>= 1, h++;
        }
    }
    // x = xs[i] における最小値を求める
    T get(int i) const {
        T x = xs[i];
        int k = i + sz;
        T res = as[k] * x + bs[k];
        while(k > 1) {
            k >>= 1;
            T tmp = as[k] * x + bs[k];
            if(tmp < res) res = tmp;
        }
        return res;
    }
};