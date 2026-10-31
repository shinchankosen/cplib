// https://github.com/shinchankosen/cplib/blob/main/src/string/trie.hpp
template<int char_size> struct TrieNode {
    int nxt[char_size];
    int exist;
    vector<int> accept;

    TrieNode() : exist(0) { memset(nxt, -1, sizeof(nxt)); }
};

template<int char_size, int margin> struct Trie {
    using Node = TrieNode<char_size>;

    vector<Node> nodes;
    int root;
    Trie() : root(0) { nodes.push_back(Node()); }

    void update_direct(int node, int id) { nodes[node].accept.push_back(id); }

    void update_child(int node, int child, int id) { ++nodes[node].exist; }

    void add(const string& str, int str_index, int node_index, int id) {
        if(str_index == size(str)) {
            update_direct(node_index, id);
        } else {
            const int c = str[str_index] - margin;
            if(nodes[node_index].nxt[c] == -1) {
                nodes[node_index].nxt[c] = size(nodes);
                nodes.push_back(Node());
            }
            add(str, str_index + 1, nodes[node_index].nxt[c], id);
            update_child(node_index, nodes[node_index].nxt[c], id);
        }
    }

    void add(const string& str, int id = -1) {
        if(id == -1) id = nodes[0].exist;
        add(str, 0, 0, id);
    }

    void query(const string& str, const function<void(int)>& f, int str_index, int node_index) {
        for(auto& idx : nodes[node_index].accept) f(idx);
        if(str_index == size(str)) {
            return;
        } else {
            const int c = str[str_index] - margin;
            if(nodes[node_index].nxt[c] == -1) return;
            query(str, f, str_index + 1, nodes[node_index].nxt[c]);
        }
    }

    void query(const string& str, const function<void(int)>& f) { query(str, f, 0, 0); }

    int count() const { return nodes[0].exist; }
};