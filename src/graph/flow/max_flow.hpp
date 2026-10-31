// https://github.com/shinchankosen/cplib/blob/main/src/graph/flow/max_flow.hpp
template<class Cap> struct mf_graph {
    public:
    mf_graph() : _n(0) {}
    mf_graph(int n) : _n(n), g(n) {}

    int add_edge(int from, int to, Cap cap) {
        int m = size(pos);
        pos.push_back({from, size(g[from])});
        int from_id = size(g[from]);
        int to_id = size(g[to]);
        if(from == to) to_id++;
        g[from].push_back(_edge{to, to_id, cap});
        g[to].push_back(_edge{from, from_id, 0});
        return m;
    }

    Cap flow(int s, int t, Cap flow_limit = numeric_limits<Cap>::max()) {
        vector<int> level(_n), iter(_n);
        queue<int> que;
        auto bfs = [&]() {
            fill(all(level), -1);
            level[s] = 0;
            while(!que.empty()) que.pop();
            que.push(s);
            while(!que.empty()) {
                int v = que.front();
                que.pop();
                for(auto e : g[v]) {
                    if(e.cap == 0 || level[e.to] >= 0) continue;
                    level[e.to] = level[v] + 1;
                    if(e.to == t) return;
                    que.push(e.to);
                }
            }
        };
        auto dfs = [&](auto self, int v, Cap up) {
            if(v == s) return up;
            Cap res = 0;
            int level_v = level[v];
            for(int& i = iter[v]; i < size(g[v]); i++) {
                _edge& e = g[v][i];
                if(level_v <= level[e.to] || g[e.to][e.rev].cap == 0) continue;
                Cap d = self(self, e.to, min(up - res, g[e.to][e.rev].cap));
                if(d <= 0) continue;
                g[v][i].cap += d;
                g[e.to][e.rev].cap -= d;
                res += d;
                if(res == up) break;
            }
            return res;
        };

        Cap flow = 0;
        while(flow < flow_limit) {
            bfs();
            if(level[t] == -1) break;
            fill(all(iter), 0);
            while(flow < flow_limit) {
                Cap f = dfs(dfs, t, flow_limit - flow);
                if(!f) break;
                flow += f;
            }
        }
        return flow;
    }

    vector<bool> min_cut(int s) {
        vector<bool> visited(_n);
        queue<int> que;
        que.push(s);
        visited[s] = true;
        while(!que.empty()) {
            int v = que.front();
            que.pop();
            for(auto e : g[v]) {
                if(e.cap && !visited[e.to]) {
                    visited[e.to] = true;
                    que.push(e.to);
                }
            }
        }
        return visited;
    }

    struct edge {
        int from, to;
        Cap cap, flow;
    };

    edge get_edge(int i) {
        int m = size(pos);
        auto _e = g[pos[i].first][pos[i].second];
        auto _re = g[_e.to][_e.rev];
        return edge{pos[i].first, _e.to, _e.cap + _re.cap, _re.cap};
    }

    vector<edge> edges() {
        int m = size(pos);
        vector<edge> result;
        for(int i = 0; i < m; i++) result.push_back(get_edge(i));
        return result;
    }

    void change_edge(int i, Cap new_cap, Cap new_flow) {
        int m = size(pos);
        auto& _e = g[pos[i].first][pos[i].second];
        auto& _re = g[_e.to][_e.rev];
        _e.cap = new_cap - new_flow;
        _re.cap = new_flow;
    }

    private:
    int _n;
    struct _edge {
        int to, rev;
        Cap cap;
    };
    vector<pair<int, int>> pos;
    vector<vector<_edge>> g;
};