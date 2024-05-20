#include <iostream>
#include <sstream>
#include <vector>


class Expression {
public:
    virtual Expression* diff(char var) = 0;
    virtual Expression* clone() = 0;
    virtual void print() = 0;
    virtual ~Expression() {};
};


class Val: public Expression {
    int value;

public:
    Val(int value): value(value) {};

    Val(const Val& other): value(other.value) {};

    Expression* clone() override {
        return new Val(*this);
    }

    Expression* diff(char var) {
        return new Val(0);
    }

    void print() {
        std::cout << value;
    }
};

class Var: public Expression {
    char value;

public:
    Var(char value): value(value) {};

    Var(const Var& other): value(other.value) {};

    Expression* clone() override {
        return new Var(*this);
    }

    Expression* diff(char var) {
        if (value == var) {
            return new Val(1);
        } else {
            return new Val(0);
        }
    }

    void print() {
        std::cout << value;
    }
};

class Binary: public Expression {
protected:
    Expression* left;
    Expression* right;

public:
    Binary(Expression* left, Expression* right): left(left), right(right) {};

    Binary(const Binary& other) {
        left = other.left->clone();
        right = other.right->clone();
    }

    void print(char z) {
        std::cout << "(";
        left->print();
        std::cout << z;
        right->print();
        std::cout << ")";
    }

    ~Binary() {
        delete left;
        delete right;
    }
};

class Unary: public Expression {
protected:
    Expression* content;

public:
    Unary(Expression* content): content(content) {};

    Unary(const Unary& other) {
        content = other.content->clone();
    }

    ~Unary() {
        delete content;
    }
};

class Add: public Binary {
public:
    Add(Expression* left, Expression* right): Binary(left, right) {};

    Add(const Add& other): Binary(other) {};

    Expression* clone() override {
        return new Add(*this);
    }

    Expression* diff(char var) {
        return new Add(left->diff(var), right->diff(var));
    }

    void print() {
        Binary::print('+');
    }
};

class Sub: public Binary {
public:
    Sub(Expression* left, Expression* right): Binary(left, right) {};

    Sub(const Sub& other): Binary(other) {};

    Expression* clone() override {
        return new Sub(*this);
    }

    Expression* diff(char var) {
        return new Sub(left->diff(var), right->diff(var));
    }

    void print() {
        Binary::print('-');
    }
};

class Mult: public Binary {
public:
    Mult(Expression* left, Expression* right): Binary(left, right) {};

    Mult(const Mult& other): Binary(other) {};

    Expression* clone() override {
        return new Mult(*this);
    }

    Expression* diff(char var) {
        return new Add(new Mult(left->clone(), right->diff(var)), new Mult(left->diff(var), right->clone()));
    }

    void print() {
        Binary::print('*');
    }
};

class Div: public Binary {
public:
    Div(Expression* left, Expression* right): Binary(left, right) {};

    Div(const Div& other): Binary(other) {};

    Expression* clone() override {
        return new Div(*this);
    }

    Expression* diff(char var) {
        return new Div(new Sub(new Mult(left->diff(var), right->clone()), new Mult(left->clone(), right->diff(var))), new Mult(right->clone(), right->clone()));
    }

    void print() {
        Binary::print('/');
    }
};

class Exponent: public Unary {
public:
    Exponent(Expression* content): Unary(content) {};

    Exponent(const Exponent& other): Unary(other) {};

    Expression* clone() override {
        return new Exponent(*this);
    }

    Expression* diff(char var) {
        return new Mult(content->diff(var), new Exponent(content->clone()));
    }

    void print() {
        std::cout << "e(";
        content->print();
        std::cout << ")";
    }
};


int main() {
    Expression* e = new Add(new Mult(new Var('x'), new Exponent(new Var('x'))), new Sub(new Div(new Val(10), new Var('y')), new Mult(new Val(10), new Var('z'))));

    e->print();
    std::cout << "\n";

    Expression* res1 = e->diff('x');
    Expression* res2 = e->diff('y');
    Expression* res3 = e->diff('z');
    Expression* res4 = e->diff('q');

    delete e;

    res1->print();
    std::cout << "\n";
    res2->print();
    std::cout << "\n";
    res3->print();
    std::cout << "\n";
    res4->print();
    std::cout << "\n" << std::endl;

    delete res1;
    delete res2;
    delete res3;
    delete res4;
}