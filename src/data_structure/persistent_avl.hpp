// https://github.com/shinchankosen/cplib/blob/main/src/data_structure/persistent_avl.hpp
const int bufSize = 18000000;
struct node {
    node const *l, *r;
    ll lz, sum;
    int sz, dep;
};
static node buf[bufSize];
using np = node const*;
using lr = pair<np, np>;
static int bufUsed = 0;
np newnode(np l, np r, ll lz, ll sum, int sz, int dep) {
    buf[bufUsed] = node{l, r, lz, sum, sz, dep};
    return buf + (bufUsed++);
}
ll prod(np x) { return x->sum + x->lz * x->sz; }
np newpar(np l, np r) { return newnode(l, r, 0, prod(l) + prod(r), l->sz + r->sz, max(l->dep, r->dep) + 1); }
np add(np x, ll alz) { return newnode(x->l, x->r, x->lz + alz, x->sum, x->sz, x->dep); }
lr merge1(np a, np b, np c) {
    if(abs(max(a->dep, b->dep) + 1 - c->dep) <= 1) return lr{newpar(a, b), c};
    if(abs(max(b->dep, c->dep) + 1 - a->dep) <= 1) return lr{a, newpar(b, c)};
    return lr{newpar(a, add(b->l, b->lz)), newpar(add(b->r, b->lz), c)};
}
lr merge2(np a, np b, ll alz, ll blz) {
    if(a->dep < b->dep) {
        blz += b->lz;
        lr x = merge2(a, b->l, alz, blz);
        return merge1(x.first, x.second, add(b->r, blz));
    } else if(a->dep > b->dep) {
        alz += a->lz;
        lr x = merge2(a->r, b, alz, blz);
        return merge1(add(a->l, alz), x.first, x.second);
    } else return lr{add(a, alz), add(b, blz)};
}
np merge(np a, np b) {
    if(!a) return b;
    if(!b) return a;
    lr x = merge2(a, b, 0, 0);
    return newpar(x.first, x.second);
}
lr split(np x, int s, ll lz) {
    // split into [0..s) and [s..end)
    if(x->sz == s) return lr{add(x, lz), nullptr};
    if(s == 0) return lr{nullptr, add(x, lz)};
    lz += x->lz;
    if(s <= x->l->sz) {
        lr g = split(x->l, s, lz);
        return lr{g.first, merge(g.second, add(x->r, lz))};
    } else {
        lr g = split(x->r, s - x->l->sz, lz);
        return lr{merge(add(x->l, lz), g.first), g.second};
    }
}
np build(ll v) { return newnode(nullptr, nullptr, v, 0, 1, 0); }