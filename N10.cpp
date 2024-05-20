#include <iostream>

template <typename Derived, int N = 1>
struct Counter {
private:
    static int objects_created;

public:
    Counter() {
        if (objects_created++ >= N) {
            throw "Too many classes created";
        }
    }

    Counter(const Counter&) {
        if (objects_created++ >= N) {
            throw "Too many classes created";
        }
    }
};


class Example: public Counter<Example, 5> {
public:
    Example(): Counter<Example, 5>() {}

    void print() {
        std::cout << "example.print" << std::endl;
    }
}; 

template<typename Derived, int N>
int Counter<Derived, N>::objects_created = 0;


int main() {
    try {
        for (int i = 0; i < 10; i++) {
            (new Example())->print();
        }
    } catch (const char* message) {
        std::cout << "Exception: " << message << std::endl;
    }
    return 0;
}