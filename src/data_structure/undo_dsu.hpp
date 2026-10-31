// https://github.com/shinchankosen/cplib/blob/main/src/data_structure/undo_dsu.hpp
struct undo_dsu {
    private:
    int _n;
    vector<int> p;
    stack<pair<int, int>> history;

    public:
    undo_dsu() : _n(0) {}
    explicit undo_dsu(int n) : _n(n), p(n, -1) {}
    int merge(int a, int b) {
        // assert(0 <= a && a < _n);
        // assert(0 <= b && b < _n);
        int x = leader(a), y = leader(b);
        if(x == y) {
            history.emplace(x, p[x]);
            history.emplace(y, p[y]);
            return x;
        }
        if(-p[x] < -p[y]) swap(x, y);
        history.emplace(x, p[x]);
        history.emplace(y, p[y]);
        p[x] += p[y];
        p[y] = x;
        return x;
    }
    bool same(int a, int b) {
        // assert(0 <= a && a < _n);
        // assert(0 <= b && b < _n);
        return leader(a) == leader(b);
    }
    int leader(int a) {
        // assert(0 <= a && a < _n);
        while(p[a] >= 0) a = p[a];
        return a;
    }
    void undo() {
        p[history.top().first] = history.top().second;
        history.pop();
        p[history.top().first] = history.top().second;
        history.pop();
    }
    int snapshot() { return history.size(); }
    void rollback(int snapshot = 0) {
        while(history.size() > snapshot) undo();
    }
    int size(int a) {
        // assert(0 <= a && a < _n);
        return -p[leader(a)];
    }
};