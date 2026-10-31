## テンプレート

- [テンプレート](src/template/template.cpp)
- [構造体テンプレート](src/template/struct_template.cpp)

## データ構造

- [セグメント木](src/data_structure/segtree.hpp)
- [遅延セグメント木](src/data_structure/lazy_segtree.hpp)
- [動的二次元セグメント木](src/data_structure/dynamic_segtree_2d.hpp)
- [Fenwick木](src/data_structure/fenwick_tree.hpp)
- [Binary Trie](src/data_structure/binary_trie.hpp)
- [AVL木](src/data_structure/avl_tree.hpp)
- [負の添字を扱える配列](src/data_structure/negative_vector.hpp)
- [Disjoint Sparse Table](src/data_structure/disjoint_sparse_table.hpp)
- [Li Chao木](src/data_structure/li_chao_tree.hpp)
- [傾き単調のConvex Hull Trick](src/data_structure/cht_slope_monotone.hpp)
- [Undo可能Union-Find](src/data_structure/undo_dsu.hpp)
- [ポテンシャル付きUnion-Find](src/data_structure/potential_dsu.hpp)
- [Wavelet Matrix](src/data_structure/wavelet_matrix.hpp)
- [Range Tree](src/data_structure/range_tree.hpp)
- [Segment Tree Beats](src/data_structure/segtree_beats.hpp)
- [Segment Tree Beatsの解説](src/data_structure/segtree_beats.md)
- [64分木](src/data_structure/tree_64.hpp)
- [Treap](src/data_structure/treap.hpp)
- [永続AVL木](src/data_structure/persistent_avl.hpp)
- [永続配列](src/data_structure/persistent_array.hpp)
- [永続セグメント木](src/data_structure/persistent_segtree.hpp)
- [永続Union-Find](src/data_structure/persistent_union_find.hpp)
- [Union-Find](src/data_structure/union_find.hpp)
- [多次元累積和](src/data_structure/prefix_sum_nd.hpp)
- [上位K個の和](src/data_structure/top_k_sum.hpp)
- [mex管理](src/data_structure/range_mex_set.hpp)

## グラフ

- [ダイクストラ法](src/graph/dijkstra.hpp)
- [ベルマンフォード法](src/graph/bellman_ford.hpp)
- [グリッド上の幅優先探索](src/graph/grid_bfs.hpp)
- [トポロジカルソート](src/graph/topological_sort.hpp)
- [強連結成分分解](src/graph/scc.hpp)
- [Low Link](src/graph/low_link.hpp)
- [二重辺連結成分分解](src/graph/two_edge_connected_components.hpp)
- [オイラー路](src/graph/eulerian_trail.hpp)
- [マンハッタン最小全域木](src/graph/manhattan_mst.hpp)
- [Dynamic Connectivity](src/graph/dynamic_connectivity.hpp)
- [牛ゲー](src/graph/ushi.hpp)

### 木

- [重心分解](src/graph/tree/centroid_decomposition.hpp)
- [木の直径・中心](src/graph/tree/diameter_center.hpp)
- [ダブリング・最小共通祖先](src/graph/tree/doubling_lca.hpp)
- [パス・部分木クエリ](src/graph/tree/hld.hpp)
- [Dominator Tree](src/graph/tree/dominator_tree.hpp)
- [Link-Cut Tree](src/graph/tree/link_cut_tree.hpp)

### フロー

- [最大流](src/graph/flow/max_flow.hpp)
- [最小費用流](src/graph/flow/min_cost_flow.hpp)
- [Hopcroft-Karp](src/graph/flow/hopcroft_karp.hpp)
- [燃やす埋める](src/graph/flow/燃やす埋める.md)

## 数学

- [Modint](src/math/modint.hpp)
- [繰り返し二乗法・剰余逆元](src/math/modpow_modinv.hpp)
- [離散対数](src/math/modlog.hpp)
- [二項係数](src/math/combination.hpp)
- [拡張ユークリッドの互除法](src/math/ext_gcd.hpp)
- [中国剰余定理](src/math/crt.hpp)
- [高速アダマール変換](src/math/fwt.hpp)
- [Subset Convolution](src/math/subset_convolution.hpp)
- [Binary GCD](src/math/binary_gcd.hpp)
- [平方剰余](src/math/modsqrt.hpp)
- [素数判定](src/math/primality.hpp)
- [原始根](src/math/primitive_root.hpp)
- [素因数分解](src/math/rho.hpp)
- [Barrett Reduction](src/math/barrett_reduction.hpp)
- [行列](src/math/matrix.hpp)
- [Floor Sum](src/math/floor_sum.hpp)
- [整数平方根](src/math/sqrtll.hpp)
- [分数](src/math/fraction.hpp)
- [Min of Mod of Linear](src/math/min_of_mod_of_linear.hpp)

## 形式的冪級数

- [畳み込み](src/fps/ntt_convolution.hpp)
- [形式的冪級数](src/fps/fps.hpp)
- [Bostan Mori](src/fps/bostan_mori.hpp)
- [Relaxed Convolution](src/fps/relaxed_convolution.hpp)

## 幾何

- [凸包](src/geometry/convex_hull.hpp)
- [幾何の基本](src/geometry/base.hpp)
- [幾何の諸関数](src/geometry/etc.hpp)

## 文字列

- [ローリングハッシュ](src/string/rolling_hash.hpp)
- [Z-algorithm](src/string/z_algorithm.hpp)
- [最長共通部分列](src/string/lcs_bitset.hpp)
- [KMP法](src/string/kmp.hpp)
- [Manacher](src/string/manacher.hpp)
- [Trie](src/string/trie.hpp)

## アルゴリズム

- [Mo-algorithm](src/algorithm/mo.hpp)
- [グリッド操作](src/algorithm/grid.hpp)
- [Monge最短路](src/algorithm/larsch.hpp)
- [Min Plus Convolution](src/algorithm/min_plus_convolution.hpp)
- [Concaveなmin plus](src/algorithm/min_plus_concave.hpp)
- [Rollback Mo](src/algorithm/rollback_mo.hpp)
- [3次元Mo](src/algorithm/mo_3d.hpp)

## ヒューリスティック

- [乱数](src/heuristic/random.hpp)

## メモ

- [QCFium法](src/memo/qcfium.hpp)
- [segmentation fault対処](src/memo/when_segmentation_fault.txt)
- [pb_ds](src/memo/ext.hpp)
- [素数](src/memo/primes.md)
- [多倍長整数](src/memo/bigint.md)
- [setの比較関数](src/memo/set_compare.md)
