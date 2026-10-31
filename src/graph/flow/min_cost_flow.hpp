// https://github.com/shinchankosen/cplib/blob/main/src/graph/flow/min_cost_flow.hpp
template<class Cap, class Cost> struct mcf_graph {
    public:
    mcf_graph() {}
    mcf_graph(int n) : _n(n), g(n) {}

    int add_edge(int from, int to, Cap cap, Cost cost) {
        int m = size(pos);
        pos.push_back({from, size(g[from])});
        int from_id = size(g[from]);
        int to_id = size(g[to]);
        if(from == to) to_id++;
        g[from].push_back(_edge{to, to_id, cap, cost});
        g[to].push_back(_edge{from, from_id, 0, -cost});
        return m;
    }

    pair<Cap, Cost> flow(int s, int t, Cap flow_limit = numeric_limits<Cap>::max()) {
        return slope(s, t, flow_limit).back();
    }
    vector<pair<Cap, Cost>> slope(int s, int t, Cap flow_limit = numeric_limits<Cap>::max()) {
        vector<Cost> dual(_n, 0), dist(_n);
        vector<int> pv(_n), pe(_n);
        vector<bool> vis(_n);
        auto dual_ref = [&]() {
            fill(all(dist), numeric_limits<Cost>::max());
            fill(all(pv), -1);
            fill(all(pe), -1);
            fill(all(vis), false);
            struct Q {
                Cost key;
                int to;
                bool operator<(const Q& r) const { return key > r.key; }
            };
            priority_queue<Q> que;
            dist[s] = 0;
            que.push(Q{0, s});
            while(!que.empty()) {
                int v = que.top().to;
                que.pop();
                if(vis[v]) continue;
                vis[v] = true;
                if(v == t) break;
                for(int i = 0; i < size(g[v]); i++) {
                    auto e = g[v][i];
                    if(vis[e.to] || !e.cap) continue;
                    Cost cost = e.cost - dual[e.to] + dual[v];
                    if(chmin(dist[e.to], dist[v] + cost)) {
                        pv[e.to] = v;
                        pe[e.to] = i;
                        que.push(Q{dist[e.to], e.to});
                    }
                }
            }
            if(!vis[t]) return false;
            for(int v = 0; v < _n; v++)
                if(vis[v]) dual[v] -= dist[t] - dist[v];
            return true;
        };
        Cap flow = 0;
        Cap cost = 0, prev_cost_per_flow = -1;
        vector<pair<Cap, Cost>> result;
        result.push_back({flow, cost});
        while(flow < flow_limit) {
            if(!dual_ref()) break;
            Cap c = flow_limit - flow;
            for(int v = t; v != s; v = pv[v]) { c = min(c, g[pv[v]][pe[v]].cap); }
            for(int v = t; v != s; v = pv[v]) {
                auto& e = g[pv[v]][pe[v]];
                e.cap -= c;
                g[v][e.rev].cap += c;
            }
            Cost d = -dual[s];
            flow += c;
            cost += c * d;
            if(prev_cost_per_flow == d) { result.pop_back(); }
            result.push_back({flow, cost});
            prev_cost_per_flow = d;
        }
        return result;
    }

    struct edge {
        int from, to;
        Cap cap, flow;
    };

    edge get_edge(int i) {
        int m = size(pos);
        auto _e = g[pos[i].first][pos[i].second];
        auto _re = g[_e.to][_e.rev];
        return edge({pos[i].first, _e.to, _e.cap + _re.cap, _re.cap});
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
        Cost cost;
    };

    vector<pair<int, int>> pos;
    vector<vector<_edge>> g;
};