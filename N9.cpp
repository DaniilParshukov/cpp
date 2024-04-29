#include <iostream>
#include <cassert>

template<typename T>
class Node {
private:
    T val_;
    int c_;
    T sum_;
    int pri_;
    Node* left_;
    Node* right_;

public:
    Node(T v): val_(v), c_(1), sum_(v), pri_(rand() % 100000), left_(nullptr), right_(nullptr) {}

    Node(const Node& other): val_(other.val_), c_(other.c_), sum_(other.sum_), pri_(other.pri_) {
        if (other.left_) {
            left_ = new Node(*(other.left_));
        } else {
            left_ = nullptr;
        }
        if (other.right_) {
            right_ = new Node(*(other.right_));
        } else {
            right_ = nullptr;
        }
    }

    Node(Node&& other): val_(other.val_), c_(other.c_), sum_(other.sum_), pri_(other.pri_), left_(other.left_), right_(other.right_) {
        other.left_ = nullptr;
        other.right_ = nullptr;
    }

    Node& operator=(Node other) {
        std::swap(val_, other.val_);
        std::swap(c_, other.c_);
        std::swap(sum_, other.sum_);
        std::swap(pri_, other.pri_);
        std::swap(left_, other.left_);
        std::swap(right_, other.right_);
        return *this;
    }

    ~Node() {
        delete right_;
        delete left_;
    }


    Node* insert(const T& val_, int pos) {
        if (pos > this->c_ or pos < 0) {
            pos = this->c_;
        }
        std::tuple<Node*, Node*> tt = this->splitBySize(pos);
        Node* t2 = new Node(val_);
        return merge(merge(std::get<0>(tt), t2), std::get<1>(tt));
    }

    Node* insert(const T& val_) {
        int pos = this->c_;
        std::tuple<Node*, Node*> tt = this->splitBySize(pos);
        Node* t2 = new Node(val_);
        return merge(merge(std::get<0>(tt), t2), std::get<1>(tt));
    }

    Node* get(int pos) {
        int lc = (this->left_ ? this->left_->c_ : 0);
        if (pos == lc) {
            return this;
        }
        if (pos < lc) {
            return this->left_->get(pos);
        } else  {
            return this->right_->get(pos);
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

    T sumInterval(int f, int t) {
        std::tuple<Node*, Node*> tn = this->splitBySize(f);
        if (std::get<1>(tn)) {
            std::tuple<Node*, Node*> tnr = std::get<1>(tn)->splitBySize(t - f + 1);
            return std::get<0>(tnr)->sum_;
        }
        return 0;
    }

private:
    void update() {
        this->c_ = 1 + (this->right_ ? this->right_->c_ : 0) + (this->left_ ? this->left_->c_ : 0);
        if (this->right_) {
            if (this->left_) {
                this->sum_ = this->val_ + this->right_->sum_ + this->left_->sum_;
            } else {
                this->sum_ = this->val_ + this->right_->sum_;
            }
        } else {
            if (this->left_) {
                this->sum_ = this->val_ + this->left_->sum_;
            } else {
                this->sum_ = this->val_;
            }
        }
    }

    Node<T>* merge(Node<T>* t1, Node<T>* t2) {
        if (!t1) { return t2; }
        if (!t2) { return t1; }
        if (t1->pri_ < t2->pri_) {
            t1->right_ = merge(t1->right_, t2);
            t1->update();
            return t1;
        } else {
            t2->left_ = merge(t1, t2->left_);
            t2->update();
            return t2;
        }
    }
   
    std::tuple<Node<T>*, Node<T>*> splitBySize(int k) {
        int lc = (this->left_ ? this->left_->c_ : 0);
        if (k <= lc) {
            if (this->left_) {
                std::tuple<Node<T>*, Node<T>*> tl = this->left_->splitBySize(k);
                this->left_ = std::get<1>(tl);
                this->update();
                return std::tuple<Node<T>*, Node<T>*> {std::get<0>(tl), this};
            } else {
                return std::tuple<Node<T>*, Node<T>*> {nullptr, this};
            }
        } else {
            if (this->right_) {
                std::tuple<Node<T>*, Node<T>*> tr = this->right_->splitBySize(k - lc - 1);
                this->right_ = std::get<0>(tr);
                this->update();
                return std::tuple<Node<T>*, Node<T>*> {this, std::get<1>(tr)};
            } else {
                return std::tuple<Node<T>*, Node<T>*> {this, nullptr};
            }
        }
    }
};


template<typename H>
class Treap {
private:
    Node<H>* root_;

public:
    Treap(): root_(nullptr) {}

    Treap(const Treap& other) {
        *root_ = *other.root_;
    }

    Treap(Treap&& other): root_(other.root_) {
        other.root_ = nullptr;
    }

    Treap& operator=(Treap other) {
        std::swap(root_, other.root_);
        return *this;
    }

    ~Treap() { delete root_; }
    
    void insert(const H& val, int pos) {
        if (root_) {
            root_ = root_->insert(val, pos);
        } else {
            root_ = new Node(val);
        }
    }

    void insert(const H& val) {
        if (root_) {
            root_ = root_->insert(val);
        } else {
            root_ = new Node(val);
        }
    }

    void erase(int pos) {
        if (root_) {
            root_ = root_->erase(pos);
        }
    }

    void eraseRegion(int pos, int count) {
        if (root_) {
            root_ = root_->eraseRegion(pos, count);
        }
    }

    H sum(int f, int t) {
        if (root_) {
            return root_->sumInterval(f, t);
        } else {
            return 0;
        }
    }
};

void test1() {
    Treap<int> treap;
    treap.insert(1, 0);
    treap.insert(0, 0);
    treap.insert(2, 2);
    assert(1 == treap.sum(0, 1));
}

void test2() {
    Treap<int> treap;
    treap.insert(0, 0);
    treap.insert(1, 1);
    treap.insert(2, 2);
    treap.insert(3, 3);
    treap.insert(4, 4);
    treap.insert(5, 5);
    treap.insert(6, 6);
    assert(5 == treap.sum(2, 3));
}

void test3() {
    Treap<int> treap;
    treap.insert(0, 0);
    treap.insert(1, 1);
    treap.insert(2, 2);
    treap.insert(3, 3);
    treap.insert(4, 4);
    treap.insert(5, 5);
    treap.insert(6, 6);
    treap.erase(2);
    treap.erase(2);
    assert(4 == treap.sum(2, 2));
}

void test4() {
    Treap<int> treap;
    treap.insert(0);
    treap.insert(1);
    treap.insert(2);
    treap.insert(3);
    treap.insert(4);
    treap.insert(5);
    treap.eraseRegion(1, 4);
    assert(5 == treap.sum(0, 1));
}

void test5() {
    Treap<int> treap;
    treap.insert(0);
    treap.insert(1);
    treap.insert(6, 2);
    treap.insert(5, 2);
    treap.insert(4, 2);
    treap.insert(3, 2);
    treap.insert(2, 2);
    treap.erase(1);
    assert(20 == treap.sum(0, 5));
}

void testWithStr() {
    Treap<std::string> treap;
    treap.insert(std::string("test "));
    treap.insert(std::string("doesn't "));
    treap.insert(std::string("passed"));
    treap.erase(1);
    assert("test passed" == treap.sum(0, 5));
}

void testWithBool() {
    Treap<bool> treap;
    treap.insert(true);
    treap.insert(true);
    treap.insert(false);
    assert(true == treap.sum(0, 3));
}

void testCopyOperator() {
    Node<int>* node = new Node(0);
    Node<int>* copy = new Node(0);
    node = node->insert(1, 1);
    node = node->insert(2, 2);
    *copy = *node;
    node = node->erase(1);
    assert(3 == copy->sumInterval(0, 2));
}

void testCopyConstructors() {
    Node<int>* node = new Node(0);
    node = node->insert(1, 1);
    node = node->insert(2, 2);
    Node<int> copy = *node;
    node = node->erase(1);
    assert(3 == copy.sumInterval(0, 2));
}

void testMoveConstructors() {
    Node<int>* node = new Node(0);
    node = node->insert(1, 1);
    node = node->insert(2, 2);
    Node<int> copy = std::move(*node);
    node = node->erase(1);
    assert(3 == copy.sumInterval(0, 2));
}

void testMoveOperator() {
    Node<int>* node = new Node(0);
    Node<int>* copy = new Node(0);
    node = node->insert(1, 1);
    node = node->insert(2, 2);
    *copy = std::move(*node);
    node = node->erase(1);
    assert(3 == copy->sumInterval(0, 2));
}


int main() {
    test1();
    test2();
    test3();
    test4();
    test5();

    testWithStr();
    testWithBool();

    testCopyOperator();
    testCopyConstructors();
    testMoveConstructors();
    testMoveOperator();
}