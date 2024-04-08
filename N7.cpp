#include <iostream>
#include <sstream>
#include <vector>


class Expression {
public:
    virtual Expression* diff(char var) = 0;
    virtual void print() = 0;
    virtual ~Expression() {};
};


class Val: public Expression {
    int value;

public:
    Val(int value): value(value) {};

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

    ~Unary() {
        delete content;
    }
};

class Add: public Binary {
public:
    Add(Expression* left, Expression* right): Binary(left, right) {};

    Expression* diff(char var) {
        return new Add(left->diff(var), right->diff(var));
    }

    void print() {
        std::cout << "(";
        left->print();
        std::cout << "+";
        right->print();
        std::cout << ")";
    }
};

class Sub: public Binary {
public:
    Sub(Expression* left, Expression* right): Binary(left, right) {};

    Expression* diff(char var) {
        return new Sub(left->diff(var), right->diff(var));
    }

    void print() {
        std::cout << "(";
        left->print();
        std::cout << "-";
        right->print();
        std::cout << ")";
    }
};

class Mult: public Binary {
public:
    Mult(Expression* left, Expression* right): Binary(left, right) {};

    Expression* diff(char var) {
        return new Add(new Mult(left, right->diff(var)), new Mult(left->diff(var), right));
    }

    void print() {
        left->print();
        std::cout << "*";
        right->print();
    }
};

class Div: public Binary {
public:
    Div(Expression* left, Expression* right): Binary(left, right) {};

    Expression* diff(char var) {
        return new Div(new Sub(new Mult(left->diff(var), right), new Mult(left, right->diff(var))), new Mult(right, right));
    }

    void print() {
        left->print();
        std::cout << "/";
        std::cout << "(";
        right->print();
        std::cout << ")";
    }
};

class Exponent: public Unary {
public:
    Exponent(Expression* content): Unary(content) {};

    Expression* diff(char var) {
        return new Mult(content->diff(var), new Exponent(content));
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

    res1->print();
    std::cout << "\n";
    res2->print();
    std::cout << "\n";
    res3->print();
    std::cout << "\n";
    res4->print();
}