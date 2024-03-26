#include <iostream>
#include <cassert>

class Node {
    int val;
    int c;
    int sum;
    int pri;
    Node* left;
    Node* right;

public:
    Node(int v): val(v), c(1), sum(v), pri(rand() % 100000), left(nullptr), right(nullptr) {}

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

    Node* insert(int val, int pos) {
        if (pos > this->c or pos < 0) {
            pos = this->c;
        }
        std::tuple<Node*, Node*> tt = this->splitBySize(pos);
        Node* t2 = new Node(val);
        return merge(merge(std::get<0>(tt), t2), std::get<1>(tt));
    }

    Node* get(int pos) {
        int lc = (this->left ? this->left->c : 0);
        if (pos == lc) {
            return this;
        }
        if (pos < lc) {
            return this->left->get(pos);
        } else  {
            return this->right->get(pos);
        }
    }

    Node* erase(int pos) {
        std::tuple<Node*, Node*> t = this->splitBySize(pos);
        if (std::get<1>(t)) {
            std::tuple<Node*, Node*> tr = std::get<1>(t)->splitBySize(1);
            return merge(std::get<0>(t), std::get<1>(tr));
        }
        return std::get<0>(t);
    }

    Node* eraseRegion(int pos, int count) {
        std::tuple<Node*, Node*> t = this->splitBySize(pos);
        if (std::get<1>(t)) {
            std::tuple<Node*, Node*> tr = std::get<1>(t)->splitBySize(count);
            return merge(std::get<0>(t), std::get<1>(tr));
        }
        return std::get<0>(t);
    }

    int sumInterval(int f, int t) {
        std::tuple<Node*, Node*> tn = this->splitBySize(f);
        if (std::get<1>(tn)) {
            std::tuple<Node*, Node*> tnr = std::get<1>(tn)->splitBySize(t - f + 1);
            return std::get<0>(tnr)->sum;
        }
        return 0;
    }

private:
    void update() {
        this->c = 1 + (this->right ? this->right->c : 0) + (this->left ? this->left->c : 0);
        this->sum = this->val + (this->right ? this->right->sum : 0) + (this->left ? this->left->sum : 0);
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
        int lc = (this->left ? this->left->c : 0);
        if (k <= lc) {
            if (this->left) {
                std::tuple<Node*, Node*> tl = this->left->splitBySize(k);
                this->left = std::get<1>(tl);
                this->update();
                return std::tuple<Node*, Node*> {std::get<0>(tl), this};
            } else {
                return std::tuple<Node*, Node*> {nullptr, this};
            }
        } else {
            if (this->right) {
                std::tuple<Node*, Node*> tr = this->right->splitBySize(k - lc - 1);
                this->right = std::get<0>(tr);
                this->update();
                return std::tuple<Node*, Node*> {this, std::get<1>(tr)};
            } else {
                return std::tuple<Node*, Node*> {this, nullptr};
            }
        }
    }
};


class Treap {
    Node* root;

public:
    Treap(): root(nullptr) {}

    Treap(const Treap& other): root(other.root) {}

    Treap& operator=(const Treap& other) {
        root = other.root;
        return *this;
    }

    ~Treap() { delete root; }
    
    void insert(int val, int pos) {
        if (root) {
            root = root->insert(val, pos);
        } else {
            root = new Node(val);
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

void testCopyOperator() {
    Node* node = new Node(0);
    Node* copy = new Node(0);
    node = node->insert(1, 1);
    node = node->insert(2, 2);
    *copy = *node;
    node = node->erase(1);
    assert(3 == copy->sumInterval(0, 2));
}

void testCopyConstructors() {
    Node* node = new Node(0);
    node = node->insert(1, 1);
    node = node->insert(2, 2);
    Node copy = *node;
    node = node->erase(1);
    assert(3 == copy.sumInterval(0, 2));
}

int main() {
    test1();
    test2();
    test3();
    test4();
    test5();
    testCopyOperator();
    testCopyConstructors();
}