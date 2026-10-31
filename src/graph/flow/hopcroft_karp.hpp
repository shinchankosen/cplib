// https://github.com/shinchankosen/cplib/blob/main/src/graph/flow/hopcroft_karp.hpp
struct bp_graph {
    int lsz, rsz;
    vector<vector<int>> L;
    vector<int> R;

    bp_graph() {}
    bp_graph(int l, int r) : lsz(l), rsz(r), L(l), R(r, -1) {}

    void add_edge(int l, int r) { L[l].push_back(r); }
    int hopcroft() {
        vector<int> dist;
        auto dfs = [&](auto self, int src) -> bool {
            int d = dist[src];
            dist[src] = -1;
            for(auto r : L[src]) {
                int l = R[r];
                if(l == -1 || (d + 1 == dist[l] && self(self, l))) {
                    R[r] = src;
                    return true;
                }
            }
            return false;
        };

        bool found;
        do {
            queue<int> q;
            dist.assign(lsz, 0);
            for(int l : R)
                if(l != -1) dist[l] = -1;
            for(int l = 0; l < lsz; ++l)
                if(dist[l] == 0) q.push(l);

            while(!q.empty()) {
                int src = q.front();
                q.pop();
                for(auto r : L[src]) {
                    int l = R[r];
                    if(l == -1 || dist[l] != -1) continue;
                    dist[l] = dist[src] + 1;
                    q.push(l);
                }
            }
            found = false;
            for(int l = 0; l < lsz; ++l) found |= (dist[l] == 0 && dfs(dfs, l));
        } while(found);
        return rsz - count(R.begin(), R.end(), -1);
    }

    vector<pair<int, int>> get_matching() {
        vector<pair<int, int>> matching;
        for(int r = 0; r < rsz; ++r) {
            if(R[r] != -1) matching.emplace_back(R[r], r);
        }
        return matching;
    }

    // false := minimum vertex cover, true := maximum independent set
    pair<vector<bool>, vector<bool>> maximum_independent_set() {
        vector<bool> lvis(lsz, true), rvis(rsz, true);
        queue<int> q;
        for(int l : R)
            if(l != -1) lvis[l] = false;
        for(int l = 0; l < lsz; ++l)
            if(lvis[l]) q.push(l);
        while(!q.empty()) {
            int src = q.front();
            q.pop();
            for(auto r : L[src]) {
                if(R[r] == src) continue;
                if(!rvis[r]) continue;
                rvis[r] = false;
                int l = R[r];
                if(l != -1 && !lvis[l]) {
                    lvis[l] = true;
                    q.push(l);
                }
            }
        }
        return {lvis, rvis};
    }

    // 孤立点がある場合，未定義動作
    vector<pair<int, int>> minimum_edge_cover() {
        vector<bool> lused(lsz, false), rused(rsz, false);
        auto res = get_matching();
        for(auto [l, r] : res) {
            lused[l] = true;
            rused[r] = true;
        }
        for(int l = 0; l < lsz; ++l) {
            if(!lused[l]) {
                int r = L[l][0];
                res.emplace_back(l, r);
            }
            for(auto r : L[l]) {
                if(!rused[r]) {
                    res.emplace_back(l, r);
                    rused[r] = true;
                    break;
                }
            }
        }
        return res;
    }
};
