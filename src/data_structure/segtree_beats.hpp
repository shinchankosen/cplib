// https://github.com/shinchankosen/cplib/blob/main/src/data_structure/segtree_beats.hpp
// depends on src/data_structure/lazy_segtree.hpp
namespace RangeChMinMaxAddSum {
template<typename Num> inline Num second_lowest(Num a, Num a2, Num c, Num c2) noexcept {
    // a < a2, c < c2 のとき全引数を昇順ソートして二番目の値
    return a == c ? std::min(a2, c2) : a2 <= c ? a2 : c2 <= a ? c2 : std::max(a, c);
}
template<typename Num> inline Num second_highest(Num a, Num a2, Num b, Num b2) noexcept {
    // a > a2, b > b2 のとき全引数を降順ソートして二番目の値
    return a == b ? std::max(a2, b2) : a2 >= b ? a2 : b2 >= a ? b2 : std::min(a, b);
}

constexpr ll BINF = 1LL << 61;

struct S {
    ll lo, hi, lo2, hi2, sum;  // 区間最小・最大値，区間最小・最大から二番目の値，区間総和
    unsigned sz, nlo, nhi;     // 区間要素数，区間最小・最大値をとる要素の個数
    bool fail;
    S() : lo(BINF), hi(-BINF), lo2(BINF), hi2(-BINF), sum(0), sz(0), nlo(0), nhi(0), fail(0) {}
    S(ll x, unsigned sz_ = 1)
         : lo(x), hi(x), lo2(BINF), hi2(-BINF), sum(x * sz_), sz(sz_), nlo(sz_), nhi(sz_), fail(0) {}
};

S e() { return S(); }
S op(S l, S r) {
    S ret;
    ret.lo = min(l.lo, r.lo), ret.hi = max(l.hi, r.hi);
    ret.lo2 = second_lowest(l.lo, l.lo2, r.lo, r.lo2);
    ret.hi2 = second_highest(l.hi, l.hi2, r.hi, r.hi2);
    ret.sum = l.sum + r.sum, ret.sz = l.sz + r.sz;
    ret.nlo = l.nlo * (l.lo <= r.lo) + r.nlo * (r.lo <= l.lo);
    ret.nhi = l.nhi * (l.hi >= r.hi) + r.nhi * (r.hi >= l.hi);
    return ret;
}

struct F {
    ll lb, ub, bias;
    F(ll chmax_ = -BINF, ll chmin_ = BINF, ll add = 0) : lb(chmax_), ub(chmin_), bias(add) {}
    static F chmin(ll x) noexcept { return F(-BINF, x, ll(0)); }
    static F chmax(ll x) noexcept { return F(x, BINF, ll(0)); }
    static F add(ll x) noexcept { return F(-BINF, BINF, x); };
};

F composition(F fnew, F fold) {
    F ret;
    ret.lb = max(min(fold.lb + fold.bias, fnew.ub), fnew.lb) - fold.bias;
    ret.ub = min(max(fold.ub + fold.bias, fnew.lb), fnew.ub) - fold.bias;
    ret.bias = fold.bias + fnew.bias;
    return ret;
}

F id() { return F(); }

S mapping(F f, S x) {
    if(x.sz == 0) return e();
    else if(x.lo == x.hi or f.lb == f.ub or f.lb >= x.hi or f.ub <= x.lo) {
        return S(min(max(x.lo, f.lb), f.ub) + f.bias, x.sz);
    } else if(x.lo2 == x.hi) {
        x.lo = x.hi2 = max(x.lo, f.lb) + f.bias;
        x.hi = x.lo2 = min(x.hi, f.ub) + f.bias;
        x.sum = x.lo * x.nlo + x.hi * x.nhi;
        return x;
    } else if(f.lb < x.lo2 and f.ub > x.hi2) {
        ll nxt_lo = max(x.lo, f.lb), nxt_hi = min(x.hi, f.ub);
        x.sum += (nxt_lo - x.lo) * x.nlo - (x.hi - nxt_hi) * x.nhi + f.bias * x.sz;
        x.lo = nxt_lo + f.bias, x.hi = nxt_hi + f.bias, x.lo2 += f.bias, x.hi2 += f.bias;
        return x;
    }
    x.fail = 1;
    return x;
}
using segtree = lazy_segtree<S, op, e, F, mapping, composition, id>;
}  // namespace RangeChMinMaxAddSum