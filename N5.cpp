#include <iostream>

template<typename T>
class ScopedPointer {
    T* pointer;
public:
    ScopedPointer(T* raw): pointer(raw) {}
    ~ScopedPointer() { delete pointer; }
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

int main() {
}