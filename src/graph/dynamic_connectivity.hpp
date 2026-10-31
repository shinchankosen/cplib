// https://github.com/shinchankosen/cplib/blob/main/src/graph/dynamic_connectivity.hpp
// dc.apply_time_range(0, 100, 1);  // 時刻 [0, 100) の範囲で辺番号 1 を追加
// dc.add_observation(50, 2);  // 時刻 50 に取得クエリ 2 を追加
// for (auto p : dc.generate()) {
//     if (p.op == Op::Begins) {
//         int edge_id = p.id;
//         // 辺番号 edge_id を張る
//     } else if (p.op == Op::Ends) {
//         // undo 操作を一回行う
//     } else if (p.op == Op::Event) {
//         int query_id = p.id;
//         // 連結判定などのクエリ処理
//     }
// }
enum class Op { Begins = 1, Ends = 2, Event = 3 };
template<class Tick = int> struct offline_dycon {  // Tick: 大小比較可能な型ならOK
    vector<pair<Tick, int>> ops;

    struct Edge {
        Tick since;
        Tick until;
        int id;
    };
    vector<Edge> es;

    offline_dycon() = default;
    void add_observation(Tick clk, int event_id) { ops.push_back({clk, event_id}); }
    void apply_time_range(Tick since, Tick until, int edge_id) { es.push_back(Edge{since, until, edge_id}); }

    struct Proc {
        Op op;
        int id;
    };
    vector<vector<Proc>> d;
    vector<Proc> ret;

    void rec(int now) {
        auto& dnow = d[now];
        ret.insert(ret.end(), dnow.cbegin(), dnow.cend());
        if(now * 2 < d.size()) rec(now * 2);
        if(now * 2 + 1 < d.size()) rec(now * 2 + 1);

        for(auto itr = dnow.rbegin(); itr != dnow.rend(); ++itr) {
            if(itr->op == Op::Begins) ret.push_back({Op::Ends, itr->id});
        }
    }

    vector<Proc> generate() {
        if(ops.empty()) return {};
        vector<Tick> ts;

        for(auto p : ops) ts.push_back(p.first);
        sort(all(ts));
        ts.erase(unique(all(ts)), ts.end());

        vector<int> t_use(ts.size() + 1);
        t_use[0] = 1;
        for(auto e : es) {
            t_use[lower_bound(all(ts), e.since) - ts.begin()] = 1;
            t_use[lower_bound(all(ts), e.until) - ts.begin()] = 1;
        }
        for(int i = 1; i < ts.size(); ++i) {
            if(!t_use[i]) ts[i] = ts[i - 1];
        }
        ts.erase(unique(all(ts)), ts.end());

        int N = ts.size(), D = 1;
        while(D < N) D *= 2;
        d.assign(D + N, {});
        for(auto e : es) {
            int l = D + (lower_bound(all(ts), e.since) - ts.begin());
            int r = D + (lower_bound(all(ts), e.until) - ts.begin());
            while(l < r) {
                if(r & 1) d[--r].push_back({Op::Begins, e.id});
                if(l & 1) d[l++].push_back({Op::Begins, e.id});
                l >>= 1;
                r >>= 1;
            }
        }

        for(auto [op, id] : ops) {
            int t = upper_bound(all(ts), op) - ts.begin();
            d.at(t + D - 1).push_back({Op::Event, id});
        }
        ret.clear();
        rec(1);
        return ret;
    }
};
