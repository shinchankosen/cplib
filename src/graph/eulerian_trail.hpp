// https://github.com/shinchankosen/cplib/blob/main/src/graph/eulerian_trail.hpp
// depends on src/data_structure/union_find.hpp
template<bool directed> struct EulerianTrail {
    vector<vector<pair<int, int>>> G;
    vector<pair<int, int>> es;
    int M;
    vector<int> usedV, usedE, deg;

    EulerianTrail(int N) : G(N), deg(N), usedV(N), M(0) {}

    void add_edge(int a, int b) {
        es.emplace_back(a, b);
        G[a].emplace_back(b, M);
        if(directed) {
            deg[a]++;
            deg[b]--;
        } else {
            G[b].emplace_back(a, M);
            deg[a]++;
            deg[b]++;
        }
        M++;
    }

    vector<int> go(int s) {
        stack<pair<int, int>> st;
        vector<int> ord;
        st.emplace(s, -1);
        while(!st.empty()) {
            int i = st.top().first;
            usedV[i] = true;
            if(G[i].empty()) {
                ord.emplace_back(st.top().second);
                st.pop();
            } else {
                auto e = G[i].back();
                G[i].pop_back();
                if(usedE[e.second]) continue;
                usedE[e.second] = true;
                if(!directed && es[e.second].first != i) swap(es[e.second].first, es[e.second].second);
                st.emplace(e);
            }
        }
        ord.pop_back();
        reverse(all(ord));
        return ord;
    }

    vector<vector<int>> enumerate_et() {
        if(directed) {
            for(auto& p : deg)
                if(p != 0) return {};
        } else {
            for(auto& p : deg) {
                if(p & 1) return {};
            }
        }
        usedE.assign(M, 0);
        vector<vector<int>> ret;
        for(int i = 0; i < size(G); i++) {
            if(G[i].empty() || usedV[i]) continue;
            ret.emplace_back(go(i));
        }
        return ret;
    }

    vector<vector<int>> enumerate_semi_et() {
        UnionFind d(size(G));
        for(auto& p : es) d.connect(p.first, p.second);
        vector<vector<int>> group(size(G));
        for(int i = 0; i < size(G); i++) group[d.root(i)].emplace_back(i);
        vector<vector<int>> ret;
        usedE.assign(M, 0);
        for(auto& vs : group) {
            if(vs.empty()) continue;
            int latte = -1, malta = -1;
            if(directed) {
                for(auto& p : vs) {
                    if(abs(deg[p]) > 1) return {};
                    else if(deg[p] == 1) {
                        if(latte >= 0) return {};
                        latte = p;
                    }
                }
            } else {
                for(auto& p : vs) {
                    if(deg[p] & 1) {
                        if(latte == -1) latte = p;
                        else if(malta == -1) malta = p;
                        else return {};
                    }
                }
            }
            ret.emplace_back(go(latte == -1 ? vs.front() : latte));
            if(ret.back().empty()) ret.pop_back();
        }
        return ret;
    }

    pair<int, int> get_edge(int i) { return es[i]; }
};