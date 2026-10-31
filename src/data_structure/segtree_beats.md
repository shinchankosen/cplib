lazy_segtreeの一部を変更して作る
```diff
void all_apply(int k, F f) {
    d[k] = mappin(f, d[k]);
-   if(k < size) lz[k] = composition(f, lz[k]);
+   if(k < size)){
+       lz[k] = composition(f, lz[k]);
+       if(d[k].fail)push(k), update(k);
+   }
}
```

- `S`は`alcoder::lazy_segtree`空参照可能なメンバ変数`fail`を持つ
- `mapping`関数による`S`の元`x`への作用の結果を得る計算が(`x`の持つ情報の不足が原因で)失敗した時のみ，`mapping`関数が返す`S`のインスタンスの`fail`の値は`true`となる
- `mapping`関数による作用以外の部分で計算は失敗しない
- 要素数1の区間を管理する`S`の元に対する`mapping`は失敗してはならない
