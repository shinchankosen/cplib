// https://github.com/shinchankosen/cplib/blob/main/src/data_structure/disjoint_sparse_table.hpp
template<typename SG> struct disjoint_sparse_table {
    using S = typename SG::S;
    vector<vector<S>> st;
    vector<int> lg;

    disjoint_sparse_table(const vector<S>& v) {
        int b = 0;
        while((1 << b) <= size(v)) b++;
        st.assign(b, vector<S>(size(v)));
        for(int i = 0; i < size(v); i++) st[0][i] = v[i];
        for(int i = 1; i < b; i++) {
            int shift = 1 << i;
            for(int j = 0; j < size(v); j += shift << 1) {
                int t = min(j + shift, (int)size(v));
                st[i][t - 1] = v[t - 1];
                for(int k = t - 2; k >= j; k--) st[i][k] = SG::op(v[k], st[i][k + 1]);
                if(size(v) <= t) break;
                st[i][t] = v[t];
                for(int k = t + 1; k < min((int)size(v), t + shift); k++) st[i][k] = SG::op(st[i][k - 1], v[k]);
            }
        }
        lg.resize(1 << b);
        for(int i = 2; i < size(lg); i++) lg[i] = lg[i >> 1] + 1;
    }
    S prod(int l, int r) {
        if(l >= --r) return st[0][l];
        int b = lg[l ^ r];
        return SG::op(st[b][l], st[b][r]);
    }
};