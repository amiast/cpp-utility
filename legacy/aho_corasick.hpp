#ifndef KOTONE_AHO_CORASICK_HPP
#define KOTONE_AHO_CORASICK_HPP 1

#include <string>
#include <vector>
#include <array>
#include <queue>
#include <memory>

namespace kotone {

struct aho_corasick {
    struct trie {
        std::array<std::shared_ptr<trie>, 26> arr;
        trie *link = nullptr;
        int mask_matched = 0;
        int index = -1;
    };

    std::shared_ptr<trie> root;
    std::vector<trie*> nodes;
    int num_strs;

    aho_corasick() : root(std::make_shared<trie>()), nodes{root.get()}, num_strs(0) {
        root->index = 0;
    }

    int insert(std::string &s) {
        trie *curr = root.get();
        for (char c : s) {
            int i = c - 'a';
            if (!curr->arr[i]) {
                curr->arr[i] = std::make_shared<trie>();
                curr->arr[i]->index = nodes.size();
                nodes.emplace_back(curr->arr[i].get());
            }
            curr = curr->arr[i].get();
        }
        curr->mask_matched |= 1 << num_strs;
        return num_strs++;
    }

    void build() {
        std::queue<trie*> queue;
        for (int i = 0; i < 26; i++) {
            if (root->arr[i]) {
                root->arr[i]->link = root.get();
                queue.emplace(root->arr[i].get());
            } else {
                root->arr[i] = root;
            }
        }

        while (queue.size()) {
            trie *curr = queue.front();
            queue.pop();

            for (int i = 0; i < 26; i++) {
                trie *child = curr->arr[i].get();
                if (!child) {
                    curr->arr[i] = curr->link->arr[i];
                    continue;
                }

                trie *fail = curr->link;
                while (fail && !fail->arr[i]) fail = fail->link;

                if (fail) child->link = fail->arr[i].get();
                else child->link = root.get();

                child->mask_matched |= child->link->mask_matched;
                queue.emplace(child);
            }
        }
    }
};

}  // namespace kotone

#endif  // KOTONE_AHO_CORASICK_HPP
