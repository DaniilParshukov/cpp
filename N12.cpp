#include <iostream>
#include <cassert>
#include <unordered_map>
#include "N6.h"

template<typename T>
class Node {
    T val_;
    int c_;
    T sum_;
    int pri_;
    Node* left_;
    Node* right_;
    Node* parent_;

public:
    class iterator {
    private:
        Node<T>* current_;
    public:
        iterator(Node<T>* root) {
            current_ = root;
            if (current_) {
                while (current_->left_) {
                    current_ = current_->left_;
                }
            }
        }

        T& operator*() {
            return current_->val_;
        }

        iterator& operator++() {
            if (current_) {
                if (current_->right_) {
                    current_ = current_->right_;
                    while (!current_->left_) {
                        current_ = current_->left_;
                    }
                } else {
                    if (current_->parent_ != nullptr) {
                        current_ = current_->parent_;
                    } else {
                        current_ = nullptr;
                    }
                }
            }
            return *this;
        }

        bool operator==(const iterator& other) {
            return current_ == other.current_;
        }

        bool operator!=(const iterator& other) {
            return current_ != other.current_;
        }
    };

    Node(T val, int pri): val_(val), c_(1), sum_(val), pri_(pri), left_(nullptr), right_(nullptr), parent_(nullptr) {}

    Node(const Node& other): val_(other.val_), c_(other.c_), sum_(other.sum_), pri_(other.pri_) {
        if (other.left_) {
            left_ = new Node(*(other.left_));
            left_->parent_ = this;
        } else {
            left_ = nullptr;
        }
        if (other.right_) {
            right_ = new Node(*(other.right_));
            right_->parent_ = this;
        } else {
            right_ = nullptr;
        }
    }

    Node(Node&& other): val_(other.val_), c_(other.c_), sum_(other.sum_), pri_(other.pri_), left_(other.left_), right_(other.right_), parent_(other.parent_) {
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
        std::swap(parent_, other.parent_);
        return *this;
    }

    ~Node() {
        delete right_;
        delete left_;
    }


    Node* insert(const T& val, int pri, int pos) {
        if (pos > c_ or pos < 0) {
            pos = c_;
        }
        std::tuple<Node*, Node*> tt = splitBySize(pos);
        Node* t2 = new Node(val, pri);
        return merge(merge(std::get<0>(tt), t2), std::get<1>(tt));
    }

    Node* insert(const T& val, int pri) {
        int pos = c_;
        std::tuple<Node*, Node*> tt = splitBySize(pos);
        Node* t2 = new Node(val, pri);
        return merge(merge(std::get<0>(tt), t2), std::get<1>(tt));
    }

    T get() {
        return val_;
    }

    T get(int pos) {
        int lc = (left_ ? left_->c_ : 0);
        if (pos == lc) {
            return val_;
        }
        if (pos < lc) {
            return left_->get(pos);
        } else  {
            return right_->get(pos - lc - 1);
        }
    }

    Node* erase(int pos, int count = 1) {
        std::tuple<Node*, Node*> t = splitBySize(pos);
        if (std::get<1>(t)) {
            std::tuple<Node*, Node*> tr = std::get<1>(t)->splitBySize( count);
            return merge(std::get<0>(t), std::get<1>(tr));
        }
        return std::get<0>(t);
    }

    T sumInterval(int f, int t) {
        std::tuple<Node*, Node*> tn = splitBySize(f);
        if (std::get<1>(tn)) {
            std::tuple<Node*, Node*> tnr = std::get<1>(tn)->splitBySize(t - f + 1);
            return std::get<0>(tnr)->sum_;
        }
        return 0;
    }

    int size(){
        return c_;
    }

private:
    void update() {
        c_ = 1 + (right_ ? right_->c_ : 0) + (left_ ? left_->c_ : 0);
        if (right_) {
            if (left_) {
                sum_ = left_->sum_ + val_ + right_->sum_;
            } else {
                sum_ = val_ + right_->sum_;
            }
        } else {
            if (left_) {
                sum_ = left_->sum_ + val_;
            } else {
                sum_ = val_;
            }
        }
        if (left_) {
            left_->parent_ = this;
        }
        if (right_) {
            right_->parent_ = this;
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
        int lc = (left_ ? left_->c_ : 0);
        if (k <= lc) {
            if (left_) {
                std::tuple<Node<T>*, Node<T>*> tl = left_->splitBySize(k);
                left_ = std::get<1>(tl);
                update();
                return std::tuple<Node<T>*, Node<T>*> {std::get<0>(tl), this};
            } else {
                return std::tuple<Node<T>*, Node<T>*> {nullptr, this};
            }
        } else {
            if (right_) {
                std::tuple<Node<T>*, Node<T>*> tr = right_->splitBySize(k - lc - 1);
                right_ = std::get<0>(tr);
                update();
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
    std::unordered_map<int, bool> priMap_;

public:
    Treap(): root_(nullptr) {}

    Treap(const Treap& other) {
        root_ = new Node(*other.root_);
        priMap_ = other.priMap_;
    }

    Treap(Treap&& other): root_(other.root_), priMap_(other.priMap_) {
        other.root_ = nullptr;
    }

    Treap& operator=(Treap other) {
        if (root_) {
            delete root_;
        }
        std::swap(root_, other.root_);
        std::swap(priMap_, other.priMap_);
        return *this;
    }

    ~Treap() { delete root_; }

    typename Node<H>::iterator begin() {
        return typename Node<H>::iterator(root_);
    }

    typename Node<H>::iterator end() {
        return typename Node<H>::iterator(nullptr);
    }
    
    void insert(const H& val) {
        int pri = -1;
        while(pri != -1) {
            int r = rand() % 100000;
            if (priMap_.find(r) == priMap_.end()) {
                priMap_[r] = true;
                pri = r;
            }
        }
        if (root_) {
            root_ = root_->insert(val, pri);
        } else {
            root_ = new Node(val, pri);
        }
    }

    void insert(const H& val, int pos) {
        int pri = -1;
        while(pri != -1) {
            int r = rand() % 100000;
            if (priMap_.find(r) == priMap_.end()) {
                priMap_[r] = true;
                pri = r;
            }
        }
        if (root_) {
            root_ = root_->insert(val, pri, pos);
        } else {
            root_ = new Node(val, pri);
        }
    }

    void erase(int pos) {
        if (root_) {
            root_ = root_->erase(pos);
        }
    }

    void erase(int pos, int count) {
        if (root_) {
            root_ = root_->erase(pos, count);
        }
    }

    H sum(int f, int t) {
        if (root_) {
            return root_->sumInterval(f, t);
        } else {
            return 0;
        }
    }

    H operator[](int i) {
        return root_->get(i);
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
    treap.erase(1, 4);
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

void testWithSquareMatrice() {
    Treap<SquareMatrice> treap;
    SquareMatrice a(3);
    treap.insert(a);
    treap.insert(a + 1);
    treap.insert(a + 2);
    assert(3 == treap.sum(0, 2)[1][1]);
}

void testWithBool() {
    Treap<bool> treap;
    treap.insert(true);
    treap.insert(true);
    treap.insert(false);
    assert(true == treap.sum(0, 3));
}

void testCopyOperator() {
    Node<int>* node = new Node(0, rand() % 100000);
    Node copy(0, rand() % 100000);
    node = node->insert(1, 1, rand() % 100000);
    node = node->insert(2, 2, rand() % 100000);
    copy = *node;
    delete node;
    assert(3 == copy.sumInterval(0, 2));
}

void testCopyCrt() {
    Node<int>* node = new Node(0, rand() % 100000);
    node = node->insert(1, 1, rand() % 100000);
    node = node->insert(2, 2, rand() % 100000);
    Node copy = *node;
    delete node;
    assert(3 == copy.sumInterval(0, 2));
}

void testMoveCrt() {
    Node<int>* node = new Node(0, rand() % 100000);
    node = node->insert(1, 1, rand() % 100000);
    node = node->insert(2, 2, rand() % 100000);
    Node copy = std::move(*node);
    delete node;
    assert(3 == copy.sumInterval(0, 2));
}

void testMoveOperator() {
    Node<int>* node = new Node(0, rand() % 100000);
    Node copy(0, rand() % 100000);
    node = node->insert(1, 1, rand() % 100000);
    node = node->insert(2, 2, rand() % 100000);
    copy = std::move(*node);
    delete node;
    assert(3 == copy.sumInterval(0, 2));
}

void testOperatorParenthesis() {
    Treap<int> treap;
    treap.insert(0);
    treap.insert(1);
    treap.insert(2);
    treap.insert(3, 1);
    assert(treap[1] == 3);
}

void testIterator1() {
    Treap<int> treap;
    treap.insert(0);
    treap.insert(1);
    treap.insert(2);
    treap.insert(3);

    int sum = 0;
    Node<int>::iterator it = treap.begin();
    Node<int>::iterator end = treap.end();

    for (; it != end; ++it) {
        sum += *it;
    }
    assert(sum == treap.sum(0, 3));
}

void testIterator2() {
    Treap<std::string> treap;
    treap.insert(std::string("apple"));
    treap.insert(std::string("orange"));
    treap.insert(std::string("tomato"));
    treap.insert(std::string("cucumber"));
    treap.insert(std::string("potato"));
    int caunt = 0;
    for (auto& str : treap) {
        if (str.size() == 6) {
            caunt++;
        }
    }
    assert(caunt == 3);
}

void testTreapCopyCrt() {
    Treap<int>* treap = new Treap<int>();
    treap->insert(0, 0);
    treap->insert(1, 1);
    Treap<int> copy = *treap;
    delete treap;
    assert(1 == copy.sum(0, 1));
}

void testTreapCopyOperator() {
    Treap<int>* treap = new Treap<int>();
    Treap<int> copy{};
    treap->insert(0, 0);
    treap->insert(1, 1);
    copy = *treap;
    delete treap;
    assert(1 == copy.sum(0, 1));
}

void testTreapMoveCrt() {
    Treap<int>* treap = new Treap<int>();
    treap->insert(0, 0);
    treap->insert(1, 1);
    Treap<int> copy = std::move(*treap);
    delete treap;
    assert(1 == copy.sum(0, 1));
}

void testTreapMoveOperator() {
    Treap<int>* treap = new Treap<int>();
    Treap<int> copy{};
    treap->insert(0, 0);
    treap->insert(1, 1);
    copy = std::move(*treap);
    delete treap;
    assert(1 == copy.sum(0, 1));
}


int main() {
    test1();
    test2();
    test3();
    test4();
    test5();

    testWithStr();
    testWithBool();
    
    testTreapCopyOperator();
    testTreapCopyCrt();
    testTreapMoveOperator();
    testTreapMoveCrt();

    testCopyOperator();
    testCopyCrt();
    testMoveCrt();
    testMoveOperator();

    testOperatorParenthesis();

    testIterator1();
    testIterator2();
}