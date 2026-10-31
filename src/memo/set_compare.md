```cpp
   auto comp = [&](const int a, const int b) {return a > b;};
   set<int, decltype(comp)> st{comp};
```