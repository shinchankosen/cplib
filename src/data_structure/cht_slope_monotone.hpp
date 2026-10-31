// https://github.com/shinchankosen/cplib/blob/main/src/data_structure/cht_slope_monotone.hpp
template<typename T> class CHT {
    using P = pair<T, T>;

    private:
    int head;
    vector<P> lines;
    // 最小値(最大値)を求めるxが単調であるか
    bool isMonotonicX;
    function<bool(T l, T r)> comp;

    public:
    // クエリが単調であった場合はflag = trueとする
    CHT(
            bool flagX = false, function<bool(T l, T r)> compFunc = [](T l, T r) { return l >= r; })
            : isMonotonicX(flagX), comp(compFunc), head(0){};

    bool check(P l1, P l2, P l3) {
        if(l1 < l3) swap(l1, l3);
        return (l3.second - l2.second) * (l2.first - l1.first) >= (l2.second - l1.second) * (l3.first - l2.first);
    }
    void add(T a, T b) {
        P line(a, b);
        while(lines.size() >= 2 + head && check(*(lines.end() - 2), lines.back(), line)) lines.pop_back();
        lines.emplace_back(line);
    }
    T f(int i, T x) { return lines[i].first * x + lines[i].second; }
    T f(P line, T x) { return line.first * x + line.second; }
    T get(T x) {
        if(isMonotonicX) {
            while(lines.size() >= 2 + head && comp(f(head, x), f(head + 1, x))) ++head;
            return f(head, x);
        } else {
            int low = -1, high = lines.size() - 1;
            while(high - low > 1) {
                int mid = (high + low) / 2;
                (comp(f(mid, x), f(mid + 1, x)) ? low : high) = mid;
            }
            return f(high, x);
        }
    }
};