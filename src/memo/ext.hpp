// https://github.com/shinchankosen/cplib/blob/main/src/memo/ext.hpp
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <ext/pb_ds/priority_queue.hpp>
#include <ext/rope>
using namespace __gnu_pbds;
using namespace __gnu_cxx;  // for ext/rope
using namespace std;
int main() {
    tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> tree;
    tree.insert(1);                       // 1
    tree.insert(10);                      // 1 10
    tree.insert(6);                       // 1 6 10
    tree.insert(7);                       // 1 6 7 10
    tree.insert(4);                       // 1 4 6 7 10
    tree.erase(6);                        // 1 4 7 10
    assert(tree.order_of_key(5) == 2);    // 5未満の要素数
    assert(*tree.find_by_order(2) == 7);  // 0-indexedで2番目の要素

    gp_hash_table<int, int> m;
    m[2] = 5;
    m[1] = 10;
    m[3] = 7;

    __gnu_pbds::priority_queue<int, greater<int>, rc_binomial_heap_tag> pq;
    auto it = pq.push(1);  // 1
    pq.push(10);           // 1 10
    assert(pq.top() == 1);
    pq.modify(it, 20);  // 10 20
    assert(pq.top() == 10);
    pq.erase_if([](int x) { return x <= 10; });  // 20
    assert(pq.top() == 20);
    assert(pq.size() == 1);
    pq.erase(it);  // empty
    assert(pq.empty());

    // access, insert, erase: O(log n)
    rope<int> v;
    v.insert(0, 1);  // 1
    v.insert(0, 2);  // 2 1
    v.insert(1, 3);  // 2 3 1
    v.insert(2, 4);  // 2 3 4 1
    v.erase(1, 2);   // 2 1
    assert(v.size() == 2);
    assert(v[0] == 2);
    assert(v[1] == 1);
}