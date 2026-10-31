// https://github.com/shinchankosen/cplib/blob/main/src/data_structure/wavelet_matrix.hpp
struct BitVector {
    private:
    vector<int> vec;

    public:
    BitVector(const vector<int>& a) {
        vec.resize(a.size() + 1);
        for(int j = 0; j < (int)a.size(); j++) { vec[j + 1] = vec[j] + a[j]; }
    }

    int get(const int i) { return vec[i + 1] - vec[i]; }

    int rank(const int b, const int i) {
        if(b == 0) return i - vec[i];
        else return vec[i];
    }
};

template<typename T, int bit_len = 62> struct WaveletMatrix {
    vector<BitVector> B;
    vector<vector<T>> acc;
    vector<int> so;
    map<T, int> sn;
    int len;

    WaveletMatrix(vector<T> vec) {
        len = vec.size();
        acc = vector<vector<T>>(bit_len, vector<T>(len + 1));
        so = vector<int>(bit_len);
        vector<T> v(vec);
        for(int b = 0; b < bit_len; b++) {
            vector<T> cur;
            cur.reserve(len);
            vector<int> bi(len);
            for(int i = 0; i < len; i++) {
                ll bit = (v[i] >> (bit_len - b - 1)) & 1;
                if(bit == 0) {
                    cur.push_back(v[i]);
                    bi[i] = 0;
                }
            }
            so[b] = cur.size();
            for(int i = 0; i < len; i++) {
                ll bit = (v[i] >> (bit_len - b - 1)) & 1;
                if(bit == 1) {
                    cur.push_back(v[i]);
                    bi[i] = 1;
                }
            }
            B.push_back(BitVector(bi));
            for(int i = 0; i < len; i++) {
                if(B[b].get(i) == 0) acc[b][i + 1] = v[i];
                acc[b][i + 1] += acc[b][i];
            }
            v = cur;
        }
        for(int i = len - 1; i >= 0; i--) { sn[v[i]] = i; }
    }

    T access(int i) {
        T ret = 0;
        for(int j = 0; j < bit_len; j++) {
            int bit = B[j].get(i);
            ret = (ret << 1) | bit;
            i = B[j].rank(bit, i) + so[j] * bit;
        }
        return ret;
    }

    int rank(T val, int i) {
        if(!sn.count(val)) return 0;
        for(int j = 0; j < bit_len; j++) {
            int bit = (val >> (bit_len - j - 1)) & 1;
            i = B[j].rank(bit, i) + so[j] * bit;
        }
        return i - sn[val];
    }

    T kthMin(int left, int right, int k) {
        T ret = 0;
        for(int j = 0; j < bit_len; j++) {
            int l = B[j].rank(0, left);
            int r = B[j].rank(0, right);
            int cnt = r - l;
            if(cnt > k) {
                left = l;
                right = r;
            } else {
                k -= cnt;
                left += so[j] - l;
                right += so[j] - r;
                ret |= (1LL << (bit_len - j - 1));
            }
        }
        return ret;
    }

    T kMinSum(int left, int right, int k) {
        T ret = 0;
        for(int j = 0; j < bit_len; j++) {
            int l = B[j].rank(0, left);
            int r = B[j].rank(0, right);
            int cnt = r - l;
            if(cnt > k) {
                left = l;
                right = r;
            } else {
                k -= cnt;
                ret += acc[j][right] - acc[j][left];
                left += so[j] - l;
                right += so[j] - r;
            }
        }
        return ret;
    }

    int lessCount(int left, int right, T upper) {
        int ret = 0;
        if(upper >= (1LL << bit_len)) return right - left;
        for(int j = 0; j < bit_len; j++) {
            int l = B[j].rank(0, left);
            int r = B[j].rank(0, right);
            int cnt = r - l;
            if((upper >> (bit_len - j - 1)) & 1) {
                ret += cnt;
                left += so[j] - l;
                right += so[j] - r;
            } else {
                left = l;
                right = r;
            }
        }
        return ret;
    }
};