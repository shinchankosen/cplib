// https://github.com/shinchankosen/cplib/blob/main/src/data_structure/tree_64.hpp
struct WordsizeTree {
    using Word = unsigned long long;
    using vw = vector<Word>;
    static constexpr int W = 64;
    int N;
    vector<vw> A;
    WordsizeTree(int length) {
        N = length;
        int n = length;
        do {
            vw a(n / W + 1, 0);
            A.emplace_back(move(a));
            n /= W;
        } while(n);
    }
    void insert(int x) {
        for(auto& a : A) {
            a[x / W] |= (Word)1 << (x % W);
            x /= W;
        }
    }
    void erase(int x) {
        for(auto& a : A) {
            a[x / W] &= ~((Word)1 << (x % W));
            if(a[x / W]) return;
            x /= W;
        }
    }
    int count(int x) const { return (int)((A[0][x / W] >> (x % W)) & 1); }
    int noLessThan(int x) const {
        if(x < 0) x = 0;
        if(N <= x) return N;
        int d = 0, i = x;
        while(true) {
            if(d >= (int)A.size()) return N;
            if(i / W >= (int)A[d].size()) return N;
            Word m = A[d][i / W] & ((~(Word)0) << (i % W));
            if(!m) {
                d++;
                i /= W;
                i++;
            } else {
                int to = (m == 0 ? W : __builtin_ctzll(m));
                i = i / W * W + to;
                if(d == 0) break;
                i *= W;
                d--;
            }
        }
        return i;
    }
    int noGreaterThan(int x) const {
        if(x < 0) return -1;
        if(N <= x) x = N - 1;
        int d = 0, i = x;
        while(true) {
            if(i < 0) return -1;
            if(d >= (int)A.size()) return -1;
            Word m = A[d][i / W] & ~((~(Word)1) << (i % W));
            if(!m) {
                d++;
                i /= W;
                i--;
            } else {
                int to = (m == 0 ? 0 : W - 1 - __builtin_clzll(m));
                i = i / W * W + to;
                if(d == 0) break;
                i *= W;
                i += W - 1;
                d--;
            }
        }
        return i;
    }
};   // thanks Nachia
