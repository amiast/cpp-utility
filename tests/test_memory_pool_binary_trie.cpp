// Verified with: https://judge.yosupo.jp/problem/set_xor_min
// Details: https://judge.yosupo.jp/submission/318039

#include <iostream>
#include <stack>
#include <kotone/memory_pool>

struct binary_trie {
  private:
    struct node {
        node *_children[2] = {nullptr, nullptr};
        int _count = 0;
    };

    kotone::memory_pool<node> _pool;
    node *_root;

  public:
    binary_trie() : _pool(256), _root(_pool.allocate()) {}

    bool insert(int x) {
        bool inserted = false;
        std::stack<node*> stack;
        stack.push(_root);
        for (int k = 29; k >= 0; k--) {
            if (!stack.top()->_children[x >> k & 1]) {
                stack.top()->_children[x >> k & 1] = _pool.allocate();
                inserted = true;
            }
            stack.push(stack.top()->_children[x >> k & 1]);
        }
        if (!inserted) return false;
        while (stack.size()) {
            stack.top()->_count++;
            stack.pop();
        }
        return true;
    }

    bool erase(int x) {
        std::stack<node*> stack;
        stack.push(_root);
        for (int k = 29; k >= 0; k--) {
            if (!stack.top()->_children[x >> k & 1]) return false;
            stack.push(stack.top()->_children[x >> k & 1]);
        }
        bool erased = false;
        for (int k = 0; k < 30; k++) {
            if (erased) {
                stack.top()->_children[x >> (k - 1) & 1] = nullptr;
                erased = false;
            }
            stack.top()->_count--;
            if (stack.top()->_count == 0) {
                _pool.deallocate(stack.top());
                erased = true;
            }
            stack.pop();
        }
        if (erased) _root->_children[x >> 29 & 1] = nullptr;
        return true;
    }

    int min_xor(int x) {
        assert(_root->_count > 0);
        int result = 0;
        node *curr = _root;
        for (int k = 29; k >= 0; k--) {
            if (curr->_children[x >> k & 1]) {
                curr = curr->_children[x >> k & 1];
            } else {
                result |= 1 << k;
                curr = curr->_children[~x >> k & 1];
            }
        }
        return result;
    }
};

int main() {
    int Q;
    std::cin >> Q;
    binary_trie trie;

    while (Q--) {
        int t, x;
        std::cin >> t >> x;
        if (t == 0) trie.insert(x);
        else if (t == 1) trie.erase(x);
        else std::cout << trie.min_xor(x) << std::endl;
    }
}
