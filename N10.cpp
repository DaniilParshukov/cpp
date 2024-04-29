#include <iostream>

template <typename Derived>
struct Counter {
private:
    static int objects_created;

public:
    Counter() {
        if (objects_created++ >= 5) {
            throw "Too many classes created";
        }
    }

    Counter(const Counter&) {
        if (objects_created++ >= 5) {
            throw "Too many classes created";
        }
    }
};


class Exemple: public Counter<Exemple> {
public:
    Exemple(): Counter() {}

    void print() {
        std::cout << "exemple.print" << std::endl;
    }
};

template<>
int Counter<Exemple>::objects_created = 0;

int main() {
    try {
        for (int i = 0; i < 10; i++) {
            (new Exemple())->print();
        }
    } catch (const char* message) {
        std::cout << "Exception: " << message << std::endl;
    }
    return 0;
}