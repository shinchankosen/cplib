// https://github.com/shinchankosen/cplib/blob/main/src/algorithm/mo_3d.hpp
struct Mo_3D {
    int width;
    vector<int> left, right, index, order;
    vector<bool> v;
    function<void(int)> add, del;

    Mo_3D(int N, int Q) : order(Q), v(N) { width = max<int>(1, pow(N, 2.0 / 3.0)); }
    void insert(int idx, int l, int r) {
        index.push_back(idx);
        left.emplace_back(l);
        right.emplace_back(r);
    }

    void run(const auto& add, const auto& del, const auto& rem, const auto& add_query, const auto& del_query) {
        this->add = add;
        this->del = del;
        order.resize(size(left));
        iota(all(order), 0);
        sort(all(order), [&](int a, int b) -> bool {
            if(left[a] / width != left[b] / width) return left[a] < left[b];
            if(right[a] / width != right[b] / width) return ((right[a] < right[b]) ^ (left[a] / width % 2));
            return bool((index[a] < index[b]) ^ (right[a] / width % 2));
        });
        int time = 0, nl = 0, nr = 0;
        for(auto idx : order) {
            while(time < index[idx]) add_query(time++, this);
            while(time > index[idx]) del_query(--time, this);
            while(nl > left[idx]) distribute(--nl);
            while(nr < right[idx]) distribute(nr++);
            while(nl < left[idx]) distribute(nl++);
            while(nr > right[idx]) distribute(--nr);
            rem(index[idx]);
        }
    }

    void distribute(int idx) {
        if(v[idx]) del(idx);
        else add(idx);
        v[idx] = !v[idx];
    }
};