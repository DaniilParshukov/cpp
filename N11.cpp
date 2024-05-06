#include <iostream>

template <int N, int I = 2>
struct isPrime: std::integral_constant<bool, std::conditional_t<(N % I == 0), std::false_type, isPrime<N, I + 1>>{}> {};

template <int N>
struct isPrime <N, N>: std::true_type {};


template <int N, int NUM = 2, int P = 0>
struct NPrime: std::integral_constant<int, std::conditional_t<static_cast<bool>(isPrime<NUM>::value), NPrime<N, NUM + 1, P + 1>, NPrime<N, NUM + 1, P>>{}> {};

template <int N, int NUM>
struct NPrime <N, NUM, N>: std::integral_constant<int, NUM - 1> {};


int main() {
    //std::cout << isPrime<7>{} << std::endl;
    std::cout << NPrime<6>{} << std::endl;
}