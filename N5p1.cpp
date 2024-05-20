#include <iostream>
#include <cassert>
#include <vector>

template<typename T>
class ScopedPointer {
    T* pointer;
public:
    ScopedPointer(T* raw): pointer(raw) {}

    ~ScopedPointer() { delete pointer; }

    ScopedPointer(const ScopedPointer& other) : pointer(new T(*other.pointer)) {}

    ScopedPointer(ScopedPointer&& other) {
        std::swap(pointer, other.pointer);
    }

    ScopedPointer& operator=(ScopedPointer other) {
        std::swap(pointer, other.pointer);
        return *this;
    }

    T& operator*() { return *pointer; }
    const T& operator*() const { return *pointer; }
    T* operator->() { return pointer; }
    const T* operator->() const { return pointer; }
};

void test1() {
    ScopedPointer<std::vector<int>>* sp = new ScopedPointer(new std::vector<int>{});
    (*sp)->push_back(1);
    (*sp)->push_back(2);
    assert((*sp)->at(0) == 1);
    ScopedPointer copy(*sp);
    delete sp;
    assert(copy->at(1) == 2);
}

void test2() {
    ScopedPointer sp1(new std::string("String"));
    ScopedPointer sp2(new std::string("Not String"));
    assert(*sp1 == std::string("String"));
    assert(*sp2 == std::string("Not String"));
    sp2 = sp1;
    sp2->append("!");
    assert(*sp1 == std::string("String"));
    assert(*sp2 == std::string("String!"));

}

int main() {
    test1();
    test2();
}