#include <iostream>
#include <sstream>
#include <vector>
#include <cassert>
#include "N6.h"

void test1() {
    SquareMatrice a(2);
    SquareMatrice b = a + 0.2;
    SquareMatrice c = b * 2 + 1.2;
    SquareMatrice d = b + c;
    assert(static_cast<double>(a) == 0);
    assert(static_cast<double>(b) == 0.8);
    assert(static_cast<double>(c) == 6.4);
    assert(static_cast<double>(d) == 7.2);
}

void test2() {
    SquareMatrice a(2);
    SquareMatrice b(2);
    a += 1;
    b += a + a;

    // example where behavior of the program will be different with returning by value and by reference
    a *= b *= b;

    assert(static_cast<std::string>(a) == static_cast<std::string>("16 16 \n16 16 \n"));
    assert(static_cast<std::string>(b) == static_cast<std::string>("8 8 \n8 8 \n"));
}

void test3() {
    double ** arr = new double*[3];
    arr[0] = new double[3] {1, 0, 0};
    arr[1] = new double[3] {0, 2, 0};
    arr[2] = new double[3] {0, 0, 3};
    std::vector<double> vec{};
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    SquareMatrice a(3, arr);
    SquareMatrice b(vec);

    assert(a == b);

    delete arr[0];
    delete arr[1];
    delete arr[2];
    delete arr;
}

void test4() {
    std::vector<double> vec{};
    vec.push_back(1);
    vec.push_back(0);
    vec.push_back(2);
    SquareMatrice b(vec);
    b += 1;
    b *= b;

    assert(b[0][0] == 6);
    assert(b[1][2] == 5);
    assert(b[0][1] == 4);
    assert(b[1][1] == 3);
    assert(b[2][2] == 11);
}


int main() {
    test1();
    test2();
    test3();
    test4();
}