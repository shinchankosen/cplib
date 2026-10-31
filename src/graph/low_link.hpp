// https://github.com/shinchankosen/cplib/blob/main/src/graph/low_link.hpp
struct LowLink {
    vector<vector<int>> g;
    vector<int> ord, low;
    vector<int> articulation;
    vector<bool> visited;
    vector<pair<int, int>> bridge;

    void dfs(int cur, int pre, int& k) {
        visited[cur] = true;
        ord[cur] = low[cur] = k++;
        bool isArticulation = false, beet = false;
        int cnt = 0;
        for(auto to : g[cur]) {
            if(to == pre && !exchange(beet, true)) continue;
            if(!visited[to]) {
                cnt++;
                dfs(to, cur, k);
                chmin(low[cur], low[to]);
                isArticulation |= pre != -1 && low[to] >= ord[cur];
                if(ord[cur] < low[to]) bridge.emplace_back(min(cur, to), max(cur, to));
            } else chmin(low[cur], ord[to]);
        }
        isArticulation |= pre == -1 && cnt > 1;
        if(isArticulation) articulation.push_back(cur);
    }

    void build(const vector<vector<int>>& g) {
        int n = g.size();
        this->g = g;
        ord.assign(n, -1);
        low.assign(n, -1);
        visited.assign(n, false);
        int k = 0;
        for(int i = 0; i < n; i++)
            if(!visited[i]) dfs(i, -1, k);
    }

    LowLink(const vector<vector<int>>& g) { build(g); }
};