// https://github.com/shinchankosen/cplib/blob/main/src/graph/two_edge_connected_components.hpp
// depends on src/graph/low_link.hpp
struct TwoEdgeConnected : LowLink {
    public:
    using LowLink::bridge;
    using LowLink::g;
    using LowLink::low;
    using LowLink::ord;

    vector<int> comp;
    vector<vector<int>> tree;
    vector<vector<int>> group;

    void build(const vector<vector<int>>& g) {
        comp.assign(size(g), -1);
        int k = 0;
        for(int i = 0; i < size(comp); i++) {
            if(comp[i] == -1) { dfs(i, -1, k); }
        }
        group.resize(k);
        for(int i = 0; i < size(g); i++) { group[comp[i]].push_back(i); }
        tree.resize(k);
        for(auto& e : bridge) {
            tree[comp[e.first]].push_back(comp[e.second]);
            tree[comp[e.second]].push_back(comp[e.first]);
        }
    }

    explicit TwoEdgeConnected(const vector<vector<int>>& g) : LowLink(g) { build(g); }

    private:
    vector<int> used;
    vector<pair<int, int>> tmp;

    void dfs(int cur, int pre, int& k) {
        if(pre != -1 && ord[pre] >= low[cur]) comp[cur] = comp[pre];
        else comp[cur] = k++;
        for(auto to : g[cur]) {
            if(comp[to] == -1) dfs(to, cur, k);
        }
    }
};