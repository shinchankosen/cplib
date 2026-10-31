// https://github.com/shinchankosen/cplib/blob/main/src/geometry/etc.hpp
// depends on src/geometry/base.hpp
// 1: a-bから見てa-cが反時計回り -1: a-bから見てa-cが時計回り
// 0: a-c-bがこの順で直線
// 2: c-a-bの順で直線 -2: a-b-cの順で直線
int ccw(const Point& a, const Point& b, const Point& c) {
    if(cross(b - a, c - a) > EPS) return 1;
    if(cross(b - a, c - a) < -EPS) return -1;
    if(dot(b - a, c - a) < -EPS) return 2;
    if(norm(b - a) < norm(c - a) - EPS) return -2;
    return 0;
}
Point projection(const Point& p, const Line& l) {
    double t = dot(p - l[0], l[1] - l[0]) / norm(l[1] - l[0]);
    return l[0] + t * (l[1] - l[0]);
}
Point reflection(const Point& p, const Line& l) { return 2.0 * projection(p, l) - p; }
bool isinterPL(const Point& p, const Line& l) { return abs(p - projection(p, l)) < EPS; }
bool isinterPS(const Point& p, const Line& s) { return ccw(s[0], s[1], p) == 0; }
bool isinterLL(const Line& l, const Line& m) {
    return abs(cross(l[1] - l[0], m[1] - m[0])) > EPS || abs(cross(l[1] - l[0], m[0] - l[0])) < EPS;
}
bool isinterSS(const Line& s, const Line& t) {
    if(norm(s[1] - s[0]) < EPS) return isinterPS(s[0], t);
    if(norm(t[1] - t[0]) < EPS) return isinterPS(t[0], s);
    return (ccw(s[0], s[1], t[0]) * ccw(s[0], s[1], t[1]) <= 0) && (ccw(t[0], t[1], s[0]) * ccw(t[0], t[1], s[1]) <= 0);
}
double distancePL(const Point& p, const Line& l) { return abs(p - projection(p, l)); }
double distancePS(const Point& p, const Line& s) {
    Point h = projection(p, s);
    if(isinterPS(h, s)) return abs(p - h);
    return min(abs(p - s[0]), abs(p - s[1]));
}
double distanceLL(const Line& l, const Line& m) {
    if(isinterLL(l, m)) return 0;
    return distancePL(l[0], m);
}
double distanceSS(const Line& s, const Line& t) {
    if(isinterSS(s, t)) return 0;
    return min(min(distancePS(s[0], t), distancePS(s[1], t)), min(distancePS(t[0], s), distancePS(t[1], s)));
}
// if(ans){ x = p->X; y = q->Y} else {cout << "not cross"}
optional<Point> crosspoint(const Line& l, const Line& m) {
    double d = cross(m[1] - m[0], l[1] - l[0]);
    if(abs(d) < EPS) return nullopt;
    return l[0] + (l[1] - l[0]) * cross(m[1] - m[0], m[1] - l[0]) / d;
}
double area(const vector<Point>& ps) {
    double res = 0;
    for(int i = 0; i < size(ps); i++) res += cross(ps[i], ps[(i + 1) % size(ps)]);
    return res / 2;
}
bool is_convex(vector<Point>& ps) {
    int n = (int)ps.size();
    for(int i = 0; i < n; ++i) {
        if(ccw(ps[i], ps[(i + 1) % n], ps[(i + 2) % n]) == -1) return false;
    }
    return true;
}
tuple<double, int, int> diameter(const vector<Point> ps) {
    int n = (int)ps.size();
    int si = 0, sj = 0;
    for(int i = 1; i < n; i++) {
        if(ps[i].Y > ps[si].Y) si = i;
        if(ps[i].Y < ps[sj].Y) sj = i;
    }
    double res = 0;
    int i = si, j = sj;
    int ri = i, rj = j;
    do {
        if(chmax(res, abs(ps[i] - ps[j]))) {
            ri = i;
            rj = j;
        }
        if(cross(ps[(i + 1) % n] - ps[i], ps[(j + 1) % n] - ps[j]) < 0) i = (i + 1) % n;
        else j = (j + 1) % n;
    } while(i != si || j != sj);
    return {res, min(ri, rj), max(ri, rj)};
}
// 2: inside, 1: border, 0: outside
int contains(const vector<Point>& ps, const Point& p) {
    int n = ps.size();
    bool in = false;
    for(int i = 0; i < n; i++) {
        Point a = ps[i] - p, b = ps[(i + 1) % n] - p;
        if(a.Y > b.Y) swap(a, b);
        if(a.Y <= EPS && EPS < b.Y && cross(a, b) < -EPS) in = !in;
        if(abs(cross(a, b)) < EPS && dot(a, b) < EPS) return 1;
    }
    return in ? 2 : 0;
}
tuple<double, int, int> closest_pair(vector<Point> ps) {
    const double INF = 1e18;
    int n = (int)ps.size();
    if(n <= 1) return {INF, -1, -1};
    using P = pair<Point, int>;
    vector<P> V(n);
    for(int i = 0; i < n; i++) V[i] = {ps[i], i};
    sort(begin(V), end(V), [](const P& a, const P& b) {
        if(fabs(a.first.X - b.first.X) > EPS) return a.first.X < b.first.X;
        else if(fabs(a.first.Y - b.first.Y) > EPS) return a.first.Y < b.first.Y;
        return a.second < b.second;
    });
    auto rec = [&](auto&& self, auto it, int n) -> tuple<double, int, int> {
        if(n <= 1) return {INF, -1, -1};
        int m = n / 2;
        double x = it[m].first.X;
        auto [d1, a1, b1] = self(self, it, m);
        auto [d2, a2, b2] = self(self, it + m, n - m);
        double d;
        int a, b;
        if(d1 < d2) {
            d = d1;
            a = a1;
            b = b1;
        } else {
            d = d2;
            a = a2;
            b = b2;
        }
        inplace_merge(it, it + m, it + n, [](const P& a, const P& b) { return a.first.Y < b.first.Y; });
        vector<P> vec;
        for(int i = 0; i < n; i++) {
            if(fabs(it[i].first.X - x) >= d) continue;
            for(int j = 0; j < size(vec); j++) {
                double dx = fabs(it[i].first.X - vec[size(vec) - j - 1].first.X);
                double dy = fabs(it[i].first.Y - vec[size(vec) - j - 1].first.Y);
                if(dy >= d) break;
                if(chmin(d, sqrt(dx * dx + dy * dy))) {
                    a = it[i].second;
                    b = vec[size(vec) - j - 1].second;
                }
            }
            vec.emplace_back(it[i]);
        }
        return {d, a, b};
    };
    auto [d, a, b] = rec(rec, V.begin(), n);
    return {d, min(a, b), max(a, b)};
}