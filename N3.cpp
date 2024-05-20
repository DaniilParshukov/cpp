#include <iostream>
#include <cassert>
#include <unordered_map>

class Node {
    int val;
    int c;
    int sum;
    int pri;
    Node* left;
    Node* right;

public:
    Node(int v, int p): val(v), c(1), sum(v), pri(p), left(nullptr), right(nullptr) {}

    Node(const Node& other): val(other.val), c(other.c), sum(other.sum), pri(other.pri) {
        if (other.left) {
            left = new Node(*(other.left));
        } else {
            left = nullptr;
        }
        if (other.right) {
            right = new Node(*(other.right));
        } else {
            right = nullptr;
        }
    }

    Node& operator=(const Node& other) {
        val = other.val;
        c = other.c;
        sum = other.sum;
        pri = other.pri;
        if (other.left) {
            left = new Node(*(other.left));
        } else {
            left = nullptr;
        }
        if (other.left) {
            right = new Node(*(other.right));
        } else {
            right = nullptr;
        }
        return *this;
    }

    ~Node() {
        delete right;
        delete left;
    }

    Node* insert(int val, int pri, int pos) {
        if (pos > c or pos < 0) {
            pos = c;
        }
        std::tuple<Node*, Node*> tt = splitBySize(pos);
        Node* t2 = new Node(val, pri);
        return merge(merge(std::get<0>(tt), t2), std::get<1>(tt));
    }

    Node* get(int pos) {
        int lc = (left ? left->c : 0);
        if (pos == lc) {
            return this;
        }
        if (pos < lc) {
            return left->get(pos);
        } else  {
            return right->get(pos);
        }
    }

    Node* erase(int pos) {
        std::tuple<Node*, Node*> t = splitBySize(pos);
        if (std::get<1>(t)) {
            std::tuple<Node*, Node*> tr = std::get<1>(t)->splitBySize(1);
            return merge(std::get<0>(t), std::get<1>(tr));
        }
        return std::get<0>(t);
    }

    Node* eraseRegion(int pos, int count) {
        std::tuple<Node*, Node*> t = splitBySize(pos);
        if (std::get<1>(t)) {
            std::tuple<Node*, Node*> tr = std::get<1>(t)->splitBySize(count);
            return merge(std::get<0>(t), std::get<1>(tr));
        }
        return std::get<0>(t);
    }

    int sumInterval(int f, int t) {
        std::tuple<Node*, Node*> tn = splitBySize(f);
        if (std::get<1>(tn)) {
            std::tuple<Node*, Node*> tnr = std::get<1>(tn)->splitBySize(t - f + 1);
            return std::get<0>(tnr)->sum;
        }
        return 0;
    }

private:
    void update() {
        c = 1 + (right ? right->c : 0) + (left ? left->c : 0);
        sum = val + (right ? right->sum : 0) + (left ? left->sum : 0);
    }

    Node* merge(Node* t1, Node* t2) {
        if (!t1) { return t2; }
        if (!t2) { return t1; }
        if (t1->pri < t2->pri) {
            t1->right = merge(t1->right, t2);
            t1->update();
            return t1;
        } else {
            t2->left = merge(t1, t2->left);
            t2->update();
            return t2;
        }
    }
   
    std::tuple<Node*, Node*> splitBySize(int k) {
        int lc = (left ? left->c : 0);
        if (k <= lc) {
            if (left) {
                std::tuple<Node*, Node*> tl = left->splitBySize(k);
                left = std::get<1>(tl);
                update();
                return std::tuple<Node*, Node*> {std::get<0>(tl), this};
            } else {
                return std::tuple<Node*, Node*> {nullptr, this};
            }
        } else {
            if (right) {
                std::tuple<Node*, Node*> tr = right->splitBySize(k - lc - 1);
                right = std::get<0>(tr);
                update();
                return std::tuple<Node*, Node*> {this, std::get<1>(tr)};
            } else {
                return std::tuple<Node*, Node*> {this, nullptr};
            }
        }
    }
};


class Treap {
    Node* root;
    std::unordered_map<int, bool> priMap;

public:
    Treap(): root(nullptr) {}

    Treap(const Treap& other) {
        root = new Node(*other.root);
        priMap = other.priMap;
    }

    Treap& operator=(const Treap& other) {
        if (root) {
            delete root;
        }
        root = new Node(*other.root);
        priMap = other.priMap;
        return *this;
    }

    ~Treap() { delete root; }
    
    void insert(int val, int pos) {
        int pri = -1;
        while(pri == -1) {
            int r = rand() % 100000;
            if (priMap.find(r) == priMap.end()) {
                priMap[r] = true;
                pri = r;
            }
        }
        if (root) {
            root = root->insert(val, pri, pos);
        } else {
            root = new Node(val, pri);
        }
    }

    void erase(int pos) {
        if (root) {
            root = root->erase(pos);
        }
    }

    void eraseRegion(int pos, int count) {
        if (root) {
            root = root->eraseRegion(pos, count);
        }
    }

    int sum(int f, int t) {
        if (root) {
            return root->sumInterval(f, t);
        } else {
            return 0;
        }
    }
};

void test1() {
    Treap* treap = new Treap();
    treap->insert(1, 0);
    treap->insert(0, 0);
    treap->insert(2, 2);
    assert(1 == treap->sum(0, 1));
}

void test2() {
    Treap* treap = new Treap();
    treap->insert(0, 0);
    treap->insert(1, 1);
    treap->insert(2, 2);
    treap->insert(3, 3);
    treap->insert(4, 4);
    treap->insert(5, 5);
    treap->insert(6, 6);
    assert(5 == treap->sum(2, 3));
}

void test3() {
    Treap* treap = new Treap();
    treap->insert(0, 0);
    treap->insert(1, 1);
    treap->insert(2, 2);
    treap->insert(3, 3);
    treap->insert(4, 4);
    treap->insert(5, 5);
    treap->insert(6, 6);
    treap->erase(2);
    treap->erase(2);
    assert(4 == treap->sum(2, 2));
}

void test4() {
    Treap* treap = new Treap();
    treap->insert(0, 0);
    treap->insert(1, 1);
    treap->insert(2, 2);
    treap->insert(3, 3);
    treap->insert(4, 4);
    treap->insert(5, 5);
    treap->eraseRegion(1, 4);
    assert(5 == treap->sum(0, 1));
}

void test5() {
    Treap* treap = new Treap();
    treap->insert(0, 0);
    treap->insert(1, 1);
    treap->insert(6, 2);
    treap->insert(5, 2);
    treap->insert(4, 2);
    treap->insert(3, 2);
    treap->insert(2, 2);
    treap->erase(1);
    assert(20 == treap->sum(0, 5));
}

void testTreapCopyCrt() {
    Treap* treap = new Treap();
    treap->insert(0, 0);
    treap->insert(1, 1);
    Treap copy = *treap;
    delete treap;
    assert(1 == copy.sum(0, 1));
}

void testTreapCopyOperator() {
    Treap* treap = new Treap();
    Treap copy{};
    treap->insert(0, 0);
    treap->insert(1, 1);
    copy = *treap;
    delete treap;
    assert(1 == copy.sum(0, 1));
}

void testCopyOperator() {
    Node* node = new Node(0, rand() % 100000);
    Node copy(0, rand() % 100000);
    node = node->insert(1, 1, rand() % 100000);
    node = node->insert(2, 2, rand() % 100000);
    copy = *node;
    delete node;
    assert(3 == copy.sumInterval(0, 2));
}

void testCopyCrt() {
    Node* node = new Node(0, rand() % 100000);
    node = node->insert(1, 1, rand() % 100000);
    node = node->insert(2, 2, rand() % 100000);
    Node copy = *node;
    delete node;
    assert(3 == copy.sumInterval(0, 2));
}

int main() {
    test1();
    test2();
    test3();
    test4();
    test5();

    testTreapCopyCrt();
    testTreapCopyOperator();

    testCopyOperator();
    testCopyCrt();
}