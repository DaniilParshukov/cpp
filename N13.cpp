#include <iostream>
#include <utility>
#include <cassert>

template <typename Checker, typename... Args>
int getIndexOfFirstMatch(Checker check, Args&&... args) {
    int index = 0;
    bool found = ((check(std::forward<Args>(args)) ? true : (++index, false)) || ...);
    return found ? index : -1;
}

void test1() {
    assert(getIndexOfFirstMatch([](int x) { return x > 5; }, 1, 2, 3, 5, 7, 11) == 4);
}

void test2() {
    assert(getIndexOfFirstMatch([](const std::string& s) { return s == "target"; }, std::string("hello"), std::string("world"), std::string("target")) == 2);
}

void test3() {
    assert(getIndexOfFirstMatch([](int x) { return x == 0; }, 1, 2, 3, 4, 5) == -1);
}

int main() {
    test1();
    test2();
    test3();
}
