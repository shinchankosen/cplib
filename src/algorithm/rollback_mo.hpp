// https://github.com/shinchankosen/cplib/blob/main/src/algorithm/rollback_mo.hpp
struct Mo_rollback {
    int width;
    vector<int> left, right, order;
    Mo_rollback(int N, int Q) : order(Q) {
        width = sqrt(N);
        iota(all(order), 0);
    }

    void insert(int l, int r) {
        left.emplace_back(l);
        right.emplace_back(r);
    }

    void run(const auto& add_left, const auto& add_right, const auto& rem, const auto& reset, const auto& snapshot,
                      const auto& rollback) {
        sort(begin(order), end(order), [&](int a, int b) {
            int ablock = left[a] / width, bblock = left[b] / width;
            if(ablock != bblock) return ablock < bblock;
            return right[a] < right[b];
        });
        reset();
        snapshot();
        for(auto idx : order) {
            if(right[idx] - left[idx] < width) {
                for(int i = left[idx]; i < right[idx]; i++) add_right(i);
                rem(idx);
                rollback();
            }
        }
        int nr = 0, last_block = -1;
        for(auto idx : order) {
            if(right[idx] - left[idx] < width) continue;
            int block = left[idx] / width;
            if(block != last_block) {
                reset();
                nr = (block + 1) * width;
                last_block = block;
            }
            while(nr < right[idx]) add_right(nr++);
            snapshot();
            for(int j = (block + 1) * width - 1; j >= left[idx]; j--) add_left(j);
            rem(idx);
            rollback();
        }
    }
};